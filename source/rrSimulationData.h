#ifndef rrSimulationDataH
#define rrSimulationDataH
#include <fstream>
#include <sstream>
#include "rrObject.h"
#include "rrStringList.h"
#include "rr-libstruct/lsMatrix.h"
#include "rrExporter.h"
namespace rr
{

using namespace ls;
using std::ofstream;
using std::stringstream;

//Class that  holds the data after a simulation...
class RR_DECLSPEC SimulationData : public rrObject
{

    protected:
        StringList              mColumnNames;
        DoubleMatrix            mTheData;
        int                     mTimePrecision;        //The precision when saved to file
        int                     mDataPrecision;        //The precision when saved to file
        string                  mName;                //For debugging purposes mainly..

    public:
                                SimulationData();
                                SimulationData(const StringList& colNames, const DoubleMatrix& theData);
        StringList              GetColumnNames() const;
        string                  GetColumnNamesAsString() const;
        void                    Allocate(const int& cSize, const int& rSize);
        void                    SetTimeDataPrecision(const int& prec);
        void                    SetDataPrecision(const int& prec);
        void                    SetColumnNames(const StringList& colNames);
        void                    SetNrOfCols(const int& cols);
        int                     GetNrOfCols() const;
        int                     GetNrOfRows() const;
        void                    SetData(const DoubleMatrix& theData);
        bool                    Load(const string& fileName);
        bool                    WriteTo(const string& fileName);


        bool                    Check() const;    //Check if containst proper data
RR_DECLSPEC    friend std::ostream&    operator << (std::ostream& ss, const SimulationData& data);
        double&                 operator() (const unsigned& row, const unsigned& col);
        double                  operator() (const unsigned& row, const unsigned& col) const;

        void                    SetName(const string& name);

        string                  GetName() const;

        pair<int,int>           Dimension() const;

};

//This function is not class member, so need to export separately

}


#endif
