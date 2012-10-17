#pragma hdrstop
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <tchar.h>
#include <dir.h>
#include <iomanip>
#include "rrLog.h"
#include "rrRoadRunner.h"
#include "rrCGenerator.h"
#include "rrException.h"
#include "rrStringUtils.h"
//---------------------------------------------------------------------------
#pragma argsused

using namespace std;
using namespace rr;

bool 		CreateDLL(const string& cmdLine);
HINSTANCE 	LoadDLL(const string& dllName);
void 		PauseBeforeExit(bool doIt = true);

const int MAXMODULE = 512;

typedef int 	(WINAPI*cfunc1)();
typedef char* 	(WINAPI*cfunc2)();

cfunc1 InitModel;
cfunc2 GetModelName;

int _tmain()
{
    bool generateCSharp = false;
    char exePath[MAXPATH];
    getcwd(exePath, MAXPATH);
    string appPath(exePath);
    gLog.Init("compileModel", lDebug5, unique_ptr<LogFile>(new LogFile("CompilingModel.log")));
    LogOutput::mLogToConsole = true;

    gLog.SetCutOffLogLevel(lDebug4);
//       	gLog.SetCutOffLogLevel(lInfo);

    Log(lDebug4)<<"Logs are going to "<<exePath<<"\\"<<gLog.GetLogFileName()<< " (and cout)";
    RoadRunner *roadRunner = NULL;

    try
    {
        //Loading models (max is 459)
        int caseNr = 1;

        //int caseNr = 41;
        if(roadRunner)
        {
            delete roadRunner;	//Hav to do this because some initialization problems(?) in libs
        }

        roadRunner = new RoadRunner;
        roadRunner->Reset();
        string modelsRootPath("C:\\rrw\\Models");
        string subFolder("l2v4");

        //int caseNr = 1;
        stringstream modelSubPath;
        stringstream modelFName;
        modelSubPath<<setfill('0')<<setw(5)<<caseNr;		//create the "00023" subfolder format
        modelFName<<setfill('0')<<setw(5)<<caseNr<<"-sbml-l2v4.xml";

        //string subFolder("");
        //model<<"feedback.xml";
        if(subFolder.size())
        {
            modelsRootPath = modelsRootPath + "\\" + subFolder + "\\" + modelSubPath.str();
        }

        string fullFilePath(modelsRootPath +   "\\" + modelFName.str());
        ifstream inFileStream(fullFilePath.c_str());
        if(!inFileStream)
        {
            throw(Exception("Failed to open the model file:" + fullFilePath));
        }
        Log(lInfo)<<"\n\n ===== Reading model file:"<<fullFilePath<<" ==============";
        std::string sbml((std::istreambuf_iterator<char>(inFileStream)), std::istreambuf_iterator<char>());
        inFileStream.close();
        Log(lDebug5)<<"Before loading SBML. SBML model code size: "<<sbml.size();

        // --------------------------------------
        CGenerator *codeGen = dynamic_cast<CGenerator*>(roadRunner->GetCodeGenerator());
		codeGen->SetXMLModelFileName(modelFName.str());

        roadRunner->loadSBML(sbml);	//This does create the source code..

        if(!codeGen)
        {
            Log(lInfo)<<"Could not get the C Code Generator?";
            throw;
        }

        string srcCodeFolder("C:\\rrw\\Testing\\rr_code_output\\c_from_rr++");
		codeGen->SaveSourceCodeToFolder(srcCodeFolder);

        //Now compile the code and load the resulting dll, and call an exported function in it...
		stringstream exeCmd;
        exeCmd<<"tcc -g -v -shared "<<codeGen->GetSourceCodeFileName()<<" -DBUILD_MODEL_DLL";


        Log(lInfo)<<"\n================ Compiling the DLL =============";
        Log(lInfo)<<"\nExecuting: "<<exeCmd.str();

        if(!CreateDLL(exeCmd.str()))
        {
			Log(lError)<<"Creating DLL failed..";
            PauseBeforeExit();
        }

        //Check if the DLL exists...
        string dllFName(GetFileNameNoPath(codeGen->GetSourceCodeFileName()));
        string dllName(appPath + "\\" + ChangeFileNameExtensionTo(dllFName,"dll"));

        //Load the DLL
        HINSTANCE dllHandle = LoadDLL(dllName);
		if(!dllHandle)
        {
			Log(lError)<<"Loading the DLL failed..";
            PauseBeforeExit();
        }

        //Load functions..
        InitModel = (cfunc1) GetProcAddress((HMODULE) dllHandle, "InitModel");
        if(InitModel == NULL)
        {
            Log(lError) << "Unable to load function." << endl;
            FreeLibrary((HMODULE) dllHandle);
            PauseBeforeExit();
        }

        GetModelName = (cfunc2) GetProcAddress((HMODULE) dllHandle, "GetModelName");
        if(GetModelName == NULL)
        {
            Log(lError) << "Unable to load function." << endl;
            FreeLibrary((HMODULE) dllHandle);
            PauseBeforeExit();
        }

        Log(lInfo)<<"\n================= Testing some DLL functions ========================";
        int res = InitModel();
        Log(lInfo)<<"The result was: "<<res;

        char* modelName = GetModelName();
        if(modelName)
        {
        	Log(lInfo)<<"Model Name is: "<<modelName;
        }

        roadRunner->SimulateSBMLFile(fullFilePath, true);


        FreeLibrary((HMODULE) dllHandle);

        Log(lInfo)<<"\nCopyright: "<<roadRunner->getCopyright()<<endl;
        delete roadRunner;
    }
    catch(Exception& ex)
    {
        Log(lError)<<"RoadRunner exception occured: "<<ex.what()<<endl;
    }

  	//-------------------------------------
    PauseBeforeExit();
	return 0;
}

HINSTANCE LoadDLL(const string& dll)
{
    HINSTANCE hLib = LoadLibrary(dll.c_str());

    if(hLib == NULL)
    {
    	cout << "Unable to load library!" << endl;
        return NULL;
    }

	TCHAR mod[MAXMODULE];
    GetModuleFileNameA((HMODULE)hLib, (LPTSTR) mod, MAXMODULE);
    string name(mod);

    cout << "Library loaded: " <<name.c_str() << endl;
	return hLib;
}

bool CreateDLL(const string& cmdLine)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

    if( !cmdLine.size() )
    {
        return false;
    }

    // Start the child process.
    if( !CreateProcess( NULL,   		// No module name (use command line)
        (char*) cmdLine.c_str(),        // Command line
        NULL,                           // Process handle not inheritable
        NULL,                           // Thread handle not inheritable
        FALSE,                          // Set handle inheritance to FALSE
        0,                              // No creation flags
        NULL,                           // Use parent's environment block
        NULL,                           // Use parent's starting directory
        &si,                            // Pointer to STARTUPINFO structure
        &pi )                           // Pointer to PROCESS_INFORMATION structure
    )
    {
        printf( "CreateProcess failed (%d).\n", GetLastError() );
        return false;
    }

    // Wait until child process exits.
    WaitForSingleObject( pi.hProcess, INFINITE );

    // Close process and thread handles.
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );
    return true;
}

void PauseBeforeExit(bool doIt)
{
	if(!doIt)
    {
    	return;
    }

    cout<<"Hit any key to exit...";
    cin.ignore(0,'\n');
    getch();
    cout<<"\nExiting....\n";
    exit(0);
}
