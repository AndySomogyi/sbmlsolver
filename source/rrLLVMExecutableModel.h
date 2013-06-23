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


    // functions --------------------------------------------------------
    virtual int getNumIndependentSpecies();
    virtual int getNumDependentSpecies();
    virtual int getNumFloatingSpecies();
    virtual int getNumBoundarySpecies();
    virtual int getNumGlobalParameters();
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
    virtual vector<double> getCurrentValues();
    virtual double getAmounts(const int& i);
    virtual void initializeRates();
    virtual void assignRates();
    virtual void assignRates(vector<double>& rates);
    virtual void convertToConcentrations();
    virtual void updateDependentSpeciesValues(double* _y);
    virtual void computeAllRatesOfChange();
    virtual void evalModel(const double& time, const vector<double>& y);
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
