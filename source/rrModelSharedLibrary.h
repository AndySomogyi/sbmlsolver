#ifndef rrModelSharedLibraryH
#define rrModelSharedLibraryH
//---------------------------------------------------------------------------
#include "Poco/SharedLibrary.h"
#include "rrObject.h"

namespace rr
{

class RR_DECLSPEC ModelSharedLibrary : public Poco::SharedLibrary, public rrObject
{
	protected:
		string 							mSharedLibFileName;
		string							mPathToLib;
    public:
										ModelSharedLibrary(const string& pathToLib = EmptyString);
		bool							setPath(const string& pathTo);
                                       ~ModelSharedLibrary();
		string							getFileName();
		string							getFullFileName();

        bool							load();
    	string							createName();

};
}
#endif
