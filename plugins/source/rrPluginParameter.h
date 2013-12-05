#ifndef rrPluginParameterH
#define rrPluginParameterH
#include "rrPluginsAPIExporter.h"
#include <string>
#include <ostream>
//---------------------------------------------------------------------------

namespace rrp
{

using std::string;
using std::ostream;

/**
 * The BaseParameter class is an interface for derived Parameters, that can be of any type.
 */

class PLUGINS_API_DECLSPEC PluginParameter
{
    public:
                                            PluginParameter(const string& name, const string& hint);
        virtual                            ~PluginParameter();
        string                              asString() const;
        string                              getName() const;
        string                              getHint() const;
        virtual string                      getValueAsString() const;

        //Pure virtuals
        virtual string                      getType() const = 0;
        virtual void*                       getValueAsPointer() = 0;
        virtual void                        setValueFromString(const string& value) = 0;


        PLUGINS_API_DECLSPEC
        friend ostream&                     operator<<(ostream& stream, const PluginParameter& outMe);

    protected:
        string                              mName;
        string                              mHint;

};

}
#endif

