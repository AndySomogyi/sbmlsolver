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
 * \internal
 * \private
 * \deprecated
 *
 * Base class for parameters. This class follows the "decorator" pattern.
 */
class PLUGINS_API_DECLSPEC BaseParameter
{
    public:
        string                              mName;
        string                              mHint;
                                            BaseParameter(const string& name, const string& hint);
        virtual                            ~BaseParameter();
        PLUGINS_API_DECLSPEC
        friend ostream&                     operator<<(ostream& stream, const BaseParameter& outMe);

        string                              asString() const;
        string                              getName() const;
        string                              getHint() const;
        virtual string                      getValueAsString() const;
        virtual void*                       getValueAsPointer() = 0;
        virtual void                        setValueFromString(const string& value) = 0;
        virtual string                      getType() const = 0;
};

}
#endif

