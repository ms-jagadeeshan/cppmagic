#include "cppmagic/BoomMode.h"

#include "cppmagic/TagInfo.h"
#include "cppmagic/TagUtility.h"

////////////////////////////////////////////////////////////////////////

cmg::BoomMode::BoomMode(const cmg::Settings& settings, const cmgSP<cmg::TagManager>& tagManager)
    : MagicModeBase(settings, tagManager)
{
}

////////////////////////////////////////////////////////////////////////

void cmg::BoomMode::preProcessTags(const std::pair<cmgSPVector<cmg::TagInfo>, cmgSPVector<cmg::TagInfo>>& tagPairs)
{
    setupParameters(tagPairs.first, cmg::FileCategory::Header);
    setupParameters(tagPairs.second, cmg::FileCategory::Source);
}

////////////////////////////////////////////////////////////////////////

cmgVector<cmg::ProtoImplTagPair> cmg::BoomMode::setupProtoImplPairs(const std::pair<cmgSPVector<cmg::TagInfo>, cmgSPVector<cmg::TagInfo>>& tagPairs)
{
    cmgVector<cmg::ProtoImplTagPair> protoImplPairs;
    auto prototypeTags = cmg::TagUtility::filter(tagPairs.first, {cmg::TagKind::PROTOTYPE});
    auto implTags = cmg::TagUtility::filter(tagPairs.second, {cmg::TagKind::FUNCTION});

    cmgUOMap<std::string, cmgSPVector<cmg::TagInfo>> implTagMap;
    for (const auto& tag : implTags)
        implTagMap[tag->scopedName()].push_back(tag);

    for (auto prototypeTag : prototypeTags)
    {
        cmgSP<cmg::TagInfo> implTag = nullptr;
        auto possibleOverloads = implTagMap[prototypeTag->scopedName()];
        for (auto possibleOverload : possibleOverloads)
        {
            if (prototypeTag->equals(possibleOverload))
            {
                implTag = possibleOverload;
                break;
            }
        }
        protoImplPairs.emplace_back(prototypeTag, implTag);
    }
    fillEmptyImpl(protoImplPairs);
    return protoImplPairs;
}

////////////////////////////////////////////////////////////////////////

void cmg::BoomMode::runImpl(cmgVector<std::string>& nonTargetLines, cmgVector<cmg::ProtoImplTagPair>& protoImplPairs, const FilePath& headerFile, const FilePath& sourceFile)
{
    if (protoImplPairs.empty())
        return;

    std::sort(protoImplPairs.begin(), protoImplPairs.end(), cmg::TagPairImplStartLineComparator());

    loadCode(nonTargetLines, protoImplPairs, headerFile, sourceFile);
    if (mActiveComparator == nullptr)
    {
        LOG_ERROR("No comparator set!");
        return;
    }

    std::sort(protoImplPairs.begin(), protoImplPairs.end(), mActiveComparator);
}

////////////////////////////////////////////////////////////////////////
