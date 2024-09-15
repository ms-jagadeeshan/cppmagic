#pragma once

#include <algorithm>
#include <string>

#include "cppmagic/FilePath.h"
#include "cppmagic/MemoryAlias.h"
#include "cppmagic/TagKinds.h"

namespace cmg
{
struct TagInfo
{
public:
    // Get the tag kind as string.
    std::string tagKindStr() const;
    // Get the scope kind as string.
    std::string scopeKindStr() const;
    // Serialize parameters comma seperated string.
    void serializeParameters();
    // Build code template.
    void buildCodeStr();
    // Get the parameters count.
    int parametersCount() const;
    // Get the scoped name.
    std::string scopedName();
    // Extract qualifier.
    std::string extractQualifier() const;
    // Get the file path of the tag.
    const FilePath& file() const;
    // Check if both are equivalent(prototype and implementation).
    bool equals(const cmgSP<TagInfo>& other) const;
    bool equals(const TagInfo& other) const;
    bool equals(const TagInfo* other) const;

public:
    // Name of symbol.
    std::string mSymbolName;
    // Store the file path.
    std::string mFilePath;
    // Store the file.
    FilePath mFile{""};
    // Store the starting line of symbol.
    int mStartLine = -1;
    // Store the ending line of symbol.
    int mEndLine = -1;
    // Store the tag kind.
    TagKind mTagKind;
    // Store the return type.
    std::string mTypeRef;
    // Store the access type.
    std::string mAccessType;
    // Store the scope.
    std::string mScope;
    // Store the scope kind. i.e class, function.
    ScopeKind mScopeKind;
    // Store the store the pattern.
    std::string mPattern;
    // Store the signature.
    std::string mSignature;
    // Store the extras.
    std::string mExtras;
    // Store the code.
    std::string mStr;
    // Used if the tag kind is parameter.
    int mNth = -1;
    // Store the paramters in order(function , prototype).
    cmgSPVector<cmg::TagInfo> mParameters;
    // Store the parameters string(comma seperated).
    std::string mParametersStr;

    // Store the defining qualifier.
    const static cmgVector<std::string> mDefQualifier;
    // Store the allowd qualifier.
    const static cmgVector<std::string> mAllowedQualifier;
};

struct TagInfoParameterComparator
{
    bool operator()(const cmgSP<TagInfo>& a, const cmgSP<TagInfo>& b) const
    {
        return a->mNth < b->mNth;
    }
};

struct TagInfoStartLineComparator
{
    bool operator()(const cmgSP<TagInfo>& a, const cmgSP<TagInfo>& b) const
    {
        return a->mStartLine < b->mStartLine;
    }
};

struct ProtoImplTagPair
{
public:
    ProtoImplTagPair(const cmgSP<cmg::TagInfo>& prototype, const cmgSP<cmg::TagInfo>& implementation)
        : mPrototype(prototype), mImplementation(implementation)
    {
    }

public:
    cmgSP<cmg::TagInfo> mPrototype;
    cmgSP<cmg::TagInfo> mImplementation;
};

struct TagPairImplStartLineComparator
{
    bool operator()(const cmgSP<ProtoImplTagPair>& a, const cmgSP<ProtoImplTagPair>& b) const
    {
        return a->mImplementation->mStartLine < b->mImplementation->mStartLine;
    }

    bool operator()(const cmg::ProtoImplTagPair& a, const cmg::ProtoImplTagPair& b) const
    {
        return a.mImplementation->mStartLine < b.mImplementation->mStartLine;
    }
};

struct TagPairProtoStartLineComparator
{
    bool operator()(const cmgSP<ProtoImplTagPair>& a, const cmgSP<ProtoImplTagPair>& b) const
    {
        return a->mPrototype->mStartLine < b->mPrototype->mStartLine;
    }

    bool operator()(const cmg::ProtoImplTagPair& a, const cmg::ProtoImplTagPair& b) const
    {
        return a.mPrototype->mStartLine < b.mPrototype->mStartLine;
    }
};
} // namespace cmg
