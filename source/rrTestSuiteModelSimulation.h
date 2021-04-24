#ifndef rrSBMLTestSuiteModelSimulationH
#define rrSBMLTestSuiteModelSimulationH
//---------------------------------------------------------------------------
#include <string>
#include "rrSBMLModelSimulation.h"
#include "rrStringUtils.h"
#include "rrRoadRunnerOptions.h"
#include "rrRoadRunnerData.h"

namespace rr
{

/**
 * @internal
 */
class RR_DECLSPEC TestSuiteModelSimulation : public SBMLModelSimulation
{
    protected:
        int                     mCurrentCaseNumber;                     //If simulating test suite cases...
        std::string                  mModelSettingsFileName;
        RoadRunnerData          mResultData;
        RoadRunnerData          mReferenceData;
        RoadRunnerData          mErrorData;
        std::string                  GetSettingsFileNameForCase(int sim_case);
        std::string                  GetReferenceDataFileNameForCase(int caseNr);
        int                     mNrOfFailingPoints;
        double                  mLargestError;

    public:
                                TestSuiteModelSimulation(const std::string& dataOutputFolder = "", const std::string& modelFilePath = "", const std::string& modelFileName = "");
                               ~TestSuiteModelSimulation();
        void                    SetCaseNumber(int cNr);
        bool                    LoadReferenceData(std::string refDataFileName = "");
        bool                    CreateErrorData();
        bool                    SaveAllData();
        bool                    CopyFilesToOutputFolder();
        double                  LargestError();
        bool                    Pass();
        int                     NrOfFailingPoints();
        virtual bool            LoadSettings(const std::string& fName = "");
        virtual bool            LoadSettingsEx(const std::string& settingsFName);
};

}



#endif
