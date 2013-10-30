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


std::ostream& operator <<(std::ostream& os, const LLVMModelData& data)
{
    os << "LLVMModelData:"                 << endl;
    os << "size: "                     << data.size << endl;                             // 0
    os << "flags: "                    << data.flags << endl;                            // 1
    os << "time: "                     << data.time << endl;                             // 2
    os << "numIndFloatingSpecies: "    << data.numIndFloatingSpecies << endl;            // 3


    os << "numIndGlobalParameters: "      << data.numIndGlobalParameters << endl;              // 6
    os << "globalParameters: "         << endl;                                          // 7
    dump_array(os, data.numIndGlobalParameters, data.globalParametersAlias);
    os << "numReactions: "             << data.numReactions << endl;                     // 8
    os << "reactionRates: "            << endl;                                          // 9
    dump_array(os, data.numReactions, data.reactionRatesAlias);
    os << "numRateRules: "             << data.numRateRules << endl;                     // 10
    os << "rateRuleValues: "           << endl;                                          // 11
    dump_array(os, data.numRateRules, data.rateRuleValuesAlias);


    os << "floatingSpeciesAmounts: "    << endl;           // 19
    dump_array(os, data.numIndFloatingSpecies, data.floatingSpeciesAmountsAlias);

    os << "numIndBoundarySpecies: "       << data.numIndBoundarySpecies << endl;  // 21

    os << "boundarySpeciesAmounts:"    << endl;                             // 23
    dump_array(os, data.numIndBoundarySpecies, data.boundarySpeciesAmountsAlias);
//    unsigned*                                boundarySpeciesCompartments;      // 24
    os << "numIndCompartments: "          << data.numIndCompartments << endl;     // 25
    os << "compartmentVolumes:"        << endl;                             // 26
    dump_array(os, data.numIndCompartments, data.compartmentVolumesAlias);
    os << "stoichiometry:"             << endl;                             // 27
    os << data.stoichiometry;


    return os;
}

void  LLVMModelData_free(LLVMModelData *data)
{
    if (data)
    {
        csr_matrix_delete(data->stoichiometry);
        ::free(data);
    }
}

} // namespace rr





