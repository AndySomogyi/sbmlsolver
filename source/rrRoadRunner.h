#ifndef rrRoadRunnerH
#define rrRoadRunnerH

#include "rr-libstruct/lsMatrix.h"
#include "rrVariableType.h"
#include "rrParameterType.h"
#include "rrSelectionRecord.h"
#include "rrRoadRunnerData.h"
#include "rrConstants.h"
#include "rrNewArrayList.h"
#include "rrParameter.h"
#include "rrRoadRunnerOptions.h"
#include "Configurable.h"

#include <string>
#include <vector>

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

    static std::string getlibSBMLVersion();
    bool unLoadModel();



    /**
     * set the location where the ModelGenerator creates temporary files, such
     * as shared libraries.
     */
    bool setTempFileFolder(const std::string& folder);

    /**
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

    double getValueForRecord(const SelectionRecord& record);

    /**
     * obtain a pointer to the simulation result.
     *
     * This is owned by the RoadRunner object.
     */
    RoadRunnerData *getSimulationResult();

    bool setSimulateOptions(const SimulateOptions& settings);

    /**
     * get a reference to the SimulateOptions that were set either
     * by setSimulateOptions or simulate.
     */
    const SimulateOptions& getSimulateOptions() const;

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
     * load an sbml document from disk.
     *
     * If options is not null, then the RoadRunner::computeAndAssignConservationLaws
     * flag is set to whatever value is specified in the options struct.
     *
     * @param fileName: path to an sbml document.
     * @param options: an options struct, if null, default values are used.
     */
    bool loadSBMLFromFile(const std::string& fileName,
            const LoadSBMLOptions* options = 0);

    /**
     * load an sbml document from a string containting the contents of an sbml document.
     *
     * If options is not null, then the RoadRunner::computeAndAssignConservationLaws
     * flag is set to whatever value is specified in the options struct.
     *
     * @param fileName: path to an sbml document.
     * @param options: an options struct, if null, default values are used.
     */
    bool loadSBML(const std::string& sbml, const LoadSBMLOptions* options = 0);

    /**
     * @deprecated, use ExecutableModel::getReactionRates
     */
    std::vector<double> getReactionRates();

    /**
     * @deprecated, use ExecutableModel::getFloatingSpeciesAmountRates
     */
    std::vector<double> getRatesOfChange();

    /**
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


    void correctMaxStep();

    /**
     * sets the value of an sbml varaible to the given value.
     *
     * @param sId: an sbml id
     * @param value: the value to set it to.
     */
    bool setValue(const std::string& sId, double value);

    /**
     * gets the value of an sbml element based on its id.
     */
    double getValue(const std::string& sId);

    /**
     * @deprecated
     * @internal
     * @private
     */
    NewArrayList getAvailableTimeCourseSymbols();

    /**
     * Returns the currently selected columns that will be returned by
     * calls to simulate() or simulateEx(,,).
     */
    std::vector<std::string> getTimeCourseSelectionList();


    void setTimeCourseSelectionList(const std::string& List);

    void setTimeCourseSelectionList(
            const std::vector<std::string>& newSelectionList);

    /**
     * Compute the steady state of the model, returns the sum of squares of the solution
     */
    double steadyState();

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
    ls::DoubleMatrix getStoichiometryMatrix();
    ls::DoubleMatrix getReorderedStoichiometryMatrix();
    ls::DoubleMatrix getFullyReorderedStoichiometryMatrix();
    ls::DoubleMatrix getConservationMatrix();
    ls::DoubleMatrix getUnscaledConcentrationControlCoefficientMatrix();
    ls::DoubleMatrix getScaledConcentrationControlCoefficientMatrix();
    ls::DoubleMatrix getUnscaledFluxControlCoefficientMatrix();
    ls::DoubleMatrix getScaledFluxControlCoefficientMatrix();
    int getNumberOfDependentSpecies();
    int getNumberOfIndependentSpecies();


    /**
     * @deprecated
     * @internal
     * @private
     */
    NewArrayList getUnscaledFluxControlCoefficientIds();

    /**
     * @deprecated
     * @internal
     * @private
     */
    NewArrayList getFluxControlCoefficientIds();

    /**
     * @deprecated
     * @internal
     * @private
     */
    NewArrayList getUnscaledConcentrationControlCoefficientIds();

    /**
     * @deprecated
     * @internal
     * @private
     */
    NewArrayList getConcentrationControlCoefficientIds();

    /**
     * @deprecated
     * @internal
     * @private
     */
    NewArrayList getElasticityCoefficientIds();

    /**
     * @deprecated
     * @internal
     * @private
     */
    NewArrayList getUnscaledElasticityCoefficientIds();

    /**
     * returns a list of floating species ids with thier names
     * prefixed with "eigen_". For example, if the model contained
     * the floating species "S1" and "S2", this would return a list
     * containing ["eigen_S1", "eigen_S2"].
     */
    std::vector<std::string> getEigenvalueIds();

    /**
     * @deprecated
     * @private
     * @internal
     */
    NewArrayList getAvailableSteadyStateSymbols();


    std::vector<std::string> getSteadyStateSelectionList();
    void setSteadyStateSelectionList(const std::vector<std::string>& newSelectionList);
    double computeSteadyStateValue(const SelectionRecord& record);

    /**
     * returns the value of the given steady state identifier.
     */
    std::vector<double> computeSteadyStateValues();

    /**
     * optionally compute the steady state and return a vector
     * of the steady state values.
     *
     * @param selection: the list of selections to get values for.
     * @param computeSteadyState: compute the steady state.
     */
    std::vector<double> computeSteadyStateValues(
            const std::vector<SelectionRecord>& selection,
            bool computeSteadyState);

    double computeSteadyStateValue(const std::string& sId);

    /**
     * returns the values selected with SimulateOptions for the current model time / timestep")
     */
    std::vector<double> getSelectedValues();

    /**
     * This method turns on / off the computation and adherence to conservation laws.
     */
    void setConservationAnalysis(bool value);

    /**
     * is conservation analysis enabled. This is set
     */
    bool getConservationAnalysis();

    /**
     * Returns the SBML with the current parameterset.
     */
    std::string writeSBML();

    int getNumberOfReactions();
    double getReactionRate(const int& index);

    /**
     * Returns the rate of changes of a species by its index
     */
    double getRateOfChange(const int& index);
    std::vector<std::string> getRateOfChangeIds();
    std::vector<double> getRatesOfChangeEx(const std::vector<double>& values);
    std::vector<double> getReactionRatesEx(const std::vector<double>& values);


    std::vector<std::string> getConservedSumIds();

    std::vector<double> getConservedSums();

    int getNumberOfCompartments();

    /**
     * Sets the value of a compartment by its index
     */
    void setCompartmentByIndex(const int& index, const double& value);

    /**
     * @deprecated, use ExecutableModel::getCompartmentVolumes
     * Returns the value of a compartment by its index
     */
    double getCompartmentByIndex(const int& index);

    /**
     * @deprecated, use ExecutableModel::getCompartmentId
     */
    std::vector<std::string> getCompartmentIds();

    /**
     * Get the number of boundary species,
     * @deprecated, use ExecutableModel::getNumBoundarySpecies
     */
    int getNumberOfBoundarySpecies();

    /**
     * @deprecated, use ExecutableModel::setBoundarySpeciesConcentrations
     */
    void setBoundarySpeciesByIndex(const int& index, const double& value);

    /**
     * @deprecated, use ExecutableModel::getBoundarySpeciesConcentrations
     */
    double getBoundarySpeciesByIndex(const int& index);

    /**
     * @deprecated, use ExecutableModel::setBoundarySpeciesConcentrations
     */
    std::vector<double> getBoundarySpeciesConcentrations();

    /**
     * @deprecated, use ExecutableModel::setBoundarySpeciesConcentrations
     */
    void setBoundarySpeciesConcentrations(const std::vector<double>& values);

    /**
     * @deprecated, use ExecutableModel::getBoundarySpeciesId
     */
    std::vector<std::string> getBoundarySpeciesIds();

    /**
     * @deprecated, use ExecutableModel::getNumFloatingSpecies
     */
    int getNumberOfFloatingSpecies();
    void setFloatingSpeciesByIndex(const int& index, const double& value);
    double getFloatingSpeciesInitialConcentrationByIndex(const int& index);
    double getFloatingSpeciesByIndex(const int& index);
    std::vector<double> getFloatingSpeciesConcentrations();
    std::vector<double> getFloatingSpeciesInitialConcentrations();
    void setFloatingSpeciesConcentrations(const std::vector<double>& values);
    void setFloatingSpeciesInitialConcentrationByIndex(const int& index,
            const double& value);
    void setFloatingSpeciesInitialConcentrations(const std::vector<double>& values);
    std::vector<std::string> getFloatingSpeciesIds();
    std::vector<std::string> getFloatingSpeciesInitialConditionIds();
    std::vector<std::string> getFloatingSpeciesAmountIds();

    /**
     * @deprecated use ExecutableModel::getNumGlobalParameters
     */
    int getNumberOfGlobalParameters();

    /**
     * @deprecated use ExecutableModel::setGlobalParameterValues
     */
    void setGlobalParameterByIndex(const int index, const double value);

    /**
     * @deprecated use ExecutableModel::getGlobalParameterValues
     */
    double getGlobalParameterByIndex(const int& index);

    /**
     * @deprecated use ExecutableModel::getGlobalParameterValues
     */
    std::vector<double> getGlobalParameterValues();

    /**
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
     * return a string containing the RoadRunner version, the compiler
     * it was built with and when it was compiled.
     */
    static std::string getVersion();

    /**
     * getVersion plus info about dependent libs versions..
     */
    static std::string getExtendedVersionInfo();

    /**
     * get the copyright string
     */
    static std::string getCopyright();

    /**
     * Get unscaled control coefficient with respect to a global parameter
     */
    double getuCC(const std::string& variableName, const std::string& parameterName);

    /**
     * Get scaled control coefficient with respect to a global parameter
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
    ls::DoubleMatrix getScaledReorderedElasticityMatrix();

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

    SimulateOptions mSettings;

    int createDefaultSteadyStateSelectionList();
    int createDefaultTimeCourseSelectionList();

    void addNthOutputToResult(ls::DoubleMatrix& results, int nRow,
            double dCurrentTime);
    bool populateResult();


    double getNthSelectedOutput(const int& index, const double& dCurrentTime);

    double getVariableValue(const VariableType::VariableType variableType,
            const int variableIndex);

    std::vector<std::string> getParameterIds();

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

    std::vector<SelectionRecord> getSteadyStateSelection(const std::vector<std::string>& newSelectionList);
    std::vector<SelectionRecord> getSelectionList();

    /**
     * @deprecated, use ExecutableModel::getBoundarySpeciesId
     */
    std::vector<std::string> getBoundarySpeciesAmountIds();

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

 \author Totte Karlsson (totte@dunescientific.com)
 \author Frank T. Bergmann (fbergman@u.washington.edu)
 \author Herbert M. Sauro  (hsauro@u.washington.edu)
 \author Andy Somogyi (andy.somogyi@gmail.com, somogyie@indiana.edu)

 \par License
 \par
 Copyright (c) 2012, Frank T Bergmann and Herbert M Sauro\n
 All rights reserved.

 \par
 Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

 \li Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

 \li Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

 \li Neither the name of University of Washington nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

 \par

 */
