#pragma hdrstop
#include <sstream>
#include "rrStringUtils.h"
#include "rrPluginParameter.h"
#include "rrParameter.h"
//---------------------------------------------------------------------------

using namespace std;
namespace rrp
{

PluginParameter::PluginParameter(const string& name, const string& hint)
:
mName(name),
mHint(hint)
{}

PluginParameter::~PluginParameter()
{}

string PluginParameter::getName() const
{
    return mName;
}

string PluginParameter::getHint() const
{
    return mHint;
}

string PluginParameter::getValueAsString() const
{
    PluginParameter* ptr = const_cast<PluginParameter*>(this);

    if(dynamic_cast< Parameter<int>* >( ptr))
    {
        return dynamic_cast< Parameter<int>* >(ptr) ->getValueAsString();
    }

    if(dynamic_cast< Parameter<double>* >(ptr))
    {
        return "double";
    }

    if(dynamic_cast< Parameter<bool>* >(ptr))
    {
        return "boolean";
    }

    return "Need to cast to subclass!";
}

string PluginParameter::asString() const
{
    stringstream val;

    val<<"Name: "<<mName<<endl;
    val<<"Type: "<<getType()<<endl;
    val<<"Value: "<<getValueAsString()<<endl;
    val<<"Hint: "<<mHint<<endl;
    return val.str();
}

string PluginParameter::getType() const
{
    string val("no info");

    //Downcasts
    PluginParameter* ptr = const_cast<PluginParameter*>(this);

    if(dynamic_cast< Parameter<int>* >(ptr))
    {
        return "integer";
    }

    if(dynamic_cast< Parameter<double>* >(ptr))
    {
        return "double";
    }

    if(dynamic_cast< Parameter<bool>* >(ptr))
    {
        return "boolean";
    }

    if(dynamic_cast< Parameter<rrc::RRCData*>* >(ptr))
    {
        return "RRCDataPtr";
    }

    return val;
}

ostream& operator<<(ostream& stream, const PluginParameter& outMe)
{
    stream<<outMe.asString();   //virtual friend idiom
    return stream;
}


}
