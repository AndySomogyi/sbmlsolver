#pragma hdrstop
#include "rrSelectionRecord.h"

#include <vector>

// TODO When we have gcc 4.4 as minimal compiler, drop poco and use C++ standard regex
#include <Poco/RegularExpression.h>

using Poco::RegularExpression;

static bool is_time(const std::string& str)
{
    Poco::RegularExpression re("\\s*time\\s*", RegularExpression::RE_CASELESS);

    return re.match(str);
}

static bool is_uec(const std::string& str, std::string& p1, std::string& p2)
{
    Poco::RegularExpression re("\\s*uec\\s*\\(\\s*(\\w*)\\s*,\\s*(\\w*)\\s*\\)\\s*", RegularExpression::RE_CASELESS);

    std::vector<std::string> matches;


    int nmatch = re.split(str, matches);

    if (nmatch > 0)
    {
        p1 = matches[0];
        p2 = matches[1];
        return true;
    }
    else
    {
        return false;
    }
}

namespace rr
{

using namespace std;



SelectionRecord::SelectionRecord(const int& _index,
        const SelectionType _type, const string& _p1,
        const string& _p2) :
        index(_index), p1(_p1), p2(_p2), selectionType(_type)
{
}

ostream& operator<<(ostream& stream, const SelectionRecord& rec)
{
    stream << "A Selection Record --" << endl;
    stream << "Index: " << rec.index << endl;
    stream << "p1: " << rec.p1 << endl;
    stream << "p2: " << rec.p1 << endl;
    stream << "SelectionType: " << rec.selectionType << endl;
    return stream;
}

}

rr::SelectionRecord::SelectionRecord(const std::string str)
{
    if (is_time(str)) {

    }
    else if(is_uec(str, p1, p2))
    {

    }
}

std::string rr::SelectionRecord::to_string()
{
    return "";
}
