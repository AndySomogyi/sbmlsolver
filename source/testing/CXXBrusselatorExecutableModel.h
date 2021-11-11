/*
 * CXXBrusselatorExecutableModel.h
 *
 *  Created on: Dec 11, 2014
 *      Author: andy
 */

#ifndef CXXBRUSSELATOREXECUTABLEMODEL_H_
#define CXXBRUSSELATOREXECUTABLEMODEL_H_

#include <rrExecutableModel.h>
#include "Dictionary.h"

namespace rrtesting
{

class CXXBrusselatorExecutableModel: public rr::ExecutableModel
{
public:
    CXXBrusselatorExecutableModel(const rr::Dictionary* options);
    virtual ~CXXBrusselatorExecutableModel();


public:

    /**
     * @author JKM
     * @date 07/31/2015
     * @brief Returns a human-readable description of the code generation backend,
     * e.g. LLVM, legacy C, etc.
     */
    virtual std::string getExecutableModelDesc() const override {
        return "Brusselator Executable Model";
    }

    /**
     * get the name of the model
     */
    virtual std::string getModelName() override;

    virtual void setTime(double _time) override;
    virtual double getTime() override;


    /**
     * Loads the initial conditions into the current model state.
     *
     * Initial conditions may have been updated at any time externally.
     */
    virtual void reset() override;



    /************************ Floating Species Section ****************************/
    #if (1) /**********************************************************************/
    /******************************************************************************/

    /**
     * dependent species are defined by rules and the only way to change them
     * is by changing the values on which they depend.
     */
    virtual int getNumDepFloatingSpecies() override;

    /**
     * total number of floating species.
     */
    virtual int getNumFloatingSpecies() override;

    virtual int getFloatingSpeciesIndex(const std::string& eid) override;
    virtual std::string getFloatingSpeciesId(size_t index) override;

    /**
     * independent species do are not defined by rules, they typically participate
     * in reactions and can have thier values set at any time.
     */
    virtual int getNumIndFloatingSpecies() override;

    /**
     * get the floating species amounts
     *
     * @param[in] len the length of the indx and values arrays.
     * @param[in] indx an array of length len of boundary species to return.
     * @param[out] values an array of at least length len which will store the
     *                returned boundary species amounts.
     */
    virtual int getFloatingSpeciesAmounts(size_t len, int const *indx,
            double *values) override;

    virtual int setFloatingSpeciesAmounts(size_t len, int const *indx,
            const double *values) override;

    virtual int getFloatingSpeciesAmountRates(size_t len, int const *indx,
            double *values) override;


    virtual int getFloatingSpeciesConcentrationRates(size_t len, int const *indx,
                double *values) override;

    /**
     * get the floating species concentrations
     *
     * @param[in] len the length of the indx and values arrays.
     * @param[in] indx an array of length len of boundary species to return.
     * @param[out] values an array of at least length len which will store the
     *                returned boundary species amounts.
     */
    virtual int getFloatingSpeciesConcentrations(size_t len, int const *indx,
            double *values) override;

    /**
     * set the floating species concentrations
     *
     * @param[in] len the length of the indx and values arrays.
     * @param[in] indx an array of length len of boundary species to return.
     * @param[in] values an array of at least length len which will store the
     *                returned boundary species amounts.
     */
    virtual int setFloatingSpeciesConcentrations(size_t len, int const *indx,
            double const *values) override;

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
    virtual int setFloatingSpeciesInitConcentrations(size_t len, int const *indx,
                double const *values) override;

    /**
     * Get the initial concentrations of the floating species,
     * uses the same indexing as the other floating species methods.
     */
    virtual int getFloatingSpeciesInitConcentrations(size_t len, int const *indx,
                    double *values) override;

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
    virtual int setFloatingSpeciesInitAmounts(size_t len, int const *indx,
                double const *values) override;

    /**
     * Get the initial amounts of the floating species,
     * uses the same indexing as the other floating species methods.
     */
    virtual int getFloatingSpeciesInitAmounts(size_t len, int const *indx,
                    double *values) override;

    /************************ End Floating Species Section ************************/
    #endif /***********************************************************************/
    /******************************************************************************/



    /************************ Boundary Species Section ****************************/
    #if (1) /**********************************************************************/
    /******************************************************************************/


    /**
     * get the number of boundary species.
     */
    virtual int getNumBoundarySpecies() override;
    virtual int getBoundarySpeciesIndex(const std::string &eid) override;
    virtual std::string getBoundarySpeciesId(size_t index) override;

    /**
     * get the boundary species amounts
     *
     * @param[in] len the length of the indx and values arrays.
     * @param[in] indx an array of length len of boundary species to return.
     * @param[out] values an array of at least length len which will store the
     *                returned boundary species amounts.
     */
    virtual int getBoundarySpeciesAmounts(size_t len, int const *indx,
            double *values) override;


    /**
     * get the boundary species concentrations
     *
     * @param[in] len the length of the indx and values arrays.
     * @param[in] indx an array of length len of boundary species to return.
     * @param[out] values an array of at least length len which will store the
     *                returned boundary species amounts.
     */
    virtual int getBoundarySpeciesConcentrations(size_t len, int const *indx,
            double *values) override;

    /**
     * set the boundary species concentrations
     *
     * @param[in] len the length of the indx and values arrays.
     * @param[in] indx an array of length len of boundary species to return.
     * @param[in] values an array of at least length len which will store the
     *                returned boundary species concentrations.
     */
    virtual int setBoundarySpeciesConcentrations(size_t len, int const *indx,
            double const *values) override;

    /**
     * set the boundary species amounts
     *
     * @param[in] len the length of the indx and values arrays.
     * @param[in] indx an array of length len of boundary species to return.
     * @param[in] values an array of at least length len which will store the
     *                returned boundary species amounts.
     */
    virtual int setBoundarySpeciesAmounts(size_t len, int const* indx,
        double const* values) override;

    /**
     * Set the initial concentrations of the boundary species.
     *
     * Takes the same indices as the other boundary species methods.
     *
     * Note, if a boundary species has an initial assignment rule,
     * than the initial conditions value can only be set by
     * updating the values on which it depends, it can not be set
     * directly.
     */
    virtual int setBoundarySpeciesInitConcentrations(size_t len, int const* indx,
        double const* values) override;

    /**
     * Get the initial concentrations of the boundary species,
     * uses the same indexing as the other boundary species methods.
     */
    virtual int getBoundarySpeciesInitConcentrations(size_t len, int const* indx,
        double* values) override;

    /**
     * Set the initial amounts of the boundary species.
     *
     * Takes the same indices as the other boundary species methods.
     *
     * Note, if a boundary species has an initial assignment rule,
     * than the initial conditions value can only be set by
     * updating the values on which it depends, it can not be set
     * directly.
     */
    virtual int setBoundarySpeciesInitAmounts(size_t len, int const* indx,
        double const* values) override;

    /**
     * Get the initial amounts of the boundary species,
     * uses the same indexing as the other boundary species methods.
     */
    virtual int getBoundarySpeciesInitAmounts(size_t len, int const* indx,
        double* values) override;

    /************************ End Boundary Species Section ************************/
    #endif /***********************************************************************/
    /******************************************************************************/


    /************************ Global Parameters Section ***************************/
    #if (1) /**********************************************************************/
    /******************************************************************************/

    /**
     * get the number of global parameters
     */
    virtual int getNumGlobalParameters() override;

    /**
     * index of the global parameter id, -1 if it does not exist.
     */
    virtual int getGlobalParameterIndex(const std::string& eid) override;

    /**
     * id of the indexed global parameter.
     */
    virtual std::string getGlobalParameterId(size_t index) override;

    /**
     * get the global parameter values
     *
     * @param[in] len the length of the indx and values arrays.
     * @param[in] indx an array of length len of boundary species to return.
     * @param[out] values an array of at least length len which will store the
     *                returned boundary species amounts.
     */
    virtual int getGlobalParameterValues(size_t len, int const *indx,
            double *values) override;

    virtual int setGlobalParameterValues(size_t len, int const *indx,
            const double *values) override;


    /************************ Global Parameters Species Section *******************/
    #endif /***********************************************************************/
    /******************************************************************************/


    /************************ Compartments Section ********************************/
    #if (1) /**********************************************************************/
    /******************************************************************************/

    virtual int getNumCompartments() override;
    virtual int getCompartmentIndex(const std::string& eid) override;
    virtual int getCompartmentIndexForFloatingSpecies(size_t index) override;
    virtual int getCompartmentIndexForBoundarySpecies(size_t index) override;
    virtual std::string getCompartmentId(size_t index) override;

    /**
     * get the compartment volumes
     *
     * @param[in] len the length of the indx and values arrays.
     * @param[in] indx an array of length len of boundary species to return.
     * @param[out] values an array of at least length len which will store the
     *                returned boundary species amounts.
     */
    virtual int getCompartmentVolumes(size_t len, int const *indx,
            double *values) override;

    virtual int setCompartmentVolumes(size_t len, int const *indx,
                const double *values) override;

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
    virtual int setCompartmentInitVolumes(size_t len, int const *indx,
                double const *values) override;

    /**
     * Get the initial volume of the compartments,
     * uses the same indexing as the other compartment methods.
     */
    virtual int getCompartmentInitVolumes(size_t len, int const *indx,
                    double *values) override;


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
    virtual void getIds(int types, std::list<std::string> &ids) override;

    /**
     * returns a bit field of the ids that this class supports.
     */
    virtual int getSupportedIdTypes() override;

    /**
     * gets the value for the given id std::string. The std::string must be a SelectionRecord
     * std::string that is accepted by this class.
     */
    virtual double getValue(const std::string& id) override;

    /**
     * sets the value coresponding to the given selection stringl
     */
    virtual void setValue(const std::string& id, double value) override;


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
    virtual int getStoichiometryMatrix(int* rows, int* cols, double** data) override;

    /**
     * Get the current stiochiometry value for the given species / reaction.
     *
     * If either are not valid, NaN is returned.
     */
    virtual double getStoichiometry(int speciesIndex, int reactionIndex) override;


    virtual int getNumConservedMoieties() override;
    virtual int getConservedMoietyIndex(const std::string& eid) override;
    virtual std::string getConservedMoietyId(size_t index) override;
    virtual int getConservedMoietyValues(size_t len, int const *indx, double *values) override;
    virtual int setConservedMoietyValues(size_t len, int const *indx,
            const double *values) override;

    virtual int getNumRateRules() override;

    /**
     * get the number of reactions the model has
     */
    virtual int getNumReactions() override;

    /**
     * get the index of a named reaction
     * @returns >= 0 on success, < 0 on failure.
     */
    virtual int getReactionIndex(const std::string& eid) override;

    /**
     * get the name of the specified reaction
     */
    virtual std::string getReactionId(size_t index) override;

    /**
     * get the std::vector of reaction rates.
     *
     * @param len: the length of the supplied buffer, must be >= reaction rates size.
     * @param indx: pointer to index array. If NULL, then it is ignored and the
     * reaction rates are copied directly into the supplied buffer.
     * @param values: pointer to user supplied buffer where rates will be stored.
     */
    virtual int getReactionRates(size_t len, int const *indx,
                double *values) override;

    /**
     * get the 'values' i.e. the what the rate rule integrates to, and
     * store it in the given array.
     *
     * The length of rateRuleValues obviously must be the number of
     * rate rules we have.
     */
    virtual void getRateRuleValues(double *rateRuleValues) override;

    /**
     * get the id of an element of the state std::vector.
     */
    virtual std::string getStateVectorId(size_t index) override;

    /**
     * The state std::vector is a std::vector of elements that are defined by
     * differential equations (rate rules) or independent floating species
     * are defined by reactions.
     *
     * To get the ids of the state std::vector elements, use getStateVectorId.
     *
     * copies the internal model state std::vector into the provided
     * buffer.
     *
     * @param[out] stateVector a buffer to copy the state std::vector into, if NULL,
     *         return the size required.
     *
     * @return the number of items coppied into the provided buffer, if
     *         stateVector is NULL, returns the length of the state std::vector.
     */
    virtual int getStateVector(double *stateVector) override;

    /**
     * sets the internal model state to the provided packed state std::vector.
     *
     * @param[in] an array which holds the packed state std::vector, must be
     *         at least the size returned by getStateVector.
     *
     * @return the number of items copied from the state std::vector, negative
     *         on failure.
     */
    virtual int setStateVector(const double *stateVector) override;

    /**
     * the state std::vector y is the rate rule values and floating species
     * concentrations concatenated. y is of length numFloatingSpecies + numRateRules.
     *
     * The state std::vector is packed such that the first n raterule elements are the
     * values of the rate rules, and the last n floatingspecies are the floating
     * species values.
     *
     * @param[in] time current simulator time
     * @param[in] y state std::vector, must be either null, or have a size of that
     *         speciefied by getStateVector. If y is null, then the model is
     *         evaluated using its current state. If y is not null, then the
     *         y is considered the state std::vector.
     * @param[out] dydt calculated rate of change of the state std::vector, if null,
     *         it is ignored.
     */
    virtual void getStateVectorRate(double time, const double *y, double* dydt=0) override;

    virtual void testConstraints() override;

    virtual std::string getInfo() override;

    virtual void print(std::ostream &stream) override;

    /******************************* Events Section *******************************/
    #if (1) /**********************************************************************/
    /******************************************************************************/

    virtual int getNumEvents() override;

    /**
     * get the event status, false if the even is not triggered, true if it is.
     *
     * The reason this returns an unsigned char instead of a bool array is this
     * array is typically stuffed into an std::vector, and std::vector<bool> is
     * well, weird as it's actually implemented as a bitfield, and can not be
     * used as a C array.
     *
     * So, on every modern system I'm aware of, bool is an unsigned char, so
     * use that data type here.
     */
    virtual int getEventTriggers(size_t len, const int *indx, unsigned char *values) override;

    virtual int applyEvents(double timeEnd, const unsigned char* previousEventStatus,
                const double *initialState, double* finalState) override;

    virtual int applyPendingEvents(double timeEnd);

    /**
     * evaluate the event 'roots' -- when events transition form triggered - non-triggered
     * or triggered to non-triggered state.
     *
     * Simplest method is to return 1 for triggered, -1 for not-triggered, so long
     * as there is a zero crossing.
     *
     * @param time[in] current time
     * @param y[in] the state std::vector
     * @param gdot[out] result event roots, this is of length numEvents.
     */
    virtual void getEventRoots(double time, const double* y, double* gdot) override;

    virtual double getNextPendingEventTime(bool pop) override;

    virtual int getPendingEventSize() override;

    virtual void resetEvents() override;


    /******************************* Events Section *******************************/
     #endif /**********************************************************************/
    /******************************************************************************/


    /**
     * Gets the index for an event id.
     * If there is no event with this id, returns -1.
     */
    virtual int getEventIndex(const std::string& eid) override;
    virtual std::string getEventId(size_t index) override;
    virtual void getEventIds(std::list<std::string>& out) override;
    virtual void getAssignmentRuleIds(std::list<std::string>& out) override;
    virtual void getRateRuleIds(std::list<std::string>& out) override;
    virtual void getInitialAssignmentIds(std::list<std::string>& out) override;
    virtual void setEventListener(size_t index, rr::EventListenerPtr eventHandler) override;
    virtual rr::EventListenerPtr getEventListener(size_t index) override;

    /**
     * Get the amount rate of change for the i'th floating species
     * given a reaction rates std::vector.
     *
     * TODO: This should be merged with getFloatingSpeciesAmountRates, but that will
     * break inteface, will do in next point release.
     *
     * TODO: If the conversion factor changes in between getting the
     * reaction rates std::vector via getReactionRates
     *
     * @param index: index of the desired floating speceis rate.
     * @param reactionRates: pointer to buffer of reaction rates.
     */
    virtual double getFloatingSpeciesAmountRate(size_t index,
            const double *reactionRates) override;

    /**
     * reset the model accordign to a bitfield specified by the
     * SelectionRecord::SelectionType values.
     */
    virtual void reset(int options) override;

    /**
     * set the seed used by the random number generator. This will by definition
     * reset the RNG.
     */
    virtual void setRandomSeed(int64_t) override;

    /**
     * get the seed used by the RNG.
     */
    virtual int64_t getRandomSeed() override;

    /**
     * Get a uniform random number between 0 and 1 created by the RNG.
     *
     * The type of RNG can be specified in the config file with the
     * RANDOM_GENERATOR_TYPE key.
     */
    virtual double getRandom() override;

    /**
     * Get the current set of flags
     */
    virtual uint32_t getFlags() const override;

    /**
     * Set certain options that determine the state of the ExecutableModel,
     * these are listed in
     */
    virtual void setFlags(uint32_t) override;
};

} /* namespace rrtesting */

#endif /* CXXBRUSSELATOREXECUTABLEMODEL_H_ */
