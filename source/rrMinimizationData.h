#ifndef rrMinimizationDataH
#define rrMinimizationDataH
#include <ostream>
#include "rrObject.h"
#include "rrStringlist.h"
#include "rrRoadRunnerData.h"
#include "rrParameter.h"
#include "rrParameters.h"
//---------------------------------------------------------------------------

namespace rr
{
using std::ostream;


class RR_DECLSPEC MinimizationData : public rrObject
{
	protected:
		RoadRunnerData                 	mObservedData;				//Observed data
		RoadRunnerData                 	mFittedData;				//Observed data
		stringstream			       	mReport;
        Parameters				    	mParameters;				//Parameters to fit

    public:
					                   	MinimizationData();
					                   ~MinimizationData();
		void					       	init();

        void							addParameter(Parameter<double>& para);
        ostream&	   			       	operator<<(const string& str);
        string					       	getReport();
		Parameters					  	getParameters();

};

template<>
std::string Parameter< MinimizationData* >::getType() const
{
    return "Pointer To Minimization Result";
}

template<>
void Parameter< MinimizationData* >::setValueFromString(const string& val)
{
	//We can't setup this data structure from a string... :(
    throw("You can't set the value of this structure from a string.. :(");
}

}
#endif
