#include "cppmagic/CppMagic.h"
#include "cppmagic/TagManager.h"
#include "cppmagic/TagUtility.h"

int main(int argc, char** argv)
{
    cmg::Settings settings;
    settings.processArguments(argc, argv);

    cmg::FileSelector fileSelector;
    fileSelector.setExtensions(settings.mHeaderExtensions, settings.mSourceExtensions);
    auto filePairs = fileSelector.selectFilePairs(settings.mHeaderDirs, settings.mSourceDirs);
    auto filePairs2 = fileSelector.selectFilePairsFromFiles(settings.mFiles);
    filePairs.insert(filePairs.end(), filePairs2.begin(), filePairs2.end());

    if (filePairs.empty())
    {
        LOG_DEBUG("Did not find any files. Exiting.");
        return 0;
    }

    for (auto& [header, source] : filePairs)
    {
        LOG_DEBUG("AbsoutePath  ==> Header: %s", header.absolutePath().c_str());
        LOG_DEBUG("                 Source: %s", source.absolutePath().c_str());
        LOG_DEBUG("RelativePath ==> Header: %s", header.relativePath().c_str());
        LOG_DEBUG("                 Source: %s", source.relativePath().c_str());
    }
    auto tagPairs = cmg::TagUtility::generateAndRead(filePairs);

    auto tagManager = std::make_shared<cmg::TagManager>();
    tagManager->addPairs(tagPairs);

    auto magic = cmg::CppMagic::getMagicMode(settings, tagManager);
    return magic->run(filePairs);
}
