#ifndef rrPluginH
#define rrPluginH
#include <sstream>
#include <string>
#include "rrp_exporter.h"
#include "rrp_exporter.h"
#include "rrOSSpecifics.h"
#include "rrPluginExporter.h"
#include "rrCapability.h"
#include "rrCapabilities.h"
#include "Configurable.h"


namespace rr
{
//Plugin callback functions

#ifndef SWIG // these make SWIG really unhappy for some reason.
typedef void    (rrCallConv *PluginWorkStartedCB)(void*);
typedef void    (rrCallConv *PluginWorkFinishedCB)(void*);
#endif

class RoadRunner;

using std::string;

/**
 * @internal
 */
class RRP_DECLSPEC Plugin : public Configurable  /* Abstract plugin */
{
    protected:
        string                           mName;
        string                           mLibraryName;
        string                           mAuthor;
        string                         mCategory;
        string                           mVersion;
        string                           mCopyright;
        string                        mImplementationLanguage;

        /**
         * a borrowed reference to a RoadRunner instance which the plugin
         * uses. n
         */
        RoadRunner                   *mRR;

        //Plugin callbacks..
        PluginWorkStartedCB            mWorkStartedCB;
        PluginWorkFinishedCB        mWorkFinishedCB;
        void*                        mUserData;

        Capabilities                   mCapabilities;    //Container for parameter data that can be exchanged to/from the plugin

    public:
                                       Plugin(const std::string& name = "", const std::string& cat = gNoneString, RoadRunner* aRR = NULL, PluginWorkStartedCB fn1 = NULL, PluginWorkFinishedCB fn2 = NULL, const string& language = "<none>");
        virtual                    ~Plugin();    //Gotta be virtual!

        bool                        assignCallbacks(PluginWorkStartedCB fnc1, PluginWorkFinishedCB fnc2 = NULL, void* userData = NULL);
        string                           getName();
        void                        setLibraryName(const string& libName);
        string                           getLibraryName();
        string                           getAuthor();
        string                           getCategory();
        string                           getVersion();
        string                           getCopyright();
        string                      getInfo();
        Capabilities*                 getCapabilities();
        Capability*                   getCapability(const string& name);

        Parameters*                    getParameters(Capability& capability); //Each capability has a set of parameters
        Parameters*                    getParameters(const string& nameOfCapability = ""); //Each capability has a set of parameters

        BaseParameter*                getParameter(const string& param, const string& capability = "");
        bool                        setParameter(const string& nameOf, const char* value);

        BaseParameter*                getParameter(const string& param, Capability& capability);
        bool                        setParameter(const string& nameOf, const char* value,     Capability& capability);


        //Virtuals
        virtual string                getResult();
        virtual bool                isWorking();
        virtual bool                resetPlugin();
        virtual bool                setInputData(void* data);
        virtual string                getStatus();

        //Pure virtuals
        virtual string                getImplementationLanguage() = 0;
        virtual bool                   execute(void* userData = NULL) = 0;
};


}

#endif
