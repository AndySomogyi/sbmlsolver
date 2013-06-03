#ifndef rrBaseParameterH
#define rrBaseParameterH
#include "rrObject.h"
//---------------------------------------------------------------------------
namespace rr
{

//Base class for parameters. This class follows the "decorator" pattern.
class RR_DECLSPEC BaseParameter : public rrObject
{
    public:
        string                              mName;
        string                              mHint;
                                            BaseParameter(const string& name, const string& hint);
        virtual                            ~BaseParameter();
        RR_DECLSPEC
        friend ostream&                 	operator<<(ostream& stream, const BaseParameter& outMe);

        string                              asString() const;
        string                              getName() const;
        string                              getHint() const;
        virtual string                      getValueAsString() const;
		virtual void*						getValueAsPointer() = 0;
        virtual	void						setValueFromString(const string& value) = 0;
        virtual string      	            getType() const = 0;
};

}
#endif

