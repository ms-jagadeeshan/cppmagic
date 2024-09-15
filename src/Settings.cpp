#include "cppmagic/Settings.h"

#include "CLI/CLI11.hpp"
#include "cppmagic/Logger.h"
#include "cppmagic/Utils.h"

////////////////////////////////////////////////////////////////////////

bool cmg::Settings::processArguments(int argc, char** argv)
{
    CLI::App app{"cppmagic"};

    CLI::App* gen = app.add_subcommand("generate", "Generate cpp file from header")
                        ->alias("make")
                        ->alias("abracadabra")
                        ->alias("skadoosh")
                        ->alias("brew")
                        ->alias("forge");

    CLI::App* sort = app.add_subcommand("sort", "Sort cpp files according to header files")
                         ->alias("jigsaw")
                         ->alias("align")
                         ->alias("organize")
                         ->alias("organise")
                         ->alias("order");

    CLI::App* boom = app.add_subcommand("boom", "Both generates and sorts")
                         ->alias("hack")
                         ->alias("magic")
                         ->alias("sync");

    addCommonOptions(gen);
    addCommonOptions(sort);
    addCommonOptions(boom);
    app.require_subcommand();

    sort->add_option("--sort", mSortingMethodStr, "Sorting method. header, alpha");
    boom->add_option("--sort", mSortingMethodStr, "Sorting method. header, alpha");

    CLI11_PARSE(app, argc, argv);

    if (mDebug)
        Logger::getInstance()->setLogLevel(LogLevel::LOG_DEBUG);

    CLI::App* subApp = nullptr;
    if (gen->parsed())
    {
        mMagicMode = MagicMode::GENERATE;
        subApp = gen;
    }
    else if (sort->parsed())
    {
        mMagicMode = MagicMode::SORT;
        subApp = sort;
    }
    else if (boom->parsed())
    {
        mMagicMode = MagicMode::BOOM;
        subApp = boom;
    }

    if (subApp->count("--in-place") > 0)
    {
        mInplaceFlag = true;
    }

    LOG_INFO("Given files size : " + std::to_string(mFiles.size()));
    if (mFiles.size() == 2)
    {
        auto file1 = mFiles[0];
        auto file2 = mFiles[1];
        if (std::filesystem::is_directory(file1) && std::filesystem::is_directory(file2))
        {
            mHeaderDirs.push_back(file1);
            mSourceDirs.push_back(file2);
            mFiles.clear();
        }
    }

    for (auto itr = mFiles.begin(); itr != mFiles.end();)
    {
        if (std::filesystem::is_directory(*itr))
        {
            mHeaderDirs.push_back(*itr);
            mSourceDirs.push_back(*itr);
            mFiles.erase(itr);
        }
        else
        {
            itr++;
        }
    }

    mSortingMethod = cmg::sortingStrToEnum(mSortingMethodStr);
    LOG_INFO("Sorting Method   : " + cmg::sortingEnumToStr(mSortingMethod));
    LOG_INFO("Header Extensions: " + cmg::utils::join(mHeaderExtensions));
    LOG_INFO("Source Extensions: " + cmg::utils::join(mSourceExtensions));
    LOG_INFO("InPlaceFlag      : " + std::string(mInplaceFlag ? "true" : "false"));
    LOG_INFO("InPlaceSuffix    : " + mInplaceSuffix);
    LOG_INFO("Files :");
    for (auto file : mFiles)
        LOG_INFO("   " + file);

    LOG_INFO("Header Directories:");
    for (auto dir : mHeaderDirs)
        LOG_INFO("   " + dir);

    LOG_INFO("Source Directories:");
    for (auto dir : mSourceDirs)
        LOG_INFO("   " + dir);

    return true;
}

////////////////////////////////////////////////////////////////////////

void cmg::Settings::addCommonOptions(CLI::App* app)
{
    app->add_option("-t,--tagfile", mTagFile, "Generated ctags file to use. If empty tags will be generated")
        ->check(CLI::ExistingFile)
        ->group("Common Options");

    app->add_option("--ctags-path", mCTagsPath, "Path of ctags executable.")
        ->default_str("/usr/bin/ctags")
        ->check(CLI::ExistingFile)
        ->group("Common Options");

    app->add_option("--header-ext", mHeaderExtensions, "File extensions to be considered as headers.")
        ->delimiter(',')
        ->group("Common Options");

    app->add_option("--source-ext", mSourceExtensions, "File extensions to be considered as sources.")
        ->delimiter(',')
        ->group("Common Options");

    app->add_option("-i,--in-place", mInplaceSuffix, "Edit files in place (makes backup if SUFFIX supplied)")
        ->expected(0, 1)
        ->option_text("SUFFIX")
        ->group("Common Options");

    app->add_flag("-d,--debug", mDebug, "Enable debug mode.")
        ->group("Common Options");

    app->add_option("files/folders", mFiles, "List of files or folders")
        ->required();
}

////////////////////////////////////////////////////////////////////////
