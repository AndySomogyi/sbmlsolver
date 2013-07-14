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

#include <rrExecutableModel.h>

namespace rr
{

class LLVMExecutableModel: public rr::ExecutableModel
{
public:
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
    virtual void computeReactionRates(double time, double* y);

    virtual void setCompartmentVolumes();
    virtual int getNumLocalParameters(int reactionId);
    virtual void computeRules(vector<double>& _y);
    virtual void computeRules(double* ay, int size);
    virtual void initializeInitialConditions();
    virtual void setParameterValues();
    virtual void setBoundaryConditions();
    virtual void setInitialConditions();
    virtual void evalInitialAssignments();

    virtual void convertToAmounts();
    virtual void computeConservedTotals();
    virtual double getConcentration(int index);

    //Access dll data
    virtual void getRateRuleValues(double *rateRuleValues);
    virtual double getAmounts(const int& i);
    virtual void initializeRates();

    virtual void setRateRuleValues(const double *rateRuleValues);

    virtual void convertToConcentrations();
    virtual void updateDependentSpeciesValues(double* _y);
    virtual void computeAllRatesOfChange();
    virtual void evalModel(double time, const double *y);
    virtual void evalEvents(const double& time, const vector<double>& y);
    virtual void resetEvents();
    virtual void testConstraints();
    virtual void initializeRateRuleSymbols();
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

    static LLVMExecutableModel* dummy();

};

} /* namespace rr */
#endif /* rrLLVMExecutableModelH */
