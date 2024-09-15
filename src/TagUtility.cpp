#include "cppmagic/TagUtility.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>

#include "cppmagic/Logger.h"
#include "nlohmann/json.hpp"

////////////////////////////////////////////////////////////////////////

cmgSPVector<cmg::TagInfo> cmg::TagUtility::read(const std::string& tagsFile)
{
    cmgSPVector<cmg::TagInfo> tags{};
    if (tagsFile.empty())
    {
        LOG_ERROR("Tags File name is empty");
        return tags;
    }

    std::ifstream fstream(tagsFile);
    if (!fstream.is_open())
    {
        LOG_ERROR("Failed to open file %s", tagsFile.c_str());
        return tags;
    }

    std::string line;
    while (std::getline(fstream, line))
    {
        auto tag = readImpl(line);
        if (tag == nullptr)
            continue;

        tags.push_back(tag);
    }
    fstream.close();

    return tags;
}

////////////////////////////////////////////////////////////////////////

cmgSPVector<cmg::TagInfo> cmg::TagUtility::read(const std::string& filePath, const std::string& tagsFile)
{
    return read({filePath}, tagsFile);
}

////////////////////////////////////////////////////////////////////////

cmgSPVector<cmg::TagInfo> cmg::TagUtility::read(const std::vector<std::string>& filePaths, const std::string& tagsFile)
{
    cmgSPVector<cmg::TagInfo> tags{};
    if (filePaths.empty() || tagsFile.empty())
    {
        LOG_ERROR("FilePaths or tagsFile is empty");
        return tags;
    }

    std::ifstream fstream(tagsFile);
    if (!fstream.is_open())
    {
        LOG_ERROR("Failed to open file %s", tagsFile.c_str());
        return tags;
    }

    std::string line;
    while (std::getline(fstream, line))
    {
        auto tag = readImpl(line);
        if (tag == nullptr)
            continue;

        if (std::find(filePaths.begin(), filePaths.end(), tag->mFilePath) != filePaths.end())
            tags.push_back(tag);
    }

    fstream.close();
    return tags;
}

////////////////////////////////////////////////////////////////////////

cmgSPVector<cmg::TagInfo> cmg::TagUtility::readFromString(const std::string& tagsJsonData)
{
    std::istringstream ss(tagsJsonData);
    std::string line;
    cmgSPVector<cmg::TagInfo> tags;
    while (std::getline(ss, line))
    {
        auto tag = readImpl(line);
        if (tag != nullptr)
            tags.push_back(tag);
    }
    return tags;
}

////////////////////////////////////////////////////////////////////////

std::string cmg::TagUtility::generate(const std::string& filePath, const std::string& ctagsPath, const std::string& workingDir)
{
    return generateImpl(std::vector<std::string>{filePath}, ctagsPath, workingDir);
}

////////////////////////////////////////////////////////////////////////

std::string cmg::TagUtility::generate(const std::vector<std::string>& filePaths, const std::string& ctagsPath, const std::string& workingDir)
{
    return generateImpl(filePaths, ctagsPath, workingDir);
}

////////////////////////////////////////////////////////////////////////

std::string cmg::TagUtility::generateRecursive(const std::string& folder, const std::string& ctagsPath, const std::string& workingDir)
{
    return generateImpl(folder, ctagsPath, workingDir);
}

////////////////////////////////////////////////////////////////////////

cmgVector<std::pair<cmgSPVector<cmg::TagInfo>, cmgSPVector<cmg::TagInfo>>> cmg::TagUtility::generateAndRead(const cmgVector<std::pair<FilePath, FilePath>> filePairs, const std::string& ctagsPath, const std::string& workingDir)
{
    cmgVector<std::pair<cmgSPVector<cmg::TagInfo>, cmgSPVector<cmg::TagInfo>>> result;
    for (const auto& [headerFile, sourceFile] : filePairs)
    {
        LOG_DEBUG("Generating tags for %s and %s", headerFile.relativePath().c_str(), sourceFile.relativePath().c_str());
        auto tagsHeaderJsonStr = generate(headerFile.relativePath(), ctagsPath, headerFile.baseDir());
        LOG_TRACE("Generated JSON: " + tagsHeaderJsonStr);
        auto headerTags = readFromString(tagsHeaderJsonStr);
        for (auto tag : headerTags)
        {
            tag->mFile = headerFile;
        }

        auto tagsSourceJsonStr = generate(sourceFile.relativePath(), ctagsPath, sourceFile.baseDir());
        LOG_TRACE("Generated JSON: " + tagsSourceJsonStr);
        auto sourceTags = readFromString(tagsSourceJsonStr);
        for (auto tag : sourceTags)
        {
            tag->mFile = sourceFile;
        }

        result.emplace_back(headerTags, sourceTags);
    }
    return result;
}

////////////////////////////////////////////////////////////////////////

void cmg::TagUtility::write(const std::string& filePath, const std::string& outPath, const std::string& ctagsPath, const std::string& workingDir)
{
    write({filePath}, outPath, ctagsPath, workingDir);
}

////////////////////////////////////////////////////////////////////////

void cmg::TagUtility::write(const std::vector<std::string>& filePaths, const std::string& outPath, const std::string& ctagsPath, const std::string& workingDir)
{
    std::string result = generateImpl(filePaths, ctagsPath, workingDir);

    std::ofstream outFile(outPath);
    if (!outFile)
    {
        throw std::runtime_error("Failed to open output file for writing.");
    }
    outFile << result;
}

////////////////////////////////////////////////////////////////////////

void cmg::TagUtility::print(const cmgSP<cmg::TagInfo>& tag)
{
    std::string str;
    str += "Name       :" + tag->mSymbolName + "\n";
    str += "FilePath   :" + tag->mFilePath + "\n";
    str += "Start Line :" + std::to_string(tag->mStartLine) + "\n";
    str += "End Line   :" + std::to_string(tag->mStartLine) + "\n";
    str += "Tag Kind   :" + tag->tagKindStr() + "\n";
    str += "Return Type:" + tag->mTypeRef + "\n";
    str += "Access     :" + tag->mAccessType + "\n";
    str += "Scope      :" + tag->mScope + "\n";
    str += "ScopeKind  :" + tag->scopeKindStr() + "\n";
    str += "Signature  :" + tag->mOgSignature + "\n";
    str += "Extras     :" + tag->mExtras + "\n";
    str += "Code       :" + tag->mStr + "\n";
}

////////////////////////////////////////////////////////////////////////

std::string cmg::TagUtility::generateImpl(const std::vector<std::string>& filePaths, const std::string& ctagsPath, const std::string& workingDir)
{
    if (filePaths.empty() || (filePaths.size() == 1 && filePaths[0].empty()))
        return "";

    std::string command = ctagsPath + " " + mOptions;

    std::string sanitizedCommand = sanitizeInput(command);
    for (const auto& file : filePaths)
        sanitizedCommand += " \"" + file + "\"";

    std::string fullCommand = sanitizedCommand;
    if (!workingDir.empty())
    {
        std::string sanitizedDir = sanitizeInput(workingDir);
        fullCommand = "cd \"" + sanitizedDir + "\" && " + sanitizedCommand;
    }

    return executeCommand(fullCommand);
}

////////////////////////////////////////////////////////////////////////

std::string cmg::TagUtility::generateImpl(const std::string& folder, const std::string& ctagsPath, const std::string& workingDir)
{
    std::string command = ctagsPath + " " + mRecursiveOption;

    std::string sanitizedCommand = sanitizeInput(command);
    sanitizedCommand += " \"" + folder + "\"";

    std::string fullCommand = sanitizedCommand;
    if (!workingDir.empty())
    {
        std::string sanitizedDir = sanitizeInput(workingDir);
        fullCommand = "cd \"" + sanitizedDir + "\" && " + sanitizedCommand;
    }

    return executeCommand(fullCommand);
}

////////////////////////////////////////////////////////////////////////

std::string cmg::TagUtility::executeCommand(const std::string& command)
{
    LOG_DEBUG("Executing command: %s", command.c_str());
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(command.c_str(), "r"), pclose);
    if (!pipe)
    {
        throw std::runtime_error("popen() failed!");
    }
    while (!feof(pipe.get()))
    {
        if (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
        {
            result += buffer.data();
        }
    }
    return result;
}

////////////////////////////////////////////////////////////////////////

std::string cmg::TagUtility::sanitizeInput(const std::string& input)
{
    std::string sanitized;
    for (char c : input)
    {
        if (c == '"' || c == '\'' || c == ';')
        {
            sanitized += '\\';
        }
        sanitized += c;
    }
    return sanitized;
}

////////////////////////////////////////////////////////////////////////

cmgSP<cmg::TagInfo> cmg::TagUtility::readImpl(const std::string& tagsJsonData)
{
    if (tagsJsonData.empty())
        return nullptr;

    nlohmann::json j;
    try
    {
        j = nlohmann::json::parse(tagsJsonData);
    }
    catch (const nlohmann::json::parse_error& e)
    {
        fprintf(stderr, "Error: %s", e.what());
        return nullptr;
    }

    if (!j.contains("kind") || !j.contains("name"))
        return nullptr;

    auto tagInfo = std::make_shared<TagInfo>();
    tagInfo->mSymbolName = j.value("name", "");
    tagInfo->mFilePath = j.value("path", "");
    tagInfo->mStartLine = j.value("line", -1);
    tagInfo->mEndLine = j.value("end", -1);
    std::string tagKind = j.value("kind", "");
    tagInfo->mTagKind = cmg::tagKindStrToEnum(tagKind);
    auto returnType = j.value("typeref", "");
    if (returnType.size() >= 9)
        tagInfo->mTypeRef = returnType.substr(9);
    tagInfo->mAccessType = j.value("access", "");
    tagInfo->mScope = j.value("scope", "");
    std::string scopeKind = j.value("kind", "");
    tagInfo->mScopeKind = cmg::scopeKindStrToEnum(scopeKind);
    tagInfo->mOgSignature = j.value("signature", "");
    tagInfo->mExtras = j.value("extras", "");
    tagInfo->mNth = j.value("nth", -1);

    return tagInfo;
}

////////////////////////////////////////////////////////////////////////

cmgSPVector<cmg::TagInfo> cmg::TagUtility::filter(const cmgSPVector<cmg::TagInfo>& tags, const cmgVector<cmg::TagKind>& tagKinds)
{
    cmgSPVector<cmg::TagInfo> filteredTags{};
    for (const auto& tag : tags)
    {
        if (cmg::utils::contains(tagKinds, tag->mTagKind))
            filteredTags.push_back(tag);
    }

    return filteredTags;
}

////////////////////////////////////////////////////////////////////////

void cmg::TagUtility::fillImplFromPrototype(cmgSP<cmg::TagInfo> implTag, const cmgSP<cmg::TagInfo>& protoTag)
{
    if (implTag == nullptr || protoTag == nullptr)
        return;

    if (protoTag->mTagKind != cmg::TagKind::PROTOTYPE)
        return;

    implTag->mSymbolName = protoTag->mSymbolName;
    implTag->mTagKind = cmg::TagKind::FUNCTION;
    implTag->mTypeRef = protoTag->mTypeRef;
    implTag->mScope = protoTag->mScope;
    implTag->mScopeKind = protoTag->mScopeKind;
    implTag->mSignature = protoTag->mSignature;
    implTag->mOgSignature = protoTag->mOgSignature;
    implTag->mParameters = protoTag->mParameters;
    implTag->mParametersStr = protoTag->mParametersStr;

    implTag->buildCodeStr();
}

////////////////////////////////////////////////////////////////////////

std::string cmg::TagUtility::getHeaderIncludeLine(const FilePath& filePath)
{
    if (filePath.empty())
        return "";

    return "#include \"" + filePath.relativePath() + "\"\n";
}

////////////////////////////////////////////////////////////////////////
