#pragma once

#include <string>

#include "cppmagic/FilePath.h"
#include "cppmagic/Logger.h"
#include "cppmagic/MemoryAlias.h"
#include "cppmagic/Utils.h"
namespace cmg
{

enum class FileCategory
{
    Header,
    Source
};

class FileSelector
{
public:
    // Constructor.
    FileSelector() = default;
    FileSelector(const std::string& headerDir, const std::string& srcDir);
    FileSelector(const std::string& singleSrcDir);

    // Select header-implementation file pairs recursively.
    cmgVector<std::pair<FilePath, FilePath>> selectFilePairs(const std::string& headerFolder, const std::string srcFolder, bool allowUnpaired = true, int maxDepth = -1);
    cmgVector<std::pair<FilePath, FilePath>> selectFilePairs(const std::string& singleSrcFolder, bool allowUnpaired = true, int maxDepth = -1);

    // Pair up header-implementation file pairs.
    cmgVector<std::pair<FilePath, FilePath>> selectFilePairsFromFiles(const cmgVector<std::string>& files, bool allowUnpaired = true);

    cmgVector<std::pair<FilePath, FilePath>> selectFilePairs(const cmgVector<std::string>& headerFolders, const cmgVector<std::string> srcFolders, bool allowUnpaired = true, int maxDepth = -1);
    cmgVector<std::pair<FilePath, FilePath>> selectFilePairs(const cmgVector<std::string>& singleSrcFolders, bool allowUnpaired = true, int maxDepth = -1);

    // Select files under the folder. true for header, false for source
    cmgVector<FilePath> selectFiles(const std::string& singleSrcFolder, const cmg::FileCategory& fileType, int depth = -1);
    cmgVector<FilePath> selectFiles(const std::string& singleSrcFolder, const cmgVector<std::string>& extensions, int depth = -1);

    // Set extensions.
    void setExtensions(const std::vector<std::string>& headerExtensions, const std::vector<std::string>& sourceExtensions);
    // Gather files.
    cmgVector<std::filesystem::path> gatherFiles(const std::filesystem::path& dirPath, const cmgVector<std::string>& extensions, int maxDepth = -1);

private:
    // Header Folder.
    std::string mHeaderDir = "";
    // Src Folder.
    std::string mSourceDir = "";
    // Single src folder.
    std::string mSingleSrcFolder = "";
    // Header extensions.
    std::vector<std::string> mHeaderExtensions = {".h", ".hpp", ".hxx", ".hh"};
    // Source extensions.
    std::vector<std::string> mSourceExtensions = {".c", ".cpp", ".cxx", ".cc"};
};
} // namespace cmg
