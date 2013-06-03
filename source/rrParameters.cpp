#pragma hdrstop
#include "rrParameters.h"

namespace rr
{

Parameters::Parameters()
{}

void Parameters::clear()
{
	for(int i = 0; i < mParas.size(); i++)
    {
    	delete mParas[i];
    }
	mParas.clear();
}

void Parameters::add(BaseParameter* me)
{
	mParas.push_back(me);
}

StringList Parameters::asStringList() const
{
	StringList list;
    for(int i = 0; i < count(); i++)
    {
    	list.add(mParas[i]->mName);
    }
    return list;
}

const BaseParameter* Parameters::operator[](const int& i) const
{
	return mParas[i];
}

BaseParameter* Parameters::operator[](const int& i)
{
	return mParas[i];
}

u_int Parameters::count() const
{
	return mParas.size();
}

BaseParameter* Parameters::getParameter(const string& paraName)
{
    for(int i = 0; i < count(); i++)
    {
    	if(paraName == mParas[i]->mName)
        {
        	return mParas[i];
        }
    }
	return NULL;
}

}
