#ifndef rrCapabilitiesH
#define rrCapabilitiesH
#include <vector>
#include "rrPluginsAPIExporter.h"
#include "rrCapability.h"
#include "rrStringList.h"
//---------------------------------------------------------------------------

namespace rrp
{
using rr::StringList;
using std::vector;

/**
 * @internal
 * @deprecated
 */
class PLUGINS_API_DECLSPEC Capabilities
{
    protected:
        string                          mName;
        string                          mDescription;
        mutable
        vector<Capability*>             mCapabilities;

    public:
                                        Capabilities(const string& name, const string& description);
        void                            add(Capability& capability);

        StringList                      asStringList();
        string                          info() const;
        u_int                           count();
        void                            clear();
        Capability*                     operator[](int i);
        Capability*                     get(const string& capName);
        bool                            setParameter(const string& name, const string& value);

        PLUGINS_API_DECLSPEC
        friend ostream&                 operator<<(ostream& stream, const Capabilities& caps);
};

}
#endif


