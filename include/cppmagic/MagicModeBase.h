#pragma once

#include "cppmagic/Core.h"
#include "cppmagic/TagManager.h"

namespace cmg
{
class MagicModeBase
{
public:
    // Constructor.
    MagicModeBase() = default;
    MagicModeBase(const cmg::Settings& settings, const cmgSP<cmg::TagManager>& tagManager);

    // Set tag manager.
    void tagManager(const cmgSP<cmg::TagManager>& tagManager);
    // Run the program.
    bool run(const cmgVector<std::pair<FilePath, FilePath>>& filePairs);
    // Register comparator.
    void registerComparator(const std::string& name, MagicComparator& comparator);

protected:
    // Pre process the tag pairs.
    virtual void preProcessTags(const std::pair<cmgSPVector<cmg::TagInfo>, cmgSPVector<cmg::TagInfo>>& tagPairs);
    // Run actual implementation.
    virtual void runImpl(cmgVector<std::string>& nonTargetLines, cmgVector<cmg::ProtoImplTagPair>& protoImplPairs, const FilePath& headerFile, const FilePath& sourceFile) = 0;
    // Pairup prototype and implementation tags.
    virtual cmgVector<cmg::ProtoImplTagPair> setupProtoImplPairs(const std::pair<cmgSPVector<cmg::TagInfo>, cmgSPVector<cmg::TagInfo>>& tagPairs) = 0;
    // Setup function parameters.
    void setupParameters(const cmgSPVector<cmg::TagInfo>& tags, const cmg::FileCategory& fileCategory);
    // Load Code.
    void loadCode(cmgVector<std::string>& nonTargetLines, cmgVector<cmg::ProtoImplTagPair>& protoImplPairs, const FilePath& headerFile, const FilePath& sourceFile);
    // Write the code.
    void writeCode(cmgVector<std::string>& nonTargetLines, cmgVector<cmg::ProtoImplTagPair>& protoImplPairs, const FilePath& headerFile, const FilePath& sourceFile);
    // Fill empty implementations.
    void fillEmptyImpl(cmgVector<cmg::ProtoImplTagPair>& protoImplPairs);

private:
    std::ofstream getFileStream(const FilePath& headerFile, const FilePath& sourceFile);

protected:
    // Store the settings.
    cmg::Settings mSettings;
    // Store the Tag Manager.
    cmgSP<cmg::TagManager> mTagManager;
    // Store the active comparator.
    MagicComparator mActiveComparator{nullptr};
    // Store the comparator map.
    cmgMap<std::string, cmg::MagicComparator> mComparatorMap;
};
} // namespace cmg
