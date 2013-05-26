#ifndef rrParameterH
#define rrParameterH
#include <vector>
#include "rrObject.h"
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
		T									mDummy;
        T&                                  mValue;

    public:
                                            Parameter(const string& name, const T& value, const string& hint);
                                            Parameter(const string& name, T& value, const string& hint);
		void								set(const T& val){mValue = val;}
		void								setValue(const string& val);
        T									getValue();
        T*									getValuePointer();
        string                      		getValueAsString() const;
	 	string								getType() const;
};

template<class T>
Parameter<T>::Parameter(const string& name, T& value, const string& hint)
:
rr::BaseParameter(name, hint),
mValue(value)
{}

template<class T>
Parameter<T>::Parameter(const string& name, const T& value, const string& hint)
:
rr::BaseParameter(name, hint),
mDummy(value),
mValue(mDummy)
{}



template<class T>
string Parameter<T>::getValueAsString() const
{
    return toString(mValue);
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

template<>
inline void Parameter<bool>::setValue(const string& val)
{
	mValue = rr::toBool(val);
}

//Integer parameter specialization
template<>
inline string Parameter<int>::getType() const
{
    return "int";
}

template<>
inline void Parameter<int>::setValue(const string& val)
{
	mValue = rr::toInt(val);
}

template<>
inline string Parameter<int>::getValueAsString() const
{
    return toString(mValue);
}

//Double parameter specialization
template<>
inline string Parameter<double>::getType() const
{
    return "double";
}

template<>
inline void Parameter<double>::setValue(const string& val)
{
	mValue = rr::toDouble(val);
}

template<>
inline string Parameter<string>::getType() const
{
    return "string";
}

template<>
inline void Parameter<string>::setValue(const string& val)
{
	mValue = val;
}

template<>
inline string Parameter< vector<string> >::getType() const
{
    return "vector<string>";
}

template<>
inline void Parameter< vector<string> >::setValue(const string& val)
{
	mValue = splitString(val,", ");
}

template<>
inline void Parameter< rrc::RRDataHandle >::setValue(const string& val)
{
	//mValue = splitString(val,", ");
}

//template<>
//string Parameter<int>::getValueAsString() const
//{
//    return toString(mValue);
//}

template<>
inline string rr::Parameter<double>::getValueAsString() const
{
    return toString(mValue, "%G");
}

}
#endif
