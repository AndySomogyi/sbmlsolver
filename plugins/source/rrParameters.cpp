#pragma hdrstop
#include "rrLogger.h"
#include "rrParameters.h"

namespace rrp
{
using namespace rr;

Parameters::Parameters(bool canClientClear)
    :mCanClientClearList(canClientClear)
{}

                                        
Parameters::~Parameters()
{
    clear();
}

Parameters::Parameters(const Parameters& cpyMe)
{
    this->mCanClientClearList = true;
    this->mParas = cpyMe.mParas;
    this->mParasIter = this->mParas.begin();
}

bool Parameters::clear()
{
    if(mCanClientClearList == false)
    {
        return false;
    }

    for(int i = 0; i < mParas.size(); i++)
    {
        if(mParas[i].second == true)
        {
            delete mParas[i].first;
        }
    }
    mParas.clear();
    return true;
}

void Parameters::add(PluginParameter* me, bool own)
{
    mParas.push_back( pair<PluginParameter*, bool>(me, own) );
}

StringList Parameters::getNames() const
{
    StringList list;
    for(int i = 0; i < count(); i++)
    {
        list.add(mParas[i].first->getName());
    }
    return list;
}

const PluginParameter* Parameters::operator[](const int& i) const
{
    return mParas[i].first;
}

PluginParameter* Parameters::operator[](const int& i)
{
    return mParas[i].first;
}

u_int Parameters::count() const
{
    return mParas.size();
}

PluginParameter* Parameters::getParameter(const string& paraName)
{
    for(int i = 0; i < count(); i++)
    {
        if(paraName == mParas[i].first->getName())
        {
            return mParas[i].first;
        }
    }
    return NULL;
}

PluginParameter* Parameters::getFirst()
{
    mParasIter = mParas.begin();
    if(mParasIter != mParas.end())
    {
        return (*mParasIter).first;
    }
    return NULL;
}

PluginParameter* Parameters::getCurrent()
{
    if(mParasIter != mParas.end())
    {
        return (*mParasIter).first;
    }
    return NULL;
}

PluginParameter* Parameters::getNext()
{
    mParasIter++;
    if(mParasIter != mParas.end())
    {
        return (*mParasIter).first;
    }
    return NULL;
}

PluginParameter* Parameters::getPrevious()
{
    mParasIter--;
    if(mParasIter != mParas.end())
    {
        return (*mParasIter).first;
    }
    return NULL;
}

ostream& operator<<(ostream& stream, const Parameters& paras)
{
    for(int i = 0; i < paras.count(); i++)
    {
        
        stream << paras.mParas[i].first;
        
    }
    return stream;
}


}
