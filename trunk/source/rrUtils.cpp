#ifdef USE_PCH
#include "rr_pch.h"
#endif
#pragma hdrstop
#include <windows.h>
#include <io.h>

#if defined(BORLANDC)
#include <dir.h>
#else
#include <direct.h>
#endif

#include <algorithm>
#include <iostream>
#include <conio.h>
#include <fstream>
#include <iomanip>
#include "rrStringUtils.h"
#include "rrUtils.h"
#include "rrLogger.h"
#include "rrMisc.h"
//---------------------------------------------------------------------------
using namespace std;
namespace rr
{

string GetUsersTempDataFolder()
{
    //Default for temporary data output is the users AppData/Local/Temp Folder
    //  Gets the temp path env string (no guarantee it's a valid path).
    TCHAR lpTempPathBuffer[MAX_PATH];
    DWORD dwRetVal = GetTempPath(   MAX_PATH,
                                    lpTempPathBuffer); // buffer for path
    if (dwRetVal > MAX_PATH || (dwRetVal == 0))
    {
        Log(lError)<<"GetTempPath failed";
    }
    else
    {
        Log(lDebug3)<<"Users temporary files folder is: "<<string(lpTempPathBuffer);
    }
    return string(lpTempPathBuffer);
}

string getWorkingDirectory()
{
    //Get the working directory
	char* buffer;
	string cwd;
	// Get the current working directory: 
	if( (buffer = _getcwd( NULL, 0 )) == NULL )
	{
		Log(lError)<<"getWorkingDirectory failed";
		return "";
	}
	else
	{
      cwd = buffer;
      free(buffer);
	}
   
	return cwd;
}

string GetFileContent(const string& fName)
{
	string content;

    vector<string> lines = GetLinesInFile(fName);
    for(int i = 0; i < lines.size(); i++)
    {
        content += lines[i];
        content += "\n";
    }

    return content;
}

vector<string> GetLinesInFile(const string& fName)
{
    vector<string> lines;

    ifstream ifs(fName.c_str());
    if(!ifs)
    {
        Log(lError)<<"Failed opening file: "<<fName;
        return lines;
    }

    std::string oneLine((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    lines = SplitString(oneLine, "\n");
    return lines;
}

std::size_t IndexOf(std::vector<std::string>& vec, const std::string& elem )
{
    int index = distance(vec.begin(), find(vec.begin(), vec.end(), elem));
    return index;
}

// String utils
string RemoveTrailingSeparator(const string& _folder, const string& sep)
{
    if((_folder.size() > 0) && (sep.size() > 0) && (_folder[_folder.size() -1] == sep[0]))
    {
        const int endOfPathIndex = _folder.rfind(sep, _folder.size());
        string folder = _folder.substr(0, endOfPathIndex);
        return folder;
    }
    else
    {
        return _folder;
    }
}

bool IsNaN(const double& aNum)
{
    return _isnan(aNum) > 0 ? true : false;
}

bool IsNullOrEmpty(const string& str)
{
    return !str.size();
}

void Pause(bool doIt, const string& msg)
{
    if(!doIt)
    {
        return;
    }

    if(msg.size() == 0)
    {
    	cout<<"Hit any key to exit...";
    }
    else
    {
    	cout<<msg;
    }
    cin.ignore(0,'\n');
    getch();
}

bool FileExists(const string& fName)
{
    if (!fName.size())
    {
        return false;
    }
    bool res = (access(fName.c_str(), 0) == 0);
    return res;
}

bool FolderExists(const string& folderName)
{
    LPCTSTR szPath = folderName.c_str();
    DWORD dwAttrib = GetFileAttributes(szPath);
    return (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

void CreateTestSuiteFileNameParts(int caseNr, const string& postFixPart, string& modelFilePath, string& modelName)
{
    stringstream modelSubPath;
    stringstream modelFileName;
    modelSubPath<<setfill('0')<<setw(5)<<caseNr;        //create the "00023" subfolder format
    modelFileName<<setfill('0')<<setw(5)<<caseNr<<postFixPart;
    modelFilePath = modelFilePath + "\\" + modelSubPath.str();
    modelName =  modelFileName.str();
}

string GetTestSuiteSubFolderName(int caseNr)
{
    stringstream modelSubPath;
    modelSubPath<<setfill('0')<<setw(5)<<caseNr;        //create the "00023" subfolder format
    return modelSubPath.str();
}

bool CreateFolder(const string& folder)
{
    if(FileExists(folder))
    {
        return true;
    }

    int res = mkdir(folder.c_str());
    return (res==0) ? true : false;
}

bool CopyValues(vector<double>& dest, double* source, const int& nrVals, const int& startIndex)
{
    if(!dest.size() || !source || startIndex > dest.size())
    {
        return false;
    }

    for(int i = startIndex; i < nrVals; i++)
    {
        dest[i] = source[i];
    }

    return true;

}

bool CopyStdVectorToCArray(const vector<double>& src, double* dest,  int size)
{
    if(!dest || size > src.size())
    {
        Log(lError)<<"Tried to copy to NULL vector, or incompatible size of vectors";
        return false;
    }

    for(int i = 0; i < size; i++)
    {
        dest[i] = src[i];
    }
    return true;
}

bool CopyStdVectorToCArray(const vector<bool>&   src,  bool*  dest,  int size)
{
    if(!dest || size > src.size())
    {
        Log(lError)<<"Tried to copy to NULL vector, or incompatible size of vectors";
        return false;
    }


    for(int i = 0; i < size; i++)
    {
        dest[i] = src[i];
    }
    return true;
}

vector<double> CreateVector(const double* src, const int& size)
{
    vector<double> dest;
    if(!src)
    {
        Log(lError)<<"Tried to copy from NULL vector";
        return dest;
    }

    dest.resize(size);
    for(int i = 0; i < size; i++)
    {
        dest[i] = src[i];
    }
    return dest;
}

bool CopyCArrayToStdVector(const int* src, vector<int>& dest, int size)
{
    if(!src)
    {
        Log(lError)<<"Tried to copy from NULL vector";
        return false;
    }

    dest.resize(size);
    for(int i = 0; i < size; i++)
    {
        dest[i] = src[i];
    }
    return true;
}

bool CopyCArrayToStdVector(const double* src, vector<double>& dest, int size)
{
    if(!src)
    {
        Log(lError)<<"Tried to copy from NULL vector";
        return false;
    }

    dest.resize(size);
    for(int i = 0; i < size; i++)
    {
        dest[i] = src[i];
    }
    return true;
}

bool CopyCArrayToStdVector(const bool* src, vector<bool>& dest, int size)
{
    if(!src)
    {
        Log(lError)<<"Tried to copy from NULL vector";
        return false;
    }

    dest.resize(size);
    for(int i = 0; i < size; i++)
    {
        dest[i] = src[i];
    }
    return true;
}

double* CreateVector(const vector<double>& vec)
{
    double* avec = new double[vec.size()];
    if(!avec)
    {
        Log(lError)<<"Failed to allocate c vector";
        return NULL;
    }

    for(int i = 0; i < vec.size(); i++)
    {
          avec[i] = vec[i];
    }
    return avec;
}


StringList getSelectionListFromSettings(const SimulationSettings& settings)
{
	//read from settings the variables found in the amounts and concentrations lists
	StringList theList;
	TSelectionRecord record;

    int nrOfVars = settings.mVariables.Count();

	for(int i = 0; i < settings.mAmount.Count(); i++)
	{
		theList.Add("[" + settings.mAmount[i] + "]");        //In the setSelection list below, the [] selects the correct 'type'
	}

	for(int i = 0; i < settings.mConcentration.Count(); i++)
	{
		theList.Add(settings.mConcentration[i]);
	}

    //We may have variables
    //A variable 'exists' only in "variables", not in the amount or concentration section
    int currCount = theList.Count();
	if( nrOfVars > currCount)
	{
        //Look for a variable that is not in the list

        for(int i = 0; i < settings.mVariables.Count(); i++)
		{
            string aVar = settings.mVariables[i];
            if(settings.mAmount.DontContain(aVar) && settings.mConcentration.DontContain(aVar))
            {
			    theList.Add(settings.mVariables[i]);
            }

        }
    }

  	theList.InsertAt(0, "time");
    return theList;
}

HINSTANCE LoadDLL(const string& dll)
{
    HINSTANCE hLib = LoadLibraryA(dll.c_str());

    if(hLib == NULL)
    {
        Log(lError) << "Unable to load library!" << endl;
        return NULL;
    }

    TCHAR mod[MAX_MODULE];
    GetModuleFileNameA((HMODULE)hLib, (LPSTR) mod, MAX_MODULE);
    string name(mod);

    Log(lDebug) << "DLL Library loaded: " <<name.c_str() << endl;
    return hLib;
}

bool UnLoadDLL(HINSTANCE dllHandle)
{
	if(!dllHandle)
    {
    	return false;
    }

	BOOL result = FreeLibrary(dllHandle);
    if(result == false)
    {
        Log(lWarning) << "Failed to unload DLL library" << endl;
        return false;
    }

	return true;
}

FARPROC GetFunctionPtr(const string& funcName, HINSTANCE DLLHandle)
{
    FARPROC handle = GetProcAddress((HMODULE) DLLHandle, funcName.c_str());
    if(handle == NULL)
    {
        Log(lError) << "Unable to load the function: " << funcName;
        return NULL;
    }
    Log(lDebug3)<<"Loaded function " << funcName;
    return handle;
}




}//end of namespace
