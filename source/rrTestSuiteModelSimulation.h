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
 * @cond PRIVATE
 */
class RR_DECLSPEC TestSuiteModelSimulation : public SBMLModelSimulation
{
    protected:
        int                     mCurrentCaseNumber;         //If simulating test suite cases.
        std::string             mIntegratorName;            //If simulating test sutie cases.
        std::filesystem::path   mModelSettingsFileName;
        RoadRunnerData          mResultData;
        RoadRunnerData          mReferenceData;
        RoadRunnerData          mErrorData;
        std::filesystem::path   GetSettingsFileNameForCase(int sim_case);
        std::filesystem::path   GetReferenceDataFileNameForCase(int caseNr);
        int                     mNrOfFailingPoints;
        double                  mLargestError;

    public:
        explicit                TestSuiteModelSimulation(const std::filesystem::path& dataOutputFolder = "", const std::filesystem::path& modelFilePath = "", const std::filesystem::path& modelFileName = "");
                               ~TestSuiteModelSimulation() override;
        void                    SetCaseNumber(int cNr);
        void                    SetIntegrator(std::string integrator);
        bool                    LoadReferenceData(std::filesystem::path refDataFileName = "");
        bool                    CreateErrorData();
        bool                    SaveAllData();
        bool                    CopyFilesToOutputFolder();
        double                  LargestError();
        bool                    Pass();
        int                     NrOfFailingPoints();
        bool                    LoadSettings(const std::filesystem::path& fName = "") override;
        virtual bool            LoadSettingsEx(const std::filesystem::path& settingsFName);
};
/** @endcond PRIVATE */
}



#endif
