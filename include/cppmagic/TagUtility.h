#pragma once

#include "cppmagic/MemoryAlias.h"
#include "cppmagic/TagInfo.h"
#include "cppmagic/Utils.h"

namespace cmg
{

class TagUtility
{
public:
    // Read tag file.
    static cmgSPVector<cmg::TagInfo> read(const std::string& tagsFile);
    static cmgSPVector<cmg::TagInfo> read(const std::string& filePath, const std::string& tagsFile);
    static cmgSPVector<cmg::TagInfo> read(const std::vector<std::string>& filePaths, const std::string& tagsFile);
    static cmgSPVector<cmg::TagInfo> readFromString(const std::string& tagsJsonData);

    // Generate tag.
    static std::string generate(const std::string& filePath, const std::string& ctagsPath = "/usr/bin/ctags", const std::string& workingDir = "");
    static std::string generate(const std::vector<std::string>& filePaths, const std::string& ctagsPath = "/usr/bin/ctags", const std::string& workingDir = "");
    static std::string generateRecursive(const std::string& folder, const std::string& ctagsPath = "/usr/bin/ctags", const std::string& workingDir = "");

    //
    static cmgVector<std::pair<cmgSPVector<cmg::TagInfo>, cmgSPVector<cmg::TagInfo>>> generateAndRead(const cmgVector<std::pair<FilePath, FilePath>> filePaths, const std::string& ctagsPath = "/usr/bin/ctags", const std::string& workingDir = "");

    // Generate and write tags.
    static void write(const std::string& filePath, const std::string& outPath, const std::string& ctagsPath = "/usr/bin/ctags", const std::string& workingDir = "");
    static void write(const std::vector<std::string>& filePaths, const std::string& outPath, const std::string& ctagsPath = "/usr/bin/ctags", const std::string& workingDir = "");

    // Filter tags.
    static cmgSPVector<cmg::TagInfo> filter(const cmgSPVector<cmg::TagInfo>& tags, const cmgVector<cmg::TagKind>& filter);
    // Fill impl from prototype.
    static void fillImplFromPrototype(cmgSP<cmg::TagInfo> implTag, const cmgSP<cmg::TagInfo>& protoTag);

    // Get the header include line.
    static std::string getHeaderIncludeLine(const FilePath& filePath);

    // Print the tag.
    static void print(const cmgSP<cmg::TagInfo>& tag);
    // Sanitize input.
    static std::string sanitizeInput(const std::string& input);

private:
    // Read implementation.
    static cmgSP<cmg::TagInfo> readImpl(const std::string& tagsJsonData);
    // Generate implementation.
    static std::string generateImpl(const std::vector<std::string>& filePaths, const std::string& ctagsPath, const std::string& workingDir);
    static std::string generateImpl(const std::string& folder, const std::string& ctagsPath, const std::string& workingDir);
    // Execute and get output from command line.
    static std::string executeCommand(const std::string& command);

private:
    static inline std::string mOptions = " --c++-kinds=* --fields=* --output-format=json -o - ";
    static inline std::string mRecursiveOption = " --c++-kinds=* --fields=* --output-format=json --recursive=yes -o - .";
};

} // namespace cmg
