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
#include "Random.h"
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
    os << "LLVMModelData:"                  << endl;
    os << "size: "                          << data.size << endl;
    os << "flags: "                         << data.flags << endl;
    os << "time: "                          << data.time << endl;
    os << "numIndFloatingSpecies: "         << data.numIndFloatingSpecies << endl;

    os << "numIndGlobalParameters: "        << data.numIndGlobalParameters << endl;
    os << "globalParameters: "              << endl;
    dump_array(os, data.numIndGlobalParameters, data.globalParametersAlias);

    os << "numReactions: "                  << data.numReactions << endl;
    os << "reactionRates: "                 << endl;
    dump_array(os, data.numReactions, data.reactionRatesAlias);

    os << "numRateRules: "                  << data.numRateRules << endl;
    os << "rateRuleValues: "                << endl;
    dump_array(os, data.numRateRules, data.rateRuleValuesAlias);

    os << "floatingSpeciesAmounts: "        << endl;
    dump_array(os, data.numIndFloatingSpecies, data.floatingSpeciesAmountsAlias);

    os << "numIndBoundarySpecies: "         << data.numIndBoundarySpecies << endl;

    os << "boundarySpeciesAmounts:"         << endl;
    dump_array(os, data.numIndBoundarySpecies, data.boundarySpeciesAmountsAlias);

    os << "numIndCompartments: "            << data.numIndCompartments << endl;
    os << "compartmentVolumes:"             << endl;
    dump_array(os, data.numIndCompartments, data.compartmentVolumesAlias);

    os << "stoichiometry:"                  << endl;
    os << data.stoichiometry;


    os << "numInitFloatingSpecies: "        << data.numInitFloatingSpecies << endl;
    os << "initFloatingSpeciesAmounts: "    << endl;
    dump_array(os, data.numInitFloatingSpecies, data.initFloatingSpeciesAmountsAlias);

    os << "numInitCompartments: "           << data.numInitCompartments << endl;
    os << "initCompartmentVolumes:"         << endl;
    dump_array(os, data.numInitCompartments, data.initCompartmentVolumesAlias);

    os << "numInitGlobalParameters: "       << data.numInitGlobalParameters << endl;
    os << "initGlobalParameters: "          << endl;
    dump_array(os, data.numInitGlobalParameters, data.initGlobalParametersAlias);


    return os;
}

void  LLVMModelData_free(LLVMModelData *data)
{
    if (data)
    {
        csr_matrix_delete(data->stoichiometry);
        delete data->random;
        ::free(data);
    }
}

} // namespace rr





