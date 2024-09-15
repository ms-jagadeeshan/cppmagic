#include "cppmagic/GenerateMode.h"

#include "cppmagic/TagUtility.h"

////////////////////////////////////////////////////////////////////////

cmg::GenerateMode::GenerateMode(const cmg::Settings& settings, const cmgSP<cmg::TagManager>& tagManager)
    : MagicModeBase(settings, tagManager)
{
}

////////////////////////////////////////////////////////////////////////

void cmg::GenerateMode::runImpl(cmgVector<std::string>& nonTargetLines, cmgVector<cmg::ProtoImplTagPair>& protoImplPairs, const FilePath& headerFile, const FilePath& sourceFile)
{
    if (protoImplPairs.empty())
        return;

    std::sort(protoImplPairs.begin(), protoImplPairs.end(), cmg::TagPairProtoStartLineComparator());

    nonTargetLines.clear();
    nonTargetLines.reserve(protoImplPairs.size() + 2);

    auto includeLine = cmg::TagUtility::getHeaderIncludeLine(headerFile);
    nonTargetLines.push_back(mSettings.mHeaderCode + includeLine);
    for (int i = 0; i < protoImplPairs.size(); ++i)
    {
        nonTargetLines.push_back(mSettings.mFunctionSpacer);
    }
    nonTargetLines.push_back(mSettings.mFooterCode);
}

////////////////////////////////////////////////////////////////////////

void cmg::GenerateMode::preProcessTags(const std::pair<cmgSPVector<cmg::TagInfo>, cmgSPVector<cmg::TagInfo>>& tagPairs)
{
    setupParameters(tagPairs.first, cmg::FileCategory::Header);
}

////////////////////////////////////////////////////////////////////////

cmgVector<cmg::ProtoImplTagPair> cmg::GenerateMode::setupProtoImplPairs(const std::pair<cmgSPVector<cmg::TagInfo>, cmgSPVector<cmg::TagInfo>>& tagPairs)
{
    cmgVector<cmg::ProtoImplTagPair> protoImplPairs;
    auto prototypeTags = cmg::TagUtility::filter(tagPairs.first, {cmg::TagKind::PROTOTYPE});

    if (prototypeTags.empty())
        return {};

    // Ignore impl tags.
    for (auto prototypeTag : prototypeTags)
    {
        if (prototypeTag == nullptr)
            continue;
        protoImplPairs.emplace_back(prototypeTag, nullptr);
    }

    fillEmptyImpl(protoImplPairs);
    return protoImplPairs;
}

////////////////////////////////////////////////////////////////////////
