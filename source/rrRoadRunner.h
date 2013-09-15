#ifndef rrRoadRunnerH
#define rrRoadRunnerH

#include "rr-libstruct/lsMatrix.h"
#include "rr-libstruct/lsLibStructural.h"
#include "rrTVariableType.h"
#include "rrTParameterType.h"
#include "rrSelectionRecord.h"
#include "rrRoadRunnerData.h"
#include "rrSimulationSettings.h"
#include "rrConstants.h"
#include "rrNewArrayList.h"
#include "rrCapabilities.h"
#include "rrParameter.h"
#include "rrRoadRunnerOptions.h"

#include <string>
#include <vector>

namespace rr
{
using std::string;
using std::vector;
using namespace ls;

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

    void setParameterValue(const TParameterType::TParameterType parameterType,
            const int parameterIndex, const double value);

    double getParameterValue(const TParameterType::TParameterType parameterType,
            const int parameterIndex);

    string getParamPromotedSBML(const string& sArg);

    LibStructural* getLibStruct();
    string getInfo();

    vector<SelectionRecord> getSteadyStateSelection(const vector<string>& newSelectionList);
    vector<SelectionRecord> getSelectionList();

    /**
     * The Compiler that the ModelGenerator is using to compile / interpret sbml code.
     */
    class Compiler* getCompiler();

    /**
     * Set the name of the externa compiler to use. Some ModelGenerators may have no use
     * for this value.
     */
    bool setCompiler(const string& compiler);

    /**
     * get a pointer to the integrator which is currently being used to
     * time evolve the system.
     */
    Integrator* getIntegrator();

    bool isModelLoaded();

    /**
     * returns the model name if a model is loaded, empty string otherwise.
     */
    string getModelName();

    static string getlibSBMLVersion();
    bool unLoadModel();

    int createTimeCourseSelectionList();

    /**
     * set the location where the ModelGenerator creates temporary files, such
     * as shared libraries.
     */
    bool setTempFileFolder(const string& folder);

    /**
     * get the ModelGenerator's temporary file directory.
     */
    string getTempFolder();

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

    RoadRunnerData getSimulationResult();   //Todo: should probably be removed..
    RoadRunnerData* getRoadRunnerData();

    bool loadSimulationSettings(const string& fName);
    bool setSimulationSettings(const SimulationSettings& settings);


    bool initializeModel();

    bool createDefaultSelectionLists();
    string getSBML();

    double getTimeStart();
    double getTimeEnd();
    int getNumPoints();
    void setTimeStart(double startTime);
    void setTimeEnd(double endTime);
    void setNumPoints(int nummberOfPoints);

    /**
     * Reset the simulator back to the initial conditions specified in the SBML model,
     * provided an SBML model is loaded.
     */
    void reset();
    void changeInitialConditions(const vector<double>& ic);

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
    bool loadSBMLFromFile(const string& fileName,
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
    bool loadSBML(const string& sbml, const LoadSBMLOptions* options = 0);

    vector<double> getReactionRates();
    vector<double> getRatesOfChange();

    vector<string> getReactionIds();


    Capability* getCapability(const string& cap_name);
    string getCapabilitiesAsXML();
    vector<string> getListOfCapabilities();
    vector<string> getListOfParameters(const string& capName);

    bool addCapability(Capability& cap);
    bool addCapabilities(Capabilities& caps);

    void setCapabilities(const string& capsStr);

    void correctMaxStep();

    bool setValue(const string& sId, const double& dValue);
    double getValue(const string& sId);
    NewArrayList getAvailableTimeCourseSymbols();

    /**
     * Returns the currently selected columns that will be returned by
     * calls to simulate() or simulateEx(,,).
     */
    vector<string> getTimeCourseSelectionList();
    void setTimeCourseSelectionList(const string& List);

    void setTimeCourseSelectionList(
            const std::vector<std::string>& newSelectionList);

    /**
     * Compute the steady state of the model, returns the sum of squares of the solution
     */
    double steadyState();

    /**
     * compute the full Jacobian at the current operating point
     */
    DoubleMatrix getFullJacobian();

    DoubleMatrix getFullReorderedJacobian();

    /**
     * Compute the reduced Jacobian at the current operating point.
     */
    DoubleMatrix getReducedJacobian();

    /**
     * Returns eigenvalues, first column real part, second column imaginary part
     */
    DoubleMatrix getEigenvalues();

    vector<Complex> getEigenvaluesCpx();

    DoubleMatrix* getLinkMatrix();
    DoubleMatrix* getNrMatrix();
    DoubleMatrix* getL0Matrix();
    DoubleMatrix getStoichiometryMatrix();
    DoubleMatrix getReorderedStoichiometryMatrix();
    DoubleMatrix getFullyReorderedStoichiometryMatrix();
    DoubleMatrix getConservationMatrix();
    DoubleMatrix getUnscaledConcentrationControlCoefficientMatrix();
    DoubleMatrix getScaledConcentrationControlCoefficientMatrix();
    DoubleMatrix getUnscaledFluxControlCoefficientMatrix();
    DoubleMatrix getScaledFluxControlCoefficientMatrix();
    int getNumberOfDependentSpecies();
    int getNumberOfIndependentSpecies();



    NewArrayList getUnscaledFluxControlCoefficientIds();
    NewArrayList getFluxControlCoefficientIds();
    NewArrayList getUnscaledConcentrationControlCoefficientIds();
    NewArrayList getConcentrationControlCoefficientIds();
    NewArrayList getElasticityCoefficientIds();
    NewArrayList getUnscaledElasticityCoefficientIds();
    vector<string> getEigenvalueIds();
    NewArrayList getAvailableSteadyStateSymbols();
    vector<string> getSteadyStateSelectionList();
    void setSteadyStateSelectionList(const vector<string>& newSelectionList);
    double computeSteadyStateValue(const SelectionRecord& record);
    vector<double> computeSteadyStateValues();

    vector<double> computeSteadyStateValues(
            const vector<SelectionRecord>& selection,
            const bool& computeSteadyState);

    double computeSteadyStateValue(const string& sId);
    vector<double> getSelectedValues();

    void computeAndAssignConservationLaws(const bool& bValue);

    /**
     * Returns the SBML with the current parameterset.
     */
    string writeSBML();

    int getNumberOfReactions();
    double getReactionRate(const int& index);

    /**
     * Returns the rate of changes of a species by its index
     */
    double getRateOfChange(const int& index);
    vector<string> getRateOfChangeIds();
    vector<double> getRatesOfChangeEx(const vector<double>& values);
    vector<double> getReactionRatesEx(const vector<double>& values);


    vector<string> getConservedSumIds();
    int getNumberOfCompartments();

    /**
     * Sets the value of a compartment by its index
     */
    void setCompartmentByIndex(const int& index, const double& value);

    /**
     * Returns the value of a compartment by its index
     */

    double getCompartmentByIndex(const int& index);

    vector<string> getCompartmentIds();

    /**
     * Get the number of boundary species
     */
    int getNumberOfBoundarySpecies();
    void setBoundarySpeciesByIndex(const int& index, const double& value);
    double getBoundarySpeciesByIndex(const int& index);
    vector<double> getBoundarySpeciesConcentrations();
    void setBoundarySpeciesConcentrations(const vector<double>& values);
    vector<string> getBoundarySpeciesIds();
    vector<string> getBoundarySpeciesAmountIds();
    int getNumberOfFloatingSpecies();
    void setFloatingSpeciesByIndex(const int& index, const double& value);
    double getFloatingSpeciesInitialConcentrationByIndex(const int& index);
    double getFloatingSpeciesByIndex(const int& index);
    vector<double> getFloatingSpeciesConcentrations();
    vector<double> getFloatingSpeciesInitialConcentrations();
    void setFloatingSpeciesConcentrations(const vector<double>& values);
    void setFloatingSpeciesInitialConcentrationByIndex(const int& index,
            const double& value);
    void setFloatingSpeciesInitialConcentrations(const vector<double>& values);
    vector<string> getFloatingSpeciesIds();
    vector<string> getFloatingSpeciesInitialConditionIds();
    vector<string> getFloatingSpeciesAmountIds();
    int getNumberOfGlobalParameters();
    void setGlobalParameterByIndex(const int& index, const double& value);
    double getGlobalParameterByIndex(const int& index);



    vector<double> getGlobalParameterValues();
    vector<string> getGlobalParameterIds();


    void evalModel();

    //These functions are better placed in a separate file, as non class members, but part of the roadrunner namespace?

    static string getVersion();
    static string getExtendedVersionInfo(); //Include info about dependent libs versions..

    static string getDescription();

    static string getCopyright();
    static string getURL();

    //RoadRunner MCA functions......

    /**
     * Get unscaled control coefficient with respect to a global parameter
     */
    double getuCC(const string& variableName, const string& parameterName);

    /**
     * Get scaled control coefficient with respect to a global parameter
     */
    double getCC(const string& variableName, const string& parameterName);

    /**
     * Get unscaled elasticity coefficient with respect to a global parameter or species
     */
    double getuEE(const string& reactionName, const string& parameterName);

    /**
     * Get unscaled elasticity coefficient with respect to a global parameter or species.
     * Optionally the model is brought to steady state after the computation.
     */
    double getuEE(const string& reactionName, const string& parameterName,
            bool computeSteadystate);

    /**
     * Get scaled elasticity coefficient with respect to a global parameter or species
     */
    double getEE(const string& reactionName, const string& parameterName);

    /**
     * Get scaled elasticity coefficient with respect to a global parameter or species.
     * Optionally the model is brought to steady state after the computation.
     */
    double getEE(const string& reactionName, const string& parameterName,
            bool computeSteadyState);

    /**
     * Compute the unscaled species elasticity matrix at the current operating point
     */
    DoubleMatrix getUnscaledElasticityMatrix();

    /**
     * Compute the unscaled elasticity matrix at the current operating point
     */
    DoubleMatrix getScaledReorderedElasticityMatrix();

    /**
     * Compute the scaled elasticity for a given reaction and given species
     */
    double getScaledFloatingSpeciesElasticity(const string& reactionName,
            const string& speciesName);

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
    DoubleMatrix mRawRoadRunnerData;
    RoadRunnerData mRoadRunnerData;

    string mCurrentSBMLFileName;

    /**
     * The Cvode object get created just after a model is created, it then
     * gets a reference to the model and holds on to it.
     */
    class CvodeInterface *mCVode;
    vector<SelectionRecord> mSelectionList;

    /**
     * ModelGenerator obtained from the factory
     */
    ModelGenerator *mModelGenerator;

    /**
     * RoadRunner, not sbml parameters
     */
    rr::Parameter<bool> mComputeAndAssignConservationLaws;

    vector<SelectionRecord> mSteadyStateSelection;
    double mTimeStart;
    double mTimeEnd;
    int mNumPoints;

    ExecutableModel* mModel;

    string mCurrentSBML;

    /**
     * structural analysis library.
     */
    LibStructural* mLS;

    SimulationSettings mSettings;

    int createDefaultSteadyStateSelectionList();
    int createDefaultTimeCourseSelectionList();

    void addNthOutputToResult(DoubleMatrix& results, int nRow,
            double dCurrentTime);
    bool populateResult();


    double getNthSelectedOutput(const int& index, const double& dCurrentTime);

    double getVariableValue(const TVariableType::TVariableType variableType,
            const int variableIndex);

    vector<string> getParameterIds();

    string createModelName(const string& mCurrentSBMLFileName);

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
