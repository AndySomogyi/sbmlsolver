#ifdef USE_PCH
#include "rr_pch.h"
#endif
#pragma hdrstop
#include <sstream>
#include "rrCapability.h"
//---------------------------------------------------------------------------

namespace rr
{

Capability::Capability(const string& name, const string& hint)
:
mName(name),
mHint(hint)
{}

Capability::~Capability()
{}

string Capability::GetName() const
{
    return mName;
}

string Capability::GetHint() const
{
    return mHint;
}

string Capability::GetValue() const
{
    return GetValueAsString();
}

string Capability::AsString() const
{
    stringstream val;

    val<<"Name: "<<mName<<endl;
    val<<"Value: "<<GetValueAsString()<<endl;
    val<<"HInt: "<<mHint<<endl;
    val<<"Type: "<<GetType()<<endl;
    return val.str();
}

string Capability::GetType() const
{
    string val("no info");

    //Downcasts
    Capability* ptr = const_cast<Capability*>(this);

    if(dynamic_cast< CapabilityType<int>* >(ptr))
    {
        return "integer";
    }

    if(dynamic_cast< CapabilityType<double>* >(ptr))
    {
        return "double";
    }

    if(dynamic_cast< CapabilityType<bool>* >(ptr))
    {
        return "boolean";
    }

    return val;
}

ostream&  operator<<(ostream& stream, const Capability& outMe)
{
    stream<<outMe.AsString();   //virtual friend idiom
    return stream;
}

template<>
string CapabilityType<double>::GetValueAsString() const
{
    return ToString(mValue);
}

template<>
string CapabilityType<int>::GetValueAsString() const
{
    return ToString(mValue);
}

}
