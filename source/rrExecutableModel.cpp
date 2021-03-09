#pragma hdrstop
#include <stdlib.h>
#include <string>
#include "rrExecutableModel.h"
#include "rrSparse.h"
#include <iomanip>



template <typename numeric_type>
static void dump_array(std::ostream &os, int n, const numeric_type *p)
{
    if (p)
    {
        os << setiosflags(std::ios::floatfield) << std::setprecision(8);
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

std::ostream& operator <<(std::ostream &stream, ExecutableModel* model)
{
    model->print(stream);

    double *tmp;

    int nFloat = model->getNumFloatingSpecies();
    int nBound = model->getNumBoundarySpecies();
    int nComp = model->getNumCompartments();
    int nGlobalParam = model->getNumGlobalParameters();
    int nEvents = model->getNumEvents();
    int nReactions = model->getNumReactions();

    stream << "* Calculated Values *" << std::endl;

    tmp = new double[nFloat];
    model->getFloatingSpeciesAmounts(nFloat, 0, tmp);
    stream << "FloatingSpeciesAmounts:" << std::endl;
    dump_array(stream, nFloat, tmp);

    /*
    model->getFloatingSpeciesAmountRates(nFloat, 0, tmp);
    stream << "FloatingSpeciesAmountRates:" << std::endl;
    dump_array(stream, nFloat, tmp);
    */

    model->getFloatingSpeciesConcentrations(nFloat, 0, tmp);
    stream << "FloatingSpeciesConcentrations:" << std::endl;
    dump_array(stream, nFloat, tmp);
    delete[] tmp;

    tmp = new double[nReactions];
    model->getReactionRates(nReactions, 0, tmp);
    stream << "Reaction Rates:" << std::endl;
    dump_array(stream, nReactions, tmp);
    delete [] tmp;

    tmp = new double[nBound];
    model->getBoundarySpeciesAmounts(nBound, 0, tmp);
    stream << "BoundarySpeciesAmounts:" << std::endl;
    dump_array(stream, nBound, tmp);

    model->getBoundarySpeciesConcentrations(nBound, 0, tmp);
    stream << "BoundarySpeciesConcentrations:" << std::endl;
    dump_array(stream, nBound, tmp);
    delete [] tmp;

    tmp = new double[nComp];
    model->getCompartmentVolumes(nComp, 0, tmp);
    stream << "CompartmentVolumes:" << std::endl;
    dump_array(stream, nComp, tmp);
    delete [] tmp;

    tmp = new double[nGlobalParam];
    model->getGlobalParameterValues(nGlobalParam, 0, tmp);
    stream << "GlobalParameters:" << std::endl;
    dump_array(stream, nGlobalParam, tmp);
    delete [] tmp;

    unsigned char *tmpEvents = new unsigned char[nEvents];
    model->getEventTriggers(nEvents, 0, tmpEvents);
    stream << "Events Trigger Status:" << std::endl;
    dump_array(stream, nEvents, (bool*)tmpEvents);
    delete [] tmpEvents;

    return stream;
}





} // namespace rr
