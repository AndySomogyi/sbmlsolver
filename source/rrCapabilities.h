#ifndef rrCapabilitiesH
#define rrCapabilitiesH
#include <vector>
#include "rrObject.h"
#include "rrCapability.h"
#include "rrStringList.h"
//---------------------------------------------------------------------------

namespace rr
{
using std::vector;

class RR_DECLSPEC Capabilities : public rrObject
{
    protected:
        string                          mName;
        string                          mDescription;
        vector<Capability*>   			mCapabilities;

    public:
                                        Capabilities(const string& name, const string& description);
        void                            add(Capability& capability);
        string                          asXML();
        StringList                      asStringList();
        u_int                           count();
        void							clear();
        Capability*	   					operator[](int i);
        Capability*						get(const string& capName);
		bool 							setParameter(const string& name, const string& value);
};

}
#endif


