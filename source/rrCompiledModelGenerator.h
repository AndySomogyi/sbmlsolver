/*
 * rrCompiledModelGenerator.h
 *
 *  Created on: May 26, 2013
 *      Author: andy
 */

#ifndef RRCOMPILEDMODELGENERATOR_H_
#define RRCOMPILEDMODELGENERATOR_H_

#include "rrModelGenerator.h"
#include "rrModelSharedLibrary.h"
#include "rrCodeBuilder.h"
#include "rrCompiler.h"

namespace rr {

/**
 * Both compiled model generators (C and CSharp) share a lot of functionality,
 * so implement that here.
 */
class RR_DECLSPEC CompiledModelGenerator : public ModelGenerator
{
protected:
    CompiledModelGenerator();
    virtual 				   ~CompiledModelGenerator();

    string                              substituteTerms(const string& reactionName, const string& inputEquation, bool bFixAmounts);
    string                              substituteTerms(const int& numReactions, const string& reactionName, const string& equation);

    //Pure Virtual functions... =====================================
    virtual string                      convertUserFunctionExpression(const string& equation) = 0;
    virtual void                        substituteEquation(const string& reactionName, Scanner& s, CodeBuilder& sb) = 0;
    virtual void                        substituteWords(const string& reactionName, bool bFixAmounts, Scanner& s, CodeBuilder& sb) = 0;
    virtual void                        substituteToken(const string& reactionName, bool bFixAmounts, Scanner& s, CodeBuilder& sb) = 0;
    virtual string                      findSymbol(const string& varName) = 0;
    virtual int                         readFloatingSpecies() = 0;
    virtual int                         readBoundarySpecies() = 0;
    virtual void                        writeOutSymbolTables(CodeBuilder& sb) = 0;
    virtual void                        writeComputeAllRatesOfChange(CodeBuilder& sb, const int& numIndependentSpecies, const int& numDependentSpecies, DoubleMatrix& L0) = 0;
    virtual void                        writeComputeConservedTotals(CodeBuilder& sb, const int& numFloatingSpecies, const int& numDependentSpecies) = 0;
    virtual void                        writeUpdateDependentSpecies(CodeBuilder& sb, const int& numIndependentSpecies, const int& numDependentSpecies, DoubleMatrix& L0) = 0;
    virtual void                        writeUserDefinedFunctions(CodeBuilder& sb) = 0;
    virtual void                        writeResetEvents(CodeBuilder& sb, const int& numEvents) = 0;
    virtual void                        writeSetConcentration(CodeBuilder& sb) = 0;
    virtual void                        writeGetConcentration(CodeBuilder& sb) = 0;
    virtual void                        writeConvertToAmounts(CodeBuilder& sb) = 0;
    virtual void                        writeConvertToConcentrations(CodeBuilder& sb) = 0;
    virtual void                        writeProperties(CodeBuilder& sb) = 0;
    virtual void                        writeAccessors(CodeBuilder& sb) = 0;
    virtual void                        writeOutVariables(CodeBuilder& sb) = 0;
    virtual void                        writeClassHeader(CodeBuilder& sb) = 0;
    virtual void                        writeTestConstraints(CodeBuilder& sb) = 0;
    virtual void                        writeEvalInitialAssignments(CodeBuilder& sb, const int& numReactions) = 0;
    virtual int                         writeComputeRules(CodeBuilder& sb, const int& numReactions) = 0;
    virtual void                        writeComputeReactionRates(CodeBuilder& sb, const int& numReactions) = 0;
    virtual void                        writeEvalEvents(CodeBuilder& sb, const int& numEvents, const int& numFloatingSpecies) = 0;
    virtual void                        writeEvalModel(CodeBuilder& sb, const int& numReactions, const int& numIndependentSpecies, const int& numFloatingSpecies, const int& numOfRules) = 0;
    virtual void                        writeEventAssignments(CodeBuilder& sb, const int& numReactions, const int& numEvents) = 0;
    virtual void                        writeSetParameterValues(CodeBuilder& sb, const int& numReactions) = 0;
    virtual void                        writeSetCompartmentVolumes(CodeBuilder& sb) = 0;
    virtual void                        writeSetBoundaryConditions(CodeBuilder& sb) = 0;
    virtual void                        writeSetInitialConditions(CodeBuilder& sb, const int& numFloatingSpecies) = 0;
    virtual string                      convertCompartmentToC(const string& compartmentName) = 0;
    virtual string                      convertSpeciesToBc(const string& speciesName) = 0;
    virtual string                      convertSpeciesToY(const string& speciesName) = 0;
    virtual string                      convertSymbolToC(const string& compartmentName) = 0;
    virtual string                      convertSymbolToGP(const string& parameterName) = 0;
};

} /* namespace rr */
#endif /* RRCOMPILEDMODELGENERATOR_H_ */
