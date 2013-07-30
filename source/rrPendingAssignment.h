#ifndef rrPendingAssignmentH
#define rrPendingAssignmentH
#include <vector>
#include "rrObject.h"
#include "rrModelData.h"

namespace rr
{

class RR_DECLSPEC PendingAssignment
{
protected:
    SModelData*                       mModelData;
    double                            Time;
    int                               Index;
    bool                              UseValuesFromTriggerTime;
    ComputeEventAssignmentHandler     ComputeAssignment;
    PerformEventAssignmentHandler     PerformAssignment;

public:
    double*                           ComputedValues;

    /**
     *  Initializes a new instance of the PendingAssignment class.
     * @param name="time"
     */
    PendingAssignment(SModelData* md, double time,
            ComputeEventAssignmentHandler computeAssignment,
            PerformEventAssignmentHandler performAssignment,
            bool useValuesFromTriggerTime,
            int index);
    int                               GetIndex();
    double                            GetTime();
    void                              AssignToModel();
};

}
#endif
