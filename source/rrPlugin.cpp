#pragma hdrstop
#include <sstream>
#include <iomanip>
#include "rrUtils.h"
#include "rrPlugin.h"
#include "rrParameter.h"
#include "../Wrappers/C/rrc_types.h" //We may want to move this header to the Source folder
//---------------------------------------------------------------------------
using namespace std;
namespace rr
{
Plugin::Plugin(const std::string& name, const std::string& cat, RoadRunner* aRR, PluginWorkStartedCB fn1, PluginWorkFinishedCB fn2)
:
mName(name),
mAuthor("Totte Karlsson"),
mCategory(cat),
mVersion("0.1"),
mCopyright("Totte Karlsson, Herbert Sauro, Systems Biology, UW 2012"),
mRR(aRR),
mWorkStartedCB(fn1),
mWorkFinishedCB(fn2),
mCapabilities(name, "<none>")
{}

Plugin::~Plugin()
{}


bool Plugin::resetPlugin()
{
	//Do whats needed in descendants
    return true;
}

bool Plugin::setInputData(void* data)
{
	//Do whats needed in descendants
    return true;
}

bool Plugin::assignCallbacks(PluginWorkStartedCB fnc1, PluginWorkFinishedCB fnc2, void* userData)
{
	mUserData = userData;
	mWorkStartedCB = fnc1;
	mWorkFinishedCB = fnc2;
	return true;
}

bool Plugin::isWorking()
{
	return false;
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

bool Plugin::setParameter(const string& nameOf, const char* value)
{
	if(!mCapabilities.count())
    {
    	return false;
    }

    return mCapabilities.setParameter(nameOf, value);
}

string Plugin::getName()
{
	return mName;
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

string Plugin::getInfo() //Obs. subclasses may over ride this function and add more info
{
    stringstream msg;
    msg<<setfill('.');
    msg<<setw(30)<<left<<"Name"<<mName<<"\n";
    msg<<setw(30)<<left<<"Author"<<mAuthor<<"\n";
    msg<<setw(30)<<left<<"Category"<<mCategory<<"\n";
    msg<<setw(30)<<left<<"Version"<<mVersion<<"\n";
    msg<<setw(30)<<left<<"Copyright"<<mCopyright<<"\n";

//	msg<<"=== Capabilities ====\n";
//    for(int i = 0; i < mCapabilities.count(); i++)
//    {
//    	if(mCapabilities[i])
//        {
//    		msg<< *(mCapabilities[i]);
//        }
//    }
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
    	//Capability cap = get
    }
    else	//Search all capabilities
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

