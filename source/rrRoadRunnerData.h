#ifndef rrRoadRunnerDataH
#define rrRoadRunnerDataH

#include "rr-libstruct/lsMatrix.h"
#include "rrExporter.h"

#include <string>
#include <vector>
#include <fstream>
#include <sstream>

namespace rr
{

using namespace ls;
using std::ofstream;
using std::stringstream;

/**
 * Class that  holds the data after a simulation.
 *
 * The columns store the variables, i.e. species S1, S2, ...
 * and the rows are the time points of each variable.
 */
class RR_DECLSPEC RoadRunnerData
{

public:
    /**
     * @param rSize: number of rows
     * @param cSize: number of columns.
     */
    RoadRunnerData(const int& rSize = 0, const int& cSize = 0);
    ~RoadRunnerData();

    /**
     * copy constructor -- copy the names and values from the given
     * parameters.
     *
     * @param colNames: list of column names
     * @param data: src data matrix.
     */
    RoadRunnerData(const std::vector<std::string>& colNames,
            const DoubleMatrix& data);

    void allocate(const int& cSize, const int& rSize);
    void allocateWeights();
    bool hasWeights() const;

    /**
     * get the names of the variables stored in this data structure.
     */
    const std::vector<std::string>& getColumnNames() const;
    std::string getColumnName(const int col) const;
    std::string getColumnNamesAsString() const;
    void setColumnNames(const std::vector<std::string>& colNames);
    int getColumnIndex(const std::string& colName) const;
    void setTimeDataPrecision(const int& prec);
    void setDataPrecision(const int& prec);
    void reSize(int rows, int cols);

    /**
     * Clear all data allocated
     */
    void clear();

    /**
     * number of rows.
     */
    int rSize() const;

    /**
     * number of columns
     */
    int cSize() const;

    /**
     * assign data.
     */
    void setData(const DoubleMatrix& theData);

    /**
     * populate from a csv file.
     */
    bool loadSimpleFormat(const std::string& fileName);
    bool writeTo(const std::string& fileName) const;    //Comment: rename to writeToFile ?
    bool readFrom(const std::string& fileName);
    bool check() const;    //Check if containst proper data

    /**
     * when this is converted to a numpy array, should this be converted to a structured
     * array (with column names) (true) or converted to a regular array (false).
     */
    bool structuredResult;

    RR_DECLSPEC
    friend std::ostream& operator <<(std::ostream& ss,
            const RoadRunnerData& data);

    RR_DECLSPEC
    friend std::istream& operator >>(std::istream& ss, RoadRunnerData& data);

    double& operator()(const unsigned& row, const unsigned& col);
    double operator()(const unsigned& row, const unsigned& col) const;
    RoadRunnerData& operator=(const RoadRunnerData& rhs);
    double weight(int row, int col) const;
    double& setWeight(int row, int col);
    void setName(const std::string& name);
    std::string getName() const;
    std::pair<int, int> dimension() const;
    bool append(const RoadRunnerData& data);
    double getTimeStart() const;
    double getTimeEnd() const;
    const DoubleMatrix& getData() const;
    const DoubleMatrix& getWeights() const;


protected:
    std::vector<std::string> mColumnNames;
    DoubleMatrix mTheData;
    DoubleMatrix mWeights;
    int mTimePrecision;            //The precision when saved to file
    int mDataPrecision;            //The precision when saved to file
    std::string mName;             //For debugging purposes mainly..
};

}

#endif
