#ifndef rrSBMLModelSimulationH
#define rrSBMLModelSimulationH
//---------------------------------------------------------------------------
#include <string>
#include "rrObject.h"
#include "rrStringUtils.h"
#include "rrSimulationSettings.h"
#include "rrSimulationData.h"

namespace rr
{
class RoadRunner;
class RR_DECLSPEC SBMLModelSimulation : public rrObject
{
    protected:
        string                  mModelFileName;
        string                  mModelFilePath;
        string                  mModelSettingsFileName;
        string                  mSimulationLogFile;
        string                  mDataOutputFolder;
        string                  mTempDataFolder;
        RoadRunner             *mEngine;
        SimulationSettings      mSettings;
        bool                    mCompileIfDllExists;

    public:
                                SBMLModelSimulation(const string& dataOutputFolder = "", const string& tempDataFilePath = "");
        virtual                ~SBMLModelSimulation();
        bool                    SetModelFilePath(const string& path);
        bool                    SetModelFileName(const string& name);
        bool                    SetDataOutputFolder(const string& name);
        string                  GetModelsFullFilePath();
        string                  GetDataOutputFolder();
        string                  GetTempDataFolder();
        bool                    UseEngine(RoadRunner* engine);

        SimulationData          GetResult();

        bool                    GenerateModelCode();
        bool                    CreateModel();
        bool                    CompileModel();
        bool                    InitializeModel();
        bool                    GenerateAndCompileModel();
        bool                    Simulate();
        virtual bool            SaveResult();
        virtual bool            LoadSettings(const string& fName = "");
        void                    CompileIfDllExists(const bool& doIt);
        bool                    CompileIfDllExists();
        bool                    SaveModelAsXML(const string& folder);

        //API
        bool                    SetTimeStart(const double& tStart);
        bool                    SetTimeEnd(const double& tEnd);
        bool                    SetNumberOfPoints(const int& pts);
        bool                    SetSelectionList(const string& list);
        bool                    LoadSBMLFromFile();                    //Use current file information to load sbml from file
};

}



#endif
