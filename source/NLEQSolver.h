// == PREAMBLE ================================================

// * Licensed under the Apache License, Version 2.0; see README

// == FILEDOC =================================================

/** @file NLEQSolver.h
* @author ETS, WBC, JKM
* @date 08/03/2015
* @copyright Apache License, Version 2.0
* @brief Provides an interface to NLEQ
**/

#ifndef rrNLEQSolverH
#define rrNLEQSolverH
#include <vector>
#include "Solver.h"
#include "rrExecutableModel.h"
using std::vector;

namespace rr
{

/**
 * @internal
 */
class RR_DECLSPEC NLEQSolver : public Solver
{

public:
    /**
     * Creates a new Instance of NLEQ for the given Model
     */
    NLEQSolver(ExecutableModel *_model = NULL);
    ~NLEQSolver();

	void loadConfigSettings();
	void loadSBMLSettings(const std::string& filename);

	/**
	* @author WBC
	* @brief Get the name for this Solver
	* @note Delegates to @ref getName
	*/
	std::string getSolverName() const;

	/**
	* @author JKM
	* @brief Get the name for this Solver
	*/
	static std::string getName();

	/**
	* @author WBC
	* @brief Get the description for this Solver
	* @note Delegates to @ref getDescription
	*/
	std::string getSolverDescription() const;

	/**
	* @author JKM
	* @brief Get the description for this Solver
	*/
	static std::string getDescription();

	/**
	* @author WBC
	* @brief Get the hint for this Solver
	* @note Delegates to @ref getHint
	*/
	std::string getSolverHint() const;

	/**
	* @author JKM
	* @brief Get the hint for this Solver
	*/
	static std::string getHint();

	// ** Getters / Setters ************************************************

	/**
	* @author WBC, ETS, MTK
	* @brief Always deterministic for CVODE
	*/
	SolverMethod getSolverMethod() const;

	/**
	* @author WBC, ETS, MTK
	* @brief Sets the value of an Solver setting (e.g. maximum_iterations)
	*/
	void setValue(std::string setting, const Variant& value);


	// ** Solver routines
	double solve(const vector<double>& yin);

private:
	ExecutableModel *model; // Model generated from the SBML. Static so we can access it from standalone function
};
}

#endif
