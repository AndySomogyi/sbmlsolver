#ifndef rrSBMLModelSimulationH
#define rrSBMLModelSimulationH
//---------------------------------------------------------------------------
#include <string>
#include "rrExporter.h"
#include "rrStringUtils.h"
#include "rrRoadRunnerOptions.h"
#include "rrRoadRunnerData.h"

namespace rr
{

class RoadRunner;

/**
 * @internal
 *
 * Read an sbml test suite, simulate and compare the results.
 */
class RR_DECLSPEC SBMLModelSimulation
{
    protected:
        std::string                  mModelFileName;
        std::string                  mModelFilePath;
        std::string                  mModelSettingsFileName;
        std::string                  mSimulationLogFile;
        std::string                  mDataOutputFolder;
        std::string                  mTempDataFolder;
        RoadRunner             *mEngine;
        SimulateOptions         mSettings;
        bool                    mCompileIfDllExists;

    public:
                                SBMLModelSimulation(const std::string& dataOutputFolder = "", const std::string& tempDataFilePath = "");
        virtual                ~SBMLModelSimulation();
        bool                    SetModelFilePath(const std::string& path);
        bool                    SetModelFileName(const std::string& name);
        bool                    SetDataOutputFolder(const std::string& name);
        std::string                  GetModelsFullFilePath();
        std::string                  GetDataOutputFolder();
        std::string                  GetTempDataFolder();
        bool                    UseEngine(RoadRunner* engine);




        bool                    SaveModelAsXML(const std::string& folder);

        //wrappers
        bool                    SetTimeStart(const double& tStart);
        bool                    SetTimeEnd(const double& tEnd);
        bool                    SetNumberOfPoints(const int& pts);
        bool                    SetSelectionList(const std::string& list);
        virtual bool            LoadSBMLFromFile();                    //Use current file information to load sbml from file
        virtual bool            Simulate();
        virtual bool            SaveResult();
        void                    loadSBMLTolerances(std::string const& filename);
        virtual bool            LoadSettings(const std::string& fName = "");
        virtual RoadRunnerData  GetResult();

        void                    ReCompileIfDllExists(const bool& doIt);
        bool                    DoCompileIfDllExists();

//        // models are created by loadSBML, if a shared lib already exits, it is loaded.
        bool                    CreateModel();
        bool                    GenerateModelCode();
        bool                    CompileModel();
        bool                    GenerateAndCompileModel();

        /// absolute tolerance for residual comparison, does not affect integrator
        double mAbsolute;
        /// relative tolerance for residual comparison, does not affect integrator
        double mRelative;
};

}



#endif
