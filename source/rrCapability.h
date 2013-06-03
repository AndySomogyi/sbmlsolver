#ifndef rrCapabilityH
#define rrCapabilityH
#include <string>
#include <vector>
#include "rrObject.h"
#include "rrParameters.h"
//---------------------------------------------------------------------------

namespace rr
{

class RR_DECLSPEC Capability : public rrObject
{
    protected:
        string                              mName;
        string                              mDescription;
        string                              mMethod;
        Parameters				            mParameters;

    public:
                                            Capability(const string& name, const string& method, const string& descr);
                                            Capability(const Capability& fromMe);
		void							 	setup(const string& name, const string& method, const string& descr);
        void                                addParameter(rr::BaseParameter* me);
        string                              asString() const;
        u_int                               nrOfParameters() const;
        rr::BaseParameter&            		operator[](const int& i);
        string                              getName() const;
        string                              getDescription() const;
        string                              getMethod() const;
        Parameters*							getParameters();
        rr::BaseParameter*	   				getParameter(const string& paraName);

		RR_DECLSPEC friend ostream&   		operator <<(ostream& os, const Capability& caps);
};

}
#endif
