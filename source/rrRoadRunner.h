#ifndef rrRoadRunnerH
#define rrRoadRunnerH
#include <string>
#include "rr-libstruct/lsMatrix.h"
#include "rr-libstruct/lsLibStructural.h"
#include "rrObject.h"
#include "rrTVariableType.h"
#include "rrTParameterType.h"
#include "rrCVODEInterface.h"
#include "rrNLEQInterface.h"
#include "rrStringList.h"
#include "rrStringListContainer.h"
#include "rrSelectionRecord.h"
#include "rrTextWriter.h"
#include "rrRoadRunnerData.h"
#include "rrSimulationSettings.h"
#include "rrCompiler.h"
#include "rrArrayList.h"
#include "rrArrayList2.h"
#include "rrXMLDocument.h"
#include "rrNOMSupport.h"
#include "rrConstants.h"
#include "rrNewArrayList.h"
#include "rrPluginManager.h"
#include "rrModelSharedLibrary.h"
#include "rrCapabilities.h"
#include "rrParameter.h"
#include "Poco/Thread.h"

namespace rr
{
using Poco::Mutex;
using std::string;
using namespace ls;

class ModelGenerator;
class SBMLModelSimulation;
class ExecutableModel;
class CSharpModelGenerator;
class CModelGenerator;

class RR_DECLSPEC RoadRunner : public rrObject
{
	private:
    	static int	  					mInstanceCount;
    	int	  							mInstanceID;
		bool                            mUseKinsol;
		const double                    mDiffStepSize;
		Capabilities					mCapabilities;
		Capability						mRRCoreCapabilities;
        const string					mModelFolder;			//Folder for XML models
		const double                    mSteadyStateThreshold;
        DoubleMatrix                    mRawRoadRunnerData;
		RoadRunnerData                  mRoadRunnerData;
	    string 							mSupportCodeFolder;		//The compiler needs this in order to compile models

		string                   		mTempFileFolder;
        string							mCurrentSBMLFileName;
		SBMLModelSimulation            *mSimulation;

		CvodeInterface                 *mCVode;
		SteadyStateSolver              *mSteadyStateSolver;
		vector<SelectionRecord>         mSelectionList;
		ModelGenerator                 *mModelGenerator;    //Pointer to one of the below ones..
		CSharpModelGenerator                *mCSharpGenerator;
		CModelGenerator                     *mCGenerator;
//		Compiler                        mCompiler;

		Parameter<bool>            		mComputeAndAssignConservationLaws;

		vector<SelectionRecord>         mSteadyStateSelection;
		double                          mTimeStart;
		double                          mTimeEnd;
		int                             mNumPoints;

		/**
		 * something in libsbml or libstructural is not thread safe.
		 */
   		static Mutex	 				mLibSBMLMutex;

		ExecutableModel*                mModel;

		string                          mCurrentSBML;

		/**
		 * structural analysis library.
		 */
		LibStructural                   mLS;

		SimulationSettings              mSettings;
		NOMSupport						mNOM;
		PluginManager					mPluginManager;

		void                            addNthOutputToResult(DoubleMatrix& results, int nRow, double dCurrentTime);
		bool                            populateResult();
		bool                            isNleqAvailable();


		double                          getNthSelectedOutput(const int& index, const double& dCurrentTime);
		vector<double>                  buildModelEvalArgument();
		double                          getVariableValue(const TVariableType& variableType, const int& variableIndex);

		StringList                      getParameterIds();
		bool 							loadSBMLIntoNOM(const string& sbml);

		/**
		 * load the sbml into the structural analysis module, mLS.
		 */
		bool 							loadSBMLIntoLibStruct(const string& sbml);
		string 							createModelName(const string& mCurrentSBMLFileName);

	public: //These should be hidden later on...
		bool                     		mConservedTotalChanged;

	public:
 										RoadRunner(const string& tempFolder = gDefaultTempFolder,
 												const string& supportCodeFolder = gDefaultSupportCodeFolder,
 												const string& compiler = gDefaultCompiler);
		virtual                        ~RoadRunner();
        int								getInstanceID();
        int								getInstanceCount();

		bool        					computeAndAssignConservationLaws();
		void                            setParameterValue(const TParameterType& parameterType, const int& parameterIndex, const double& value);
		double                          getParameterValue(const TParameterType& parameterType, const int& parameterIndex);

		string                  		getParamPromotedSBML(const string& sArg);
		NOMSupport*						getNOM();
		string							getInfo();
        PluginManager&					getPluginManager();


		vector<SelectionRecord>         getSteadyStateSelection(const StringList& newSelectionList);
		vector<SelectionRecord>         getSelectionList();

		/**
		 * The Compiler that the ModelGenerator is using to compile / interpret sbml code.
		 */
   		Compiler*						getCompiler();

   		/**
   		 * Set the name of the externa compiler to use. Some ModelGenerators may have no use
   		 * for this value.
   		 */
   		bool                            setCompiler(const string& compiler);


		//Functions --------------------------------------------------------------------
        bool                            isModelLoaded();

        string                          getModelName();
		string							getlibSBMLVersion();
        bool                            unLoadModel();
        bool                            unLoadModelDLL();
        CvodeInterface*                 getCVodeInterface();
        NLEQInterface*                  getNLEQInterface();
        int 							createDefaultSteadyStateSelectionList();
        int                             createDefaultTimeCourseSelectionList();
		int                             createTimeCourseSelectionList();

		/**
		 * set the location where the ModelGenerator creates temporary files, such
		 * as shared libraries.
		 */
		bool                     		setTempFileFolder(const string& folder);

		/**
		 * get the ModelGenerator's temporary file directory.
		 */
		string                   		getTempFolder();

		//Simulation stuff
		DoubleMatrix                    simulate();
		bool                            simulate2();
		DoubleMatrix                    simulateEx(const double& startTime, const double& endTime, const int& numberOfPoints);
		bool                            simulate2Ex(const double& startTime = 0, const double& endTime = 5, const int& numberOfPoints = 50);

		double                          getValueForRecord(const SelectionRecord& record);

		void                            partOfSimulation(SBMLModelSimulation* simulation){mSimulation = simulation;}
		RoadRunnerData                  getSimulationResult();
        bool							loadSimulationSettings(const string& fName);
		bool                            useSimulationSettings(SimulationSettings& settings);
		DoubleMatrix                    runSimulation();
		bool                            initializeModel();
		bool                            simulateSBMLFile(const string& fileName, const bool& useConservationLaws);
		bool 							createDefaultSelectionLists();
		string                          getSBML();
		double                          getTimeStart();
		double                          getTimeEnd();
		int                             getNumPoints();
		void                            setTimeStart(const double& startTime);
		void                            setTimeEnd(const double& endTime);
		void                            setNumPoints(const int& nummberOfPoints);

		/**
		 * Reset the simulator back to the initial conditions specified in the SBML model,
		 * provided an SBML model is loaded.
		 */
		void                            reset();
		void                            changeInitialConditions(const vector<double>& ic);

		/**
		 * get the ModelGenerator that is used to create executable (runnable) models.
		 */
		ModelGenerator*                 getModelGenerator();


		//Model generation
		ExecutableModel*				getModel();

		bool                            loadSBMLFromFile(const string& fileName, const bool& forceReCompile = false);
		bool                            loadSBML(const string& sbml, const bool& forceReCompile = false);

		vector<double>                  getReactionRates();
		vector<double>                  getRatesOfChange();
		StringList                      getSpeciesIds();
		StringList                      getReactionIds();

 		// ---------------------------------------------------------------------
		// Start of Level 2 API Methods
		// ---------------------------------------------------------------------
		Capabilities&                   getCapabilities();
		Capability* 	                getCapability(const string& cap_name);
		string                          getCapabilitiesAsXML();
		StringList                      getListOfCapabilities();
		StringList                      getListOfParameters(const string& capName);

        bool						  	addCapability(Capability& cap);
        bool						  	addCapabilities(Capabilities& caps);
        bool							setCapabilityParameter(const string& cap, const string& parameter, const string& value);

		void                            setCapabilities(const string& capsStr);

		void                            setTolerances(const double& aTol, const double& rTol);
		void                            correctMaxStep();

		bool                            setValue(const string& sId, const double& dValue);
		double                          getValue(const string& sId);
		NewArrayList                    getAvailableTimeCourseSymbols();
		StringList                      getTimeCourseSelectionList();
		void                            setTimeCourseSelectionList(const string& List);
		void                            setTimeCourseSelectionList(const StringList& newSelectionList);
		double                          oneStep(const double& currentTime, const double& stepSize);
		double                          oneStep(const double& currentTime, const double& stepSize, const bool& reset);

		// ---------------------------------------------------------------------
		// Start of Level 3 API Methods
		// ---------------------------------------------------------------------
		double                          steadyState();
		DoubleMatrix                    getFullJacobian();
		DoubleMatrix 					getFullReorderedJacobian();
		DoubleMatrix                    getReducedJacobian();
        DoubleMatrix                    getEigenvalues();
		DoubleMatrix                    getEigenvaluesFromMatrix (DoubleMatrix m);
		vector<Complex>                 getEigenvaluesCpx();

		// ---------------------------------------------------------------------
		// Start of Level 4 API Methods
		// ---------------------------------------------------------------------
		DoubleMatrix*                   getLinkMatrix();
		DoubleMatrix*                   getNrMatrix();
		DoubleMatrix*                   getL0Matrix();
		DoubleMatrix                    getStoichiometryMatrix();
		DoubleMatrix                    getReorderedStoichiometryMatrix();
		DoubleMatrix                    getFullyReorderedStoichiometryMatrix();
 		DoubleMatrix                    getConservationMatrix();
		DoubleMatrix                    getUnscaledConcentrationControlCoefficientMatrix();
		DoubleMatrix                    getScaledConcentrationControlCoefficientMatrix();
        DoubleMatrix                    getUnscaledFluxControlCoefficientMatrix();
        DoubleMatrix                    getScaledFluxControlCoefficientMatrix();
		int                             getNumberOfDependentSpecies();
		int                             getNumberOfIndependentSpecies();
		void                            computeContinuation(const double& stepSize, const int& independentVariable, const string& parameterTypeStr);
        NewArrayList                 	getUnscaledFluxControlCoefficientIds();
		NewArrayList           	      	getFluxControlCoefficientIds();
        NewArrayList                 	getUnscaledConcentrationControlCoefficientIds();
		NewArrayList                 	getConcentrationControlCoefficientIds();
		NewArrayList                 	getElasticityCoefficientIds();
		NewArrayList                 	getUnscaledElasticityCoefficientIds();
		StringList                      getEigenvalueIds();
		NewArrayList                 	getAvailableSteadyStateSymbols();
		StringList                   	getSteadyStateSelectionList();
		void                            setSteadyStateSelectionList(const StringList& newSelectionList);
		double                          computeSteadyStateValue(const SelectionRecord& record);
		vector<double>                  computeSteadyStateValues();
		vector<double>                  computeSteadyStateValues(const StringList& selection);
		vector<double>                  computeSteadyStateValues(const vector<SelectionRecord>& selection, const bool& computeSteadyState);
		double                          computeSteadyStateValue(const string& sId);
		vector<double>                  getSelectedValues();


		void                     		computeAndAssignConservationLaws(const bool& bValue);
		double*                         steadyStateParameterScan(const string& symbol, const double& startValue, const double& endValue, const double& stepSize);

		/**
		 * Returns the SBML with the current parameterset.
		 */
		string                          writeSBML();
		int                             getNumberOfLocalParameters(const int& reactionId);
		void                            setLocalParameterByIndex(const int& reactionId, const int index, const double& value);
		double                          getLocalParameterByIndex(const int& reactionId, const int& index);
		void                            setLocalParameterValues(const int& reactionId, const vector<double>& values);
		vector<double>                  getLocalParameterValues(const int& reactionId);
		StringList                      getLocalParameterIds(const int& reactionId);
		StringList                      getAllLocalParameterTupleList();
		int                             getNumberOfReactions();
		double                          getReactionRate(const int& index);
		double                          getRateOfChange(const int& index);
		StringList                      getRateOfChangeIds();
		vector<double>                  getRatesOfChangeEx(const vector<double>& values);
		vector<double>                  getReactionRatesEx(const vector<double>& values);
		vector<string>                  getFloatingSpeciesIdsArray();
		vector<string>                  getGlobalParameterIdsArray();
		int                             getNumberOfCompartments();
		void                            setCompartmentByIndex(const int& index, const double& value);
		double                          getCompartmentByIndex(const int& index);
		void                            setCompartmentVolumes(const vector<double>& values);
		StringList                      getCompartmentIds();
		int                             getNumberOfBoundarySpecies();
		void                            setBoundarySpeciesByIndex(const int& index, const double& value);
		double                          getBoundarySpeciesByIndex(const int& index);
		vector<double>                  getBoundarySpeciesConcentrations();
		void                            setBoundarySpeciesConcentrations(const vector<double>& values);
		StringList                      getBoundarySpeciesIds();
		StringList                      getBoundarySpeciesAmountIds();
		int                             getNumberOfFloatingSpecies();
		void                            setFloatingSpeciesByIndex(const int& index, const double& value);
		double                          getFloatingSpeciesByIndex(const int& index);
		vector<double>                  getFloatingSpeciesConcentrations();
		vector<double>                  getFloatingSpeciesInitialConcentrations();
		void                            setFloatingSpeciesConcentrations(const vector<double>& values);
		void                            setFloatingSpeciesInitialConcentrationByIndex(const int& index, const double& value);
		void                            setFloatingSpeciesInitialConcentrations(const vector<double>& values);
		StringList                      getFloatingSpeciesIds();
		StringList                      getFloatingSpeciesInitialConditionIds();
		StringList                      getFloatingSpeciesAmountIds();
		int                             getNumberOfGlobalParameters();
		void                            setGlobalParameterByIndex(const int& index, const double& value);
		double                          getGlobalParameterByIndex(const int& index);
		void                            setGlobalParameterValues(const vector<double>& values);
		vector<double>                  getGlobalParameterValues();
		StringList                      getGlobalParameterIds();
		StringList                      getAllGlobalParameterTupleList();
		void                            evalModel();

		//These functions are better placed in a separate file, as non class members, but part of the roadrunner namespace?
		string                          getName();
		string                          getVersion();
		string							getExtendedVersionInfo();	//Include info about dependent libs versions..
		string                          getAuthor();
		string                          getDescription();
		string                          getDisplayName();
		string                          getCopyright();
		string                          getURL();

		//Plugin stuff
        bool							loadPlugins();
        bool							unLoadPlugins();

		//RoadRunner MCA functions......

        //[Help("Get unscaled control coefficient with respect to a global parameter")]
		double 							getuCC(const string& variableName, const string& parameterName);

		//[Help("Get scaled control coefficient with respect to a global parameter")]
		double 							getCC(const string& variableName, const string& parameterName);

		//[Help("Get unscaled elasticity coefficient with respect to a global parameter or species")]
		double 							getuEE(const string& reactionName, const string& parameterName);

		//[Help("Get unscaled elasticity coefficient with respect to a global parameter or species. Optionally the model is brought to steady state after the computation.")]
		double 							getuEE(const string& reactionName, const string& parameterName, bool computeSteadystate);

    	//[Help("Get scaled elasticity coefficient with respect to a global parameter or species")]
		double 							getEE(const string& reactionName, const string& parameterName);

		//[Help("Get scaled elasticity coefficient with respect to a global parameter or species. Optionally the model is brought to steady state after the computation.")]
		double 							getEE(const string& reactionName, const string& parameterName, bool computeSteadyState);

        // Get a single species elasticity value
        // IMPORTANT:
        // Assumes that the reaction rates have been precomputed at the operating point !!
		double 							getUnscaledSpeciesElasticity(int reactionId, int speciesIndex);

		//"Returns the elasticity of a given reaction to a given parameter. Parameters can be boundary species or global parameters"
		double 							getUnScaledElasticity(const string& reactionName, const string& parameterName);

		//"Compute the unscaled species elasticity matrix at the current operating point")]
		DoubleMatrix 					getUnscaledElasticityMatrix();

		//"Compute the unscaled elasticity matrix at the current operating point")]
		DoubleMatrix 					getScaledReorderedElasticityMatrix();

		//[Help("Compute the unscaled elasticity for a given reaction and given species")]
		double 							getUnscaledFloatingSpeciesElasticity(const string& reactionName, const string& speciesName);

		//[Help("Compute the scaled elasticity for a given reaction and given species")]
		double 							getScaledFloatingSpeciesElasticity(const string& reactionName, const string& speciesName);

		// Changes a given parameter type by the given increment
		void 							changeParameter(TParameterType parameterType, int reactionIndex, int parameterIndex, double originalValue, double increment);

        //[Help("Returns the unscaled elasticity for a named reaction with respect to a named parameter (local or global)"
        double 							getUnscaledParameterElasticity(const string& reactionName, const string& parameterName);

        //"Compute the value for a particular unscaled concentration control coefficients with respect to a local parameter"
        double 							getUnscaledConcentrationControlCoefficient(const string& speciesName, const string& localReactionName, const string& parameterName);

        //"Compute the value for a particular scaled concentration control coefficients with respect to a local parameter"
        double 							getScaledConcentrationControlCoefficient(const string& speciesName, const string& localReactionName, const string& parameterName);

        //"Compute the value for a particular concentration control coefficient, permitted parameters include global parameters, boundary conditions and conservation totals"
        double 							getUnscaledConcentrationControlCoefficient(const string& speciesName, const string& parameterName);

        //"Compute the value for a particular scaled concentration control coefficients with respect to a global or boundary species parameter"
        double 							getScaledConcentrationControlCoefficient(const string& speciesName, const string& parameterName);

		//[Help("Compute the value for a particular unscaled flux control coefficients with respect to a local parameter")
        double 							getUnscaledFluxControlCoefficient(const string& fluxName, const string& localReactionName, const string& parameterName);

        //"Compute the value for a particular flux control coefficient, permitted parameters include global parameters, boundary conditions and conservation totals"
        double 							getUnscaledFluxControlCoefficient(const string& reactionName, const string& parameterName);

        //[Help("Compute the value for a particular scaled flux control coefficients with respect to a local parameter");]
        double 							getScaledFluxControlCoefficient(const string& reactionName, const string& localReactionName, const string& parameterName);

        //    "Compute the value for a particular scaled flux control coefficients with respect to a global or boundary species parameter"
        double 							getScaledFluxControlCoefficient(const string& reactionName, const string& parameterName);

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

\author		Totte Karlsson (totte@dunescientific.com)
\author  	Frank T. Bergmann (fbergman@u.washington.edu)
\author     Herbert M. Sauro  (hsauro@u.washington.edu)

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
