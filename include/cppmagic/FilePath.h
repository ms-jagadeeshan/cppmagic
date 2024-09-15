#pragma once

#include <filesystem>
#include <string>

struct FilePair
{
    std::string headerFile;
    std::string srcFile;
};

class FilePath
{
public:
    // Constructor
    explicit FilePath(const std::string& filePath, const std::string& relativeTo = ".");

    // Print.
    void print() const;
    // Update the file path.
    void update(const std::string& filePath, const std::string& baseDir);
    // Set Base dir.
    void updateBaseDir(const std::string& baseDir);
    // Is directory.
    bool isDirectory() const;
    // Is regular file.
    bool isRegularFile() const;
    // Return std::filesystem::path
    std::filesystem::path stdPath() const;
    // Is file path empty.
    bool empty() const;
    // Exists.
    bool isExist() const;
    //  Get Filename.
    const std::string& fileName() const;
    // Get Filestem.
    const std::string& fileStem() const;
    // Get extension.
    const std::string& extension() const;
    // Get the base directory.
    const std::string& baseDir() const;
    // Get relative path.
    const std::string& relativePath() const;
    // Get relative path stem.
    const std::string& relativePathStem() const;
    // Get absolute path.
    const std::string& absolutePath() const;

private:
    // Path
    // Unresolved path.
    std::string mUnresolvedPath;
    // Absolute full path.
    std::string mFullPath;
    // std::filesystem path object
    std::filesystem::path mPath;

    // File Components.
    // File Name.
    std::string mFileName;
    // File stem (name without extension).
    std::string mFileStem;
    // File extension.
    std::string mExt;

    // Relative Path.
    // Relative path from base directory.
    std::string mRelPath;
    // Relative path without extension.
    std::string mRelPathStem;
    // Base directory for relative path.
    std::string mBaseDir;

    // File Type.
    std::filesystem::file_type mFileType;
};
