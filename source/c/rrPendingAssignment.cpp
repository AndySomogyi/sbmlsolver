#pragma hdrstop
#include "rrPendingAssignment.h"
#include "rrLogger.h"
#include <assert.h>



namespace rr
{

using namespace std;

PendingAssignment::PendingAssignment(
        ModelData* md,
        double time,
        ComputeEventAssignmentHandler computeAssignment,
        PerformEventAssignmentHandler performAssignment,
        bool useValuesFromTriggerTime,
        int index,
        double *preComputedValues) :
        computedValues(0),
        modelData(md),
        time(time),
        index(index),
        useValuesFromTriggerTime(useValuesFromTriggerTime),
        computeAssignment(computeAssignment),
        performAssignment(performAssignment)
{
    if (preComputedValues)
    {
        computedValues = preComputedValues;
    }
    else if (useValuesFromTriggerTime)
    {
        computedValues = computeAssignment(modelData);
    }
    else
    {
        computedValues = 0;
    }
}

PendingAssignment::PendingAssignment()
{
    computedValues = 0;
    modelData = 0;
    time = 0;
    index = 0;
    useValuesFromTriggerTime = 0;
    computeAssignment = 0;
    performAssignment = 0;
}

PendingAssignment::PendingAssignment( const PendingAssignment& other)
{
    computedValues = other.computedValues;
    modelData = other.modelData;
    time = other.time;
    index = other.index;
    useValuesFromTriggerTime = other.useValuesFromTriggerTime;
    computeAssignment = other.computeAssignment;
    performAssignment = other.performAssignment;

    PendingAssignment *p = const_cast<PendingAssignment*>(&other);
    p->clear();
}

void PendingAssignment::clear()
{
    computedValues = 0;
    modelData = 0;
    time = 0;
    index = 0;
    useValuesFromTriggerTime = 0;
    computeAssignment = 0;
    performAssignment = 0;
}

PendingAssignment& PendingAssignment::operator=( const PendingAssignment& other)
{
    computedValues = other.computedValues;
    modelData = other.modelData;
    time = other.time;
    index = other.index;
    useValuesFromTriggerTime = other.useValuesFromTriggerTime;
    computeAssignment = other.computeAssignment;
    performAssignment = other.performAssignment;

    PendingAssignment *p = const_cast<PendingAssignment*>(&other);
    p->clear();

    return *this;
}

void PendingAssignment::eval()
{
    if (!useValuesFromTriggerTime)
    {
        computedValues = computeAssignment(modelData);
    }

    assert(computedValues);
    performAssignment(modelData, computedValues);
    computedValues = 0;
}

int PendingAssignment::getIndex()
{
    return index;
}

double PendingAssignment::getTime()
{
    return time;
}

PendingAssignment::~PendingAssignment()
{
    if (computedValues != 0)
    {
        Log(Logger::PRIO_WARNING) << "PendingAssigment with index " <<
                index << " was created at time " << time << ", but was never was used";
    }
}

}
