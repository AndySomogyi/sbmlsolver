#ifndef rrParameterH
#define rrParameterH
#include <vector>
#include <string>
#include "rrStringList.h"
#include "rrConstants.h"
#include "rrBaseParameter.h"
#include "rrStringUtils.h"
#include "../wrappers/C/rrc_types.h"
//---------------------------------------------------------------------------
namespace rrp
{
using std::string;

/**
 * @internal
 * @deprecated
 */
template<class T>
class Parameter : public BaseParameter
{
    protected:
        T                              mDummy;
        T&                             mValue;

    public:
                                       Parameter(const string& name, const T& value, const string& hint = "");
                                       Parameter(const Parameter<T>& para);
        void                           setValue(T* val);
        void                           setValue(const T& val);
        void                           setValueFromString(const string& val);
        T                              getValue() const;
        T&                             getValueReference();
        T*                             getValuePointer();
        void*                          getValueAsPointer();
        string                         getValueAsString() const;
        string                         getType() const;
};

template<class T>
Parameter<T>::Parameter(const string& name, const T& value, const string& hint)
:
BaseParameter(name, hint),
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
//    mValue = val;
//}

template<class T>
string Parameter<T>::getValueAsString() const
{
    return rr::toString(mValue);
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
T& Parameter<T>::getValueReference()
{
    return mValue;
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


//================= Int ===============================
template<>
inline string Parameter<int>::getType() const
{
    return "int";
}

template<>
inline void Parameter<int>::setValue(const int& val)
{
    mValue = val;
}

template<>
inline void Parameter<int>::setValueFromString(const string& val)
{
    mValue = rr::toInt(val);
}

template<>
inline string Parameter<int>::getValueAsString() const
{
    return rr::toString(mValue);
}

//================= Double ===============================
template<>
inline string Parameter<double>::getType() const
{
    return "double";
}

template<>
inline void Parameter<double>::setValue(const double& val)
{
    mValue = val;
}

template<>
inline void Parameter<double>::setValueFromString(const string& val)
{
    mValue = rr::toDouble(val);
}

template<>
inline string Parameter<double>::getValueAsString() const
{
    return rr::toString(mValue, "%G");
}

//================= String ===============================
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
inline void rrp::Parameter<string>::setValueFromString(const string& val)
{
    mValue = val;
}

//================= vector<string> ===============================
template<>
inline string Parameter< std::vector<string> >::getType() const
{
    return "vector<string>";
}

template<>
inline void Parameter< std::vector<string> >::setValueFromString(const string& val)
{
    mValue = rr::splitString(val,", ");
}

//================= rr::StringList ===============================
template<>
inline string Parameter< rr::StringList >::getType() const
{
    return "StringList";
}

template<>
inline string Parameter<rr::StringList>::getValueAsString() const
{
    return rr::toString(mValue, "\n");
}


template<>
inline void Parameter< rr::StringList >::setValueFromString(const string& val)
{
    mValue = rr::splitString(val,", ");
}

//================= RRCDataPtr ===============================
template<>
inline void Parameter< rrc::RRCDataPtr >::setValueFromString(const string& val)
{
    //mValue = splitString(val,", ");
}

template<>
inline string Parameter<rrc::RRCDataPtr>::getValueAsString() const
{
    return "";
}

}
#endif
