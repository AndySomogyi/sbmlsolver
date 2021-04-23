#pragma hdrstop
#include "rrRule.h"
//---------------------------------------------------------------------------




namespace rr
{

RRRule::RRRule(const std::string& rule, const std::string& ruleTypeStr)
:
mTheRule(rule),
mRuleTypeStr(ruleTypeStr),
mRuleType(rtUnknown)
{
    AssignType();
}

std::string RRRule::GetLHS()
{
    //Rules have equal signs, or?
    std::string lhs = mTheRule.substr(0, mTheRule.find('='));
    return lhs;
}

std::string RRRule::GetRHS()
{
    std::string rhs = mTheRule.substr(mTheRule.find('=') + 1);
    return rhs;
}

RuleType RRRule::GetType()
{
    return mRuleType;
}

void RRRule::AssignType()
{
    mRuleType = GetRuleTypeFromString(mRuleTypeStr);
}

RuleType GetRuleTypeFromString(const std::string& str)
{
    if(str == "Algebraic_Rule")
    {
        return rtAlgebraic;
    }
    else if(str == "Assignment_Rule")
    {
        return rtAssignment;
    }
    else if(str == "Rate_Rule")
    {
        return rtRate;
    }
    else
    {
        return rtUnknown;
    }
}

}
