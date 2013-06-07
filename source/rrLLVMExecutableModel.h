/*
 * rrLLVMExecutableModel.h
 *
 *  Created on: Jun 3, 2013
 *      Author: Andy Somogyi, andy.somogyi@gmail.com
 */

#ifndef RRLLVMEXECUTABLEMODEL_H_
#define RRLLVMEXECUTABLEMODEL_H_

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
    virtual void assignCVodeInterface(CvodeInterface* cvodeI);
    virtual void setTime(double _time);
    virtual double getTime();
    virtual ModelData& getModelData();

    virtual CvodeInterface* getCvodeInterface();

    // functions --------------------------------------------------------
    virtual int getNumIndependentVariables();
    virtual int getNumDependentVariables();
    virtual int getNumTotalVariables();
    virtual int getNumBoundarySpecies();
    virtual int getNumGlobalParameters();
    virtual int getNumCompartments();
    virtual int getNumReactions();
    virtual int getNumRules();
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

    virtual const SymbolList &getReactions();
    virtual const SymbolList &getGlobalParameters();
    virtual const SymbolList &getBoundarySpecies();
    virtual const SymbolList &getCompartments();
    virtual const SymbolList &getConservations();
    virtual const SymbolList &getFloatingSpeciesAmounts();
    virtual const SymbolList &getFloatingSpeciesConcentrations();

    virtual const StringList getCompartmentNames();
    virtual const StringList getConservationNames();
    virtual const StringList getGlobalParameterNames();
    virtual const StringList getReactionNames();
    virtual const StringList getFloatingSpeciesConcentrationNames(); //Just returns the Ids...!
    virtual const StringList getBoundarySpeciesNames();
};

} /* namespace rr */
#endif /* RRLLVMEXECUTABLEMODEL_H_ */
