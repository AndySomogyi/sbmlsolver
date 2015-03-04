/*
 * LPSolver.h
 *
 *  Created on: Feb 22, 2015
 *      Author: andy
 */

#ifndef SOURCE_FBC_LPSOLVER_H_
#define SOURCE_FBC_LPSOLVER_H_

#include <list>

namespace rr
{
namespace fbc
{

class LPSolver
{
public:
	LPSolver();
	virtual ~LPSolver();

	LPSolver(const int * rowIndices,
		     const int * colIndices,
		     const double * elements,
		     unsigned n);


};

} /* namespace fbc */
} /* namespace rr */

#endif /* SOURCE_FBC_LPSOLVER_H_ */
