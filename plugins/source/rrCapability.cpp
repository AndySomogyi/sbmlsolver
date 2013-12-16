#pragma hdrstop
#include <sstream>
#include "rrParameter.h"
#include "rrCapability.h"

using namespace std;

namespace rrp
{

Capability::Capability(const string& name, const string& description)
:
mName(name),
//mMethod(method),
mDescription(description),
mParameters(false)
{}

Capability::~Capability()
{}

Capability::Capability(const Capability& from)
:
mName(from.mName),
//mMethod(from.mMethod),
mDescription(from.mDescription),
mParameters(from.mParameters)
{}

void Capability::setup(const string& name, const string& descr)
{
    mName = name;
//    mMethod = method;
    mDescription = descr;
}

Parameters* Capability::getParameters()
{
    return &mParameters;
}

PluginParameter& Capability::operator[](const int& i)
{

    return *(mParameters[i]);
}

string Capability::getName() const
{
    return mName;
}

string Capability::getDescription() const
{
    return mDescription;
}

//string Capability::getMethod() const
//{
//    return mMethod;
//}

u_int Capability::nrOfParameters() const
{
    return mParameters.count();
}

void Capability::addParameter(PluginParameter* me)
{
    mParameters.add(me);
}

string Capability::asString()  const
{
    stringstream caps;
    caps<<"Name: " << mName <<endl;
//    caps<<"Method: " << mMethod<<endl;
    caps<<"Description: " << mDescription<<endl;

    for(int i = 0; i < nrOfParameters(); i++)
    {
        caps << *(mParameters[i]) <<endl;
    }
    return caps.str();
}

PluginParameter* Capability::getParameter(const string& paraName)
{
    for(u_int i = 0; i < mParameters.count(); i++)
    {
        if(mParameters[i] && mParameters[i]->getName() == paraName)
        {
            return mParameters[i];
        }
    }
    return NULL;
}

ostream& operator <<(ostream& os, const Capability& caps)
{
    os<<"Parameters for capability: "<<caps.mName<<"\n";

    for(u_int i = 0; i < caps.nrOfParameters(); i++)
    {
        os<< *(caps.mParameters[i]);

        if(i < caps.nrOfParameters() -1 )
        {
            os<<"  "<<endl;
        }
    }
    return os;
}

}
