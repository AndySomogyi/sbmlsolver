#ifndef rrRoadRunnerDataH
#define rrRoadRunnerDataH
#include <fstream>
#include <sstream>
#include "rr-libstruct/lsMatrix.h"
#include "rr-libstruct/lsLibStructural.h"

#include "rrObject.h"
#include "rrStringList.h"
#include "rrExporter.h"
namespace rr
{

using namespace ls;
using std::ofstream;
using std::stringstream;

//Class that  holds the data after a simulation...
class RR_DECLSPEC RoadRunnerData : public rrObject
{
    protected:
        StringList              mColumnNames;
        DoubleMatrix            mTheData;
        int                     mTimePrecision;        //The precision when saved to file
        int                     mDataPrecision;        //The precision when saved to file
        string                  mName;                //For debugging purposes mainly..

    public:
                                RoadRunnerData();
                                RoadRunnerData(const StringList& colNames, const DoubleMatrix& theData);
        void                    allocate(const int& cSize, const int& rSize);

        StringList              getColumnNames() const;
        string					getColumnName(const int& col) const;
        string                  getColumnNamesAsString() const;
        void                    setColumnNames(const StringList& colNames);

        void                    setTimeDataPrecision(const int& prec);
        void                    setDataPrecision(const int& prec);

		void					reSize(int rows, int cols);
        int						rSize() const;
        int						cSize() const;
        void                    setData(const DoubleMatrix& theData);
        bool                    load(const string& fileName);
        bool                    writeTo(const string& fileName);
        bool                    check() const;    //Check if containst proper data

		RR_DECLSPEC
        friend std::ostream&    operator << (std::ostream& ss, const RoadRunnerData& data);

		RR_DECLSPEC
        friend std::istream&    operator >> (std::istream& ss, RoadRunnerData& data);

        double&                 operator() (const unsigned& row, const unsigned& col);
        double                  operator() (const unsigned& row, const unsigned& col) const;
        RoadRunnerData&			operator=  (const RoadRunnerData& rhs);
        void                    setName(const string& name);
        string                  getName() const;
        pair<int,int>           dimension() const;
        bool					append(const RoadRunnerData& data);
        double					getTimeStart();
        double					getTimeEnd();
};

}

#endif
