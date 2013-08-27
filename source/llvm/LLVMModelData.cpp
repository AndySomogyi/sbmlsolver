/*
 * rrLLVMModelData.cpp
 *
 *  Created on: Aug 8, 2013
 *      Author: andy
 */


#include "LLVMModelData.h"


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
    // in certain cases, the data returned by c++ new may be alligned differently than
    // malloc, so just use calloc here just to be safe, plus calloc returns zero
    // initialized memory.

    data.floatingSpeciesAmounts = (double*)calloc(data.numFloatingSpecies, sizeof(double));
    data.floatingSpeciesAmountRates = (double*)calloc(data.numFloatingSpecies, sizeof(double));
    data.rateRuleRates = (double*)calloc(data.numRateRules, sizeof(double));
    data.reactionRates = (double*)calloc(data.numReactions, sizeof(double));
    data.dependentSpeciesConservedSums = (double*)calloc(data.numDependentSpecies, sizeof(double));
    data.globalParameters = (double*)calloc(data.numGlobalParameters, sizeof(double));
    data.compartmentVolumes = (double*)calloc(data.numCompartments, sizeof(double));
    data.boundarySpeciesAmounts = (double*)calloc(data.numBoundarySpecies, sizeof(double));
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




    //double* floatingSpeciesInitConcentrations;    // 17


    os << "floatingSpeciesAmountRates: " << endl;       // 18
    dump_array(os, data.numFloatingSpecies, data.floatingSpeciesAmountRates);

    os << "floatingSpeciesAmounts: "    << endl;           // 19
    dump_array(os, data.numFloatingSpecies, data.floatingSpeciesAmounts);

    os << "numBoundarySpecies: "       << data.numBoundarySpecies << endl;  // 21

    os << "boundarySpeciesAmounts:"    << endl;                             // 23
    dump_array(os, data.numBoundarySpecies, data.boundarySpeciesAmounts);
//    unsigned*                                boundarySpeciesCompartments;      // 24
    os << "numCompartments: "          << data.numCompartments << endl;     // 25
    os << "compartmentVolumes:"        << endl;                             // 26
    dump_array(os, data.numCompartments, data.compartmentVolumes);
    os << "stoichiometry:"             << endl;                             // 27
    os << data.stoichiometry;


    return os;
}

void  LLVMModelData::freeBuffers(LLVMModelData &data)
{
    free(data.floatingSpeciesAmounts);
    free(data.floatingSpeciesAmountRates);
    free(data.rateRuleRates);
    free(data.rateRuleValues);
    free(data.reactionRates);
    free(data.dependentSpeciesConservedSums);
    free(data.globalParameters);
    free(data.compartmentVolumes);
    free(data.boundarySpeciesAmounts);

    csr_matrix_delete(data.stoichiometry);
}

} // namespace rr





