#pragma once

#include "cppmagic/MagicModeBase.h"

namespace cmg
{
class SortMode : public MagicModeBase
{
public:
    // Contructor.
    SortMode() = default;
    SortMode(const cmg::Settings& settings, const cmgSP<cmg::TagManager>& tagManager = nullptr);

private:
    void preProcessTags(const std::pair<cmgSPVector<cmg::TagInfo>, cmgSPVector<cmg::TagInfo>>& tagPairs) override;
    void runImpl(cmgVector<std::string>& nonTargetLines, cmgVector<cmg::ProtoImplTagPair>& protoImplPairs, const FilePath& headerFile, const FilePath& sourceFile) override;
    cmgVector<cmg::ProtoImplTagPair> setupProtoImplPairs(const std::pair<cmgSPVector<cmg::TagInfo>, cmgSPVector<cmg::TagInfo>>& tagPairs) override;

private:
};

} // namespace cmg