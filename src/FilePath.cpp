#include "cppmagic/FilePath.h"

#include <iostream>

////////////////////////////////////////////////////////////////////////

FilePath::FilePath(const std::string& filePath, const std::string& relativeTo)
    : mUnresolvedPath(filePath)
{
    mBaseDir = std::filesystem::absolute(relativeTo).lexically_normal().string();
    if (mUnresolvedPath.empty())
    {
        return;
    }

    mPath = std::filesystem::path(filePath);
    mFullPath = std::filesystem::absolute(mPath).lexically_normal().string();
    mFileName = mPath.filename();
    mFileStem = mPath.stem();
    mExt = mPath.extension();
    mFileType = std::filesystem::status(mPath).type();
    updateBaseDir(relativeTo);
}

////////////////////////////////////////////////////////////////////////

void FilePath::print() const
{
    std::cout << "Unresolved Path: " << mUnresolvedPath << std::endl;
    std::cout << "Full Path: " << mFullPath << std::endl;
    std::cout << "File Name: " << mFileName << std::endl;
    std::cout << "File Stem: " << mFileStem << std::endl;
    std::cout << "Extension: " << mExt << std::endl;
    std::cout << "Relative Path: " << mRelPath << std::endl;
    std::cout << "Relative Path without Extension: " << mRelPathStem << std::endl;
    std::cout << "Base Directory for Relative Path: " << mBaseDir << std::endl;
}

void FilePath::update(const std::string& filePath, const std::string& baseDir)
{
    mBaseDir = std::filesystem::absolute(baseDir).lexically_normal().string();
    if (mUnresolvedPath.empty())
    {
        return;
    }

    mPath = std::filesystem::path(filePath);
    mFullPath = std::filesystem::absolute(mPath).lexically_normal().string();
    mFileName = mPath.filename();
    mFileStem = mPath.stem();
    mExt = mPath.extension();
    mFileType = std::filesystem::status(mPath).type();
    updateBaseDir(baseDir);
}

////////////////////////////////////////////////////////////////////////

void FilePath::updateBaseDir(const std::string& baseDir)
{
    auto relPath = mPath.lexically_relative(baseDir);
    mRelPath = relPath.string();
    mRelPathStem = relPath.replace_extension("");
}

////////////////////////////////////////////////////////////////////////

bool FilePath::isDirectory() const
{
    return std::filesystem::is_directory(mPath);
}

////////////////////////////////////////////////////////////////////////

bool FilePath::isRegularFile() const
{
    return std::filesystem::is_regular_file(mPath);
}

std::filesystem::path FilePath::stdPath() const
{
    return mPath;
}
////////////////////////////////////////////////////////////////////////

bool FilePath::empty() const
{
    return mUnresolvedPath.empty();
}

////////////////////////////////////////////////////////////////////////

bool FilePath::isExist() const
{
    return std::filesystem::exists(mPath);
}

////////////////////////////////////////////////////////////////////////

const std::string& FilePath::fileName() const
{
    return mFileName;
}

////////////////////////////////////////////////////////////////////////

const std::string& FilePath::fileStem() const
{
    return mFileStem;
}

////////////////////////////////////////////////////////////////////////

const std::string& FilePath::extension() const
{
    return mExt;
}

////////////////////////////////////////////////////////////////////////

const std::string& FilePath::baseDir() const
{
    return mBaseDir;
}

////////////////////////////////////////////////////////////////////////

const std::string& FilePath::relativePath() const
{
    return mRelPath;
}

////////////////////////////////////////////////////////////////////////

const std::string& FilePath::relativePathStem() const
{
    return mRelPathStem;
}

////////////////////////////////////////////////////////////////////////

const std::string& FilePath::absolutePath() const
{
    return mFullPath;
}

////////////////////////////////////////////////////////////////////////
