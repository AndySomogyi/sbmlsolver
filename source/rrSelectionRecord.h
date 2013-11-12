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
        CONCENTRATION =                     (0x1 << 1),
        AMOUNT =                            (0x1 << 2),
        RATE =                              (0x1 << 3),
        BOUNDARY =                          (0x1 << 4),
        FLOATING =                          (0x1 << 5),
        COMPARTMENT =                       (0x1 << 6),
        GLOBAL_PARAMETER =                  (0x1 << 7),
        REACTION =                          (0x1 << 8),
        INITIAL =                           (0x1 << 9),
        UNSCALED =                          (0x1 << 10),
        ELASTICITY =                        (0x1 << 11),
        CONTROL =                           (0x1 << 12),
        EIGENVALUE =                        (0x1 << 13),
        ELEMENT =                           (0x1 << 14),
        STOICHIOMETRY =                     (0x1 << 15),
        UNKNOWN =                           (0x1 << 16),
        DEPENDENT =                         (0x1 << 17),
        INDEPENDENT =                       (0x1 << 18),
        CONSREVED_MOIETY =                  (0x1 << 19) | DEPENDENT, // are by definition dependent.

        UNKNOWN_CONCENTRATION =             UNKNOWN | CONCENTRATION,


        FLOATING_AMOUNT =                   FLOATING | AMOUNT,
        BOUNDARY_AMOUNT =                   BOUNDARY | AMOUNT,

        BOUNDARY_CONCENTRATION =            BOUNDARY | CONCENTRATION,
        FLOATING_CONCENTRATION =            FLOATING | CONCENTRATION,
        FLOATING_AMOUNT_RATE =              FLOATING | RATE,
        FLOATING_CONCENTRATION_RATE =       FLOATING | CONCENTRATION | RATE,
        REACTION_RATE =                     REACTION | RATE,
        INITIAL_FLOATING_AMOUNT =           INITIAL | FLOATING,
        INITIAL_FLOATING_CONCENTRATION =    INITIAL | FLOATING | CONCENTRATION,
        UNSCALED_ELASTICITY =               UNSCALED | ELASTICITY,
        UNSCALED_CONTROL =                  UNSCALED | CONTROL,
        UNKNOWN_ELEMENT =                   UNKNOWN | ELEMENT
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
