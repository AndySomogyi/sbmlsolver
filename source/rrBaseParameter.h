#ifndef rrBaseParameterH
#define rrBaseParameterH
#include "rrObject.h"
//---------------------------------------------------------------------------
namespace rr
{

class RR_DECLSPEC BaseParameter : public rrObject
{
    public:
        string                              mName;
        string                              mHint;
                                            BaseParameter(const string& name, const string& hint);
        virtual                            ~BaseParameter();
        RR_DECLSPEC
        friend ostream&                 	operator<<(ostream& stream, const BaseParameter& outMe);

        string                              asString() 	const;
        string                              getName() 	const;
        string                              getHint() 	const;
        virtual string      	            getType() 	const;
        virtual string                      getValueAsString() 	const;
};

}
#endif

