#pragma hdrstop
#include <algorithm>
#include "sbml/Model.h"
#include "sbml/SBMLDocument.h"
#include "rr-libstruct/lsLibStructural.h"
#include "rrStringListContainer.h"
#include "rrStringUtils.h"
#include "rrUtils.h"
#include "rrRule.h"
#include "rrScanner.h"
#include "rrLogger.h"
#include "rrRoadRunner.h"
#include "rrException.h"
#include "rrCModelGenerator.h"
#include "rrCompiledExecutableModel.h"
//---------------------------------------------------------------------------

namespace rr
{

using namespace std;
using namespace ls;
using namespace libsbml;

Mutex               CModelGenerator::mCompileMutex;

CModelGenerator::CModelGenerator(const string& tempFolder, const string& supportCodeFolder, const string& compiler)
:
CompiledModelGenerator(),
mTempFileFolder(tempFolder),
mCompiler(supportCodeFolder, compiler),
mModel(0),
mModelLib(0)
{}

CModelGenerator::~CModelGenerator(){}

int CModelGenerator::getNumberOfFloatingSpecies()
{
    return mFloatingSpeciesConcentrationList.size();    //Todo: is there a list of floating species?
}

string CModelGenerator::getHeaderCode()
{
    return mHeader.ToString();
}

string CModelGenerator::getSourceCode()
{
    return mSource.ToString();
}

string CModelGenerator::getHeaderCodeFileName()
{
    return mHeaderCodeFileName;
}

string CModelGenerator::getSourceCodeFileName()
{
    return mSourceCodeFileName;
}


// Generates the Model Code from the SBML string
string CModelGenerator::generateModelCode(const string& sbmlStr, const bool& _computeAndAssignConsevationLaws)
{
    //This function now assume that the sbml already been loaded into NOM and libstruct..
    mComputeAndAssignConsevationLaws  = _computeAndAssignConsevationLaws;
    Log(lDebug2)<<"Entering CModelGenerators generateModelCode function";
    StringList  Warnings;
    CodeBuilder ignore;     //The Write functions below are inherited with a CodeBuilder in the
                            //prototype that is not to be used..

    //Clear header and source file objects...
    mHeader.Clear();
    mSource.Clear();

    mModelName = mNOM->getModelName();
    if(!mModelName.size())
    {
        Log(lWarning)<<"Model name is empty. ModelName is assigned 'NameNotSet'.";
        mModelName = "NameNotSet";
    }

    Log(lDebug1)<<"Processing model: "<<mModelName;
    mNumReactions  = mNOM->getNumReactions();

    Log(lDebug3)<<"Number of reactions:"<<mNumReactions;

    mGlobalParameterList.Clear();
    mModifiableSpeciesReferenceList.Clear();
    mLocalParameterList.reserve(mNumReactions);
    mReactionList.Clear();
    mBoundarySpeciesList.Clear();
    mFloatingSpeciesConcentrationList.Clear();
    mFloatingSpeciesAmountsList.Clear();
    mCompartmentList.Clear();
    mConservationList.Clear();
    mFunctionNames.empty();
    mFunctionParameters.empty();

    if(mComputeAndAssignConsevationLaws)
    {
        mNumIndependentSpecies     = mLibStruct->getNumIndSpecies();
        mIndependentSpeciesList = mLibStruct->getIndependentSpecies();
        mDependentSpeciesList   = mLibStruct->getDependentSpecies();
    }
    else
    {
        mNumIndependentSpecies = mLibStruct->getNumSpecies();
        mIndependentSpeciesList = mLibStruct->getSpecies();
    }

    // Load the compartment array (name and value)
    mNumCompartments            = readCompartments();

    // Read FloatingSpecies
    mNumFloatingSpecies         = readFloatingSpecies();
    mNumDependentSpecies        = mNumFloatingSpecies - mNumIndependentSpecies;

    // Load the boundary species array (name and value)
    mNumBoundarySpecies     = readBoundarySpecies();

    // Get all the parameters into a list, global and local
    mNumGlobalParameters     = readGlobalParameters();
    mNumModifiableSpeciesReferences = readModifiableSpeciesReferences();

    // Load up local parameters next
    readLocalParameters(mNumReactions, mLocalParameterDimensions, mTotalLocalParmeters);
    mNumEvents = mNOM->getNumEvents();

    //Write model to String builder...
    writeClassHeader(ignore);
    writeOutVariables(ignore);
    writeOutSymbolTables(ignore);

    ///// Write non exports
    mHeader.NewLine("\n//NON - EXPORTS ========================================");
    mHeader.AddFunctionProto("void", "InitializeDelays(ModelData* md)");

    ///// Start of exported functions
    mHeader.NewLine("\n//EXPORTS ========================================");
    mHeader.AddFunctionExport("int", "InitModelData(ModelData* md)");
    mHeader.AddFunctionExport("int", "InitModel(ModelData* md)");

    mHeader.AddFunctionExport("char*", "getModelName(ModelData* md)");
    ///////////////

    writeResetEvents(ignore, mNumEvents);
    writeSetConcentration(ignore);
    writeGetConcentration(ignore);
    writeConvertToAmounts(ignore);
    writeConvertToConcentrations(ignore);
    writeProperties(ignore);
    writeAccessors(ignore);
    writeUserDefinedFunctions(ignore);
    writeSetInitialConditions(ignore, mNumFloatingSpecies);
    writeSetBoundaryConditions(ignore);
    writeSetCompartmentVolumes(ignore);
    writeSetParameterValues(ignore, mNumReactions);
    writeComputeConservedTotals(ignore, mNumFloatingSpecies, mNumDependentSpecies);

    // Get the L0 matrix
    int nrRows;
    int nrCols;

    ls::DoubleMatrix* aL0 = initializeL0(nrRows, nrCols);     //Todo: What is this doing? answer.. it is used below..
    writeUpdateDependentSpecies(ignore, mNumIndependentSpecies, mNumDependentSpecies, *aL0);
    int numOfRules = writeComputeRules(ignore, mNumReactions);

    writeComputeAllRatesOfChange(ignore, mNumIndependentSpecies, mNumDependentSpecies, *aL0);
    delete aL0;
    writeComputeReactionRates(ignore, mNumReactions);
    writeEvalModel(ignore, mNumReactions, mNumIndependentSpecies, mNumFloatingSpecies, numOfRules);
    writeEvalEvents(ignore, mNumEvents, mNumFloatingSpecies);
    writeEventAssignments(ignore, mNumReactions, mNumEvents);
    writeEvalInitialAssignments(ignore, mNumReactions);
    writeTestConstraints(ignore);

    writeInitModelDataFunction(mHeader, mSource);
    writeInitFunction(mHeader, mSource);

    mHeader<<"\n\n#endif //modelH"<<NL();
    string modelCode = mHeader.ToString() + mSource.ToString();

    Log(lDebug5)<<" ------ Model Code --------\n"
            <<modelCode
            <<" ----- End of Model Code -----\n";

    return modelCode;
}

void CModelGenerator::writeClassHeader(CodeBuilder& ignore)
{
    //Create c code header file....
    mHeader<<"#ifndef modelH"<<endl;
    mHeader<<"#define modelH"<<endl;
    mHeader<<"#include <stdio.h>"<<endl;
    mHeader<<"#include <stdbool.h>"<<endl;
    mHeader<<"#include \"rrModelData.h\"\t             //Contains the structure defining model data passed to the shared library."<<endl;
    mHeader<<"#include \"rrCExporter.h\"\t             //Export Stuff."<<endl;


    mHeader<<append("//************************************************************************** " + NL());
    mHeader<<"//Number of floating species: "<<mFloatingSpeciesConcentrationList.size()<<endl;
    for (int i = 0; i < mFloatingSpeciesConcentrationList.size(); i++)
    {
        mHeader<<"\t// y["<<i<<"] = "<<mFloatingSpeciesConcentrationList[i].name<<endl;//{2}", NL());
    }

    mHeader<<append("//************************************************************************** " + NL());
    mHeader<<append(NL());
//    mHeader<<Format("D_S struct TModel{0}", NL());
//    mHeader<<append("{" + NL());

    //Header of the source file...
    mSource<<"#include <math.h>"<<endl;
    mSource<<"#include <stdio.h>"<<endl;
    mSource<<"#include <stdlib.h>"<<endl;
    mSource<<"#include <string.h>"<<endl;
    mSource<<"#include \"rrSupport.h\"\t     //Supportfunctions for event handling.."<<endl;
}

void CModelGenerator::writeOutVariables(CodeBuilder& ignore)
{}


void CModelGenerator::writeComputeAllRatesOfChange(CodeBuilder& ignore, const int& numIndependentSpecies, const int& numDependentSpecies, DoubleMatrix& L0)
{
     //In header
       mHeader.AddFunctionExport("void", "computeAllRatesOfChange(ModelData* md)");
    mSource<<append("//Uses the equation: dSd/dt = L0 dSi/dt" + NL());
    mSource<<"void computeAllRatesOfChange(ModelData* md)\n{";

    mSource<<gNL<<gTab<<"int i;\n";

    // calloc is often preferable as it creates zero initialized memory.
    // this is free'd just before this function exits.
    mSource<<"\n\tdouble* dTemp = (double*) calloc((md->amountsSize + md->rateRulesSize), sizeof(double));\n";

    for (int i = 0; i < numAdditionalRates(); i++)
    {
        mSource<<format("\tdTemp[{0}] = {1};{2}", i, mMapRateRule[i], NL());
    }

    mSource<<gTab<<"for(i = 0; i < md->amountsSize; i++)\n";
    mSource<<gTab<<"{\n"<<gTab<<gTab<<"dTemp[i + md->rateRulesSize] = md->amounts[i];\n\t}";
    mSource<<append("\n\t//amounts.CopyTo(dTemp, rateRules.Length); " + NL());

    mSource<<append("\t__evalModel(md, md->time, dTemp);" + NL());
    bool isThereAnEntry = false;
    for (int i = 0; i < numDependentSpecies; i++)
    {
        mSource<<format("\tmd->dydt[{0}] = ", (numIndependentSpecies + i));
        isThereAnEntry = false;
        for (int j = 0; j < numIndependentSpecies; j++)
        {
            string dyName = format("md->dydt[{0}]", j);

            if (L0(i,j) > 0)
            {
                isThereAnEntry = true;
                if (L0(i,j) == 1)
                {
                    mSource<<format(" + {0};{1}", dyName, NL());
                }
                else
                {
                    mSource<<format(" + (double){0}{1}{2};{3}", writeDouble(L0(i,j)), mFixAmountCompartments, dyName, NL());
                }
            }
            else if (L0(i,j) < 0)
            {
                isThereAnEntry = true;
                if (L0(i,j) == -1)
                {
                    mSource<<format(" - {0};{1}", dyName, NL());
                }
                else
                {
                    mSource<<format(" - (double){0}{1}{2};{3}", writeDouble(fabs(L0(i,j))), mFixAmountCompartments, dyName, NL());
                }
            }
        }
        if (!isThereAnEntry)
        {
            mSource<<append("0;");
        }
        mSource<<"\n";
    }

    // done with temp buffer
    mSource << "\n\tfree(dTemp);\n";
    mSource<<format("}{0}{0}", NL());
}

void CModelGenerator::writeComputeConservedTotals(CodeBuilder& ignore, const int& numFloatingSpecies, const int& numDependentSpecies)
{
    mHeader.AddFunctionExport("void", "computeConservedTotals(ModelData* md)");
    mSource<<"// Uses the equation: C = Sd - L0*Si"<<endl;
    mSource<<"void computeConservedTotals(ModelData* md)\n{";

    if (numDependentSpecies > 0)
    {
        string factor;
        ls::DoubleMatrix *gamma = mLibStruct->getGammaMatrix();


        for (int i = 0; i < numDependentSpecies; i++)
        {
            mSource<<format("\n\tmd->ct[{0}] = ", i);
            for (int j = 0; j < numFloatingSpecies; j++)
            {
                double current = (gamma != NULL) ? (*gamma)(i,j) : 1.0;    //Todo: This is a bug? We should not be here if the matrix is NULL.. Triggered by model 00029

                if ( current != 0.0 )
                {
                    if (!gamma)//IsNaN(current)) //C# code is doing one of these.. factor = "" .. ??
                    {
                        // TODO: fix this
                        factor = "";
                    }
                    else if (fabsl(current) == 1.0)
                    {
                        factor = "";
                    }
                    else
                    {
                        factor = writeDouble(fabsl(current)) +
                                 mFixAmountCompartments;
                    }

                    if (current > 0)
                    {
                        string cYY = convertSpeciesToY(mFloatingSpeciesConcentrationList[j].name);
                        string cTC = convertCompartmentToC(mFloatingSpeciesConcentrationList[j].compartmentName);
                        mSource<<append(" + " + factor + "md->" + cYY +
                                  mFixAmountCompartments +
                                  convertCompartmentToC(mFloatingSpeciesConcentrationList[j].compartmentName));
                    }
                    else
                    {
                        mSource<<append(" - " + factor + convertSpeciesToY(mFloatingSpeciesConcentrationList[j].name) +
                                  mFixAmountCompartments +
                                  convertCompartmentToC(mFloatingSpeciesConcentrationList[j].compartmentName));
                    }
                }
            }
            mSource<<append(";" + NL());
            mConservationList.Add(Symbol("CSUM" + toString(i))); //TODO: how to deal with this?
        }
    }
    else
    {
        //mSource<<"printf(\"In an empty ComputeConservedTotals!\\n \");\n";
    }
    mSource<<"}\n\n";
}

void CModelGenerator::writeUpdateDependentSpecies(CodeBuilder& ignore, const int& numIndependentSpecies, const int& numDependentSpecies, DoubleMatrix& L0)
{
    mHeader.AddFunctionExport("void", "updateDependentSpeciesValues(ModelData* md, double* y)");
    mSource<<append("// Compute values of dependent species " + NL());
    mSource<<append("// Uses the equation: Sd = C + L0*Si" + NL());
    mSource<<"void updateDependentSpeciesValues(ModelData* md, double* y)\n{";

    // Use the equation: Sd = C + L0*Si to compute dependent concentrations

    if (numDependentSpecies > 0)
    {
        for (int i = 0; i < numDependentSpecies; i++)
        {
            mSource<<format("\n\tmd->y[{0}] = ", (i + numIndependentSpecies));
            mSource<<format("(md->ct[{0}]", i);
            string cLeftName =
                convertCompartmentToC(
                    mFloatingSpeciesConcentrationList[i + numIndependentSpecies].compartmentName);

            for (int j = 0; j < numIndependentSpecies; j++)
            {
                string yName = format("y[{0}]", j);
                string cName = convertCompartmentToC(mFloatingSpeciesConcentrationList[j].compartmentName);
                double* mat = L0.GetPointer();
                double matElementValue = L0(i,j);

                if (L0(i,j) > 0) // In C# code there is no checking for index out of bound..
                {
                    if (L0(i,j) == 1)
                    {
                        mSource<<format(" + {0}\t{1}{2}{3}{0}\t",
                            "",
                            yName,
                            mFixAmountCompartments,
                            cName);
                    }
                    else
                    {
                        mSource<<format("{0} + (double){1}{2}{3}{2}{4}",
                            "",
                            writeDouble(L0(i,j)),
                            mFixAmountCompartments,
                            yName,
                            cName);
                    }
                }
                else if (L0(i,j) < 0)
                {
                    if (L0(i,j) == -1)
                    {
                        mSource<<format("{0} - {1}{2}{3}",
                            "",
                            yName,
                            mFixAmountCompartments,
                            cName);
                    }
                    else
                    {
                        mSource<<format("{0} - (double){1}{2}{3}{2}{4}",
                            "",
                            writeDouble(fabsl(L0(i,j))),
                            mFixAmountCompartments,
                            yName,
                            cName);
                    }
                }
            }
            mSource<<format(") / {0};{1}", cLeftName, NL());
        }
    }
    mSource<<format("}{0}{0}", NL());
}

void CModelGenerator::writeUserDefinedFunctions(CodeBuilder& ignore)
{
    for (int i = 0; i < mNOM->getNumFunctionDefinitions(); i++)
    {
        try
        {
            StringListContainer oList = mNOM->getNthFunctionDefinition(i);
            StringList aList = oList[0];

              string sName = aList[0];
              //sName.Trim();
            mFunctionNames.add(sName);
            StringList oArguments = oList[1];
            StringList list2 = oList[2];
            string sBody = list2[0];

            mSource<<format("// User defined function:  {0}{1}", sName, NL());
            mSource<<format("\t double {0} (", sName);

            for (int j = 0; j < oArguments.Count(); j++)
            {
                mSource<<append("double " + (string)oArguments[j]);
                mFunctionParameters.add((string)oArguments[j]);
                if (j < oArguments.Count() - 1)
                {
                    mSource<<append(", ");
                }
            }
            string userFunc = convertUserFunctionExpression(sBody);

            if(userFunc.find("spf_piecewise") != string::npos)
            {
                convertFunctionCallToUseVarArgsSyntax("spf_piecewise", userFunc);
            }

            if(userFunc.find("spf_and") != string::npos)
            {
                convertFunctionCallToUseVarArgsSyntax("spf_and", userFunc);
            }

            if(userFunc.find("spf_or") != string::npos)
            {
                convertFunctionCallToUseVarArgsSyntax("spf_or", userFunc);
            }

            if(userFunc.find("spf_xor") != string::npos)
            {
                convertFunctionCallToUseVarArgsSyntax("spf_xor", userFunc);
            }

            mSource<<append(")" + NL() + "\t{" + NL() + "\t\t return " +
                      userFunc
                      + ";" + NL() + "\t}" + NL() + NL());
        }
        catch (const Exception& ex)
        {
            CodeBuilder msg;
            msg<<"Error while trying to get Function Definition #" << i <<ex.what() << "\r\n\r\n";
            throw Exception(msg.ToString());
        }
    }
}

void CModelGenerator::writeResetEvents(CodeBuilder& ignore, const int& numEvents)
{
      mHeader.AddFunctionExport("void", "resetEvents(ModelData* md)");
      mSource<<"void resetEvents(ModelData* md)\n{";
      for (int i = 0; i < numEvents; i++)
      {
          mSource<<format("\n\tmd->eventStatusArray[{0}] = false;{1}", i, NL());
          mSource<<format("\tmd->previousEventStatusArray[{0}] = false;", i);
          if(i == numEvents -1)
          {
              mSource<<"\n";
          }
      }
      mSource<<format("}{0}", NL());
}

void CModelGenerator::writeSetConcentration(CodeBuilder& ignore)
{
    mHeader.AddFunctionExport("void", "setConcentration(ModelData* md, int index, double value)");
    mSource<<"\nvoid setConcentration(ModelData* md, int index, double value)\n{";
    mSource<<format("\n\tdouble volume = 0.0;{0}", NL());
    mSource<<format("\tmd->y[index] = value;{0}", NL());
    mSource<<format("\tswitch (index)\n\t{{0}", NL());

    for (int i = 0; i < mFloatingSpeciesConcentrationList.size(); i++)
    {
        mSource<<format("\t\tcase {0}:\n\t\t\tvolume = {1};{2}",
          i,
          convertCompartmentToC(mFloatingSpeciesConcentrationList[i].compartmentName),
          NL());
      mSource<<format("\t\tbreak;{0}", NL());
    }

    mSource<<format("\t}{0}", NL());

    mSource<<format("\tmd->amounts[index] = md->y[index]*volume;{0}", NL());
    mSource<<format("}{0}{0}", NL());
}

void CModelGenerator::writeGetConcentration(CodeBuilder& ignore)
{
    mHeader.AddFunctionExport("double", "getConcentration(ModelData* md,int index)");
    mSource<<format("double getConcentration(ModelData* md, int index)\n{{0}", NL());
    mSource<<format("\treturn md->y[index];{0}", NL());
    mSource<<format("}{0}{0}", NL());
}

void CModelGenerator::writeConvertToAmounts(CodeBuilder& ignore)
{
    mHeader.AddFunctionExport("void", "convertToAmounts(ModelData* md)");
    mSource<<format("void convertToAmounts(ModelData* md)\n{{0}", NL());
    for (int i = 0; i < mFloatingSpeciesConcentrationList.size(); i++)
    {
        mSource<<format("\tmd->amounts[{0}] = md->y[{0}]*{1};{2}",
            i,
            convertCompartmentToC(mFloatingSpeciesConcentrationList[i].compartmentName),
            NL());
    }
    mSource<<format("}{0}{0}", NL());
}

void CModelGenerator::writeConvertToConcentrations(CodeBuilder& ignore)
{
    mHeader.AddFunctionExport("void", "convertToConcentrations(ModelData* md)");
    mSource<<"void convertToConcentrations(ModelData* md)\n{";
    for (int i = 0; i < mFloatingSpeciesConcentrationList.size(); i++)
    {
        mSource<<"\n\tmd->y[" << i << "] = md->amounts[" << i << "] / " <<
                  convertCompartmentToC(mFloatingSpeciesConcentrationList[i].compartmentName) << ";";
    }
    mSource<<append("\n}" + NL() + NL());
}

void CModelGenerator::writeProperties(CodeBuilder& ignore)
{
}

void CModelGenerator::writeAccessors(CodeBuilder& ignore)
{
    mHeader.AddFunctionExport("int", "getNumLocalParameters(ModelData* md, int reactionId)");
    mSource<<"int getNumLocalParameters(ModelData* md, int reactionId)\n{\n\t";
    mSource<<"return md->localParameterDimensions[reactionId];\n}\n\n";
}

string CModelGenerator::findSymbol(const string& varName)
{
      int index = 0;
      if (mFloatingSpeciesConcentrationList.find(varName, index))
      {
          return format("md->y[{0}]", index);
      }
      else if (mGlobalParameterList.find(varName, index))
      {
          return format("md->gp[{0}]", index);
      }
      else if (mBoundarySpeciesList.find(varName, index))
      {
          return format("md->bc[{0}]", index);
      }
      else if (mCompartmentList.find(varName, index))
      {
          return format("md->c[{0}]", index);
      }
      else if (mModifiableSpeciesReferenceList.find(varName, index))
      {
          return format("md->sr[{0}]", index);
      }
      else
      {
          throw Exception(format("Unable to locate lefthand side symbol in assignment[{0}]", varName));
      }
}

void CModelGenerator::writeTestConstraints(CodeBuilder& ignore)
{
    mHeader.AddFunctionExport("void", "testConstraints(ModelData* md)");
    mSource<<append("void testConstraints(ModelData* md)" + NL());
    mSource<<append("{");

    for (int i = 0; i < mNOM->getNumConstraints(); i++)
    {
        string sMessage;
        string sCheck = mNOM->getNthConstraint(i, sMessage);

        mSource<<append("\tif (" + substituteTerms(mNOM->getNumReactions(), "", sCheck) + " == 0.0 )" + NL());
        mSource<<append("\t\tthrow new Exception(\"" + sMessage + "\");" + NL());
    }

    mSource<<append("}" + NL() + NL());
}

void CModelGenerator::writeEvalInitialAssignments(CodeBuilder& ignore, const int& numReactions)
{
    mHeader.AddFunctionExport("void", "evalInitialAssignments(ModelData* md)");
    mSource<<append("void evalInitialAssignments(ModelData* md)" + NL());
    mSource<<append("{\n");

    int numInitialAssignments = mNOM->getNumInitialAssignments();

    if (numInitialAssignments > 0)
    {
        vector< pair<string, string> > oList;// = new List<Pair<string, string>>();
        for (int i = 0; i < numInitialAssignments; i++)
        {
            pair<string, string> pair = mNOM->getNthInitialAssignmentPair(i);
            oList.push_back(mNOM->getNthInitialAssignmentPair(i));
        }

        // sort them ...
        bool bChange = true;
        int nIndex = -1;
        while (bChange)
        {
            bChange = false;

            for (int i = 0; i < oList.size(); i++)
            {
                pair<string, string> current = oList[i];
                for (int j = i + 1; j < oList.size(); j++)
                {
                    if (expressionContainsSymbol(current.second, oList[j].first))
                    {
                        bChange = true;
                        nIndex = j;
                        break;
                    }
                }
                if (bChange)
                {
                    break;
                }
            }

            if (bChange)
            {
                pair<string, string> pairToMove = oList[nIndex];
                oList.erase(oList.begin() + nIndex);
                //oList.RemoveAt(nIndex);
                oList.insert(oList.begin(), pairToMove);    //Todo: check it this is correct...
            }
        }

        vector< pair<string, string> >::iterator iter;
        for(iter = oList.begin(); iter < oList.end(); iter++)
        {
            pair<string, string>& pair = (*iter);
            string leftSideRule = findSymbol(pair.first);
            string rightSideRule = pair.second;
            if (leftSideRule.size())
            {
                mSource<<append(leftSideRule + " = ");
                string temp = append(substituteTerms(numReactions, "", rightSideRule) + ";" + NL());
                mSource<<temp;
            }
        }
    }
    for (int i = 0; i < mNOM->getModel()->getNumEvents(); i++)
    {
        libsbml::Event *current = mNOM->getModel()->getEvent(i);
        string initialTriggerValue = toString(current->getTrigger()->getInitialValue());//.ToString().ToLowerInvariant();
        mSource<<append("\tmd->eventStatusArray[" + toString(i) + "] = " + initialTriggerValue + ";" + NL());
        mSource<<append("\tmd->previousEventStatusArray[" + toString(i) + "] = " + initialTriggerValue + ";" + NL());
    }
    mSource<<append("}" + NL() + NL());
}

int CModelGenerator::writeComputeRules(CodeBuilder& ignore, const int& numReactions)
{
    IntStringHashTable mapVariables;
    mapVariables.clear();
    int numRateRules = 0;
    int numOfRules = mNOM->getNumRules();

    mHeader.AddFunctionExport("void", "computeRules(ModelData* md, double* y)");
    mSource<<"void computeRules(ModelData* md, double* y)\n{\n";

    for (int i = 0; i < numOfRules; i++)
    {
        try
        {
            string leftSideRule = "";
            string rightSideRule = "";
            string ruleType = mNOM->getNthRuleType(i);

            // We only support assignment and ode rules at the moment
            string eqnRule = mNOM->getNthRule(i);
            RRRule aRule(eqnRule, ruleType);
            string varName       = trim(aRule.GetLHS());
            string rightSide     = trim(aRule.GetRHS());

            bool isRateRule = false;

            switch (aRule.GetType())
            {
                case rtAlgebraic:
                    Log(lWarning)<<"RoadRunner does not yet support algebraic rules in SBML, they will be ignored.";
                    leftSideRule = "";//NULL;
                break;

                case rtAssignment:
                    leftSideRule = findSymbol(varName);
                break;

                case rtRate:
                    isRateRule = true;
                    int index;
                    if (mFloatingSpeciesConcentrationList.find(varName,  index))
                    {
                        leftSideRule = format("\n\tmd->dydt[{0}]", index);
                        mFloatingSpeciesConcentrationList[index].rateRule = true;
                    }
                    else
                    {
                        leftSideRule = "\n\tmd->rateRules[" + toString(numRateRules) + "]";
                        mMapRateRule[numRateRules] = findSymbol(varName);
                        mapVariables[numRateRules] = varName;
                        numRateRules++;
                    }
                break;
            }

            // Run the equation through MathML to carry out any conversions (eg ^ to Pow)
            if(rightSide.size())
            {
                string rightSideMathml    = NOMSupport::convertStringToMathML(rightSide);
                rightSideRule             = NOMSupport::convertMathMLToString(rightSideMathml);
            }

            if (leftSideRule.size())
            {
                mSource<<gTab<<append(leftSideRule + " = ");
                int speciesIndex;
                bool isSpecies = mFloatingSpeciesConcentrationList.find(varName, speciesIndex);

                Symbol* symbol = (speciesIndex != -1) ? &(mFloatingSpeciesConcentrationList[speciesIndex]) : NULL;
                string sCompartment;

                if(isRateRule && mNOM->multiplyCompartment(varName, sCompartment) && (rightSide.find(sCompartment) == string::npos))
                {
                    string temp = format("({0}) * {1};{2}", substituteTerms(numReactions, "", rightSideRule), findSymbol(sCompartment), NL());
                    //temp = ReplaceWord("time", "md->time", temp);
                    mSource<<temp;
                }
                else
                {
                    if (isSpecies && !isRateRule && symbol != NULL && symbol->hasOnlySubstance && symbol->compartmentName.size() != 0)
                    {
                        mSource<<format("({0}) / {1};{2}", substituteTerms(numReactions, "", rightSideRule), findSymbol(symbol->compartmentName), NL());
                    }
                    else
                    {
                        string temp   = format("{0};{1}", substituteTerms(numReactions, "", rightSideRule), NL());
                        //temp = ReplaceWord("time", "md->time", temp);

                        if(temp.find("spf_piecewise") != string::npos)
                        {
                            convertFunctionCallToUseVarArgsSyntax("spf_piecewise", temp);
                        }
                        temp = removeNewLines(temp);
                        mSource<<temp;
                    }
                }

                if (mNOM->isCompartment(varName))
                {
                    mSource<<append("\n\tconvertToConcentrations(md);\n");
                }
            }
        }
        catch (const Exception& e)
        {
            throw CoreException("Error while trying to get Rule #" + toString(i) + e.Message());
        }
    }

    mSource<<append("\n}" + NL() + NL());

//  mHeader.FormatArray("D_S double", "_rateRules", numRateRules, "Vector containing values of additional rate rules"); //Todo: why is t his here in nowhere?
//    mHeader<<"D_S int _rateRulesSize="<<numRateRules<<";           // Number of rateRules   \n"; //Todo: why is this here in nowhere?
    mHeader.AddFunctionExport("void", "InitializeRates(ModelData* md)");

    mSource<<"void InitializeRates(ModelData* md)\n{";

    for (int i = 0; i < numRateRules; i++)
    {
        mSource<<"\n\tmd->rateRules[" << i << "] = " << mMapRateRule[i] << ";" << NL();
    }

    mSource<<append("}" + NL() + NL());

    mHeader.AddFunctionExport("void", "AssignRatesA(ModelData* md)");
    mSource<<append("void AssignRatesA(ModelData* md)\n{" + NL());

    for (int i = 0; i < mMapRateRule.size(); i++)
    {
        if(!i)
        {
            mSource<<"\n";
        }
        mSource<<"\t"<<(string) mMapRateRule[i] << " = md->rateRules[" << i << "];\n";
    }

    mSource<<append("}" + NL() + NL());

    mHeader.AddFunctionExport("void", "InitializeRateRuleSymbols(ModelData* md)");
    mSource<<"void InitializeRateRuleSymbols(ModelData* md) \n{";
    for (int i = 0; i < mMapRateRule.size(); i++)
    {
        if(!i)
        {
            mSource<<"\n";
        }

        string varName = (string) mapVariables[i];
        if(varName.size())
        {
            double value = mNOM->getValue(varName);
            if (!isNaN(value))
            {
                mSource<<gTab<<mMapRateRule[i] << " = " << toString(value, mDoubleFormat) << ";" << NL();
            }
        }
    }

    mSource<<"}\n\n";
    mHeader.AddFunctionExport("void", "AssignRatesB(ModelData* md, double oRates[])");
    mSource<<"void AssignRatesB(ModelData* md, double oRates[])\n{";

    for (int i = 0; i < mMapRateRule.size(); i++)
    {
        if(!i)
        {
            mSource<<"\n";
        }

        mSource<< mMapRateRule[i] << " = oRates[" << i << "];" << NL();
    }

    mSource<<append("}" + NL() + NL());
    mHeader.AddFunctionExport("double*", "GetCurrentValues(ModelData* md)");
    mSource<<"double* GetCurrentValues(ModelData* md)\n{";
    mSource<<"\n\tdouble* dResult = (double*) calloc(" << numAdditionalRates() << ", sizeof(double));\n";

    for (int i = 0; i < mMapRateRule.size(); i++)
    {
           if(!i)
        {
            mSource<<"\n";
        }

        mSource<<"\tdResult[" << i << "] = " << mMapRateRule[i] << ";" << NL();
    }
    mSource<<"\treturn dResult;\n";

    mSource<<append("}" + NL() + NL());
    return numOfRules;
}

void CModelGenerator::writeComputeReactionRates(CodeBuilder& ignore, const int& numReactions)
{
    mHeader.AddFunctionExport("void", "computeReactionRates(ModelData* md, double time, double *y)");
    mSource<<append("// Compute the reaction rates" + NL());
    mSource<<"void computeReactionRates(ModelData* md, double time, double *y)\n{";    //Todo: what is time doing here?


    for (int i = 0; i < numReactions; i++)
    {
        string kineticLaw = mNOM->getKineticLaw(i);

        // The following code is for the case when the kineticLaw contains a ^ in place
        // of pow for exponent handling. It would not be needed in the case when there is
        // no ^ in the kineticLaw.
        string subKineticLaw;
//        if (kineticLaw.IndexOf("^", System.StringComparison.Ordinal) > 0) //Todo: fix this...
//        {
//            string kineticLaw_mathml = mNOM->convertStringToMathML(kineticLaw);
//            subKineticLaw = mNOM->convertMathMLToString(kineticLaw_mathml);
//        }
//        else
        {
            subKineticLaw = kineticLaw;
        }

        string modKineticLaw = substituteTerms(mReactionList[i].name, subKineticLaw, true) + ";";

        // modify to use current y ...
        modKineticLaw = substitute(modKineticLaw, "_y[", "y[");
        string expression = format("\n\tmd->rates[{0}] = {1}{2}", i, modKineticLaw, NL());

        if(expression.find("spf_and") != string::npos)
        {
            convertFunctionCallToUseVarArgsSyntax("spf_and", expression);
        }

        if(expression.find("spf_or") != string::npos)
        {
            convertFunctionCallToUseVarArgsSyntax("spf_or", expression);
        }

        if(expression.find("spf_xor") != string::npos)
        {
            convertFunctionCallToUseVarArgsSyntax("spf_xor", expression);
        }

        if(expression.find("spf_squarewave") != string::npos)
        {
            convertFunctionCallToUseVarArgsSyntax("spf_squarewave", expression);
        }

        if(expression.find("spf_piecewise") != string::npos)
        {
            convertFunctionCallToUseVarArgsSyntax("spf_piecewise", expression);
        }

        expression = removeChars(expression, "\t \n");
        mSource<<"\n\t"<<expression<<"\n";
    }

    mSource<<format("}{0}{0}", NL());
}

void CModelGenerator::writeEvalEvents(CodeBuilder& ignore, const int& numEvents, const int& numFloatingSpecies)
{
    mSource<<append("//Event handling function" + NL());
    mHeader.AddFunctionExport("void", "evalEvents(ModelData* md, double timeIn, double *oAmounts)");
    mSource<<append("void evalEvents(ModelData* md, double timeIn, double *oAmounts)" + NL());
    mSource<<append("{" + NL());

    if (numEvents > 0)
    {
        for (int i = 0; i < numAdditionalRates(); i++)
        {
            mSource<<gTab<<(string) mMapRateRule[i] << " = oAmounts[" << i << "];" << NL();
        }
        for (int i = 0; i < numFloatingSpecies; i++)
        {
            mSource<<"\tmd->y[" << i << "] = oAmounts[" << (i + numAdditionalRates()) << "]/" <<
                      convertCompartmentToC(mFloatingSpeciesConcentrationList[i].compartmentName) << ";" << NL();
        }
    }

    mSource<<append("\tmd->time = timeIn;" + NL());
    mSource<<append("\tupdateDependentSpeciesValues(md, md->y);" + NL());
    mSource<<append("\tcomputeRules(md, md->y);" + NL());

    for (int i = 0; i < numEvents; i++)
    {
        ArrayList ev = mNOM->getNthEvent(i);
        StringList tempList = ev[0];
        string eventString = tempList[0];

        eventString = substituteTerms(0, "", eventString);
        mSource<<"\tmd->previousEventStatusArray[" << i << "] = md->eventStatusArray[" << i << "];" << NL();
        convertFunctionCallToUseVarArgsSyntax("spf_and", eventString);
        eventString = removeNewLines(eventString);
        mSource<<append("\tif (" + eventString + " == 1.0)\n\t{" + NL());
        mSource<<append("\t\tmd->eventStatusArray[" + toString(i) + "] = true;" + NL());
        mSource<<append("\t\tmd->eventTests[" + toString(i) + "] = 1;" + NL());
        mSource<<append("\n\t}\n\telse\n\t{\n");
        mSource<<append("\t\tmd->eventStatusArray[" + toString(i) + "] = false;" + NL());
        mSource<<append("\t\tmd->eventTests[" + toString(i) + "] = -1;" + NL());
        mSource<<append("\t}" + NL());
    }
    mSource<<append("}" + NL() + NL());
}

void CModelGenerator::writeEvalModel(CodeBuilder& ignore, const int& numReactions, const int& numIndependentSpecies, const int& numFloatingSpecies, const int& numOfRules)
{
    mHeader.AddFunctionExport("void", "__evalModel(ModelData* md, double, double*)");
    mSource<<append("//Model Function" + NL());
    mSource<<"void __evalModel(ModelData* md, double timein, double* oAmounts)\n{";


    for (int i = 0; i < numAdditionalRates(); i++)
    {
        mSource<<"\n"<<(string)mMapRateRule[i] << " = oAmounts[" << i << "];" << NL();
    }

    for (int i = 0; i < numFloatingSpecies; i++)
    {
        mSource<<"\n\tmd->y[" << i << "] = oAmounts[" << i + numAdditionalRates() << "]/" <<
                  convertCompartmentToC(mFloatingSpeciesConcentrationList[i].compartmentName) << ";" << NL();
    }

    mSource<<append(NL());
    mSource<<append("\tconvertToAmounts(md);" + NL());
    mSource<<append("\tmd->time = timein;  // Don't remove" + NL());
    mSource<<append("\tupdateDependentSpeciesValues(md, md->y);" + NL());

    if (numOfRules > 0)
    {
        mSource<<append("\tcomputeRules(md, md->y);" + NL());
    }

    mSource<<append("\tcomputeReactionRates(md, md->time, md->y);" + NL());

    // write out the ODE equations
    string stoich;
    for (int i = 0; i < numIndependentSpecies; i++)
    {
        CodeBuilder eqnBuilder;// = new CodeBuilder(" ");
        string floatingSpeciesName = mIndependentSpeciesList[i];
        for (int j = 0; j < numReactions; j++)
        {
            Reaction *oReaction = mNOM->getModel()->getReaction(j);
            int numProducts = (int) oReaction->getNumProducts();
            double productStoichiometry;
            for (int k1 = 0; k1 < numProducts; k1++)
            {
                SpeciesReference* product = oReaction->getProduct(k1);

                string productName = product->getSpecies();
                if (floatingSpeciesName == productName)
                {
                    productStoichiometry = product->getStoichiometry();

                    if (product->isSetId() && product->getLevel() > 2)
                    {
                        stoich = "(" +
                             substituteTerms(numReactions, "",
                                product->getId()) +
                             ") * ";
                    }
                    else if (product->isSetStoichiometry())
                    {
                        if (productStoichiometry != 1)
                        {
                            int denom = product->getDenominator();
                            if (denom != 1)
                            {
                                stoich = format("((double){0}/(double){1})*", writeDouble(productStoichiometry), denom);
                            }
                            else
                            {
                                stoich = writeDouble(productStoichiometry) + '*';
                            }
                        }
                        else
                        {
                            stoich = "";
                        }
                    }
                    else
                    {
                        if (product->isSetStoichiometryMath() && product->getStoichiometryMath()->isSetMath())
                        {
                            stoich = "(" +
                                     substituteTerms(numReactions, "",
                                        SBML_formulaToString(product->getStoichiometryMath()->getMath())) +
                                     ") * ";
                        }
                        else
                        {
                            stoich = "";
                        }
                    }
                    eqnBuilder<<format(" + {0}md->rates[{1}]", stoich, j);
                }
            }

            int numReactants = (int)oReaction->getNumReactants();
            double reactantStoichiometry;
            for (int k1 = 0; k1 < numReactants; k1++)
            {
                SpeciesReference *reactant = oReaction->getReactant(k1);
                string reactantName = reactant->getSpecies();
                if (floatingSpeciesName == reactantName)
                {
                    reactantStoichiometry = reactant->getStoichiometry();

                    if (reactant->isSetId() && reactant->getLevel() > 2)
                    {
                        stoich = format("({0}) * ", substituteTerms(numReactions, "", reactant->getId()));
                    }
                    else if (reactant->isSetStoichiometry())
                    {
                        if (reactantStoichiometry != 1)
                        {
                            int denom = reactant->getDenominator();
                            if (denom != 1)
                            {
                                stoich = format("((double){0}/(double){1})*", writeDouble(reactantStoichiometry), denom);
                            }
                            else
                            {
                                stoich = writeDouble(reactantStoichiometry) + "*";
                            }
                        }
                        else
                        {
                            stoich = "";
                        }
                    }

                    else
                    {
                        if (reactant->isSetStoichiometryMath() && reactant->getStoichiometryMath()->isSetMath())
                        {
                            stoich = "(" +
                                     substituteTerms(numReactions, "",
                                        SBML_formulaToString(reactant->getStoichiometryMath()->getMath())) +
                                     ") * ";
                        }
                        else
                        {
                            stoich = "";
                        }
                    }

                    eqnBuilder<<append(format(" - {0}md->rates[{1}]", stoich, j));
                }
            }
        }

        string finalStr = eqnBuilder.ToString();//.trim();

        if (isNullOrEmpty(finalStr))
        {
            finalStr = "    0.0";
        }

        if (mNOM->getSBMLDocument()->getLevel() > 2)
        {
            // remember to take the conversion factor into account
            string factor = "";
            Species* species = mNOM->getModel()->getSpecies(floatingSpeciesName);
            if (species != NULL)
            {
                if (species->isSetConversionFactor())
                {
                    factor = species->getConversionFactor();
                }
                else if (mNOM->getModel()->isSetConversionFactor())
                {
                    factor = mNOM->getModel()->getConversionFactor();
                }
            }

            if (!isNullOrEmpty(factor))
            {
                finalStr = findSymbol(factor) + " * (" + finalStr + ")";
            }
        }

        // If the floating species has a raterule then prevent the dydt
        // in the model function from overriding it. I think this is expected behavior.
        if (!mFloatingSpeciesConcentrationList[i].rateRule)
        {
            mSource<<"\tmd->dydt[" << i << "] =" << finalStr << ";" << NL();
        }
    }

    mSource<<append("\tconvertToAmounts(md);" + NL());
    mSource<<append("}" + NL() + NL());
}

void CModelGenerator::writeEventAssignments(CodeBuilder& ignore, const int& numReactions, const int& numEvents)
{
    StringList delays;
    vector<bool> eventType;
    vector<bool> eventPersistentType;
    if (numEvents > 0)
    {
        //Get array of pointers functions
        mSource<<("TEventAssignmentDelegate* Get_eventAssignments(ModelData* md) \n{\n\treturn md->eventAssignments;\n}\n\n");
        mSource<<("TPerformEventAssignmentDelegate* Get_performEventAssignments(ModelData* md) \n{\n\treturn md->performEventAssignments;\n}\n\n");
        mSource<<("TComputeEventAssignmentDelegate* Get_computeEventAssignments(ModelData* md) \n{\n\treturn md->computeEventAssignments;\n}\n\n");
        mSource<<("TEventDelayDelegate* GetEventDelays(ModelData* md) \n{\n\treturn md->eventDelays;\n}\n\n");
        mSource<<append("// Event assignments" + NL());
        for (int i = 0; i < numEvents; i++)
        {
            ArrayList ev = mNOM->getNthEvent(i);
            eventType.push_back(mNOM->getNthUseValuesFromTriggerTime(i));
            eventPersistentType.push_back(mNOM->getModel()->getEvent(i)->getTrigger()->getPersistent());

            StringList event = ev[1];
            int numItems = event.Count();
            string str = substituteTerms(numReactions, "", event[0]);
            delays.add(str);

            mSource<<format("void eventAssignment_{0}(ModelData* md) \n{{1}", i, NL());

            string funcName(format("performEventAssignment_{0}(ModelData* md, double* values)", i));
            mHeader.AddFunctionExport("void", funcName);
            mSource<<format("\tperformEventAssignment_{0}(md, computeEventAssignment_{0}(md) );{1}", i, NL());
            mSource<<append("}\n\n");

            funcName = (format("computeEventAssignment_{0}(ModelData* md)", i));
            mHeader.AddFunctionExport("double*", funcName);

            mSource<<format("double* computeEventAssignment_{0}(ModelData* md)\n{{1}", i, NL());
            StringList oTemp;
            StringList oValue;
            int nCount = 0;
            int numAssignments = ev.Count() - 2;

            mSource<<format("\t\tdouble* values = (double*) malloc(sizeof(double)*{0});{1}", numAssignments, NL());
            for (int j = 2; j < ev.Count(); j++)
            {
                StringList asgn = (StringList) ev[j];
                //string assignmentVar = substituteTerms(numReactions, "", (string)asgn[0]);
                string assignmentVar = findSymbol((string)asgn[0]);
                string str;
                Symbol *species = getSpecies(assignmentVar);


                if (species != NULL && species->hasOnlySubstance)
                {
                    str = format("{0} = ({1}) / {2}", assignmentVar, substituteTerms(numReactions, "", (string)asgn[1]), findSymbol(species->compartmentName));
                }
                else
                {
                    str = format("{0} = {1}", assignmentVar, substituteTerms(numReactions, "", (string) asgn[1]));
                }

                string sTempVar = format("values[{0}]", nCount);

                oTemp.add(assignmentVar);
                oValue.add(sTempVar);

                str = sTempVar+ str.substr(str.find(" ="));
                nCount++;
                string temp = format("\t\t{0};{1}", str, NL());
                mSource<<temp;
            }
            mSource<<append("\treturn values;" + NL());
            mSource<<append("}" + NL());
            mSource<<format("void performEventAssignment_{0}(ModelData* md, double* values) \n{{1}", i, NL());

            for (int j = 0; j < oTemp.Count(); j++)
            {
                mSource<<format("\t\t{0} = values[{1}];{2}", oTemp[j], j, NL());
                string aStr = (string) oTemp[j];
                aStr = trim(aStr);

                if (startsWith(aStr, "md->c[")) //Todo:May have to trim?
                {
                    mSource<<append("\t\tconvertToConcentrations(md);" + NL());
                }
            }

            mSource<<append("}" + NL());
        }
        mSource<<append("\t" + NL());
    }

    //Have to create TEventDelegate functions here
    for (int i = 0; i < delays.Count(); i++)
    {
        mSource<<"double GetEventDelay_"<<i<<"(ModelData* md)\n{\n\treturn "<<delays[i]<<";\n}\n\n";
    }

    mSource<<"void InitializeDelays(ModelData* md)\n{\n";

    for (int i = 0; i < delays.Count(); i++)
    {
        mSource<<format("\tmd->eventDelays[{0}] = (TEventDelayDelegate) malloc(sizeof(TEventDelayDelegate) * 1);{2}", i, delays[i], NL());

        //Inititialize
        mSource<<format("\tmd->eventDelays[{0}] = GetEventDelay_{0};\n", i);
        mSource<<format("\tmd->eventType[{0}] = {1};{2}", i, toString((eventType[i] ? true : false)), NL());
        mSource<<format("\tmd->eventPersistentType[{0}] = {1};{2}", i, (eventPersistentType[i] ? "true" : "false"), NL());
    }
    mSource<<"}\n\n";

    mHeader.AddFunctionExport("void", "computeEventPriorities(ModelData* md)");
    mSource<<"void computeEventPriorities(ModelData* md)\n{";
    for (int i = 0; i < numEvents; i++)
    {
        libsbml::Event* current = mNOM->getModel()->getEvent(i);

        if (current->isSetPriority() && current->getPriority()->isSetMath())
        {
            string priority = SBML_formulaToString(current->getPriority()->getMath());
            mSource<<"\n"<<format("\tmd->eventPriorities[{0}] = {1};{2}", i, substituteTerms(numReactions, "", priority), NL());

        }
        else
        {
            mSource<<"\n"<<format("\tmd->eventPriorities[{0}] = 0;{1}", i, NL());
        }
    }
    mSource<<format("}{0}{0}", NL());
}

void CModelGenerator::writeSetParameterValues(CodeBuilder& ignore, const int& numReactions)
{
    mHeader.AddFunctionExport("void", "setParameterValues(ModelData* md)");
    mSource<<"void setParameterValues(ModelData* md)\n{";


    for (int i = 0; i < mGlobalParameterList.size(); i++)
    {
        //If !+INF
        string para = format("\n\t{0} = (double){1};{2}",
                      convertSymbolToGP(mGlobalParameterList[i].name),
                      writeDouble(mGlobalParameterList[i].value),
                      NL());
        //If a parameter is INF, it means it is not initialized properly ??
        if(para.find("INF") == string::npos && para.find("NAN") == string::npos)
        {
            mSource<<para;
        }
    }

    // Initialize local parameter values
    for (int i = 0; i < numReactions; i++)
    {
        for (int j = 0; j < mLocalParameterList[i].size(); j++)
        {
            mSource<<format("\n\t_lp[{0}][{1}] = (double){2};{3}", i, j, writeDouble(mLocalParameterList[i][j].value), NL());
        }
    }

    mSource<<append("}" + NL() + NL());
}

void CModelGenerator::writeSetCompartmentVolumes(CodeBuilder& ignore)
{
    mHeader.AddFunctionExport("void", "setCompartmentVolumes(ModelData* md)");
    mSource << "void setCompartmentVolumes(ModelData* md)\n{";

    for (int i = 0; i < mCompartmentList.size(); i++)
    {
        mSource<<append("\n\t" + convertSymbolToC(mCompartmentList[i].name) + " = (double)" +
                  writeDouble(mCompartmentList[i].value) + ";" + NL());

        // at this point we also have to take care of all initial assignments for compartments as well as
        // the assignment rules on compartments ... otherwise we are in trouble :)
        stack<string> initializations = mNOM->getMatchForSymbol(mCompartmentList[i].name);
        while (initializations.size() > 0)
        {
            string term(initializations.top());
            string sub = substituteTerms(mNumReactions, "", term);
            mSource<<append("\t" + sub + ";" + NL());
            initializations.pop();
        }
    }

    mSource<<append("}" + NL() + NL());
}

void CModelGenerator::writeSetBoundaryConditions(CodeBuilder& ignore)
{
    mHeader.AddFunctionExport("void", "setBoundaryConditions(ModelData* md)");
    mSource<<"void setBoundaryConditions(ModelData* md)\n{\n";

    for (int i = 0; i < mBoundarySpeciesList.size(); i++)
    {
        if (isNullOrEmpty(mBoundarySpeciesList[i].formula))
        {
            mSource<<append("\t" + convertSpeciesToBc(mBoundarySpeciesList[i].name) + " = (double)" +
                      writeDouble(mBoundarySpeciesList[i].value) + ";" + NL());
        }
        else
        {
            mSource<<append("\t\t" + convertSpeciesToBc(mBoundarySpeciesList[i].name) + " = (double)" +
                      mBoundarySpeciesList[i].formula + ";" + NL());
        }
    }
    mSource<<append("}" + NL() + NL());
}


void CModelGenerator::writeSetInitialConditions(CodeBuilder& ignore, const int& numFloatingSpecies)
{
    mHeader.AddFunctionExport("void", "initializeInitialConditions(ModelData* md)");
    mSource<<"void initializeInitialConditions(ModelData* md)\n{";

    for (int i = 0; i < mFloatingSpeciesConcentrationList.size(); i++)
    {
        if (isNullOrEmpty(mFloatingSpeciesConcentrationList[i].formula))
        {
            mSource<<append("\n\tmd->init_" + convertSpeciesToY(mFloatingSpeciesConcentrationList[i].name) + " = (double)" +
                      writeDouble(mFloatingSpeciesConcentrationList[i].value) + ";");
        }
        else
        {
            string formula = mFloatingSpeciesConcentrationList[i].formula;
            mSource<<append("\n\tmd->init_" + convertSpeciesToY(mFloatingSpeciesConcentrationList[i].name) + " = (double) " +
                      formula + ";");
        }
    }

    mSource<<append("\n}" + NL() + NL());

    // ------------------------------------------------------------------------------
    mHeader.AddFunctionExport("void", "setInitialConditions(ModelData* md)");
    mSource<<"void setInitialConditions(ModelData* md)";
    mSource<<"\n{";

    for (int i = 0; i < numFloatingSpecies; i++)
    {
        mSource<<"\n\tmd->y[" << i << "] =  md->init_y[" << i << "];";
        mSource<<"\n\tmd->amounts[" << i << "] = md->y[" << i << "]*" <<
                  convertCompartmentToC(mFloatingSpeciesConcentrationList[i].compartmentName) << ";" << NL();
    }
    mSource<<append("}" + NL() + NL());
}

string CModelGenerator::convertSpeciesToY(const string& speciesName)
{
    int index;
    if (mFloatingSpeciesConcentrationList.find(speciesName, index))
    {
        return "y[" + toString(index) + "]";
    }

    throw new CoreException("Internal Error: Unable to locate species: " + speciesName);
}

string CModelGenerator::convertSpeciesToBc(const string& speciesName)
{
    int index;
    if (mBoundarySpeciesList.find(speciesName, index))
    {
        return "md->bc[" + toString(index) + "]";
    }

    throw CoreException("Internal Error: Unable to locate species: " + speciesName);
}

string CModelGenerator::convertCompartmentToC(const string& compartmentName)
{
    int index;
    if (mCompartmentList.find(compartmentName, index))
    {
        return "md->c[" + toString(index) + "]";
    }

    throw CoreException("Internal Error: Unable to locate compartment: " + compartmentName);
}

string CModelGenerator::convertSymbolToGP(const string& parameterName)
{
    int index;
    if (mGlobalParameterList.find(parameterName, index))
    {
        return "md->gp[" + toString(index) + "]";
    }
      throw CoreException("Internal Error: Unable to locate parameter: " + parameterName);
}

string CModelGenerator::convertSymbolToC(const string& compartmentName)
{
    int index;
    if (mCompartmentList.find(compartmentName, index))
    {
        return "md->c[" + toString(index) + "]";
    }
      throw CoreException("Internal Error: Unable to locate compartment: " + compartmentName);
}

void CModelGenerator::writeOutSymbolTables(CodeBuilder& ignore)
{
    mSource<<append("void loadSymbolTables(ModelData* md)\n{");

    int nrFuncs = 0;
    for (int i = 0; i < mFloatingSpeciesConcentrationList.size(); i++)
    {
        mSource<<format("\n\tmd->variableTable[{0}] = \"{1}\";", i, mFloatingSpeciesConcentrationList[i].name);
        nrFuncs++;
    }

    for (int i = 0; i < mBoundarySpeciesList.size(); i++)
    {
        mSource<<format("\n\tmd->boundaryTable[{0}] = \"{1}\";", i, mBoundarySpeciesList[i].name);
        nrFuncs++;
    }

    for (int i = 0; i < mGlobalParameterList.size(); i++)
    {
        string name = mGlobalParameterList[i].name;
           mSource<<format("\n\tmd->globalParameterTable[{0}] = \"{1}\";", i, mGlobalParameterList[i].name);
        nrFuncs++;
    }
    if(nrFuncs > 0)
    {
        mSource<<"\n";
    }
    mSource<<format("}{0}{0}", NL());
}

int CModelGenerator::readFloatingSpecies()
{
    // Load a reordered list into the variable list.
    StringList reOrderedList;

    if(mComputeAndAssignConsevationLaws)
    {
       reOrderedList = mLibStruct->getReorderedSpecies();
    }
    else
    {
        reOrderedList = mLibStruct->getSpecies();
    }

    StringListContainer oFloatingSpecies = mNOM->getListOfFloatingSpecies();

    for (int i = 0; i < reOrderedList.Count(); i++)
    {
        for (int j = 0; j < oFloatingSpecies.Count(); j++)
        {
            StringList oTempList = oFloatingSpecies[j];
              if(reOrderedList[i] != (const string&) oTempList[0])
              {
                  continue;
              }

            string compartmentName = mNOM->getNthFloatingSpeciesCompartmentName(j);
            bool bIsConcentration  = toBool(oTempList[2]);
            double dValue = toDouble(oTempList[1]);
            if (isNaN(dValue))
            {
                  dValue = 0;
            }

            Symbol *symbol = NULL;
            if (bIsConcentration)
            {
              symbol = new Symbol(reOrderedList[i], dValue, compartmentName);
            }
            else
            {
              int nCompartmentIndex;
              mCompartmentList.find(compartmentName, nCompartmentIndex);

              double dVolume = mCompartmentList[nCompartmentIndex].value;
              if (isNaN(dVolume))
              {
                dVolume = 1;
              }

              stringstream formula;
              formula<<toString(dValue,mDoubleFormat)<<"/ md->c["<<nCompartmentIndex<<"]";

              symbol = new Symbol(reOrderedList[i],
                  dValue / dVolume,
                  compartmentName,
                  formula.str());
            }

            if(mNOM->getModel())
            {
                Species *aSpecies = mNOM->getModel()->getSpecies(reOrderedList[i]);
                if(aSpecies)
                {
                    symbol->hasOnlySubstance = aSpecies->getHasOnlySubstanceUnits();
                    symbol->constant = aSpecies->getConstant();
                }
            }
            else
            {
                //TODO: How to report error...?
                //Log an error...
                symbol->hasOnlySubstance = false;
            }
            Log(lDebug5)<<"Adding symbol to mFloatingSpeciesConcentrationList:"<<(*symbol);
            mFloatingSpeciesConcentrationList.Add(*(symbol));
            delete symbol;
            break;
          }
          //throw RRException("Reordered Species " + reOrderedList[i] + " not found.");
      }
      return oFloatingSpecies.Count();
}

int CModelGenerator::readBoundarySpecies()
{
    int numBoundarySpecies;
    StringListContainer oBoundarySpecies = mNOM->getListOfBoundarySpecies();
    numBoundarySpecies = oBoundarySpecies.Count(); // sp1.size();
    for (int i = 0; i < numBoundarySpecies; i++)
    {
        StringList oTempList     = oBoundarySpecies[i];
        string sName             = oTempList[0];
        string compartmentName     = mNOM->getNthBoundarySpeciesCompartmentName(i);
        bool bIsConcentration     = toBool(oTempList[2]);
        double dValue             = toDouble(oTempList[1]);
        if (isNaN(dValue))
        {
            dValue = 0;
        }

        Symbol *symbol = NULL;
        if (bIsConcentration)
        {
            //Todo: memoryleak
            symbol = new Symbol(sName, dValue, compartmentName);
        }
        else
        {
            int nCompartmentIndex;
            double dVolume;
            if(mCompartmentList.find(compartmentName, nCompartmentIndex))
            {
                dVolume = mCompartmentList[nCompartmentIndex].value;
            }
            else
            {
                if (isNaN(dVolume))
                {
                    dVolume = 1;
                }
            }
            stringstream formula;
            formula<<toString(dValue, mDoubleFormat)<<"/ md->c["<<nCompartmentIndex<<"]";
            symbol = new Symbol(sName,
                                dValue / dVolume,
                                compartmentName,
                                formula.str());
        }

        if(mNOM->getModel())
        {
            Species* species = mNOM->getModel()->getSpecies(sName);
            if(species)
            {
                symbol->hasOnlySubstance = species->getHasOnlySubstanceUnits();
                symbol->constant = species->getConstant();
            }
        }
        else
        {
            //TODO: How to report error...?
            //Log an error...
            symbol->hasOnlySubstance = false;

        }
        mBoundarySpeciesList.Add(*symbol);
    }
    return numBoundarySpecies;
}

//This function is obsolete.. initialize all model data in roadrunner instead..
void CModelGenerator::writeInitModelDataFunction(CodeBuilder& ignore, CodeBuilder& source)
{
    source.Line("\n//Function to initialize the model data structure. Returns an integer indicating result");
    source.Line("int InitModelData(ModelData* md)");
    source.Line("{");
    source.Line("\tprintf(\"Size of md   %d\\n\",  (int) sizeof(md));");
//    source.Line("\tprintf(\"Size of SModelData  %d\",  (int) sizeof(SModelData));");
    source.Line("\tprintf(\"Size of ModelData   %d\\n\",  (int) sizeof(ModelData));");
//    source.Line("\tprintf(\"Size of ModelData*  %d\\n\", (int) sizeof(&ModelData));");
    source.Line("\tprintf(\"Size of ModelData.eventDelays  %d\\n\", (int) sizeof(md->eventDelays));");
    source.TLine("return 0;");
    source.Line("}");
    source.NewLine();
}

//This function is obsolete.. initialize all model data in roadrunner instead..
void CModelGenerator::writeInitFunction(CodeBuilder& ignore, CodeBuilder& source)
{
    source.Line("\n//Function to initialize the model data structure. Returns an integer indicating result");
    source.Line("int InitModel(ModelData* md)");
    source.Line("{");

//    source<<"\t"<<append("InitModelData(md);" , NL());
    source<<"\t"<<append("setCompartmentVolumes(md);" , NL());
    source<<"\t"<<append("InitializeDelays(md);" , NL());

    // Declare any eventAssignment delegates
    if (mNumEvents > 0)
    {
        for (int i = 0; i < mNumEvents; i++)
        {
            string iStr = toString(i);
            source<<append("\tmd->eventAssignments[" + iStr + "] = eventAssignment_" + iStr +";" + NL());
            source<<append("\tmd->computeEventAssignments[" + iStr + "] = (TComputeEventAssignmentDelegate) computeEventAssignment_" + iStr + ";" + NL());
            source<<append("\tmd->performEventAssignments[" + iStr + "] = (TPerformEventAssignmentDelegate) performEventAssignment_" + iStr + ";" + NL());
        }

        source<<append("\tresetEvents(md);" + NL());

        //Test to call a function
        source<<append("\tmd->eventAssignments[0](md);\n");
        source<<append(NL());
    }

    if (mNumModifiableSpeciesReferences > 0)
    {
        for (int i = 0; i < mModifiableSpeciesReferenceList.size(); i++)
        {
            source<<append("\t\tmd->sr[" + toString(i) + "] = " + writeDouble(mModifiableSpeciesReferenceList[i].value) + ";" + NL());
        }
        source<<append(NL());
    }

    source.TLine("return 0;");
    source.Line("}");
    source.NewLine();
}

void CModelGenerator::write_getModelNameFunction(CodeBuilder& ignore, CodeBuilder& source)
{
    source.Line("char* getModelName(ModelData* md)");
    source<<"{"                                         <<endl;
    source.TLine("return md->modelName;");
    source<<"}"                                         <<endl;
    source.NewLine();
}

bool CModelGenerator::saveSourceCodeToFolder(const string& folder, const string& baseName)
{
    string fName         = getFileName(baseName);
    mHeaderCodeFileName = joinPath(folder, fName);
    mHeaderCodeFileName = changeFileExtensionTo(mHeaderCodeFileName, ".h");

    ofstream outFile(mHeaderCodeFileName.c_str());
    if(!outFile)
    {
        throw(Exception("Failed to open file:" + mHeaderCodeFileName));
    }

    outFile<<getHeaderCode();
    Log(lDebug3)<<"Wrote header to file: "<<mHeaderCodeFileName;
    outFile.close();

    mSourceCodeFileName = changeFileExtensionTo(mHeaderCodeFileName, ".c");
    outFile.open(mSourceCodeFileName.c_str());

    //We don't know the name of the file until here..
    //Write an include statement to it..
    vector<string> fNameParts = splitString(mSourceCodeFileName,"\\");
    string headerFName = fNameParts[fNameParts.size() - 1];

    headerFName = changeFileExtensionTo(headerFName, ".h");
    outFile<<"#include \""<<getFileName(headerFName)<<"\"\n"<<endl;
    outFile<<getSourceCode();
    outFile.close();
    Log(lDebug3)<<"Wrote source code to file: "<<mSourceCodeFileName;

    return true;
}

string CModelGenerator::convertUserFunctionExpression(const string& equation)
{
    if(!equation.size())
    {
        Log(lError)<<"The equation string supplied to "<<__FUNCTION__<<" is empty";
        return "";
    }
    Scanner s;
    stringstream ss;
    ss<<equation;
    s.AssignStream(ss);
    s.startScanner();
    s.nextToken();
    CodeBuilder  mSource;

    try
    {
        while (s.token() != CodeTypes::tEndOfStreamToken)
           {
            string theToken = s.tokenString;
               switch (s.token())
               {
                case CodeTypes::tWordToken:
                    if(theToken == "pow")
                    {
                        mSource<<append("spf_pow");
                    }
                    else if(theToken == "sqrt")
                    {
                        mSource<<append("sqrt");
                      }
                    else if(theToken == "log")
                    {
                        mSource<<append("spf_log");
                    }
                    else if(theToken == "log10")
                    {
                        mSource<<append("Log10");
                    }
                    else if(theToken == "floor")
                    {
                        mSource<<append("spf_floor");
                    }
                    else if(theToken == "ceil")
                    {
                        mSource<<append("spf_ceil");
                    }
                    else if(theToken == "factorial")
                    {
                        mSource<<append("spf_factorial");
                    }
                    else if(theToken == "exp")
                    {
                        mSource<<append("Math.Exp");
                    }
                    else if(theToken == "sin")
                    {
                        mSource<<append("sin");
                    }
                    else if(theToken == "cos")
                    {
                        mSource<<append("cos");
                    }
                    else if(theToken == "tan")
                    {
                        mSource<<append("tan");
                    }
                    else if(theToken == "abs")
                    {
                        mSource<<append("spf_abs");
                    }
                    else if(theToken == "asin")
                    {
                        mSource<<append("asin");
                    }
                    else if(theToken == "acos")
                    {
                        mSource<<append("acos");
                    }
                    else if(theToken == "atan")
                    {
                        mSource<<append("atan");
                    }
                    else if(theToken == "sec")
                    {
                        mSource<<append("MathKGI.Sec");
                    }
                    else if(theToken == "csc")
                    {
                        mSource<<append("MathKGI.Csc");
                    }
                    else if(theToken == "cot")
                    {
                        mSource<<append("MathKGI.Cot");
                    }
                    else if(theToken == "arcsec")
                    {
                        mSource<<append("MathKGI.Asec");
                    }
                    else if(theToken == "arccsc")
                    {
                        mSource<<append("MathKGI.Acsc");
                    }
                    else if(theToken == "arccot")
                    {
                        mSource<<append("MathKGI.Acot");
                    }
                    else if(theToken == "sinh")
                    {
                        mSource<<append("Math.Sinh");
                    }
                    else if(theToken == "cosh")
                    {
                        mSource<<append("Math.Cosh");
                    }
                    else if(theToken == "tanh")
                    {
                        mSource<<append("Math.Tanh");
                    }
                    else if(theToken == "arcsinh")
                    {
                        mSource<<append("MathKGI.Asinh");
                    }
                    else if(theToken == "arccosh")
                    {
                        mSource<<append("MathKGI.Acosh");
                    }
                    else if(theToken == "arctanh")
                    {
                        mSource<<append("MathKGI.Atanh");
                    }
                    else if(theToken == "sech")
                    {
                        mSource<<append("MathKGI.Sech");
                    }
                    else if(theToken == "csch")
                    {
                        mSource<<append("MathKGI.Csch");
                    }
                    else if(theToken == "coth")
                    {
                        mSource<<append("MathKGI.Coth");
                    }
                    else if(theToken == "arcsech")
                    {
                        mSource<<append("MathKGI.Asech");
                    }
                    else if(theToken == "arccsch")
                    {
                        mSource<<append("MathKGI.Acsch");
                    }
                    else if(theToken == "arccoth")
                    {
                               mSource<<append("MathKGI.Acoth");
                    }
                    else if(theToken == "pi")
                    {
                        mSource<<append("PI");
                    }
                    else if(theToken == "exponentiale")
                    {
                        mSource<<append("Math.E");
                    }
                    else if(theToken == "avogadro")
                    {
                        mSource<<append("6.02214179e23");
                    }
                    else if(theToken == "true")
                    {
                        mSource<<append("1.0");
                    }
                    else if(theToken == "false")
                    {
                        mSource<<append("0.0");
                    }
                    else if(theToken == "gt")
                    {
                        mSource<<append("spf_gt");
                    }
                    else if(theToken == "lt")
                    {
                        mSource<<append("spf_lt");
                    }
                    else if(theToken == "eq")
                    {
                        mSource<<append("spf_eq");
                    }
                    else if(theToken == "neq")
                    {
                        mSource<<append("spf_neq");
                    }
                    else if(theToken == "geq")
                    {
                        mSource<<append("spf_geq");
                    }
                    else if(theToken == "leq")
                    {
                        mSource<<append("spf_leq");
                    }
                    else if(theToken == "and")
                    {
                        mSource<<append("supportFunction._and");
                    }
                    else if(theToken == "or")
                    {
                        mSource<<append("supportFunction._or");
                    }
                    else if(theToken == "not")
                    {
                        mSource<<append("supportFunction._not");
                    }
                    else if(theToken == "xor")
                    {
                        mSource<<append("supportFunction._xor");
                    }
                    else if(theToken == "root")
                    {
                        mSource<<append("spf_root");
                    }
                    else if (theToken == "squarewave")
                    {
                        mSource<<append("spf_squarewave");
                    }
                    else if(theToken == "piecewise")
                    {
                        mSource<<append("spf_piecewise");
                    }
                    else if (!mFunctionParameters.Contains(s.tokenString))
                    {
                        throw Exception("Token '" + s.tokenString + "' not recognized.");
                    }
                    else
                    {
                        mSource<<append(s.tokenString);
                    }

                break; //Word token

                   case CodeTypes::tDoubleToken:
                       mSource<<append(writeDouble(s.tokenDouble));
                       break;
                   case CodeTypes::tIntToken:
                    mSource<<append((int) s.tokenInteger);
                       break;
                   case CodeTypes::tPlusToken:
                   mSource<<append("+");
                   break;
                   case CodeTypes::tMinusToken:
                   mSource<<append("-");
                   break;
                   case CodeTypes::tDivToken:
                   mSource<<append("/");
                   break;
                   case CodeTypes::tMultToken:
                   mSource<<append(mFixAmountCompartments);
                   break;
                   case CodeTypes::tPowerToken:
                   mSource<<append("^");
                   break;
                   case CodeTypes::tLParenToken:
                   mSource<<append("(");
                   break;
                   case CodeTypes::tRParenToken:
                   mSource<<append(")");
                   break;
                   case CodeTypes::tCommaToken:
                   mSource<<append(",");
                   break;
                   case CodeTypes::tEqualsToken:
                   mSource<<append(" = ");
                   break;
                   case CodeTypes::tTimeWord1:
                   mSource<<append("time");
                   break;
                   case CodeTypes::tTimeWord2:
                   mSource<<append("time");
                   break;
                   case CodeTypes::tTimeWord3:
                   mSource<<append("time");
                   break;
                   case CodeTypes::tAndToken:
                   mSource<<append("spf_and");
                   break;
                   case CodeTypes::tOrToken:
                   mSource<<append("spf_or");
                   break;
                   case CodeTypes::tNotToken:
                   mSource<<append("spf_not");
                   break;
                   case CodeTypes::tLessThanToken:
                   mSource<<append("spf_lt");
                   break;
                   case CodeTypes::tLessThanOrEqualToken:
                   mSource<<append("spf_leq");
                   break;
                   case CodeTypes::tMoreThanOrEqualToken:
                   mSource<<append("spf_geq");
                   break;
                   case CodeTypes::tMoreThanToken:
                   mSource<<append("spf_gt");
                   break;
                   case CodeTypes::tXorToken:
                   mSource<<append("spf_xor");
                   break;
                   default:
                   stringstream msg;
                   msg<< "Unknown token in convertUserFunctionExpression: " << s.tokenToString(s.token()) <<
                           "Exception raised in Module:roadRunner, Method:convertUserFunctionExpression";
                   throw Exception(msg.str());
               }
               s.nextToken();
        }
    }
    catch (const Exception& e)
    {
       throw CoreException(e.Message());
    }
    return mSource.ToString();
}

void CModelGenerator::substituteEquation(const string& reactionName, Scanner& s, CodeBuilder& mSource)
{
    string theToken(s.tokenString);
    if(theToken == "pow")
    {
        mSource<<append("spf_pow");
    }
    else if(theToken == "sqrt")
    {
        mSource<<append("sqrt");
    }
    else if(theToken == "log")
    {
        mSource<<append("spf_log");
    }
    else if(theToken == "floor")
    {
        mSource<<append("spf_floor");
    }
    else if(theToken == "ceil")
    {
        mSource<<append("spf_ceil");
    }
    else if(theToken == "factorial")
    {
        mSource<<append("spf_factorial");
    }
    else if(theToken == "log10")
    {
        mSource<<append("spf_log10");
    }
    else if(theToken == "exp")
    {
        mSource<<append("spf_exp");
    }
    else if(theToken == "abs")
    {
        mSource<<append("spf_abs");
    }
    else if(theToken == "sin")
    {
        mSource<<append("spf_sin");
    }
    else if(theToken == "cos")
    {
        mSource<<append("cos");
    }
    else if(theToken == "tan")
    {
        mSource<<append("tan");
    }
    else if(theToken == "asin")
    {
        mSource<<append("asin");
    }
    else if(theToken == "acos")
    {
        mSource<<append("acos");
    }
    else if(theToken == "atan")
    {
        mSource<<append("atan");
    }
    else if(theToken == "sec")
    {
        mSource<<append("sec");
    }
    else if(theToken == "csc")
    {
        mSource<<append("csc");
    }
    else if(theToken == "cot")
    {
        mSource<<append("cot");
    }
    else if(theToken == "arcsec")
    {
        mSource<<append("asec");
    }
    else if(theToken == "arccsc")
    {
        mSource<<append("arccsc");
    }
    else if(theToken == "arccot")
    {
        mSource<<append("arccot");
    }
    else if(theToken == "sinh")
    {
        mSource<<append("sinh");
    }
    else if(theToken == "cosh")
    {
        mSource<<append("cosh");
    }
    else if(theToken == "tanh")
    {
        mSource<<append("tanh");
    }
    else if(theToken == "arcsinh")
    {
        mSource<<append("arcsinh");
    }
    else if(theToken == "arccosh")
    {
        mSource<<append("arccosh");
    }
    else if(theToken == "arctanh")
    {
        mSource<<append("arctanh");
    }
    else if(theToken == "sech")
    {
        mSource<<append("sech");
    }
    else if(theToken == "csch")
    {
        mSource<<append("csch");
    }
    else if(theToken == "coth")
    {
        mSource<<append("coth");
    }
    else if(theToken == "arcsech")
    {
        mSource<<append("arcsech");
    }
    else if(theToken == "arccsch")
    {
        mSource<<append("arccsch");
    }
    else if(theToken == "arccoth")
    {
        mSource<<append("arccoth");
    }
    else if(theToken == "pi")
    {
        mSource<<append("PI");
    }
    else if(theToken == "avogadro")
    {
        mSource<<append("6.02214179e23");
    }
    else if(theToken == "exponentiale")
    {
        mSource<<append("E");
    }
    else if(theToken == "true")
    {
        //mSource<<append("true");
        mSource<<append("1.0");
    }
    else if(theToken == "false")
    {
        //mSource<<append("false");
        mSource<<append("0.0");
    }
    else if(theToken == "NaN")
    {
        mSource<<append("NaN");
    }
    else if(theToken == "INF")
    {
        mSource<<append("INF");
    }
    else if(theToken == "geq")
    {
        mSource<<append("spf_geq");
    }
    else if(theToken == "leq")
    {
        mSource<<append("spf_leq");
    }
    else if(theToken == "gt")
    {
        mSource<<append("spf_gt");
    }
    else if(theToken == "lt")
    {
        mSource<<append("spf_lt");
    }
    else if(theToken == "eq")
    {
        mSource<<append("spf_eq");
    }
    else if(theToken == "neq")
    {
        mSource<<append("spf_neq");
    }
    else if(theToken == "and")
    {
        mSource<<append("spf_and");
    }
    else if(theToken == "or")
    {
        mSource<<append("spf_or");
    }
    else if(theToken == "not")
    {
        mSource<<append("spf_not");
    }
    else if(theToken == "xor")
    {
        mSource<<append("spf_xor");
    }
    else if(theToken == "root")
    {
        mSource<<append("spf_root");
    }
    else if(theToken == "squarewave")
    {
        mSource<<append("spf_squarewave");
    }
    else if(theToken == "piecewise")
    {
        mSource<<append("spf_piecewise");
    }
    else if(theToken == "delay")
    {
        mSource<<append("spf_delay");
        Log(lWarning)<<"RoadRunner does not yet support delay differential equations in SBML, they will be ignored (i.e. treated as delay = 0).";
    }
    else
    {
        bool bReplaced = false;
        int index;
        if (mReactionList.find(reactionName, index))
        {
            int nParamIndex = 0;
            if (mLocalParameterList[index].find(s.tokenString, nParamIndex))
            {
                mSource<<append("_lp[" + toString(index) + "][" + toString(nParamIndex) + "]");
                bReplaced = true;
            }
        }

        if (mBoundarySpeciesList.find(s.tokenString, index))
        {
            mSource<<append("_bc[" + toString(index) + "]");
            bReplaced = true;
        }
        if (!bReplaced &&
            (mFunctionParameters.Count() != 0 && !mFunctionParameters.Contains(s.tokenString)))
        {
            throw Exception("Token '" + s.tokenString + "' not recognized.");
        }
    }
}

void CModelGenerator::substituteWords(const string& reactionName, bool bFixAmounts, Scanner& s, CodeBuilder& mSource)
{
    // Global parameters have priority
    int index;
    if (mGlobalParameterList.find(s.tokenString, index))
    {
        mSource<<format("md->gp[{0}]", index);
    }
    else if (mBoundarySpeciesList.find(s.tokenString, index))
    {
        mSource<<format("md->bc[{0}]", index);

        Symbol symbol = mBoundarySpeciesList[index];
        if (symbol.hasOnlySubstance)
        {
            // we only store concentration for the boundary so we better
            // fix that.
            int nCompIndex = 0;
            if (mCompartmentList.find(symbol.compartmentName, nCompIndex))
            {
                mSource<<format("{0}_c[{1}]", mFixAmountCompartments, nCompIndex);
            }
        }
    }
    else if (mFloatingSpeciesConcentrationList.find(s.tokenString, index))
    {
        Symbol floating1 = mFloatingSpeciesConcentrationList[index];
        if (floating1.hasOnlySubstance)
        {
            mSource<<format("md->amounts[{0}]", index);
        }
        else
        {
            mSource<<format("md->y[{0}]", index);
        }
    }
    else if (mCompartmentList.find(s.tokenString, index))
    {
        mSource<<format("md->c[{0}]", index);
    }
    else if (mFunctionNames.Contains(s.tokenString))
    {
        mSource<<format("{0} ", s.tokenString);
    }
    else if (mModifiableSpeciesReferenceList.find(s.tokenString, index))
    {
        mSource<<format("md->sr[{0}]", index);
    }
    else if (mReactionList.find(s.tokenString, index))
    {
        mSource<<format("md->rates[{0}]", index);
    }
    else
    {
        substituteEquation(reactionName, s, mSource);
    }
}

void CModelGenerator::substituteToken(const string& reactionName, bool bFixAmounts, Scanner& s, CodeBuilder& mSource)
{
    string aToken = s.tokenString;
    CodeTypes codeType = s.token();
    switch(codeType)
    {
        case CodeTypes::tWordToken:
        case CodeTypes::tExternalToken:
        case CodeTypes::tExtToken:
            substituteWords(reactionName, bFixAmounts, s, mSource);
            break;

        case CodeTypes::tDoubleToken:
            mSource<<append("(double) " + writeDouble(s.tokenDouble));
            break;
        case CodeTypes::tIntToken:
            mSource<<append("(double)" + writeDouble((double)s.tokenInteger));
            break;
        case CodeTypes::tPlusToken:
            mSource<<format("+{0}\t", NL());
            break;
        case CodeTypes::tMinusToken:
            mSource<<format("-{0}\t", NL());
            break;
        case CodeTypes::tDivToken:
            mSource<<format("/{0}\t", NL());
            break;
        case CodeTypes::tMultToken:
            mSource<<format("*{0}\t", NL());
            break;
        case CodeTypes::tPowerToken:
            mSource<<format("^{0}\t", NL());
            break;
        case CodeTypes::tLParenToken:
            mSource<<append("(");
            break;
        case CodeTypes::tRParenToken:
            mSource<<format("){0}\t", NL());
            break;
        case CodeTypes::tCommaToken:
            mSource<<append(",");
            break;
        case CodeTypes::tEqualsToken:
            mSource<<format(" = {0}\t", NL());
            break;
        case CodeTypes::tTimeWord1:
            mSource<<append("md->time");
            break;
        case CodeTypes::tTimeWord2:
            mSource<<append("md->time");
            break;
        case CodeTypes::tTimeWord3:
            mSource<<append("md->time");
            break;
        case CodeTypes::tAndToken:
            mSource<<format("{0}spf_and", NL());
            break;
        case CodeTypes::tOrToken:
            mSource<<format("{0}spf_or", NL());
            break;
        case CodeTypes::tNotToken:
            mSource<<format("{0}spf_not", NL());
            break;
        case CodeTypes::tLessThanToken:
            mSource<<format("{0}spf_lt", NL());
            break;
        case CodeTypes::tLessThanOrEqualToken:
            mSource<<format("{0}spf_leq", NL());
            break;
        case CodeTypes::tMoreThanOrEqualToken:
            mSource<<format("{0}spf_geq", NL());
            break;
        case CodeTypes::tMoreThanToken:
            mSource<<format("{0}spf_gt", NL());
            break;
        case CodeTypes::tXorToken:
            mSource<<format("{0}spf_xor", NL());
            break;
        default:
        string aToken = s.tokenToString(s.token());
        Exception ae = Exception(
                 format("Unknown token in substituteTerms: {0}", aToken,
                 "Exception raised in Module:roadRunner, Method:substituteTerms"));
         throw ae;
    }
}

bool CModelGenerator::generateModelCode(const string& sbml, const string& modelName, bool computeAndAssignConsevationLaws)
{
    if(sbml.size())
    {
        mCurrentSBML = sbml;
    }

    string modelCode = generateModelCode(mCurrentSBML, computeAndAssignConsevationLaws);

    if(!modelCode.size())
    {
        Log(lError)<<"Failed to generate model code";
        return false;
    }

    // just use the temp folder we're given
    //*string tempFileFolder;
    //*if(mSimulation)
    //*{
    //*    tempFileFolder = mSimulation->GetTempDataFolder();
    //*}
    //*else
    //*{
    //*    tempFileFolder = mTempFileFolder;
    //*}

    if(!saveSourceCodeToFolder(mTempFileFolder, modelName))
    {
        Log(lError)<<"Failed saving generated source code";
    }

    return true;
}

bool CModelGenerator::compileModel()
{
    if(!compileCurrentModel())
    {
        Log(lError)<<"Failed compiling model";
        return false;
    }

    return true;
}

bool CModelGenerator::compileCurrentModel()
{
    //*CModelGenerator *codeGen = dynamic_cast<CModelGenerator*>(mModelGenerator);
    //*if(!codeGen)
    //*{
    //*    //CodeGenerator has not been allocaed
    //*    Log(lError)<<"Generate code before compiling....";
    //*    return false;
    //*}

    //Compile the model
    if(!mCompiler.compileSource(getSourceCodeFileName()))
    {
        Log(lError)<<"Model failed compilation";
        return false;
    }
    Log(lDebug)<<"Model compiled successfully. ";
    Log(lDebug)<<mModelLib->getFullFileName()<<" was created";
    return true;
}


bool CModelGenerator::setTemporaryDirectory(const string& path)
{
    if(folderExists(path))
    {
        Log(lDebug)<<"Setting temp file folder to "<< path;
        mCompiler.setOutputPath(path);
        mTempFileFolder = path;
        return true;
    }
    else
    {
        stringstream msg;
        msg<<"The folder: "<<path<<" don't exist...";
        Log(lError)<<msg.str();

        CoreException e(msg.str());
        throw(e);
        //      return false;
    }
}

bool CModelGenerator::initializeModel()
{
    if(!mModel)
    {
        throw Exception("CModelGenerator::initializeModel() called without model");
    }

    //*mConservedTotalChanged = false;
    mModel->setCompartmentVolumes();
    mModel->initializeInitialConditions();
    mModel->setParameterValues();
    mModel->setCompartmentVolumes();
    mModel->setBoundaryConditions();
    mModel->setInitialConditions();
    mModel->convertToAmounts();
    mModel->evalInitialAssignments();

    mModel->computeRules(mModel->getModelData().y, mModel->getModelData().ySize);
    mModel->convertToAmounts();

    return true;
}


ExecutableModel *CModelGenerator::createModel(const string& sbml, LibStructural *ls, NOMSupport *nom,
        bool forceReCompile, bool computeAndAssignConsevationLaws)
{
    mLibStruct = ls;
    mNOM = nom;
    mCurrentSBML = sbml;
    mModelLib = new ModelSharedLibrary();



    //clear temp folder of roadrunner generated files, only if roadRunner instance == 1
    Log(lDebug)<<"Loading SBML into simulator";
    if (!sbml.size())
    {
        throw(CoreException("SBML string is empty!"));
    }

    // the RoadRunner object has already loaded the libstruct and nom.
    //*loadSBMLIntoLibStruct(sbml);
    //*{   //Scope for Mutex
    //*    Mutex::ScopedLock lock(mLibSBMLMutex);
    //*    loadSBMLIntoNOM(sbml);  //There is something in here that is not threadsafe... causes crash with multiple threads, without mutex
    //*}

    //    string modelName  = createModelName(mCurrentSBMLFileName);
    string modelName = getMD5(sbml);

    //Check if model has been compiled
    mModelLib->setPath(mTempFileFolder);

    //Creates a name for the shared lib
    mModelLib->createName(modelName);
    if(forceReCompile)
    {
        //If the dll is loaded.. unload it..
        if (mModelLib->isLoaded())
        {
            mModelLib->unload();
        }
    }

    generateModelCode(sbml, modelName, computeAndAssignConsevationLaws);

    Mutex::ScopedLock lock(mCompileMutex);
    try
    {
        //Can't have multiple threads compiling to the same dll at the same time..
        if(!fileExists(mModelLib->getFullFileName()) || forceReCompile == true)
        {
            if(!compileModel())
            {
                Log(lError)<<"Failed to generate and compile model";
                return 0;
            }

            if(!mModelLib->load())
            {
                Log(lError)<<"Failed to load model DLL";
                return 0;
            }
        }
        else
        {
            Log(lDebug)<<"Model compiled files already generated.";
            if(!mModelLib->isLoaded())
            {
                if(!mModelLib->load())
                {
                    Log(lError)<<"Failed to load model DLL";
                    return 0;
                }
            }
            else
            {
                Log(lDebug)<<"Model lib is already loaded.";
            }
        }

    }//End of scope for compile Mutex
    catch(const Exception& ex)
    {
        Log(lError)<<"Compiler problem: "<<ex.what();
    }

    //Create a model
    if(mModelLib->isLoaded())
    {
        ExecutableModel *rrCModel = new CompiledExecutableModel(*this, mModelLib);
        mModel = rrCModel;
    }
    else
    {
        Log(lError)<<"Failed to create model from DLL";
        mModel = NULL;
    }

    //Finally intitilaize the model..
    if(!initializeModel())
    {
        Log(lError)<<"Failed Initializing C Model";
        return 0;
    }

    // returning the completed model, we're done with it.
    ExecutableModel *result = mModel;
    mModel = 0;
    mModelLib = 0;

    // clear state
    ModelGenerator::reset();

    return result;
}

Compiler* CModelGenerator::getCompiler()
{
    return &mCompiler;
}


bool CModelGenerator::setCompiler(const string& compiler)
{
    return mCompiler.setCompiler(compiler);
}

string CModelGenerator::getTemporaryDirectory()
{
    return mTempFileFolder;
}

}//Namespace

