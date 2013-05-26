#ifdef USE_PCH
#include "rr_pch.h"
#endif
#pragma hdrstop
#include "rrMinimizationData.h"
//---------------------------------------------------------------------------
namespace rr
{

MinimizationData::MinimizationData()
{}

MinimizationData::~MinimizationData()
{}

MinimizationData::MinimizationData(const MinimizationData& data)
{
	mExperimentalData = data.mExperimentalData;
    mModelData = data.mModelData;
    mParameters = data.mParameters;
}

MinimizationData& MinimizationData::operator=(MinimizationData& rhs)
{
	mExperimentalData 	= rhs.mExperimentalData;
    mModelData 		= rhs.mModelData;
    mResidualsData 	= rhs.mResidualsData;
    mParameters 	= rhs.mParameters;
	return *this;
}

void MinimizationData::init()
{
	//Empty report
	mReport.str("");
    mParameters.clear();
}

bool MinimizationData::reset()
{
	mParameters.clear();
	mReport.str("");
}

void MinimizationData::setSelectionList(const StringList& selList)
{
 	mSelectionList = selList;
}

RoadRunnerData MinimizationData::getInputData()
{
	return mExperimentalData;
}

RoadRunnerData MinimizationData::getModelData()
{
	return mModelData;
}

RoadRunnerData MinimizationData::getResidualsData()
{
	return mResidualsData;
}

RoadRunnerData& MinimizationData::getInputDataReference()
{
	return mExperimentalData;
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

void MinimizationData::addParameter(const string& name, const int& val)
{
	mParameters.add(new Parameter<int>(name, val, ""));
}

void MinimizationData::setInputData(RoadRunnerData& data)
{
	mExperimentalData = data;
}

void MinimizationData::setModelData(RoadRunnerData& data)
{
	mModelData = data;
}

void MinimizationData::setResidualsData(RoadRunnerData& data)
{
	mResidualsData = data;
}

ostream& MinimizationData::operator<<(const string& str)
{
    mReport << str;
	return mReport;
}

string MinimizationData::getReport()
{
	return mReport.str();
}

Parameters MinimizationData::getParameters()
{
	return mParameters;
}
}

