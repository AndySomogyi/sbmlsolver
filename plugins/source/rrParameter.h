#ifndef rrParameterH
#define rrParameterH
#include <vector>
#include <string>
#include <sstream>
#include "rrLogger.h"
#include "rrUtils.h"
#include "rrStringList.h"
#include "rrConstants.h"
#include "rrPluginParameter.h"
#include "rrStringUtils.h"
#include "rrRoadRunnerData.h"
#include "rrParameters.h"
#include "../wrappers/C/rrc_types.h"
//---------------------------------------------------------------------------
namespace rrp
{
using std::string;

/**
    Template class implementing a PluginParameter. As this class is a template, it can be of any type.

    The characteristics of a Parameter is its type and its value. Various functions assist in setting/getting
    a parameters value.

    Of importance is the ability to get a pointer to its internal value, e.g. \a getValuePointer(), or \a getValueHandle(), as it can be used as a
    handle in the derived Plugins C API.
*/
template<class T>
class Parameter : public PluginParameter
{
    protected:
        T                              mDummy;
        T&                             mValue;

    public:
        /**
            Create a parameter, assigning a name, value, and optionally a Hint.
        */
                                       Parameter(const string& name, const T& value, const string& hint = "");
                                       Parameter(const Parameter<T>& para);
        void                           setValue(T* val);
        void                           setValue(const T& val);
        void                           setValueFromString(const string& val);
        T                              getValue() const;
        T&                             getValueReference();
        T*                             getValuePointer();
        void*                          getValueHandle();
        string                         getValueAsString() const;
        string                         getType() const;
};

template<class T>
Parameter<T>::Parameter(const string& name, const T& value, const string& hint)
:
PluginParameter(name, hint),
mDummy(value),
mValue(mDummy)
{}

template<class T>
void Parameter<T>::setValue(const T& val)
{
    mValue = val;
}

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
void* Parameter<T>::getValueHandle()
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

//================= char* ===============================
template<>
inline string Parameter<char*>::getType() const
{
    return "string"; //Don't call it char* ?
}


template<>
inline void Parameter<char*>::setValue(char** val)
{
    rr::freeText(mValue);
    mValue = rr::createText(string(val[0]));
}

template<>
inline void Parameter<char*>::setValue(char* const& val)
{
    mValue = rr::createText(string(val));
}

template<>
inline char* Parameter<char*>::getValue() const
{
    return mValue;
}

template<>
inline void Parameter<char*>::setValueFromString(const string& val)
{
    mValue = rr::createText(val);
}

template<>
inline string Parameter<char*>::getValueAsString() const
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

//================= std::string ===============================
template<>
inline string Parameter<string>::getType() const
{
    return "std::string";
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
    return mValue.AsString();
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
    //Todo: implement this ugly conversion?
}

template<>
inline string Parameter<rrc::RRCDataPtr>::getValueAsString() const
{
    return "";
}

//============= RoadRunner data ===========================
template<>
inline string Parameter<rr::RoadRunnerData>::getValueAsString() const
{
    std::stringstream rrData;
    rrData << (mValue);
    return rrData.str();
}

template<>
inline void Parameter<rr::RoadRunnerData>::setValueFromString(const string& val)
{
    //This is not implemented, but could easily be.    
}

template<>
inline void Parameter<rr::RoadRunnerData>::setValue(const rr::RoadRunnerData& val)
{
    mValue = val;
}

template<>
inline void Parameter<rr::RoadRunnerData>::setValue(rr::RoadRunnerData* val)
{
    mValue = (*val);
}

template<>
inline string Parameter<rr::RoadRunnerData>::getType() const
{
    return "roadRunnerDataOld";
}

//Pointer to data
template<>
inline string Parameter<rr::RoadRunnerData*>::getValueAsString() const
{
    std::stringstream rrData;
    rrData << (mValue);
    return rrData.str();
}

template<>
inline void Parameter<rr::RoadRunnerData*>::setValueFromString(const string& val)
{
    //This is not implemented, but could easily be.    
}

//template<>
//inline void Parameter<rr::RoadRunnerData*>::setValue(const rr::RoadRunnerData& val)
//{
//    mValue = val;
//}
//
//template<>
//inline void Parameter<rr::RoadRunnerData*>::setValue(rr::RoadRunnerData* val)
//{
//    mValue = (*val);
//}

template<>
inline string Parameter<rr::RoadRunnerData*>::getType() const
{
    return "roadRunnerData";
}

//============ RRStringArray
template<>
inline string Parameter<rrc::RRStringArray>::getValueAsString() const
{
    //Todo:: when needed
    return string("");
}

template<>
inline void Parameter<rrc::RRStringArray>::setValueFromString(const string& val)
{
    //Todo.. clear current list first..
    rr::StringList list = rr::splitString(val, ",");
    mValue.Count = list.size();
    for(int i = 0; i < mValue.Count; i++)
    {
        mValue.String[i] = rr::createText(list[i]);
    }
}

template<>
inline string Parameter<rrc::RRStringArray>::getType() const
{
    return "RRStringArray";
}

//========== Parameters container
template<>
inline string Parameter<Parameters>::getValueAsString() const
{
    StringList list = mValue.getNames();
    return list.AsString();
}

template<>
inline void Parameter<Parameters>::setValue(Parameters* val)
{
    mValue = (*val);
}

template<>
inline void Parameter<Parameters>::setValueFromString(const string& val)
{
    Log(rr::lError)<<"Trying to set Parameters container by a string..";
    return;
}

template<>
inline void Parameter<Parameters>::setValue(const Parameters& val)
{
    mValue = val;
}


template<>
inline string Parameter<Parameters>::getType() const
{
    return "listOfParameters";
}

}
#endif
