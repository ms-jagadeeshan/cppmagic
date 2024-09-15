#pragma once

#include "CLI/CLI11.hpp"
#include "cppmagic/MemoryAlias.h"
#include "cppmagic/SortingMethod.h"

namespace cmg
{
enum class MagicMode
{
    GENERATE,
    SORT,
    BOOM
};

class Settings
{
public:
    Settings() = default;
    ~Settings() = default;

    bool processArguments(int argc, char** argv);

private:
    void addCommonOptions(CLI::App* app);

public:
    // Path of ctags executable
    std::string mCTagsPath{"/usr/bin/ctags"};
    // Debug Flag.
    bool mDebug{false};
    // Enum sorting.
    cmg::SortingMethod mSortingMethod{cmg::SortingMethod::HEADER};
    std::string mSortingMethodStr;
    // File extensions to be considered as headers.
    cmgVector<std::string> mHeaderExtensions{".h", ".hpp", ".hxx", ".hh"};
    // File extensions to be considered as sources.
    cmgVector<std::string> mSourceExtensions{".cpp", ".c", ".cxx", ".cc"};
    // Magic mode.
    cmg::MagicMode mMagicMode{cmg::MagicMode::GENERATE};
    // Whether to do inplace replace.
    bool mInplaceFlag{false};
    // Inplace Suffix.
    std::string mInplaceSuffix{""};
    // Files to be reordered.
    cmgVector<std::string> mFiles;
    // Header folder.
    cmgVector<std::string> mHeaderDirs{};
    // Source folder.
    cmgVector<std::string> mSourceDirs{};
    // Whether to generate tags.
    std::string mTagFile{""};
    // Sorting method. header, alpha.
    std::string mSorting{"header"};
    std::string mHeaderCode = "";
    std::string mFooterCode = "\n//////////////////////////////////////////////////////////////////////////////\n";
    // Function spacer.
    std::string mFunctionSpacer = "\n//////////////////////////////////////////////////////////////////////////////\n\n";
};
} // namespace cmg
