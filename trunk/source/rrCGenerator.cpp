#ifdef USE_PCH
#include "rr_pch.h"
#endif
#pragma hdrstop
#include <algorithm>
#include <cctype>
#include "sbml/Model.h"
#include "sbml/SBMLDocument.h"
#include "rrCSharpGenerator.h"
#include "rr-libstruct/lsLibstructural.h"
#include "rrArrayList.h"
#include "rrStringUtils.h"
#include "rrUtils.h"
#include "rrRule.h"
#include "rrScanner.h"
#include "rrLogger.h"
#include "rrRoadRunner.h"
#include "rrException.h"
#include "rrCGenerator.h"
//---------------------------------------------------------------------------

using namespace std;
using namespace ls;

namespace rr
{
//CGenerator::CGenerator(RoadRunner* rr)
CGenerator::CGenerator(NOMSupport& nom)
:
ModelGenerator(nom)
{}

CGenerator::~CGenerator(){}

int    CGenerator::GetNumberOfFloatingSpecies()
{
    return floatingSpeciesConcentrationList.size();    //Todo: is there a list of floating species?
}

string CGenerator::GetHeaderCode()
{
    return mHeader.ToString();
}

string CGenerator::GetSourceCode()
{
    return mSource.ToString();
}

string CGenerator::GetHeaderCodeFileName()
{
    return mHeaderCodeFileName;
}

string CGenerator::GetSourceCodeFileName()
{
    return mSourceCodeFileName;
}


// Generates the Model Code from the SBML string
string CGenerator::generateModelCode(const string& sbmlStr, const bool& _computeAndAssignConsevationLaws)
{
	mComputeAndAssignConsevationLaws  = _computeAndAssignConsevationLaws;
    Log(lDebug4)<<"Entering CGenerators generateModelCode(string) function";
    StringList  Warnings;
    CodeBuilder ignore;     //The Write functions below are inherited with a CodeBuilder in the
                            //prototype that is not to be used..

    //Clear header and source file objects...
    mHeader.Clear();
    mSource.Clear();
    mNOM.Reset();
    string sASCII = mNOM.convertTime(sbmlStr, "time");

    Log(lDebug4)<<"Loading SBML into NOM";
    mNOM.loadSBML(sASCII.c_str(), "time");

    mModelName = mNOM.getModelName();
    if(!mModelName.size())
    {
        Log(lWarning)<<"Model name is empty. ModelName is assigned 'NameNotSet'.";
        mModelName = "NameNotSet";
    }

    Log(lInfo)<<"Processing model: "<<mModelName;
    mNumReactions = mNOM.getNumReactions();

    Log(lDebug3)<<"Number of reactions:"<<mNumReactions;

    globalParameterList.Clear();
    ModifiableSpeciesReferenceList.Clear();
    localParameterList.reserve(mNumReactions);
    reactionList.Clear();
    boundarySpeciesList.Clear();
    floatingSpeciesConcentrationList.Clear();
    floatingSpeciesAmountsList.Clear();
    compartmentList.Clear();
    conservationList.Clear();
    mfunctionNames.empty();
    mfunctionParameters.empty();

    LibStructural* libStruct = LibStructural::getInstance();
    string msg;
    try
    {
        Log(lDebug3)<<"Loading sbml into StructAnalysis";
        if(!libStruct)
        {
            throw;
        }
        msg = libStruct->loadSBML(sASCII);
        if(!msg.size())
        {
            Log(lError)<<"Failed loading sbml into StructAnalysis";
        }
    }
    catch(...)
    {
        Log(lError)<<"Failed loading sbml into StructAnalysis";
    }


    Log(lDebug3)<<"Message from StructAnalysis.LoadSBML function\n"<<msg;

    //if (mRR && mRR->mComputeAndAssignConservationLaws)
	if(mComputeAndAssignConsevationLaws)
    {
        mNumIndependentSpecies = libStruct->getNumIndSpecies();
        independentSpeciesList = libStruct->getIndependentSpecies();
        dependentSpeciesList   = libStruct->getDependentSpecies();
    }
    else
    {
        mNumIndependentSpecies = mLibStruct->getNumSpecies();
        independentSpeciesList = mLibStruct->getSpecies();
    }

    // Load the compartment array (name and value)
    mNumCompartments            = ReadCompartments();

    // Read FloatingSpecies
    mNumFloatingSpecies         = ReadFloatingSpecies();
    mNumDependentSpecies        = mNumFloatingSpecies - mNumIndependentSpecies;

    // Load the boundary species array (name and value)
    mNumBoundarySpecies     = ReadBoundarySpecies();

    // Get all the parameters into a list, global and local
    mNumGlobalParameters     = ReadGlobalParameters();
    mNumModifiableSpeciesReferences = ReadModifiableSpeciesReferences();

    // Load up local parameters next
    ReadLocalParameters(mNumReactions, mLocalParameterDimensions, mTotalLocalParmeters);
    mNumEvents = mNOM.getNumEvents();

    //Write model to String builder...
    WriteClassHeader(ignore);
    WriteOutVariables(ignore);
    WriteOutSymbolTables(ignore);

    ///// Write non exports
    mHeader.NewLine("\n//NON - EXPORTS ========================================");
    mHeader.AddFunctionProto("void", "InitializeDelays()");

    ///// Start of exported functions
    mHeader.NewLine("\n//EXPORTS ========================================");
    mHeader.AddFunctionExport("int", "InitModel()");
    mHeader.AddFunctionExport("char*", "GetModelName()");
    ///////////////

    WriteResetEvents(ignore, mNumEvents);
    WriteSetConcentration(ignore);
    WriteGetConcentration(ignore);
    WriteConvertToAmounts(ignore);
    WriteConvertToConcentrations(ignore);
    WriteProperties(ignore);
    WriteAccessors(ignore);
    WriteUserDefinedFunctions(ignore);
    WriteSetInitialConditions(ignore, mNumFloatingSpecies);
    WriteSetBoundaryConditions(ignore);
    WriteSetCompartmentVolumes(ignore);
    WriteSetParameterValues(ignore, mNumReactions);
    WriteComputeConservedTotals(ignore, mNumFloatingSpecies, mNumDependentSpecies);

    // Get the L0 matrix
    int nrRows;
    int nrCols;

    ls::DoubleMatrix* aL0 = InitializeL0(nrRows, nrCols);     //Todo: What is this doing? answer.. it is used below..
    WriteUpdateDependentSpecies(ignore, mNumIndependentSpecies, mNumDependentSpecies, *aL0);
    int numOfRules = WriteComputeRules(ignore, mNumReactions);

    WriteComputeAllRatesOfChange(ignore, mNumIndependentSpecies, mNumDependentSpecies, *aL0);
	delete aL0;
    WriteComputeReactionRates(ignore, mNumReactions);
    WriteEvalModel(ignore, mNumReactions, mNumIndependentSpecies, mNumFloatingSpecies, numOfRules);
    WriteEvalEvents(ignore, mNumEvents, mNumFloatingSpecies);
    WriteEventAssignments(ignore, mNumReactions, mNumEvents);
    WriteEvalInitialAssignments(ignore, mNumReactions);
    WriteTestConstraints(ignore);

    WriteInitFunction(mHeader, mSource);

    mHeader<<"\n\n#endif //modelH"<<NL();
    return mHeader.ToString() + mSource.ToString();
}

void CGenerator::WriteClassHeader(CodeBuilder& ignore)
{
    //Create c code header file....
    mHeader<<"#ifndef modelH"<<endl;
    mHeader<<"#define modelH"<<endl;
    mHeader<<"#include <stdio.h>"<<endl;
    mHeader<<"#include <stdbool.h>"<<endl;
    mHeader<<"#include \"rrCExporter.h\"\t             //Export Stuff."<<endl;


    mHeader<<Append("//************************************************************************** " + NL());
    mHeader<<"//Number of floating species: "<<floatingSpeciesConcentrationList.size()<<endl;
    for (int i = 0; i < floatingSpeciesConcentrationList.size(); i++)
    {
        mHeader<<"\t// y["<<i<<"] = "<<floatingSpeciesConcentrationList[i].name<<endl;//{2}", NL());
    }

    mHeader<<Append("//************************************************************************** " + NL());
    mHeader<<Append(NL());
//    mHeader<<Format("D_S struct TModel{0}", NL());
//    mHeader<<Append("{" + NL());

    //Header of the source file...
    mSource<<"#include <math.h>"<<endl;
    mSource<<"#include <stdio.h>"<<endl;
    mSource<<"#include \"rrSupport.h\"\t     //Supportfunctions for event handling.."<<endl;
}

void CGenerator::WriteOutVariables(CodeBuilder& ignore)
{
    mHeader.FormatVariable("D_S char*",                   			"mModelName");
    mHeader.FormatVariable("D_S char**",                            "mWarnings");
    mHeader.FormatArray("D_S double",                               "_gp",                             mNumGlobalParameters +
                                                                                                    mTotalLocalParmeters,                       "Vector containing all the global parameters in the System  ");

    if(mNumModifiableSpeciesReferences)
    {
        mHeader.FormatArray("D_S double",                           "_sr",                             mNumModifiableSpeciesReferences,            "Vector containing all the modifiable species references  ");
    }

    //Arrays
    mHeader.FormatArray("D_S double*",                              "_lp",                             mNumReactions,                              "Vector containing all the local parameters in the System  ");
    mHeader.FormatArray("D_S double",                               "_y",                              floatingSpeciesConcentrationList.size(),    "Vector containing the concentrations of all floating species");
    mHeader.FormatArray("D_S double",                               "_init_y",                         floatingSpeciesConcentrationList.Count(),   "Vector containing the initial concentrations of all floating species");
    mHeader.FormatArray("D_S double",                               "_amounts",                        floatingSpeciesConcentrationList.size(),    "Vector containing the amounts of all floating species ");
    mHeader.FormatArray("D_S double",                               "_bc",                             mNumBoundarySpecies,                        "Vector containing all the boundary species concentration values");
    mHeader.FormatArray("D_S double",                               "_c",                              mNumCompartments                         ,  "Vector containing all the compartment values   ");
    mHeader.FormatArray("D_S double",                               "_dydt",                           floatingSpeciesConcentrationList.size() ,   "Vector containing rates of changes of all species   ");
    mHeader.FormatArray("D_S double",                               "_rates",                          mNumReactions                             , "Vector containing the rate laws of all reactions    ");
    mHeader.FormatArray("D_S double",                               "_ct",                             mNumDependentSpecies                     ,  "Vector containing values of all conserved sums      ");
    mHeader.FormatArray("D_S double",                               "mEventTests",                     mNumEvents                                 ,"Vector containing results of any event tests        ");

    mHeader<<"\ttypedef double (*TEventDelayDelegate)();"<<endl;
    mHeader.FormatArray("TEventDelayDelegate",                      "mEventDelay",                        mNumEvents                                 ,"Array of trigger function pointers");
    mHeader.AddFunctionExport("TEventDelayDelegate*",               "GetEventDelays()");
    mHeader.FormatArray("bool",                                     "_eventType",                      mNumEvents                                , "Array holding the status whether events are useValuesFromTriggerTime or not");
    mHeader.FormatArray("bool",                                     "_eventPersistentType",            mNumEvents                                , "Array holding the status whether events are persitstent or not");
    mHeader.FormatVariable("D_S double",                    		"mTime");
    mHeader.FormatVariable("D_S int",                               "numIndependentVariables");
    mHeader.FormatVariable("D_S int",                               "numDependentVariables");
    mHeader.FormatVariable("D_S int",                               "numTotalVariables");
    mHeader.FormatVariable("D_S int",                               "numBoundaryVariables");
    mHeader.FormatVariable("D_S int",                               "numGlobalParameters");
    mHeader.FormatVariable("D_S int",                               "numCompartments");
    mHeader.FormatVariable("D_S int",                               "numReactions");
    mHeader.FormatVariable("D_S int",                               "numRules");
    mHeader.FormatVariable("D_S int",                               "numEvents");

    mHeader.FormatArray("D_S char*",                                "variableTable",                floatingSpeciesConcentrationList.size());
    mHeader.FormatArray("D_S char*",                                "boundaryTable",                boundarySpeciesList.size());
    mHeader.FormatArray("D_S char*",                                "globalParameterTable",         globalParameterList.size());
    mHeader.FormatArray("D_S int",                                  "localParameterDimensions",     mNumReactions );

    mHeader<<"\ntypedef void __cdecl (*TEventAssignmentDelegate)();"<<endl;
    mHeader.FormatVariable("D_S TEventAssignmentDelegate*",         "_eventAssignments","");
    mHeader.AddFunctionExport("TEventAssignmentDelegate*",          "Get_eventAssignments()");
    mHeader.FormatArray("D_S double",                               "_eventPriorities", mNumEvents);

    mHeader<<"\ntypedef double* (*TComputeEventAssignmentDelegate)();"<<endl;
    mHeader.AddFunctionExport("TComputeEventAssignmentDelegate*",   "Get_computeEventAssignments()");
    mHeader.FormatVariable("D_S TComputeEventAssignmentDelegate*",  "_computeEventAssignments");

    mHeader<<"\ntypedef void (*TPerformEventAssignmentDelegate)();"<<endl;
    mHeader.FormatVariable("D_S TPerformEventAssignmentDelegate*",  "_performEventAssignments");
    mHeader.AddFunctionExport("TPerformEventAssignmentDelegate*",   "Get_performEventAssignments()");

    mHeader.FormatArray("D_S bool",                                 "mEventStatusArray",             mNumEvents);
    mHeader.FormatArray("D_S bool",                                 "_previousEventStatusArray",     mNumEvents);
}


void CGenerator::WriteComputeAllRatesOfChange(CodeBuilder& ignore, const int& numIndependentSpecies, const int& numDependentSpecies, DoubleMatrix& L0)
{
     //In header
	mHeader.AddFunctionExport("void", "computeAllRatesOfChange()");
    mSource<<Append("//Uses the equation: dSd/dt = L0 dSi/dt" + NL());
    mSource<<"void computeAllRatesOfChange()\n{";
//    mSource<<"\n#if defined(DEBUG_C_DLL)\n";
//    mSource.TLine("printf(\"In function computeAllRatesOfChange()\\n\"); ");
//    mSource<<"#endif\n";

    mSource<<tab<<"int i;\n";
//    mSource<<"\n\t//double* dTemp = (double*) malloc( sizeof(double)* (amounts.Length + rateRules.Length) );\n";
    mSource<<"\n\tdouble* dTemp = (double*) malloc( sizeof(double)* (_amountsSize + _rateRulesSize) );\n"; //Todo: Check this

    for (int i = 0; i < NumAdditionalRates(); i++)
    {
        mSource<<Format("\tdTemp[{0}] = {1};{2}", i, mMapRateRule[i], NL());
    }

    mSource<<tab<<"for(i = 0; i < _amountsSize; i++)\n";
    mSource<<tab<<"{\n"<<tab<<tab<<"dTemp[i + _rateRulesSize] = _amounts[i];\n\t}";
    mSource<<Append("\n\t//amounts.CopyTo(dTemp, rateRules.Length); " + NL());

    mSource<<Append("\tevalModel(mTime, dTemp);" + NL());
    bool isThereAnEntry = false;
    for (int i = 0; i < numDependentSpecies; i++)
    {
        mSource<<Format("\t_dydt[{0}] = ", (numIndependentSpecies + i));
        isThereAnEntry = false;
        for (int j = 0; j < numIndependentSpecies; j++)
        {
            string dyName = Format("_dydt[{0}]", j);

            if (L0(i,j) > 0)
            {
                isThereAnEntry = true;
                if (L0(i,j) == 1)
                {
                    mSource<<Format(" + {0};{1}", dyName, NL());
                }
                else
                {
                    mSource<<Format(" + (double){0}{1}{2};{3}", WriteDouble(L0(i,j)), STR_FixAmountCompartments, dyName, NL());
                }
            }
            else if (L0(i,j) < 0)
            {
                isThereAnEntry = true;
                if (L0(i,j) == -1)
                {
                    mSource<<Format(" - {0};{1}", dyName, NL());
                }
                else
                {
                    mSource<<Format(" - (double){0}{1}{2};{3}", WriteDouble(fabs(L0(i,j))), STR_FixAmountCompartments, dyName, NL());
                }
            }
        }
        if (!isThereAnEntry)
        {
            mSource<<Append("0");
        }
        mSource<<"\n";
    }

    mSource<<Format("}{0}{0}", NL());
}

void CGenerator::WriteComputeConservedTotals(CodeBuilder& ignore, const int& numFloatingSpecies, const int& numDependentSpecies)
{
    mHeader.AddFunctionExport("void", "computeConservedTotals()");
    mSource<<"// Uses the equation: C = Sd - L0*Si"<<endl;
    mSource<<"void computeConservedTotals()\n{";

    if (numDependentSpecies > 0)
    {
        string factor;
        ls::DoubleMatrix *gamma = mLibStruct->getGammaMatrix();

//        double* matPtr =
//        DoubleMatrix gamma(matPtr, numDependentSpecies, numFloatingSpecies);
        for (int i = 0; i < numDependentSpecies; i++)
        {
            mSource<<Format("\n\t_ct[{0}] = ", i);
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
                        factor = WriteDouble(fabsl(current)) +
                                 STR_FixAmountCompartments;
                    }

                    if (current > 0)
                    {
                        string cYY = convertSpeciesToY(floatingSpeciesConcentrationList[j].name);
                        string cTC = convertCompartmentToC(floatingSpeciesConcentrationList[j].compartmentName);
                        mSource<<Append(" + " + factor + convertSpeciesToY(floatingSpeciesConcentrationList[j].name) +
                                  STR_FixAmountCompartments +
                                  convertCompartmentToC(floatingSpeciesConcentrationList[j].compartmentName));
                    }
                    else
                    {
                        mSource<<Append(" - " + factor + convertSpeciesToY(floatingSpeciesConcentrationList[j].name) +
                                  STR_FixAmountCompartments +
                                  convertCompartmentToC(floatingSpeciesConcentrationList[j].compartmentName));
                    }
                }
            }
            mSource<<Append(";" + NL());
            conservationList.Add(Symbol("CSUM" + ToString(i))); //TODO: how to deal with this?
        }
    }
    else
    {
        //mSource<<"printf(\"In an empty ComputeConservedTotals!\\n \");\n";
    }
    mSource<<"}\n\n";
}

void CGenerator::WriteUpdateDependentSpecies(CodeBuilder& ignore, const int& numIndependentSpecies, const int& numDependentSpecies, DoubleMatrix& L0)
{
    mHeader.AddFunctionExport("void", "updateDependentSpeciesValues(double* y)");
    mSource<<Append("// Compute values of dependent species " + NL());
    mSource<<Append("// Uses the equation: Sd = C + L0*Si" + NL());
    mSource<<"void updateDependentSpeciesValues(double* y)\n{";

    // Use the equation: Sd = C + L0*Si to compute dependent concentrations

    if (numDependentSpecies > 0)
    {
        for (int i = 0; i < numDependentSpecies; i++)
        {
            mSource<<Format("\n\t_y[{0}] = ", (i + numIndependentSpecies));
            mSource<<Format("(_ct[{0}]", i);
            string cLeftName =
                convertCompartmentToC(
                    floatingSpeciesConcentrationList[i + numIndependentSpecies].compartmentName);

            for (int j = 0; j < numIndependentSpecies; j++)
            {
                string yName = Format("y[{0}]", j);
                string cName = convertCompartmentToC(floatingSpeciesConcentrationList[j].compartmentName);
                double* mat = L0.GetPointer();
                double matElementValue = L0(i,j);

                if (L0(i,j) > 0) // In C# code there is no checking for index out of bound..
                {
                    if (L0(i,j) == 1)
                    {
                        mSource<<Format(" + {0}\t{1}{2}{3}{0}\t",
                            "",
                            yName,
                            STR_FixAmountCompartments,
                            cName);
                    }
                    else
                    {
                        mSource<<Format("{0} + (double){1}{2}{3}{2}{4}",
                            "",
                            WriteDouble(L0(i,j)),
                            STR_FixAmountCompartments,
                            yName,
                            cName);
                    }
                }
                else if (L0(i,j) < 0)
                {
                    if (L0(i,j) == -1)
                    {
                        mSource<<Format("{0} - {1}{2}{3}",
                            "",
                            yName,
                            STR_FixAmountCompartments,
                            cName);
                    }
                    else
                    {
                        mSource<<Format("{0} - (double){1}{2}{3}{2}{4}",
                            "",
                            WriteDouble(fabsl(L0(i,j))),
                            STR_FixAmountCompartments,
                            yName,
                            cName);
                    }
                }
            }
            mSource<<Format(") / {0};{1}", cLeftName, NL());
        }
    }
    mSource<<Format("}{0}{0}", NL());
}

void CGenerator::WriteUserDefinedFunctions(CodeBuilder& ignore)
{
    for (int i = 0; i < mNOM.getNumFunctionDefinitions(); i++)
    {
        try
        {
            ArrayList  oList = mNOM.getNthFunctionDefinition(i);
			Log(lDebug2)<<"NOM Nth Function defintion:"<<oList;
			ArrayListItemObject* ptr = const_cast<ArrayListItemObject*>(&oList[0]);

            if(!(dynamic_cast<ArrayListItem<string>*>(ptr)))
            {
                throw("Bad..");
            }

			string sName = oList.GetString(0);
            mfunctionNames.Add(sName);
            StringList oArguments = oList.GetStringList(1);
            string sBody = oList.GetString(2);

            mSource<<Format("// User defined function:  {0}{1}", sName, NL());
            mSource<<Format("\t double {0} (", sName);

            for (int j = 0; j < oArguments.Count(); j++)
            {
                mSource<<Append("double " + (string)oArguments[j]);
                mfunctionParameters.Add((string)oArguments[j]);
                if (j < oArguments.Count() - 1)
                {
                    mSource<<Append(", ");
                }
            }
            string userFunc = convertUserFunctionExpression(sBody);

            if(userFunc.find("spf_piecewise") != string::npos)
            {
                ConvertFunctionCallToUseVarArgsSyntax("spf_piecewise", userFunc);
            }

            if(userFunc.find("spf_and") != string::npos)
            {
                ConvertFunctionCallToUseVarArgsSyntax("spf_and", userFunc);
            }

            if(userFunc.find("spf_or") != string::npos)
            {
                ConvertFunctionCallToUseVarArgsSyntax("spf_or", userFunc);
            }

            if(userFunc.find("spf_xor") != string::npos)
            {
                ConvertFunctionCallToUseVarArgsSyntax("spf_xor", userFunc);
            }

            mSource<<Append(")" + NL() + "\t{" + NL() + "\t\t return " +
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

void CGenerator::WriteResetEvents(CodeBuilder& ignore, const int& numEvents)
{
      mHeader.AddFunctionExport("void", "resetEvents()");
      mSource<<"void resetEvents()\n{";
      for (int i = 0; i < numEvents; i++)
      {
          mSource<<Format("\n\tmEventStatusArray[{0}] = false;{1}", i, NL());
          mSource<<Format("\t_previousEventStatusArray[{0}] = false;", i);
          if(i == numEvents -1)
          {
              mSource<<"\n";
          }
      }
      mSource<<Format("}{0}", NL());
}

void CGenerator::WriteSetConcentration(CodeBuilder& ignore)
{
    mHeader.AddFunctionExport("void", "setConcentration(int index, double value)");
    mSource<<"\nvoid setConcentration(int index, double value)\n{";
    mSource<<Format("\n\tdouble volume = 0.0;{0}", NL());
    mSource<<Format("\t_y[index] = value;{0}", NL());
    mSource<<Format("\tswitch (index)\n\t{{0}", NL());

    for (int i = 0; i < floatingSpeciesConcentrationList.size(); i++)
    {
        mSource<<Format("\t\tcase {0}:\n\t\t\tvolume = {1};{2}",
          i,
          convertCompartmentToC(floatingSpeciesConcentrationList[i].compartmentName),
          NL());
      mSource<<Format("\t\tbreak;{0}", NL());
    }

    mSource<<Format("\t}{0}", NL());

    mSource<<Format("\t_amounts[index] = _y[index]*volume;{0}", NL());
    mSource<<Format("}{0}{0}", NL());
}

void CGenerator::WriteGetConcentration(CodeBuilder& ignore)
{
    mHeader.AddFunctionExport("double", "getConcentration(int index)");
    mSource<<Format("double getConcentration(int index)\n{{0}", NL());
    mSource<<Format("\treturn _y[index];{0}", NL());
    mSource<<Format("}{0}{0}", NL());
}

void CGenerator::WriteConvertToAmounts(CodeBuilder& ignore)
{
    mHeader.AddFunctionExport("void", "convertToAmounts()");
    mSource<<Format("void convertToAmounts()\n{{0}", NL());
    for (int i = 0; i < floatingSpeciesConcentrationList.size(); i++)
    {
        mSource<<Format("\t_amounts[{0}] = _y[{0}]*{1};{2}",
            i,
            convertCompartmentToC(floatingSpeciesConcentrationList[i].compartmentName),
            NL());
    }
    mSource<<Format("}{0}{0}", NL());
}

void CGenerator::WriteConvertToConcentrations(CodeBuilder& ignore)
{
    mHeader.AddFunctionExport("void", "convertToConcentrations()");
    mSource<<"void convertToConcentrations()\n{";
    for (int i = 0; i < floatingSpeciesConcentrationList.size(); i++)
    {
        mSource<<"\n\t_y[" << i << "] = _amounts[" << i << "] / " <<
                  convertCompartmentToC(floatingSpeciesConcentrationList[i].compartmentName) << ";";
    }
    mSource<<Append("\n}" + NL() + NL());
}

void CGenerator::WriteProperties(CodeBuilder& ignore)
{
////            // ------------------------------------------------------------------------------
////            sb.Append("\tpublic double[] y {" + NL());
////            sb.Append("\t\tget { return _y; } " + NL());
////            sb.Append("\t\tset { _y = value; } " + NL());
////            sb.Append("\t}" + NL() + NL());
////
////            // ------------------------------------------------------------------------------
////            sb.Append("\tpublic double[] init_y {" + NL());
////            sb.Append("\t\tget { return _init_y; } " + NL());
////            sb.Append("\t\tset { _init_y = value; } " + NL());
////            sb.Append("\t}" + NL() + NL());
////
////            // ------------------------------------------------------------------------------
////            sb.Append("\tpublic double[] amounts {" + NL());
////            sb.Append("\t\tget { return _amounts; } " + NL());
////            sb.Append("\t\tset { _amounts = value; } " + NL());
////            sb.Append("\t}" + NL() + NL());
////
////            // ------------------------------------------------------------------------------
////            sb.Append("\tpublic double[] bc {" + NL());
////            sb.Append("\t\tget { return _bc; } " + NL());
////            sb.Append("\t\tset { _bc = value; } " + NL());
////            sb.Append("\t}" + NL() + NL());
////
////            // ------------------------------------------------------------------------------
////            sb.Append("\tpublic double[] gp {" + NL());
////            sb.Append("\t\tget { return _gp; } " + NL());
////            sb.Append("\t\tset { _gp = value; } " + NL());
////            sb.Append("\t}" + NL() + NL());
////
////            // ------------------------------------------------------------------------------
////            sb.Append("\tpublic double[] sr {" + NL());
////            sb.Append("\t\tget { return _sr; } " + NL());
////            sb.Append("\t\tset { _sr = value; } " + NL());
////            sb.Append("\t}" + NL() + NL());
////
////            // ------------------------------------------------------------------------------
////            sb.Append("\tpublic double[][] lp {" + NL());
////            sb.Append("\t\tget { return _lp; } " + NL());
////            sb.Append("\t\tset { _lp = value; } " + NL());
////            sb.Append("\t}" + NL() + NL());
////
////            // ------------------------------------------------------------------------------
////            sb.Append("\tpublic double[] c {" + NL());
////            sb.Append("\t\tget { return _c; } " + NL());
////            sb.Append("\t\tset { _c = value; } " + NL());
////            sb.Append("\t}" + NL() + NL());
////
////            // ------------------------------------------------------------------------------
////            sb.Append("\tpublic double[] dydt {" + NL());
////            sb.Append("\t\tget { return _dydt; }" + NL());
////            sb.Append("\t\tset { _dydt = value; }" + NL());
////            sb.Append("\t}" + NL() + NL());
////
////            // ------------------------------------------------------------------------------
////            sb.Append("\tpublic double[] rateRules {" + NL());
////            sb.Append("\t\tget { return _rateRules; }" + NL());
////            sb.Append("\t\tset { _rateRules = value; }" + NL());
////            sb.Append("\t}" + NL() + NL());
////
////            // ------------------------------------------------------------------------------
////            sb.Append("\tpublic double[] rates {" + NL());
////            sb.Append("\t\tget { return _rates; }" + NL());
////            sb.Append("\t\tset { _rates = value; }" + NL());
////            sb.Append("\t}" + NL() + NL());
////
////            // ------------------------------------------------------------------------------
////            sb.Append("\tpublic double[] ct {" + NL());
////            sb.Append("\t\tget { return _ct; }" + NL());
////            sb.Append("\t\tset { _ct = value; }" + NL());
////            sb.Append("\t}" + NL() + NL());
////
////            // ------------------------------------------------------------------------------
////            sb.Append("\tpublic double[] eventTests {" + NL());
////            sb.Append("\t\tget { return _eventTests; }" + NL());
////            sb.Append("\t\tset { _eventTests = value; }" + NL());
////            sb.Append("\t}" + NL() + NL());
////
////            // ------------------------------------------------------------------------------
////            sb.Append("\tpublic TEventDelayDelegate[] eventDelay {" + NL());
////            sb.Append("\t\tget { return _eventDelay; }" + NL());
////            sb.Append("\t\tset { _eventDelay = value; }" + NL());
////            sb.Append("\t}" + NL() + NL());
////            // ------------------------------------------------------------------------------
////            sb.Append("\tpublic bool[] eventType {" + NL());
////            sb.Append("\t\tget { return _eventType; }" + NL());
////            sb.Append("\t\tset { _eventType = value; }" + NL());
////            sb.Append("\t}" + NL() + NL());
////            // ------------------------------------------------------------------------------
////            sb.Append("\tpublic bool[] eventPersistentType {" + NL());
////            sb.Append("\t\tget { return _eventPersistentType; }" + NL());
////            sb.Append("\t\tset { _eventPersistentType = value; }" + NL());
////            sb.Append("\t}" + NL() + NL());
////            // ------------------------------------------------------------------------------
////            sb.Append("\tpublic bool[] eventStatusArray {" + NL());
////            sb.Append("\t\tget { return _eventStatusArray; }" + NL());
////            sb.Append("\t\tset { _eventStatusArray = value; }" + NL());
////            sb.Append("\t}" + NL() + NL());
////
////            // ------------------------------------------------------------------------------
////            sb.Append("\tpublic bool[] previousEventStatusArray {" + NL());
////            sb.Append("\t\tget { return _previousEventStatusArray; }" + NL());
////            sb.Append("\t\tset { _previousEventStatusArray = value; }" + NL());
////            sb.Append("\t}" + NL() + NL());
////
////            // ------------------------------------------------------------------------------
////            sb.Append("\tpublic double[] eventPriorities {" + NL());
////            sb.Append("\t\tget { return _eventPriorities; }" + NL());
////            sb.Append("\t\tset { _eventPriorities = value; }" + NL());
////            sb.Append("\t}" + NL() + NL());
////
////            // ------------------------------------------------------------------------------
////            sb.Append("\tpublic TEventAssignmentDelegate[] eventAssignments {" + NL());
////            sb.Append("\t\tget { return _eventAssignments; }" + NL());
////            sb.Append("\t\tset { _eventAssignments = value; }" + NL());
////            sb.Append("\t}" + NL() + NL());
////            // ------------------------------------------------------------------------------
////            sb.Append("\tpublic TComputeEventAssignmentDelegate[] computeEventAssignments {" + NL());
////            sb.Append("\t\tget { return _computeEventAssignments; }" + NL());
////            sb.Append("\t\tset { _computeEventAssignments = value; }" + NL());
////            sb.Append("\t}" + NL() + NL());
////            // ------------------------------------------------------------------------------
////            sb.Append("\tpublic TPerformEventAssignmentDelegate[] performEventAssignments {" + NL());
////            sb.Append("\t\tget { return _performEventAssignments; }" + NL());
////            sb.Append("\t\tset { _performEventAssignments = value; }" + NL());
////            sb.Append("\t}" + NL() + NL());
////            // ------------------------------------------------------------------------------
////            sb.Append("\tpublic double time {" + NL());
////            sb.Append("\t\tget { return _time; }" + NL());
////            sb.Append("\t\tset { _time = value; }" + NL());
////            sb.Append("\t}" + NL() + NL());

}

void CGenerator::WriteAccessors(CodeBuilder& ignore)
{
//    sb<<Append("\tpublic int getNumIndependentVariables {" + NL());
//    sb<<Append("\t\tget { return numIndependentVariables; }" + NL());
//    sb<<Append("\t}" + NL() + NL());
//
//    sb<<Append("\tpublic int getNumDependentVariables {" + NL());
//    sb<<Append("\t\tget { return numDependentVariables; }" + NL());
//    sb<<Append("\t}" + NL() + NL());
//
//    sb<<Append("\tpublic int getNumTotalVariables {" + NL());
//    sb<<Append("\t\tget { return numTotalVariables; }" + NL());
//    sb<<Append("\t}" + NL() + NL());
//
//    sb<<Append("\tpublic int getNumBoundarySpecies {" + NL());
//    sb<<Append("\t\tget { return numBoundaryVariables; }" + NL());
//    sb<<Append("\t}" + NL() + NL());
//
//    sb<<Append("\tpublic int getNumGlobalParameters {" + NL());
//    sb<<Append("\t\tget { return numGlobalParameters; }" + NL());
//    sb<<Append("\t}" + NL() + NL());
//
    mHeader.AddFunctionExport("int", "getNumLocalParameters(int reactionId)");
    mSource<<"int getNumLocalParameters(int reactionId)\n{\n\t";
    mSource<<"return localParameterDimensions[reactionId];\n}\n\n";
//    sb<<Append("\tpublic int getNumLocalParameters(int reactionId)" + NL());
//    sb<<Append("\t{" + NL());
//    sb<<Append("\t\treturn localParameterDimensions[reactionId];" + NL());
//    sb<<Append("\t}" + NL() + NL());
//
//    sb<<Append("\tpublic int getNumCompartments {" + NL());
//    sb<<Append("\t\tget { return numCompartments; }" + NL());
//    sb<<Append("\t}" + NL() + NL());
//
//    sb<<Append("\tpublic int getNumReactions {" + NL());
//    sb<<Append("\t\tget { return numReactions; }" + NL());
//    sb<<Append("\t}" + NL() + NL());
//
//    sb<<Append("\tpublic int getNumEvents {" + NL());
//    sb<<Append("\t\tget { return numEvents; }" + NL());
//    sb<<Append("\t}" + NL() + NL());
//
//    sb<<Append("\tpublic int getNumRules {" + NL());
//    sb<<Append("\t\tget { return numRules; }" + NL());
//    sb<<Append("\t}" + NL() + NL());
//
//    sb<<Append("\tpublic List<string> Warnings {" + NL());
//    sb<<Append("\t\tget { return _Warnings; }" + NL());
//    sb<<Append("\t\tset { _Warnings = value; }" + NL());
//    sb<<Append("\t}" + NL() + NL());
}

string CGenerator::FindSymbol(const string& varName)
{
      int index = 0;
      if (floatingSpeciesConcentrationList.find(varName, index))
      {
          return Format("_y[{0}]", index);
      }
      else if (globalParameterList.find(varName, index))
      {
          return Format("_gp[{0}]", index);
      }
      else if (boundarySpeciesList.find(varName, index))
      {
          return Format("_bc[{0}]", index);
      }
      else if (compartmentList.find(varName, index))
      {
          return Format("_c[{0}]", index);
      }
      else if (ModifiableSpeciesReferenceList.find(varName, index))
          return Format("_sr[{0}]", index);

      else
          throw Exception(Format("Unable to locate lefthand side symbol in assignment[{0}]", varName));
}

void CGenerator::WriteTestConstraints(CodeBuilder& ignore)
{
    mHeader.AddFunctionExport("void", "testConstraints()");
    mSource<<Append("void testConstraints()" + NL());
    mSource<<Append("{");

    for (int i = 0; i < mNOM.getNumConstraints(); i++)
    {
        string sMessage;
        string sCheck = mNOM.getNthConstraint(i, sMessage);

        mSource<<Append("\tif (" + substituteTerms(mNOM.getNumReactions(), "", sCheck) + " == 0.0 )" + NL());
        mSource<<Append("\t\tthrow new Exception(\"" + sMessage + "\");" + NL());
    }

    mSource<<Append("}" + NL() + NL());
}

void CGenerator::WriteEvalInitialAssignments(CodeBuilder& ignore, const int& numReactions)
{
    mHeader.AddFunctionExport("void", "evalInitialAssignments()");
    mSource<<Append("void evalInitialAssignments()" + NL());
    mSource<<Append("{\n");

    int numInitialAssignments = mNOM.getNumInitialAssignments();

    if (numInitialAssignments > 0)
    {
        vector< pair<string, string> > oList;// = new List<Pair<string, string>>();
        for (int i = 0; i < numInitialAssignments; i++)
        {
            pair<string, string> pair = mNOM.getNthInitialAssignmentPair(i);
            oList.push_back(mNOM.getNthInitialAssignmentPair(i));
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
                    if (ExpressionContainsSymbol(current.second, oList[j].first))
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
            string leftSideRule = FindSymbol(pair.first);
            string rightSideRule = pair.second;
            if (leftSideRule.size())
            {
                mSource<<Append(leftSideRule + " = ");
                string temp = Append(substituteTerms(numReactions, "", rightSideRule) + ";" + NL());
                temp = ReplaceWord("time", "mTime", temp);
                mSource<<temp;
            }
        }
    }
    for (int i = 0; i < mNOM.GetModel()->getNumEvents(); i++)
    {
        libsbml::Event *current = mNOM.GetModel()->getEvent(i);
        string initialTriggerValue = ToString(current->getTrigger()->getInitialValue());//.ToString().ToLowerInvariant();
        mSource<<Append("\tmEventStatusArray[" + ToString(i) + "] = " + initialTriggerValue + ";" + NL());
        mSource<<Append("\t_previousEventStatusArray[" + ToString(i) + "] = " + initialTriggerValue + ";" + NL());
    }
    mSource<<Append("}" + NL() + NL());
}

int CGenerator::WriteComputeRules(CodeBuilder& ignore, const int& numReactions)
{
    IntStringHashTable mapVariables;
    mapVariables.clear();
    int numRateRules = 0;
    int numOfRules = mNOM.getNumRules();

    mHeader.AddFunctionExport("void", "computeRules(double* y)");
    mSource<<"void computeRules(double* y)\n{\n";

    for (int i = 0; i < numOfRules; i++)
    {
        try
        {
            string leftSideRule = "";
            string rightSideRule = "";
            string ruleType = mNOM.getNthRuleType(i);

            // We only support assignment and ode rules at the moment
            string eqnRule = mNOM.getNthRule(i);
            RRRule aRule(eqnRule, ruleType);
            string varName       = Trim(aRule.GetLHS());
            string rightSide     = Trim(aRule.GetRHS());

            bool isRateRule = false;

            switch (aRule.GetType())
            {
                case rtAlgebraic:
                    Warnings.Add("RoadRunner does not yet support algebraic rules in SBML, they will be ignored.");
                    leftSideRule = "";//NULL;
                break;

                case rtAssignment:
                    leftSideRule = FindSymbol(varName);
                break;

                case rtRate:
                    isRateRule = true;
                    int index;
                    if (floatingSpeciesConcentrationList.find(varName,  index))
                    {
                        leftSideRule = Format("\n\t_dydt[{0}]", index);
                        floatingSpeciesConcentrationList[index].rateRule = true;
                    }
                    else
                    {
                        leftSideRule = "\n\t_rateRules[" + ToString(numRateRules) + "]";
                        mMapRateRule[numRateRules] = FindSymbol(varName);
                        mapVariables[numRateRules] = varName;
                        numRateRules++;
                    }
                break;
            }

            // Run the equation through MathML to carry out any conversions (eg ^ to Pow)
            if(rightSide.size())
            {
                string rightSideMathml    = mNOM.convertStringToMathML(rightSide);
                rightSideRule             = mNOM.convertMathMLToString(rightSideMathml);
            }

            if (leftSideRule.size())
            {
                mSource<<tab<<Append(leftSideRule + " = ");
                int speciesIndex;
                bool isSpecies = floatingSpeciesConcentrationList.find(varName, speciesIndex);

                Symbol* symbol = (speciesIndex != -1) ? &(floatingSpeciesConcentrationList[speciesIndex]) : NULL;
                string sCompartment;

                if(isRateRule && mNOM.MultiplyCompartment(varName, sCompartment) && (rightSide.find(sCompartment) == string::npos))
                {
                    string temp = Format("({0}) * {1};{2}", substituteTerms(numReactions, "", rightSideRule), FindSymbol(sCompartment), NL());
                    temp = ReplaceWord("time", "mTime", temp);
                    mSource<<temp;
                }
                else
                {
                    if (isSpecies && !isRateRule && symbol != NULL && symbol->hasOnlySubstance && symbol->compartmentName.size() != 0)
                    {
                        mSource<<Format("({0}) / {1};{2}", substituteTerms(numReactions, "", rightSideRule), FindSymbol(symbol->compartmentName), NL());
                    }
                    else
                    {
                        string temp   = Format("{0};{1}", substituteTerms(numReactions, "", rightSideRule), NL());
                        temp = ReplaceWord("time", "mTime", temp);

                        if(temp.find("spf_piecewise") != string::npos)
            			{
                			ConvertFunctionCallToUseVarArgsSyntax("spf_piecewise", temp);
            			}
                        temp = RemoveNewLines(temp);
                        mSource<<temp;
                    }
                }

                if (mNOM.IsCompartment(varName))
                {
                    mSource<<Append("\n\tconvertToConcentrations();\n");
                }
            }
        }
        catch (const Exception& e)
        {
            throw new SBWApplicationException("Error while trying to get Rule #" + ToString(i) + e.Message());
        }
    }

    mSource<<Append("\n}" + NL() + NL());

    mHeader.FormatArray("D_S double", "_rateRules", numRateRules, "Vector containing values of additional rate rules"); //Todo: why is t his here in nowhere?
//    mHeader<<"D_S int _rateRulesSize="<<numRateRules<<";           // Number of rateRules   \n"; //Todo: why is this here in nowhere?
    mHeader.AddFunctionExport("void", "InitializeRates()");
    mSource<<"void InitializeRates()\n{";

    for (int i = 0; i < numRateRules; i++)
    {
        mSource<<"\n\t_rateRules[" << i << "] = " << mMapRateRule[i] << ";" << NL();
    }

    mSource<<Append("}" + NL() + NL());

    mHeader.AddFunctionExport("void", "AssignRatesA()");
    mSource<<Append("void AssignRatesA()\n{" + NL());

    for (int i = 0; i < mMapRateRule.size(); i++)
    {
        if(!i)
        {
            mSource<<"\n";
        }
        mSource<<"\t"<<(string) mMapRateRule[i] << " = _rateRules[" << i << "];\n";
    }

    mSource<<Append("}" + NL() + NL());

    mHeader.AddFunctionExport("void", "InitializeRateRuleSymbols()");
    mSource<<"void InitializeRateRuleSymbols() \n{";
    for (int i = 0; i < mMapRateRule.size(); i++)
    {
        if(!i)
        {
            mSource<<"\n";
        }

        string varName = (string) mapVariables[i];
        if(varName.size())
        {
        	double value = mNOM.getValue(varName);
	        if (!IsNaN(value))
    	    {
        	    mSource<<tab<<mMapRateRule[i] << " = " << ToString(value, mDoubleFormat) << ";" << NL();
        	}
        }
    }

    mSource<<"}\n\n";
    mHeader.AddFunctionExport("void", "AssignRatesB(double oRates[])");
    mSource<<"void AssignRatesB(double oRates[])\n{";

    for (int i = 0; i < mMapRateRule.size(); i++)
    {
        if(!i)
        {
            mSource<<"\n";
        }

        mSource<< mMapRateRule[i] << " = oRates[" << i << "];" << NL();
    }

    mSource<<Append("}" + NL() + NL());
    mHeader.AddFunctionExport("double*", "GetCurrentValues()");
    mSource<<"double* GetCurrentValues()\n{";
    mSource<<"\n\tdouble* dResult = (double*) malloc(sizeof(double)*"<<NumAdditionalRates()<<");\n";

    for (int i = 0; i < mMapRateRule.size(); i++)
    {
           if(!i)
        {
            mSource<<"\n";
        }

        mSource<<"\tdResult[" << i << "] = " << mMapRateRule[i] << ";" << NL();
    }
    mSource<<"\treturn dResult;\n";

    mSource<<Append("}" + NL() + NL());
    return numOfRules;
}

void CGenerator::WriteComputeReactionRates(CodeBuilder& ignore, const int& numReactions)
{
    mHeader.AddFunctionExport("void", "computeReactionRates(double time, double *y)");
    mSource<<Append("// Compute the reaction rates" + NL());
    mSource<<"void computeReactionRates(double time, double *y)\n{";    //Todo: what is time doing here?


    for (int i = 0; i < numReactions; i++)
    {
        string kineticLaw = mNOM.getKineticLaw(i);

        // The following code is for the case when the kineticLaw contains a ^ in place
        // of pow for exponent handling. It would not be needed in the case when there is
        // no ^ in the kineticLaw.
        string subKineticLaw;
//        if (kineticLaw.IndexOf("^", System.StringComparison.Ordinal) > 0) //Todo: fix this...
//        {
//            string kineticLaw_mathml = mNOM.convertStringToMathML(kineticLaw);
//            subKineticLaw = mNOM.convertMathMLToString(kineticLaw_mathml);
//        }
//        else
        {
            subKineticLaw = kineticLaw;
        }

        string modKineticLaw = substituteTerms(reactionList[i].name, subKineticLaw, true) + ";";

        // modify to use current y ...
        modKineticLaw = Substitute(modKineticLaw, "_y[", "y[");
        string expression = Format("\n\t_rates[{0}] = {1}{2}", i, modKineticLaw, NL());

        if(expression.find("spf_and") != string::npos)
        {
            ConvertFunctionCallToUseVarArgsSyntax("spf_and", expression);
        }

        if(expression.find("spf_or") != string::npos)
        {
            ConvertFunctionCallToUseVarArgsSyntax("spf_or", expression);
        }

        if(expression.find("spf_xor") != string::npos)
        {
            ConvertFunctionCallToUseVarArgsSyntax("spf_xor", expression);
        }

        if(expression.find("spf_squarewave") != string::npos)
        {
            ConvertFunctionCallToUseVarArgsSyntax("spf_squarewave", expression);
        }

		if(expression.find("spf_piecewise") != string::npos)
        {
            ConvertFunctionCallToUseVarArgsSyntax("spf_piecewise", expression);
        }

        expression = RemoveChars(expression, "\t \n");
        mSource<<"\n\t"<<expression<<"\n";
    }

    mSource<<Format("}{0}{0}", NL());
}

void CGenerator::WriteEvalEvents(CodeBuilder& ignore, const int& numEvents, const int& numFloatingSpecies)
{
    mSource<<Append("//Event handling function" + NL());
    mHeader.AddFunctionExport("void", "evalEvents(double timeIn, double *oAmounts)");
    mSource<<Append("void evalEvents(double timeIn, double *oAmounts)" + NL());
    mSource<<Append("{" + NL());

    if (numEvents > 0)
    {
        for (int i = 0; i < NumAdditionalRates(); i++)
        {
            mSource<<tab<<(string) mMapRateRule[i] << " = oAmounts[" << i << "];" << NL();
        }
        for (int i = 0; i < numFloatingSpecies; i++)
        {
            mSource<<"\t_y[" << i << "] = oAmounts[" << (i + NumAdditionalRates()) << "]/" <<
                      convertCompartmentToC(floatingSpeciesConcentrationList[i].compartmentName) << ";" << NL();
        }
    }

    mSource<<Append("\tmTime = timeIn;" + NL());
    mSource<<Append("\tupdateDependentSpeciesValues(_y);" + NL());
    mSource<<Append("\tcomputeRules(_y);" + NL());

    for (int i = 0; i < numEvents; i++)
    {
        ArrayList ev = mNOM.getNthEvent(i);
        StringList tempList = ev.GetStringList(0);
        string eventString = tempList[0];

        eventString = substituteTerms(0, "", eventString);
        eventString = ReplaceWord("time", "mTime", eventString);
        mSource<<"\t_previousEventStatusArray[" << i << "] = mEventStatusArray[" << i << "];" << NL();


        ConvertFunctionCallToUseVarArgsSyntax("spf_and", eventString);
        eventString = RemoveNewLines(eventString);
        mSource<<Append("\tif (" + eventString + " == 1.0)\n\t{" + NL());
//        mSource<<Append("\t printf(\"Time for lt true: %f\n\", timeIn);\n");
        mSource<<Append("\t\tmEventStatusArray[" + ToString(i) + "] = true;" + NL());
        mSource<<Append("\t\tmEventTests[" + ToString(i) + "] = 1;" + NL());
        mSource<<Append("\n\t}\n\telse\n\t{\n");
//         mSource<<Append("\t printf(\"Time for lt false: %f\n\", timeIn);\n");
        mSource<<Append("\t\tmEventStatusArray[" + ToString(i) + "] = false;" + NL());
        mSource<<Append("\t\tmEventTests[" + ToString(i) + "] = -1;" + NL());
        mSource<<Append("\t}" + NL());
    }
    mSource<<Append("}" + NL() + NL());
}

void CGenerator::WriteEvalModel(CodeBuilder& ignore, const int& numReactions, const int& numIndependentSpecies, const int& numFloatingSpecies, const int& numOfRules)
{
    mHeader.AddFunctionExport("void", "evalModel(double, double*)");
    mSource<<Append("//Model Function" + NL());
    mSource<<"void evalModel (double timein, double* oAmounts)\n{";


    for (int i = 0; i < NumAdditionalRates(); i++)
    {
        mSource<<"\n"<<(string)mMapRateRule[i] << " = oAmounts[" << i << "];" << NL();
    }

    for (int i = 0; i < numFloatingSpecies; i++)
    {
        mSource<<"\n\t_y[" << i << "] = oAmounts[" << i + NumAdditionalRates() << "]/" <<
                  convertCompartmentToC(floatingSpeciesConcentrationList[i].compartmentName) << ";" << NL();
    }

    mSource<<Append(NL());
    mSource<<Append("\tconvertToAmounts();" + NL());
    mSource<<Append("\tmTime = timein;  // Don't remove" + NL());
    mSource<<Append("\tupdateDependentSpeciesValues (_y);" + NL());

    if (numOfRules > 0)
    {
        mSource<<Append("\tcomputeRules (_y);" + NL());
    }

    mSource<<Append("\tcomputeReactionRates (mTime, _y);" + NL());

    // Write out the ODE equations
    string stoich;
    for (int i = 0; i < numIndependentSpecies; i++)
    {
        CodeBuilder eqnBuilder;// = new CodeBuilder(" ");
        string floatingSpeciesName = independentSpeciesList[i];
        for (int j = 0; j < numReactions; j++)
        {
            Reaction *oReaction = mNOM.GetModel()->getReaction(j);
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
                                stoich = Format("((double){0}/(double){1})*", WriteDouble(productStoichiometry), denom);
                            }
                            else
                            {
                                stoich = WriteDouble(productStoichiometry) + '*';
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
                    eqnBuilder<<Format(" + {0}_rates[{1}]", stoich, j);
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
                        stoich = Format("({0}) * ", substituteTerms(numReactions, "", reactant->getId()));
                    }
                    else if (reactant->isSetStoichiometry())
                    {
                        if (reactantStoichiometry != 1)
                        {
                            int denom = reactant->getDenominator();
                            if (denom != 1)
                            {
                                stoich = Format("((double){0}/(double){1})*", WriteDouble(reactantStoichiometry), denom);
                            }
                            else
                            {
                                stoich = WriteDouble(reactantStoichiometry) + "*";
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

                    eqnBuilder<<Append(Format(" - {0}_rates[{1}]", stoich, j));
                }
            }
        }

        string final = eqnBuilder.ToString();//.Trim();

        if (IsNullOrEmpty(final))
        {
            final = "    0.0";
        }

        if (mNOM.GetSBMLDocument()->getLevel() > 2)
        {
            // remember to take the conversion factor into account
            string factor = "";
            Species* species = mNOM.GetModel()->getSpecies(floatingSpeciesName);
            if (species != NULL)
            {
                if (species->isSetConversionFactor())
                {
                    factor = species->getConversionFactor();
                }
                else if (mNOM.GetModel()->isSetConversionFactor())
                {
                    factor = mNOM.GetModel()->getConversionFactor();
                }
            }

            if (!IsNullOrEmpty(factor))
            {
                final = FindSymbol(factor) + " * (" + final + ")";
            }
        }

        // If the floating species has a raterule then prevent the dydt
        // in the model function from overriding it. I think this is expected behavior.
        if (!floatingSpeciesConcentrationList[i].rateRule)
        {
            mSource<<"\t_dydt[" << i << "] =" << final << ";" << NL();
        }
    }

    mSource<<Append("\tconvertToAmounts();" + NL());
    mSource<<Append("}" + NL() + NL());
}

void CGenerator::WriteEventAssignments(CodeBuilder& ignore, const int& numReactions, const int& numEvents)
{
    StringList delays;
    vector<bool> eventType;
    vector<bool> eventPersistentType;
    if (numEvents > 0)
    {
        //Get array of pointers functions
        mSource<<("TEventAssignmentDelegate* Get_eventAssignments() \n{\n\treturn _eventAssignments;\n}\n\n");
        mSource<<("TPerformEventAssignmentDelegate* Get_performEventAssignments() \n{\n\treturn _performEventAssignments;\n}\n\n");
        mSource<<("TComputeEventAssignmentDelegate* Get_computeEventAssignments() \n{\n\treturn _computeEventAssignments;\n}\n\n");
        mSource<<("TEventDelayDelegate* GetEventDelays() \n{\n\treturn mEventDelay;\n}\n\n");
        mSource<<Append("// Event assignments" + NL());
        for (int i = 0; i < numEvents; i++)
        {
            ArrayList ev = mNOM.getNthEvent(i);
            eventType.push_back(mNOM.getNthUseValuesFromTriggerTime(i));
            eventPersistentType.push_back(mNOM.GetModel()->getEvent(i)->getTrigger()->getPersistent());

            StringList event = ev.GetStringList(1);
            int numItems = event.Count();
            string str = substituteTerms(numReactions, "", event[0]);
            delays.Add(str);

            mSource<<Format("void eventAssignment_{0}() \n{{1}", i, NL());

            string funcName(Format("performEventAssignment_{0}(double* values)", i));
            mHeader.AddFunctionExport("void", funcName);
            mSource<<Format("\tperformEventAssignment_{0}( computeEventAssignment_{0}() );{1}", i, NL());
            mSource<<Append("}\n\n");

            funcName = (Format("computeEventAssignment_{0}()", i));
            mHeader.AddFunctionExport("double*", funcName);

            mSource<<Format("double* computeEventAssignment_{0}()\n{{1}", i, NL());
            StringList oTemp;
            StringList oValue;
            int nCount = 0;
            int numAssignments = ev.Count() - 2;
//            mSource<<"\n\tprintf(\"In computeEventAssignment_ \\n\");\n\n";
            mSource<<Format("\t\tdouble* values = (double*) malloc(sizeof(double)*{0});{1}", numAssignments, NL());
            for (int j = 2; j < ev.Count(); j++)
            {
                StringList asgn = ev.GetStringList(j);
                //string assignmentVar = substituteTerms(numReactions, "", (string)asgn[0]);
                string assignmentVar = FindSymbol((string)asgn[0]);
                string str;
                Symbol *species = GetSpecies(assignmentVar);


                if (species != NULL && species->hasOnlySubstance)
                {
                    str = Format("{0} = ({1}) / {2}", assignmentVar, substituteTerms(numReactions, "", (string)asgn[1]), FindSymbol(species->compartmentName));
                }
                else
                {
                    str = Format("{0} = {1}", assignmentVar, substituteTerms(numReactions, "", (string) asgn[1]));
                }

                string sTempVar = Format("values[{0}]", nCount);

                oTemp.Add(assignmentVar);
                oValue.Add(sTempVar);

                str = sTempVar+ str.substr(str.find(" ="));
                nCount++;
                string temp = Format("\t\t{0};{1}", str, NL());
                temp = ReplaceWord("time", "mTime", temp);
                mSource<<temp;
            }
            mSource<<Append("\treturn values;" + NL());
            mSource<<Append("}" + NL());
            mSource<<Format("void performEventAssignment_{0}(double* values) \n{{1}", i, NL());

            for (int j = 0; j < oTemp.Count(); j++)
            {
                mSource<<Format("\t\t{0} = values[{1}];{2}", oTemp[j], j, NL());
                string aStr = (string) oTemp[j];
                aStr = Trim(aStr);

                if (StartsWith(aStr, "_c[")) //Todo:May have to trim?
                {
                    mSource<<Append("\t\tconvertToConcentrations();" + NL());
                }
            }

            mSource<<Append("}" + NL());
        }
        mSource<<Append("\t" + NL());
    }

    //Have to create TEventDelegate functions here
    for (int i = 0; i < delays.Count(); i++)
    {
        mSource<<"double GetEventDelay_"<<i<<"()\n{\n\treturn "<<delays[i]<<";\n}\n\n";
    }

    mSource<<"void InitializeDelays()\n{\n";

    for (int i = 0; i < delays.Count(); i++)
    {
        mSource<<Format("\tmEventDelay[{0}] = (TEventDelayDelegate) malloc(sizeof(TEventDelayDelegate) * 1);{2}", i, delays[i], NL());

        //Inititialize
        mSource<<Format("\tmEventDelay[{0}] = GetEventDelay_{0};\n", i);
        mSource<<Format("\t_eventType[{0}] = {1};{2}", i, ToString((eventType[i] ? true : false)), NL());
        mSource<<Format("\t_eventPersistentType[{0}] = {1};{2}", i, (eventPersistentType[i] ? "true" : "false"), NL());
    }
    mSource<<"}\n\n";

    mHeader.AddFunctionExport("void", "computeEventPriorities()");
    mSource<<"void computeEventPriorities()\n{";
//    mSource<<"\n\t"<<"printf(\"In computeEventPriorities\");\n";
    for (int i = 0; i < numEvents; i++)
    {
        libsbml::Event* current = mNOM.GetModel()->getEvent(i);

        if (current->isSetPriority() && current->getPriority()->isSetMath())
        {
            string priority = SBML_formulaToString(current->getPriority()->getMath());
            mSource<<"\n"<<Format("\t_eventPriorities[{0}] = {1};{2}", i, substituteTerms(numReactions, "", priority), NL());

        }
        else
        {
            mSource<<"\n"<<Format("\t_eventPriorities[{0}] = 0;{1}", i, NL());
        }
    }
    mSource<<Format("}{0}{0}", NL());
}

void CGenerator::WriteSetParameterValues(CodeBuilder& ignore, const int& numReactions)
{
    mHeader.AddFunctionExport("void", "setParameterValues()");
    mSource<<"void setParameterValues()\n{";


    for (int i = 0; i < globalParameterList.size(); i++)
    {
        //If !+INF
        string para = Format("\n\t{0} = (double){1};{2}",
                      convertSymbolToGP(globalParameterList[i].name),
                      WriteDouble(globalParameterList[i].value),
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
        for (int j = 0; j < localParameterList[i].size(); j++)
            mSource<<Format("\n\t_lp[{0}][{1}] = (double){2};{3}",
                          i,
                          j,
                          WriteDouble(localParameterList[i][j].value),
                          NL());
    }

    mSource<<Append("}" + NL() + NL());
}

void CGenerator::WriteSetCompartmentVolumes(CodeBuilder& ignore)
{
    mHeader.AddFunctionExport("void", "setCompartmentVolumes()");
    mSource << "void setCompartmentVolumes()\n{";

    for (int i = 0; i < compartmentList.size(); i++)
    {
        mSource<<Append("\n\t" + convertSymbolToC(compartmentList[i].name) + " = (double)" +
                  WriteDouble(compartmentList[i].value) + ";" + NL());

        // at this point we also have to take care of all initial assignments for compartments as well as
        // the assignment rules on compartments ... otherwise we are in trouble :)
        stack<string> initializations = mNOM.GetMatchForSymbol(compartmentList[i].name);
        while (initializations.size() > 0)
        {
            string term(initializations.top());
            string sub = substituteTerms(mNumReactions, "", term);
            mSource<<Append("\t" + sub + ";" + NL());
            initializations.pop();
        }
    }

    mSource<<Append("}" + NL() + NL());
}

void CGenerator::WriteSetBoundaryConditions(CodeBuilder& ignore)
{
    mHeader.AddFunctionExport("void", "setBoundaryConditions()");
    mSource<<"void setBoundaryConditions()\n{\n";

    for (int i = 0; i < boundarySpeciesList.size(); i++)
    {
        if (IsNullOrEmpty(boundarySpeciesList[i].formula))
        {
            mSource<<Append("\t" + convertSpeciesToBc(boundarySpeciesList[i].name) + " = (double)" +
                      WriteDouble(boundarySpeciesList[i].value) + ";" + NL());
        }
        else
        {
            mSource<<Append("\t\t" + convertSpeciesToBc(boundarySpeciesList[i].name) + " = (double)" +
                      boundarySpeciesList[i].formula + ";" + NL());
        }
    }
    mSource<<Append("}" + NL() + NL());
}


void CGenerator::WriteSetInitialConditions(CodeBuilder& ignore, const int& numFloatingSpecies)
{
    mHeader.AddFunctionExport("void", "initializeInitialConditions()");
    mSource<<"void initializeInitialConditions()\n{";

    for (int i = 0; i < floatingSpeciesConcentrationList.size(); i++)
    {
        if (IsNullOrEmpty(floatingSpeciesConcentrationList[i].formula))
        {
            mSource<<Append("\n\t_init" + convertSpeciesToY(floatingSpeciesConcentrationList[i].name) + " = (double)" +
                      WriteDouble(floatingSpeciesConcentrationList[i].value) + ";");
        }
        else
        {
            string formula = floatingSpeciesConcentrationList[i].formula;
            mSource<<Append("\n\t_init" + convertSpeciesToY(floatingSpeciesConcentrationList[i].name) + " = (double) " +
                      formula + ";");
        }
    }

    mSource<<Append("\n}" + NL() + NL());

    // ------------------------------------------------------------------------------
    mHeader.AddFunctionExport("void", "setInitialConditions()");
    mSource<<"void setInitialConditions()";
    mSource<<"\n{";

    for (int i = 0; i < numFloatingSpecies; i++)
    {
        mSource<<"\n\t_y[" << i << "] =  _init_y[" << i << "];";
        mSource<<"\n\t_amounts[" << i << "] = _y[" << i << "]*" <<
                  convertCompartmentToC(floatingSpeciesConcentrationList[i].compartmentName) << ";" << NL();
    }
    mSource<<Append("}" + NL() + NL());
}

string CGenerator::convertSpeciesToY(const string& speciesName)
{
    int index;
    if (floatingSpeciesConcentrationList.find(speciesName, index))
    {
        return "_y[" + ToString(index) + "]";
    }
    throw new SBWApplicationException("Internal Error: Unable to locate species: " + speciesName);
}

string CGenerator::convertSpeciesToBc(const string& speciesName)
{
    int index;
    if (boundarySpeciesList.find(speciesName, index))
    {
        return "_bc[" + ToString(index) + "]";
    }
    throw SBWApplicationException("Internal Error: Unable to locate species: " + speciesName);
}

string CGenerator::convertCompartmentToC(const string& compartmentName)
{
    int index;
    if (compartmentList.find(compartmentName, index))
    {
        return "_c[" + ToString(index) + "]";
    }

    throw RRException("Internal Error: Unable to locate compartment: " + compartmentName);
}

string CGenerator::convertSymbolToGP(const string& parameterName)
{
    int index;
    if (globalParameterList.find(parameterName, index))
    {
        return "_gp[" + ToString(index) + "]";
    }
      throw SBWApplicationException("Internal Error: Unable to locate parameter: " + parameterName);
}

string CGenerator::convertSymbolToC(const string& compartmentName)
{
    int index;
    if (compartmentList.find(compartmentName, index))
    {
        return "_c[" + ToString(index) + "]";
    }
      throw SBWApplicationException("Internal Error: Unable to locate compartment: " + compartmentName);
}

string CGenerator::convertUserFunctionExpression(const string& equation)
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
                        mSource<<Append("spf_pow");
                    }
                    else if(theToken == "sqrt")
                    {
                        mSource<<Append("sqrt");
                      }
                    else if(theToken == "log")
                    {
                        mSource<<Append("spf_log");
                    }
                    else if(theToken == "log10")
                    {
                        mSource<<Append("Log10");
                    }
                    else if(theToken == "floor")
                    {
                        mSource<<Append("spf_floor");
                    }
                    else if(theToken == "ceil")
                    {
                        mSource<<Append("spf_ceil");
                    }
                    else if(theToken == "factorial")
                    {
                        mSource<<Append("spf_factorial");
                    }
                    else if(theToken == "exp")
                    {
                        mSource<<Append("Math.Exp");
                    }
                    else if(theToken == "sin")
                    {
                        mSource<<Append("sin");
                    }
                    else if(theToken == "cos")
                    {
                        mSource<<Append("cos");
                    }
                    else if(theToken == "tan")
                    {
                        mSource<<Append("tan");
                    }
                    else if(theToken == "abs")
                    {
                        mSource<<Append("spf_abs");
                    }
                    else if(theToken == "asin")
                    {
                        mSource<<Append("asin");
                    }
                    else if(theToken == "acos")
                    {
                        mSource<<Append("acos");
                    }
                    else if(theToken == "atan")
                    {
                        mSource<<Append("atan");
                    }
                    else if(theToken == "sec")
                    {
                        mSource<<Append("MathKGI.Sec");
                    }
                    else if(theToken == "csc")
                    {
                        mSource<<Append("MathKGI.Csc");
                    }
                    else if(theToken == "cot")
                    {
                        mSource<<Append("MathKGI.Cot");
                    }
                    else if(theToken == "arcsec")
                    {
                        mSource<<Append("MathKGI.Asec");
                    }
                    else if(theToken == "arccsc")
                    {
                        mSource<<Append("MathKGI.Acsc");
                    }
                    else if(theToken == "arccot")
                    {
                        mSource<<Append("MathKGI.Acot");
                    }
                    else if(theToken == "sinh")
                    {
                        mSource<<Append("Math.Sinh");
                    }
                    else if(theToken == "cosh")
                    {
                        mSource<<Append("Math.Cosh");
                    }
                    else if(theToken == "tanh")
                    {
                        mSource<<Append("Math.Tanh");
                    }
                    else if(theToken == "arcsinh")
                    {
                        mSource<<Append("MathKGI.Asinh");
                    }
                    else if(theToken == "arccosh")
                    {
                        mSource<<Append("MathKGI.Acosh");
                    }
                    else if(theToken == "arctanh")
                    {
                        mSource<<Append("MathKGI.Atanh");
                    }
                    else if(theToken == "sech")
                    {
                        mSource<<Append("MathKGI.Sech");
                    }
                    else if(theToken == "csch")
                    {
                        mSource<<Append("MathKGI.Csch");
                    }
                    else if(theToken == "coth")
                    {
                        mSource<<Append("MathKGI.Coth");
                    }
                    else if(theToken == "arcsech")
                    {
                        mSource<<Append("MathKGI.Asech");
                    }
                    else if(theToken == "arccsch")
                    {
                        mSource<<Append("MathKGI.Acsch");
                    }
                    else if(theToken == "arccoth")
                    {
                               mSource<<Append("MathKGI.Acoth");
                    }
                    else if(theToken == "pi")
                    {
                        mSource<<Append("PI");
                    }
                    else if(theToken == "exponentiale")
                    {
                        mSource<<Append("Math.E");
                    }
                    else if(theToken == "avogadro")
                    {
                        mSource<<Append("6.02214179e23");
                    }
                    else if(theToken == "true")
                    {
                        mSource<<Append("1.0");
                    }
                    else if(theToken == "false")
                    {
                        mSource<<Append("0.0");
                    }
                    else if(theToken == "gt")
                    {
                        mSource<<Append("spf_gt");
                    }
                    else if(theToken == "lt")
                    {
                        mSource<<Append("spf_lt");
                    }
                    else if(theToken == "eq")
                    {
                        mSource<<Append("spf_eq");
                    }
                    else if(theToken == "neq")
                    {
                        mSource<<Append("spf_neq");
                    }
                    else if(theToken == "geq")
                    {
                        mSource<<Append("spf_geq");
                    }
                    else if(theToken == "leq")
                    {
                        mSource<<Append("spf_leq");
                    }
                    else if(theToken == "and")
                    {
                        mSource<<Append("supportFunction._and");
                    }
                    else if(theToken == "or")
                    {
                        mSource<<Append("supportFunction._or");
                    }
                    else if(theToken == "not")
                    {
                        mSource<<Append("supportFunction._not");
                    }
                    else if(theToken == "xor")
                    {
                        mSource<<Append("supportFunction._xor");
                    }
                    else if(theToken == "root")
                    {
                        mSource<<Append("spf_root");
                    }
					else if (theToken == "squarewave")
					{
						mSource<<Append("spf_squarewave");
					}
                    else if(theToken == "piecewise")
                    {
                        mSource<<Append("spf_piecewise");
                    }
                    else if (!mfunctionParameters.Contains(s.tokenString))
                    {
                        throw Exception("Token '" + s.tokenString + "' not recognized.");
                    }
                    else
                    {
                        mSource<<Append(s.tokenString);
                    }

                break; //Word token

                   case CodeTypes::tDoubleToken:
                       mSource<<Append(WriteDouble(s.tokenDouble));
                       break;
                   case CodeTypes::tIntToken:
                    mSource<<Append((int) s.tokenInteger);
                       break;
                   case CodeTypes::tPlusToken:
                   mSource<<Append("+");
                   break;
                   case CodeTypes::tMinusToken:
                   mSource<<Append("-");
                   break;
                   case CodeTypes::tDivToken:
                   mSource<<Append("/");
                   break;
                   case CodeTypes::tMultToken:
                   mSource<<Append(STR_FixAmountCompartments);
                   break;
                   case CodeTypes::tPowerToken:
                   mSource<<Append("^");
                   break;
                   case CodeTypes::tLParenToken:
                   mSource<<Append("(");
                   break;
                   case CodeTypes::tRParenToken:
                   mSource<<Append(")");
                   break;
                   case CodeTypes::tCommaToken:
                   mSource<<Append(",");
                   break;
                   case CodeTypes::tEqualsToken:
                   mSource<<Append(" = ");
                   break;
                   case CodeTypes::tTimeWord1:
                   mSource<<Append("time");
                   break;
                   case CodeTypes::tTimeWord2:
                   mSource<<Append("time");
                   break;
                   case CodeTypes::tTimeWord3:
                   mSource<<Append("time");
                   break;
                   case CodeTypes::tAndToken:
                   mSource<<Append("spf_and");
                   break;
                   case CodeTypes::tOrToken:
                   mSource<<Append("spf_or");
                   break;
                   case CodeTypes::tNotToken:
                   mSource<<Append("spf_not");
                   break;
                   case CodeTypes::tLessThanToken:
                   mSource<<Append("spf_lt");
                   break;
                   case CodeTypes::tLessThanOrEqualToken:
                   mSource<<Append("spf_leq");
                   break;
                   case CodeTypes::tMoreThanOrEqualToken:
                   mSource<<Append("spf_geq");
                   break;
                   case CodeTypes::tMoreThanToken:
                   mSource<<Append("spf_gt");
                   break;
                   case CodeTypes::tXorToken:
                   mSource<<Append("spf_xor");
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
       throw new SBWApplicationException(e.Message());
    }
    return mSource.ToString();
}

void CGenerator::SubstituteEquation(const string& reactionName, Scanner& s, CodeBuilder& mSource)
{
    string theToken(s.tokenString);
    if(theToken == "pow")
    {
        mSource<<Append("spf_pow");
    }
    else if(theToken == "sqrt")
    {
        mSource<<Append("sqrt");
    }
    else if(theToken == "log")
    {
        mSource<<Append("spf_log");
    }
    else if(theToken == "floor")
    {
        mSource<<Append("spf_floor");
    }
    else if(theToken == "ceil")
    {
        mSource<<Append("spf_ceil");
    }
    else if(theToken == "factorial")
    {
        mSource<<Append("spf_factorial");
    }
    else if(theToken == "log10")
    {
        mSource<<Append("spf_log10");
    }
    else if(theToken == "exp")
    {
        mSource<<Append("spf_exp");
    }
    else if(theToken == "abs")
    {
        mSource<<Append("spf_abs");
    }
    else if(theToken == "sin")
    {
        mSource<<Append("spf_sin");
    }
    else if(theToken == "cos")
    {
        mSource<<Append("cos");
    }
    else if(theToken == "tan")
    {
        mSource<<Append("tan");
    }
    else if(theToken == "asin")
    {
        mSource<<Append("asin");
    }
    else if(theToken == "acos")
    {
        mSource<<Append("acos");
    }
    else if(theToken == "atan")
    {
        mSource<<Append("atan");
    }
    else if(theToken == "sec")
    {
        mSource<<Append("sec");
    }
    else if(theToken == "csc")
    {
        mSource<<Append("csc");
    }
    else if(theToken == "cot")
    {
        mSource<<Append("cot");
    }
    else if(theToken == "arcsec")
    {
        mSource<<Append("asec");
    }
    else if(theToken == "arccsc")
    {
        mSource<<Append("arccsc");
    }
    else if(theToken == "arccot")
    {
        mSource<<Append("arccot");
    }
    else if(theToken == "sinh")
    {
        mSource<<Append("sinh");
    }
    else if(theToken == "cosh")
    {
        mSource<<Append("cosh");
    }
    else if(theToken == "tanh")
    {
        mSource<<Append("tanh");
    }
    else if(theToken == "arcsinh")
    {
        mSource<<Append("arcsinh");
    }
    else if(theToken == "arccosh")
    {
        mSource<<Append("arccosh");
    }
    else if(theToken == "arctanh")
    {
        mSource<<Append("arctanh");
    }
    else if(theToken == "sech")
    {
        mSource<<Append("sech");
    }
    else if(theToken == "csch")
    {
        mSource<<Append("csch");
    }
    else if(theToken == "coth")
    {
        mSource<<Append("coth");
    }
    else if(theToken == "arcsech")
    {
        mSource<<Append("arcsech");
    }
    else if(theToken == "arccsch")
    {
        mSource<<Append("arccsch");
    }
    else if(theToken == "arccoth")
    {
        mSource<<Append("arccoth");
    }
    else if(theToken == "pi")
    {
        mSource<<Append("PI");
    }
    else if(theToken == "avogadro")
    {
        mSource<<Append("6.02214179e23");
    }
    else if(theToken == "exponentiale")
    {
        mSource<<Append("E");
    }
    else if(theToken == "true")
    {
        //mSource<<Append("true");
        mSource<<Append("1.0");
    }
    else if(theToken == "false")
    {
        //mSource<<Append("false");
        mSource<<Append("0.0");
    }
    else if(theToken == "NaN")
    {
        mSource<<Append("NaN");
    }
    else if(theToken == "INF")
    {
        mSource<<Append("INF");
    }
    else if(theToken == "geq")
    {
        mSource<<Append("spf_geq");
    }
    else if(theToken == "leq")
    {
        mSource<<Append("spf_leq");
    }
    else if(theToken == "gt")
    {
        mSource<<Append("spf_gt");
    }
    else if(theToken == "lt")
    {
        mSource<<Append("spf_lt");
    }
    else if(theToken == "eq")
    {
        mSource<<Append("spf_eq");
    }
    else if(theToken == "neq")
    {
        mSource<<Append("spf_neq");
    }
    else if(theToken == "and")
    {
        mSource<<Append("spf_and");
    }
    else if(theToken == "or")
    {
        mSource<<Append("spf_or");
    }
    else if(theToken == "not")
    {
        mSource<<Append("spf_not");
    }
    else if(theToken == "xor")
    {
        mSource<<Append("spf_xor");
    }
    else if(theToken == "root")
    {
        mSource<<Append("spf_root");
    }
	else if(theToken == "squarewave")
	{
		mSource<<Append("spf_squarewave");
	}
    else if(theToken == "piecewise")
    {
        mSource<<Append("spf_piecewise");
    }
    else if(theToken == "delay")
    {
        mSource<<Append("spf_delay");
        Warnings.Add("RoadRunner does not yet support delay differential equations in SBML, they will be ignored (i.e. treated as delay = 0).");
    }
    else
    {
        bool bReplaced = false;
        int index;
        if (reactionList.find(reactionName, index))
        {
            int nParamIndex = 0;
            if (localParameterList[index].find(s.tokenString, nParamIndex))
            {
                mSource<<Append("_lp[" + ToString(index) + "][" + ToString(nParamIndex) + "]");
                bReplaced = true;
            }
        }

        if (boundarySpeciesList.find(s.tokenString, index))
        {
            mSource<<Append("_bc[" + ToString(index) + "]");
            bReplaced = true;
        }
        if (!bReplaced &&
            (mfunctionParameters.Count() != 0 && !mfunctionParameters.Contains(s.tokenString)))
        {
            throw Exception("Token '" + s.tokenString + "' not recognized.");
        }
    }
}

void CGenerator::SubstituteWords(const string& reactionName, bool bFixAmounts, Scanner& s, CodeBuilder& mSource)
{
    // Global parameters have priority
    int index;
    if (globalParameterList.find(s.tokenString, index))
    {
        mSource<<Format("_gp[{0}]", index);
    }
    else if (boundarySpeciesList.find(s.tokenString, index))
    {
        mSource<<Format("_bc[{0}]", index);

        Symbol symbol = boundarySpeciesList[index];
        if (symbol.hasOnlySubstance)
        {
            // we only store concentration for the boundary so we better
            // fix that.
            int nCompIndex = 0;
            if (compartmentList.find(symbol.compartmentName, nCompIndex))
            {
                mSource<<Format("{0}_c[{1}]", STR_FixAmountCompartments, nCompIndex);
            }
        }
    }
    else if (floatingSpeciesConcentrationList.find(s.tokenString, index))
    {
        Symbol floating1 = floatingSpeciesConcentrationList[index];
        if (floating1.hasOnlySubstance)
        {
            mSource<<Format("_amounts[{0}]", index);
        }
        else
        {
            mSource<<Format("_y[{0}]", index);
        }
    }
    else if (compartmentList.find(s.tokenString, index))
    {
        mSource<<Format("_c[{0}]", index);
    }
    else if (mfunctionNames.Contains(s.tokenString))
    {
        mSource<<Format("{0} ", s.tokenString);
    }
    else if (ModifiableSpeciesReferenceList.find(s.tokenString, index))
    {
        mSource<<Format("_sr[{0}]", index);
    }
    else if (reactionList.find(s.tokenString, index))
    {
        mSource<<Format("_rates[{0}]", index);
    }
    else
    {
        SubstituteEquation(reactionName, s, mSource);
    }
}

void CGenerator::SubstituteToken(const string& reactionName, bool bFixAmounts, Scanner& s, CodeBuilder& mSource)
{
    string aToken = s.tokenString;
    CodeTypes codeType = s.token();
    switch(codeType)
    {
        case CodeTypes::tWordToken:
        case CodeTypes::tExternalToken:
        case CodeTypes::tExtToken:
            SubstituteWords(reactionName, bFixAmounts, s, mSource);
            break;

        case CodeTypes::tDoubleToken:
            mSource<<Append("(double) " + WriteDouble(s.tokenDouble));
            break;
        case CodeTypes::tIntToken:
            mSource<<Append("(double)" + WriteDouble((double)s.tokenInteger));
            break;
        case CodeTypes::tPlusToken:
            mSource<<Format("+{0}\t", NL());
            break;
        case CodeTypes::tMinusToken:
            mSource<<Format("-{0}\t", NL());
            break;
        case CodeTypes::tDivToken:
            mSource<<Format("/{0}\t", NL());
            break;
        case CodeTypes::tMultToken:
            mSource<<Format("*{0}\t", NL());
            break;
        case CodeTypes::tPowerToken:
            mSource<<Format("^{0}\t", NL());
            break;
        case CodeTypes::tLParenToken:
            mSource<<Append("(");
            break;
        case CodeTypes::tRParenToken:
            mSource<<Format("){0}\t", NL());
            break;
        case CodeTypes::tCommaToken:
            mSource<<Append(",");
            break;
        case CodeTypes::tEqualsToken:
            mSource<<Format(" = {0}\t", NL());
            break;
      case CodeTypes::tTimeWord1:
            mSource<<Append("mTime");
            break;
        case CodeTypes::tTimeWord2:
            mSource<<Append("mTime");
            break;
        case CodeTypes::tTimeWord3:
            mSource<<Append("mTime");
            break;
        case CodeTypes::tAndToken:
            mSource<<Format("{0}spf_and", NL());
            break;
        case CodeTypes::tOrToken:
            mSource<<Format("{0}spf_or", NL());
            break;
        case CodeTypes::tNotToken:
            mSource<<Format("{0}spf_not", NL());
            break;
        case CodeTypes::tLessThanToken:
            mSource<<Format("{0}spf_lt", NL());
            break;
        case CodeTypes::tLessThanOrEqualToken:
            mSource<<Format("{0}spf_leq", NL());
            break;
        case CodeTypes::tMoreThanOrEqualToken:
            mSource<<Format("{0}spf_geq", NL());
            break;
        case CodeTypes::tMoreThanToken:
            mSource<<Format("{0}spf_gt", NL());
            break;
        case CodeTypes::tXorToken:
            mSource<<Format("{0}spf_xor", NL());
            break;
        default:
        string aToken = s.tokenToString(s.token());
        Exception ae = Exception(
                 Format("Unknown token in substituteTerms: {0}", aToken,
                 "Exception raised in Module:roadRunner, Method:substituteTerms"));
         throw ae;
    }
}

void CGenerator::WriteOutSymbolTables(CodeBuilder& ignore)
{
    mSource<<Append("void loadSymbolTables()\n{");

    int nrFuncs = 0;
    for (int i = 0; i < floatingSpeciesConcentrationList.size(); i++)
    {
        mSource<<Format("\n\tvariableTable[{0}] = \"{1}\";", i, floatingSpeciesConcentrationList[i].name);
        nrFuncs++;
    }

    for (int i = 0; i < boundarySpeciesList.size(); i++)
    {
        mSource<<Format("\n\tboundaryTable[{0}] = \"{1}\";", i, boundarySpeciesList[i].name);
        nrFuncs++;
    }

    for (int i = 0; i < globalParameterList.size(); i++)
    {
        string name = globalParameterList[i].name;
           mSource<<Format("\n\tglobalParameterTable[{0}] = \"{1}\";", i, globalParameterList[i].name);
        nrFuncs++;
    }
    if(nrFuncs > 0)
    {
        mSource<<"\n";
    }
    mSource<<Format("}{0}{0}", NL());
}

int CGenerator::ReadFloatingSpecies()
{
    // Load a reordered list into the variable list.
    StringList reOrderedList;
    //if (mRR && mRR->RoadRunner::mComputeAndAssignConservationLaws)
	if(mComputeAndAssignConsevationLaws)
    {
       reOrderedList = mLibStruct->getReorderedSpecies();
    }
    else
    {
        reOrderedList = mLibStruct->getSpecies();
    }

    ArrayList oFloatingSpecies = mNOM.getListOfFloatingSpecies();

    Log(lDebug)<<"Got List: "<< oFloatingSpecies;
    for (int i = 0; i < reOrderedList.Count(); i++)
    {
        for (int j = 0; j < oFloatingSpecies.Count(); j++)
        {
            StringList oTempList = oFloatingSpecies.GetStringList(j);
              if(reOrderedList[i] != (const string&) oTempList[0])
              {
                  continue;
              }

            string compartmentName = mNOM.getNthFloatingSpeciesCompartmentName(j);
            bool bIsConcentration  = ToBool(oTempList[2]);
            double dValue = ToDouble(oTempList[1]);
            if (IsNaN(dValue))
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
              compartmentList.find(compartmentName, nCompartmentIndex);

              double dVolume = compartmentList[nCompartmentIndex].value;
              if (IsNaN(dVolume))
              {
                dVolume = 1;
              }

              stringstream formula;
              formula<<ToString(dValue,mDoubleFormat)<<"/ _c["<<nCompartmentIndex<<"]";

              symbol = new Symbol(reOrderedList[i],
                  dValue / dVolume,
                  compartmentName,
                  formula.str());
            }

            if(mNOM.GetModel())
            {
                Species *aSpecies = mNOM.GetModel()->getSpecies(reOrderedList[i]);
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
            Log(lDebug5)<<"Adding symbol to floatingSpeciesConcentrationList:"<<(*symbol);
            floatingSpeciesConcentrationList.Add(*(symbol));
            delete symbol;
            break;
          }
          //throw RRException("Reordered Species " + reOrderedList[i] + " not found.");
      }
      return oFloatingSpecies.Count();
}

int CGenerator::ReadBoundarySpecies()
{
    int numBoundarySpecies;
    ArrayList oBoundarySpecies = mNOM.getListOfBoundarySpecies();
    numBoundarySpecies = oBoundarySpecies.Count(); // sp1.size();
    for (int i = 0; i < numBoundarySpecies; i++)
    {
        StringList oTempList     = oBoundarySpecies.GetStringList(i);
        string sName             = oTempList[0];
        string compartmentName     = mNOM.getNthBoundarySpeciesCompartmentName(i);
        bool bIsConcentration     = ToBool(oTempList[2]);
        double dValue             = ToDouble(oTempList[1]);
        if (IsNaN(dValue))
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
            if(compartmentList.find(compartmentName, nCompartmentIndex))
            {
                dVolume = compartmentList[nCompartmentIndex].value;
            }
            else
            {
                if (IsNaN(dVolume))
                {
                    dVolume = 1;
                }
            }
            stringstream formula;
            formula<<ToString(dValue, mDoubleFormat)<<"/ _c["<<nCompartmentIndex<<"]";
            symbol = new Symbol(sName,
                                dValue / dVolume,
                                compartmentName,
                                formula.str());
        }

        if(mNOM.GetModel())
        {
            Species* species = mNOM.GetModel()->getSpecies(sName);
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
        boundarySpeciesList.Add(*symbol);
    }
    return numBoundarySpecies;
}

void CGenerator::WriteInitFunction(CodeBuilder& ignore, CodeBuilder& source)
{
    source.Line("\n//Function to initialize the model data structure. Returns an integer indicating result");
    source.Line("int InitModel()");
    source.Line("{");

    //The following is from the constructor..
    source<<"\t"<<Append("numIndependentVariables = " ,     mNumIndependentSpecies, ";" , NL());
    source<<"\t"<<Append("numDependentVariables = " ,       mNumDependentSpecies , ";" , NL());
    source<<"\t"<<Append("numTotalVariables = " ,           mNumFloatingSpecies , ";" , NL());
    source<<"\t"<<Append("numBoundaryVariables = " ,        mNumBoundarySpecies , ";" , NL());
    source<<"\t"<<Append("numGlobalParameters = " ,         globalParameterList.size() , ";" , NL());
    source<<"\t"<<Append("numCompartments = " ,             compartmentList.size() , ";" , NL());
    source<<"\t"<<Append("numReactions = " ,                reactionList.size() , ";" , NL());
    source<<"\t"<<Append("numEvents = " ,                   mNumEvents , ";" , NL());

    source<<tab<<    "mModelName = (char*) malloc(sizeof(char)*"<<strlen(mModelName.c_str()) + 1<<");" <<endl;
       source<<tab<<    "strcpy(mModelName,\""<<mModelName<<"\");"<<endl;

    source<<"\t"<<Append("setCompartmentVolumes();" , NL());
    source<<"\t"<<Append("InitializeDelays();" , NL());

    // Declare any eventAssignment delegates
    if (mNumEvents > 0)
    {
        source<<Append("\t_eventAssignments = (TEventAssignmentDelegate*) malloc(sizeof(TEventAssignmentDelegate)*numEvents);" , NL());
//        source<<Append("\t_eventPriorities = (double*) malloc(sizeof(double)* numEvents);" , NL());

//        source<<Append("\t\t_computeEventAssignments= new TComputeEventAssignmentDelegate[numEvents];" , NL());
        source<<Append("\t_computeEventAssignments = (TComputeEventAssignmentDelegate*) malloc(sizeof(TComputeEventAssignmentDelegate)*numEvents);" , NL());

//        source<<Append("\t\t_performEventAssignments= new TPerformEventAssignmentDelegate[numEvents];" , NL());
        source<<Append("\t_performEventAssignments = (TPerformEventAssignmentDelegate*) malloc(sizeof(TPerformEventAssignmentDelegate)*numEvents);" , NL());

        for (int i = 0; i < mNumEvents; i++)
        {
            string iStr = ToString(i);
//            source<<Append("\t_eventAssignments[" + iStr + "] = (TEventAssignmentDelegate) eventAssignment_" + iStr +";" + NL());
            source<<Append("\t_eventAssignments[" + iStr + "] = eventAssignment_" + iStr +";" + NL());
            source<<Append("\t_computeEventAssignments[" + iStr + "] = (TComputeEventAssignmentDelegate) computeEventAssignment_" + iStr + ";" + NL());
            source<<Append("\t_performEventAssignments[" + iStr + "] = (TPerformEventAssignmentDelegate) performEventAssignment_" + iStr + ";" + NL());
        }

        source<<Append("\tresetEvents();" + NL());
        ////Test to call a function
        source<<Append("\t_eventAssignments[0]();\n");
        source<<Append(NL());
    }

    if (mNumModifiableSpeciesReferences > 0)
    {
        for (int i = 0; i < ModifiableSpeciesReferenceList.size(); i++)
        {
            source<<Append("\t\t_sr[" + ToString(i) + "] = " + WriteDouble(ModifiableSpeciesReferenceList[i].value) + ";" + NL());
        }
        source<<Append(NL());
    }

    // Declare space for local parameters
    for (int i = 0; i < mNumReactions; i++)
    {
        source<<Append("\tlocalParameterDimensions[" + ToString(i) + "] = " , mLocalParameterDimensions[i] , ";" + NL());
        source<<"\t_lp["<<i<<"] = (double*) malloc(sizeof(double)*"<<mLocalParameterDimensions[i]<<");"<<endl;
    }

    source.TLine("return 0;");
    source.Line("}");
    source.NewLine();
    source.Line("char* GetModelName()");
    source<<"{"                                         <<endl;
    source.TLine("return mModelName;");
    source<<"}"                                         <<endl;
}

bool CGenerator::SaveSourceCodeToFolder(const string& folder)
{
    string fName = ExtractFileName(mCurrentXMLModelFileName);
    mHeaderCodeFileName = JoinPath(folder, fName);
    mHeaderCodeFileName = ChangeFileExtensionTo(mHeaderCodeFileName, ".h");

    ofstream outFile(mHeaderCodeFileName.c_str());
    if(!outFile)
    {
        throw(Exception("Failed to open file:" + mHeaderCodeFileName));
    }

    outFile<<GetHeaderCode();
    Log(lDebug3)<<"Wrote header to file: "<<mHeaderCodeFileName;
    outFile.close();

    mSourceCodeFileName = ChangeFileExtensionTo(mHeaderCodeFileName, ".c");
    outFile.open(mSourceCodeFileName.c_str());

    //We don't know the name of the file until here..
    //Write an include statement to it..
    vector<string> fNameParts = SplitString(mSourceCodeFileName,"\\");
    string headerFName = fNameParts[fNameParts.size() - 1];

    headerFName = ChangeFileExtensionTo(headerFName, ".h");
    outFile<<"#include \""<<ExtractFileName(headerFName)<<"\"\n"<<endl;
    outFile<<GetSourceCode();
    outFile.close();
    Log(lDebug3)<<"Wrote source code to file: "<<mSourceCodeFileName;

    return true;
}


}//Namespace

