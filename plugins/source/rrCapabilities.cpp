#pragma hdrstop
#include "rrException.h"
#include "rrBaseParameter.h"
#include "rrParameter.h"
#include "rrLogger.h"
#include "rrCapabilities.h"
#include "rrCapability.h"


using namespace std;

namespace rrp
{

Capabilities::Capabilities(const string& name, const string& description)
:
mName(name),
mDescription(description)
{}

void Capabilities::add(Capability& capability)
{
    mCapabilities.push_back(&capability);
}

void Capabilities::clear()
{
    mCapabilities.clear();
}

Capability* Capabilities::operator[](int i)
{
    if(mCapabilities.size())
    {
        return (mCapabilities[i]);
    }
    return NULL;
}

Capability* Capabilities::get(const string& capName)
{
    for(int i = 0; i < count(); i++)
    {
        Capability* aCap = (mCapabilities[i]);
        if(aCap && aCap->getName() == capName)
        {
            return aCap;
        }
    }
    return NULL;
}

StringList Capabilities::asStringList()
{
    StringList caps;

    //Add capabilitys
    for(int i = 0; i < count(); i++)
    {
        Capability& aCapability = *(mCapabilities[i]);
        caps.add(aCapability.getName());
    }

    return caps;
}
string Capabilities::info() const
{
    stringstream st;
    vector<Capability*>::iterator iter;

    for(iter = mCapabilities.begin(); iter != mCapabilities.end(); iter++)
    {
        Capability* aCap = (*iter);
        st<<(*aCap);
    }
    return st.str();
}

u_int Capabilities::count()
{
    return mCapabilities.size();
}

//Not giving a capability name, search for first parameter with name 'name'
bool Capabilities::setParameter(const string& name, const string& value)
{
    for(int i = 0; i < count(); i++)
    {
        Capability* capability = mCapabilities[i];

        if(!capability)
        {
            return false;
        }

        Parameters* paras = capability[i].getParameters();

        if(paras)
        {
            BaseParameter* aParameter = paras->getParameter(name);
            if(aParameter)
            {
                aParameter->setValueFromString(value);
                return true;
            }
        }
    }

    return false;
}

ostream& operator<<(ostream& stream, const Capabilities& caps)
{
    stream<<"Capability: "<<caps.mName<<endl;
    stream<<"Description: "<<caps.mDescription<<endl;
    stream<<caps.info();
    return stream;
}

}
