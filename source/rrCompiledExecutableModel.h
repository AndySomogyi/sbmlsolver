#ifndef rrCompiledExecutableModel_H_
#define rrCompiledExecutableModel_H_
#include <list>
#include "rrExecutableModel.h"
#include "rrModelSharedLibrary.h"
#include "rrCModelGenerator.h"
#include "rrModelData.h"
#include "rrNOMSupport.h"
#include "rr-libstruct/lsLibStructural.h"
#include <stack>

namespace rr
{
using namespace ls;
using Poco::SharedLibrary;
class CGenerator;
class CompiledModelState;

//Function pointer typedefs..
typedef void     (rrCallConv *c_void_MDS)(ModelData*); //MDS stands for ModelDataStructure
typedef int      (rrCallConv *c_int_MDS)(ModelData*);
typedef int      (rrCallConv *c_int_MDS_int)(ModelData*, int);
typedef char*    (rrCallConv *c_charStar_MDS)(ModelData*);
typedef void     (rrCallConv *c_void_MDS_doubleStar)(ModelData*, const double*);
typedef double   (rrCallConv *c_double_MDS_int)(ModelData*, int);
typedef double*  (rrCallConv *c_doubleStar_MDS)(ModelData*);
typedef void     (rrCallConv *c_void_MDS_double_doubleStar)(ModelData*, double, const double*);
typedef void     (rrCallConv *c_void_MDS_int_double)(ModelData*, int, double);

typedef ComputeEventAssignmentHandler* (rrCallConv *c_ComputeEventAssignmentHandlerStar)();
typedef EventDelayHandler* (rrCallConv *c_GetEventDelayHandlerStar)();

/**
 * Both the CModelGenerator and the CSharpModelGenerator use the same
 * paradigm of producing source code, calling a external compiler and
 * loadig the resulting shared library. This class implements the
 * ExecutableModel interface using this paradigm.
 */
class RR_DECLSPEC CompiledExecutableModel: public ExecutableModel
{

public:
    /**
     * makes a copy of the ModelSymbols and keeps it.
     * takes ownership of the shared lib.
     */
    CompiledExecutableModel(const ModelSymbols& symbols, ModelSharedLibrary* dll);
    virtual ~CompiledExecutableModel();

    virtual bool getConservedSumChanged();
    virtual void setConservedSumChanged(bool);

    virtual string getModelName();

    virtual void setTime(double _time);
    virtual double getTime();
    virtual ModelData& getModelData();

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
    virtual int pushState(unsigned options);

    /**
     * restore the state from a previously saved state, if the state stack
     * is empty, this has no effect.
     *
     * @returns the size of the saved stack after the top has been poped.
     */
    virtual int popState(unsigned options);

    // functions --------------------------------------------------------
    virtual int getNumIndependentSpecies();
    virtual int getNumDependentSpecies();

    virtual int getNumFloatingSpecies();
    virtual int getFloatingSpeciesIndex(const string& name);
    virtual string getFloatingSpeciesName(int index);

    virtual int getNumBoundarySpecies();
    virtual int getBoundarySpeciesIndex(const string &name);
    virtual string getBoundarySpeciesName(int index);
    virtual int getBoundarySpeciesCompartmentIndex(int index);

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
     * set the boundary species concentrations
     *
     * @param[in] len the length of the indx and values arrays.
     * @param[in] indx an array of length len of boundary species to return.
     * @param[in] values an array of at least length len which will store the
     *                returned boundary species amounts.
     */
    virtual int setBoundarySpeciesConcentrations(int len, int const *indx,
            double const *values);

    virtual int getNumGlobalParameters();
    virtual int getGlobalParameterIndex(const std::string& name);
    virtual string getGlobalParameterName(int index);
    virtual double getGlobalParameterValue(int index);
    virtual void setGlobalParameterValue(int index, double value);

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

    virtual int getNumCompartments();
    virtual int getCompartmentIndex(const string& name);
    virtual string getCompartmentName(int index);

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

    /**
     * get the number of reactions the model has
     */
    virtual int getNumReactions();

    /**
     * get the index of a named reaction
     * @returns >= 0 on success, < 0 on failure.
     */
    virtual int getReactionIndex(const std::string& reactionName);

    /**
     * get the name of the specified reaction
     */
    virtual string getReactionName(int index);

    virtual int getNumEvents();
    virtual void computeEventPriorites();
    void setConcentration(int index, double value);
    virtual void evalReactionRates();
    virtual void setCompartmentVolumes();
    virtual int getNumLocalParameters(int reactionId);

    virtual void computeRules();

    virtual void initializeInitialConditions();
    virtual void setParameterValues();
    virtual void setBoundaryConditions();
    virtual void setInitialConditions();
    virtual void evalInitialAssignments();

    virtual void convertToAmounts();
    virtual void computeConservedTotals();


    //Access dll data
    virtual void getRateRuleValues(double *rateRuleValues);

    double getAmount(const int i);
    virtual void initializeRates();

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
    virtual void evalModel(double time, const double *y, double *dydt = 0);

    virtual void evalEvents(const double time, const double *y);
    virtual void resetEvents();
    virtual void testConstraints();
    virtual void initializeRateRuleSymbols();
    virtual string getInfo();

    virtual void print(std::ostream &stream);

    /**
     * get the event time delays
     *
     * @param[in] len the length of the indx and values arrays.
     * @param[in] indx an array of length len of event indices
     * @param[out] values an array of at least length len which will store the
     *                event delays.
     */
    virtual uint getEventDelays(uint len, uint const *indx, double *values);

    virtual uint getEventPriorities(uint len, uint const *indx, double *values);

    virtual void eventAssignment(uint eventId);

    virtual double* evalEventAssignment(uint eventId);

    virtual void applyEventAssignment(uint eventId, double *values);

    virtual const SymbolList& getConservations();
    virtual const StringList getConservationNames();

private:
    double getFloatingSpeciesConcentration(int index);

    bool mConservedSumChanged;

    /**
     * initialize and allocate space for the ModelData buffers.
     */
    bool setupModelData();

    /**
     * setup the function pointer variables to point to the C functions
     * in the loaded shared library.
     */
    virtual bool setupDLLFunctions();

    //This structure holds data generated/used in the shared model lib..
    //some of it could be made global in the dll later on, like modelName..
    int mDummyInt;
    int mDummyDouble;
    double* mDummyDoubleArray;

    /**
     * the data that is exchanged with the loaded shared lib,
     * and all sorts of other routines such as CVODE.
     */
    ModelData                         mData;
    ModelSymbols                      ms;

    /**
     * If all functions are found properly in the dll, this one is true
     */
    bool                              mIsInitialized;
    ModelSharedLibrary*               mDLL;

    std::stack<CompiledModelState*>        modelStates;

    //Function pointers...
    c_int_MDS                         cInitModel;
    c_int_MDS                         cInitModelData;
    c_charStar_MDS                    cgetModelName;
    c_void_MDS                        cinitializeInitialConditions;
    c_void_MDS                        csetParameterValues;
    c_void_MDS                        csetCompartmentVolumes;
    c_int_MDS_int                     cgetNumLocalParameters;
    c_void_MDS                        csetBoundaryConditions;
    c_void_MDS                        csetInitialConditions;
    c_void_MDS                        cevalInitialAssignments;
    c_void_MDS                        cupdateDependentSpeciesValues;
    c_void_MDS                        ccomputeRules;
    c_void_MDS                        cconvertToAmounts;
    c_void_MDS                        ccomputeConservedTotals;
    c_double_MDS_int                  cgetConcentration;
    c_doubleStar_MDS                  cGetCurrentValues;
    c_void_MDS_double_doubleStar      cevalModel;
    c_void_MDS                        cconvertToConcentrations;
    c_void_MDS_double_doubleStar      cevalEvents;
    c_void_MDS                        ccomputeAllRatesOfChange;
    c_void_MDS                        cAssignRates_a;
    c_void_MDS_doubleStar             cAssignRates_b;
    c_void_MDS                        ctestConstraints;
    c_void_MDS                        cresetEvents;
    c_void_MDS                        cInitializeRates;
    c_void_MDS                        cInitializeRateRuleSymbols;
    c_void_MDS_int_double             csetConcentration;
    c_void_MDS                        cComputeReactionRates;
    c_void_MDS                        ccomputeEventPriorities;
};
}
#endif
