#ifndef rrExecutableModelH
#define rrExecutableModelH
#include "rrOSSpecifics.h"

#include <string>
#include <ostream>

namespace rr
{

using std::string;

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
     * reset the model to its original state
     */
    virtual void reset() = 0;


    virtual int getNumIndependentSpecies() = 0;
    virtual int getNumDependentSpecies() = 0;

    virtual int getNumFloatingSpecies() = 0;
    virtual int getFloatingSpeciesIndex(const string& eid) = 0;
    virtual string getFloatingSpeciesId(int index) = 0;

    virtual int getNumBoundarySpecies() = 0;
    virtual int getBoundarySpeciesIndex(const string &eid) = 0;
    virtual string getBoundarySpeciesId(int index) = 0;
    virtual int getBoundarySpeciesCompartmentIndex(int index) = 0;

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
     * a pointless method that will go away
     */
    virtual int setFloatingSpeciesInitConcentrations(int len, int const *indx,
                double const *values) = 0;

    /**
     * pointless
     */
    virtual int getFloatingSpeciesInitConcentrations(int len, int const *indx,
                    double *values) = 0;

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


    virtual int getNumGlobalParameters() = 0;
    virtual int getGlobalParameterIndex(const string& eid) = 0;

    virtual string getGlobalParameterId(int index) = 0;


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

    virtual int getNumCompartments() = 0;
    virtual int getCompartmentIndex(const string& eid) = 0;
    virtual string getCompartmentId(int index) = 0;

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
     * no clue how this is supposed to work, here for compatibility
     */
    virtual double getStoichiometry(int index) = 0;


    virtual int getNumConservedSums() = 0;
    virtual int getConservedSumIndex(const string& eid) = 0;
    virtual string getConservedSumId(int index) = 0;
    virtual int getConservedSums(int len, int const *indx, double *values) = 0;
    virtual int setConservedSums(int len, int const *indx,
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

    virtual int getNumEvents() = 0;

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

    virtual void resetEvents() = 0;

    virtual void testConstraints() = 0;

    virtual string getInfo() = 0;

    virtual void print(std::ostream &stream) = 0;

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
    virtual int getEventStatus(int len, const int *indx, unsigned char *values) = 0;


    virtual void evalEvents(double timeEnd, const unsigned char* previousEventStatus,
                const double *initialState, double* finalState) = 0;

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

    virtual int applyPendingEvents(const double *stateVector, double timeEnd, double tout) = 0;

    virtual double getNextPendingEventTime(bool pop) = 0;

    virtual int getPendingEventSize() = 0;

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


}
#endif
