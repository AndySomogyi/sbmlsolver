#ifndef rrUtilsH
#define rrUtilsH
#if defined(_WIN32)
#include <windows.h>
#endif

#include <float.h>    //ms compatible IEEE functions, e.g. _isnan
#include <vector>
#include <string>
#include <iostream>
#include <set>
#include "rrExporter.h"
#include "rrConstants.h"
#include "rrStringList.h"
#include "rrSimulationSettings.h"
#include "rrStringUtils.h"
//---------------------------------------------------------------------------

namespace rr
{

using std::vector;
using std::string;
using std::set;

RR_DECLSPEC double 			gaussNoise(double mean, double sigma);
RR_DECLSPEC string 			getTime();
RR_DECLSPEC string 			getDateTime();
RR_DECLSPEC string 			getMD5(const string& text);
RR_DECLSPEC void		 	sleep(int ms);
//Misc.
RR_DECLSPEC std::size_t     indexOf(std::vector<std::string>& vec, const std::string& elem );
RR_DECLSPEC bool            isNaN(const double& aNum);
RR_DECLSPEC bool            isNullOrEmpty(const string& str);    //Can't be null, but empty
RR_DECLSPEC void            pause(bool doIt = true, const string& msg = gEmptyString);

//String utilities
RR_DECLSPEC string          removeTrailingSeparator(const string& fldr, const char sep = gPathSeparator);//"\\");

//File  Utilities
RR_DECLSPEC int 			populateFileSet(const string& modelsFolder, set<string>& models);
RR_DECLSPEC bool            fileExists(const string& fileN);
RR_DECLSPEC bool            folderExists(const string& folderN);
RR_DECLSPEC bool            createFolder(const string& path);

RR_DECLSPEC string			getParentFolder(const string& path);
RR_DECLSPEC string    		getCurrentExeFolder();
RR_DECLSPEC string          getUsersTempDataFolder();
RR_DECLSPEC string			getCWD();
RR_DECLSPEC const char		getPathSeparator();

RR_DECLSPEC vector<string>  getLinesInFile(const string& fName);
RR_DECLSPEC string  		getFileContent(const string& fName);
RR_DECLSPEC void            createTestSuiteFileNameParts(int caseNr, const string& postFixPart, string& FilePath, string& modelFileName, string& settingsFileName);
RR_DECLSPEC string          getTestSuiteSubFolderName(int caseNr);

//CArray utilities
RR_DECLSPEC bool            copyCArrayToStdVector(const int* src,     vector<int>& dest, int size);

RR_DECLSPEC bool            copyCArrayToStdVector(const double* src,  vector<double>& dest, int size);
RR_DECLSPEC double*         createVector(const vector<double>& vec);

RR_DECLSPEC bool            copyValues(vector<double>& dest, double* source, const int& nrVals, const int& startIndex);

RR_DECLSPEC vector<double>  createVector(const double* src, const int& size);

RR_DECLSPEC bool            copyCArrayToStdVector(const bool* src,    vector<bool>& dest, int size);
RR_DECLSPEC bool            copyStdVectorToCArray(const vector<double>& src, double* dest,  int size);
RR_DECLSPEC bool            copyStdVectorToCArray(const vector<bool>&   src,  bool*  dest,  int size);

//SelectionList
RR_DECLSPEC StringList      getSelectionListFromSettings(const SimulationSettings& settings);

#if defined(WIN32)
RR_DECLSPEC HINSTANCE       loadDLL(const string& dll);
RR_DECLSPEC bool       		unLoadDLL(HINSTANCE dllHandle);
RR_DECLSPEC FARPROC 		getFunctionPtr(const string& funcName, HINSTANCE DLLHandle);
RR_DECLSPEC string 			getWINAPIError(DWORD errorCode, LPTSTR lpszFunction);
#endif

#undef CreateFile
RR_DECLSPEC bool     		createFile(const string& fName, std::ios_base::openmode mode = std::ios::trunc );

} // rr Namespace
#endif
