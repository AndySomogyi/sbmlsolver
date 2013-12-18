#pragma hdrstop
#include "rrp_api.h"
#include <sstream>
#include "rrParameter.h"
#include "rrRoadRunner.h"
#include "rrPluginManager.h"
#include "rrPlugin.h"
#include "rrLogger.h"
#include "rrException.h"
#include "rrUtils.h"
#include "rrStringUtils.h"
#include "rrc_api.h"
#include "rrc_utilities.h"
#include "rrc_macros.h"
#include "rrc_cpp_support.h"
#include "rrp_cpp_support.h"
#include "rrp_parameter_api.h"

namespace rrp
{
using namespace std;
using namespace rr;
using namespace rrc;

char*       gLastError                  = NULL;

RRPluginManagerHandle rrp_cc createPluginManager(const char* _pluginDir)
{
    start_try
        string pluginDir;
        if(!_pluginDir)
        {
            pluginDir = ".";
        }
        else
        {
            pluginDir = _pluginDir;
        }

        PluginManager* pm = new PluginManager(pluginDir);
        return pm;
    catch_ptr_macro
}


/**
 * free the plugin manager
 */
bool rrp_cc freePluginManager(RRPluginManagerHandle handle)
{
    start_try
        PluginManager *pm = castToPluginManager(handle);
        delete pm;
        return true;
    catch_bool_macro
}

//PLUGIN Functions

RRPluginHandle rrp_cc getFirstPlugin(RRPluginManagerHandle handle)
{
    start_try
        PluginManager *pm = castToPluginManager(handle);
        return pm->getFirstPlugin();
    catch_ptr_macro
}

RRPluginHandle rrp_cc getNextPlugin(RRPluginManagerHandle handle)
{
    start_try
        PluginManager *pm = castToPluginManager(handle);
        return pm->getNextPlugin();
    catch_ptr_macro
}

RRPluginHandle rrp_cc getPreviousPlugin(RRPluginManagerHandle handle)
{
    start_try
        PluginManager *pm = castToPluginManager(handle);
        return pm->getPreviousPlugin();
    catch_ptr_macro
}

RRPluginHandle rrp_cc getCurrentPlugin(RRPluginManagerHandle handle)
{
    start_try
        PluginManager *pm = castToPluginManager(handle);
        return pm->getCurrentPlugin();
    catch_ptr_macro
}

RRPluginHandle rrp_cc loadPlugin(RRPluginManagerHandle handle, const char* pluginName)
{
    start_try
        PluginManager *pm = castToPluginManager(handle);
        if(pm->load(pluginName))
        {
            return pm->getPlugin(pluginName);
        }
        else
        {
            setError("Failed loading plugin: " + string(pluginName));
            return NULL;
        }
    catch_ptr_macro
}

bool rrp_cc loadPlugins(RRPluginManagerHandle handle)
{
    start_try
        PluginManager *pm = castToPluginManager(handle);
        return pm->load();
    catch_bool_macro
}

bool rrp_cc unLoadPlugins(RRPluginManagerHandle handle)
{
    start_try
        PluginManager *pm = castToPluginManager(handle);
        return pm->unload();
    catch_bool_macro
}

bool rrp_cc unLoadPlugin(RRPluginManagerHandle handle, RRPluginHandle plugin)
{
    start_try
        PluginManager *pm = castToPluginManager(handle);
        Plugin* aPlugin = castToPlugin(plugin);
        if(pm->unload(aPlugin))
        {
            aPlugin = NULL;
            return true;
        }
        else
        {
            return false;
        }
    catch_bool_macro
}

int rrp_cc getNumberOfPlugins(RRPluginManagerHandle handle)
{
    start_try
        PluginManager *pm = castToPluginManager(handle);
        return pm->getNumberOfPlugins();
    catch_int_macro
}

RRPluginHandle rrp_cc getPlugin(RRPluginManagerHandle handle, const char* pluginName)
{
    start_try
        PluginManager *pm = castToPluginManager(handle);
        Plugin* aPlugin = pm->getPlugin(pluginName);
        return aPlugin;
    catch_ptr_macro
}

//long rrp_cc getPluginSharedLibHandle(RRPluginManagerHandle handle, RRPluginHandle pluginName)
//{
//    start_try
//        PluginManager *pm = castToPluginManager(handle);
//        Plugin* aPlugin = pm->getPlugin(pluginName);
//
//        return aPlugin;
//    catch_ptr_macro
//}

RRHandle rrp_cc getRRHandleFromPlugin(RRPluginHandle handle)
{
    start_try
        Plugin* aPlugin = castToPlugin(handle);
        return aPlugin->getRoadRunnerInstance();
    catch_ptr_macro
}

char* rrp_cc getPluginNames(RRPluginManagerHandle handle)
{
    start_try
        PluginManager *pm = castToPluginManager(handle);
        StringList names = pm->getPluginNames();
        return createText(names.AsString());
    catch_ptr_macro
}

char* rrp_cc getPluginLibraryNames(RRPluginManagerHandle handle)
{
    start_try
        PluginManager *pm = castToPluginManager(handle);
        StringList names = pm->getPluginLibraryNames();
        return createText(names.AsString());
    catch_ptr_macro
}

char* rrp_cc getPluginName(RRPluginHandle handle)
{
    start_try
        Plugin* aPlugin = castToPlugin(handle);
        return rr::createText(aPlugin->getName());
    catch_ptr_macro
}

RRParametersHandle rrp_cc getPluginParameters(RRPluginHandle handle)
{
    start_try
        Plugin* aPlugin = castToPlugin(handle);
        return aPlugin->getParameters();
    catch_ptr_macro
}

char* rrp_cc getListOfPluginParameterNames(RRPluginHandle handle)
{
    start_try
        Plugin* aPlugin = castToPlugin(handle);        
        return createText(aPlugin->getParameterNames().AsString());
    catch_ptr_macro
}

RRParameterHandle rrp_cc getPluginParameter(RRPluginHandle handle, const char* parameterName)
{
    start_try
        Plugin* aPlugin = castToPlugin(handle);
        PluginParameter *para = NULL;
        if(aPlugin)
        {           
            return aPlugin->getParameter(parameterName);
        }
        return NULL;
    catch_ptr_macro
}

bool rrp_cc setPluginParameter(RRPluginHandle handle, const char* parameterName, const char* value)
{
    start_try
        Plugin* aPlugin = castToPlugin(handle);
        PluginParameter* aParameter = (PluginParameter*) getPluginParameter(aPlugin, parameterName);
        return setParameterByString(aParameter, value);
    catch_bool_macro
}

char* rrp_cc getPluginInfo(RRPluginHandle handle)
{
    start_try
        Plugin* aPlugin = castToPlugin(handle);
        return rr::createText(aPlugin->getInfo());
    catch_ptr_macro
}

unsigned int rrp_cc getPluginManualNrOfBytes(RRPluginHandle handle)
{
    start_try
        Plugin* aPlugin = castToPlugin(handle);
        return aPlugin->getPDFManualByteSize();
    catch_ptr_macro
}

unsigned char* rrp_cc getPluginManualAsPDF(RRPluginHandle handle)
{
    start_try
        Plugin* aPlugin = castToPlugin(handle);
        return aPlugin->getManualAsPDF();
    catch_ptr_macro
}

char* rrp_cc getPluginStatus(RRPluginHandle handle)
{
    start_try
        Plugin* aPlugin = castToPlugin(handle);
        return rr::createText(aPlugin->getStatus());
    catch_ptr_macro
}

bool rrp_cc assignRoadRunnerInstance(RRPluginHandle pHandle, RRHandle rrHandle)
{
    start_try
        Plugin* aPlugin = castToPlugin(pHandle);
        return (aPlugin) ? aPlugin->assignRoadRunnerInstance(static_cast<RoadRunner*>(rrHandle)) : false;
    catch_bool_macro
}

bool rrp_cc executePlugin(RRPluginHandle handle)
{
    return executePluginEx(handle, false);
}

bool rrp_cc executePluginEx(RRPluginHandle handle, bool inAThread)
{
    start_try
        Plugin* aPlugin = castToPlugin(handle);
        return (aPlugin) ? aPlugin->execute(inAThread) : false;
    catch_bool_macro
}

bool rrp_cc assignOnStartedEvent(RRPluginHandle handle, PluginEvent theCB, void* userData1, void* userData2)
{
    start_try
        Plugin* aPlugin = castToPlugin(handle);
        return (aPlugin) ? aPlugin->assignOnStartedEvent(theCB, userData1, userData2) : false;
    catch_bool_macro
}

bool rrp_cc assignOnProgressEvent(RRPluginHandle handle, PluginEvent theCB, void* userData1, void* userData2)
{
    start_try
        Plugin* aPlugin = castToPlugin(handle);
        return (aPlugin) ? aPlugin->assignOnProgressEvent(theCB, userData1, userData2) : false;
    catch_bool_macro
}

bool rrp_cc assignOnFinishedEvent(RRPluginHandle handle, PluginEvent theCB, void* userData1, void* userData2)
{
    start_try
        Plugin* aPlugin = castToPlugin(handle);
        return (aPlugin) ? aPlugin->assignOnFinishedEvent(theCB, userData1, userData2) : false;
    catch_bool_macro
}

char* rrp_cc getPluginResult(RRPluginHandle handle)
{
    start_try
        Plugin* aPlugin = castToPlugin(handle);
        return rr::createText(aPlugin->getResult());
    catch_ptr_macro
}

bool rrp_cc assignPluginInput(RRPluginHandle handle, void* data)
{
    start_try
        Plugin* aPlugin = castToPlugin(handle);
        return (aPlugin) ? aPlugin->assignInput(data) : false;
    catch_bool_macro
}

bool rrp_cc resetPlugin(RRPluginHandle handle)
{
    start_try
        Plugin* aPlugin = castToPlugin(handle);
        return aPlugin->resetPlugin();
    catch_bool_macro
}

bool rrp_cc isBeingTerminated(RRPluginHandle handle)
{
    start_try
        Plugin* aPlugin = castToPlugin(handle);
        return aPlugin->isBeingTerminated();
    catch_bool_macro
}

void rrp_cc terminateWork(RRPluginHandle handle)
{
    start_try
        Plugin* aPlugin = castToPlugin(handle);
        aPlugin->terminate();
    catch_void_macro
}

bool rrp_cc wasTerminated(RRPluginHandle handle)
{
    start_try
        Plugin* aPlugin = castToPlugin(handle);
        return aPlugin->wasTerminated();
    catch_bool_macro
}

bool rrp_cc isPluginWorking(RRPluginHandle handle)
{
    start_try
        Plugin* aPlugin = castToPlugin(handle);
        return aPlugin->isWorking();
    catch_bool_macro
}

RRDataHandle rrp_cc getRoadRunnerDataHandle(RRHandle handle)
{
    start_try
        RoadRunner* rri = castToRoadRunner(handle);
        return rri->getSimulationResult();
    catch_ptr_macro
}

RRCDataPtr rrp_cc createRRCData(RRDataHandle rrDataHandle)
{
    start_try
        RoadRunnerData* data = rrc::castToRRData(rrDataHandle);
        return rrp::createRRCData((*data));
    catch_ptr_macro
}

int rrp_cc  getRRCDataNumRows (RRCDataPtr result)
{
    if (result == NULL)
    {
       setError ("result argument is null in getResultNumRows");
       return -1;
    }
    return result->RSize;
}

int  rrp_cc  getRRCDataNumCols (RRCDataPtr result)
{
    if (result == NULL)
    {
       setError ("result argument is null in getResultNumCols");
       return -1;
    }
    return result->CSize;
}

bool rrp_cc getRRCDataElementF(RRCDataPtr result, int r, int c, double *value)
{
    return rrc::getRRCDataElement(result, r, c, value);
}

char* rrp_cc stringArrayToStringFWD(const RRStringArrayPtr list)
{
    return rrc::stringArrayToString(list);
}

char* rrp_cc getLastPluginError()
{   
    return rrc::getLastError();
}

bool rrp_cc freeText(char* text)
{
    return rr::freeText(text);
}

char* rrp_cc getPluginPropertiesAsXML(RRPluginHandle handle)
{
    start_try
        Plugin* aPlugin = castToPlugin(handle);
        return createText(aPlugin->getPluginPropertiesAsXML().c_str());
    catch_ptr_macro
}


bool rrp_cc getRoadRunnerDataElement(RRDataHandle data, int row, int col, double* value)
{
    start_try
        RoadRunnerData* rrData = castToRRData(data);
        const DoubleMatrix& theData = rrData->getData();
        *value = theData.Element(row, col);
        return true;
    catch_bool_macro
}

char* rrp_cc getRoadRunnerDataColumnHeader(RRDataHandle _data)
{
    start_try
        RoadRunnerData* data = castToRRData(_data);
        return createText(data->getColumnNamesAsString());        
    catch_ptr_macro
}

int rrp_cc getRoadRunnerDataNumRows(RRDataHandle _data)
{
    start_try
        RoadRunnerData* data = castToRRData(_data);
        return data->rSize();        
    catch_int_macro
}

int rrp_cc getRoadRunnerDataNumCols(RRDataHandle _data)
{
    start_try
        RoadRunnerData* data = castToRRData(_data);
        return data->cSize();        
    catch_int_macro
}

RRDataHandle rrp_cc createRoadRunnerData(int nRows, int nCols, char* colNames)
{
    start_try
        RoadRunnerData* data = new RoadRunnerData(nRows, nCols);
        if (colNames)
        {
            string cNames(colNames);
            StringList colNames(cNames, ",");
            data->setColumnNames(colNames);
        }
        return data;        
    catch_ptr_macro
}

bool rrp_cc freeRoadRunnerData(RRDataHandle rrData)
{
    start_try
        RoadRunnerData* data = castToRRData(rrData);
        delete data;
        return true;
    catch_bool_macro
}

bool rrp_cc writeRoadRunnerDataToFile(RRDataHandle rrData, char* fName)
{
    start_try
        RoadRunnerData* data = castToRRData(rrData);
        return data->writeTo(fName);        
    catch_bool_macro
}

bool rrp_cc readRoadRunnerDataFromFile(RRDataHandle rrData, char* fName)
{
    start_try
        RoadRunnerData* data = castToRRData(rrData);
        return data->readFrom(fName);        
    catch_bool_macro

}


}
