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
        std::string                             mLibName;
        std::string                            mPathToLib;
        SharedLibrary                    mTheLib;

    public:
                                        ModelSharedLibrary(const std::string& pathToLib = "");
                                       ~ModelSharedLibrary();

        bool                            setPath(const std::string& pathTo);
        std::string                            createName(const std::string& name = "");
        std::string                            getName();
        std::string                            getFullFileName();

        bool                            load();
        bool                            load(const std::string& name);
        bool                            unload();
        bool                            isLoaded();
        void*                            getSymbol(const std::string& name);
        bool                            hasSymbol(const std::string& name);

};
}
#endif
