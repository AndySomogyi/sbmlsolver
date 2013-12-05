#ifndef rrBaseParameterH
#define rrBaseParameterH
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

class PLUGINS_API_DECLSPEC BaseParameter
{
    public:
                                            BaseParameter(const string& name, const string& hint);
        virtual                            ~BaseParameter();
        string                              asString() const;
        string                              getName() const;
        string                              getHint() const;
        virtual string                      getValueAsString() const;

        //Pure virtuals
        virtual string                      getType() const = 0;
        virtual void*                       getValueAsPointer() = 0;
        virtual void                        setValueFromString(const string& value) = 0;


        PLUGINS_API_DECLSPEC
        friend ostream&                     operator<<(ostream& stream, const BaseParameter& outMe);

    protected:
        string                              mName;
        string                              mHint;

};

}
#endif

