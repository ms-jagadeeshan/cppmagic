#include "cppmagic/TagInfo.h"

#include <regex>

#include "cppmagic/Logger.h"
#include "cppmagic/Utils.h"

////////////////////////////////////////////////////////////////////////

const cmgVector<std::string> cmg::TagInfo::mDefQualifier = {"0", "default", "delete"};
const cmgVector<std::string> cmg::TagInfo::mAllowedQualifier = {"final", "override", "noexcept", "const"};

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
        auto qualifier = extractQualifier();
        // If qualifier any of delete, default, 0, then don't build c++ code.
        LOG_DEBUG("Symbol : %s , qualifier : %s", scopedName().c_str(), qualifier.c_str());
        if (cmg::utils::contains(mDefQualifier, qualifier))
        {
            LOG_DEBUG("Function '%s' declaration contains qualifier '%s', so not building code.", scopedName().c_str(), qualifier.c_str());
            mStr = "";
            return;
        }
        // If qualifier is final, override, const, noexcept, then allow.
        if (cmg::utils::contains(mAllowedQualifier, qualifier))
            qualifier = " " + qualifier;
        else
            qualifier = "";

        cmgVector<std::string> paramStr{};
        for (auto parameter : mParameters)
            paramStr.push_back(parameter->mTypeRef + " " + parameter->mSymbolName);

        mStr = mTypeRef + " " + this->scopedName()
               + "(" + cmg::utils::join(paramStr, ", ") + ")"
               + qualifier + "\n{\n}\n";
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
    static std::regex pattern(R"(\)\s*=?\s*(\w+|\d+)\s*;\s*\$\/$)");
    std::smatch match;

    std::string qualifier = "";
    if (std::regex_search(mPattern, match, pattern))
        qualifier = match[1];
    return qualifier;
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
