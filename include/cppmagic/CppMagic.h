#pragma once

#include "cppmagic/Core.h"
#include "cppmagic/MagicModeBase.h"
#include "cppmagic/Settings.h"
#include "cppmagic/TagManager.h"

namespace cmg
{
class CppMagic
{
public:
    static cmgSP<cmg::MagicModeBase> getMagicMode(const cmg::Settings& settings, const cmgSP<cmg::TagManager>& tagManager = nullptr);

private:
    static cmgSP<cmg::MagicModeBase> getMagicMode(const cmg::MagicMode& mode);
};
} // namespace cmg
