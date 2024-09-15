#include "cppmagic/TagManager.h"

////////////////////////////////////////////////////////////////////////

bool cmg::TagManager::add(const cmgSP<cmg::TagInfo>& tag)
{
    if (tag == nullptr)
        return false;

    mFileTagsMap[tag->file().absolutePath()].push_back(tag);
    mFileTagKindMap[tag->mFilePath][tag->mTagKind].push_back(tag);
    return true;
}

////////////////////////////////////////////////////////////////////////

bool cmg::TagManager::add(const cmgSPVector<cmg::TagInfo>& tags)
{
    for (const auto& tag : tags)
    {
        add(tag);
    }
    return true;
}

////////////////////////////////////////////////////////////////////////

bool cmg::TagManager::addPair(const cmgSPVector<cmg::TagInfo>& headerTags, const cmgSPVector<cmg::TagInfo>& sourceTags)
{
    if (headerTags.empty())
        return false;

    auto fileStem = headerTags.front()->file().relativePathStem();

    // Check if already exist.
    if (findPair(fileStem))
        return false;

    mTagPairsMap[fileStem] = mTagPairs.size();
    mTagPairs.emplace_back(headerTags, sourceTags);

    cmgMap<cmg::TagKind, std::pair<cmgSPVector<cmg::TagInfo>, cmgSPVector<cmg::TagInfo>>> tagMap;
    for (auto headerTag : headerTags)
    {
        add(headerTag);
        tagMap[headerTag->mTagKind].first.push_back(headerTag);
    }
    for (auto sourceTag : sourceTags)
    {
        add(sourceTag);
        tagMap[sourceTag->mTagKind].second.push_back(sourceTag);
    }
    mTagPairsKindMap.emplace_back(tagMap);

    return true;
}

////////////////////////////////////////////////////////////////////////

bool cmg::TagManager::addPairs(const cmgVector<std::pair<cmgSPVector<cmg::TagInfo>, cmgSPVector<cmg::TagInfo>>> tagPairs)
{
    for (auto tagPair : tagPairs)
    {
        addPair(tagPair.first, tagPair.second);
    }
    return true;
}

////////////////////////////////////////////////////////////////////////

bool cmg::TagManager::findPair(const std::string& fileStem)
{
    return mTagPairsMap.find(fileStem) != mTagPairsMap.end();
}

////////////////////////////////////////////////////////////////////////

cmgSPVector<cmg::TagInfo> cmg::TagManager::getTags(const std::string& absolutePath, const cmg::TagKind& tagKind)
{
    auto itr = mFileTagsMap.find(absolutePath);
    if (itr == mFileTagsMap.end())
        return {};

    if (tagKind == cmg::TagKind::ANY)
        return mFileTagsMap[absolutePath];
    else
        return mFileTagKindMap[absolutePath][tagKind];
}

////////////////////////////////////////////////////////////////////////

std::pair<cmgSPVector<cmg::TagInfo>, cmgSPVector<cmg::TagInfo>> cmg::TagManager::getTagPairs(const std::string& fileStem, const cmg::TagKind& tagKind)
{
    auto itr = mTagPairsMap.find(fileStem);
    if (itr == mTagPairsMap.end())
        return {};

    if (tagKind == cmg::TagKind::ANY)
        return {mTagPairs[itr->second].first, mTagPairs[itr->second].second};
    else
        return {mTagPairsKindMap[itr->second][tagKind].first, mTagPairsKindMap[itr->second][tagKind].second};
}

////////////////////////////////////////////////////////////////////////
