#ifndef rrPluginH
#define rrPluginH
#include <sstream>
#include <string>
#include "rrPluginsAPIExporter.h"
#include "rrPluginsAPISettings.h"
#include "rrCapabilities.h"

namespace rr
{
class RoadRunner;
}

namespace rrp
{
using namespace rr;
class PluginManager;

//Plugin callback functions
typedef void    (callback_cc *PluginCallBackFnc)(void* data1, void* data2);

using std::string;

class PLUGINS_API_DECLSPEC Plugin
{
    friend PluginManager;
    protected:
        string                          mName;
        string                          mLibraryName;
        string                          mAuthor;
        string                          mCategory;
        string                          mVersion;
        string                          mCopyright;
        string                          mImplementationLanguage;

                                        //! Boolean indicating if a user wants to terminate the work in a plugin
        bool                            mTerminate;
        bool                            mIsWorking; //Set to true if plugin is working

        /**
         * a pointer to a RoadRunner instance which the plugin
         * uses.
         */
        RoadRunner                     *mRR;
        const PluginManager            *mPM; //A plugin can't change a plugin manager..

        //Plugin callback functionality
        PluginCallBackFnc               mWorkStartedCB;
        PluginCallBackFnc               mWorkProgressCB;
        PluginCallBackFnc               mWorkFinishedCB;
        void                           *mWorkStartedData1;
        void                           *mWorkStartedData2;
        void                           *mWorkProgressData1;
        void                           *mWorkProgressData2;
        void                           *mWorkFinishedData1;
        void                           *mWorkFinishedData2;

        Capabilities                    mCapabilities;          //Container for parameter data that can be exchanged to/from the plugin by using parameters
        void                           *mClientData;            //Data passed trough the execute function,

    public:
                                        Plugin( const string& name = gEmptyString,
                                                const string& cat = gNoneString,
                                                RoadRunner* aRR = NULL,
                                                const string& language = gNoneString,
                                                const PluginManager* pm = NULL);

        virtual                        ~Plugin();

        string                          getName();
        void                            setLibraryName(const string& libName);
        string                          getLibraryName();
        string                          getAuthor();
        string                          getCategory();
        string                          getVersion();
        string                          getCopyright();

        //Misc.
        RoadRunner*                     getRoadRunnerInstance();

        //Plugin documentation
        string                          getInfo();
        string                          getExtendedInfo();
        virtual unsigned char*          getManualAsPDF() const;
        virtual unsigned int            getPDFManualByteSize();

        Capabilities*                   getCapabilities();
        Capability*                     getCapability(const string& name);
        string                          getCapabilitiesAsXML();

        Parameters*                     getParameters(Capability& capability); //Each capability has a set of parameters
        Parameters*                     getParameters(const string& nameOfCapability = ""); //Each capability has a set of parameters

        PluginParameter*                  getParameter(const string& param, const string& capability = "");
        bool                            setParameter(const string& nameOf, const char* value);

        PluginParameter*                  getParameter(const string& param, Capability& capability);
        bool                            setParameter(const string& nameOf, const char* value, Capability& capability);

                                        //! User has the ability to terminate the work in a plugin by calling terminate()
        void                            terminate();

                                        //! Check if we are in the process of being terminated
        bool                            isBeingTerminated();

                                        //!check if the plugin was terminated
        bool                            wasTerminated();

                                        //!Assign a roadrunner instance for the plugin to use
        bool                            assignRoadRunnerInstance(RoadRunner* rr);

        //Virtuals
        virtual bool                    assignPluginStartedCallBack(PluginCallBackFnc pluginStarted, void* userData1 = NULL, void* userData2 = NULL);
        virtual bool                    assignPluginProgressCallBack(PluginCallBackFnc pluginsProgress, void* userData1 = NULL, void* userData2 = NULL);
        virtual bool                    assignPluginFinishedCallBack(PluginCallBackFnc pluginsFinished, void* userData1 = NULL, void* userData2 = NULL);

        virtual string                  getResult();
        virtual bool                    isWorking();

        virtual bool                    resetPlugin();
        virtual bool                    setInputData(void* data);
        virtual string                  getStatus();

        //Pure virtuals
        virtual string                  getImplementationLanguage() = 0;
        virtual bool                    execute(void* clientData = NULL, bool inAThread = false) = 0;

};

}

#endif
