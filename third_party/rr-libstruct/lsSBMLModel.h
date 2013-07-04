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
private:
    /**
     * If the _Document exists, that means we created it, and
     * we have to delete it.
     */
    SBMLDocument*                _Document;

    /**
     * always owned by someone else.
     */
    const Model*                 _Model;

    /**
     * privat ctor, used when we create a document, and thus
     * are responsible for deleting it.
     */
    SBMLmodel(libsbml::SBMLDocument *document);

    /**
     * disallow creation of empty SBMLmodel.
     */
    SBMLmodel();

public:
    static SBMLmodel*            FromFile(std::string &sFileName);
    static SBMLmodel*            FromSBML(std::string &sSBML);

    /**
     * Attach SBMLmodel to an existing sbml model.
     */
    SBMLmodel(const libsbml::Model *model);

    /**
     * Create an sbml document from an sbml formatted string.
     */
    SBMLmodel(const std::string &sSBML);


    ~SBMLmodel(void);

    const Model*                getModel() const;
    int                         numFloatingSpecies() const;
    int                         numReactions() const ;
    const Species*              getNthFloatingSpecies(int n) const;
    const Species*              getNthBoundarySpecies(int n) const;
    const Reaction*             getNthReaction(int n) const;
};

}
#endif

#endif //ls_SBML_MODEL_H
