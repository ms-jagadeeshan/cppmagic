#include "cppmagic/TagInfo.h"

#include "cppmagic/Utils.h"

////////////////////////////////////////////////////////////////////////

std::string cmg::TagInfo::tagKindStr() const
{
    return cmg::tagKindEnumToStr(mTagKind);
}

////////////////////////////////////////////////////////////////////////

std::string cmg::TagInfo::scopeKindStr() const
{
    return cmg::scopeKindEnumToStr(mScopeKind);
}

////////////////////////////////////////////////////////////////////////

void cmg::TagInfo::serializeParameters()
{
    if (mParameters.empty())
        return;

    std::sort(mParameters.begin(), mParameters.end(), cmg::TagInfoParameterComparator());
    cmgVector<std::string> parameters;
    for (auto param : mParameters)
    {
        parameters.push_back(param->mTypeRef);
    }
    mParametersStr = cmg::utils::join(parameters, ",");
}

////////////////////////////////////////////////////////////////////////

void cmg::TagInfo::buildCodeStr()
{
    if (mTagKind == cmg::TagKind::FUNCTION)
    {
        cmgVector<std::string> paramStr{};
        for (auto parameter : mParameters)
            paramStr.push_back(parameter->mTypeRef + " " + parameter->mSymbolName);

        mStr = mTypeRef + " " + this->scopedName()
               + "(" + cmg::utils::join(paramStr, ", ") + ")"
               + this->extractQualifier() + "\n{\n}\n";
    }
}

////////////////////////////////////////////////////////////////////////

int cmg::TagInfo::parametersCount() const
{
    return mParameters.size();
}

////////////////////////////////////////////////////////////////////////

std::string cmg::TagInfo::scopedName()
{
    if (!mScope.empty())
        return mScope + "::" + mSymbolName;
    else
        return mSymbolName;
}

////////////////////////////////////////////////////////////////////////

std::string cmg::TagInfo::extractQualifier() const
{
    return "";
}

////////////////////////////////////////////////////////////////////////

const FilePath& cmg::TagInfo::file() const
{
    return mFile;
}

////////////////////////////////////////////////////////////////////////

bool cmg::TagInfo::equals(const cmgSP<TagInfo>& other) const
{
    if (other == nullptr)
        return false;

    return equals(*other);
}

////////////////////////////////////////////////////////////////////////

bool cmg::TagInfo::equals(const TagInfo& other) const
{
    return mSymbolName == other.mSymbolName && mTypeRef == other.mTypeRef && mScope == other.mScope && parametersCount() == other.parametersCount() && mParametersStr == other.mParametersStr;
}

////////////////////////////////////////////////////////////////////////

bool cmg::TagInfo::equals(const TagInfo* other) const
{
    if (other == nullptr)
        return false;
    return equals(*other);
}

////////////////////////////////////////////////////////////////////////
