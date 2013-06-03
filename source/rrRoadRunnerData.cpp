#ifdef USE_PCH
#include "rr_pch.h"
#endif
#pragma hdrstop
#include <iomanip>
#include "rrException.h"
#include "rrLogger.h"
#include "rrUtils.h"
#include "rrStringUtils.h"
#include "rrIniFile.h"
#include "Poco/TemporaryFile.h"
#include "rrRoadRunnerData.h"

//---------------------------------------------------------------------------
using namespace std;

namespace rr
{

RoadRunnerData::RoadRunnerData(const int& rSize, const int& cSize )
:
mTimePrecision(6),
mDataPrecision(16)
{
	if(cSize && rSize)
    {
    	allocate(rSize, cSize);
    }
}

RoadRunnerData::RoadRunnerData(const StringList& colNames, const DoubleMatrix& theData)
:
mColumnNames(colNames),
mTheData(theData)
{}

RoadRunnerData::~RoadRunnerData()
{}


int RoadRunnerData::cSize() const
{
    return mTheData.CSize();
}

int RoadRunnerData::rSize() const
{
    return mTheData.RSize();
}

double RoadRunnerData::getTimeStart()
{
	//Find time column
    int timeCol = mColumnNames.indexOf("time");
    if(timeCol != -1)
    {
    	return mTheData(0,timeCol);
    }
    return gDoubleNaN;
}

double RoadRunnerData::getTimeEnd()
{
	//Find time column
    int timeCol = mColumnNames.indexOf("time");
    if(timeCol != -1)
    {
    	return mTheData(rSize() -1 ,timeCol);
    }
    return gDoubleNaN;
}

void RoadRunnerData::setName(const string& name)
{
    mName = name;
//    mTheData.SetNamePointer(&mName);
}

RoadRunnerData& RoadRunnerData::operator= (const RoadRunnerData& rhs)
{
	if(this == &rhs)
    {
    	return *this;
    }

    mTheData = rhs.mTheData;
    mWeights = rhs.mWeights;
    mColumnNames = rhs.mColumnNames;
    return *this;
}

void RoadRunnerData::allocateWeights()
{
	//Create matrix with weights... initialize all elements to 1
    mWeights.Allocate(mTheData.RSize(), mTheData.CSize());
    for(int i = 0; i < rSize(); i++)
    {
    	for(int j = 0; j < cSize(); j++)
        {
        	if(j == 0)
            {
        		mWeights(i,j) = i + 1;
            }
            else
            {
				mWeights(i,j) = 1;
            }
     	}
    }
}

bool RoadRunnerData::append(const RoadRunnerData& data)
{
	//When appending data, the number of rows have to match with current data
    if(mTheData.RSize() > 0)
    {
		if(data.rSize() != rSize())
        {
        	return false;
        }
	}
    else
    {
    	(*this) = data;
        return true;
    }

    int currColSize = cSize();

    RoadRunnerData temp(mColumnNames, mTheData);

    int newCSize = cSize() + data.cSize();
    mTheData.resize(data.rSize(), newCSize );

	for(int row = 0; row < temp.rSize(); row++)
    {
    	for( int col = 0; col < temp.cSize(); col++)
        {
        	mTheData(row, col) = temp(row, col);
        }
    }

    for(int row = 0; row < mTheData.RSize(); row++)
    {
        for(int col = 0; col < data.cSize(); col++)
        {
            mTheData(row, col + currColSize) = data(row, col);
        }
    }

    for(int col = 0; col < data.cSize(); col++)
    {
    	mColumnNames.Append(data.getColumnName(col));
    }
	return true;
}

StringList RoadRunnerData::getColumnNames() const
{
    return mColumnNames;
}

string RoadRunnerData::getColumnName(const int& col) const
{
	if(col < mColumnNames.Count())
    {
		return mColumnNames[col];
    }

    return "Bad Column..";
}

int RoadRunnerData::getColumnIndex(const string& colName)
{
	return mColumnNames.indexOf(colName);
}

pair<int,int> RoadRunnerData::dimension() const
{
    return pair<int,int>(mTheData.RSize(), mTheData.CSize());
}

string RoadRunnerData::getName() const
{
    return mName;
}

void RoadRunnerData::setTimeDataPrecision(const int& prec)
{
    mTimePrecision = prec;
}

void RoadRunnerData::setDataPrecision(const int& prec)
{
    mDataPrecision = prec;
}

string RoadRunnerData::getColumnNamesAsString() const
{
    return mColumnNames.AsString();
}

void RoadRunnerData::allocate(const int& cSize, const int& rSize)
{
    mTheData.Allocate(cSize, rSize);
}

//=========== OPERATORS
double& RoadRunnerData::operator() (const unsigned& row, const unsigned& col)
{
    return mTheData(row,col);
}

bool RoadRunnerData::hasWeights() const
{
	return (mWeights.size() > 0) ? true : false;
}

double RoadRunnerData::weight(int row, int col) const
{
	return mWeights(row, col);
}

double& RoadRunnerData::setWeight(int row, int col)
{
	return mWeights(row, col);
}

double RoadRunnerData::operator() (const unsigned& row, const unsigned& col) const
{
    return mTheData(row,col);
}

void RoadRunnerData::setColumnNames(const StringList& colNames)
{
    mColumnNames = colNames;
    Log(lDebug3)<<"Simulation Data Columns: "<<mColumnNames;
}

//void RoadRunnerData::setNrOfCols(const int& cols)
//{
//    mTheData.Allocate(1, cols);
//}
//
//void RoadRunnerData::setNrOfRows(const int& rows)
//{
//    mTheData.Allocate(rows, cols);
//}

void RoadRunnerData::reSize(int rows, int cols)
{
    mTheData.Allocate(rows, cols);
}

void RoadRunnerData::setData(const DoubleMatrix& theData)
{
    mTheData = theData;
    Log(lDebug5)<<"Simulation Data =========== \n"<<mTheData;
    check();
}

bool RoadRunnerData::check() const
{
    if(mTheData.CSize() != mColumnNames.Count())
    {
        Log(lError)<<"Number of columns ("<<mTheData.CSize()<<") in simulation data is not equal to number of columns in column header ("<<mColumnNames.Count()<<")";
        return false;
    }
    return true;
}

bool RoadRunnerData::loadSimpleFormat(const string& fName)
{
    if(!fileExists(fName))
    {
        return false;
    }

    vector<string> lines = getLinesInFile(fName.c_str());
    if(!lines.size())
    {
        Log(lError)<<"Failed reading/opening file "<<fName;
        return false;
    }

    mColumnNames = StringList(lines[0]);
    Log(lInfo)<<mColumnNames;

    mTheData.resize(lines.size() -1, mColumnNames.Count());

    for(int i = 0; i < mTheData.RSize(); i++)
    {
        StringList aLine(lines[i+1]);
        for(int j = 0; j < aLine.Count(); j++)
        {
            mTheData(i,j) = toDouble(aLine[j]);
        }
    }

    return true;
}

bool RoadRunnerData::writeTo(const string& fileName)
{
	ofstream aFile(fileName.c_str());
    if(!aFile)
    {
    	Log(lError)<<"Failed opening file: "<<fileName;
        return false;
    }
    aFile<<(*this);
    aFile.close();

    return true;
}

ostream& operator << (ostream& ss, const RoadRunnerData& data)
{
    //Check that the dimensions of col header and data is ok
    if(!data.check())
    {
        Log(lError)<<"Can't write data.. the dimension of the header don't agree with nr of cols of data";
        return ss;
    }

    ss<<"[INFO]"<<endl;
	ss<<"ROAD_RUNNER_VERSION=0.5"	<<endl;
    ss<<"CREATOR=rrWinC-0.1"		<<endl;
    ss<<"NUMBER_OF_COLS="			<<data.cSize()<<endl;
    ss<<"NUMBER_OF_ROWS="			<<data.rSize()<<endl;
    ss<<"COLUMN_HEADERS="			<<data.getColumnNamesAsString()<<endl;

    ss<<endl;
	ss<<"[DATA]"<<endl;
    //Then the data
    for(u_int row = 0; row < data.mTheData.RSize(); row++)
    {
        for(u_int col = 0; col < data.mTheData.CSize(); col++)
        {
            if(col == 0)
            {
                ss<<setprecision(data.mTimePrecision)<<data.mTheData(row, col);
            }
            else
            {
                ss<<setprecision(data.mDataPrecision)<<data.mTheData(row, col);
            }

            if(col <data.mTheData.CSize() -1)
            {
                ss << ",";
            }
            else
            {
                ss << endl;
            }
        }
    }

    if(data.mWeights.isAllocated())
    {
		//Write weights section
        ss<<endl;
        ss<<"[WEIGHTS]"<<endl;

        //Then the data
        for(u_int row = 0; row < data.mWeights.RSize(); row++)
        {
            for(u_int col = 0; col < data.mWeights.CSize(); col++)
            {
                if(col == 0)
                {
                    ss<<setprecision(data.mTimePrecision)<<data.mWeights(row, col);
                }
                else
                {
                    ss<<setprecision(data.mDataPrecision)<<data.mWeights(row, col);
                }

                if(col <data.mTheData.CSize() -1)
                {
                    ss << ",";
                }
                else
                {
                    ss << endl;
                }
            }
        }


    }
    return ss;
}

//Stream data from a file
istream& operator >> (istream& ss, RoadRunnerData& data)
{
	//Read in all lines into a string
    std::string oneLine((std::istreambuf_iterator<char>(ss)), std::istreambuf_iterator<char>());

    //This is pretty inefficient
    string tempFileName = joinPath(getUsersTempDataFolder(), "rrTempFile.dat");
    ofstream tempFile(tempFileName.c_str());

    tempFile << oneLine;
    tempFile.close();

    //Create a iniFile object and parse the data
    IniFile ini(tempFileName, true);

    IniSection* infoSection = ini.GetSection("INFO");
    if(!infoSection)
    {
    	Log(lError)<<"RoadRunnder data file is missing section: [INFO]. Exiting reading file...";
        return ss;
    }

	//Setup header
    IniKey* colNames = infoSection->GetKey("COLUMN_HEADERS");

    if(!colNames)
    {
    	Log(lError)<<"RoadRunnder data file is missing ini key: COLUMN_HEADERS. Exiting reading file...";
        return ss;
    }

    data.setColumnNames(colNames->mValue);

    //Read number of cols and rows and setup data
    IniKey* aKey1 = infoSection->GetKey("NUMBER_OF_COLS");
    IniKey* aKey2 = infoSection->GetKey("NUMBER_OF_ROWS");
    if(!aKey1 || !aKey2)
    {
    	Log(lError)<<"RoadRunnder data file is missing ini key: NUMBER_OF_COLS and/or NUMBER_OF_ROWS. Exiting reading file...";
        return ss;
    }

    int rDim = aKey2->AsInt();
    int cDim = aKey1->AsInt();
    data.reSize(rDim, cDim);

	//get data section
	IniSection* dataSection = ini.GetSection("DATA");
    if(!dataSection)
    {
    	Log(lError)<<"RoadRunnder data file is missing ini section: DATA. Exiting reading file...";
        return ss;
    }

    StringList lines = splitString(dataSection->GetNonKeysAsString(), "\n");
    for(int row = 0; row < lines.Count(); row++)
    {
    	string oneLine = lines[row];
    	StringList aLine(splitString(oneLine, ','));
        if(aLine.Count() != cDim)
        {
        	throw(CoreException("Bad roadrunner data in data file"));
        }

    	for(int col = 0; col < cDim; col++)
        {
        	Log(lDebug5)<<"Word "<<aLine[col];
        	double value = toDouble(trim(aLine[col],' '));
    		data(row, col) = value;
        }
    }

    //Weights ??

	IniSection* weightsSection = ini.GetSection("WEIGHTS");
    if(!weightsSection)	//Optional
    {
    	Log(lDebug)<<"RoadRunnder data file is missing section: WEIGHTS. ";
        return ss;
    }
	data.mWeights.Allocate(rDim, cDim);

    lines = splitString(weightsSection->GetNonKeysAsString(), "\n");
    for(int row = 0; row < lines.Count(); row++)
    {
    	string oneLine = lines[row];
    	StringList aLine(splitString(oneLine, ','));
        if(aLine.Count() != cDim)
        {
        	throw(CoreException("Bad roadrunner data in data file"));
        }

    	for(int col = 0; col < cDim; col++)
        {
        	Log(lDebug5)<<"Word "<<aLine[col];
        	double value = toDouble(aLine[col]);
    		data.mWeights(row, col) = value;
        }
    }

    return ss;
}

}//namespace
