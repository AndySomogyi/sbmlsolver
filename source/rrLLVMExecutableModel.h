/*
 * rrLLVMExecutableModel.h
 *
 *  Created on: Jun 3, 2013
 *
 * Author: Andy Somogyi,
 *     email decode: V1 = "."; V2 = "@"; V3 = V1;
 *     andy V1 somogyi V2 gmail V3 com
 */

#ifndef rrLLVMExecutableModelH
#define rrLLVMExecutableModelH

#include "rrExecutableModel.h"
#include "rrLLVMModelDataSymbols.h"
#include "rrLLVMIncludes.h"

#include "rrLLVMEvalInitialConditionsCodeGen.h"
#include "rrLLVMEvalReactionRatesCodeGen.h"

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
    virtual ModelData& getModelData();

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

    /**
     * A ExecutableModel holds a stack of states, the entire state of this
     * model is pushed onto the saved state stack, and the current state
     * remains unchanged.
     *
     * @returns the size of the saved stack after the current state has been
     * pushed.
     */
    virtual int pushState(unsigned);

    /**
     * restore the state from a previously saved state, if the state stack
     * is empty, this has no effect.
     *
     * @returns the size of the saved stack after the top has been poped.
     */
    virtual int popState(unsigned);


    // functions --------------------------------------------------------
    virtual int getNumIndependentSpecies();
    virtual int getNumDependentSpecies();
    virtual int getNumFloatingSpecies();
    virtual int getNumBoundarySpecies();
    virtual int getNumGlobalParameters();
    virtual double getGlobalParameterValue(int index);
    virtual void setGlobalParameterValue(int index, double value);


    virtual int getNumCompartments();
    virtual int getNumReactions();
    virtual int getNumEvents();
    virtual void computeEventPriorites();
    virtual void setConcentration(int index, double value);
    virtual void evalReactionRates(double time, double* y);


    virtual int getNumLocalParameters(int reactionId);

    virtual void computeRules();

    virtual void initializeInitialConditions();


    virtual void setInitialConditions();
    virtual void evalInitialAssignments();

    virtual void convertToAmounts();
    virtual void computeConservedTotals();
    virtual double getConcentration(int index);

    //Access dll data
    virtual void getRateRuleValues(double *rateRuleValues);
    virtual double getAmounts(const int& i);



    virtual void setRateRuleValues(const double *rateRuleValues);

    virtual void convertToConcentrations();
    virtual void updateDependentSpeciesValues(double* _y);
    virtual void computeAllRatesOfChange();

    /**
     * where most of the juicy bits occur.
     */
    virtual void evalModel(double time, const double *y);

    virtual void evalEvents(const double& time, const vector<double>& y);

    virtual void resetEvents();
    virtual void testConstraints();

    virtual string getInfo();

    virtual int getFloatingSpeciesIndex(const string&);
    virtual string getFloatingSpeciesName(int);
    virtual int getBoundarySpeciesIndex(const string&);
    virtual string getBoundarySpeciesName(int);
    virtual int getBoundarySpeciesCompartmentIndex(int);
    virtual int getGlobalParameterIndex(const string&);
    virtual string getGlobalParameterName(int);
    virtual int getCompartmentIndex(const string&);
    virtual string getCompartmentName(int);
    virtual int getReactionIndex(const string&);
    virtual string getReactionName(int);


    virtual const SymbolList &getConservations();

    virtual const StringList getConservationNames();

    /**
     * using the current model state, evaluate and store all the reaction rates.
     */
    void evalReactionRates();

    static LLVMExecutableModel* dummy();

private:
    ModelData *modelData;
    LLVMModelDataSymbols *symbols;
    llvm::LLVMContext *context;
    llvm::ExecutionEngine *executionEngine;
    std::string *errStr;


    LLVMEvalInitialConditionsCodeGen::FunctionPtr evalInitialConditionsPtr;
    LLVMEvalReactionRatesCodeGen::FunctionPtr evalReactionRatesPtr;

    friend class LLVMModelGenerator;
};

} /* namespace rr */
#endif /* rrLLVMExecutableModelH */
