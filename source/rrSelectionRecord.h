#ifndef rrSelectionRecordH
#define rrSelectionRecordH
#include <string>
#include <iomanip>
#include <ostream>
#include "rrExporter.h"
#include "rrObject.h"
//---------------------------------------------------------------------------
namespace rr
{
namespace SelectionType
{

}

class RR_DECLSPEC SelectionRecord
{
public:
    enum SelectionType
    {
            clTime = 0,
            clBoundarySpecies,
            clFloatingSpecies,
            clFlux,
            clRateOfChange,
            clVolume,
            clParameter,
    /*7*/   clFloatingAmount,
    /*8*/   clBoundaryAmount,
            clElasticity,
            clUnscaledElasticity,
            clEigenValue,
            clUnknown,
            clStoichiometry
    };
    int index;
    string p1;
    string p2;

    SelectionType selectionType;
    SelectionRecord(const int& index = 0,
            const SelectionType type = clUnknown,
            const string& p1 = "", const string& p2 = "");

};

ostream& operator<< (ostream& stream, const SelectionRecord& rec);
}


#endif
