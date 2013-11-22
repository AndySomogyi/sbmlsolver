#ifndef rrPendingAssignmentH
#define rrPendingAssignmentH
#include <vector>
#include "rrExporter.h"
#include "rrModelData.h"

namespace rr
{

/**
 * Holds a handle to a block of memory allocated by the model, the model
 * is also responsible for freeing this block.
 *
 * Therefore, this class can not be copied, the copy constructors are
 * actually a move, so that these can be stored in a vector.
 */
class RR_DECLSPEC PendingAssignment
{
public:

    /**
     *  Initializes a new instance of the PendingAssignment class.
     * @param name="time"
     */
    PendingAssignment(ModelData* md, double time,
            ComputeEventAssignmentHandler computeAssignment,
            PerformEventAssignmentHandler performAssignment,
            bool useValuesFromTriggerTime, int index,
            double *preComputedValues);

    ~PendingAssignment();

    PendingAssignment() ;

    /**
     * move all the fields to the new PendingAssigment, the current
     * fields are zeroed out.
     */
    PendingAssignment( const PendingAssignment& other );

    /**
     * same as copy ctor.
     */
    PendingAssignment& operator=( const PendingAssignment& );

    void clear();

    int getIndex();
    double getTime();
    void eval();

private:
    double* computedValues;
    ModelData* modelData;
    double time;
    int index;
    bool useValuesFromTriggerTime;
    ComputeEventAssignmentHandler computeAssignment;
    PerformEventAssignmentHandler performAssignment;

};

}
#endif
