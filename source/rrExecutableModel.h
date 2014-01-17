#ifndef rrExecutableModelH
#define rrExecutableModelH
#include "rrOSSpecifics.h"

#include <stdint.h>
#include <string>
#include <list>
#include <ostream>

namespace rr
{

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
    virtual std::string getModelName() = 0;

    virtual void setTime(double _time) = 0;
    virtual double getTime() = 0;

    /**
     * evaluate the initial conditions specified in the sbml, this entails
     * evaluating all InitialAssigments, AssigmentRules, initial values, etc...
     *
     * Sets the the concentrations and ammounts to the values specified
     * by the initial conditions, ModelData::floatingSpeciesInitConcentrations, i.e. the ModelData::floatingSpeciesAmounts[:]
     * are set to ModelData::floatingSpeciesInitConcentrations[:] * compartment volume, and ModelData::y[:] is
     * set to ModelData::floatingSpeciesInitConcentrations[:].
     *
     * This sets the concentrations and ammounts to either initialAmount or
     * initialConcentration (which ever exists) or 0 if they are missing. A
     * later call to evalInitialAssignments will apply any initialAssigments to
     * update the concentations and ammounts.
     *
     * The the model state is fully set.
     */
    virtual void evalInitialConditions() = 0;

    /**
     * Loads the initial conditions into the current model state.
     *
     * Initial conditions may have been updated at any time externally.
     */
    virtual void reset() = 0;



    /************************ Floating Species Section ****************************/
    #if (1) /**********************************************************************/
    /******************************************************************************/

    /**
     * dependent species are defined by rules and the only way to change them
     * is by changing the values on which they depend.
     */
    virtual int getNumDepFloatingSpecies() = 0;

    /**
     * total number of floating species.
     */
    virtual int getNumFloatingSpecies() = 0;

    virtual int getFloatingSpeciesIndex(const std::string& eid) = 0;
    virtual std::string getFloatingSpeciesId(int index) = 0;

    /**
     * independent species do are not defined by rules, they typically participate
     * in reactions and can have thier values set at any time.
     */
    virtual int getNumIndFloatingSpecies() = 0;

    /**
     * get the floating species amounts
     *
     * @param[in] len the length of the indx and values arrays.
     * @param[in] indx an array of length len of boundary species to return.
     * @param[out] values an array of at least length len which will store the
     *                returned boundary species amounts.
     */
    virtual int getFloatingSpeciesAmounts(int len, int const *indx,
            double *values) = 0;

    virtual int setFloatingSpeciesAmounts(int len, int const *indx,
            const double *values) = 0;

    virtual int getFloatingSpeciesAmountRates(int len, int const *indx,
            double *values) = 0;


    virtual int getFloatingSpeciesConcentrationRates(int len, int const *indx,
                double *values) = 0;

    /**
     * get the floating species concentrations
     *
     * @param[in] len the length of the indx and values arrays.
     * @param[in] indx an array of length len of boundary species to return.
     * @param[out] values an array of at least length len which will store the
     *                returned boundary species amounts.
     */
    virtual int getFloatingSpeciesConcentrations(int len, int const *indx,
            double *values) = 0;

    /**
     * set the floating species concentrations
     *
     * @param[in] len the length of the indx and values arrays.
     * @param[in] indx an array of length len of boundary species to return.
     * @param[in] values an array of at least length len which will store the
     *                returned boundary species amounts.
     */
    virtual int setFloatingSpeciesConcentrations(int len, int const *indx,
            double const *values) = 0;

    /**
     * Set the initial concentrations of the floating species.
     *
     * Takes the same indices as the other floating species methods.
     *
     * Note, if a floating species has an initial assignment rule,
     * than the initial conditions value can only be set by
     * updating the values on which it depends, it can not be set
     * directly.
     */
    virtual int setFloatingSpeciesInitConcentrations(int len, int const *indx,
                double const *values) = 0;

    /**
     * Get the initial concentrations of the floating species,
     * uses the same indexing as the other floating species methods.
     */
    virtual int getFloatingSpeciesInitConcentrations(int len, int const *indx,
                    double *values) = 0;

    /**
     * Set the initial amounts of the floating species.
     *
     * Takes the same indices as the other floating species methods.
     *
     * Note, if a floating species has an initial assignment rule,
     * than the initial conditions value can only be set by
     * updating the values on which it depends, it can not be set
     * directly.
     */
    virtual int setFloatingSpeciesInitAmounts(int len, int const *indx,
                double const *values) = 0;

    /**
     * Get the initial amounts of the floating species,
     * uses the same indexing as the other floating species methods.
     */
    virtual int getFloatingSpeciesInitAmounts(int len, int const *indx,
                    double *values) = 0;

    /************************ End Floating Species Section ************************/
    #endif /***********************************************************************/
    /******************************************************************************/



    /************************ Boundary Species Section ****************************/
    #if (1) /**********************************************************************/
    /******************************************************************************/


    /**
     * get the number of boundary species.
     */
    virtual int getNumBoundarySpecies() = 0;
    virtual int getBoundarySpeciesIndex(const std::string &eid) = 0;
    virtual std::string getBoundarySpeciesId(int index) = 0;

    /**
     * get the boundary species amounts
     *
     * @param[in] len the length of the indx and values arrays.
     * @param[in] indx an array of length len of boundary species to return.
     * @param[out] values an array of at least length len which will store the
     *                returned boundary species amounts.
     */
    virtual int getBoundarySpeciesAmounts(int len, int const *indx,
            double *values) = 0;


    /**
     * get the boundary species concentrations
     *
     * @param[in] len the length of the indx and values arrays.
     * @param[in] indx an array of length len of boundary species to return.
     * @param[out] values an array of at least length len which will store the
     *                returned boundary species amounts.
     */
    virtual int getBoundarySpeciesConcentrations(int len, int const *indx,
            double *values) = 0;

    /**
     * set the boundary species concentrations
     *
     * @param[in] len the length of the indx and values arrays.
     * @param[in] indx an array of length len of boundary species to return.
     * @param[in] values an array of at least length len which will store the
     *                returned boundary species amounts.
     */
    virtual int setBoundarySpeciesConcentrations(int len, int const *indx,
            double const *values) = 0;


    /************************ End Boundary Species Section ************************/
    #endif /***********************************************************************/
    /******************************************************************************/


    /************************ Global Parameters Section ***************************/
    #if (1) /**********************************************************************/
    /******************************************************************************/

    /**
     * get the number of global parameters
     */
    virtual int getNumGlobalParameters() = 0;

    /**
     * index of the global parameter id, -1 if it does not exist.
     */
    virtual int getGlobalParameterIndex(const std::string& eid) = 0;

    /**
     * id of the indexed global parameter.
     */
    virtual std::string getGlobalParameterId(int index) = 0;

    /**
     * get the global parameter values
     *
     * @param[in] len the length of the indx and values arrays.
     * @param[in] indx an array of length len of boundary species to return.
     * @param[out] values an array of at least length len which will store the
     *                returned boundary species amounts.
     */
    virtual int getGlobalParameterValues(int len, int const *indx,
            double *values) = 0;

    virtual int setGlobalParameterValues(int len, int const *indx,
            const double *values) = 0;


    /************************ Global Parameters Species Section *******************/
    #endif /***********************************************************************/
    /******************************************************************************/


    /************************ Compartments Section ********************************/
    #if (1) /**********************************************************************/
    /******************************************************************************/

    virtual int getNumCompartments() = 0;
    virtual int getCompartmentIndex(const std::string& eid) = 0;
    virtual std::string getCompartmentId(int index) = 0;

    /**
     * get the compartment volumes
     *
     * @param[in] len the length of the indx and values arrays.
     * @param[in] indx an array of length len of boundary species to return.
     * @param[out] values an array of at least length len which will store the
     *                returned boundary species amounts.
     */
    virtual int getCompartmentVolumes(int len, int const *indx,
            double *values) = 0;

    virtual int setCompartmentVolumes(int len, int const *indx,
                const double *values) = 0;

    /**
     * Set the initial volumes of the compartments.
     *
     * Takes the same indices as the other compartment methods.
     *
     * Note, if a compartment has an initial assignment rule,
     * than the initial conditions value can only be set by
     * updating the values on which it depends, it can not be set
     * directly.
     */
    virtual int setCompartmentInitVolumes(int len, int const *indx,
                double const *values) = 0;

    /**
     * Get the initial volume of the compartments,
     * uses the same indexing as the other compartment methods.
     */
    virtual int getCompartmentInitVolumes(int len, int const *indx,
                    double *values) = 0;


    /************************ End Compartments Species Section ********************/
    #endif /***********************************************************************/
    /******************************************************************************/


    /************************ Selection Ids Species Section ***********************/
    #if (1) /**********************************************************************/
    /******************************************************************************/

    /**
     * populates a given list with all the ids that this class can accept.
     *
     * @param ids: a list of strings that will be filled by this class.
     * @param types: the types of ids that are requested. Can be set to
     * 0xffffffff to request all the ids that this class supports.
     * This should by a bitwise OR of the filelds in SelectionRecord::SelectionType
     */
    virtual void getIds(int types, std::list<std::string> &ids) = 0;

    /**
     * returns a bit field of the ids that this class supports.
     */
    virtual int getSupportedIdTypes() = 0;

    /**
     * gets the value for the given id string. The string must be a SelectionRecord
     * string that is accepted by this class.
     */
    virtual double getValue(const std::string& id) = 0;

    /**
     * sets the value coresponding to the given selection stringl
     */
    virtual void setValue(const std::string& id, double value) = 0;


    /************************ End Selection Ids Species Section *******************/
    #endif /***********************************************************************/
    /******************************************************************************/

    /**
     * allocate a block of memory and copy the stochiometric values into it,
     * and return it.
     *
     * The caller is responsible for freeing the memory that is referenced by data.
     *
     * @param[out] rows will hold the number of rows in the matrix.
     * @param[out] cols will hold the number of columns in the matrix.
     * @param[out] data a pointer which will hold a newly allocated memory block.
     */
    virtual int getStoichiometryMatrix(int* rows, int* cols, double** data) = 0;

    /**
     * Get the current stiochiometry value for the given species / reaction.
     *
     * If either are not valid, NaN is returned.
     */
    virtual double getStoichiometry(int speciesIndex, int reactionIndex) = 0;


    virtual int getNumConservedMoieties() = 0;
    virtual int getConservedMoietyIndex(const std::string& eid) = 0;
    virtual std::string getConservedMoietyId(int index) = 0;
    virtual int getConservedMoietyValues(int len, int const *indx, double *values) = 0;
    virtual int setConservedMoietyValues(int len, int const *indx,
            const double *values) = 0;

    virtual int getNumRules() = 0;

    /**
     * get the number of reactions the model has
     */
    virtual int getNumReactions() = 0;

    /**
     * get the index of a named reaction
     * @returns >= 0 on success, < 0 on failure.
     */
    virtual int getReactionIndex(const std::string& eid) = 0;

    /**
     * get the name of the specified reaction
     */
    virtual std::string getReactionId(int index) = 0;

    virtual int getReactionRates(int len, int const *indx,
                double *values) = 0;

    /**
     * Evaluate the reaction rates using the current model state.
     *
     * The reaction rates are stored in ModelData::reactionRates.
     */
    virtual void evalReactionRates() = 0;

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
    virtual void getStateVectorRate(double time, const double *y, double* dydt=0) = 0;

    virtual void testConstraints() = 0;

    virtual std::string getInfo() = 0;

    virtual void print(std::ostream &stream) = 0;

    /******************************* Events Section *******************************/
    #if (1) /**********************************************************************/
    /******************************************************************************/

    virtual int getNumEvents() = 0;

    /**
     * get the event status, false if the even is not triggered, true if it is.
     *
     * The reason this returns an unsigned char instead of a bool array is this
     * array is typically stuffed into an std::vector, and std::vector<bool> is
     * seriously broken and can not be used as a C array.
     *
     * So, on every modern system I'm aware of, bool is an unsigned char, so
     * use that data type here.
     */
    virtual int getEventTriggers(int len, const int *indx, unsigned char *values) = 0;

    virtual void evalEvents(double timeEnd, const unsigned char* previousEventStatus,
                const double *initialState, double* finalState) = 0;

    virtual int applyPendingEvents(const double *stateVector, double timeEnd, double tout) = 0;

    /**
     * evaluate the event 'roots' -- when events transition form triggered - non-triggered
     * or triggered to non-triggered state.
     *
     * Simplest method is to return 1 for triggered, -1 for not-triggered, so long
     * as there is a zero crossing.
     *
     * @param time[in] current time
     * @param y[in] the state vector
     * @param gdot[out] result event roots, this is of length numEvents.
     */
    virtual void evalEventRoots(double time, const double* y, double* gdot) = 0;

    virtual double getNextPendingEventTime(bool pop) = 0;

    virtual int getPendingEventSize() = 0;

    virtual void resetEvents() = 0;

    /**
     * need a virtual destructor as object implementing this interface
     * can be deleted directly, i.e.
     * ExecutableModel *p = createModel(...);
     * delete p;
     */
    virtual ~ExecutableModel() {};

    /******************************* Events Section *******************************/
     #endif /**********************************************************************/
    /******************************************************************************/

    /******************************* Deprecated Section ***************************/

    /**
     * @deprecated
     * did something with the old C backend, obsolete now.
     */
    virtual bool getConservedSumChanged() = 0;

    /**
     * @deprecated
     * compatability with C backend, does nothing anymore.
     */
    virtual void computeConservedTotals() = 0;

    /**
     * @deprecated
     * did something with old C backend, here for compatility.
     */
    virtual void setConservedSumChanged(bool) = 0;

    /**
     * @deprecated
     * The old C backend had state variables for everything,
     * here only for compatability, does nothing in LLVM.
     *
     * Only for floating species.
     */
    virtual void convertToAmounts() = 0;

    /**
     * @deprecated
     */
    virtual void convertToConcentrations() = 0;

    /**
     * @deprecated
     */
    virtual void updateDependentSpeciesValues() = 0;

    /**
     * @deprecated
     */
    virtual void computeAllRatesOfChange() = 0;
};

/**
 * dump the model to a stream convenience func
 */
RR_DECLSPEC std::ostream& operator << (std::ostream &stream, ExecutableModel* model);


}
#endif
