#ifndef rrRoadRunnerH
#define rrRoadRunnerH

#include "rr-libstruct/lsMatrix.h"
#include "rrVariableType.h"
#include "rrParameterType.h"
#include "rrSelectionRecord.h"
#include "rrRoadRunnerData.h"
#include "rrConstants.h"
#include "rrNewArrayList.h"
#include "rrCapabilities.h"
#include "rrParameter.h"
#include "rrRoadRunnerOptions.h"

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
class RR_DECLSPEC RoadRunner
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

    bool computeAndAssignConservationLaws();

    void setParameterValue(const ParameterType::ParameterType parameterType,
            const int parameterIndex, const double value);

    double getParameterValue(const ParameterType::ParameterType parameterType,
            const int parameterIndex);

    std::string getParamPromotedSBML(const std::string& sArg);


    std::string getInfo();

    std::vector<SelectionRecord> getSteadyStateSelection(const std::vector<std::string>& newSelectionList);
    std::vector<SelectionRecord> getSelectionList();

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

    int createTimeCourseSelectionList();

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

    const SimulateOptions& getSimulateOptions() const;


    bool initializeModel();

    bool createDefaultSelectionLists();
    std::string getSBML();


    /**
     * Reset the simulator back to the initial conditions specified in the SBML model,
     * provided an SBML model is loaded.
     */
    void reset();
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

    std::vector<double> getReactionRates();
    std::vector<double> getRatesOfChange();

    std::vector<std::string> getReactionIds();


    Capability* getCapability(const std::string& cap_name);
    std::string getCapabilitiesAsXML();
    std::vector<std::string> getListOfCapabilities();
    std::vector<std::string> getListOfParameters(const std::string& capName);

    bool addCapability(Capability& cap);
    bool addCapabilities(Capabilities& caps);

    void setCapabilities(const std::string& capsStr);

    void correctMaxStep();

    bool setValue(const std::string& sId, const double& dValue);
    double getValue(const std::string& sId);
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



    NewArrayList getUnscaledFluxControlCoefficientIds();
    NewArrayList getFluxControlCoefficientIds();
    NewArrayList getUnscaledConcentrationControlCoefficientIds();
    NewArrayList getConcentrationControlCoefficientIds();
    NewArrayList getElasticityCoefficientIds();
    NewArrayList getUnscaledElasticityCoefficientIds();
    std::vector<std::string> getEigenvalueIds();
    NewArrayList getAvailableSteadyStateSymbols();
    std::vector<std::string> getSteadyStateSelectionList();
    void setSteadyStateSelectionList(const std::vector<std::string>& newSelectionList);
    double computeSteadyStateValue(const SelectionRecord& record);
    std::vector<double> computeSteadyStateValues();

    std::vector<double> computeSteadyStateValues(
            const std::vector<SelectionRecord>& selection,
            const bool& computeSteadyState);

    double computeSteadyStateValue(const std::string& sId);
    std::vector<double> getSelectedValues();

    void computeAndAssignConservationLaws(const bool& bValue);

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
     * Returns the value of a compartment by its index
     */

    double getCompartmentByIndex(const int& index);

    std::vector<std::string> getCompartmentIds();

    /**
     * Get the number of boundary species
     */
    int getNumberOfBoundarySpecies();
    void setBoundarySpeciesByIndex(const int& index, const double& value);
    double getBoundarySpeciesByIndex(const int& index);
    std::vector<double> getBoundarySpeciesConcentrations();
    void setBoundarySpeciesConcentrations(const std::vector<double>& values);
    std::vector<std::string> getBoundarySpeciesIds();
    std::vector<std::string> getBoundarySpeciesAmountIds();
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
    int getNumberOfGlobalParameters();
    void setGlobalParameterByIndex(const int& index, const double& value);
    double getGlobalParameterByIndex(const int& index);



    std::vector<double> getGlobalParameterValues();
    std::vector<std::string> getGlobalParameterIds();


    void evalModel();

    //These functions are better placed in a separate file, as non class members, but part of the roadrunner namespace?

    static std::string getVersion();
    static std::string getExtendedVersionInfo(); //Include info about dependent libs versions..

    static std::string getDescription();

    static std::string getCopyright();
    static std::string getURL();

    //RoadRunner MCA functions......

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
    Capabilities mCapabilities;
    Capability mRRCoreCapabilities;
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
    rr::Parameter<bool> mComputeAndAssignConservationLaws;

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

};

}

#endif

/*! \mainpage RoadRunner C++ Library

 \par
 This document describes the application programming interface (API) of RoadRunner, an open source (BSD) library for computing structural characteristics of cellular networks.
 \par

 \par Dependencies
 The RoadRunner library depend on several third-party libraries, CLapack, libSBML, Sundials, NLEQ, Poco and Pugi. These are provided with the binary installation where necessary.
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
