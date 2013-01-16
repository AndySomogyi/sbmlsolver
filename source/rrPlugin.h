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
		string 		        mCategory;
		string				mName;
        string				mAuthor;
        string				mVersion;
        string				mCopyright;

    public:
	    					Plugin(const std::string& fName = EmptyString);
        				   ~Plugin();
		virtual bool 		Load();
		virtual bool		Unload();
        virtual bool		Execute() = 0;
        string				GetCategory();
        bool				SetCategory(const string& name);

};

}

#endif
