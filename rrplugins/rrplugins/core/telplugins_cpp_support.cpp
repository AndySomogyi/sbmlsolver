#pragma hdrstop
#include "rrplugins/pluginBaseClass/telPlugin.h"
#include "rrplugins/common/telStringUtils.h"
#include "telAPIHandleManager.h"
#include "telTelluriumData.h"
#include "telplugins_cpp_support.h"

namespace tlpc
{
using namespace tlp;

//A global handle manager
APIHandleManager gHM;

char* gLastError = NULL;

void tpSetError(const string& err)
{
    if(gLastError)
    {
        delete [] gLastError;
    }

    gLastError = tlp::createText(err);
}

Plugin* tpRegisterPlugin(Plugin* plugin)
{
    gHM.registerHandle(plugin, typeid(Plugin*).name());

    //Register any attributes of the plugin that may be
    //accessed trough the API
    //Each plugin must have a list of properties
    Properties* props = plugin->getProperties();
    gHM.registerHandle(props, typeid(props).name());

    //The property container do have properties
    PropertyBase* prop = props->getFirst();
    while(prop)
    {
        gHM.registerHandle(prop, typeid(prop).name());

        //We also need to register any properties that may
        //reside inside a property that is of types that can be accessed byt he API, e.g. properties(listofProperties)
        if(prop->getType() == "listOfProperties")
        {
            Properties* propValue = (Properties*) prop->getValueHandle();
            gHM.registerHandle(propValue, typeid(propValue).name());
        }

        if(prop->getType() == "telluriumData")
        {
            TelluriumData* propValue = (TelluriumData*) prop->getValueHandle();
            gHM.registerHandle(propValue, typeid(propValue).name());
        }

        if(prop->getType() == "matrix")
        {
            ls::Matrix<double>* propValue = (ls::Matrix<double>*) prop->getValueHandle();
            gHM.registerHandle(propValue, typeid(propValue).name());
        }

        prop = props->getNext();
    }
    return plugin;
}

char*  tpCreateText(const string& str)
{
    char* textHandle = tlp::createText(str);
    return (char*) gHM.registerHandle(textHandle, typeid(textHandle).name());
}

}
