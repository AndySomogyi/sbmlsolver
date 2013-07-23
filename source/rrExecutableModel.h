#ifndef rrExecutableModelH
#define rrExecutableModelH
#include "rrOSSpecifics.h"
#include "rrModelData.h"
#include "rrSymbolList.h"
#include "rrStringList.h"

#include <list>
#include <string>
#include <vector>
#include <ostream>

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

    virtual bool getConservedSumChanged() = 0;
    virtual void setConservedSumChanged(bool) = 0;

    /**
     * evaluate the initial conditions specified in the sbml, this entails
     * evaluating all InitialAssigments, AssigmentRules, initial values, etc...
     *
     * The the model state is fully set.
     */
    virtual void evalInitialConditions() = 0;

    /**
     * reset the model to its original state
     */
    virtual void reset() = 0;

    /**
     * get a reference to the internal ModelData structure.
     */
    virtual ModelData& getModelData() = 0;

    enum StateStackOptions
    {
        /**
         * Default behavior is to pop and restore previous state, (like OpenGL),
         * this pops the last save and discards it without restoring the state.
         */
        PopDiscard = 0x00000001
    };

    /**
     * A ExecutableModel holds a stack of states, the entire state of this
     * model is pushed onto the saved state stack, and the current state
     * remains unchanged.
     *
     * @param[in] options a bitfield of options from StateStackOptions.
     *
     * @returns the size of the saved stack after the current state has been
     * pushed.
     */
    virtual int pushState(unsigned options = 0) = 0;

    /**
     * restore the state from a previously saved state, if the state stack
     * is empty, this has no effect.
     *
     * @param[in] options a bitfield of options from StateStackOptions.
     *
     * @returns the size of the saved stack after the top has been poped.
     */
    virtual int popState(unsigned options = 0) = 0;

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

    /**
     * Evaluate the reaction rates using the current model state.
     *
     * The reaction rates are stored in ModelData::reactionRates.
     */
    virtual void evalReactionRates() = 0;


    virtual int getNumLocalParameters(int reactionId) = 0;

    /**
     * sets the ammounts (ModelData::ammounts) by multipying the concentations
     * (ModelData::y) by the compartment volume that the species belongs to.
     *
     * Only for floating species.
     */
    virtual void convertToAmounts() = 0;

    virtual void computeConservedTotals() = 0;

    virtual double getConcentration(int index) = 0;

    /**
     * set the 'values' of the rate rules.
     *
     * Rate rules are a set of rate equations, i.e dy/dt, where y is some
     * existing model variable, i.e. volume, parameter, etc...
     *
     * This function sets all of the values that the derivatives integrated
     * to, i.e. if dy/dt is the rate of change of say a compartment volume,
     * than this function takes that compartment volume and stores it in
     * the appropriate place.
     *
     * The length of rateRuleValues obviously must be the number of
     * rate rules we have.
     */
    virtual void setRateRuleValues(const double *rateRuleValues) = 0;


    /**
     * get the 'values' i.e. the what the rate rule integrates to, and
     * store it in the given array.
     *
     * The length of rateRuleValues obviously must be the number of
     * rate rules we have.
     */
    virtual void getRateRuleValues(double *rateRuleValues) = 0;

    /**
     * copies the internal model state vector into the provided
     * buffer.
     *
     * @param[out] stateVector a buffer to copy the state vector into, if NULL,
     *         return the size required.
     *
     * @return the number of items coppied into the provided buffer, if
     *         stateVector is NULL, returns the length of the state vector.
     */
    virtual int getStateVector(double *stateVector) = 0;

    /**
     * sets the internal model state to the provided packed state vector.
     *
     * @param[in] an array which holds the packed state vector, must be
     *         at least the size returned by getStateVector.
     *
     * @return the number of items copied from the state vector, negative
     *         on failure.
     */
    virtual int setStateVector(const double *stateVector) = 0;


    virtual void convertToConcentrations() = 0;
    virtual void updateDependentSpeciesValues() = 0;
    virtual void computeAllRatesOfChange() = 0;

    /**
     * the state vector y is the rate rule values and floating species
     * concentrations concatenated. y is of length numFloatingSpecies + numRateRules.
     *
     * The state vector is packed such that the first n raterule elements are the
     * values of the rate rules, and the last n floatingspecies are the floating
     * species values.
     *
     * @param[in] time current simulator time
     * @param[in] y state vector, must be either null, or have a size of that
     *         speciefied by getStateVector. If y is null, then the model is
     *         evaluated using its current state. If y is not null, then the
     *         y is considered the state vector.
     * @param[out] dydt calculated rate of change of the state vector, if null,
     *         it is ignored.
     */
    virtual void evalModel(double time, const double *y, double* dydt=0) = 0;

    virtual void evalEvents(const double time, const double *y) = 0;
    virtual void resetEvents() = 0;
    virtual void testConstraints() = 0;

    virtual string getInfo() = 0;
    
    virtual void print(std::ostream &stream) = 0;


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
 * dump the model to a stream convenience func
 */
RR_DECLSPEC std::ostream& operator << (std::ostream &stream, ExecutableModel* model);


/**
 * zero out the memory occupied by a ModelData struct, equivalent to
 * memset(&data, 0, sizeof(ModelData));
 *
 * performs NO allocaation of memory.
 *
 */
RR_DECLSPEC void initModelData(ModelData &data);

/**
 * dump the ModelData to an output stream.
 */
RR_DECLSPEC std::ostream& operator<< (std::ostream& os, const ModelData& data);

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
