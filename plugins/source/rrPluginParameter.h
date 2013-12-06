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
 * The PluginParameter class is an interface for derived Parameters, that can be of any type.
 * A parameter has a name and a hint, containing the purpose of the actual parameter.
 */
class PLUGINS_API_DECLSPEC PluginParameter
{
    public:
        /**
         * The constructor populates the parameters name and hint.
         */
                                            PluginParameter(const string& name, const string& hint);
        /**
         * De-allocate any memory allocated.
         */
        virtual                            ~PluginParameter();

        /**
         * Return the parameter in a string representation
         */
        string                              asString() const;

        /**
         * Return the parameters name
         */
        string                              getName() const;

        /**
         * Return the parameters hint
         */
        string                              getHint() const;

        //Pure virtuals
        /**
         * Get the value of the parameter, as a string. This function need to be implemented in any derived class.
         \note In the case of a complex type, such as RoadRunnerData for example, expect the returned string to be
         of considerate size.
         */
        virtual string                      getValueAsString() const;

        /**
         * Get the type of the parameter. This function need to be implemented in any derived class.
         */
        virtual string                      getType() const;

        /**
         * Get an opaque pointer to the value of the parameter. This function need to be implemented in any derived class.
         */
        virtual void*                       getValueHandle();

        /**
         * Set the value of the parameter, using string representation. This function need to be implemented in any derived class.
         */
        virtual void                        setValueFromString(const string& value) = 0;

        /**
         * Ouput the parameter to an output stream. This function need to be implemented in any derived class.
         */
        PLUGINS_API_DECLSPEC
        friend ostream&                     operator<<(ostream& stream, const PluginParameter& outMe);

    protected:
        /**
         * The name of the parameter.
         */
        string                              mName;

        /**
         * The hint of the parameter.
         */
        string                              mHint;

};

}
#endif

