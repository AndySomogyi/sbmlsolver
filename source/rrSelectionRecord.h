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
		INITIAL_VALUE,
        INITIAL_CONCENTRATION,
        STOICHIOMETRY,
        UNKNOWN_ELEMENT,
        UNKNOWN_CONCENTRATION,
        UNKNOWN
    };

    /**
     * After the selection record is created by the roadrunner object, this
     * is an index into the variables list that the ExecutableModel has.
     */
    int index;

    /**
     * For single argument selections, i.e. ee( S1 ), this is the argument, for
     * two arg selections, this is the first arg.
     */
    string p1;

    /**
     * the second argument of two arg selections.
     */
    string p2;

    /**
     * the type of selection.
     */
    SelectionType selectionType;

    /**
     * Selections really should be constructed by the RoadRunner::createSelection
     * method.
     *
     * This just creates a new ebmpy selection.
     */
    SelectionRecord(const int& index = 0,
            const SelectionType type = UNKNOWN,
            const string& p1 = "", const string& p2 = "");

    /**
     * creates a empty selection.
     */
    SelectionRecord(const std::string str);

    /**
     * get a short description of this selection, this returns a string
     * compatable with RoadRunner::createSelection.
     */
    std::string to_string() const;

    /**
     * gets a longer description of this selection, this is intended
     * to called as a python __repr__
     */
    std::string to_repr() const;
};

ostream& operator<< (ostream& stream, const SelectionRecord& rec);
}


#endif
