#ifndef rrModelSharedLibraryH
#define rrModelSharedLibraryH
//---------------------------------------------------------------------------
#include "Poco/SharedLibrary.h"
#include "rrExporter.h"
#include <string>



namespace rr
{

using std::string;
using Poco::SharedLibrary;

/**
 * Access an actual compiled shared library (.so, .dll or .dylib) that
 * was compiled by a ModelGenerator and provides access to the exported
 * C functions.
 */
class RR_DECLSPEC ModelSharedLibrary
{
    protected:
        string                             mLibName;
        string                            mPathToLib;
        SharedLibrary                    mTheLib;

    public:
                                        ModelSharedLibrary(const string& pathToLib = "");
                                       ~ModelSharedLibrary();

        bool                            setPath(const string& pathTo);
        string                            createName(const string& name = "");
        string                            getName();
        string                            getFullFileName();

        bool                            load();
        bool                            load(const string& name);
        bool                            unload();
        bool                            isLoaded();
        void*                            getSymbol(const string& name);
        bool                            hasSymbol(const string& name);

};
}
#endif
