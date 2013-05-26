#ifdef USE_PCH
#include "rr_pch.h"
#endif
#pragma hdrstop
#include "rrMinimizationData.h"
//---------------------------------------------------------------------------
namespace rr
{

MinimizationData::MinimizationData()
{

}

MinimizationData::~MinimizationData()
{

}

void MinimizationData::init()
{
	//Empty report
	mReport.str("");
    mParameters.clear();
}

void MinimizationData::addParameter(Parameter<double>& para)
{
	mParameters.add(&para);
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

}

