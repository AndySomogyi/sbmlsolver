#ifndef rrRoadRunnerH
#define rrRoadRunnerH

#include "rr-libstruct/lsMatrix.h"
#include "rrVariableType.h"
#include "rrParameterType.h"
#include "rrSelectionRecord.h"
#include "rrRoadRunnerData.h"
#include "rrConstants.h"
#include "rrRoadRunnerOptions.h"
#include "Configurable.h"

#include <string>
#include <vector>
#include <list>

namespace ls
{
class LibStructural;
}

namespace rr
{

class ModelGenerator;
class SBMLModelSimulation;
class ExecutableModel;
class Integrator;

/**
 * The main RoadRunner class.
 *
 * The RoadRunner class is responsible for loading and simulating SBML models.
 *
 * MemoryManagment: Any pointer returned by a get... method is owned by the
 * RoadRunner object and does NOT have to be deleted.
 */
class RR_DECLSPEC RoadRunner : public Configurable
{

public:

    /**
     * All three of the RoadRunner options default to the empty string, in this
     * case, the default values are used.
     *
     * @param compiler: If LLVM build is enabled, the compiler defaults to LLVM.
     * @param tempDir: If the old external C compiler is used, this is the
     *                    where the C files are written to.
     * @param supportCodeDir: If the old external C compiler is used, this is
     *      the location where roadrunner C include files are.
     */
    RoadRunner(const std::string& compiler = "", const std::string& tempDir = "",
            const std::string& supportCodeDir = "");

    /**
     * free any memory this class allocated
     */
    virtual ~RoadRunner();

    /**
     * When there are multiple instances of RoadRunner, this is the instance id.
     */
    int getInstanceID();

    /**
     * Number of currently running RoadRunner instances.
     */
    int getInstanceCount();

    /**
     * given an sbml document, this method moves all the local parameters
     * to global parameters.
     */
    static std::string getParamPromotedSBML(const std::string& sArg);

    /**
     * information about the current state of this object.
     */
    std::string getInfo();

    /**
     * The Compiler that the ModelGenerator is using to compile / interpret sbml code.
     */
    class Compiler* getCompiler();

    /**
     * Set the name of the externa compiler to use. Some ModelGenerators may have no use
     * for this value.
     */
    bool setCompiler(const std::string& compiler);

    /**
     * get a pointer to the integrator which is currently being used to
     * time evolve the system.
     */
    Integrator* getIntegrator();

    bool isModelLoaded();

    /**
     * returns the model name if a model is loaded, empty string otherwise.
     */
    std::string getModelName();

    bool unLoadModel();

    /**
     * @internal
     * @deprecated
     *
     * C backend only
     * set the location where the ModelGenerator creates temporary files, such
     * as shared libraries.
     */
    bool setTempFileFolder(const std::string& folder);

    /**
     * @internal
     * @deprecated
     *
     * legacy C backend only
     * get the ModelGenerator's temporary file directory.
     */
    std::string getTempFolder();

    /**
     * Carry out a single integration step using a stepsize as indicated
     * in the method call. Arguments: double CurrentTime, double StepSize,
     * bool: reset integrator if true, Return Value: new CurrentTime.
     */
    double oneStep(double currentTime, double stepSize, bool reset = true);

    /**
     * simulate the current SBML model.
     *
     * If options is null, then the current simulation settings (start time,
     * end time, n steps) are used. If options is not null, then the
     * current simulation settings are set to the values specified by
     * options and they are used.
     *
     * @returns a RoadRunnerData object which is owned by the the RoadRunner
     * object if successfull, 0 on failure.
     */
    const RoadRunnerData *simulate(const SimulateOptions* options = 0);

    /**
     * obtain a pointer to the simulation result.
     *
     * This is owned by the RoadRunner object.
     */
    RoadRunnerData *getSimulationResult();

    void setSimulateOptions(const SimulateOptions& settings);

    /**
     * get a reference to the SimulateOptions that were set either
     * by setSimulateOptions or simulate.
     */
    SimulateOptions& getSimulateOptions();

    /**
     * Get a reference to the options that determine how this class should behave.
     *
     * These are general options. For options specific for loading or simulationg,
     * @see getSimulateOptions.
     */
    RoadRunnerOptions& getOptions();

    /**
     * get the currently loaded sbml document as a string.
     */
    std::string getSBML();

    /**
     * Reset the simulator back to the initial conditions specified in the SBML model,
     * provided an SBML model is loaded.
     */
    void reset();

    /**
     * @internal
     * set the floating species initial concentrations.
     *
     * equivalent to ExecutableModel::reset, then ExecutableModel::setFloatingSpeciesConcentrations
     *
     * @deprecated
     */
    void changeInitialConditions(const std::vector<double>& ic);

    /**
     * get the ModelGenerator that is used to create executable (runnable) models.
     */
    ModelGenerator* getModelGenerator();

    /**
     * get a pointer to the ExecutableModel owned by the RoadRunner object.
     */
    ExecutableModel* getModel();

    /**
     * load an sbml document from anywhere.
     *
     * If options is not null, then the RoadRunner::computeAndAssignConservationLaws
     * flag is set to whatever value is specified in the options struct.
     *
     * @param uriOrSBML: a URI, local path or sbml document contents.
     * @param options: an options struct, if null, default values are used.
     */
    bool load(const std::string& uriOrSBML,
            const LoadSBMLOptions* options = 0);

    /**
     * @internal
     * @deprecated, use ExecutableModel::getReactionRates
     */
    std::vector<double> getReactionRates();

    /**
     * @internal
     * @deprecated, use ExecutableModel::getFloatingSpeciesAmountRates
     */
    std::vector<double> getRatesOfChange();

    /**
     * @internal
     * @deprecated
     * returns a list of reaction ids obtained from
     * ExecutableModel::getReactionId
     */
    std::vector<std::string> getReactionIds();

    /**
     * creates a new xml element that represent the current state of this
     * Configurable object and all if its child objects.
     *
     * This node needs to be consumed by Configurable::xmlFromConfigNode
     */
    virtual _xmlNode *createConfigNode();

    /**
     * Given an xml element, the Configurable object should pick its needed
     * values that are stored in the element and use them to set its
     * internal configuration state.
     */
    virtual void loadConfig(const _xmlDoc* doc);

    /**
     * recurse through all of the child configurable objects that this
     * class ownes and build an assemble all of thier configuration parameters
     * into a single xml document which is returned as a string.
     *
     * The value of this result depends on what child objects are presently loaded.
     */
    std::string getConfigurationXML();

    /**
     * given a xml document, which should have been returned from getConfigurationXML,
     * this method recurses though all the child configurable elements and sets thier
     * configuration to the values specified in the document.
     */
    void setConfigurationXML(const std::string& xml);


    /**
     * @internal
     * @deprecated
     */
    void correctMaxStep();

/************************ Selection Ids Species Section ***********************/
#if (1) /**********************************************************************/
/******************************************************************************/

    /**
     * create a selection record. This record can be used to select values.
     */
    rr::SelectionRecord createSelection(const std::string& str);

    /**
     * Returns the currently selected columns that will be returned by
     * calls to simulate() or simulateEx(,,).
     */
    std::vector<rr::SelectionRecord>& getSelections();

    /**
     * Creates a new selection based on the selection string,
     * and returns the value it queries.
     */
    double getValue(const std::string& sel);

    double getValue(const SelectionRecord& record);


    void setSelections(const std::vector<std::string>& selections);

    void setSelections(const std::vector<rr::SelectionRecord>& selections);


    /**
     * returns the values selected with SimulateOptions for the current model time / timestep")
     */
    std::vector<double> getSelectedValues();

    /**
     * populates a given list with all the ids that this class can accept.
     */
    void getIds(int types, std::list<std::string> &ids);

    /**
     * returns a bit field of the ids that this class supports.
     */
    int getSupportedIdTypes();


    /**
     * sets the value coresponding to the given selection string
     */
    bool setValue(const std::string& id, double value);

/************************ End Selection Ids Species Section *******************/
#endif /***********************************************************************/
/******************************************************************************/

    /**
     * compute the full Jacobian at the current operating point
     */
    ls::DoubleMatrix getFullJacobian();

    ls::DoubleMatrix getFullReorderedJacobian();

    /**
     * Compute the reduced Jacobian at the current operating point.
     */
    ls::DoubleMatrix getReducedJacobian();

    /**
     * Returns eigenvalues, first column real part, second column imaginary part
     */
    ls::DoubleMatrix getEigenvalues();

    std::vector<Complex> getEigenvaluesCpx();

    ls::DoubleMatrix* getLinkMatrix();
    ls::DoubleMatrix* getNrMatrix();
    ls::DoubleMatrix* getL0Matrix();


    ls::DoubleMatrix getConservationMatrix();
    ls::DoubleMatrix getUnscaledConcentrationControlCoefficientMatrix();
    ls::DoubleMatrix getScaledConcentrationControlCoefficientMatrix();
    ls::DoubleMatrix getUnscaledFluxControlCoefficientMatrix();
    ls::DoubleMatrix getScaledFluxControlCoefficientMatrix();
    int getNumberOfDependentSpecies();
    int getNumberOfIndependentSpecies();


    /**
     * returns the list of floating species, but with a "eigen(...)" string
     * wrapped around them.
     */
    std::vector<std::string> getEigenvalueIds();

    /**
     * Returns the unscaled elasticity for a named reaction with respect to a
     * named parameter
     */
    double getUnscaledParameterElasticity(const string& reactionName,
            const string& parameterName);


    Matrix<double> getFrequencyResponse(double startFrequency,
            int numberOfDecades, int numberOfPoints,
            const string& parameterName, const string& variableName,
            bool useDB, bool useHz);

    /**
     * This method turns on / off the computation and adherence to conservation laws.
     */
    void setConservedMoietyAnalysis(bool value);

    /**
     * is conservation analysis enabled. This is set
     */
    bool getConservedMoietyAnalysis();

    /**
     * Returns the SBML with the current parameterset.
     */
    std::string getCurrentSBML();

    /**
     * @internal
     * @deprecated
     */
    int getNumberOfReactions();

    /**
     * @internal
     * @deprecated
     */
    double getReactionRate(const int& index);

    /*! \cond PRIVATE */


    /**
     * @deprecated
     * @internal
     * Returns the rate of changes of a species by its index
     */
    double getRateOfChange(const int& index);

    /*! \endcond */


    std::vector<std::string> getRateOfChangeIds();


    std::vector<std::string> getConservedMoietyIds();

    std::vector<double> getConservedMoietyValues();

    int getNumberOfCompartments();

    /**
     * @internal
     * @deprecated
     * Sets the value of a compartment by its index
     */
    void setCompartmentByIndex(const int& index, const double& value);

    /**
     * @internal
     * @deprecated, use ExecutableModel::getCompartmentVolumes
     * Returns the value of a compartment by its index
     */
    double getCompartmentByIndex(const int& index);

    /**
     * @internal
     * @deprecated, use ExecutableModel::getCompartmentId
     */
    std::vector<std::string> getCompartmentIds();

    /**
     * @internal
     * Get the number of boundary species,
     * @deprecated, use ExecutableModel::getNumBoundarySpecies
     */
    int getNumberOfBoundarySpecies();

    /**
     * @internal
     * @deprecated, use ExecutableModel::setBoundarySpeciesConcentrations
     */
    void setBoundarySpeciesByIndex(const int& index, const double& value);

    /**
     * @internal
     * @deprecated, use ExecutableModel::getBoundarySpeciesConcentrations
     */
    double getBoundarySpeciesByIndex(const int& index);

    /**
     * @internal
     * @deprecated, use ExecutableModel::setBoundarySpeciesConcentrations
     */
    std::vector<double> getBoundarySpeciesConcentrations();

    /**
     * @internal
     * @deprecated, use ExecutableModel::setBoundarySpeciesConcentrations
     */
    void setBoundarySpeciesConcentrations(const std::vector<double>& values);

    /**
     * @internal
     * @deprecated, use ExecutableModel::getBoundarySpeciesId
     */
    std::vector<std::string> getBoundarySpeciesIds();

    /**
     * @internal
     * @deprecated, use ExecutableModel::getNumFloatingSpecies
     */
    int getNumberOfFloatingSpecies();

    /**
     * @internal
     * @deprecated
     * get / set conc.
     */
    double getFloatingSpeciesByIndex(int index);

    /**
     * @internal
     * @deprecated
     */
    void setFloatingSpeciesByIndex(int index, double value);

    /**
     * @internal
     * @deprecated
     */
    std::vector<double> getFloatingSpeciesConcentrations();

    /**
     * @internal
     * @deprecated
     */
    std::vector<double> getFloatingSpeciesInitialConcentrations();

    /**
     * @internal
     * @deprecated
     */
    void setFloatingSpeciesConcentrations(const std::vector<double>& values);

    /**
     * @internal
     * @deprecated
     */
    void setFloatingSpeciesInitialConcentrationByIndex(const int& index,
            const double& value);

    /**
     * @internal
     * @deprecated
     */
    void setFloatingSpeciesInitialConcentrations(const std::vector<double>& values);

    /**
     * @internal
     * @deprecated
     */
    std::vector<std::string> getFloatingSpeciesIds();

    /**
     * @internal
     * @deprecated
     */
    std::vector<std::string> getFloatingSpeciesInitialConditionIds();

    /**
     * @internal
     * @deprecated use ExecutableModel::getNumGlobalParameters
     */
    int getNumberOfGlobalParameters();

    /**
     * @internal
     * @deprecated use ExecutableModel::setGlobalParameterValues
     */
    void setGlobalParameterByIndex(const int index, const double value);

    /**
     * @internal
     * @deprecated use ExecutableModel::getGlobalParameterValues
     */
    double getGlobalParameterByIndex(const int& index);

    /**
     * @internal
     * @deprecated use ExecutableModel::getGlobalParameterValues
     */
    std::vector<double> getGlobalParameterValues();

    /**
     * @internal
     * @deprecated use ExecutableModel::getGlobalParameterIds
     */
    std::vector<std::string> getGlobalParameterIds();

    /**
     * The C back end requires this to be called to update
     * model variables if anyting is changes. Does nothing
     * in LLVM back end as everything is automatically handled
     * with lazy evaluation.
     */
    void evalModel();

    /**
     * getVersion plus info about dependent libs versions..
     */
    static std::string getExtendedVersionInfo();


    /**
     * Get unscaled control coefficient with respect to a global parameter
     *
     * variableName must be either a reaction or floating species.
     *
     * parameterName must be eithe a global parameter, boundary species, or
     * conserved sum.
     */
    double getuCC(const std::string& variableName, const std::string& parameterName);

    /**
     * Get scaled control coefficient with respect to a global parameter
     *
     * The variableName must be either a reaction id, or a floating species id.
     *
     * The parameterName must be either a global parameter, boundary species,
     * or conserved sum.
     */
    double getCC(const std::string& variableName, const std::string& parameterName);

    /**
     * Get unscaled elasticity coefficient with respect to a global parameter or species
     */
    double getuEE(const std::string& reactionName, const std::string& parameterName);

    /**
     * Get unscaled elasticity coefficient with respect to a global parameter or species.
     * Optionally the model is brought to steady state after the computation.
     */
    double getuEE(const std::string& reactionName, const std::string& parameterName,
            bool computeSteadystate);

    /**
     * Get scaled elasticity coefficient with respect to a global parameter or species
     */
    double getEE(const std::string& reactionName, const std::string& parameterName);

    /**
     * Get scaled elasticity coefficient with respect to a global parameter or species.
     * Optionally the model is brought to steady state after the computation.
     */
    double getEE(const std::string& reactionName, const std::string& parameterName,
            bool computeSteadyState);

    /**
     * Compute the unscaled species elasticity matrix at the current operating point
     */
    ls::DoubleMatrix getUnscaledElasticityMatrix();

    /**
     * Compute the unscaled elasticity matrix at the current operating point
     */
    ls::DoubleMatrix getScaledElasticityMatrix();

    /**
     * Compute the scaled elasticity for a given reaction and given species
     */
    double getScaledFloatingSpeciesElasticity(const std::string& reactionName,
            const std::string& speciesName);

    /**
     * Get a single species elasticity value
     * IMPORTANT:
     * Assumes that the reaction rates have been precomputed at the operating point !!
     */
    double getUnscaledSpeciesElasticity(int reactionId, int speciesIndex);


    /******************************* Steady State Section *************************/
    #if (1) /**********************************************************************/
    /******************************************************************************/

    /**
     * Compute the steady state of the model, returns the sum of squares of the
     * solution
     */
    double steadyState();

    /**
     * returns the current set of steady state selections.
     */
    std::vector<rr::SelectionRecord>& getSteadyStateSelections();

    /**
     * parses the given list of strings and generates selections records
     * which will be used for the steady state selections.
     */
    void setSteadyStateSelections(const std::vector<std::string>&
            steadyStateSelections);

    /**
     * makes a copy of an existing list of selection records. These will be
     * saved and used for selection values in getSteadyStateValues().
     */
    void setSteadyStateSelections(const std::vector<rr::SelectionRecord>&
            steadyStateSelections);

    /**
     * Performs a steady state calculation (evolves the system to a steady
     * state), then calculates and returns the set of values specifed by
     * the steady state selections.
     */
    std::vector<double> getSteadyStateValues();

    /******************************* End Steady State Section *********************/
    #endif /***********************************************************************/
    /******************************************************************************/

private:
    static int mInstanceCount;
    int mInstanceID;
    bool mUseKinsol;
    const double mDiffStepSize;

    const double mSteadyStateThreshold;
    ls::DoubleMatrix mRawRoadRunnerData;
    RoadRunnerData mRoadRunnerData;

    std::string mCurrentSBMLFileName;

    /**
     * The Cvode object get created just after a model is created, it then
     * gets a reference to the model and holds on to it.
     */
    class CvodeInterface *mCVode;
    std::vector<SelectionRecord> mSelectionList;

    /**
     * ModelGenerator obtained from the factory
     */
    ModelGenerator *mModelGenerator;

    /**
     * RoadRunner, not sbml parameters
     */
    bool mComputeAndAssignConservationLaws;

    std::vector<SelectionRecord> mSteadyStateSelection;

    ExecutableModel* mModel;

    std::string mCurrentSBML;

    /**
     * structural analysis library.
     */
    LibStructural* mLS;

    /**
     * options that are specific to the simulation
     */
    SimulateOptions simulateOptions;

    /**
     * various general options that can be modified by external callers.
     */
    RoadRunnerOptions options;


    int createDefaultSteadyStateSelectionList();
    int createDefaultTimeCourseSelectionList();

    void addNthOutputToResult(ls::DoubleMatrix& results, int nRow,
            double dCurrentTime);
    bool populateResult();


    double getNthSelectedOutput(const int& index, const double& dCurrentTime);

    double getVariableValue(const VariableType::VariableType variableType,
            const int variableIndex);



    std::string createModelName(const std::string& mCurrentSBMLFileName);

    /**
     * the LibStruct is normally null, only created on demand here.
     */
    LibStructural* getLibStruct();

    bool initializeModel();

    bool createDefaultSelectionLists();

    /**
     * creates a selection list from the amounts / conc / variables ivars of the
     * SimulationOptions struct.
     */
    int createTimeCourseSelectionList();

    /**
     * Set a sbml model variable to a value.
     *
     * @parameterType the type of sbml element
     */
    void setParameterValue(const ParameterType::ParameterType parameterType,
            const int parameterIndex, const double value);

    double getParameterValue(const ParameterType::ParameterType parameterType,
            const int parameterIndex);

    /**
     * Changes a given parameter type by the given increment
     */
    void changeParameter(ParameterType::ParameterType parameterType,
            int reactionIndex, int parameterIndex, double originalValue,
            double increment);


    std::vector<SelectionRecord> getSelectionList();

    /**
     * the xml string that is given in setConfigurationXML.
     *
     * Needed because the NLEQ is only created in the steadyState method.
     */
    std::string configurationXML;


    friend class aFinalizer;
};

}

#endif

/*! \mainpage RoadRunner C++ Library

 \par
 This document describes the application programming interface (API) of RoadRunner, an open source (BSD) library for computing structural characteristics of cellular networks.
 \par

 \par Dependencies
 The RoadRunner library depend on several third-party libraries, CLapack, libSBML (libxml2), Sundials, NLEQ, and Poco. These are provided with the binary installation where necessary.
 \par

 \author Andy Somogyi (andy.somogyi@gmail.com, somogyie@indiana.edu)
 \author Totte Karlsson (totte@dunescientific.com)
 \author Herbert M. Sauro  (hsauro@u.washington.edu)

 \par
 Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

 \li Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

 \li Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

 \li Neither the name of University of Washington nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

 \par

 */
