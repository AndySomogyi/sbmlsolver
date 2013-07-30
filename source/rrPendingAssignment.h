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

//namespace LibRoadRunner
//{
//    internal class PendingAssignment
//    {
//        /// <summary>
//        /// Initializes a new instance of the PendingAssignment class.
//        /// </summary>
//        /// <param name="time"></param>
//        public PendingAssignment(double time, ComputeEventAssignmentHandler computeAssignment,
//                                 PerformEventAssignmentHandler performAssignment, bool useValuesFromTriggerTime, int index)
//        {
//            Time = time;
//            ComputeAssignment = computeAssignment;
//            PerformAssignment = performAssignment;
//            Index = index;
//            UseValuesFromTriggerTime = useValuesFromTriggerTime;
//            if (useValuesFromTriggerTime)
//                ComputedValues = computeAssignment();
//        }
//
//        public int Index { get; set; }
//
//        public double Time { get; set; }
//
//        public double[] ComputedValues { get; set; }
//        public ComputeEventAssignmentHandler ComputeAssignment { get; set; }
//        public PerformEventAssignmentHandler PerformAssignment { get; set; }
//        public bool UseValuesFromTriggerTime { get; set; }
//
//        public void AssignToModel()
//        {
//            if (!UseValuesFromTriggerTime)
//                ComputedValues = ComputeAssignment();
//            PerformAssignment(ComputedValues);
//        }
//    }
//}

