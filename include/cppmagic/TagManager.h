#pragma once

#include "cppmagic/MemoryAlias.h"
#include "cppmagic/TagInfo.h"

namespace cmg
{
class TagManager
{
public:
    TagManager() = default;

    // Add any tags based on absolute path.
    bool add(const cmgSP<cmg::TagInfo>& tag);
    bool add(const cmgSPVector<cmg::TagInfo>& tags);

    // Add pair tags based on relative path stem(One time addition).
    bool addPair(const cmgSPVector<cmg::TagInfo>& headerTags, const cmgSPVector<cmg::TagInfo>& sourceTags);
    bool addPairs(const cmgVector<std::pair<cmgSPVector<cmg::TagInfo>, cmgSPVector<cmg::TagInfo>>> tagPairs);

    // Find the tag pairs.
    bool findPair(const std::string& fileStem);

    // Get the tags based on absolute path.
    cmgSPVector<cmg::TagInfo> getTags(const std::string& absolutePath, const cmg::TagKind& tagKind = cmg::TagKind::ANY);
    // Get the tag pairs based on relative path stem.
    std::pair<cmgSPVector<cmg::TagInfo>, cmgSPVector<cmg::TagInfo>> getTagPairs(const std::string& fileStem, const cmg::TagKind& tagKind = cmg::TagKind::ANY);

private:
    // Any tags.
    // mFileTagsMap[absolutePath] = Tags.
    cmgMap<std::string, cmgSPVector<cmg::TagInfo>> mFileTagsMap;
    // mFileTagKindMap[absolutePath][tagKind] = tags.
    cmgMap<std::string, cmgMap<cmg::TagKind, cmgSPVector<cmg::TagInfo>>> mFileTagKindMap;

    // Pair tags.
    // var[relativeStem]= index.
    cmgMap<std::string, int> mTagPairsMap;
    // var[index]={headerTags,sourceTags}
    cmgVector<std::pair<cmgSPVector<cmg::TagInfo>, cmgSPVector<cmg::TagInfo>>> mTagPairs;
    // var[index][kind] = {headerTags,sourceTags}
    cmgVector<cmgMap<cmg::TagKind, std::pair<cmgSPVector<cmg::TagInfo>, cmgSPVector<cmg::TagInfo>>>> mTagPairsKindMap;
};

} // namespace cmg
