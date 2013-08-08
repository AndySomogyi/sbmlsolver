/*
 * rrLLVMModelData.cpp
 *
 *  Created on: Aug 8, 2013
 *      Author: andy
 */


#include "rrLLVMModelData.h"


#include <stdlib.h>
#include <string.h>
#include "rrExecutableModel.h"
#include "rrSparse.h"
#include <iomanip>



using namespace std;



static void dump_array(std::ostream &os, int n, const double *p)
{
    if (p)
    {
        os << setiosflags(ios::floatfield) << setprecision(8);
        os << '[';
        for (int i = 0; i < n; ++i)
        {
            os << p[i];
            if (i < n - 1)
            {
                os << ", ";
            }
        }
        os << ']' << endl;
    }
    else
    {
        os << "NULL" << endl;
    }
}


namespace rr
{



void LLVMModelData::init(LLVMModelData &data)
{
    //Zero data structure..
    memset(&data, 0, sizeof(rr::LLVMModelData));
}


void LLVMModelData::allocBuffers(LLVMModelData &data, const string& modelName)
{
    data.modelName = strdup(modelName.c_str());

    // in certain cases, the data returned by c++ new may be alligned differently than
    // malloc, so just use calloc here just to be safe, plus calloc returns zero
    // initialized memory.


    data.floatingSpeciesAmounts = (double*)calloc(data.numFloatingSpecies, sizeof(double));
    data.floatingSpeciesAmountRates = (double*)calloc(data.numFloatingSpecies, sizeof(double));
    data.rateRuleRates = (double*)calloc(data.numRateRules, sizeof(double));
    data.floatingSpeciesConcentrations = (double*)calloc(data.numFloatingSpecies, sizeof(double));
    data.reactionRates = (double*)calloc(data.numReactions, sizeof(double));
    data.dependentSpeciesConservedSums = (double*)calloc(data.numDependentSpecies, sizeof(double));
    data.floatingSpeciesInitConcentrations = (double*)calloc(data.numFloatingSpecies, sizeof(double));
    data.globalParameters = (double*)calloc(data.numGlobalParameters, sizeof(double));
    data.compartmentVolumes = (double*)calloc(data.numCompartments, sizeof(double));
    data.boundarySpeciesConcentrations = (double*)calloc(data.numBoundarySpecies, sizeof(double));
    data.boundarySpeciesAmounts = (double*)calloc(data.numBoundarySpecies, sizeof(double));
    data.eventPriorities = (double*)calloc(data.eventPrioritiesSize, sizeof(double));
    data.eventStatusArray = (bool*)calloc(data.eventStatusArraySize, sizeof(bool));
    data.previousEventStatusArray = (bool*)calloc(data.previousEventStatusArraySize, sizeof(bool));
    data.eventPersistentType = (bool*)calloc(data.eventPersistentTypeSize, sizeof(bool));
    data.eventTests = (double*)calloc(data.eventTestsSize, sizeof(double));
    data.eventType = (bool*)calloc(data.eventTypeSize, sizeof(bool));
    data.floatingSpeciesCompartments = (unsigned*)calloc(data.numFloatingSpecies, sizeof(unsigned));
    data.boundarySpeciesCompartments = (unsigned*)calloc(data.numBoundarySpecies, sizeof(unsigned));
    data.work = (double*)calloc(data.workSize, sizeof(double));

    //Event function pointer stuff
    data.eventAssignments =
            (LLVMEventAssignmentHandler*)calloc(data.numEvents, sizeof(LLVMEventAssignmentHandler*));
    data.computeEventAssignments =
            (LLVMComputeEventAssignmentHandler*)calloc(data.numEvents, sizeof(LLVMComputeEventAssignmentHandler*));
    data.performEventAssignments =
            (LLVMPerformEventAssignmentHandler*)calloc(data.numEvents, sizeof(LLVMPerformEventAssignmentHandler*));
    data.eventDelays =
            (LLVMEventDelayHandler*)calloc(data.numEvents, sizeof(LLVMEventDelayHandler*));
}


std::ostream& operator <<(std::ostream& os, const LLVMModelData& data)
{
    os << "LLVMModelData:"                 << endl;
    os << "size: "                     << data.size << endl;                             // 0
    os << "flags: "                    << data.flags << endl;                            // 1
    os << "time: "                     << data.time << endl;                             // 2
    os << "numIndependentSpecies: "    << data.numIndependentSpecies << endl;            // 3
    os << "numDependentSpecies: "      << data.numDependentSpecies << endl;              // 4
    os << "dependentSpeciesConservedSums:" << endl;                                      // 5
    dump_array(os, data.numDependentSpecies, data.dependentSpeciesConservedSums);
    os << "numGlobalParameters: "      << data.numGlobalParameters << endl;              // 6
    os << "globalParameters: "         << endl;                                          // 7
    dump_array(os, data.numGlobalParameters, data.globalParameters);
    os << "numReactions: "             << data.numReactions << endl;                     // 8
    os << "reactionRates: "            << endl;                                          // 9
    dump_array(os, data.numReactions, data.reactionRates);
    os << "numRateRules: "             << data.numRateRules << endl;                     // 10
    os << "rateRuleValues: "           << endl;                                          // 11
    dump_array(os, data.numRateRules, data.rateRuleValues);
    os << "rateRuleRates: "            << endl;                                          // 11
    dump_array(os, data.numRateRules, data.rateRuleRates);

//    unsigned*                                localParametersOffsets;           // 12
//
//
//    unsigned*                                localParametersNum;               // 13
//
//
//    double*                             localParameters;                  // 14

    os << "numFloatingSpecies: "       << data.numFloatingSpecies << endl;       // 15

    os << "floatingSpeciesConcentrations: " << endl;                                     // 16
    dump_array(os, data.numFloatingSpecies, data.floatingSpeciesConcentrations);



    //double* floatingSpeciesInitConcentrations;    // 17


    os << "floatingSpeciesAmountRates: " << endl;       // 18
    dump_array(os, data.numFloatingSpecies, data.floatingSpeciesAmountRates);

    os << "floatingSpeciesAmounts: "    << endl;           // 19
    dump_array(os, data.numFloatingSpecies, data.floatingSpeciesAmounts);

//    unsigned*                                floatingSpeciesCompartments;      // 20
    os << "numBoundarySpecies: "       << data.numBoundarySpecies << endl;  // 21
    os << "boundarySpeciesConcentrations:" << endl;                         // 22
    dump_array(os, data.numBoundarySpecies, data.boundarySpeciesConcentrations);
    os << "boundarySpeciesAmounts:"    << endl;                             // 23
    dump_array(os, data.numBoundarySpecies, data.boundarySpeciesAmounts);
//    unsigned*                                boundarySpeciesCompartments;      // 24
    os << "numCompartments: "          << data.numCompartments << endl;     // 25
    os << "compartmentVolumes:"        << endl;                             // 26
    dump_array(os, data.numCompartments, data.compartmentVolumes);
    os << "stoichiometry:"             << endl;                             // 27
    os << data.stoichiometry;
//    int                                 numEvents;                        // 28
//    int                                 eventTypeSize;                    // 29
//    bool*                               eventType;                        // 30
//    int                                 eventPersistentTypeSize;          // 31
//    bool*                               eventPersistentType;              // 32
//    int                                 eventTestsSize;                   // 33
//    double*                             eventTests;                       // 34
//    int                                 eventPrioritiesSize;              // 35
//    double*                             eventPriorities;                  // 36
//    int                                 eventStatusArraySize;             // 37
//    bool*                               eventStatusArray;                 // 38
//    int                                 previousEventStatusArraySize;     // 39
//    bool*                               previousEventStatusArray;         // 40
//    int                                 workSize;                         // 41
//    double*                             work;                             // 42
//    EventDelayHandler*                eventDelays;                      // 43
//    EventAssignmentHandler*           eventAssignments;                 // 44
//    ComputeEventAssignmentHandler*    computeEventAssignments;          // 45
//    PerformEventAssignmentHandler*    performEventAssignments;          // 46
//    char*                               modelName;                        // 47
//    char**                              variableTable;                    // 48
//    char**                              boundaryTable;                    // 49
//    char**                              globalParameterTable;             // 50
//    int                                 srSize;                           // 51
//    double*                             sr;                               // 52





    return os;
}

void  LLVMModelData::freeBuffers(LLVMModelData &data)
{
    free(data.modelName);

    free(data.floatingSpeciesAmounts);
    free(data.floatingSpeciesAmountRates);
    free(data.rateRuleRates);
    free(data.floatingSpeciesConcentrations);
    free(data.floatingSpeciesCompartments);
    free(data.reactionRates);
    free(data.dependentSpeciesConservedSums);
    free(data.floatingSpeciesInitConcentrations);
    free(data.globalParameters);
    free(data.compartmentVolumes);
    free(data.boundarySpeciesConcentrations);
    free(data.boundarySpeciesCompartments);
    free(data.boundarySpeciesAmounts);
    free(data.eventPriorities);
    free(data.eventStatusArray);
    free(data.previousEventStatusArray);
    free(data.eventPersistentType);
    free(data.eventTests);
    free(data.eventType);
    free(data.work);

    //Event function pointer stuff
    free(data.eventAssignments);
    free(data.computeEventAssignments);
    free(data.performEventAssignments);
    free(data.eventDelays);

    csr_matrix_delete(data.stoichiometry);
}


void LLVMModelData::clone(LLVMModelData *dst, LLVMModelData *src)
{
    memcpy(dst, src, sizeof(LLVMModelData));
    allocBuffers(*dst, src->modelName);
    copyBuffers(dst, src);
}

void LLVMModelData::copyBuffers(LLVMModelData *dst, LLVMModelData *src)
{
    if (dst->floatingSpeciesAmounts && src->floatingSpeciesAmounts)
    {
        memcpy(dst->floatingSpeciesAmounts, src->floatingSpeciesAmounts,
                src->numFloatingSpecies * sizeof(double));
    }

    if (dst->floatingSpeciesAmountRates && src->floatingSpeciesAmountRates)
    {
        memcpy(dst->floatingSpeciesAmountRates, src->floatingSpeciesAmountRates,
                src->numFloatingSpecies * sizeof(double));
    }

    if (dst->rateRuleRates && src->rateRuleRates)
    {
        memcpy(dst->rateRuleRates, src->rateRuleRates,
                src->numRateRules * sizeof(double));
    }

    if (dst->floatingSpeciesConcentrations
            && src->floatingSpeciesConcentrations)
    {
        memcpy(dst->floatingSpeciesConcentrations,
                src->floatingSpeciesConcentrations,
                src->numFloatingSpecies * sizeof(double));
    }

    if (dst->reactionRates && src->reactionRates)
    {
        memcpy(dst->reactionRates, src->reactionRates,
                src->numReactions * sizeof(double));
    }

    if (dst->dependentSpeciesConservedSums
            && src->dependentSpeciesConservedSums)
    {
        memcpy(dst->dependentSpeciesConservedSums,
                src->dependentSpeciesConservedSums,
                src->numDependentSpecies * sizeof(double));
    }

    if (dst->floatingSpeciesInitConcentrations
            && src->floatingSpeciesInitConcentrations)
    {
        memcpy(dst->floatingSpeciesInitConcentrations,
                src->floatingSpeciesInitConcentrations,
                src->numFloatingSpecies * sizeof(double));
    }

    if (dst->globalParameters && src->globalParameters)
    {
        memcpy(dst->globalParameters, src->globalParameters,
                src->numGlobalParameters * sizeof(double));
    }

    if (dst->compartmentVolumes && src->compartmentVolumes)
    {
        memcpy(dst->compartmentVolumes, src->compartmentVolumes,
                src->numCompartments * sizeof(double));
    }

    if (dst->boundarySpeciesConcentrations
            && src->boundarySpeciesConcentrations)
    {
        memcpy(dst->boundarySpeciesConcentrations,
                src->boundarySpeciesConcentrations,
                src->numBoundarySpecies * sizeof(double));
    }

    if (dst->boundarySpeciesAmounts && src->boundarySpeciesAmounts)
    {
        memcpy(dst->boundarySpeciesAmounts, src->boundarySpeciesAmounts,
                src->numBoundarySpecies * sizeof(double));
    }

    if (dst->eventPriorities && src->eventPriorities)
    {
        memcpy(dst->eventPriorities, src->eventPriorities,
                src->eventPrioritiesSize * sizeof(double));
    }

    if (dst->eventStatusArray && src->eventStatusArray)
    {
        memcpy(dst->eventStatusArray, src->eventStatusArray,
                src->eventStatusArraySize * sizeof(bool));
    }

    if (dst->previousEventStatusArray && src->previousEventStatusArray)
    {
        memcpy(dst->previousEventStatusArray, src->previousEventStatusArray,
                src->previousEventStatusArraySize * sizeof(bool));
    }

    if (dst->eventPersistentType && src->eventPersistentType)
    {
        memcpy(dst->eventPersistentType, src->eventPersistentType,
                src->eventPersistentTypeSize * sizeof(bool));
    }

    if (dst->eventTests && src->eventTests)
    {
        memcpy(dst->eventTests, src->eventTests,
                src->eventTestsSize * sizeof(double));
    }

    if (dst->eventType && src->eventType)
    {
        memcpy(dst->eventType, src->eventType,
                src->eventTypeSize * sizeof(bool));
    }

    if (dst->floatingSpeciesCompartments && src->floatingSpeciesCompartments)
    {
        memcpy(dst->floatingSpeciesCompartments,
                src->floatingSpeciesCompartments,
                src->numFloatingSpecies * sizeof(int));
    }

    if (dst->boundarySpeciesCompartments && src->boundarySpeciesCompartments)
    {
        memcpy(dst->boundarySpeciesCompartments,
                src->boundarySpeciesCompartments,
                src->numBoundarySpecies * sizeof(int));
    }

    if (dst->work && src->work)
    {
        memcpy(dst->work, src->work, src->workSize * sizeof(int));
    }

    //Event function pointer stuff
    if (dst->eventAssignments && src->eventAssignments)
    {
        memcpy(dst->eventAssignments, src->eventAssignments,
                src->numEvents * sizeof(EventAssignmentHandler*));
    }

    if (dst->computeEventAssignments && src->computeEventAssignments)
    {
        memcpy(dst->computeEventAssignments, src->computeEventAssignments,
                src->numEvents * sizeof(ComputeEventAssignmentHandler*));
    }

    if (dst->performEventAssignments && src->performEventAssignments)
    {
        memcpy(dst->performEventAssignments, src->performEventAssignments,
                src->numEvents * sizeof(PerformEventAssignmentHandler*));
    }

    if (dst->eventDelays && src->eventDelays)
    {
        memcpy(dst->eventDelays, src->eventDelays,
                src->numEvents * sizeof(EventDelayHandler*));
    }

}



} // namespace rr





