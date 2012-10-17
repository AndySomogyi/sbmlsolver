#ifndef ls_SBML_MODEL_H
#define ls_SBML_MODEL_H
#ifndef NO_SBML

#include <string>
#include <sbml/SBMLDocument.h>
#include <sbml/Model.h>
#include <sbml/Species.h>

namespace ls
{
using namespace libsbml;

class SBMLmodel
{
public:
    static SBMLmodel* FromFile(std::string &sFileName);
    static SBMLmodel* FromSBML(std::string &sSBML);
    SBMLmodel(std::string &sSBML);
    SBMLmodel();
    ~SBMLmodel(void);

    void InitializeFromSBML(std::string &sSBML);
    void InitializeFromFile(std::string &sFileName);

    Model* getModel() { return _Model; }
    int numFloatingSpecies()
    {
        return (int) _Model->getNumSpecies() - _Model->getNumSpeciesWithBoundaryCondition();
    }
    int numReactions()
    {
        return (int) _Model->getNumReactions();
    }

    const Species* getNthFloatingSpecies(int n)
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
    const Species* getNthBoundarySpecies(int n)
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

    const Reaction* getNthReaction(int n)
    {
        return _Model->getReaction(n);
    }

private:
    SBMLDocument *_Document;
    Model* _Model;
};

}
#endif

#endif //ls_SBML_MODEL_H
