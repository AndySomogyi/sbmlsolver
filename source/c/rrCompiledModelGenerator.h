/*
 * rrCompiledModelGenerator.h
 *
 *  Created on: May 26, 2013
 *      Author: andy
 */

#ifndef RRCOMPILEDMODELGENERATOR_H_
#define RRCOMPILEDMODELGENERATOR_H_


#include "rrModelSharedLibrary.h"
#include "rrCodeBuilder.h"
#include "rrScanner.h"
#include "rrNOMSupport.h"
#include "rrSymbol.h"
#include "rrModelSymbols.h"
#include "rr-libstruct/lsMatrix.h"
#include "rr-libstruct/lsLibStructural.h"

// TODO UGLY!!!!
// using namespace in header!
using namespace ls;

namespace rr {

/**
 * Both compiled model generators (C and CSharp) share a lot of functionality,
 * so implement that here.
 */
class RR_DECLSPEC CompiledModelGenerator
{
protected:
    CompiledModelGenerator();
    virtual    ~CompiledModelGenerator();

    string                              substituteTerms(const string& reactionName, const string& inputEquation, bool bFixAmounts);
    string                              substituteTerms(const int& numReactions, const string& reactionName, const string& equation);

    virtual string                      convertUserFunctionExpression(const string& equation) = 0;
    virtual void                        substituteEquation(const string& reactionName, Scanner& s, CodeBuilder& sb) = 0;
    virtual void                        substituteWords(const string& reactionName, bool bFixAmounts, Scanner& s, CodeBuilder& sb) = 0;
    virtual void                        substituteToken(const string& reactionName, bool bFixAmounts, Scanner& s, CodeBuilder& sb) = 0;
    virtual string                      findSymbol(const string& varName) = 0;
    virtual void                        writeComputeAllRatesOfChange(CodeBuilder& sb, const int& numIndependentSpecies,
                                                                        const int& numDependentSpecies, ls::DoubleMatrix& L0) = 0;

    virtual void                        writeComputeConservedTotals(CodeBuilder& sb, const int& numFloatingSpecies,
                                                                        const int& numDependentSpecies) = 0;

    virtual void                        writeUpdateDependentSpecies(CodeBuilder& sb, const int& numIndependentSpecies,
                                                                        const int& numDependentSpecies, ls::DoubleMatrix& L0) = 0;

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

    void                                reset();

    int                                 numAdditionalRates();        //this variable is the size of moMapRateRule

    /**
     * Refernce to libstruct library
     * this are set by createModel, and for the time being remain after createModel
     * completes.
     */
    ls::LibStructural*                      mLibStruct;

    /**
     * Object that provide some wrappers and new "NOM" functions.
     * this are set by createModel, and for the time being remain after createModel
     * completes.
     */
    NOMSupport*                         mNOM;

    bool                                mComputeAndAssignConsevationLaws;

    const string                        mFixAmountCompartments;

    StringList                          mWarnings;

    /**
     * get various information about the model in a user displayable format.
     */
    virtual string                      getInfo();

    /**
     * creates a new AST node
     */
    static ASTNode*                     cleanEquation(ASTNode* ast);
    static string                       cleanEquation(const string& equation);

    ls::DoubleMatrix*                   initializeL0(int& nrRows, int& nrCols);
    bool                                expressionContainsSymbol(ASTNode* ast, const string& symbol);
    bool                                expressionContainsSymbol(const string& expression, const string& symbol);
    const Symbol*                       getSpecies(const string& id);
    int                                 readGlobalParameters();
    void                                readLocalParameters(const int& numReactions,  vector<int>& localParameterDimensions, int& totalLocalParmeters);
    int                                 readCompartments();

    string                              writeDouble(const double& value, const string& format = "%G");

    /**
     * hold all the symbolic (AKA metadata) information in the model
     * The idea is that all the memebers of this class will be const, and the entire
     * thing will be created anew each time createModel is called.
     *
     * This thing will creted in one shot, then all the code building will access it.
     */
    ModelSymbols                         ms;
};

} /* namespace rr */
#endif /* RRCOMPILEDMODELGENERATOR_H_ */
