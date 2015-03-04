/*
 * FBCExecutableModel.cpp
 *
 *  Created on: Feb 22, 2015
 *      Author: andy
 */

#include <fbc/FBCExecutableModel.h>


#include <sbml/packages/fbc/extension/FbcExtension.h>
#include <sbml/packages/fbc/extension/FbcModelPlugin.h>
#include <sbml/packages/fbc/extension/FbcSBMLDocumentPlugin.h>
#include <sbml/packages/fbc/extension/FbcSpeciesPlugin.h>

#include "llvm/LLVMModelDataSymbols.h"

#include <list>

#include <limits>

#include <coin/CoinLpIO.hpp>

#include "rrLogger.h"

typedef std::list<std::string> StringList;

using namespace libsbml;

namespace rr
{
namespace fbc
{

FBCExecutableModel::FBCExecutableModel(const std::string sbml, const rr::Dictionary* dict)
{
	doc = cxx11_ns::unique_ptr<SBMLDocument>(readSBMLFromString(sbml.c_str()));

	if(!doc) {
		// TODO throw exception
	}

	FbcModelPlugin *plugin = (FbcModelPlugin*)(doc->getModel()->getPlugin("fbc"));

	if(!plugin) {
		// TODO throw exception
	}

	plugin->getListOfFluxBounds();

	Model *model = doc->getModel();


	// init the species
	const ListOfSpecies* species = model->getListOfSpecies();
	for(uint i = 0; i < species->size(); ++i) {
		const Species* s = species->get(i);

		// TODO need to check for rules
		if(s->getBoundaryCondition()) {
			uint size = boundarySpeciesMap.size();
			boundarySpeciesMap[s->getId()] = size;
		} else {
			uint size = floatingSpeciesMap.size();
			floatingSpeciesMap[s->getId()] = size;
		}
	}


	std::vector<int> stoichRowIndx;
	std::vector<int> stoichColIndx;
	std::vector<double> stoichValues;

	const ListOfReactions *reactions = model->getListOfReactions();
	int nreact = reactions->size();

	for(unsigned i = 0; i < nreact; ++i) {
		const Reaction* r = reactions->get(i);
		uint rid = reactionsMap.size();
		reactionsMap[r->getId()] = rid;


		{
			const ListOfSpeciesReferences *products = r->getListOfProducts();
			for(unsigned j = 0; j < products->size(); ++j) {

				const SpeciesReference *ref =
						dynamic_cast<const SpeciesReference*>(products->get(j));

				if(!ref) {
					// TODO exception
				}

				uint sid = floatingSpeciesMap[ref->getSpecies()];

				stoichRowIndx.push_back(sid);
				stoichColIndx.push_back(rid);

				std::cout << "prod stoich, row: " << sid << ", rid: " << rid << ", value: " << ref->getStoichiometry() << "\n";
				stoichValues.push_back(ref->getStoichiometry());
			}
		}


		{
			const ListOfSpeciesReferences *reactants = r->getListOfProducts();
			for(unsigned j = 0; j < reactants->size(); ++j) {

				const SpeciesReference *ref =
						dynamic_cast<const SpeciesReference*>(reactants->get(j));

				if(!ref) {
					// TODO exception
				}

				uint sid = floatingSpeciesMap[ref->getSpecies()];

				stoichRowIndx.push_back(sid);
				stoichColIndx.push_back(rid);

				std::cout << "reac stoich, row: " << sid << ", rid: " << rid << ", value: " << ref->getStoichiometry() << "\n";
				stoichValues.push_back(-1 * ref->getStoichiometry());
			}
		}
	} // reactions





	/*
	 * <fbc:listOfObjectives fbc:activeObjective="obj1">
     *    <fbc:objective fbc:id="obj1" fbc:type="maximize">
     *       <fbc:listOfFluxObjectives>
     *          <fbc:fluxObjective fbc:reaction="R1" fbc:coefficient="1"/>
     *          <fbc:fluxObjective fbc:reaction="R2" fbc:coefficient="2"/>
     *       </fbc:listOfFluxObjectives>
     *    </fbc:objective>
     * </fbc:listOfObjectives>
	 */




	// create the objective coeficients, can have multiple objective, so
	// just add them???
	// the active objective takes prescidence
	// TODO, is this right
	std::vector<double> objective(reactionsMap.size());

	{
		const ListOfObjectives* objectives = plugin->getListOfObjectives();

		std::string active = objectives->getActiveObjective();

		for (uint i = 0; i < objectives->size(); ++i) {
			const Objective* o = objectives->get(i);

			ObjectiveType_t type = o->getObjectiveType();

			// todo check for OBJECTIVE_TYPE_UNKNOWN

			double scale = (type == OBJECTIVE_TYPE_MAXIMIZE ? 1 : -1);

			// active takes precidence
			// TODO, is this correct way to do this?
			if(o->getId() == active) {
				scale *= 2.0;
			}

			const ListOfFluxObjectives *fluxObjectives = o->getListOfFluxObjectives();

			for(uint j = 0; j < fluxObjectives->size(); ++j) {
				const FluxObjective* fluxObj = fluxObjectives->get(j);

				uint rid = reactionsMap[fluxObj->getReaction()];
				double coef = fluxObj->getCoefficient();

				objective[rid] += scale * coef;
			}
		}
	}


	/*
	 * <fbc:listOfFluxBounds>
 <fbc:fluxBound fbc:id="R1b" fbc:reaction="R1" fbc:operation="greaterEqual" fbc:value="1.2"/>
 <fbc:fluxBound fbc:id="R2b" fbc:reaction="R2" fbc:operation="lessEqual" fbc:value="-1.2"/>
 <fbc:fluxBound fbc:id="R3b" fbc:reaction="R3" fbc:operation="greaterEqual" fbc:value="-INF"/>
 <fbc:fluxBound fbc:id="R4b" fbc:reaction="R4" fbc:operation="lessEqual" fbc:value="INF"/>
 <fbc:fluxBound fbc:id="R5b" fbc:reaction="R5" fbc:operation="equal" fbc:value="1"/>
</fbc:listOfFluxBounds>
	 */

	std::vector<double> lbound(reactionsMap.size(), COIN_DBL_MIN);
	std::vector<double> ubound(reactionsMap.size(), COIN_DBL_MAX);

	{
		const ListOfFluxBounds* fluxBounds = plugin->getListOfFluxBounds();
		for(uint i = 0; i < fluxBounds->size(); ++i) {
			const FluxBound* fb = fluxBounds->get(i);

			uint rid = reactionsMap[fb->getReaction()];
			double value = fb->getValue();
			double epsilon = std::numeric_limits<double>::epsilon();

			switch(fb->getFluxBoundOperation()) {
			case FLUXBOUND_OPERATION_LESS_EQUAL:
				ubound[rid] = value;
				break;
			case FLUXBOUND_OPERATION_GREATER_EQUAL:
				lbound[rid] = value;
				break;
			case FLUXBOUND_OPERATION_LESS:
				ubound[rid] = value - epsilon;
				break;
			case FLUXBOUND_OPERATION_GREATER:
				lbound[rid] = value + epsilon;
				break;
			case FLUXBOUND_OPERATION_EQUAL:
				lbound[rid] = value;
				ubound[rid] = value;
				break;
			case FLUXBOUND_OPERATION_UNKNOWN:
				// TODO throw exception
				break;
			}
		}
	}

	std::vector<double> rowBounds(floatingSpeciesMap.size(), 0);

	matrix = CoinPackedMatrix(true, &stoichRowIndx[0], &stoichColIndx[0], &stoichValues[0], stoichValues.size());

	for(int i = 0; i < matrix.getNumRows(); ++i) {
		std::cout << "row " << i << ": ";
		for(int j = 0; j < matrix.getNumRows(); ++j) {
			std::cout << matrix.getCoefficient(i, j) << ",    ";
		}
		std::cout << "\n";
	}



	lpModel.loadProblem(matrix, &lbound[0], &ubound[0], &objective[0],
			&rowBounds[0], &rowBounds[0]);

}

FBCExecutableModel::~FBCExecutableModel()
{
}

std::string FBCExecutableModel::getModelName()
{
    return "";
}

void FBCExecutableModel::setTime(double _time)
{
}

double FBCExecutableModel::getTime()
{
    return 0;
}

void FBCExecutableModel::reset()
{
}

int FBCExecutableModel::getNumDepFloatingSpecies()
{
    return 0;
}

int FBCExecutableModel::getNumFloatingSpecies()
{
    return 0;
}

int FBCExecutableModel::getFloatingSpeciesIndex(const std::string& eid)
{
    return 0;
}

std::string FBCExecutableModel::getFloatingSpeciesId(int index)
{
    return "";
}

int FBCExecutableModel::getNumIndFloatingSpecies()
{
    return 0;
}

int FBCExecutableModel::getFloatingSpeciesAmounts(int len, const int* indx,
		double* values)
{
    return 0;
}

int FBCExecutableModel::setFloatingSpeciesAmounts(int len, const int* indx,
		const double* values)
{
    return 0;
}

int FBCExecutableModel::getFloatingSpeciesAmountRates(int len, const int* indx,
		double* values)
{
    return 0;
}

int FBCExecutableModel::getFloatingSpeciesConcentrationRates(int len,
		const int* indx, double* values)
{
    return 0;
}

int FBCExecutableModel::getFloatingSpeciesConcentrations(int len,
		const int* indx, double* values)
{
    return 0;
}

int FBCExecutableModel::setFloatingSpeciesConcentrations(int len,
		const int* indx, const double* values)
{
    return 0;
}

int FBCExecutableModel::setFloatingSpeciesInitConcentrations(int len,
		const int* indx, const double* values)
{
    return 0;
}

int FBCExecutableModel::getFloatingSpeciesInitConcentrations(int len,
		const int* indx, double* values)
{
    return 0;
}

int FBCExecutableModel::setFloatingSpeciesInitAmounts(int len, const int* indx,
		const double* values)
{
    return 0;
}

int FBCExecutableModel::getFloatingSpeciesInitAmounts(int len, const int* indx,
		double* values)
{
    return 0;
}

int FBCExecutableModel::getNumBoundarySpecies()
{
    return 0;
}

int FBCExecutableModel::getBoundarySpeciesIndex(const std::string& eid)
{
    return 0;
}

std::string FBCExecutableModel::getBoundarySpeciesId(int index)
{
    return "";
}

int FBCExecutableModel::getBoundarySpeciesAmounts(int len, const int* indx,
		double* values)
{
    return 0;
}

int FBCExecutableModel::getBoundarySpeciesConcentrations(int len,
		const int* indx, double* values)
{
    return 0;
}

int FBCExecutableModel::setBoundarySpeciesConcentrations(int len,
		const int* indx, const double* values)
{
    return 0;
}

int FBCExecutableModel::getNumGlobalParameters()
{
    return 0;
}

int FBCExecutableModel::getGlobalParameterIndex(const std::string& eid)
{
    return 0;
}

std::string FBCExecutableModel::getGlobalParameterId(int index)
{
    return "";
}

int FBCExecutableModel::getGlobalParameterValues(int len, const int* indx,
		double* values)
{
    return 0;
}

int FBCExecutableModel::setGlobalParameterValues(int len, const int* indx,
		const double* values)
{
    return 0;
}

int FBCExecutableModel::getNumCompartments()
{
    return 0;
}

int FBCExecutableModel::getCompartmentIndex(const std::string& eid)
{
    return 0;
}

std::string FBCExecutableModel::getCompartmentId(int index)
{
    return "";
}

int FBCExecutableModel::getCompartmentVolumes(int len, const int* indx,
		double* values)
{
    return 0;
}

int FBCExecutableModel::setCompartmentVolumes(int len, const int* indx,
		const double* values)
{
    return 0;
}

int FBCExecutableModel::setCompartmentInitVolumes(int len, const int* indx,
		const double* values)
{
    return 0;
}

int FBCExecutableModel::getCompartmentInitVolumes(int len, const int* indx,
		double* values)
{
    return 0;
}

void FBCExecutableModel::getIds(int types, std::list<std::string>& ids)
{
}

int FBCExecutableModel::getSupportedIdTypes()
{
    return 0;
}

double FBCExecutableModel::getValue(const std::string& id)
{
    return 0;
}

void FBCExecutableModel::setValue(const std::string& id, double value)
{
}

int FBCExecutableModel::getStoichiometryMatrix(int* rows, int* cols,
		double** data)
{
    return 0;
}

double FBCExecutableModel::getStoichiometry(int speciesIndex, int reactionIndex)
{
    return 0;
}

int FBCExecutableModel::getNumConservedMoieties()
{
    return 0;
}

int FBCExecutableModel::getConservedMoietyIndex(const std::string& eid)
{
    return 0;
}

std::string FBCExecutableModel::getConservedMoietyId(int index)
{
    return "";
}

int FBCExecutableModel::getConservedMoietyValues(int len, const int* indx,
		double* values)
{
    return 0;
}

int FBCExecutableModel::setConservedMoietyValues(int len, const int* indx,
		const double* values)
{
    return 0;
}

int FBCExecutableModel::getNumRateRules()
{
    return 0;
}

int FBCExecutableModel::getNumReactions()
{
    return 0;
}

int FBCExecutableModel::getReactionIndex(const std::string& eid)
{
    return 0;
}

std::string FBCExecutableModel::getReactionId(int index)
{
    return "";
}

int FBCExecutableModel::getReactionRates(int len, const int* indx,
		double* values)
{
    return 0;
}

void FBCExecutableModel::getRateRuleValues(double* rateRuleValues)
{
}

std::string FBCExecutableModel::getStateVectorId(int index)
{
    return "";
}

int FBCExecutableModel::getStateVector(double* stateVector)
{
    return 0;
}

int FBCExecutableModel::setStateVector(const double* stateVector)
{
    return 0;
}

void FBCExecutableModel::getStateVectorRate(double time, const double* y,
		double* dydt)
{
}

void FBCExecutableModel::testConstraints()
{
}

std::string FBCExecutableModel::getInfo()
{
    return "";
}

void FBCExecutableModel::print(std::ostream& stream)
{
}

int FBCExecutableModel::getNumEvents()
{
    return 0;
}

int FBCExecutableModel::getEventTriggers(int len, const int* indx,
		unsigned char* values)
{
    return 0;
}

int FBCExecutableModel::applyEvents(double timeEnd,
		const unsigned char* previousEventStatus, const double* initialState,
		double* finalState)
{
    return 0;
}

void FBCExecutableModel::getEventRoots(double time, const double* y,
		double* gdot)
{
}

double FBCExecutableModel::getNextPendingEventTime(bool pop)
{
    return 0;
}

int FBCExecutableModel::getPendingEventSize()
{
    return 0;
}

void FBCExecutableModel::resetEvents()
{
}

int FBCExecutableModel::getEventIndex(const std::string& eid)
{
    return 0;
}

std::string FBCExecutableModel::getEventId(int index)
{
    return "";
}

void FBCExecutableModel::setEventListener(int index,
		EventListenerPtr eventHandler)
{
}

EventListenerPtr FBCExecutableModel::getEventListener(int index)
{
    return 0;
}

double FBCExecutableModel::getFloatingSpeciesAmountRate(int index,
		const double* reactionRates)
{
    return 0;
}

void FBCExecutableModel::reset(int options)
{
}

void FBCExecutableModel::setRandomSeed(int64_t longLongInt)
{
}

int64_t FBCExecutableModel::getRandomSeed()
{
    return 0;
}

double FBCExecutableModel::getRandom()
{
    return 0;
}

uint32_t FBCExecutableModel::getFlags() const
{
    return 0;
}

void FBCExecutableModel::setFlags(uint32_t unsignedInt)
{
}

void FBCExecutableModel::writeLP(const std::string& fname)
{
	CoinLpIO io;

	/*
	 * void setLpDataWithoutRowAndColNames(
			      const CoinPackedMatrix& m,
			      const double* collb, const double* colub,
			      const double* obj_coeff,
			      const char* integrality,
			      const double* rowlb, const double* rowub);
	 */

	const double* collb = lpModel.getColLower();
	const double* colub = lpModel.getColUpper();
	const double* obj = lpModel.getObjCoefficients();
	const double* rowlb = lpModel.getRowLower();
	const double* rowub = lpModel.getRowUpper();


	io.setLpDataWithoutRowAndColNames(matrix,
		      collb, colub,
		      obj,
		      NULL,
		      rowlb, rowub);



	io.writeLp(fname.c_str(), true);
}

} /* namespace fbc */
} /* namespace rr */
