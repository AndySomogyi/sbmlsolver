#ifndef rrExecutableModelH
#define rrExecutableModelH
#include <list>
#include <string>
#include <vector>

#include "rrOSSpecifics.h"
#include "rrModelData.h"
#include "rrSymbolList.h"
#include "rrStringList.h"

namespace rr
{

using std::string;
using std::vector;

/**
 * The ExecutableModel interface provides a way to access an
 * sbml model that was compiled, JIT'd or interpreted
 * as executable (runnable) module.
 *
 * An ExecutableModel holds a ModelData structure, all the simulation
 * values are stored in the ModelData struct, i.e. the dynamic state
 * of the model is fully contained in the ModelData structure.
 *
 * An ExecutableModel shoud also contain all of the initial condisions,
 * rules, functions and whatever other semantic information that was
 * specified in the sbml model.
 */
class RR_DECLSPEC ExecutableModel
{
public:
    /**
     * get the name of the model
     */
    virtual string getModelName() = 0;

    virtual void setTime(double _time) = 0;
    virtual double getTime() = 0;

    /**
     * get a reference to the internal ModelData structure.
     */
    virtual ModelData& getModelData() = 0;

    /**
     * Sets the initial floating species concentrations, ModelData::floatingSpeciesInitConcentrations to
     * the initial value specified in the sbml model. The initial concentration
     * value can be spefied either by a constant value, or with an assigment rule.
     *
     * TODO: Perhaps this should be renamed 'initializeInitialConcentrations' ???
     */
    virtual void initializeInitialConditions() = 0;

    /**
     * Sets the the concentrations and ammounts to the values specified
     * by the initial conditions, ModelData::floatingSpeciesInitConcentrations, i.e. the ModelData::floatingSpeciesAmounts[:]
     * are set to ModelData::floatingSpeciesInitConcentrations[:] * compartment volume, and ModelData::y[:] is
     * set to ModelData::floatingSpeciesInitConcentrations[:].
     *
     * This sets the concentrations and ammounts to either initialAmount or
     * initialConcentration (which ever exists) or 0 if they are missing. A
     * later call to evalInitialAssignments will apply any initialAssigments to
     * update the concentations and ammounts.
     */
    virtual void setInitialConditions() = 0;

    // functions --------------------------------------------------------
    virtual int getNumIndependentSpecies() = 0;
    virtual int getNumDependentSpecies() = 0;

    virtual int getNumFloatingSpecies() = 0;
    virtual int getFloatingSpeciesIndex(const string& name) = 0;
    virtual string getFloatingSpeciesName(int index) = 0;

    virtual int getNumBoundarySpecies() = 0;
    virtual int getBoundarySpeciesIndex(const string &name) = 0;
    virtual string getBoundarySpeciesName(int index) = 0;
    virtual int getBoundarySpeciesCompartmentIndex(int index) = 0;

    virtual int getNumGlobalParameters() = 0;
    virtual int getGlobalParameterIndex(const string& name) = 0;
    virtual string getGlobalParameterName(int index) = 0;
    virtual double getGlobalParameterValue(int index) = 0;
    virtual void setGlobalParameterValue(int index, double value) = 0;

    virtual int getNumCompartments() = 0;
    virtual int getCompartmentIndex(const string& name) = 0;
    virtual string getCompartmentName(int index) = 0;

    /**
     * get the number of reactions the model has
     */
    virtual int getNumReactions() = 0;

    /**
     * get the index of a named reaction
     * @returns >= 0 on success, < 0 on failure.
     */
    virtual int getReactionIndex(const std::string& reactionName) = 0;

    /**
     * get the name of the specified reaction
     */
    virtual std::string getReactionName(int index) = 0;

    virtual int getNumEvents() = 0;
    virtual void computeEventPriorites() = 0;
    virtual void setConcentration(int index, double value) = 0;
    virtual void computeReactionRates(double time, double* y) = 0;

    /**
     * Sets the compartment volumes to the conditions specified in the sbml model.
     * Compartment volumes can be specified either with a fixed value, or
     * a rule based assigment.
     *
     * TODO: would it make more sense to call this 'setInitialVolumes' ???
     *       note, it is redundent using 'Compartment' and 'Volume' as
     *       only compartments have volumes.
     *
     * This sets the compartment volumes to the size attribute specified in the
     * smbl. If an initialAssigment element exists, it will will be applied
     * by evalInitialAssignments.
     */
    virtual void setCompartmentVolumes() = 0;

    virtual int getNumLocalParameters(int reactionId) = 0;

    virtual void computeRules(vector<double>& _y) = 0;

    virtual void computeRules(double* ay, int size) = 0;

    virtual void setParameterValues() = 0;

    /**
     * set the concentations (ModelData::bc) to the initialConcentration or
     * initialAmmount value specified in the sbml. If an initialAssigment
     * element exists, it can be applied with evalInitialAssignments.
     */
    virtual void setBoundaryConditions() = 0;

    /**
     * evaluate and applies all of the initialAssigment rules. This
     * will overwrite any values subseqently specified by initialConcentration,
     * initialSize, ....
     */
    virtual void evalInitialAssignments() = 0;

    /**
     * sets the ammounts (ModelData::ammounts) by multipying the concentations
     * (ModelData::y) by the compartment volume that the species belongs to.
     *
     * Only for floating species.
     */
    virtual void convertToAmounts() = 0;

    virtual void computeConservedTotals() = 0;

    virtual double getConcentration(int index) = 0;

    //Access dll data
    virtual vector<double> getCurrentValues() = 0;
    virtual double getAmounts(const int& i) = 0;
    virtual void initializeRates() = 0;
    virtual void assignRates() = 0;
    virtual void assignRates(vector<double>& rates) = 0;
    virtual void convertToConcentrations() = 0;
    virtual void updateDependentSpeciesValues(double* _y) = 0;
    virtual void computeAllRatesOfChange() = 0;

    /**
     * the state vector y is the rate rule values and floating species
     * concentrations concatenated. y is of length numFloatingSpecies + numRateRules.
     */
    virtual void evalModel(const double& time, const vector<double>& y) = 0;
    virtual void evalEvents(const double& time, const vector<double>& y) = 0;
    virtual void resetEvents() = 0;
    virtual void testConstraints() = 0;
    virtual void initializeRateRuleSymbols() = 0;
    virtual string getInfo() = 0;


    virtual const SymbolList &getConservations() = 0;
    virtual const StringList getConservationNames() = 0;

    /**
     * need a virtual destructor as object implementing this interface
     * can be deleted directly, i.e.
     * ExecutableModel *p = createModel(...);
     * delete p;
     */
    virtual ~ExecutableModel() {};
};

/**
 * zero out the memory occupied by a ModelData struct, equivalent to
 * memset(&data, 0, sizeof(ModelData));
 *
 * performs NO allocaation of memory.
 *
 */
void RR_DECLSPEC initModelData(ModelData &data);

/**
 * Allocate memory for all the data buffers in a ModelData structure,
 * (with the exception of the stoichiometry struct),
 * based on what each buffer size variable indicates, i.e.
 * if data.ySize is 10, data.y will now point to a length 10 double array.
 * The data.modelName field will be a newly allocated null terminated c string.
 *
 * The ModelData structure buffers should be freed with freeModelDataBuffers.
 *
 * The following arrays are also allocated here, the model
 * populates these with pointers to static strings.
 *
 * char**                              variableTable;
 * char**                              boundaryTable;
 * char**                              globalParameterTable;
 */
void RR_DECLSPEC allocModelDataBuffers(ModelData &data, const string& modelName);

/**
 * Free all the data referenced by the ModelData pointers.
 * Does NOT free the ModelData itself, ONLY the buffer data.
 */
void RR_DECLSPEC freeModelDataBuffers(ModelData &data);

}
#endif
