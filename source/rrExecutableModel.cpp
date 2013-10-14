#pragma hdrstop
#include <stdlib.h>
#include <string.h>
#include "rrExecutableModel.h"
#include "rrSparse.h"
#include <iomanip>

using namespace std;

template <typename numeric_type>
static void dump_array(std::ostream &os, int n, const numeric_type *p)
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

    stream << "* Calculated Values *" << endl;

    tmp = new double[nFloat];
    model->getFloatingSpeciesAmounts(nFloat, 0, tmp);
    stream << "FloatingSpeciesAmounts:" << endl;
    dump_array(stream, nFloat, tmp);

    /*
    model->getFloatingSpeciesAmountRates(nFloat, 0, tmp);
    stream << "FloatingSpeciesAmountRates:" << endl;
    dump_array(stream, nFloat, tmp);
    */

    model->getFloatingSpeciesConcentrations(nFloat, 0, tmp);
    stream << "FloatingSpeciesConcentrations:" << endl;
    dump_array(stream, nFloat, tmp);
    delete[] tmp;

    tmp = new double[nReactions];
    model->getReactionRates(nReactions, 0, tmp);
    stream << "Reaction Rates:" << endl;
    dump_array(stream, nReactions, tmp);
    delete tmp;

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

    unsigned char *tmpEvents = new unsigned char[nEvents];
    model->getEventTriggers(nEvents, 0, tmpEvents);
    stream << "Events Trigger Status:" << endl;
    dump_array(stream, nEvents, (bool*)tmpEvents);
    delete tmpEvents;

    return stream;
}





} // namespace rr
