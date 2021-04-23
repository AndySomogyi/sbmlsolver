#ifndef rrRuleH
#define rrRuleH
#include <string>
#include "rrExporter.h"

using std::string;
namespace rr
{

enum RuleType {rtAlgebraic = 0, rtAssignment, rtRate, rtUnknown};

RuleType GetRuleTypeFromString(const std::string& str);

class RR_DECLSPEC RRRule
{
    protected:
        std::string                 mTheRule;
        std::string                 mRuleTypeStr;
        RuleType            mRuleType;
        void                AssignType();

    public:
                            RRRule(const std::string& rule, const std::string& ruleType);
        std::string                GetLHS();
        std::string                GetRHS();
        RuleType            GetType();
};


}
#endif
