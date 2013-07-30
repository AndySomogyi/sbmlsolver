#include <stdlib.h>
#include <string.h>
#include "rrExecutableModel.h"
#include "rrSparse.h"
#include <iomanip>

using namespace std;

static void dump_array(std::ostream &os, int n, const double *p)
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


namespace rr
{

std::ostream& operator <<(std::ostream &stream, ExecutableModel* model)
{
    model->print(stream);

    double *tmp;

    int nFloat = model->getNumFloatingSpecies();
    int nBound = model->getNumBoundarySpecies();
    int nComp = model->getNumCompartments();
    int nGlobalParam = model->getNumGlobalParameters();

    stream << "* Calculated Values *" << endl;

    tmp = new double[nFloat];
    model->getFloatingSpeciesAmounts(nFloat, 0, tmp);
    stream << "FloatingSpeciesAmounts:" << endl;
    dump_array(stream, nFloat, tmp);

    model->getFloatingSpeciesConcentrations(nFloat, 0, tmp);
    stream << "FloatingSpeciesConcentrations:" << endl;
    dump_array(stream, nFloat, tmp);
    delete[] tmp;

    tmp = new double[nBound];
    model->getBoundarySpeciesAmounts(nBound, 0, tmp);
    stream << "BoundarySpeciesAmounts:" << endl;
    dump_array(stream, nBound, tmp);

    model->getBoundarySpeciesConcentrations(nBound, 0, tmp);
    stream << "BoundarySpeciesConcentrations:" << endl;
    dump_array(stream, nBound, tmp);
    delete tmp;

    tmp = new double[nComp];
    model->getCompartmentVolumes(nComp, 0, tmp);
    stream << "CompartmentVolumes:" << endl;
    dump_array(stream, nComp, tmp);
    delete tmp;

    tmp = new double[nGlobalParam];
    model->getGlobalParameterValues(nGlobalParam, 0, tmp);
    stream << "GlobalParameters:" << endl;
    dump_array(stream, nGlobalParam, tmp);
    delete tmp;

    return stream;
}


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

void allocModelDataBuffers(ModelData &data, const string& modelName)
{
    data.modelName = strdup(modelName.c_str());

    // in certain cases, the data returned by c++ new may be alligned differently than
    // malloc, so just use rrCalloc here just to be safe, plus rrCalloc returns zero
    // initialized memory.


    data.floatingSpeciesAmounts = (double*)rrCalloc(data.numFloatingSpecies, sizeof(double));
    data.floatingSpeciesAmountRates = (double*)rrCalloc(data.numFloatingSpecies, sizeof(double));
    data.rateRules = (double*)rrCalloc(data.numRateRules, sizeof(double));
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
    data.floatingSpeciesCompartments = (int*)rrCalloc(data.numFloatingSpecies, sizeof(int));
    data.boundarySpeciesCompartments = (int*)rrCalloc(data.numBoundarySpecies, sizeof(int));
    data.work = (double*)rrCalloc(data.workSize, sizeof(int));

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
    os << "ModelData:"                 << endl;
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
    os << "rateRules: "                << endl;                                          // 11
    dump_array(os, data.numRateRules, data.rateRules);

//    int*                                localParametersOffsets;           // 12
//
//
//    int*                                localParametersNum;               // 13
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

//    int*                                floatingSpeciesCompartments;      // 20
    os << "numBoundarySpecies: "       << data.numBoundarySpecies << endl;  // 21
    os << "boundarySpeciesConcentrations:" << endl;                         // 22
    dump_array(os, data.numBoundarySpecies, data.boundarySpeciesConcentrations);
    os << "boundarySpeciesAmounts:"    << endl;                             // 23
    dump_array(os, data.numBoundarySpecies, data.boundarySpeciesAmounts);
//    int*                                boundarySpeciesCompartments;      // 24
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

void  freeModelDataBuffers(ModelData &data)
{
    free(data.modelName);

    free(data.floatingSpeciesAmounts);
    free(data.floatingSpeciesAmountRates);
    free(data.rateRules);
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
    free(data.work);

    //Event function pointer stuff
    free(data.eventAssignments);
    free(data.computeEventAssignments);
    free(data.performEventAssignments);
    free(data.eventDelays);

    csr_matrix_delete(data.stoichiometry);
}


void RR_DECLSPEC modeldata_clone(ModelData *dst, ModelData *src)
{
    memcpy(dst, src, sizeof(ModelData));
    allocModelDataBuffers(*dst, src->modelName);
    modeldata_copy_buffers(dst, src);
}

void RR_DECLSPEC modeldata_copy_buffers(ModelData *dst, ModelData *src)
{
    memcpy(dst->floatingSpeciesAmounts, src->floatingSpeciesAmounts,
            src->numFloatingSpecies * sizeof(double));
    memcpy(dst->floatingSpeciesAmountRates, src->floatingSpeciesAmountRates,
            src->numFloatingSpecies * sizeof(double));
    memcpy(dst->rateRules, src->rateRules,
            src->numRateRules * sizeof(double));
    memcpy(dst->floatingSpeciesConcentrations, src->floatingSpeciesConcentrations,
            src->numFloatingSpecies * sizeof(double));
    memcpy(dst->reactionRates, src->reactionRates,
            src->numReactions * sizeof(double));
    memcpy(dst->dependentSpeciesConservedSums, src->dependentSpeciesConservedSums,
            src->numDependentSpecies * sizeof(double));
    memcpy(dst->floatingSpeciesInitConcentrations, src->floatingSpeciesInitConcentrations,
            src->numFloatingSpecies * sizeof(double));
    memcpy(dst->globalParameters, src->globalParameters,
            src->numGlobalParameters * sizeof(double));
    memcpy(dst->compartmentVolumes, src->compartmentVolumes,
            src->numCompartments * sizeof(double));
    memcpy(dst->boundarySpeciesConcentrations, src->boundarySpeciesConcentrations,
            src->numBoundarySpecies * sizeof(double));
    memcpy(dst->boundarySpeciesAmounts, src->boundarySpeciesAmounts,
            src->numBoundarySpecies * sizeof(double));
    memcpy(dst->sr, src->sr,
            src->srSize * sizeof(double));
    memcpy(dst->eventPriorities, src->eventPriorities,
            src->eventPrioritiesSize * sizeof(double));
    memcpy(dst->eventStatusArray, src->eventStatusArray,
            src->eventStatusArraySize * sizeof(bool));
    memcpy(dst->previousEventStatusArray, src->previousEventStatusArray,
            src->previousEventStatusArraySize * sizeof(bool));
    memcpy(dst->eventPersistentType, src->eventPersistentType,
            src->eventPersistentTypeSize * sizeof(bool));
    memcpy(dst->eventTests, src->eventTests,
            src->eventTestsSize * sizeof(double));
    memcpy(dst->eventType, src->eventType,
            src->eventTypeSize * sizeof(bool));
    memcpy(dst->floatingSpeciesCompartments, src->floatingSpeciesCompartments,
            src->numFloatingSpecies * sizeof(int));
    memcpy(dst->boundarySpeciesCompartments, src->boundarySpeciesCompartments,
            src->numBoundarySpecies * sizeof(int));
    memcpy(dst->work, src->work,
            src->workSize * sizeof(int));

    //Event function pointer stuff
    memcpy(dst->eventAssignments, src->eventAssignments,
            src->numEvents * sizeof(EventAssignmentHandler*));
    memcpy(dst->computeEventAssignments, src->computeEventAssignments,
            src->numEvents * sizeof(ComputeEventAssignmentHandler*));
    memcpy(dst->performEventAssignments, src->performEventAssignments,
            src->numEvents * sizeof(PerformEventAssignmentHandler*));
    memcpy(dst->eventDelays, src->eventDelays,
            src->numEvents * sizeof(EventDelayHandler*));

}



} // namespace rr
