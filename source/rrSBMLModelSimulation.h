#ifndef rrSBMLModelSimulationH
#define rrSBMLModelSimulationH
//---------------------------------------------------------------------------
#include <string>
#include <filesystem>
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
        std::filesystem::path        mModelFilePath;
        std::filesystem::path        mModelSettingsFileName;
        std::filesystem::path        mSimulationLogFile;
        std::filesystem::path        mDataOutputFolder;
        std::filesystem::path        mTempDataFolder;
        RoadRunner                   *mEngine;
        SimulateOptions              mSettings;
        bool                         mCompileIfDllExists;

    public:
                                SBMLModelSimulation(std::filesystem::path  dataOutputFolder = "", std::filesystem::path  tempDataFilePath = "");
        virtual                ~SBMLModelSimulation();
        bool                    SetModelFilePath(const std::filesystem::path& path);
        bool                    SetModelFileName(const std::string& name);
        bool                    SetDataOutputFolder(const std::filesystem::path& name);
        std::filesystem::path   GetModelsFullFilePath();
        std::filesystem::path   GetDataOutputFolder();
        std::filesystem::path   GetTempDataFolder();
        bool                    UseEngine(RoadRunner* engine);


        bool                    SaveModelAsXML( std::filesystem::path& folder);

        //wrappers
        bool                    SetTimeStart(const double& tStart);
        bool                    SetTimeEnd(const double& tEnd);
        bool                    SetNumberOfPoints(const int& pts);
        bool                    SetSelectionList(const std::string& list);
        virtual bool            LoadSBMLFromFile();                    //Use current file information to load sbml from file
        virtual bool            Simulate();
        virtual bool            SaveResult();
        void                    loadSBMLTolerances(std::filesystem::path const& filename);
        virtual bool            LoadSettings(const std::filesystem::path& fName = "");
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
