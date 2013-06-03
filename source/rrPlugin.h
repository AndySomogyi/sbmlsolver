#ifndef rrPluginH
#define rrPluginH
#include <sstream>
#include <string>
#include "rrObject.h"
#include "rrExporter.h"
#include "rrCapability.h"
#include "rrCapabilities.h"
//---------------------------------------------------------------------------

/* Abstract plugin */
namespace rr
{

//Plugin callback functions
typedef void    (rrCallConv *PluginWorkStartedCB)(void*);
typedef void    (rrCallConv *PluginWorkFinishedCB)(void*);
class RoadRunner;

using std::string;
class RR_DECLSPEC Plugin : public rrObject
{
	protected:
		string			           	mName;
        string			           	mAuthor;
		string 		                mCategory;
        string			           	mVersion;
        string			           	mCopyright;
        RoadRunner		           *mRR;			//This is a handle to the roadRunner instance, creating the plugin

		//Plugin callbacks..
	    PluginWorkStartedCB			mWorkStartedCB;
	    PluginWorkFinishedCB        mWorkFinishedCB;
		void*						mUserData;

        Capabilities	   			mCapabilities;

    public:
	    				           	Plugin(const std::string& name = gEmptyString, const std::string& cat = gNoneString, RoadRunner* aRR = NULL, PluginWorkStartedCB fn1 = NULL, PluginWorkFinishedCB fn2 = NULL);
        virtual 		           ~Plugin();	//Gotta be virtual!

        bool						assignCallbacks(PluginWorkStartedCB fnc1, PluginWorkFinishedCB fnc2 = NULL, void* userData = NULL);
        string			           	getName();
        string			           	getAuthor();
        string			           	getCategory();
        string			           	getVersion();
        string			           	getCopyright();
        string                      getInfo();
        Capabilities* 				getCapabilities();
        Capability*   				getCapability(const string& name);

        Parameters*					getParameters(Capability& capability); //Each capability has a set of parameters
        Parameters*					getParameters(const string& nameOfCapability = ""); //Each capability has a set of parameters

        BaseParameter*				getParameter(const string& param, const string& capability = gEmptyString);
        BaseParameter*				getParameter(const string& param, Capability& capability);

        bool						setParameter(const string& nameOf, const char* value);
        bool						setParameter(const string& nameOf, const char* value, 	Capability& capability);

        //Logging
        vector<string>&				getLog();
        bool						emptyLog(); //Has to be made thread safe

		//Virtuals
        virtual string				getResult();
        virtual bool				isWorking();
        virtual bool				resetPlugin();
        virtual bool				setInputData(void* data);
		//Pure virtuals
        virtual bool	           	execute(void* userData = NULL) = 0;



};


}

#endif
