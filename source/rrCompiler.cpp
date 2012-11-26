#ifdef USE_PCH
#include "rr_pch.h"
#endif
#pragma hdrstop
#include <sstream>
#if defined(__CODEGEARC__)
#include <dir.h>
#elif defined(_MSVC)
#include <direct.h>
#endif
#include "rrLogger.h"
#include "rrCompiler.h"
#include "rrException.h"
#include "rrStringUtils.h"
#include "rrUtils.h"
#include "rrRoadRunner.h"
//---------------------------------------------------------------------------

using namespace std;
namespace rr
{

Compiler::Compiler(const string& supportCodeFolder, const string& compiler)
:
mSupportCodeFolder(supportCodeFolder),
mCompilerName(ExtractFileName(compiler)),
mCompilerLocation(ExtractFilePath(compiler))
{
	if(mSupportCodeFolder.size() > 0)
    {
        if(!setupCompiler(mSupportCodeFolder))
        {
            Log(lWarning)<<"Roadrunner internal compiler setup failed. ";
        }
    }
}

Compiler::~Compiler(){}

bool Compiler::setupCompiler(const string& supportCodeFolder)
{
    mSupportCodeFolder = supportCodeFolder;

    if(!FolderExists(mSupportCodeFolder))
    {
    	Log(lError)<<"The roadrunner support code folder : "<<mSupportCodeFolder<<" does not exist.";
        return false;
    }

    return true;
}

bool Compiler::SetCompiler(const string& compiler)
{
    mCompilerName = compiler;
	return true;
}

string Compiler::GetDLLName()
{
    return mDLLFileName;
}

bool Compiler::CompileC_DLL(const string& sourceFileName)
{
    //Compile the code and load the resulting dll, and call an exported function in it...
#if defined(_WIN32) || defined(__CODEGEARC__)
    string dllFName(ChangeFileExtensionTo(ExtractFileName(sourceFileName), "dll"));
#else
    string dllFName(string("lib") + ChangeFileExtensionTo(ExtractFileName(sourceFileName), "so"));
#endif
    mDLLFileName = JoinPath(ExtractFilePath(sourceFileName), dllFName);

    //Setup compiler environment
    SetupCompilerEnvironment();

    string exeCmd = CreateCompilerCommand(sourceFileName);

    //exeCmd += " > compileLog.log";
    Log(lDebug2)<<"Compiling model..";
    Log(lDebug)<<"\nExecuting compile command: "<<exeCmd;

    if(!Compile(exeCmd))
    {
        Log(lError)<<"Creating DLL failed..";
        throw Exception("Creating Model DLL failed..");
    }

    //Check if the DLL exists...
    return FileExists(mDLLFileName);
}

bool Compiler::setCompilerLocation(const string& path)
{
	if(!FolderExists(path))
	{
		Log(lError)<<"Tried to set invalid path: "<<path<<" for compiler location";
		return false;
	}
	mCompilerLocation = path;
	return true;
}

string	Compiler::getCompilerLocation()
{
	return mCompilerLocation;
}

bool Compiler::setSupportCodeFolder(const string& path)
{
	if(!FolderExists(path))
	{
		Log(lError)<<"Tried to set invalid path: "<<path<<" for compiler location";
		return false;
	}
	mSupportCodeFolder = path;
	return true;
}

string	Compiler::getSupportCodeFolder()
{
	return mSupportCodeFolder;
}

bool Compiler::SetupCompilerEnvironment()
{
    mIncludePaths.clear();
    mLibraryPaths.clear();
    mCompilerFlags.clear();
    if(ExtractFileNameNoExtension(mCompilerName) == "tcc")
    {
        mIncludePaths.push_back(".");
        mIncludePaths.push_back(JoinPath(mCompilerLocation, "include"));
        mLibraryPaths.push_back(".");
		mLibraryPaths.push_back(JoinPath(mCompilerLocation, "lib"));

        mCompilerFlags.push_back("-g");         //-g adds runtime debug information
        mCompilerFlags.push_back("-shared");
        mCompilerFlags.push_back("-rdynamic");  //-rdynamic : Export global symbols to the dynamic linker
                                                //-b : Generate additional support code to check memory allocations and array/pointer bounds. `-g' is implied.

        //LogLevel                              //-v is for verbose
        switch(gLog.GetLogLevel())
        {
            case lInfo:		mCompilerFlags.push_back("-v");           break;
            case lDebug:   mCompilerFlags.push_back("-vv");           break;
            case lDebug1:   mCompilerFlags.push_back("-vvv");         break;
        }
    }
    else if(mCompilerName == "bcc")
    {

    }

    mIncludePaths.push_back(mSupportCodeFolder);
    return true;
}

string Compiler::CreateCompilerCommand(const string& sourceFileName)
{
    stringstream exeCmd;
    if(ExtractFileNameNoExtension(mCompilerName) == "tcc")
    {
        exeCmd<<JoinPath(mCompilerLocation, mCompilerName);
        //Add compiler flags
        for(int i = 0; i < mCompilerFlags.size(); i++)
        {
            exeCmd<<" "<<mCompilerFlags[i];
        }
        exeCmd<<" "<<sourceFileName<<" " \
        <<JoinPath(mSupportCodeFolder, "rrSupport.c");


        exeCmd<<" -o"<<mDLLFileName;
#if defined(WIN32)
		exeCmd<<" -DBUILD_MODEL_DLL ";
#endif
        //Add include paths
        for(int i = 0; i < mIncludePaths.size(); i++)
        {
            exeCmd<<" -I"<<mIncludePaths[i]<<" " ;
        }

        //Add library paths
        for(int i = 0; i < mLibraryPaths.size(); i++)
        {
            exeCmd<<" -L"<<mLibraryPaths[i]<<" " ;
        }
    }
    else if(mCompilerName == "bcc")
    {
        exeCmd<<"bcc32 -WD -DBUILD_MODEL_DLL ";
        exeCmd<<" -e"<<mDLLFileName<<" -vu +r:\\rrInstalls\\xe\\rr_support\\bcc.cfg " \
        <<sourceFileName \
        <<" r:\\rrInstalls\\xe\\rr_support\\rrSupport.c";

    }
    return exeCmd.str();
}

bool Compiler::Compile(const string& cmdLine)
{
	string toFile(cmdLine);
    toFile += " > ";
    string tmpFolder = rr::RoadRunner::getTempFileFolder();
    toFile += JoinPath(rr::RoadRunner::getTempFileFolder(), "compilation.log");
	int val = system(toFile.c_str());
    if(val ==0)
    {
	Log(lInfo)<<"Compile system call returned: "<<val;
		return true;
    }
    else
    {
	Log(lError)<<"Compile system call returned: "<<val;
    	return false;
    }
}

string getCompilerMessages()
{
    return "No messages yet";
}

} //namespace rr

