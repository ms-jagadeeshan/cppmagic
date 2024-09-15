#include "cppmagic/CppMagic.h"

#include "cppmagic/BoomMode.h"
#include "cppmagic/GenerateMode.h"
#include "cppmagic/SortMode.h"

////////////////////////////////////////////////////////////////////////

cmgSP<cmg::MagicModeBase> cmg::CppMagic::getMagicMode(const cmg::Settings& settings, const cmgSP<cmg::TagManager>& tagManager)
{
    auto mode = settings.mMagicMode;
    cmgSP<cmg::MagicModeBase> magic;
    switch (mode)
    {
    case cmg::MagicMode::GENERATE:
        magic = std::make_shared<cmg::GenerateMode>(settings, tagManager);
        break;
    case cmg::MagicMode::SORT:
        magic = std::make_shared<cmg::SortMode>(settings, tagManager);
        break;
    case cmg::MagicMode::BOOM:
        magic = std::make_shared<cmg::BoomMode>(settings, tagManager);
        break;
    default:
        magic = std::make_shared<cmg::BoomMode>(settings, tagManager);
        break;
    }
    return magic;
}

////////////////////////////////////////////////////////////////////////

cmgSP<cmg::MagicModeBase> cmg::CppMagic::getMagicMode(const cmg::MagicMode& mode)
{
    cmgSP<cmg::MagicModeBase> magic;
    switch (mode)
    {
    case cmg::MagicMode::GENERATE:
        magic = std::make_shared<cmg::GenerateMode>();
        break;
    case cmg::MagicMode::SORT:
        break;
    case cmg::MagicMode::BOOM:
        break;
    default:
        break;
    }
    magic = std::make_shared<cmg::GenerateMode>();
    return magic;
}

////////////////////////////////////////////////////////////////////////
