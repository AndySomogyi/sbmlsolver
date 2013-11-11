#ifndef rrPluginH
#define rrPluginH
#include <sstream>
#include <string>
#include "rrPluginsAPIExporter.h"
#include "rrOSSpecifics.h"
#include "rrPluginsAPIExporter.h"
#include "rrCapabilities.h"
#include "Configurable.h"

namespace rr
{
class RoadRunner;
}

namespace rrp
{
using namespace rr;
class PluginManager;
//Plugin callback functions
#ifndef SWIG // these make SWIG really unhappy for some reason.
typedef void    (callback_cc *PluginCallBackFnc)(void*);
#endif

using std::string;

/**
 * @internal
 */
class PLUGINS_API_DECLSPEC Plugin : public Configurable  /* Abstract plugin */
{
    protected:
        string                          mName;
        string                          mLibraryName;
        string                          mAuthor;
        string                          mCategory;
        string                          mVersion;
        string                          mCopyright;
        string                          mImplementationLanguage;

        /**
         * a pointer to a RoadRunner instance which the plugin
         * uses.
         */
        RoadRunner                     *mRR;
        const PluginManager*            mPM; //A plugin can't change a plugin manager..


        //Plugin callbacks..
        PluginCallBackFnc               mWorkStartedCB;
        PluginCallBackFnc               mWorkProgressCB;
        PluginCallBackFnc               mWorkFinishedCB;
        void*                           mUserData;

        Capabilities                    mCapabilities;    //Container for parameter data that can be exchanged to/from the plugin

    public:
                                        Plugin(	const string& name = gEmptyString, 
												const string& cat = gNoneString, 
												RoadRunner* aRR = NULL, 
												PluginCallBackFnc fn1 = NULL, PluginCallBackFnc fn2 = NULL, PluginCallBackFnc fn3 = NULL, 
												const string& language = gNoneString, 
												const PluginManager* pm = NULL);

		virtual                        ~Plugin();    //Gotta be virtual!

        string                          getName();
        void                            setLibraryName(const string& libName);
        string                          getLibraryName();
        string                          getAuthor();
        string                          getCategory();
        string                          getVersion();
        string                          getCopyright();
        string                          getInfo();
        string                          getExtendedInfo();

        Capabilities*                   getCapabilities();
        Capability*                     getCapability(const string& name);

        Parameters*                     getParameters(Capability& capability); //Each capability has a set of parameters
        Parameters*                     getParameters(const string& nameOfCapability = ""); //Each capability has a set of parameters

        BaseParameter*                  getParameter(const string& param, const string& capability = "");
        bool                            setParameter(const string& nameOf, const char* value);

        BaseParameter*                  getParameter(const string& param, Capability& capability);
        bool                            setParameter(const string& nameOf, const char* value, Capability& capability);

        //Virtuals
        virtual bool                    assignPluginStartedCallBack(PluginCallBackFnc pluginStarted, void* userData = NULL);
        virtual bool                    assignPluginProgressCallBack(PluginCallBackFnc pluginsProgress, void* userData = NULL);
        virtual bool                    assignPluginFinishedCallBack(PluginCallBackFnc pluginsFinished, void* userData = NULL);
        virtual bool                    assignCallBacks(PluginCallBackFnc pluginStarted, PluginCallBackFnc pluginsProgress, PluginCallBackFnc pluginsFinished = NULL, void* userData = NULL);
        virtual string                  getResult();
        virtual bool                    isWorking();
        virtual bool                    resetPlugin();
        virtual bool                    setInputData(void* data);
        virtual string                  getStatus();

        //Pure virtuals
        virtual string                  getImplementationLanguage() = 0;
        virtual bool                    execute(void* userData = NULL, bool inAThread = false) = 0;
};

}

#endif
