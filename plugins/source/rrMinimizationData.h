#ifndef rrMinimizationDataH
#define rrMinimizationDataH
#include <ostream>
#include "rrPluginsAPIExporter.h"
#include "rrStringList.h"
#include "rrRoadRunnerData.h"
#include "rrParameter.h"
#include "rrParameters.h"
#include "rrLogger.h"
//---------------------------------------------------------------------------

namespace rrp
{
using std::ostream;
using rr::RoadRunnerData;
/**
 * @internal
 */
class PLUGINS_API_DECLSPEC MinimizationData
{
    protected:
//        RoadRunnerData                  mObservedData;                  //Observed data
        RoadRunnerData                  mModelData;                     //Model data
        RoadRunnerData                  mResidualsData;                 //Residuals data
        Parameters                      mParameters;                    //Parameters to fit
        Parameters                      mParametersOut;                 //Parameters that was fitted
        double                          mNorm;                          //Norm
        StringList                      mObservedDataSelectionList;
        StringList                      mModelDataSelectionList;

    public:
                                        MinimizationData();
                                        ~MinimizationData();
                                        MinimizationData(const MinimizationData& data);
        MinimizationData&               operator=(MinimizationData& rhs);
        void                            init();

        void                            addParameter(const string& name, const double& value);
        void                            addParameter(const string& name, const int& value);
        void                            addFittedParameter(const string& name, const double& value);
        void                            setNorm(const double& norm);
        double                          getNorm();

        void                            setObservedDataSelectionList(const string& list);
        StringList                      getObservedDataSelectionList();
        void                            setModelDataSelectionList(const string& selList);
        StringList                      getModelDataSelectionList();

//        void                            setInputData(const RoadRunnerData& data);
        void                            setModelData(const RoadRunnerData& data);
        void                            setResidualsData(const RoadRunnerData& data);

//        RoadRunnerData                  getObservedData();
        RoadRunnerData                  getModelData();
        RoadRunnerData                  getResidualsData();

//        RoadRunnerData&                 getObservedDataReference();
        RoadRunnerData&                 getModelDataReference();
        RoadRunnerData&                 getResidualsDataReference();

        PLUGINS_API_DECLSPEC
        friend ostream&                 operator<<(ostream& stream, const MinimizationData& outMe);
        string                          getReport() const;
        Parameters                      getParameters();
        Parameters                      getParametersOut();
        bool                            reset();
};

template<>
inline std::string Parameter< MinimizationData >::getType() const
{
    return "Pointer To Minimization Result";
}

template<>
inline string Parameter< MinimizationData >::getValueAsString() const
{
    Log(rr::lInfo)<<"Getting minimization data object as a string is not implemented";
    return "";//    throw("You can't get the value of this structure as string.. ");
}

template<>
inline void Parameter< MinimizationData >::setValueFromString(const string& val)
{
    //We can't setup this data structure from a string... :(
    Log(rr::lInfo)<<"Setting minimization data object from string is not implemented";
    return;
}

template<>
inline void Parameter< MinimizationData >::setValue(MinimizationData* val)
{
    mValue = *(val);
}

}
#endif
