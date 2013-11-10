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
        TIME =                              (0x1 << 0),  // => 0x00000001,
        BOUNDARY_CONCENTRATION =            (0x1 << 1),  // => 0x00000001,
        FLOATING_CONCENTRATION =            (0x1 << 2),  // =>
        REACTION_RATE =                     (0x1 << 3),  // =>
        FLOATING_AMOUNT_RATE =              (0x1 << 4),  // =>
        FLOATING_CONCENTRATION_RATE =       (0x1 << 5),  // =>
        COMPARTMENT =                       (0x1 << 6),  // =>
        GLOBAL_PARAMETER =                  (0x1 << 7),  // =>
        FLOATING_AMOUNT =                   (0x1 << 8),  // =>
        BOUNDARY_AMOUNT =                   (0x1 << 9),  // =>
        ELASTICITY =                        (0x1 << 10), // =>
        UNSCALED_ELASTICITY =               (0x1 << 11), // =>
        CONTROL =                           (0x1 << 12), // =>
        UNSCALED_CONTROL =                  (0x1 << 13), // =>
        EIGENVALUE =                        (0x1 << 14), // =>
        INITIAL_FLOATING_AMOUNT =           (0x1 << 15), // =>
        INITIAL_FLOATING_CONCENTRATION =    (0x1 << 16), // =>
        STOICHIOMETRY =                     (0x1 << 17), // =>
        UNKNOWN_ELEMENT =                   (0x1 << 18), // =>
        UNKNOWN_CONCENTRATION =             (0x1 << 19), // =>
        UNKNOWN =                           (0x1 << 20)  // =>
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
