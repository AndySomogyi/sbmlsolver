/**
 * This file is part of SBMLSolver.
 *
 * SBMLSolver is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Foobar is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with SBMLSolver.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef SBMLSOLVER_H_
#define SBMLSOLVER_H_

#include <SBMLSolverOptions.h>
#include "rrOSSpecifics.h"
#include "rr-libstruct/lsMatrix.h"
#include "rrSelectionRecord.h"
#include <string>
#include <vector>
#include <list>
#include "include/MxReactionNetwork.h"

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
 * The main SBMLSolver class.
 *
 * The SBMLSolver class is responsible for loading and simulating SBML models.
 *
 * MemoryManagment: Any pointer returned by a get... method is owned by the
 * RoadRunner object and does NOT have to be deleted.
 */
class RR_DECLSPEC SBMLSolver : public ISBMLSolver
{

public:

    /**
     * create an empty RoadRunner object.
     */
    SBMLSolver();

    /**
     * load an sbml document from anywhere.
     *
     * same arguments as load.
     *
     * If options is not null, then the RoadRunner::computeAndAssignConservationLaws
     * flag is set to whatever value is specified in the options struct.
     *
     * @param uriOrSBML: a URI, local path or sbml document contents.
     * @param options: an options struct, if null, default values are used.
     */
    SBMLSolver(const std::string& uriOrSBML, const Dictionary* options = 0);

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
    SBMLSolver(const std::string& compiler, const std::string& tempDir,
            const std::string& supportCodeDir);

    /**
     * free any memory this class allocated
     */
    virtual ~SBMLSolver();

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
     *
     * An exception is raised if the string is invalid.
     */
    void setCompiler(const std::string& compiler);

    /**
     * get a pointer to the integrator which is currently being used to
     * time evolve the system.
     */
    Integrator *getIntegrator();

    /**
     * gets a pointer to a specific integrator.
     *
     * Throws a std::invalid_argument if intg is not valid.
     */
    Integrator *getIntegrator(Integrator::IntegratorId intg);

    bool isModelLoaded();

    /**
     * returns the model name if a model is loaded, empty string otherwise.
     */
    std::string getModelName();

    bool unLoadModel();


    /**
     * Carry out a single integration step using a stepsize as indicated
     * in the method call.
     *
     * @param t0 starting time
     * @param tf final time
     * @param options override current options.
     */
    double integrate(double t0, double tf, const SimulateOptions* options = 0);

    /**
     * @deprecated, use integrate instead.
     */
    double oneStep(double currentTime, double stepSize, bool reset = true);

    /**
     * Simulate the current SBML model.
     *
     * If options is null, then the current simulation settings (start time,
     * end time, n steps) are used. If options is not null, then the
     * current simulation settings are set to the values specified by
     * options and they are used.
     *
     * The options Dictionary may contain a large number of options, for a complete
     * list of all available options for each integrator type, @see IntegratorFactory,
     * @see SimulateOptions.
     *
     * For example, to perform a simulation from time 0 to 10 with 1000 steps, using a
     * stiff integtator, you would:
     * @code
     * RoadRunner r = RoadRunner("someFile.xml");
     * BasicDictionary opt;
     * opt.setItem("start", 0);
     * opt.setItem("duration", 10);
     * opt.setItem("steps", 1000);
     * opt.setItem("stiff", true);
     * const DoubleMatrix *result = r.simulate(&opt);
     * @endcode
     *
     * Similarly, if one wants to use a stochastic integrator, such as the Gillespie
     * integrator, this is set via the "integrator" key, i.e.
     * @code
     * RoadRunner r = RoadRunner("someFile.xml");
     * BasicDictionary opt;
     * opt.setItem("integrator", "gillespie");
     * opt.setItem("start", 0);
     * opt.setItem("duration", 10);
     * opt.setItem("steps", 1000);
     * opt.setItem("stiff", true);
     * opt.setItem("seed", 12345);
     * const DoubleMatrix *result = r.simulate(&opt);
     * @endcode
     * Here, the "integrator" specifies the integrator to use. The "stiff" key
     * is only used by the deterministic solvers, and it is safely ignored by the
     * stochastic solvers. Also, the "seed" sets the random seed that the integrator
     * uses. For more information about all of the avaialble options for each integrator,
     * @see IntegratorFactory::getIntegratorOptions".
     *
     * @throws an std::exception if any options are invalid.
     * @returns a borrowed reference to a DoubleMatrix object if successfull.
     */
    const ls::DoubleMatrix *simulate(const Dictionary* options = 0);

    /**
     * RoadRunner keeps a copy of the simulation data around until the
     * next call to simulate. This matrix can be obtained here.
     */
    const ls::DoubleMatrix* getSimulationData() const;

    #ifndef SWIG // deprecated methods not SWIG'ed

    #endif

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
    SBMLSolverOptions& getOptions();


    void setOptions(const SBMLSolverOptions&);

    /**
     * get the originally loaded sbml document as a string.
     *
     * This may optionally up or downconvert the document to a different version, if
     * the level and version arguments are non-zero.
     *
     * If both arguments are zero, then the document is left alone and the
     */
    std::string getSBML(int level = 0, int version = 0);


    /**
     * Returns the SBML with the current model parameters. This is different than
     * getSBML which returns the orginal SBML.
     *
     * This may optionally up or downconvert the document to a different version, if
     * the level and version arguments are non-zero.
     *
     * If both arguments are zero, then the document is left alone and the original
     * version is returned.
     */
    std::string getCurrentSBML(int level = 0, int version = 0);

    /**
     * Picks up default options from config.
     *
     * The current default is
     * SelectionRecord::TIME | SelectionRecord::RATE | SelectionRecord::FLOATING.
     */
    void reset();

    /**
     * reset the model accordign to a bitfield specified by the
     * SelectionRecord::SelectionType values.
     *
     * Note, this would make more sense as an unsigned, however SWIG has issues mapping
     * an enum to an unsigned, but seems to map enums just fine to an int.
     *
     * For example, to reset the floating species, time and rate rule values:
     * @code
     * r.reset(SelectionRecord::TIME | SelectionRecord::RATE | SelectionRecord::FLOATING);
     * @endcode
     *
     * @param options a bitmask made from the SelectionRecord::SelectionTypes values.
     */
    void reset(int options);

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
     * get a pointer to the ExecutableModel owned by the RoadRunner object.
     */
    ExecutableModel* getModel();

    /**
     * load an sbml document from anywhere.
     *
     * If options is not null, then the RoadRunner::computeAndAssignConservationLaws
     * flag is set to whatever value is specified in the options struct.
     *
     * WARNING, will no longer return a value in next version.
     *
     * @param uriOrSBML: a URI, local path or sbml document contents.
     * @param options: an options struct, if null, default values are used.
     */
    void load(const std::string& uriOrSBML,
            const Dictionary* options = 0);


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
     *
     * raises an exception in the selection string is invalid.
     */
    void setValue(const std::string& id, double value);

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
     * @param h The step sized used for central difference method.
     *          If negative, the default value from the config file is used.
     */
    ls::DoubleMatrix getReducedJacobian(double h = -1.0);

    /**
     * Returns the eigenvalues of the full jacobian.
     *
     * If the eigenvalues are all real, this returns a N x 1 matrix,
     * if complex, returns an N x 2 matrix where the first column is the
     * real values and the second is the imaginary part.
     */
    std::vector<ls::Complex> getFullEigenValues();

    /**
     * Returns the eigenvalues of the reduced jacobian.
     *
     * If the eigenvalues are all real, this returns a N x 1 matrix,
     * if complex, returns an N x 2 matrix where the first column is the
     * real values and the second is the imaginary part.
     */
    std::vector<ls::Complex> getReducedEigenValues();


    ls::DoubleMatrix getLinkMatrix();

    /**
     * get the reduced stochiometry matrix. If conservation conversion is enabled,
     * this is the matrix that coresponds to the independent species.
     *
     * A synonym for getReducedStoichiometryMatrix().
     */
    ls::DoubleMatrix getNrMatrix();


    /**
     * Returns the K matrix (right nullspace of Nr)
     * The K matrix has the structure, [I K0]'
     */
    ls::DoubleMatrix getKMatrix();

    /**
     * get the reduced stochiometry matrix. If conservation conversion is enabled,
     * this is the matrix that coresponds to the independent species.
     *
     * A synonym for getNrMatrix().
     */
    ls::DoubleMatrix getReducedStoichiometryMatrix();

    /**
     * Get the stoichiometry matrix that coresponds to the full model, even it
     * it was converted via conservation conversion.
     */
    ls::DoubleMatrix getFullStoichiometryMatrix();


    ls::DoubleMatrix getL0Matrix();


    ls::DoubleMatrix getConservationMatrix();
    ls::DoubleMatrix getUnscaledConcentrationControlCoefficientMatrix();
    ls::DoubleMatrix getScaledConcentrationControlCoefficientMatrix();
    ls::DoubleMatrix getUnscaledFluxControlCoefficientMatrix();
    ls::DoubleMatrix getScaledFluxControlCoefficientMatrix();


    /**
     * returns the list of floating species, but with a "eigen(...)" string
     * wrapped around them.
     */
    std::vector<std::string> getEigenValueIds();

    /**
     * Returns the unscaled elasticity for a named reaction with respect to a
     * named parameter
     */
    double getUnscaledParameterElasticity(const string& reactionName,
            const string& parameterName);


    ls::DoubleMatrix getFrequencyResponse(double startFrequency,
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
     *
     * The steady state solver and whatever options it needs may be specified
     * via the given dictionary. For a list of all available steady state solvers,
     * @see SteadyStateSolverFactory.
     *
     * @param dict a pointer to a dictionary which has the steady state options.
     * May be NULL, in this case the existing options are used.
     */
    double steadyState(const Dictionary* dict = 0);

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



    /******** !!! DEPRECATED INTERNAL METHODS * THESE WILL BE REMOVED!!! **********/
    #if (1) /**********************************************************************/
    /******************************************************************************/

    #ifndef SWIG // deprecated methods not SWIG'ed

    /**
     * @internal
     * @deprecated
     */
    RR_DEPRECATED(int getNumberOfReactions());

    /**
     * @internal
     * @deprecated
     */
    RR_DEPRECATED(double getReactionRate(const int& index));

    /**
     * @internal
     * @deprecated
     */
    RR_DEPRECATED(double getRateOfChange(const int& index));

    /**
     * @internal
     * @deprecated
     */
    RR_DEPRECATED(std::vector<std::string> getRateOfChangeIds());

    /**
     * @internal
     * @deprecated
     */
    RR_DEPRECATED(std::vector<std::string> getConservedMoietyIds());

    /**
     * @internal
     * @deprecated
     */
    RR_DEPRECATED(std::vector<double> getConservedMoietyValues());

    /**
     * @internal
     * @deprecated
     */
    RR_DEPRECATED(int getNumberOfCompartments());

    /**
     * @internal
     * @deprecated
     */
    RR_DEPRECATED(void setCompartmentByIndex(const int& index, const double& value));

    /**
     * @internal
     * @deprecated
     */
    RR_DEPRECATED(double getCompartmentByIndex(const int& index));

    /**
     * @internal
     * @deprecated
     */
    RR_DEPRECATED(std::vector<std::string> getCompartmentIds());

    /**
     * @internal
     * @deprecated
     */
    RR_DEPRECATED(int getNumberOfBoundarySpecies());

    /**
     * @internal
     * @deprecated
     */
    RR_DEPRECATED(void setBoundarySpeciesByIndex(const int& index, const double& value));

    /**
     * @internal
     * @deprecated
     */
    RR_DEPRECATED(double getBoundarySpeciesByIndex(const int& index));

    /**
     * @internal
     * @deprecated
     */
    RR_DEPRECATED(std::vector<double> getBoundarySpeciesConcentrations());

    /**
     * @internal
     * @deprecated
     */
    RR_DEPRECATED(void setBoundarySpeciesConcentrations(const std::vector<double>& values));

    /**
     * @internal
     * @deprecated
     */
    RR_DEPRECATED(std::vector<std::string> getBoundarySpeciesIds());

    /**
     * @internal
     * @deprecated
     */
    RR_DEPRECATED(int getNumberOfFloatingSpecies());

    /**
     * @internal
     * @deprecated
     */
    RR_DEPRECATED(double getFloatingSpeciesByIndex(int index));

    /**
     * @internal
     * @deprecated
     */
    RR_DEPRECATED(void setFloatingSpeciesByIndex(int index, double value));

    /**
     * @internal
     * @deprecated
     */
    RR_DEPRECATED(std::vector<double> getFloatingSpeciesConcentrations());

    /**
     * @internal
     * @deprecated
     */
    RR_DEPRECATED(std::vector<double> getFloatingSpeciesInitialConcentrations());

    /**
     * @internal
     * @deprecated
     */
    RR_DEPRECATED(void setFloatingSpeciesConcentrations(const std::vector<double>& values));

    /**
     * @internal
     * @deprecated
     */
    RR_DEPRECATED(void setFloatingSpeciesInitialConcentrationByIndex(const int& index,
            const double& value));

    /**
     * @internal
     * @deprecated
     */
    RR_DEPRECATED(void setFloatingSpeciesInitialConcentrations(const std::vector<double>& values));

    /**
     * @internal
     * @deprecated
     */
    RR_DEPRECATED(std::vector<std::string> getFloatingSpeciesIds());

    /**
     * @internal
     * @deprecated
     */
    RR_DEPRECATED(std::vector<std::string> getFloatingSpeciesInitialConditionIds());

    /**
     * @internal
     * @deprecated use ExecutableModel::getNumGlobalParameters
     */
    RR_DEPRECATED(int getNumberOfGlobalParameters());

    /**
     * @internal
     * @deprecated use ExecutableModel::setGlobalParameterValues
     */
    RR_DEPRECATED(void setGlobalParameterByIndex(const int index, const double value));

    /**
     * @internal
     * @deprecated use ExecutableModel::getGlobalParameterValues
     */
    RR_DEPRECATED(double getGlobalParameterByIndex(const int& index));

    /**
     * @internal
     * @deprecated use ExecutableModel::getGlobalParameterValues
     */
    RR_DEPRECATED(std::vector<double> getGlobalParameterValues());

    /**
     * @internal
     * @deprecated use ExecutableModel::getGlobalParameterIds
     */
    RR_DEPRECATED(std::vector<std::string> getGlobalParameterIds());

    /**
     * @internal
     * @deprecated
     */
    void evalModel();

    /**
     * @internal
     * @deprecated
     */
    RR_DEPRECATED(int getNumberOfDependentSpecies());

    /**
     * @internal
     * @deprecated
     */
    RR_DEPRECATED(int getNumberOfIndependentSpecies());


    /**
     * @internal
     * @deprecated, use ExecutableModel::getReactionRates
     */
    RR_DEPRECATED(std::vector<double> getReactionRates());

    /**
     * @internal
     * @deprecated
     * returns a list of reaction ids obtained from
     * ExecutableModel::getReactionId
     */
    RR_DEPRECATED(std::vector<std::string> getReactionIds());

    /**
     * @internal
     * @deprecated
     *
     * C backend only
     * set the location where the ModelGenerator creates temporary files, such
     * as shared libraries.
     */
    void setTempDir(const std::string& folder);

    /**
     * @internal
     * @deprecated
     *
     * legacy C backend only
     * get the ModelGenerator's temporary file directory.
     */
    std::string getTempDir();

    #endif // #ifndef SWIG


    /******** !!! DEPRECATED INTERNAL METHODS * THESE WILL BE REMOVED!!! **********/
    #endif  /**********************************************************************/
    /******************************************************************************/


    virtual HRESULT hello(const char* foo);
    virtual HRESULT getScalarSystem(IScalarSystem **scalarSystem);

private:


    int createDefaultSteadyStateSelectionList();
    int createDefaultTimeCourseSelectionList();

    /**
     * copies the current selection values into the n'th row of the
     * given matrix
     */
    void getSelectedValues(ls::DoubleMatrix& results, int nRow,
            double currentTime);

    /**
     * copies the current selection values into the given vector.
     */
    void getSelectedValues(std::vector<double> &results, double currentTime);

    bool populateResult();


    double getNthSelectedOutput(unsigned index, double currentTime);

    enum VariableType
    {
        vtSpecies = 0, vtFlux
    };

    double getVariableValue(const VariableType variableType,
            const int variableIndex);

    /**
     * the LibStruct is normally null, only created on demand here.
     */
    ls::LibStructural* getLibStruct();

    /**
     * If the specified integrator does not exist, create it, and point the
     * integrator pointer to it.
     */
    void updateIntegrator();

    bool createDefaultSelectionLists();

    /**
     * creates a selection list from the amounts / conc / variables ivars of the
     * SimulationOptions struct.
     */
    int createTimeCourseSelectionList();




    std::vector<SelectionRecord> getSelectionList();

    /**
     * The simulateOptions may be changed at any time. This updates
     * the integrators just before they are used with the
     * potentially changed options.
     */
    void updateSimulateOptions();


    enum JacobianMode {
        JACOBIAN_FULL, JACOBIAN_REDUCED
    };

    std::vector< std::complex<double> > getEigenValues(JacobianMode mode);

    /**
     * private implementation class, can only access if inside
     * the implementation file.
     */
    class RoadRunnerImpl* impl;
};

}

#endif // SBMLSOLVER_H_
