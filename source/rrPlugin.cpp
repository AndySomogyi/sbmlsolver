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
mWorkFinishedCB(fn2)
{}

Plugin::~Plugin()
{}

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

bool Plugin::setParameter(const string& nameOf, void* value, Capability& capability)
{

	//Go trough the parameter container and look for parameter
    for(int i = 0; i < capability.nrOfParameters(); i++)
    {
        BaseParameter* aParameter = const_cast<BaseParameter*>( &(capability[i]) );

        if(dynamic_cast< Parameter<int>* >(aParameter))
        {
	        Parameter<int> *aIntPar = dynamic_cast< Parameter<int>* >(aParameter);
            int *aVal = reinterpret_cast<int*>(value);
        	aIntPar->setValue( *aVal);
            return true;
        }
    }
	return false;
}

bool Plugin::setParameter(const string& nameOf, const char* value, Capability& capability)
{
	//Go trough the parameter container and look for parameter
    for(int i = 0; i < capability.nrOfParameters(); i++)
    {
        BaseParameter* aParameter = const_cast<BaseParameter*>( &(capability[i]) );

        if(dynamic_cast< Parameter<int>* >(aParameter))
        {
	        Parameter<int> *aIntPar = dynamic_cast< Parameter<int>* >(aParameter);
            int aVal = rr::ToInt(value);
        	aIntPar->setValue( aVal);
            return true;
        }
    }
	return false;
}

bool Plugin::setParameter(const string& nameOf, const char* value)
{
	if(!mCapabilities.size())
    {
    	return false;
    }
    Capability& capability = mCapabilities[0];

	//Go trough the parameter container and look for parameter
    for(int i = 0; i < capability.nrOfParameters(); i++)
    {
        BaseParameter* aParameter = const_cast<BaseParameter*>( &(capability[i]) );

        if(aParameter->mName == nameOf)
        {
            if(dynamic_cast< Parameter<int>* >(aParameter))
            {
                Parameter<int> *aIntPar = dynamic_cast< Parameter<int>* >(aParameter);
                int aVal = *((int*) value);
                aIntPar->setValue( aVal);
                return true;
            }

            if(dynamic_cast< Parameter<double>* >(aParameter))
            {
                Parameter<double> *aPar = dynamic_cast< Parameter<double>* >(aParameter);
                double aVal = * ((double*) value);
                aPar->setValue( aVal);
                return true;
            }

            if(dynamic_cast< Parameter<string>* >(aParameter))
            {
                Parameter<string> *aPar = dynamic_cast< Parameter<string>* >(aParameter);
                string aVal = * ((string*) value);
                aPar->setValue( aVal);
                return true;
            }

            if(dynamic_cast< Parameter<rrc::RRVector*>* >(aParameter))
            {
                Parameter<rrc::RRVector*> *aPar = dynamic_cast< Parameter<rrc::RRVector*>* >(aParameter);
                aPar->setValue( (rrc::RRVector*) value);
                return true;
            }

            if(dynamic_cast< Parameter<rrc::RRData*>* >(aParameter))
            {
                Parameter<rrc::RRData*> *aPar = dynamic_cast< Parameter<rrc::RRData*>* >(aParameter);
                aPar->setValue( (rrc::RRData*) value);
                return true;
            }

            if(dynamic_cast< Parameter< vector<string> >* >(aParameter))
            {
                Parameter< vector<string> > *aPar = dynamic_cast< Parameter< vector<string> >* >(aParameter);
                vector<string> val = * ((vector<string>*) value);
                aPar->setValue( val);
                return true;
            }
        }
    }

	return false;
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

	msg<<"=== Capabilities ====\n";
    for(int i = 0; i < mCapabilities.size(); i++)
    {
    	msg<<mCapabilities[i];
    }
    return msg.str();
}

vector<Capability>*	 Plugin::getCapabilities()
{
	return &mCapabilities;
}

Parameters* Plugin::getParameters(const string& capName)
{
	//Return parameters for capability with name
    for(int i = 0; i < mCapabilities.size(); i++)
    {
        if(mCapabilities[i].getName() == capName)
        {
            return mCapabilities[i].getParameters();
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
    	for(int i = 0; i < mCapabilities.size(); i++)
        {
        	if(mCapabilities[i].getParameter(para))
            {
            	return mCapabilities[i].getParameter(para);
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
	for(int i = 0; i < mCapabilities.size(); i++)
    {
		if(mCapabilities[i].getName() == name)
        {
        	return &(mCapabilities[i]);
        }
    }
    return NULL;
}

//PluginLogger::PluginLogger(vector<string>* container)
//:
//mLogs(container)
//{}
//
//PluginLogger::~PluginLogger()
//{
//	vector<string> lines = rr::SplitString(mStream.str(),"\n");
//
//    for(int i = 0; i < lines.size(); i++)
//    {
//	   mLogs->push_back(lines[i].c_str());
//
//    }
//}
//
//std::ostringstream& PluginLogger::Get()
//{
//	return mStream;
//}
//

}

