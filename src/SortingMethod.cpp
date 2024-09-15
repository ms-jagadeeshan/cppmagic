#include "cppmagic/SortingMethod.h"

////////////////////////////////////////////////////////////////////////

namespace cmg
{
const static inline cmgUOMap<std::string, SortingMethod> sortingStrEnumMap = {
    {"header", SortingMethod::HEADER},
    {"alpha", SortingMethod::ALPHABETICAL}};

const static inline cmgUOMap<SortingMethod, std::string> sortingEnumStrMap = {
    {SortingMethod::HEADER, "header"},
    {SortingMethod::ALPHABETICAL, "alpha"}};

const static inline cmgUOMap<SortingMethod, MagicComparator> sortingComparatorMap = {
    {SortingMethod::HEADER, [](const ProtoImplTagPair& a, const ProtoImplTagPair& b) {
         return a.mPrototype->mStartLine < b.mPrototype->mStartLine;
     }},
    {SortingMethod::ALPHABETICAL, [](const ProtoImplTagPair& f1, const ProtoImplTagPair& f2) {
         if (f1.mImplementation->mSymbolName == f2.mImplementation->mSymbolName)
         {
             return f1.mImplementation->mTypeRef < f2.mImplementation->mTypeRef;
         }
         return f1.mImplementation->mSymbolName < f2.mImplementation->mSymbolName;
     }}};

} // namespace cmg

////////////////////////////////////////////////////////////////////////

cmg::SortingMethod cmg::sortingStrToEnum(const std::string& str)
{
    if (str.empty())
        return cmg::SortingMethod::HEADER;

    auto itr = cmg::sortingStrEnumMap.find(str);
    if (itr == cmg::sortingStrEnumMap.end())
        return cmg::SortingMethod::HEADER;

    return itr->second;
}

////////////////////////////////////////////////////////////////////////

cmgVector<std::string> cmg::sortingMethods()
{
    cmgVector<std::string> methods{};
    for (const auto& [_, value] : cmg::sortingEnumStrMap)
        methods.push_back(value);
    return methods;
}

////////////////////////////////////////////////////////////////////////

std::string cmg::sortingEnumToStr(const cmg::SortingMethod& method)
{
    auto itr = cmg::sortingEnumStrMap.find(method);
    if (itr == cmg::sortingEnumStrMap.end())
        return "header";

    return itr->second;
}

////////////////////////////////////////////////////////////////////////

cmg::MagicComparator cmg::getComparator(cmg::SortingMethod method)
{
    auto itr = sortingComparatorMap.find(method);
    if (itr != sortingComparatorMap.end())
    {
        return itr->second;
    }
    return sortingComparatorMap.at(cmg::SortingMethod::HEADER);
}

////////////////////////////////////////////////////////////////////////
