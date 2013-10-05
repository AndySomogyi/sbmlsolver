#ifndef rrSelectionRecordH
#define rrSelectionRecordH
#include <string>
#include <iomanip>
#include <ostream>
#include "rrExporter.h"
#include <string>
//---------------------------------------------------------------------------
namespace rr
{
using std::string;
using std::ostream;

/**
 * a way to find sbml model elements using the RoadRunner syntax.
 */
class RR_DECLSPEC SelectionRecord
{
public:
    enum SelectionType
    {
        TIME = 0,
        BOUNDARY_CONCENTRATION,
        FLOATING_CONCENTRATION,
        REACTION_RATE,
        FLOATING_AMOUNT_RATE,
        COMPARTMENT,
        GLOBAL_PARAMETER,
        FLOATING_AMOUNT,
        BOUNDARY_AMOUNT,
        ELASTICITY,
        UNSCALED_ELASTICITY,
        CONTROL,
        UNSCALED_CONTROL,
        EIGENVALUE,
        STOICHIOMETRY,
        UNKNOWN_ELEMENT,
        UNKNOWN_CONCENTRATION,
        UNKNOWN
    };
    int index;
    string p1;
    string p2;

    SelectionType selectionType;
    SelectionRecord(const int& index = 0,
            const SelectionType type = UNKNOWN,
            const string& p1 = "", const string& p2 = "");

    SelectionRecord(const std::string str);

    std::string to_string();

};

ostream& operator<< (ostream& stream, const SelectionRecord& rec);
}


#endif
