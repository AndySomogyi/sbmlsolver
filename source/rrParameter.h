#ifndef rrParameterH
#define rrParameterH
#include <vector>
#include "rrObject.h"
#include "rrConstants.h"
#include "rrBaseParameter.h"
#include "rrStringUtils.h"
#include "../Wrappers/C/rrc_types.h"
//---------------------------------------------------------------------------
namespace rr
{

template<class T>
class Parameter : public BaseParameter
{
    protected:
        T                                   mValue;

    public:
                                            Parameter(const string& name, const T& value, const string& hint);
		void								setValue(const T& val){mValue = val;}
        T									getValue();
        T*									getValuePointer();
        string                      		getValueAsString() const;
	 	string								getType() const;
};

template<class T>
Parameter<T>::Parameter(const string& name, const T& value, const string& hint)
:
rr::BaseParameter(name, hint),
mValue(value)
{}

template<class T>
string Parameter<T>::getValueAsString() const
{
    return ToString(mValue);
}

template<class T>
T Parameter<T>::getValue()
{
    return mValue;
}

template<class T>
T* Parameter<T>::getValuePointer()
{
    return &mValue;
}

// =========================== SPECIALIZATONS
//bool parameter specialization
template<>
inline string Parameter<bool>::getType() const
{
    return "bool";
}

//Integer parameter specialization
template<>
inline string Parameter<int>::getType() const
{
    return "int";
}

template<>
inline string Parameter<int>::getValueAsString() const
{
    return ToString(mValue);
}

//Double parameter specialization
template<>
inline string Parameter<double>::getType() const
{
    return "double";
}

template<>
inline string rr::Parameter<double>::getValueAsString() const
{
    return ToString(mValue, "%G");
}

template<>
inline string Parameter<string>::getType() const
{
    return "string";
}

template<>
inline string Parameter< vector<string> >::getType() const
{
    return "vector<string>";
}


template<>
inline string Parameter< rrc::RRData >::getType() const
{
    return "RRData";
}

template<>
inline string Parameter<rrc::RRData>::getValueAsString() const
{
    return "Bla...";//ToString(mValue);
}

//#if defined(_MSC_VER)
//template<>
//string Parameter<int>::getValueAsString() const;
//
//template<>
//string Parameter<double>::getValueAsString() const;
//#endif


}
#endif
