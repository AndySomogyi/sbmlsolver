/*
 * GillespieIntegrator.h
 *
 *  Created on: Apr 23, 2014
 *      Author: andy
 */

#ifndef GILLESPIEINTEGRATOR_H_
#define GILLESPIEINTEGRATOR_H_

#include "Integrator.h"
#include "rrRoadRunnerOptions.h"
#include "rrExecutableModel.h"
#include "tr1proxy/rr_random.h"


namespace rr
{

class ExecutableModel;

class GillespieIntegrator: public Integrator
{
public:
    GillespieIntegrator(ExecutableModel* model);
    virtual ~GillespieIntegrator();

	std::string getIntegratorName() const;
	std::string getIntegratorDescription() const;
	std::string getIntegratorHint() const;
	IntegrationMethod getIntegrationMethod() const;
	void setValue(std::string setting, const Variant& value);

	double integrate(double t0, double tf);
	void restart(double timeStart);
	IntegratorListenerPtr getListener();
	void setListener(IntegratorListenerPtr);

private:
    ExecutableModel *model;
    cxx11_ns::mt19937 engine;
	//unsigned long seed;
    double timeScale;
    double stoichScale;
    int nReactions;
    int floatingSpeciesStart;		// starting index of floating species
    double* reactionRates;
    double* reactionRatesBuffer;
    int stateVectorSize;
    double* stateVector;
    double* stateVectorRate;
    // m rows x n cols
    // offset = row*NUMCOLS + column
    int stoichRows;
    int stoichCols;
    double* stoichData;

    double urand();
	void setEngineSeed(unsigned long seed);
    unsigned long getSeed() const;

	inline double getStoich(uint species, uint reaction)
	{
		return stoichData[species * stoichCols + reaction];
	}
};

} /* namespace rr */

#endif /* GILLESPIEINTEGRATOR_H_ */
