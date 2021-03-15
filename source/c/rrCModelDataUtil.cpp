/*
 * rrCModelDataUtil.cpp
 *
 *  Created on: Aug 8, 2013
 *      Author: andy
 */
#pragma hdrstop
#include "rrCModelDataUtil.h"

#include <iomanip>
#include <cstdlib>
#include <cstring>




static void dump_array(std::ostream &os, int n, const double *p)
{
    if (p)
    {
        os << std::setiosflags(std::ios::floatfield) << std::setprecision(8);
        os << '[';
        for (int i = 0; i < n; ++i)
        {
            os << p[i];
            if (i < n - 1)
            {
                os << ", ";
            }
        }
        os << ']' << std::endl;
    }
    else
    {
        os << "NULL" << std::endl;
    }
}


namespace rr
{




void initModelData(ModelData &data)
{
    //Zero data structure..
    memset(&data, 0, sizeof(rr::ModelData));
}

/**
 * many implementations have a rrCalloc which returns a non zero pointer
 * if a zero size data block is requested, this returns zero which
 * makes debugging easier.
 */
static void *rrCalloc(size_t nmemb, size_t size)
{
    return nmemb * size != 0 ? calloc(nmemb, size) : 0;
}

void allocModelDataBuffers(ModelData &data, const std::string& modelName)
{
    data.modelName = strdup(modelName.c_str());

    // in certain cases, the data returned by c++ new may be alligned differently than
    // malloc, so just use rrCalloc here just to be safe, plus rrCalloc returns zero
    // initialized memory.


    data.floatingSpeciesAmounts = (double*)rrCalloc(data.numFloatingSpecies, sizeof(double));
    data.floatingSpeciesAmountRates = (double*)rrCalloc(data.numFloatingSpecies, sizeof(double));
    data.rateRuleRates = (double*)rrCalloc(data.numRateRules, sizeof(double));
    data.floatingSpeciesConcentrations = (double*)rrCalloc(data.numFloatingSpecies, sizeof(double));
    data.reactionRates = (double*)rrCalloc(data.numReactions, sizeof(double));
    data.dependentSpeciesConservedSums = (double*)rrCalloc(data.numDependentSpecies, sizeof(double));
    data.floatingSpeciesInitConcentrations = (double*)rrCalloc(data.numFloatingSpecies, sizeof(double));
    data.globalParameters = (double*)rrCalloc(data.numGlobalParameters, sizeof(double));
    data.compartmentVolumes = (double*)rrCalloc(data.numCompartments, sizeof(double));
    data.boundarySpeciesConcentrations = (double*)rrCalloc(data.numBoundarySpecies, sizeof(double));
    data.boundarySpeciesAmounts = (double*)rrCalloc(data.numBoundarySpecies, sizeof(double));
    data.sr = (double*)rrCalloc(data.srSize, sizeof(double));
    data.eventPriorities = (double*)rrCalloc(data.eventPrioritiesSize, sizeof(double));
    data.eventStatusArray = (bool*)rrCalloc(data.eventStatusArraySize, sizeof(bool));
    data.previousEventStatusArray = (bool*)rrCalloc(data.previousEventStatusArraySize, sizeof(bool));
    data.eventPersistentType = (bool*)rrCalloc(data.eventPersistentTypeSize, sizeof(bool));
    data.eventTests = (double*)rrCalloc(data.eventTestsSize, sizeof(double));
    data.eventType = (bool*)rrCalloc(data.eventTypeSize, sizeof(bool));
    data.floatingSpeciesCompartments = (unsigned*)rrCalloc(data.numFloatingSpecies, sizeof(unsigned));
    data.boundarySpeciesCompartments = (unsigned*)rrCalloc(data.numBoundarySpecies, sizeof(unsigned));

    //Event function pointer stuff
    data.eventAssignments =
            (EventAssignmentHandler*)rrCalloc(data.numEvents, sizeof(EventAssignmentHandler*));
    data.computeEventAssignments =
            (ComputeEventAssignmentHandler*)rrCalloc(data.numEvents, sizeof(ComputeEventAssignmentHandler*));
    data.performEventAssignments =
            (PerformEventAssignmentHandler*)rrCalloc(data.numEvents, sizeof(PerformEventAssignmentHandler*));
    data.eventDelays =
            (EventDelayHandler*)rrCalloc(data.numEvents, sizeof(EventDelayHandler*));
}


std::ostream& operator <<(std::ostream& os, const ModelData& data)
{
    os << "ModelData:"                 << std::endl;
    os << "time: "                     << data.time << std::endl;                             // 2
    os << "numIndependentSpecies: "    << data.numIndependentSpecies << std::endl;            // 3
    os << "numDependentSpecies: "      << data.numDependentSpecies << std::endl;              // 4
    os << "dependentSpeciesConservedSums:" << std::endl;                                      // 5
    dump_array(os, data.numDependentSpecies, data.dependentSpeciesConservedSums);
    os << "numGlobalParameters: "      << data.numGlobalParameters << std::endl;              // 6
    os << "globalParameters: "         << std::endl;                                          // 7
    dump_array(os, data.numGlobalParameters, data.globalParameters);
    os << "numReactions: "             << data.numReactions << std::endl;                     // 8
    os << "reactionRates: "            << std::endl;                                          // 9
    dump_array(os, data.numReactions, data.reactionRates);
    os << "numRateRules: "             << data.numRateRules << std::endl;                     // 10
    os << "rateRuleValues: "           << std::endl;                                          // 11
    dump_array(os, data.numRateRules, data.rateRuleValues);
    os << "rateRuleRates: "            << std::endl;                                          // 11
    dump_array(os, data.numRateRules, data.rateRuleRates);

//    unsigned*                                localParametersOffsets;           // 12
//
//
//    unsigned*                                localParametersNum;               // 13
//
//
//    double*                             localParameters;                  // 14

    os << "numFloatingSpecies: "       << data.numFloatingSpecies << std::endl;       // 15

    os << "floatingSpeciesConcentrations: " << std::endl;                                     // 16
    dump_array(os, data.numFloatingSpecies, data.floatingSpeciesConcentrations);



    //double* floatingSpeciesInitConcentrations;    // 17


    os << "floatingSpeciesAmountRates: " << std::endl;       // 18
    dump_array(os, data.numFloatingSpecies, data.floatingSpeciesAmountRates);

    os << "floatingSpeciesAmounts: "    << std::endl;           // 19
    dump_array(os, data.numFloatingSpecies, data.floatingSpeciesAmounts);

//    unsigned*                                floatingSpeciesCompartments;      // 20
    os << "numBoundarySpecies: "       << data.numBoundarySpecies << std::endl;  // 21
    os << "boundarySpeciesConcentrations:" << std::endl;                         // 22
    dump_array(os, data.numBoundarySpecies, data.boundarySpeciesConcentrations);
    os << "boundarySpeciesAmounts:"    << std::endl;                             // 23
    dump_array(os, data.numBoundarySpecies, data.boundarySpeciesAmounts);
//    unsigned*                                boundarySpeciesCompartments;      // 24
    os << "numCompartments: "          << data.numCompartments << std::endl;     // 25
    os << "compartmentVolumes:"        << std::endl;                             // 26
    dump_array(os, data.numCompartments, data.compartmentVolumes);
    os << "stoichiometry:"             << std::endl;                             // 27
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

void  freeModelDataBuffers(ModelData &data)
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
    free(data.sr);
    free(data.eventPriorities);
    free(data.eventStatusArray);
    free(data.previousEventStatusArray);
    free(data.eventPersistentType);
    free(data.eventTests);
    free(data.eventType);

    //Event function pointer stuff
    free(data.eventAssignments);
    free(data.computeEventAssignments);
    free(data.performEventAssignments);
    free(data.eventDelays);
}


} /* namespace rr */
