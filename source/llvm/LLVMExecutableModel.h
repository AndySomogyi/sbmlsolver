/*
 * LLVMExecutableModel.h
 *
 *  Created on: Jun 3, 2013
 *
 * Author: Andy Somogyi,
 *     email decode: V1 = "."; V2 = "@"; V3 = V1;
 *     andy V1 somogyi V2 gmail V3 com
 */

#ifndef LLVMExecutableModelH
#define LLVMExecutableModelH

#include "rrExecutableModel.h"
#include "LLVMModelDataSymbols.h"
#include "LLVMIncludes.h"

#include "EvalInitialConditionsCodeGen.h"
#include "EvalReactionRatesCodeGen.h"
#include "EvalRateRuleRatesCodeGen.h"
#include "GetValuesCodeGen.h"
#include "GetEventValuesCodeGen.h"
#include "EventAssignCodeGen.h"
#include "EventTriggerCodeGen.h"
#include "EventQueue.h"

namespace rr
{

class RR_DECLSPEC LLVMExecutableModel: public ExecutableModel
{
public:

    /**
     * the default ctor just zeros out all our private bits, then
     * the main construction is handled by the model generator.
     */
    LLVMExecutableModel();


    virtual ~LLVMExecutableModel();

    /**
     * get the name of the model
     */
    virtual string getModelName();
    virtual void setTime(double _time);
    virtual double getTime();

    virtual bool getConservedSumChanged();

    virtual void setConservedSumChanged(bool);

    /**
     * evaluate the initial conditions specified in the sbml, this entails
     * evaluating all InitialAssigments, AssigmentRules, initial values, etc...
     *
     * The the model state is fully set.
     */
    virtual void evalInitialConditions();

    /**
     * reset the model to its original state
     */
    virtual void reset();


    virtual int getNumIndependentSpecies();
    virtual int getNumDependentSpecies();
    virtual int getNumFloatingSpecies();
    virtual int getNumBoundarySpecies();
    virtual int getNumGlobalParameters();

    virtual int getNumCompartments();

    /**
     * get the global parameter values
     *
     * @param[in] len the length of the indx and values arrays.
     * @param[in] indx an array of length len of boundary species to return.
     * @param[out] values an array of at least length len which will store the
     *                returned boundary species amounts.
     */
    virtual int getGlobalParameterValues(int len, int const *indx,
            double *values);

    virtual int setGlobalParameterValues(int len, int const *indx,
            const double *values);

    virtual int getNumReactions();

    virtual int getReactionRates(int len, int const *indx,
                    double *values);

    /**
     * get the compartment volumes
     *
     * @param[in] len the length of the indx and values arrays.
     * @param[in] indx an array of length len of boundary species to return.
     * @param[out] values an array of at least length len which will store the
     *                returned boundary species amounts.
     */
    virtual int getCompartmentVolumes(int len, int const *indx,
            double *values);

    virtual int getNumRules();



    virtual int getNumLocalParameters(int reactionId);


    virtual void convertToAmounts();
    virtual void computeConservedTotals();


    //Access dll data
    virtual void getRateRuleValues(double *rateRuleValues);



    virtual void setRateRuleValues(const double *rateRuleValues);

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
    virtual int getStateVector(double *stateVector);

    /**
     * sets the internal model state to the provided packed state vector.
     *
     * @param[in] an array which holds the packed state vector, must be
     *         at least the size returned by getStateVector.
     *
     * @return the number of items copied from the state vector, negative
     *         on failure.
     */
    virtual int setStateVector(const double *stateVector);

    virtual void convertToConcentrations();
    virtual void updateDependentSpeciesValues();
    virtual void computeAllRatesOfChange();


    /**
     * where most of the juicy bits occur.
     *
     * the state vector y is the rate rule values and floating species
     * concentrations concatenated. y is of length numFloatingSpecies + numRateRules.
     *
     * The state vector is packed such that the first n raterule elements are the
     * values of the rate rules, and the last n floatingspecies are the floating
     * species values.
     *
     * @param[in] time current simulator time
     * @param[in] y state vector, must be of size returned by getStateVector
     * @param[out] dydt calculated rate of change of the state vector, if null,
     * it is ignored.
     */
    virtual void evalModel(double time, const double *y, double* dydt=0);


    virtual void testConstraints();

    virtual string getInfo();

    virtual int getFloatingSpeciesIndex(const string&);
    virtual string getFloatingSpeciesId(int);
    virtual int getBoundarySpeciesIndex(const string&);
    virtual string getBoundarySpeciesId(int);
    virtual int getBoundarySpeciesCompartmentIndex(int);

    /**
     * get the floating species amounts
     *
     * @param[in] len the length of the indx and values arrays.
     * @param[in] indx an array of length len of boundary species to return.
     * @param[out] values an array of at least length len which will store the
     *                returned boundary species amounts.
     */
    virtual int getFloatingSpeciesAmounts(int len, int const *indx,
            double *values);

    virtual int getFloatingSpeciesAmountRates(int len, int const *indx,
            double *values);

    /**
     * get the floating species concentrations
     *
     * @param[in] len the length of the indx and values arrays.
     * @param[in] indx an array of length len of boundary species to return.
     * @param[out] values an array of at least length len which will store the
     *                returned boundary species amounts.
     */
    virtual int getFloatingSpeciesConcentrations(int len, int const *indx,
            double *values);


    /**
     * set the floating species concentrations
     *
     * @param[in] len the length of the indx and values arrays.
     * @param[in] indx an array of length len of boundary species to return.
     * @param[in] values an array of at least length len which will store the
     *                returned boundary species amounts.
     */
    virtual int setFloatingSpeciesConcentrations(int len, int const *indx,
            double const *values);

    virtual int setFloatingSpeciesAmounts(int len, int const *indx,
            const double *values);


    /**
     * get the boundary species amounts
     *
     * @param[in] len the length of the indx and values arrays.
     * @param[in] indx an array of length len of boundary species to return.
     * @param[out] values an array of at least length len which will store the
     *                returned boundary species amounts.
     */
    virtual int getBoundarySpeciesAmounts(int len, int const *indx,
            double *values);

    /**
     * get the boundary species concentrations
     *
     * @param[in] len the length of the indx and values arrays.
     * @param[in] indx an array of length len of boundary species to return.
     * @param[out] values an array of at least length len which will store the
     *                returned boundary species amounts.
     */
    virtual int getBoundarySpeciesConcentrations(int len, int const *indx,
            double *values);

    /**
     * get the boundary species concentrations
     *
     * @param[in] len the length of the indx and values arrays.
     * @param[in] indx an array of length len of boundary species to return.
     * @param[in] values an array of at least length len which will store the
     *                returned boundary species amounts.
     */
    virtual int setBoundarySpeciesConcentrations(int len, int const *indx,
            double const *values);


    virtual int getGlobalParameterIndex(const string&);
    virtual string getGlobalParameterId(int);
    virtual int getCompartmentIndex(const string&);
    virtual string getCompartmentId(int);
    virtual int getReactionIndex(const string&);
    virtual string getReactionId(int);

    virtual void print(std::ostream &stream);

    virtual int getNumConservedSums();
    virtual int getConservedSumIndex(const string& name);
    virtual string getConservedSumId(int index);
    virtual int getConservedSums(int len, int const *indx, double *values);
    virtual int setConservedSums(int len, int const *indx,
            const double *values);



    /**
     * using the current model state, evaluate and store all the reaction rates.
     */
    virtual void evalReactionRates();









    virtual int setCompartmentVolumes(int len, int const *indx,
            const double *values);
    virtual int setFloatingSpeciesInitConcentrations(int len, int const *indx,
            double const *values);
    virtual int getFloatingSpeciesInitConcentrations(int len, int const *indx,
            double *values);
    virtual double getStoichiometry(int index);


    /******************************* Events Section *******************************/
    #if (1) /**********************************************************************/
    /******************************************************************************/
public:
    virtual int getNumEvents();

    virtual int getEventTriggers(int len, const int *indx, unsigned char *eventState);

    virtual void evalEvents(double timeEnd, const unsigned char* previousEventState,
            const double *initialState, double* finalState);

    virtual int applyPendingEvents(const double *stateVector, double timeEnd,
            double tout);

    virtual void evalEventRoots(double time, const double* y, double* gdot);

    virtual double getNextPendingEventTime(bool pop);

    virtual int getPendingEventSize();

    virtual void resetEvents();

    inline double getEventDelay(uint event)
    {
        return getEventDelayPtr(&modelData, event);
    }

    inline double getEventPriority(uint event)
    {
        return getEventPriorityPtr(&modelData, event);
    }

    inline bool getEventTrigger(uint event)
    {
        return getEventTriggerPtr(&modelData, event);
    }

    inline bool getEventUseValuesFromTriggerTime(uint event)
    {
        assert(event < symbols->getEventAttributes().size()
            && "event out of bounds");
        return symbols->getEventAttributes()[event]
                & EventUseValuesFromTriggerTime;
    }

    inline bool getEventInitialValue(uint event)
    {
        assert(event < symbols->getEventAttributes().size()
            && "event out of bounds");
        return symbols->getEventAttributes()[event] & EventInitialValue;
    }

    inline bool getEventPersistent(uint event)
    {
        assert(event < symbols->getEventAttributes().size()
            && "event out of bounds");
        return symbols->getEventAttributes()[event] & EventPersistent;
    }

    inline double getEventAssignTime(uint event)
    {
        assert(event < symbols->getEventAttributes().size()
                    && "event out of bounds");
        return eventAssignTimes[event];
    }

private:

    /**
     * previous state
     * get current state
     * current state becomes previous state for next itteration
     * evaluate first pending event
     */
    bool applyEvents(unsigned char* prevEventState,
            unsigned char* currEventState);


    EventQueue pendingEvents;

    /**
     * the time delayed events were triggered.
     */
    std::vector<double> eventAssignTimes;

    /******************************* Events Section *******************************/
    #endif /***********************************************************************/
    /******************************************************************************/

private:
    LLVMModelData modelData;
    LLVMModelData modelDataCopy;
    LLVMModelDataSymbols *symbols;
    llvm::LLVMContext *context;
    llvm::ExecutionEngine *executionEngine;
    std::string *errStr;
    int stackDepth;


    EvalInitialConditionsCodeGen::FunctionPtr evalInitialConditionsPtr;
    EvalReactionRatesCodeGen::FunctionPtr evalReactionRatesPtr;
    GetBoundarySpeciesAmountCodeGen::FunctionPtr getBoundarySpeciesAmountPtr;
    GetFloatingSpeciesAmountCodeGen::FunctionPtr getFloatingSpeciesAmountPtr;
    GetBoundarySpeciesConcentrationCodeGen::FunctionPtr getBoundarySpeciesConcentrationPtr;
    GetFloatingSpeciesConcentrationCodeGen::FunctionPtr getFloatingSpeciesConcentrationPtr;
    GetCompartmentVolumeCodeGen::FunctionPtr getCompartmentVolumePtr;
    GetGlobalParameterCodeGen::FunctionPtr getGlobalParameterPtr;
    EvalRateRuleRatesCodeGen::FunctionPtr evalRateRuleRatesPtr;
    GetEventTriggerCodeGen::FunctionPtr getEventTriggerPtr;
    GetEventPriorityCodeGen::FunctionPtr getEventPriorityPtr;
    GetEventDelayCodeGen::FunctionPtr getEventDelayPtr;
    EventTriggerCodeGen::FunctionPtr eventTriggerPtr;
    EventAssignCodeGen::FunctionPtr eventAssignPtr;

    double getFloatingSpeciesConcentration(int index);

    static LLVMExecutableModel* dummy();

    friend class LLVMModelGenerator;
};

} /* namespace rr */
#endif /* LLVMExecutableModelH */
