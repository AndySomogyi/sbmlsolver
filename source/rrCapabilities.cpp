#ifdef USE_PCH
#include "rr_pch.h"
#endif
#pragma hdrstop
#include "rrException.h"
#include "rrBaseParameter.h"
#include "rrParameter.h"
#include "rrLogger.h"
#include "rrRoadRunner.h"
#include "rrCapabilities.h"
#include "rrCVODEInterface.h"
#include "rrCapability.h"

//---------------------------------------------------------------------------
namespace rr
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

Capability*	Capabilities::operator[](int i)
{
	return (mCapabilities[i]);
}

Capability*	Capabilities::get(const string& capName)
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

string Capabilities::asXML()
{
    //Create XML
    rrXMLDocument doc;
    xml_node mainNode = doc.append_child("capabilities");
    mainNode.append_attribute("name") = mName.c_str();
    mainNode.append_attribute("description") = mDescription.c_str();

    //Add capabilitys
    for(int i = 0; i < count(); i++)
    {
    	Capability& aCapability = *(mCapabilities[i]);
        xml_node capabilityNode = mainNode.append_child("capability");
        capabilityNode.append_attribute("name") 	   	= aCapability.getName().c_str();
        capabilityNode.append_attribute("method")	   	= aCapability.getMethod().c_str();
        capabilityNode.append_attribute("description") 	= aCapability.getDescription().c_str();

        pugi::xml_node parameters = capabilityNode.append_child("parameters");

        //Add parameters within each capability
        for(int j = 0; j < aCapability.nrOfParameters(); j++)
        {
            rr::BaseParameter* parameter = const_cast<rr::BaseParameter*>(&(aCapability[j]));

            pugi::xml_node parameterNode = parameters.append_child("parameter");
            parameterNode.append_attribute("name") 	= parameter->getName().c_str();
            parameterNode.append_attribute("value") = parameter->getValueAsString().c_str();
            parameterNode.append_attribute("hint") 	= parameter->getHint().c_str();
            parameterNode.append_attribute("type") 	= parameter->getType().c_str();
        }
    }

    stringstream xmlS;
    doc.print(xmlS,"  ", format_indent);
    return xmlS.str();
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

}
