#pragma hdrstop
#include <sstream>
#include "rrMinimizationData.h"
//---------------------------------------------------------------------------
namespace rrp
{
using namespace std;
MinimizationData::MinimizationData()
{}

MinimizationData::~MinimizationData()
{}

MinimizationData::MinimizationData(const MinimizationData& data)
{
    mObservedData = data.mObservedData;
    mModelData = data.mModelData;
    mParameters = data.mParameters;
}

MinimizationData& MinimizationData::operator=(MinimizationData& rhs)
{
    mObservedData     = rhs.mObservedData;
    mModelData         = rhs.mModelData;
    mResidualsData     = rhs.mResidualsData;
    mParameters     = rhs.mParameters;
    return *this;
}

void MinimizationData::init()
{
    //Empty report
    mParameters.clear();
    mParametersOut.clear();
    mNorm = -1;
}

bool MinimizationData::reset()
{
    mParameters.clear();
    mParametersOut.clear();
    return true;
}

void MinimizationData::setObservedDataSelectionList(const string& selList)
{
     mObservedDataSelectionList = StringList(selList);
}

StringList MinimizationData::getObservedDataSelectionList()
{
    return mObservedDataSelectionList;
}

void MinimizationData::setModelDataSelectionList(const string& selList)
{
     mModelDataSelectionList = StringList(selList);
}

StringList MinimizationData::getModelDataSelectionList()
{
    return mModelDataSelectionList;
}

RoadRunnerData MinimizationData::getObservedData()
{
    return mObservedData;
}

RoadRunnerData MinimizationData::getModelData()
{
    return mModelData;
}

RoadRunnerData MinimizationData::getResidualsData()
{
    return mResidualsData;
}

RoadRunnerData& MinimizationData::getObservedDataReference()
{
    return mObservedData;
}

RoadRunnerData& MinimizationData::getModelDataReference()
{
    return mModelData;
}

RoadRunnerData& MinimizationData::getResidualsDataReference()
{
    return mResidualsData;
}

void MinimizationData::addParameter(const string& name, const double& val)
{
    mParameters.add(new Parameter<double>(name, val, ""));
}

void MinimizationData::addFittedParameter(const string& name, const double& val)
{
    mParametersOut.add(new Parameter<double>(name, val, ""));
}

void MinimizationData::addParameter(const string& name, const int& val)
{
    mParameters.add(new Parameter<int>(name, val, ""));
}

void MinimizationData::setInputData(const RoadRunnerData& data)
{
    mObservedData = data;
}

void MinimizationData::setModelData(const RoadRunnerData& data)
{
    mModelData = data;
}

void MinimizationData::setResidualsData(const RoadRunnerData& data)
{
    mResidualsData = data;
}

Parameters MinimizationData::getParameters()
{
    return mParameters;
}

Parameters MinimizationData::getParametersOut()
{
    return mParametersOut;
}

void MinimizationData::setNorm(const double& norm)
{
    mNorm = norm;
}

double MinimizationData::getNorm()
{
    return mNorm;
}

string MinimizationData::getReport() const
{
    stringstream  msg;
    msg<<"Parameters: \n";
    msg<<mParameters.asStringList();
    msg<<"\n";

    msg<<"Observed Data ===============\n";
    msg<<mObservedData<<"\n\n";

    msg<<"ModelData Data ===============\n";
    msg<<mModelData<<"\n\n";

    msg<<"Residuals ===============\n";
    msg<<mResidualsData<<"\n\n";

    msg<<"End of minimization data report";
    return msg.str();
}

ostream& operator<<(ostream& stream, const MinimizationData& outMe)
{
    stream<<outMe.getReport();   //virtual friend idiom
    return stream;
}

}

