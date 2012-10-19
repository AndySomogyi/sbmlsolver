#ifdef USE_PCH
#include "rr_pch.h"
#endif
#pragma hdrstop
#include <string>
#include <vector>
#include <sbml/SBMLDocument.h>
#include <sbml/Model.h>
#include <sbml/SBMLReader.h>
#include "lsSBMLModel.h"
#include "lsUtils.h"
//---------------------------------------------------------------------------


using namespace ls;
using namespace ls;
using namespace std;

SBMLmodel* SBMLmodel::FromFile(string &sFileName)
{
    SBMLmodel *oResult = new SBMLmodel();
    oResult->InitializeFromFile(sFileName);
    return oResult;
}

SBMLmodel* SBMLmodel::FromSBML(string &sSBML)
{
    SBMLmodel *oResult = new SBMLmodel();
    oResult->InitializeFromSBML(sSBML);
    return oResult;
}

SBMLmodel::SBMLmodel() : _Document(NULL), _Model(NULL)
{
}


SBMLmodel::SBMLmodel(string &sSBML) : _Document(NULL), _Model(NULL)
{
    InitializeFromSBML(sSBML);
}


void SBMLmodel::InitializeFromSBML(std::string &sSBML)
{
    SBMLReader oReader;
    _Document = oReader.readSBMLFromString(sSBML);
    _Model = _Document->getModel();
    if (_Model == NULL)
        throw new ApplicationException("Invalid SBML Model", "The SBML model was invalid. Please validate it using a SBML validator such as: http://sys-bio.org/validate.");

}
void SBMLmodel::InitializeFromFile(std::string &sFileName)
{
    SBMLReader oReader;
    _Document = oReader.readSBML(sFileName);
    _Model = _Document->getModel();
    if (_Model == NULL)
        throw new ApplicationException("Invalid SBML Model", "The SBML model was invalid. Please validate it using a SBML validator such as: http://sys-bio.org/validate.");
}


SBMLmodel::~SBMLmodel(void)
{
    delete _Document;
}
