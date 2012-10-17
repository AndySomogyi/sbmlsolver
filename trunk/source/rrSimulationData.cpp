#ifdef USE_PCH
#include "rr_pch.h"
#endif
#pragma hdrstop
#include <iomanip>
#include "rrLogger.h"
#include "rrUtils.h"
#include "rrStringUtils.h"
#include "rrSimulationData.h"
//---------------------------------------------------------------------------
using namespace std;

namespace rr
{

SimulationData::SimulationData()
:
mTimePrecision(6),
mDataPrecision(16)
{}

SimulationData::SimulationData(const StringList& colNames, const DoubleMatrix& theData)
:
mColumnNames(colNames),
mTheData(theData)
{}

int SimulationData::GetNrOfCols() const
{
    return mTheData.CSize();
}

int SimulationData::GetNrOfRows() const
{
    return mTheData.RSize();
}

void SimulationData::SetName(const string& name)
{
    mName = name;
//    mTheData.SetNamePointer(&mName);
}

StringList SimulationData::GetColumnNames() const
{
    return mColumnNames;
}

pair<int,int> SimulationData::Dimension() const
{
    return pair<int,int>(mTheData.RSize(), mTheData.CSize());
}

string SimulationData::GetName() const
{
    return mName;
}

void SimulationData::SetTimeDataPrecision(const int& prec)
{
    mTimePrecision = prec;
}

void SimulationData::SetDataPrecision(const int& prec)
{
    mDataPrecision = prec;
}

string SimulationData::GetColumnNamesAsString() const
{
    return mColumnNames.AsString();
}

void SimulationData::Allocate(const int& cSize, const int& rSize)
{
    mTheData.Allocate(cSize, rSize);
}

//=========== OPERATORS
double& SimulationData::operator() (const unsigned& row, const unsigned& col)
{
    return mTheData(row,col);
}

double SimulationData::operator() (const unsigned& row, const unsigned& col) const
{
    return mTheData(row,col);
}

void SimulationData::SetColumnNames(const StringList& colNames)
{
    mColumnNames = colNames;
    Log(lDebug3)<<"Simulation Data Columns: "<<mColumnNames;
}

void SimulationData::SetNrOfCols(const int& cols)
{
    mTheData.Allocate(1, cols);
}

void SimulationData::SetData(const DoubleMatrix& theData)
{
    mTheData = theData;
    Log(lDebug5)<<"Simulation Data =========== \n"<<mTheData;
    Check();
}

bool SimulationData::Check() const
{
    if(mTheData.CSize() != mColumnNames.Count())
    {
        Log(lError)<<"Number of columns in simulation data is not equal to number of columns in column header!";
        return false;
    }
    return true;
}

bool SimulationData::Load(const string& fName)
{
    if(!FileExists(fName))
    {
        return false;
    }

    vector<string> lines = GetLinesInFile(fName.c_str());
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
            mTheData(i,j) = ToDouble(aLine[j]);
        }
    }

    return true;
}

bool SimulationData::WriteTo(const string& fileName)
{
    return false;
}

ostream& operator << (ostream& ss, const SimulationData& data)
{
    //Check that the dimensions of col header and data is ok
    if(!data.Check())
    {
        Log(lError)<<"Can't write data..";
        return ss;
    }

    //First create the header
    for(u_int i = 0; i < data.mColumnNames.Count(); i++)
    {
        ss<<data.mColumnNames[i];
        if(i < data.mColumnNames.Count() - 1)
        {
            ss << ",";
        }
        else
        {
            ss << endl;
        }
    }
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

    return ss;
}

}//namespace
