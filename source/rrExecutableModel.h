#ifndef rrExecutableModel_H_
#define rrExecutableModel_H_
#include <list>
#include <string>
#include <vector>

#include "rrOSSpecifics.h"
#include "rrModelData.h"
#include "rrSymbolList.h"
#include "rrStringList.h"

namespace rr
{

using std::string;
using std::vector;

class CvodeInterface;

/**
 * The ExecutableModel interface provides a way to access an
 * sbml model that was compiled, JIT'd or interpreted
 * as executable (runnable) module.
 */
class RR_DECLSPEC ExecutableModel
{
public:
	virtual string									getModelName() = 0;
	virtual void                                    assignCVodeInterface(CvodeInterface* cvodeI) = 0;
	virtual void                                    setTime(double _time) = 0;
	virtual double                                  getTime() = 0;
	virtual ModelData&								getModelData() = 0;

	virtual CvodeInterface*                         getCvodeInterface() = 0;

	// functions --------------------------------------------------------
	virtual int                                     getNumIndependentVariables() = 0;
	virtual int                                     getNumDependentVariables() = 0;
	virtual int                                     getNumTotalVariables() = 0;
	virtual int                                     getNumBoundarySpecies() = 0;
	virtual int                                     getNumGlobalParameters() = 0;
	virtual int                                     getNumCompartments() = 0;
	virtual int                                     getNumReactions() = 0;
	virtual int                                     getNumRules() = 0;
	virtual int                                     getNumEvents() = 0;
	virtual void                                    computeEventPriorites() = 0;
	virtual void                                    setConcentration(int index, double value) = 0;
	virtual void                                    computeReactionRates(double time, double* y) = 0;


	virtual void                                    setCompartmentVolumes() = 0;
	virtual int                                     getNumLocalParameters(int reactionId) = 0;
	virtual void                                    computeRules(vector<double>& _y) = 0;
	virtual void                                    computeRules(double* ay, int size) = 0;
	virtual void                                    initializeInitialConditions() = 0;
	virtual void                                    setParameterValues() = 0;
	virtual void                                    setBoundaryConditions() = 0;
	virtual void                                    setInitialConditions() = 0;
	virtual void                                    evalInitialAssignments() = 0;

	virtual void                                    convertToAmounts() = 0;
	virtual void                                    computeConservedTotals() = 0;
	virtual double                                  getConcentration(int index) = 0;

	//Access dll data
	virtual vector<double>                          getCurrentValues() = 0;
	virtual double                                  getAmounts(const int& i) = 0;
	virtual void                                    initializeRates() = 0;
	virtual void                                    assignRates() = 0;
	virtual void                                    assignRates(vector<double>& rates) = 0;
	virtual void                                    convertToConcentrations() = 0;
	virtual void                                    updateDependentSpeciesValues(double* _y) = 0;
	virtual void                                    computeAllRatesOfChange() = 0;
	virtual void                                    evalModel(const double& time,  const vector<double>& y) = 0;
	virtual void                                    evalEvents(const double& time, const vector<double>& y) = 0;
	virtual void                                    resetEvents() = 0;
	virtual void                                    testConstraints() = 0;
	virtual void                                    initializeRateRuleSymbols() = 0;
	virtual string									getInfo() = 0;

	virtual SymbolList                              &getReactions() = 0;
	virtual SymbolList                              &getGlobalParameters() = 0;
	virtual SymbolList                              &getBoundarySpecies() = 0;
	virtual SymbolList                              &getCompartments() = 0;
	virtual SymbolList                              &getConservations() = 0;
	virtual SymbolList                              &getFloatingSpeciesAmounts() = 0;
	virtual SymbolList                              &getFloatingSpeciesConcentrations() = 0;

    virtual StringList                              getCompartmentNames() = 0;
    virtual StringList                              getConservationNames() = 0;
    virtual StringList                              getGlobalParameterNames() = 0;
    virtual StringList                              getReactionNames() = 0;
    virtual StringList                              getFloatingSpeciesConcentrationNames() = 0;    //Just returns the Ids...!
    virtual StringList                              getBoundarySpeciesNames() = 0;

	/**
	 * need a virtual destructor as object implementing this interface
	 * can be deleted directly, i.e.
	 * ExecutableModel *p = createModel(...);
	 * delete p;
	 */
	virtual											~ExecutableModel() {};
};

/**
 * zero out the memory occupied by a ModelData struct, equivalent to
 * memset(&data, 0, sizeof(ModelData));
 *
 * performs NO allocaation of memory.
 *
 */
void RR_DECLSPEC initModelData(ModelData &data);

/**
 * Allocate memory for all the data buffers in a ModelData structure,
 * based on what each buffer size variable indicates, i.e.
 * if data.ySize is 10, data.y will now point to a length 10 double array.
 * The data.modelName field will be a newly allocated null terminated c string.
 *
 * The ModelData structure buffers should be freed with freeModelDataBuffers.
 * TODO: figure out if
 * char**                              variableTable;
 * char**                              boundaryTable;
 * char**                              globalParameterTable;
 *
 * need allocation or freeing.
 */
void RR_DECLSPEC allocModelDataBuffers(ModelData &data, const string& modelName);

/**
 * Free all the data referenced by the ModelData pointers.
 * Does NOT free the ModelData itself, ONLY the buffer data.
 */
void RR_DECLSPEC freeModelDataBuffers(ModelData &data);


}
#endif
