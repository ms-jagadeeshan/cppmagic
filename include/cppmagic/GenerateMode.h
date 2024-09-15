#pragma once

#include "cppmagic/MagicModeBase.h"

namespace cmg
{
class GenerateMode : public MagicModeBase
{
public:
    // Contructor.
    GenerateMode() = default;
    GenerateMode(const cmg::Settings& settings, const cmgSP<cmg::TagManager>& tagManager = nullptr);

private:
    void runImpl(cmgVector<std::string>& nonTargetLines, cmgVector<cmg::ProtoImplTagPair>& protoImplPairs, const FilePath& headerFile, const FilePath& sourceFile) override;
    void preProcessTags(const std::pair<cmgSPVector<cmg::TagInfo>, cmgSPVector<cmg::TagInfo>>& tagPairs) override;
    cmgVector<cmg::ProtoImplTagPair> setupProtoImplPairs(const std::pair<cmgSPVector<cmg::TagInfo>, cmgSPVector<cmg::TagInfo>>& tagPairs) override;

private:
};
} // namespace cmg
