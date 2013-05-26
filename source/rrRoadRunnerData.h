#ifndef rrRoadRunnerDataH
#define rrRoadRunnerDataH
#include <fstream>
#include <sstream>
#include "rr-libstruct/lsMatrix.h"
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
        DoubleMatrix			mWeights;				//Matrix containing weights
        int                     mTimePrecision;        	//The precision when saved to file
        int                     mDataPrecision;        	//The precision when saved to file
        string                  mName;                	//For debugging purposes mainly..

    public:
                                RoadRunnerData(const int& rSize = 0, const int& cSize = 0);
                               ~RoadRunnerData();
                                RoadRunnerData(const StringList& colNames, const DoubleMatrix& theData);
        void                    allocate(const int& cSize, const int& rSize);
        void                    allocateWeights();
        bool					hasWeights() const;


        StringList              getColumnNames() const;
        string					getColumnName(const int& col) const;
        string                  getColumnNamesAsString() const;
        void                    setColumnNames(const StringList& colNames);
        int						getColumnIndex(const string& colName);

        void                    setTimeDataPrecision(const int& prec);
        void                    setDataPrecision(const int& prec);

		void					reSize(int rows, int cols);
        int						rSize() const;
        int						cSize() const;
        void                    setData(const DoubleMatrix& theData);
        bool                    loadSimpleFormat(const string& fileName);
        bool                    writeTo(const string& fileName);
        bool                    check() const;    //Check if containst proper data

		RR_DECLSPEC
        friend std::ostream&    operator << (std::ostream& ss, const RoadRunnerData& data);

		RR_DECLSPEC
        friend std::istream&    operator >> (std::istream& ss, RoadRunnerData& data);

        double&                 operator() (const unsigned& row, const unsigned& col);
        double                  operator() (const unsigned& row, const unsigned& col) const;
        RoadRunnerData&			operator=  (const RoadRunnerData& rhs);
        double					weight(int row, int col) const;
        double&					setWeight(int row, int col);
        void                    setName(const string& name);
        string                  getName() const;
        pair<int,int>           dimension() const;
        bool					append(const RoadRunnerData& data);
        double					getTimeStart();
        double					getTimeEnd();
};

}

#endif
