#ifndef rrCompiledExecutableModel_H_
#define rrCompiledExecutableModel_H_
#include <list>
#include "rrExecutableModel.h"
#include "rrModelSharedLibrary.h"
#include "rrCModelGenerator.h"
#include "rrModelData.h"
#include "rrNOMSupport.h"
#include "rr-libstruct/lsLibStructural.h"

namespace rr
{
using namespace ls;
using Poco::SharedLibrary;
class CGenerator;
class CvodeInterface;

typedef void    (rrCallConv *c_void_MDS)(ModelData*);//MDS stands for ModelDataStructure
typedef int     (rrCallConv *c_int_MDS)(ModelData*);
typedef int     (rrCallConv *c_int_MDS_int)(ModelData*, int);
typedef char*   (rrCallConv *c_charStar_MDS)(ModelData*);
typedef void    (rrCallConv *c_void_MDS_doubleStar)(ModelData*, double*);
typedef double  (rrCallConv *c_double_MDS_int)(ModelData*, int);
typedef double* (rrCallConv *c_doubleStar_MDS)(ModelData*);
typedef void    (rrCallConv *c_void_MDS_double_doubleStar)(ModelData*, double, double*);
typedef void    (rrCallConv *c_void_MDS_int_double)(ModelData*, int, double);


typedef TComputeEventAssignmentDelegate* (rrCallConv *c_TComputeEventAssignmentDelegateStar)();
typedef TEventDelayDelegate* (rrCallConv *c_GetEventDelayDelegatesStar)();

/**
 * Both the CModelGenerator and the CSharpModelGenerator use the same
 * paradigm of producing source code, calling a external compiler and
 * loadig the resulting shared library. This class implements the
 * ExecutableModel interface using this paradigm.
 */
class RR_DECLSPEC CompiledExecutableModel : public ExecutableModel, public rrObject
{

public:
    CompiledExecutableModel(CModelGenerator& generator, ModelSharedLibrary* dll);
    virtual                                        ~CompiledExecutableModel();

    virtual string                                  getModelName();
    virtual void                                    assignCVodeInterface(CvodeInterface* cvodeI);
    virtual void                                    setTime(double _time);
    virtual double                                  getTime();
    virtual ModelData&                              getModelData();
    virtual CvodeInterface*                         getCvodeInterface();

    // functions --------------------------------------------------------
    virtual int                                     getNumIndependentVariables();
    virtual int                                     getNumDependentVariables();
    virtual int                                     getNumTotalVariables();
    virtual int                                     getNumBoundarySpecies();
    virtual int                                     getNumGlobalParameters();
    virtual int                                     getNumCompartments();
    virtual int                                     getNumReactions();
    virtual int                                     getNumRules();
    virtual int                                     getNumEvents();
    virtual void                                    computeEventPriorites();
    virtual void                                    setConcentration(int index, double value);
    virtual void                                    computeReactionRates(double time, double* y);
    virtual void                                    setCompartmentVolumes();
    virtual int                                     getNumLocalParameters(int reactionId);
    virtual void                                    computeRules(vector<double>& _y);
    virtual void                                    computeRules(double* ay, int size);
    virtual void                                    initializeInitialConditions();
    virtual void                                    setParameterValues();
    virtual void                                    setBoundaryConditions();
    virtual void                                    setInitialConditions();
    virtual void                                    evalInitialAssignments();

    virtual void                                    convertToAmounts();
    virtual void                                    computeConservedTotals();
    virtual double                                  getConcentration(int index);

    //Access dll data
    virtual vector<double>                          getCurrentValues();
    virtual double                                  getAmounts(const int& i);
    virtual void                                    initializeRates();
    virtual void                                    assignRates();
    virtual void                                    assignRates(vector<double>& rates);
    virtual void                                    convertToConcentrations();
    virtual void                                    updateDependentSpeciesValues(double* _y);
    virtual void                                    computeAllRatesOfChange();
    virtual void                                    evalModel(const double& time,  const vector<double>& y);
    virtual void                                    evalEvents(const double& time, const vector<double>& y);
    virtual void                                    resetEvents();
    virtual void                                    testConstraints();
    virtual void                                    initializeRateRuleSymbols();
    virtual string                                  getInfo();

    virtual SymbolList&                             getReactions();
    virtual SymbolList&                             getGlobalParameters();
    virtual SymbolList&                             getBoundarySpecies();
    virtual SymbolList&                             getCompartments();
    virtual SymbolList&                             getConservations();
    virtual SymbolList&                             getFloatingSpeciesAmounts();
    virtual SymbolList&                             getFloatingSpeciesConcentrations();

    virtual StringList                              getCompartmentNames();
    virtual StringList                              getConservationNames();
    virtual StringList                              getGlobalParameterNames();
    virtual StringList                              getReactionNames();
    virtual StringList                              getFloatingSpeciesConcentrationNames();    //Just returns the Ids...!
    virtual StringList                              getBoundarySpeciesNames();

private:

    /**
     * initialize and allocate space for the ModelData buffers.
     */
    bool setupModelData();

    /**
     * setup the function pointer variables to point to the C functions
     * in the loaded shared library.
     */
    virtual bool 						setupDLLFunctions();

    //This structure holds data generated/used in the shared model lib..
    //some of it could be made global in the dll later on, like modelName..
    int 								mDummyInt;
    int									mDummyDouble;
    double* 							mDummyDoubleArray;

    /**
     * the data that is exchanged with the loaded shared lib,
     * and all sorts of other routines such as CVODE.
     */
    ModelData 							mData;
    CvodeInterface* 					mCvodeInterface;
    CModelGenerator& 					mCG;

    /**
     * Reference to libstruct library
     */
    LibStructural& 						mLibStruct;

    /**
     * Object that provide some wrappers and new "NOM" functions.
     */
    NOMSupport& 						mNOM;

    /**
     * If all functions are found properly in the dll, this one is true
     */
    bool mIsInitialized;

    ModelSharedLibrary* mDLL;

    //Function pointers...
    c_int_MDS                               cInitModel;
    c_int_MDS                               cInitModelData;
    c_charStar_MDS                          cgetModelName;
    c_void_MDS                              cinitializeInitialConditions;
    c_void_MDS                              csetParameterValues;
    c_void_MDS                              csetCompartmentVolumes;
    c_int_MDS_int                           cgetNumLocalParameters;
    c_void_MDS                              csetBoundaryConditions;
    c_void_MDS                              csetInitialConditions;
    c_void_MDS                              cevalInitialAssignments;
    c_void_MDS_doubleStar                   cupdateDependentSpeciesValues;
    c_void_MDS_doubleStar                   ccomputeRules;
    c_void_MDS                              cconvertToAmounts;
    c_void_MDS                              ccomputeConservedTotals;
    c_double_MDS_int                        cgetConcentration;
    c_doubleStar_MDS                        cGetCurrentValues;
    c_void_MDS_double_doubleStar            cevalModel;
    c_void_MDS                              cconvertToConcentrations;
    c_void_MDS_double_doubleStar            cevalEvents;
    c_void_MDS                              ccomputeAllRatesOfChange;
    c_void_MDS                              cAssignRates_a;
    c_void_MDS_doubleStar                   cAssignRates_b;
    c_void_MDS                              ctestConstraints;
    c_void_MDS                              cresetEvents;
    c_void_MDS                              cInitializeRates;
    c_void_MDS                              cInitializeRateRuleSymbols;
    c_void_MDS_int_double                   csetConcentration;
    c_void_MDS_double_doubleStar            cComputeReactionRates;
    c_void_MDS                              ccomputeEventPriorities;
};
}
#endif
