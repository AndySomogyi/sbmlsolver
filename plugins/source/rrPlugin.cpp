#pragma hdrstop
#include <sstream>
#include <iomanip>
#include "rrUtils.h"
#include "rrPlugin.h"
#include "rrParameter.h"
//#include "rrp_types.h" //We may want to move this header to the Source folder
//---------------------------------------------------------------------------

namespace rrp
{
using namespace std;
Plugin::Plugin( const string& name,
                const string& category,
                RoadRunner* aRR,
                PluginCallBackFnc fn1,
                PluginCallBackFnc fn2,
                PluginCallBackFnc fn3,
                const string& language,
                const PluginManager* pm)
:
mName(name),
mAuthor("Totte Karlsson"),
mCategory(category),
mVersion("0.1"),
mCopyright("Totte Karlsson, Herbert Sauro, Systems Biology, UW 2012"),
mRR(aRR),
mWorkStartedCB(fn1),
mWorkProgressCB(fn1),
mWorkFinishedCB(fn2),
mCapabilities(name, category),
mImplementationLanguage(language),
mPM(pm)
{}

Plugin::~Plugin()
{}

bool Plugin::resetPlugin()
{
    //Do whats needed in descendants
    return true;
}

bool Plugin::setInputData(void* userData)
{
    //Do whats needed in descendants
    mUserData = userData;
    return true;
}

bool Plugin::assignPluginStartedCallBack(PluginCallBackFnc startedFnc, void* userData)
{
    mUserData = userData;
    mWorkStartedCB = startedFnc;
    return true;
}

bool Plugin::assignPluginProgressCallBack(PluginCallBackFnc progress, void* userData)
{
    mUserData = userData;
    mWorkProgressCB = progress;
    return true;
}

bool Plugin::assignPluginFinishedCallBack(PluginCallBackFnc endFnc, void* userData)
{
    mUserData = userData;
    mWorkFinishedCB = endFnc;
    return true;
}

bool Plugin::assignCallBacks(PluginCallBackFnc start, PluginCallBackFnc progress, PluginCallBackFnc end, void* userData)
{
    mUserData = userData;
    mWorkStartedCB = start;
    mWorkProgressCB = progress;
    mWorkFinishedCB = end;
    return true;
}

bool Plugin::isWorking()
{
    return false;
}

void Plugin::setLibraryName(const string& libName)
{
    mLibraryName = libName;
}

bool Plugin::setParameter(const string& nameOf, const char* value)
{
    if(!mCapabilities.count())
    {
        return false;
    }

    string val(value);
    return mCapabilities.setParameter(nameOf, val);
}

bool Plugin::setParameter(const string& nameOf, const char* value, Capability& capability)
{
    //Go trough the parameter container and look for parameter
    for(int i = 0; i < capability.nrOfParameters(); i++)
    {
        BaseParameter* aPar = const_cast<BaseParameter*>( &(capability[i]) );

        if(aPar->mName == nameOf)
        {
//            if(dynamic_cast< Parameter<int>* >(aParameter))
//            {
//                Parameter<int> *aIntPar = dynamic_cast< Parameter<int>* >(aParameter);
//                int aVal = rr::ToInt(value);
                aPar->setValueFromString( value);
//                return true;
            //}
        }
    }
    return false;
}

string Plugin::getName()
{
    return mName;
}

string Plugin::getLibraryName()
{
    return mLibraryName;
}

string Plugin::getAuthor()
{
    return mAuthor;
}

string Plugin::getCategory()
{
    return mCategory;
}

string Plugin::getVersion()
{
    return mVersion;
}

string Plugin::getCopyright()
{
    return mCopyright;
}

string Plugin::getStatus()
{
    stringstream msg;
    msg<<"Has RoadRunner instance: ";
    if(mRR)
    {
        msg<<" True\n";
    }
    else
    {
        msg<<" False\n";
    }

    return msg.str();
}

string Plugin::getInfo() //Obs. subclasses may over ride this function and add more info
{
    stringstream msg;
    msg<<setfill('.');
    msg<<setw(30)<<left<<"Name"<<mName<<"\n";
    msg<<setw(30)<<left<<"Author"<<mAuthor<<"\n";
    msg<<setw(30)<<left<<"Category"<<mCategory<<"\n";
    msg<<setw(30)<<left<<"Version"<<mVersion<<"\n";
    msg<<setw(30)<<left<<"Copyright"<<mCopyright<<"\n";
    return msg.str();
}

string Plugin::getExtendedInfo()
{
    stringstream msg;
    msg<<getInfo();
    msg<<"\nCapabilities Info\n";
    msg<<(*getCapabilities());
    return msg.str();
}

Capabilities* Plugin::getCapabilities()
{
    return &mCapabilities;
}

Parameters* Plugin::getParameters(const string& capName)
{
    //Return parameters for capability with name
    for(int i = 0; i < mCapabilities.count(); i++)
    {
        if(mCapabilities[i]->getName() == capName)
        {
            return mCapabilities[i]->getParameters();
        }
    }

    return NULL;
}

Parameters* Plugin::getParameters(Capability& capability)
{
    return capability.getParameters();
}

BaseParameter* Plugin::getParameter(const string& para, const string& capability)
{
    //If capability string is empty, search all capabilites
    if(capability.size())
    {
        Capability* cap = mCapabilities.get(capability);
        return cap ? cap->getParameter(para) : NULL;
    }
    else    //Search all capabilities
    {
        for(int i = 0; i < mCapabilities.count(); i++)
        {
            if(mCapabilities[i]->getParameter(para))
            {
                return mCapabilities[i]->getParameter(para);
            }
        }
    }
    return NULL;
}

BaseParameter* Plugin::getParameter(const string& para, Capability& capability)
{
    return capability.getParameter(para);
}

Capability* Plugin::getCapability(const string& name)
{
    for(int i = 0; i < mCapabilities.count(); i++)
    {
        if(mCapabilities[i]->getName() == name)
        {
            return (mCapabilities[i]);
        }
    }
    return NULL;
}

string Plugin::getResult()
{
    return "This plugin don't have any result..";
}

}

