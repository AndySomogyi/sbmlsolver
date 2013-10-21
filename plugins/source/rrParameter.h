#ifndef rrParameterH
#define rrParameterH
#include <vector>
#include <string>
#include "rrConstants.h"
#include "rrBaseParameter.h"
#include "rrStringUtils.h"
#include "../wrappers/C/rrc_types.h"
//---------------------------------------------------------------------------
namespace rrp
{
using namespace rr;
/**
 * @internal
 * @deprecated
 */
template<class T>
class Parameter : public BaseParameter
{
    protected:
        T                                   mDummy;
        T&                                  mValue;

    public:
                                            Parameter(const std::string& name, const T& value, const std::string& hint = "");
                                            Parameter(const Parameter<T>& para);
        void                                setValue(T* val);
        void                                setValue(const T& val);
        void                                setValueFromString(const std::string& val);
        T                                   getValue() const;
        T*                                  getValuePointer();
        void*                               getValueAsPointer();
        std::string                         getValueAsString() const;
        std::string                         getType() const;
};

//template<class T>
//inline Parameter<T>::Parameter(const string& name, const T& value, const string& hint)
//:
//rr::BaseParameter(name, hint),
//mValue(value)
//{}

template<class T>
rrp::Parameter<T>::Parameter(const std::string& name, const T& value, const std::string& hint)
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
string rrp::Parameter<T>::getValueAsString() const
{
    return toString(mValue);
}

template<class T>
T rrp::Parameter<T>::getValue() const
{
    return mValue;
}

template<class T>
T* rrp::Parameter<T>::getValuePointer()
{
    return &mValue;
}

template<class T>
void* rrp::Parameter<T>::getValueAsPointer()
{
    return (void*) &mValue;
}

//=========================SPECIALIZATIONS ===================================

//================= BOOL ===============================
template<>
inline std::string rrp::Parameter<bool>::getType() const
{
    return "bool";
}


template<>
inline void rrp::Parameter<bool>::setValue(const bool& val)
{
    mValue = val;
}

template<>
inline void rrp::Parameter<bool>::setValueFromString(const std::string& val)
{
    mValue = rr::toBool(val);
}

//Integer parameter specialization
template<>
inline std::string rrp::Parameter<int>::getType() const
{
    return "int";
}

template<>
inline void rrp::Parameter<int>::setValueFromString(const std::string& val)
{
    mValue = rr::toInt(val);
}

template<>
inline string rrp::Parameter<int>::getValueAsString() const
{
    return toString(mValue);
}

//Double parameter specialization
template<>
inline std::string rrp::Parameter<double>::getType() const
{
    return "double";
}

//template<>
//void Parameter<double>::setValue(const double& val)
//{
//    mValue = val;
//}

template<>
inline void rrp::Parameter<double>::setValueFromString(const std::string& val)
{
    mValue = rr::toDouble(val);
}

template<>
inline string rrp::Parameter<std::string>::getType() const
{
    return "string";
}


template<>
inline void rrp::Parameter<std::string>::setValueFromString(const std::string& val)
{
    mValue = val;
}

template<>
inline std::string rrp::Parameter< std::vector<std::string> >::getType() const
{
    return "vector<string>";
}

template<>
inline void Parameter< std::vector<std::string> >::setValueFromString(const std::string& val)
{
    mValue = splitString(val,", ");
}

template<>
inline void Parameter< rrc::RRCDataPtr >::setValueFromString(const std::string& val)
{
    //mValue = splitString(val,", ");
}

template<>
inline std::string rrp::Parameter<double>::getValueAsString() const
{
    return toString(mValue, "%G");
}

template<>
inline std::string rrp::Parameter<rrc::RRCDataPtr>::getValueAsString() const
{
    return "";
}

}
#endif
