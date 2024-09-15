#include "cppmagic/MagicModeBase.h"

#include "cppmagic/TagUtility.h"

////////////////////////////////////////////////////////////////////////

cmg::MagicModeBase::MagicModeBase(const cmg::Settings& settings, const cmgSP<cmg::TagManager>& tagManager)
    : mTagManager(tagManager), mSettings(settings)
{
    for (auto sortingMethod : cmg::sortingMethods())
    {
        auto comparator = cmg::getComparator(cmg::sortingStrToEnum(sortingMethod));
        mComparatorMap[sortingMethod] = comparator;
    }
    mActiveComparator = cmg::getComparator(mSettings.mSortingMethod);
}

////////////////////////////////////////////////////////////////////////

void cmg::MagicModeBase::tagManager(const cmgSP<cmg::TagManager>& tagManager)
{
    mTagManager = tagManager;
}

////////////////////////////////////////////////////////////////////////

bool cmg::MagicModeBase::run(const cmgVector<std::pair<FilePath, FilePath>>& filePairs)
{

    if (mTagManager == nullptr)
    {
        LOG_ERROR("Tag Manager is not set. Please use the 'tagManager' function to set the tag manager.");
        return false;
    }
    for (auto& [headerFile, sourceFile] : filePairs)
    {
        const auto& tagPairs = mTagManager->getTagPairs(headerFile.relativePathStem());

        preProcessTags(tagPairs);
        auto protoImplPairs = setupProtoImplPairs(tagPairs);

        cmgVector<std::string> nonTargetLines;
        runImpl(nonTargetLines, protoImplPairs, headerFile, sourceFile);
        writeCode(nonTargetLines, protoImplPairs, headerFile, sourceFile);
    }
    return true;
}

////////////////////////////////////////////////////////////////////////

void cmg::MagicModeBase::registerComparator(const std::string& name, MagicComparator& comparator)
{
    mComparatorMap.try_emplace(name, comparator);
}

////////////////////////////////////////////////////////////////////////

void cmg::MagicModeBase::preProcessTags(const std::pair<cmgSPVector<cmg::TagInfo>, cmgSPVector<cmg::TagInfo>>& tagPairs)
{
}

////////////////////////////////////////////////////////////////////////

void cmg::MagicModeBase::fillEmptyImpl(cmgVector<cmg::ProtoImplTagPair>& protoImplPairs)
{
    for (int i = 0; i < protoImplPairs.size();)
    {
        const auto& prototype = protoImplPairs[i].mPrototype;
        auto& impl = protoImplPairs[i].mImplementation;

        if (prototype == nullptr)
            protoImplPairs.erase(protoImplPairs.begin() + i);
        else
        {
            if (impl == nullptr)
            {
                auto implTag = std::make_shared<cmg::TagInfo>();
                cmg::TagUtility::fillImplFromPrototype(implTag, prototype);
                impl = implTag;
            }
            i++;
        }
    }
}

////////////////////////////////////////////////////////////////////////

void cmg::MagicModeBase::setupParameters(const cmgSPVector<cmg::TagInfo>& tags, const cmg::FileCategory& fileCategory)
{
    auto parameterTags = cmg::TagUtility::filter(tags, {cmg::TagKind::PARAMETER});

    cmgVector<cmg::TagKind> filter;
    switch (fileCategory)
    {
    case cmg::FileCategory::Header:
        filter = {cmg::TagKind::PROTOTYPE};
        break;
    case cmg::FileCategory::Source:
        filter = {cmg::TagKind::FUNCTION};
        break;
    default:
        return;
    }

    auto functionTags = cmg::TagUtility::filter(tags, filter);
    std::sort(parameterTags.begin(), parameterTags.end(), cmg::TagInfoStartLineComparator());

    for (const auto& tag : functionTags)
    {
        auto functionScopedName = tag->mScope + "::" + tag->mSymbolName;
        for (const auto& parameterTag : parameterTags)
        {
            if (functionScopedName == parameterTag->mScope)
            {
                LOG_TRACE("Parameter: %s", parameterTag->mSymbolName.c_str());
                if ((parameterTag->mStartLine >= tag->mStartLine) && (parameterTag->mStartLine <= tag->mEndLine))
                {
                    tag->mParameters.push_back(parameterTag);
                    LOG_TRACE("Adding parameter %s to function %s", parameterTag->mSymbolName.c_str(), tag->mSymbolName.c_str());
                }
            }
        }
        tag->serializeParameters();
    }
}

////////////////////////////////////////////////////////////////////////

void cmg::MagicModeBase::loadCode(cmgVector<std::string>& nonTargetLines, cmgVector<cmg::ProtoImplTagPair>& protoImplPairs, const FilePath& headerFile, const FilePath& sourceFile)
{
    cmgVector<std::string> headerFileLines = cmg::utils::readLinesFromFile(headerFile.absolutePath());
    cmgVector<std::string> sourceFileLines = cmg::utils::readLinesFromFile(sourceFile.absolutePath());

    nonTargetLines.reserve(protoImplPairs.size() + 2);
    nonTargetLines.push_back("");

    int currentIndex = 0;
    for (auto protoImplPair : protoImplPairs)
    {
        const auto& prototype = protoImplPair.mPrototype;
        auto& impl = protoImplPair.mImplementation;
        auto startIndex = impl->mStartLine - 1;
        auto endIndex = impl->mEndLine - 1;
        if (startIndex < 0)
        {
            nonTargetLines.push_back(mSettings.mFunctionSpacer);
            continue;
        }

        std::ostringstream target;
        std::ostringstream nonTarget;

        while (currentIndex < startIndex && currentIndex < sourceFileLines.size())
        {
            nonTarget << sourceFileLines[currentIndex] << "\n";
            currentIndex++;
        }
        nonTargetLines.push_back(nonTarget.str());

        while (currentIndex <= endIndex && currentIndex < sourceFileLines.size())
        {
            target << sourceFileLines[currentIndex] << "\n";
            currentIndex++;
        }
        impl->mStr = target.str();
    }
    std::ostringstream footer;
    while (currentIndex < sourceFileLines.size())
    {
        footer << sourceFileLines[currentIndex] << "\n";
        currentIndex++;
    }
    nonTargetLines.push_back(footer.str());
}

////////////////////////////////////////////////////////////////////////

void cmg::MagicModeBase::writeCode(cmgVector<std::string>& nonTargetLines, cmgVector<cmg::ProtoImplTagPair>& protoImplPairs, const FilePath& headerFile, const FilePath& sourceFile)
{
    if (nonTargetLines.empty() || protoImplPairs.empty())
    {
        LOG_ERROR("No code to write");
        return;
    }
    auto fileStream = getFileStream(headerFile, sourceFile);

    if (!fileStream.is_open())
        return;

    // Header + Function Spacers + Footer = Non-Target Code.
    if (nonTargetLines.size() != protoImplPairs.size() + 2)
    {
        LOG_ERROR("Expected %zu lines in non-target code, got %zu lines only", protoImplPairs.size() + 2, nonTargetLines.size());
        return;
    }

    fileStream << nonTargetLines[0];

    for (size_t ntIndex = 1, tIndex = 0; tIndex < protoImplPairs.size(); ++tIndex, ++ntIndex)
    {
        fileStream << nonTargetLines[ntIndex];
        fileStream << protoImplPairs[tIndex].mImplementation->mStr;
    }

    fileStream << nonTargetLines.back();
}

////////////////////////////////////////////////////////////////////////

std::ofstream cmg::MagicModeBase::getFileStream(const FilePath& headerFile, const FilePath& sourceFile)
{
    FilePath newSourceFile{""};
    std::ofstream fileStream;
    LOG_DEBUG("Source file path given is '%s'", sourceFile.absolutePath().c_str());
    if (sourceFile.empty())
    {
        std::string sourceFilePath = sourceFile.baseDir() + "/" + headerFile.relativePathStem() + mSettings.mSourceExtensions.front();
        newSourceFile = FilePath(sourceFilePath, sourceFile.baseDir());
    }
    else
    {
        newSourceFile = sourceFile;
    }

    if (newSourceFile.isExist() && newSourceFile.isRegularFile())
    {
        if (mSettings.mInplaceFlag)
        {
            // If suffix is set, rename the file.
            if (!mSettings.mInplaceSuffix.empty())
            {
                std::error_code ec;
                auto newPath = sourceFile.stdPath();
                newPath += mSettings.mInplaceSuffix;
                LOG_DEBUG("Renaming '%s' to '%s'", sourceFile.stdPath().c_str(), newPath.c_str());
                std::filesystem::rename(sourceFile.stdPath(), newPath, ec);
                LOG_DEBUG("Rename status: %d, %s", ec.value(), ec.message().c_str());
            }
        }
        else
            newSourceFile = FilePath(newSourceFile.absolutePath() + "_magic");
    }

    if (!newSourceFile.empty())
    {
        LOG_DEBUG("Writing magic code to '%s'", newSourceFile.absolutePath().c_str());
        fileStream.open(newSourceFile.absolutePath());
        if (!fileStream.is_open())
            LOG_ERROR("Failed to open file for writing '%s'", newSourceFile.absolutePath().c_str());
    }
    else
    {
        LOG_ERROR("Could not decide file for writing magic code '%s'", headerFile.stdPath().c_str());
    }
    return fileStream;
}

////////////////////////////////////////////////////////////////////////
