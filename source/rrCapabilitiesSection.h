#ifndef rrCapabilitiesSectionH
#define rrCapabilitiesSectionH
#include <string>
#include <vector>
#include "rrObject.h"
//---------------------------------------------------------------------------

namespace rr
{
class Capability;

class RR_DECLSPEC CapabilitiesSection : public rrObject
{
    protected:
        string                              mName;
        string                              mMethod;
        string                              mDescription;
        vector<const Capability*>           mCapabilities;

    public:
                                            CapabilitiesSection(const string& name, const string& method, const string& descr);
                                            CapabilitiesSection(const CapabilitiesSection& fromMe);
        void                                Add(const Capability* me);
        string                              AsString();
        u_int                               Count();
        const Capability&                   operator[](const int& i) const;
        string                              GetName();
        string                              GetDescription();
        string                              GetMethod();
};

}

#endif
