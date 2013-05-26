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
		T									mDummy;
        T&                                  mValue;

    public:
                                            Parameter(const string& name, const T& value, const string& hint = gEmptyString);
                                            Parameter(const Parameter<T>& para);
		void								setValue(T* val);
		void								setValue(const T& val);
		//void								setValue(const string& val);//{setValueFromString(val);}
		void								setValueFromString(const string& val);
        T									getValue() const;
        T*									getValuePointer();
        void*								getValueAsPointer();
        string                      		getValueAsString() const;
	 	string								getType() const;
};

//template<class T>
//inline Parameter<T>::Parameter(const string& name, const T& value, const string& hint)
//:
//rr::BaseParameter(name, hint),
//mValue(value)
//{}

template<class T>
Parameter<T>::Parameter(const string& name, const T& value, const string& hint)
:
rr::BaseParameter(name, hint),
mDummy(value),
mValue(mDummy)
{}


//template<class T>
//Parameter<T>::Parameter(const Parameter<T>& para)
//:
//rr::BaseParameter(para.getName(), para.getHint()),
//mDummy(para.getValue()),
//mValue(mDummy)
//{ }

//template<class T>
//void Parameter<T>::setValue(const T& val)
//{
//	mValue = val;
//}

template<class T>
string Parameter<T>::getValueAsString() const
{
    return toString(mValue);
}

template<class T>
T Parameter<T>::getValue() const
{
    return mValue;
}

template<class T>
T* Parameter<T>::getValuePointer()
{
    return &mValue;
}

template<class T>
void* Parameter<T>::getValueAsPointer()
{
    return (void*) &mValue;
}

//=========================SPECIALIZATIONS ===================================

//================= BOOL ===============================
template<>
inline string Parameter<bool>::getType() const
{
    return "bool";
}


template<>
inline void Parameter<bool>::setValue(const bool& val)
{
	mValue = val;
}

template<>
inline void Parameter<bool>::setValueFromString(const string& val)
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
inline void Parameter<int>::setValueFromString(const string& val)
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

//template<>
//void Parameter<double>::setValue(const double& val)
//{
//	mValue = val;
//}

template<>
inline void Parameter<double>::setValueFromString(const string& val)
{
	mValue = rr::toDouble(val);
}

template<>
inline string Parameter<string>::getType() const
{
    return "string";
}

//template<>
//void Parameter<string>::setValue(const string& val)
//{
//	mValue = val;
//}

template<>
inline void Parameter<string>::setValueFromString(const string& val)
{
	mValue = val;
}

template<>
inline string Parameter< vector<string> >::getType() const
{
    return "vector<string>";
}

template<>
inline void Parameter< vector<string> >::setValueFromString(const string& val)
{
	mValue = splitString(val,", ");
}

template<>
inline void Parameter< rrc::RRDataHandle >::setValueFromString(const string& val)
{
	//mValue = splitString(val,", ");
}

template<>
inline string rr::Parameter<double>::getValueAsString() const
{
    return toString(mValue, "%G");
}

}
#endif
