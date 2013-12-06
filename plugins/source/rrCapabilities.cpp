#pragma hdrstop
#include "libxml/tree.h"
#include "libxml/xpath.h"
#include "rrException.h"
#include "rrPluginParameter.h"
#include "rrParameter.h"
#include "rrLogger.h"
#include "rrCapabilities.h"
#include "rrCapability.h"

namespace rrp
{
using namespace std;

Capabilities::Capabilities()//const string& name, const string& description)
//:
//mName(name),
//mDescription(description)
{}

Capabilities::~Capabilities()
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
            PluginParameter* aParameter = paras->getParameter(name);
            if(aParameter)
            {
                aParameter->setValueFromString(value);
                return true;
            }
        }
    }

    return false;
}

string Capabilities::asXML()
{
    xmlDocPtr doc = NULL;           /* document pointer */
    xmlNodePtr root_node = NULL;
    LIBXML_TEST_VERSION;

    doc = xmlNewDoc(BAD_CAST "1.0");
    xmlKeepBlanksDefault(0);
    root_node = xmlNewNode(NULL, BAD_CAST "capabilities");
    xmlDocSetRootElement(doc, root_node);

    //Add capabilitys
    for(int i = 0; i < count(); i++)
    {
        Capability& aCapability = *(mCapabilities[i]);
        xmlNodePtr capNode = xmlNewChild(root_node, NULL, BAD_CAST "capability",NULL);
//        xmlNewProp(capNode, BAD_CAST "name", BAD_CAST mName.c_str());
//        xmlNewProp(capNode, BAD_CAST "description", BAD_CAST mDescription.c_str());

        xmlNodePtr parameters = xmlNewChild(capNode, NULL, BAD_CAST "parameters",NULL);

        //Add parameters within each capability
        for(int j = 0; j < aCapability.nrOfParameters(); j++)
        {
            xmlNodePtr paraNode = xmlNewChild(parameters, NULL, BAD_CAST "para", NULL);

            PluginParameter* parameter = const_cast<PluginParameter*>(&(aCapability[j]));
            xmlNewProp(paraNode, BAD_CAST "name",   BAD_CAST parameter->getName().c_str());
            xmlNewProp(paraNode, BAD_CAST "value",  BAD_CAST parameter->getValueAsString().c_str());
            xmlNewProp(paraNode, BAD_CAST "hint",   BAD_CAST parameter->getHint().c_str());
            xmlNewProp(paraNode, BAD_CAST "type",   BAD_CAST parameter->getType().c_str());
        }
    }

    xmlChar *xmlbuff;
    int buffersize;
    xmlDocDumpFormatMemory(doc, &xmlbuff, &buffersize, 1);

    Log(rr::Logger::LOG_INFORMATION)<<(char*) xmlbuff;

    string xml = xmlbuff != NULL ? string((char*) xmlbuff) : string("");
    xmlFreeDoc(doc);
    xmlCleanupParser();
    return xml;
}

ostream& operator<<(ostream& stream, const Capabilities& caps)
{
//    stream<<"Capability: "<<caps.mName<<endl;
//    stream<<"Description: "<<caps.mDescription<<endl;
    stream<<caps.info();
    return stream;
}

}
