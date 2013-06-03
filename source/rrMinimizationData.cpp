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
	mObservedData = data.mObservedData;
    mModelData = data.mModelData;
    mParameters = data.mParameters;
}

MinimizationData& MinimizationData::operator=(MinimizationData& rhs)
{
	mObservedData 	= rhs.mObservedData;
    mModelData 		= rhs.mModelData;
    mResidualsData 	= rhs.mResidualsData;
    mParameters 	= rhs.mParameters;
	return *this;
}

void MinimizationData::init()
{
	//Empty report
    mParameters.clear();
}

bool MinimizationData::reset()
{
	mParameters.clear();
	return true;
}

void MinimizationData::setExperimentalDataSelectionList(const string& selList)
{
 	mExperimentalDataSelectionList = StringList(selList);
}

StringList MinimizationData::getExperimentalDataSelectionList()
{
	return mExperimentalDataSelectionList;
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
}

