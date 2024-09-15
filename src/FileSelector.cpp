#include "cppmagic/FileSelector.h"

#include <algorithm>
#include <iostream>

////////////////////////////////////////////////////////////////////////

cmg::FileSelector::FileSelector(const std::string& headerDir, const std::string& srcDir)
    : mHeaderDir(headerDir), mSourceDir(srcDir)
{
}

////////////////////////////////////////////////////////////////////////

cmg::FileSelector::FileSelector(const std::string& singleSrcDir)
    : mSingleSrcFolder(singleSrcDir)
{
}

////////////////////////////////////////////////////////////////////////

cmgVector<std::pair<FilePath, FilePath>> cmg::FileSelector::selectFilePairs(const std::string& headerDir, const std::string sourceDir, bool allowUnpaired, int maxDepth)
{
    // Gather header files.
    cmgUOMap<std::string, FilePath> headerToPath;
    auto headers = gatherFiles(headerDir, mHeaderExtensions, -1);
    for (const auto& headerPath : headers)
    {
        auto headerFile = FilePath(headerPath, headerDir);
        headerToPath.try_emplace(headerFile.relativePathStem(), headerFile);
    }

    // Gather source files.
    cmgUOMap<std::string, FilePath> sourceToPath;
    auto sources = gatherFiles(sourceDir, mSourceExtensions, -1);
    for (const auto& sourcePath : sources)
    {
        auto sourceFile = FilePath(sourcePath, sourceDir);
        sourceToPath.try_emplace(sourceFile.relativePathStem(), sourceFile);
    }

    // Map corresponding header and source files.
    std::vector<std::pair<FilePath, FilePath>> pairedFiles;
    for (const auto& [pathStem, headerFile] : headerToPath)
    {
        auto itr = sourceToPath.find(pathStem);
        if (itr != sourceToPath.end())
        {
            pairedFiles.emplace_back(headerFile, itr->second);
        }
        else if (allowUnpaired)
        {
            pairedFiles.emplace_back(headerFile, FilePath("", sourceDir));
        }
    }

    return pairedFiles;
}

cmgVector<std::pair<FilePath, FilePath>> cmg::FileSelector::selectFilePairs(const std::string& singleSrcFolder, bool allowUnpaired, int maxDepth)
{
    return selectFilePairs(singleSrcFolder, singleSrcFolder, allowUnpaired, maxDepth);
}

////////////////////////////////////////////////////////////////////////

cmgVector<std::pair<FilePath, FilePath>> cmg::FileSelector::selectFilePairsFromFiles(const cmgVector<std::string>& files, bool allowUnpaired)
{
    std::unordered_map<std::string, std::string> headerFiles;
    std::unordered_map<std::string, std::string> sourceFiles;

    for (const auto& file : files)
    {
        auto filePath = FilePath(file);

        if (std::find(mHeaderExtensions.begin(), mHeaderExtensions.end(), filePath.extension()) != mHeaderExtensions.end())
        {
            headerFiles[filePath.relativePathStem()] = file;
        }

        else if (std::find(mSourceExtensions.begin(), mSourceExtensions.end(), filePath.extension()) != mSourceExtensions.end())
        {
            sourceFiles[filePath.relativePathStem()] = file;
        }
    }

    // Pair header and source files by their stem
    std::vector<std::pair<FilePath, FilePath>> pairedFiles;
    for (const auto& [stem, headerFile] : headerFiles)
    {
        if (sourceFiles.find(stem) != sourceFiles.end())
        {
            pairedFiles.emplace_back(headerFile, sourceFiles[stem]);
        }
        else
        {
            pairedFiles.emplace_back(headerFile, FilePath(""));
        }
    }
    return pairedFiles;
}

////////////////////////////////////////////////////////////////////////

cmgVector<std::pair<FilePath, FilePath>> cmg::FileSelector::selectFilePairs(const cmgVector<std::string>& headerFolders, const cmgVector<std::string> srcFolders, bool allowUnpaired, int maxDepth)
{
    if (headerFolders.size() != srcFolders.size())
    {
        LOG_ERROR("Number of header and source folders must be the same. %d vs %d", headerFolders.size(), srcFolders.size());
        return {};
    }

    cmgVector<std::pair<FilePath, FilePath>> out{};

    for (size_t i = 0; i < headerFolders.size(); ++i)
    {
        const auto& headerFolder = headerFolders[i];
        const auto& srcFolder = srcFolders[i];
        LOG_DEBUG("Selecting Pairs: %s, %s", headerFolder.c_str(), srcFolder.c_str());

        auto result = selectFilePairs(headerFolder, srcFolder, allowUnpaired, maxDepth);
        LOG_DEBUG("Found %d Pairs", result.size());

        for (auto resultItr = result.begin(); resultItr != result.end(); ++resultItr)
        {
            LOG_DEBUG("  Header: %s, Source: %s", resultItr->first.absolutePath().c_str(), resultItr->second.absolutePath().c_str());
        }
        out.insert(out.end(), std::make_move_iterator(result.begin()), std::make_move_iterator(result.end()));
    }
    return out;
}

////////////////////////////////////////////////////////////////////////

cmgVector<std::pair<FilePath, FilePath>> cmg::FileSelector::selectFilePairs(const cmgVector<std::string>& singleSrcFolders, bool allowUnpaired, int maxDepth)
{
    return selectFilePairs(singleSrcFolders, singleSrcFolders, allowUnpaired, maxDepth);
}

////////////////////////////////////////////////////////////////////////

cmgVector<FilePath> cmg::FileSelector::selectFiles(const std::string& singleSrcFolder, const cmg::FileCategory& fileType, int depth)
{
    std::vector<std::string> extensions;
    if (fileType == FileCategory::Header)
    {
        extensions = mHeaderExtensions;
    }
    else if (fileType == FileCategory::Source)
    {
        extensions = mSourceExtensions;
    }
    auto files = gatherFiles(singleSrcFolder, mHeaderExtensions, depth);

    cmgVector<FilePath> result;
    for (auto file : files)
    {
        result.emplace_back(file, singleSrcFolder);
    }

    return result;
}

////////////////////////////////////////////////////////////////////////

void cmg::FileSelector::setExtensions(const std::vector<std::string>& headerExtensions, const std::vector<std::string>& sourceExtensions)
{
    mHeaderExtensions = headerExtensions;
    mSourceExtensions = sourceExtensions;
}

////////////////////////////////////////////////////////////////////////

cmgVector<std::filesystem::path> cmg::FileSelector::gatherFiles(const std::filesystem::path& dirPath, const cmgVector<std::string>& extensions, int maxDepth)
{
    std::vector<std::filesystem::path> files;
    std::vector<std::pair<std::filesystem::path, int>> stack;
    stack.emplace_back(dirPath, 0);

    while (!stack.empty())
    {
        auto [currentPath, depth] = stack.back();
        stack.pop_back();

        if (maxDepth != -1 && depth > maxDepth)
            continue;

        try
        {
            for (const auto& entry : std::filesystem::directory_iterator(currentPath))
            {
                if (entry.is_directory())
                {
                    stack.emplace_back(entry.path(), depth + 1);
                }
                else if (entry.is_regular_file())
                {
                    std::string ext = entry.path().extension().string();
                    if (std::find(extensions.begin(), extensions.end(), ext) != extensions.end())
                    {
                        files.push_back(entry.path());
                    }
                }
            }
        }
        catch (const std::filesystem::filesystem_error& e)
        {
            LOG_ERROR("Error accessing %s: %s", currentPath.string().c_str(), e.what());
            std::cerr << "Error accessing " << currentPath << ": " << e.what() << '\n';
        }
    }

    return files;
}

////////////////////////////////////////////////////////////////////////
