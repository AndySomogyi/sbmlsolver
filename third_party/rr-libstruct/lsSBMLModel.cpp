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
using namespace std;

static const char* InvalidModelMessage = "Invalid SBML Model";
static const char* InvalidModelDetailedMessage = "The SBML model was invalid. Please validate it using a SBML validator such as: http://sys-bio.org/validate.";

namespace ls
{

SBMLmodel::SBMLmodel(const string &sSBML)
    :
    _Document(readSBMLFromString(sSBML.c_str())),
    _Model(_Document->getModel())
{
    if (_Model == NULL)
        throw ApplicationException(InvalidModelMessage, InvalidModelDetailedMessage);
}

SBMLmodel::SBMLmodel(SBMLDocument *document)
    :
    _Document(document),
    _Model(_Document->getModel())
{
    if (_Model == NULL)
        throw ApplicationException(InvalidModelMessage, InvalidModelDetailedMessage);
}

SBMLmodel::SBMLmodel(const libsbml::Model *model)
    :
    _Document(0),
    _Model(model)
{
    if (_Model == NULL)
        throw ApplicationException(InvalidModelMessage, InvalidModelDetailedMessage);
}

SBMLmodel::~SBMLmodel(void)
{
    delete _Document;
}

SBMLmodel* SBMLmodel::FromFile(string &sFileName)
{
    SBMLDocument *document = readSBMLFromFile(sFileName.c_str());
    return new SBMLmodel(document);
}

SBMLmodel* SBMLmodel::FromSBML(string &sSBML)
{
    SBMLDocument *document = readSBMLFromString(sSBML.c_str());
    return new SBMLmodel(document);
}

const Model* SBMLmodel::getModel() const
{
    return _Model;
}

int SBMLmodel::numFloatingSpecies() const
{
    return (int) _Model->getNumSpecies() - _Model->getNumSpeciesWithBoundaryCondition();
}

int SBMLmodel::numReactions() const
{
    return (int) _Model->getNumReactions();
}

const Species* SBMLmodel::getNthFloatingSpecies(int n) const
{
    int nCount = 0;
    for (unsigned int i = 0; i < _Model->getNumSpecies(); i++)
    {
        if (!_Model->getSpecies(i)->getBoundaryCondition())
        {
            if (nCount == n)
                return _Model->getSpecies(i);
            nCount ++;
        }
    }
    return NULL;
}

const Species* SBMLmodel::getNthBoundarySpecies(int n) const
{
    int nCount = 0;
    for (unsigned int i = 0; i < _Model->getNumSpecies(); i++)
    {
        if (_Model->getSpecies(i)->getBoundaryCondition())
        {
            if (nCount == n)
                return _Model->getSpecies(i);
            nCount ++;
        }
    }
    return NULL;
}

const Reaction* SBMLmodel::getNthReaction(int n) const
{
    return _Model->getReaction(n);
}


}//namespace ls

