#pragma once

#include <functional>
#include <string>

#include "cppmagic/TagInfo.h"

namespace cmg
{
using MagicComparator = std::function<bool(const cmg::ProtoImplTagPair&, const cmg::ProtoImplTagPair&)>;

enum class SortingMethod
{
    HEADER,
    ALPHABETICAL
};

SortingMethod sortingStrToEnum(const std::string& str);
cmgVector<std::string> sortingMethods();
std::string sortingEnumToStr(const cmg::SortingMethod& method);
MagicComparator getComparator(cmg::SortingMethod method);

} // namespace cmg
