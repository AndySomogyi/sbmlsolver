#ifndef rrCapabilityH
#define rrCapabilityH
#include <string>
#include <vector>
#include "rrPluginsAPIExporter.h"
#include "rrParameters.h"
//---------------------------------------------------------------------------

namespace rrp
{

/**
 * @internal
 * @deprecated
 */
class PLUGINS_API_DECLSPEC Capability
{
    protected:
        string                              mName;
        string                              mDescription;
        string                              mMethod;
        Parameters                          mParameters;

    public:
                                            Capability(const string& name, const string& method, const string& descr);
                                            ~Capability();
                                            Capability(const Capability& fromMe);

        void                                setup(const string& name, const string& method, const string& descr);
        void                                addParameter(PluginParameter* me);
        string                              asString() const;
        u_int                               nrOfParameters() const;
        PluginParameter&                      operator[](const int& i);
        string                              getName() const;
        string                              getDescription() const;
        string                              getMethod() const;
        Parameters*                         getParameters();
        PluginParameter*                      getParameter(const string& paraName);

        PLUGINS_API_DECLSPEC
        friend ostream&                     operator <<(ostream& os, const Capability& caps);
};

}
#endif
