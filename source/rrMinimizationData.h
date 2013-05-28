#ifndef rrMinimizationDataH
#define rrMinimizationDataH
#include <ostream>
#include "rrObject.h"
#include "rrStringlist.h"
#include "rrRoadRunnerData.h"
#include "rrParameter.h"
#include "rrParameters.h"
#include "rrLogger.h"
//---------------------------------------------------------------------------

namespace rr
{
using std::ostream;


class RR_DECLSPEC MinimizationData : public rrObject
{
	protected:
		RoadRunnerData                 	mObservedData;				//Observed data
		RoadRunnerData                 	mModelData;					//Observed data
		RoadRunnerData                 	mResidualsData;				//Observed data
        Parameters				    	mParameters;				//Parameters to fit
        StringList						mExperimentalDataSelectionList;
        StringList						mModelDataSelectionList;
    public:
					                   	MinimizationData();
					                   ~MinimizationData();
					                   	MinimizationData(const MinimizationData& data);
		MinimizationData&				operator=(MinimizationData& rhs);
		void					       	init();

        void							addParameter(const string& name, const double& value);
        void							addParameter(const string& name, const int& value);
        void							setExperimentalDataSelectionList(const string& list);
        StringList						getExperimentalDataSelectionList();
        void							setModelDataSelectionList(const string& selList);
        StringList						getModelDataSelectionList();

        void							setInputData(const RoadRunnerData& data);
        void							setModelData(const RoadRunnerData& data);
        void							setResidualsData(const RoadRunnerData& data);

        RoadRunnerData					getObservedData();
        RoadRunnerData					getModelData();
        RoadRunnerData					getResidualsData();

        RoadRunnerData&					getObservedDataReference();
        RoadRunnerData&					getModelDataReference();
        RoadRunnerData&					getResidualsDataReference();

        ostream&	   			       	operator<<(const string& str);
        string					       	getReport();
		Parameters					  	getParameters();
        bool							reset();

};

template<>
inline std::string Parameter< MinimizationData >::getType() const
{
    return "Pointer To Minimization Result";
}

template<>
inline string Parameter< MinimizationData >::getValueAsString() const
{
    Log(lInfo)<<"Getting minimization data object as a string is not implemented";
	return "";//    throw("You can't get the value of this structure as string.. ");
}

template<>
inline void Parameter< MinimizationData >::setValueFromString(const string& val)
{
	//We can't setup this data structure from a string... :(
    Log(lInfo)<<"Setting minimization data object from string is not implemented";
    return;
}

template<>
inline void Parameter< MinimizationData >::setValue(MinimizationData* val)
{
	mValue = *(val);
}

}
#endif
