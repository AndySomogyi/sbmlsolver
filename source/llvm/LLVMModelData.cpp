/*
 * rrLLVMModelData.cpp
 *
 *  Created on: Aug 8, 2013
 *      Author: andy
 */

#pragma hdrstop
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


namespace rrllvm
{



void LLVMModelData::init(LLVMModelData &data)
{
    //Zero data structure..
    memset(&data, 0, sizeof(LLVMModelData));
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

    os << "numFloatingSpecies: "       << data.numFloatingSpecies << endl;       // 15

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
    free(data.rateRuleValues);
    free(data.reactionRates);
    free(data.dependentSpeciesConservedSums);
    free(data.globalParameters);
    free(data.compartmentVolumes);
    free(data.boundarySpeciesAmounts);

    csr_matrix_delete(data.stoichiometry);
}

} // namespace rr





