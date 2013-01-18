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

    public:
	    					Plugin(const std::string& name = EmptyString, const std::string& cat = EmptyString);
        				   ~Plugin();

        string				GetName();
        string				GetAuthor();
        string				GetCategory();
        string				GetVersion();
        string				GetCopyright();
        string              GetInfo();
        virtual bool		Execute() = 0;

};

}

#endif
