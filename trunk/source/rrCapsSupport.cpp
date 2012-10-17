#ifdef USE_PCH
#include "rr_pch.h"
#endif
#pragma hdrstop
#include "rrLogger.h"
#include "rrRoadRunner.h"
#include "rrCapsSupport.h"
#include "rrCVodeInterface.h"
#include "rrCapabilitiesSection.h"
#include "rrCapability.h"
//---------------------------------------------------------------------------

namespace rr
{

CapsSupport::CapsSupport(RoadRunner* rr)
:
mName("RoadRunner"),
mDescription("Settings For RoadRunner"),
mRoadRunner(rr)
{

    if(mRoadRunner && mRoadRunner->GetCVodeInterface())
    {
        CvodeInterface*  cvode = mRoadRunner->GetCVodeInterface();
        CapabilitiesSection integration("integration", "CVODE", "CVODE Integrator");

        integration.Add(new CapabilityType<int>(    "BDFOrder",     cvode->MaxBDFOrder,     "Maximum order for BDF Method"));
        integration.Add(new CapabilityType<int>(    "AdamsOrder",   cvode->MaxAdamsOrder,   "Maximum order for Adams Method"));
        integration.Add(new CapabilityType<double>( "rtol",         cvode->relTol,          "Relative Tolerance"));
        integration.Add(new CapabilityType<double>( "atol",         cvode->absTol,          "Absolute Tolerance"));
        integration.Add(new CapabilityType<int>(    "maxsteps",     cvode->MaxNumSteps,     "Maximum number of internal stepsc"));
        integration.Add(new CapabilityType<double>( "initstep",     cvode->InitStep,        "the initial step size"));
        integration.Add(new CapabilityType<double>( "minstep",      cvode->MinStep,         "specifies a lower bound on the magnitude of the step size."));
        integration.Add(new CapabilityType<double>( "maxstep",      cvode->MaxStep,         "specifies an upper bound on the magnitude of the step size."));
        integration.Add(new CapabilityType<bool>(   "conservation", mRoadRunner->mComputeAndAssignConservationLaws,
                                                                                            "enables (=true) or disables (=false) the conservation analysis \
                                                                                            of models for timecourse simulations."));

        //Add section to Capablities
        Add(integration);
    }

    if(mRoadRunner && mRoadRunner->GetNLEQInterface())
    {
        NLEQInterface* solver = mRoadRunner->GetNLEQInterface();
        CapabilitiesSection steady("SteadyState", "NLEQ2", "NLEQ2 Steady State Solver");
        steady.Add(new CapabilityType<int>("MaxIterations", solver->maxIterations, "Maximum number of newton iterations"));
        steady.Add(new CapabilityType<double>("relativeTolerance", solver->relativeTolerance, "Relative precision of solution components"));
        Add(steady);
    }

    if(!Count())
    {
        Log(lInfo)<<"A model has not been loaded, so  capabilities are not available.";
    }
}

u_int CapsSupport::Count()
{
    return mCapabilitiesSections.size();
}

void CapsSupport::Add(const CapabilitiesSection& section)
{
    mCapabilitiesSections.push_back(section);
}

string CapsSupport::AsXMLString()
{
    //Create XML
    rrXMLDoc doc;
    xml_node mainNode = doc.append_child("caps");
    mainNode.append_attribute("name") = mName.c_str();
    mainNode.append_attribute("description") = mDescription.c_str();

    //Add sections
    for(int i = 0; i < Count(); i++)
    {
        CapabilitiesSection& section = mCapabilitiesSections[i];

        pugi::xml_node section_node = mainNode.append_child("section");
        section_node.append_attribute("name") = section.GetName().c_str();
        section_node.append_attribute("method") = section.GetMethod().c_str();
        section_node.append_attribute("description") = section.GetDescription().c_str();

        //Add capabilites within each section
        for(int j = 0; j < section.Count(); j++)
        {
            Capability* cap = const_cast<Capability*>(&(section[j]));
            pugi::xml_node cap_node = mainNode.append_child("cap");
            cap_node.append_attribute("name") = cap->GetName().c_str();
            cap_node.append_attribute("value") = cap->GetValue().c_str();
            cap_node.append_attribute("hint") = cap->GetHint().c_str();
            cap_node.append_attribute("type") = cap->GetType().c_str();
        }
    }

    stringstream xmlS;
    doc.print(xmlS,"  ", format_indent);
    return xmlS.str();
}


const Capability& CapabilitiesSection::operator[](const int& i) const
{
    return *(mCapabilities[i]);
}

string CapabilitiesSection::GetName()
{
    return mName;
}

string CapabilitiesSection::GetDescription()
{
    return mDescription;
}

string CapabilitiesSection::GetMethod()
{
    return mMethod;
}

u_int CapabilitiesSection::Count()
{
    return mCapabilities.size();
}

void CapabilitiesSection::Add(const Capability* me)
{
    mCapabilities.push_back(me);
}

string CapabilitiesSection::AsString()
{
    stringstream caps;
    caps<<"Section: " << mName <<endl;
    caps<<"Method: " << mMethod<<endl;
    caps<<"Description: " << mDescription<<endl;

    for(int i = 0; i < Count(); i++)
    {
        caps <<*(mCapabilities[i])<<endl;
    }
    return caps.str();
}
}

