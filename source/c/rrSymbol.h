#ifndef rrSymbolH
#define rrSymbolH
#include <string>
#include <ostream>
#include <limits>
#include "rrExporter.h"
using std::string;
using std::ostream;

namespace rr
{
/**
 * An SBML species definition.
 * TODO: should this be named something better than 'Symbol', say SpeciesDefinition or something???
 * TODO: Why do we even have this? Shoud'nt this just be a sbml::Species ???
 */
class RR_DECLSPEC Symbol
{
public:
    /**
     * Set if species also has a rate rule. Use to prevent a
     * floatingSpeciesAmountRates being output
     * in the model function if there is a rate rule for it.
     */
    bool                rateRule;

    /**
     * set to initial concentration. If initialAmmount is given, this is converted to
     * a concentration via initialAmmount / initialCompartmentVolume.
     * TODO: should this be named 'initialConcentration'???
     */
    double              value;
    bool                constant;


    /**
     * Used when symbol is a species
     */
    std::string              compartmentName;

    /**
     * used when symbol is a species
     */
    bool                hasOnlySubstance;

    /**
     * There are only two places where formula was created, in
     * rrModelSymbols.cpp, readBoundarySpecies and readFloatingSpeciesConcentrationList
     * In both cases, formula was formated as the exact same C language std::string of
     * formula < <toString(dValue, ModelSymbols::mDoubleFormat)<< "/ md->c[" << nCompartmentIndex << "]";,
     *
     * where dValue was always the initialAmmount. So, ammount / volume = concentration.
     *
     * General idea behind 'formula' was to convert initialAmmount to an initial concentration,
     * that logic belongs in the code generator, not the symbolic processor,
     * so we just declare what the initialAmmount and compartment volume
     * here and leave the logic of converting to concentration
     * in the code generators.
     *
     * There is no need to keep the compartment index here, the code generators
     * can just grab it from the compartment list using the compartment name.
     */
    std::string              formula;

    /**
     * Used when storing local parameters, keyName is the reaction name
     */
    std::string              keyName;
    std::string              name;

    //Constructors
    Symbol(const std::string& _name = "", const double& _value = std::numeric_limits<double>::quiet_NaN());
    Symbol(const std::string& _keyName, const std::string& _name, const double& _value= std::numeric_limits<double>::quiet_NaN());
    Symbol(const std::string& _name, const double& _value, const std::string& _compartmentName);
    Symbol(const std::string& _name, const double& _value, const std::string& _compartmentName, const std::string& _formula);

}; //class rr::Symbol

std::ostream& operator<<(std::ostream& stream, const Symbol& symbol);

}//namespace rr
#endif


