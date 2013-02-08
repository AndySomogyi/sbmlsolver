#ifndef rrPluginH
#define rrPluginH
#include <string>
#include "rrObject.h"
#include "rrExporter.h"

//X-platform shared library class
#include "Poco/SharedLibrary.h"
//---------------------------------------------------------------------------

/* Abstract plugin */
namespace rr
{
class RoadRunner;

const string noneStr = "<none>";
using Poco::SharedLibrary;
using std::string;
class RR_DECLSPEC Plugin : public rrObject
{
	protected:
		string				mName;
        string				mAuthor;
		string 		        mCategory;
        string				mVersion;
        string				mCopyright;
        RoadRunner		   *mRR;		//This is a handle to the roadRunner isntance, creating the plugin

    public:
	    					Plugin(const std::string& name = EmptyString, const std::string& cat = noneStr, RoadRunner* aRR = NULL);
        virtual 		   ~Plugin();	//Gotta be virtual!

        virtual string				GetName();
        virtual string				GetAuthor();
        virtual string				GetCategory();
        virtual string				GetVersion();
        virtual string				GetCopyright();
        virtual string              GetInfo();
        virtual bool		Execute() = 0;
};

}

#endif
