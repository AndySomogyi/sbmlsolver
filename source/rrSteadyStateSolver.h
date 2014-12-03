#ifndef rrSteadyStateSolverH
#define rrSteadyStateSolverH
#include "rrExporter.h"
#include "Configurable.h"
#include "rrExecutableModel.h"
#include "Dictionary.h"
#include <vector>


namespace rr
{

/**
 * place holder for now.
 */
struct SteadyStateOptions
{
};

/**
 * Interface that all steady state solvers must implement.
 *
 *
 */
class RR_DECLSPEC SteadyStateSolver : public Configurable
{

public:

    virtual ~SteadyStateSolver() {};
    virtual double solve(const std::vector<double>& yin) = 0;


    /**
     * factory method to create a new steady state solver.
     */
    static SteadyStateSolver* New(const SteadyStateOptions*, ExecutableModel*);
};

/**
 * Factory class that creates steady state solvers,
 * and lists all available steady state solvers and their
 * supported options.
 */
class RR_DECLSPEC SteadyStateSolverFactory
{
public:

    /**
     * factory method to create a new steady state solver.
     */
    static SteadyStateSolver* New(const Dictionary* doct, ExecutableModel* model);

    /**
     * The list of steady state solver names that are currently implemented.
     */
    static std::vector<std::string> getSteadyStateNames();

    /**
     * list of options that each steady state solver supports.
     *
     * Each dictionary is populated with the default values that
     * the solver will be created with.
     *
     * @returns a list of borrowed Dictionary references.
     */
    static std::vector<const Dictionary*> getSteadyStateOptions();

    /**
     * Get a dictionary of options for a specific solver.
     *
     * @param name: the name of a valid solver
     * @returns a borrowed reference to a Dictionary
     * @throws std::exception if the argument is not a valid integrator.
     */
    static const Dictionary* getSteadyStateOptions(const std::string& name);

};

}
#endif
