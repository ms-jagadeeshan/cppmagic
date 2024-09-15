#pragma once

#include <string>
#include <unordered_map>

namespace cmg
{
enum class TagKind
{
    // Function
    PROTOTYPE,
    FUNCTION,
    PARAMETER,
    // Enum
    ENUM,
    ENUM_MEMBER,
    // Others
    MEMBER,
    CLASS,
    NAMESPACE,
    TYPEDEF,
    VARIABLE,
    LOCAL_VARIABLE,
    TPARAM,
    // Extras
    HEADER,
    FILE,
    ANY,
    UNKNOWN
};

const static std::unordered_map<std::string, TagKind> tagKindStrMap = {
    {"prototype", TagKind::PROTOTYPE},
    {"function", TagKind::FUNCTION},
    {"parameter", TagKind::PARAMETER},
    {"enum", TagKind::ENUM},
    {"enumerator", TagKind::ENUM_MEMBER},
    {"member", TagKind::MEMBER},
    {"class", TagKind::CLASS},
    {"namespace", TagKind::NAMESPACE},
    {"typedef", TagKind::TYPEDEF},
    {"variable", TagKind::VARIABLE},
    {"local", TagKind::LOCAL_VARIABLE},
    {"tparam", TagKind::TPARAM},
    {"header", TagKind::HEADER},
    {"file", TagKind::FILE},
    {"unknown", TagKind::UNKNOWN}};

const static std::unordered_map<TagKind, std::string> tagStrKindMap = {
    {TagKind::PROTOTYPE, "prototype"},
    {TagKind::FUNCTION, "function"},
    {TagKind::PARAMETER, "parameter"},
    {TagKind::ENUM, "enum"},
    {TagKind::ENUM_MEMBER, "enumerator"},
    {TagKind::MEMBER, "member"},
    {TagKind::CLASS, "class"},
    {TagKind::NAMESPACE, "namespace"},
    {TagKind::TYPEDEF, "typedef"},
    {TagKind::VARIABLE, "variable"},
    {TagKind::LOCAL_VARIABLE, "local"},
    {TagKind::TPARAM, "tparam"},
    {TagKind::HEADER, "header"},
    {TagKind::FILE, "file"},
    {TagKind::UNKNOWN, "unknown"}};

static inline TagKind tagKindStrToEnum(const std::string& str)
{
    if (str.empty())
        return TagKind::UNKNOWN;

    auto itr = tagKindStrMap.find(str);
    if (itr == tagKindStrMap.end())
        return TagKind::UNKNOWN;
    return itr->second;
}

static inline std::string tagKindEnumToStr(const TagKind& kind)
{
    auto itr = tagStrKindMap.find(kind);
    if (itr == tagStrKindMap.end())
        return "unknown";
    return itr->second;
}

enum class ScopeKind
{
    CLASS,
    NAMESPACE,
    ENUM,
    PROTOTYPE,
    UNKNOWN
};

const static inline std::unordered_map<std::string, ScopeKind> scopeKindStrMap = {
    {"class", ScopeKind::CLASS},
    {"namespace", ScopeKind::NAMESPACE},
    {"enum", ScopeKind::ENUM},
    {"prototype", ScopeKind::PROTOTYPE},
    {"unknown", ScopeKind::UNKNOWN}};

const static inline std::unordered_map<ScopeKind, std::string> scopeStrKindMap = {
    {ScopeKind::CLASS, "class"},
    {ScopeKind::NAMESPACE, "namespace"},
    {ScopeKind::ENUM, "enum"},
    {ScopeKind::PROTOTYPE, "prototype"},
    {ScopeKind::UNKNOWN, "unknown"}};

static inline ScopeKind scopeKindStrToEnum(const std::string& str)
{
    if (str.empty())
        return ScopeKind::UNKNOWN;

    auto itr = scopeKindStrMap.find(str);
    if (itr == scopeKindStrMap.end())
        return ScopeKind::UNKNOWN;

    return itr->second;
}

static inline std::string scopeKindEnumToStr(const ScopeKind& kind)
{
    auto itr = scopeStrKindMap.find(kind);
    if (itr == scopeStrKindMap.end())
        return "unknown";

    return itr->second;
}

} // namespace cmg
