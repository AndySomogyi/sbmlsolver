#ifdef USE_PCH
#include "rr_pch.h"
#endif
#pragma hdrstop
#include <iostream>
#include <complex>
#include "rrRoadRunner.h"
#include "rrException.h"
#include "rrModelGenerator.h"
#include "rrCompiler.h"
#include "rrStreamWriter.h"
#include "rrLogger.h"
#include "rrCSharpGenerator.h"
#include "rrCGenerator.h"
#include "rrStringUtils.h"
#include "rrModelFromC.h"
#include "rrSBMLModelSimulation.h"
#include "rr-libstruct/lsLA.h"
#include "rr-libstruct/lsLibla.h"
#include "rrModelState.h"
#include "rrArrayList.h"
#include "rrCapsSupport.h"
//---------------------------------------------------------------------------

using namespace std;
using namespace ls;

//We only need to give lthe linker the folder where libs are
//using the pragma comment. Works for MSVC and codegear
#if defined(__CODEGEARC__)
#pragma comment(lib, "sundials_cvode.lib")
#pragma comment(lib, "sundials_nvecserial.lib")
#pragma comment(lib, "nleq-static.lib")
#pragma comment(lib, "rr-libstruct-static.lib")
#pragma comment(lib, "libsbml-static.lib")
#pragma comment(lib, "libxml2_xe.lib")
#pragma comment(lib, "blas.lib")
#pragma comment(lib, "lapack.lib")
#pragma comment(lib, "libf2c.lib")
#endif

namespace rr
{

//Initialize statics..
//bool RoadRunner::mComputeAndAssignConservationLaws     	= false;
//bool RoadRunner::mConservedTotalChanged             	= false;
//bool RoadRunner::mReMultiplyCompartments             	= true;

RoadRunner::RoadRunner(const string& compiler) :
    emptyModelStr("A model needs to be loaded before one can use this method"),
	mModelFolder("..//Models"),
    STEADYSTATE_THRESHOLD(1.E-2),
    mCVode(NULL),
    steadyStateSolver(NULL),
    mL(NULL),
    mL0(NULL),
    _L0(NULL),
    mN(NULL),
    mNr(NULL),
    DiffStepSize(0.05),
    mTimeStart(0),
    mTimeEnd(10),
    mNumPoints(21),
    mCurrentSBML(""),
    mModel(NULL),
    mModelDLL(NULL),
    mSimulation(NULL),
    mModelXMLFileName("sbml_model"),
    UseKinsol(false),
    mComputeAndAssignConservationLaws(true),
    mConservedTotalChanged(false),
    mCompiler(compiler)
{
     Log(lDebug4)<<"In RoadRunner ctor";
     mLS = LibStructural::getInstance();
     mCSharpGenerator    = new CSharpGenerator(mNOM);
     mCGenerator         = new CGenerator(mNOM);//Todo: memoryleak
     mModelGenerator     = mCGenerator;
     mTempFileFolder     = GetUsersTempDataFolder();
}

RoadRunner::~RoadRunner()
{
    Log(lDebug4)<<"In RoadRunner DTOR";
    delete mCSharpGenerator;
    delete mCGenerator;
    delete mModel;
    delete mCVode;
    if(mModelDLL)
    {
        //Unload the DLL
        FreeLibrary(mModelDLL);
    }

    delete mLS;
}

NOMSupport* RoadRunner::getNOM()
{
	return &mNOM;
}

CvodeInterface* RoadRunner::GetCVodeInterface()
{
    if(!mCVode && mModel != NULL)
    {
        mCVode = new CvodeInterface(this, mModel);
    }
    return mCVode;
}

bool RoadRunner::setCompiler(const string& compiler)
{
    return mCompiler.SetCompiler(compiler);
}

NLEQInterface* RoadRunner::GetNLEQInterface()
{
    if(!steadyStateSolver && mModel != NULL)
    {
        steadyStateSolver = new NLEQInterface(mModel);
    }
    return dynamic_cast<NLEQInterface*>(steadyStateSolver);
}

bool RoadRunner::isModelLoaded()
{
    return mModel ? true : false;
}

bool RoadRunner::UseSimulationSettings(SimulationSettings& settings)
{
    mSettings   = settings;
    mTimeStart  = mSettings.mStartTime;
    mTimeEnd    = mSettings.mEndTime;
    mNumPoints  = mSettings.mSteps + 1;
    return true;
}

bool RoadRunner::ComputeAndAssignConservationLaws()
{
	return mComputeAndAssignConservationLaws;
}

CGenerator*	RoadRunner::GetCGenerator()
{
	return dynamic_cast<CGenerator*>(mCGenerator);
}

CSharpGenerator* RoadRunner::GetCSharpGenerator()
{
	return dynamic_cast<CSharpGenerator*>(mCSharpGenerator);
}

bool RoadRunner::SetTempFileFolder(const string& folder)
{
	if(FolderExists(folder))
	{
		mTempFileFolder = folder;
		return true;
	}
	else
	{
		Log(lError)<<"The folder: "<<folder<<" don't exist...";
		return false;
	}
}

string RoadRunner::GetTempFileFolder()
{
	return mTempFileFolder;
}

int RoadRunner::createDefaultTimeCourseSelectionList()
{
	StringList theList;
    StringList oFloating  = getFloatingSpeciesIds();

	theList.Add("time");
    for(int i = 0; i < oFloating.Count(); i++)
    {
        theList.Add(oFloating[i]);
    }

    setTimeCourseSelectionList(theList);

	Log(lDebug)<<"The following is selected:";
	for(int i = 0; i < selectionList.size(); i++)
	{
		Log(lDebug)<<selectionList[i];
	}
    return selectionList.size();
}

int RoadRunner::createTimeCourseSelectionList()
{

	StringList theList = getSelectionListFromSettings(mSettings);

    if(theList.Count() < 2)
    {
        //AutoSelect
        theList.Add("Time");

        //Get All floating species
       StringList oFloating  = getFloatingSpeciesIds();
       for(int i = 0; i < oFloating.Count(); i++)
       {
            theList.Add(oFloating[i]);
       }
    }

	setTimeCourseSelectionList(theList);

	Log(lDebug)<<"The following is selected:";
	for(int i = 0; i < selectionList.size(); i++)
	{
		Log(lDebug)<<selectionList[i];
	}

	if(selectionList.size() < 2)
	{
		Log(lWarning)<<"You have not made a selection. No data is selected";
		return 0;
	}
	return selectionList.size();
}

ModelGenerator* RoadRunner::GetCodeGenerator()
{
	return mModelGenerator;
}

//NOM exposure
string RoadRunner::getParamPromotedSBML(const string& sArg)
{
	if(mModelGenerator)
    {
    	//This does not look very nice>>
    	return mModelGenerator->mNOM.getParamPromotedSBML(sArg);
    }

    return "";
}

void RoadRunner::Reset()
{
	if(mModelGenerator)
	{
		mModelGenerator->Reset();
	}
}

string RoadRunner::GetModelSourceCode()
{
	return mModelCode;
}

string RoadRunner::getCSharpCode()
{
    if(mCSharpGenerator)
    {
        return mCSharpGenerator->getSourceCode();
    }
    return "";
}

string RoadRunner::getCHeaderCode()
{
    if(mCGenerator)
    {
        return mCGenerator->GetHeaderCode();
    }
    return "";
}

string RoadRunner::getCSourceCode()
{
    if(mCGenerator)
    {
        return mCGenerator->GetSourceCode();
    }
    return "";
}

bool RoadRunner::InitializeModel()
{
    if(!mModel)
    {
        //Now create the Model using the compiled DLL
        mModel = CreateModel();

        if(!mModel)
        {
			Log(lError)<<"Failed Creating Model";
            return false ;
        }
    }

//    modelLoaded = true;
    mConservedTotalChanged = false;
    mModel->setCompartmentVolumes();
    mModel->initializeInitialConditions();
    mModel->setParameterValues();
    mModel->setCompartmentVolumes();
    mModel->setBoundaryConditions();
    mModel->setInitialConditions();
    mModel->convertToAmounts();
    mModel->evalInitialAssignments();

    mModel->computeRules(mModel->y, *mModel->ySize);
    mModel->convertToAmounts();

    if (mComputeAndAssignConservationLaws)
    {
        mModel->computeConservedTotals();
    }

    if(mCVode)
    {
        delete mCVode;
    }
    mCVode = new CvodeInterface(this, mModel);
	mModel->AssignCVodeInterface(mCVode);

    reset();
    return true;
}

SimulationData RoadRunner::GetSimulationResult()
{
    return mSimulationData;
}

double RoadRunner::GetValueForRecord(const TSelectionRecord& record)
{
    double dResult;

    switch (record.selectionType)
    {
        case TSelectionType::clFloatingSpecies:
            dResult = mModel->getConcentration(record.index);
		break;

        case TSelectionType::clBoundarySpecies:
            dResult = mModel->bc[record.index];
        break;

        case TSelectionType::clFlux:
            dResult = mModel->rates[record.index];
        break;

        case TSelectionType::clRateOfChange:
            dResult = mModel->dydt[record.index];
        break;

        case TSelectionType::clVolume:
            dResult = mModel->c[record.index];
        break;

        case TSelectionType::clParameter:
            {
                if (record.index > ((*mModel->gpSize) - 1))
                {
                    dResult = mModel->ct[record.index - (*mModel->gpSize)];
                }
                else
                {
                    dResult = mModel->gp[record.index];
                }
            }
		break;

        case TSelectionType::clFloatingAmount:
            dResult = mModel->amounts[record.index];
        break;

        case TSelectionType::clBoundaryAmount:
            int nIndex;
            if (mModelGenerator->compartmentList.find(mModelGenerator->boundarySpeciesList[record.index].compartmentName, nIndex))
            {
                dResult = mModel->bc[record.index] * mModel->c[nIndex];
            }
            else
            {
                dResult = 0.0;
            }
        break;

        case TSelectionType::clElasticity:
            dResult = getEE(record.p1, record.p2, false);
        break;

        case TSelectionType::clUnscaledElasticity:
            dResult = getuEE(record.p1, record.p2, false);
        break;

		// ********  Todo: Enable this.. ***********
        case TSelectionType::clEigenValue:    
//            vector< complex<double> >oComplex = LA.GetEigenValues(getReducedJacobian());
//            if (oComplex.Length > record.index)
//            {
//                dResult = oComplex[record.index].Real;
//            }
//            else
//                dResult = Double.NaN;
                dResult = 0.0;
        break;

        case TSelectionType::clStoichiometry:
            dResult = mModel->sr[record.index];
        break;

        default:
            dResult = 0.0;
        break;
    }
    return dResult;
}

double RoadRunner::GetNthSelectedOutput(const int& index, const double& dCurrentTime)
{
    TSelectionRecord record = selectionList[index];

    if (record.selectionType == TSelectionType::clTime)
    {
        return dCurrentTime;
    }
    else
    {
		return GetValueForRecord(record);
    }
}

void RoadRunner::AddNthOutputToResult(DoubleMatrix& results, int nRow, double dCurrentTime)
{
	stringstream msg;
    for (u_int j = 0; j < selectionList.size(); j++)
    {
        double out =  GetNthSelectedOutput(j, dCurrentTime);
        results(nRow,j) = out;
        msg<<tab<<out;
    }
    Log(lDebug1)<<"Added result row\t"<<nRow<<" : "<<msg.str();
}

vector<double> RoadRunner::BuildModelEvalArgument()
{
    vector<double> dResult;
    dResult.resize((*mModel->amountsSize) + (*mModel->rateRulesSize) );

    vector<double> dCurrentRuleValues = mModel->GetCurrentValues();

    for(int i = 0; i < (*mModel->rateRulesSize); i++)
    {
        dResult[i] = dCurrentRuleValues[i];
    }

    for(int i = 0; i < (*mModel->amountsSize); i++)
    {
        dResult[i + (*mModel->rateRulesSize)] = mModel->amounts[i];
    }

    return dResult;
}

////        private double[] BuildModelEvalArgument()
////        {
////            var dResult = new double[model.amounts.Length + model.rateRules.Length];
////            double[] dCurrentRuleValues = model.GetCurrentValues();
////            dCurrentRuleValues.CopyTo(dResult, 0);
////            model.amounts.CopyTo(dResult, model.rateRules.Length);
////            return dResult;
////        }

DoubleMatrix RoadRunner::runSimulation()
{
    if (mNumPoints <= 1)
    {
        mNumPoints = 2;
    }

	double hstep = (mTimeEnd - mTimeStart) / (mNumPoints - 1);
    int nrCols = selectionList.size();
    if(!nrCols)
    {
        nrCols = createDefaultTimeCourseSelectionList();
    }

    DoubleMatrix results(mNumPoints, nrCols);

    if(!mModel)
    {
        return results;
    }

    vector<double> y;
    y = BuildModelEvalArgument();
    mModel->evalModel(mTimeStart, y);
    AddNthOutputToResult(results, 0, mTimeStart);

    if (mCVode->HaveVariables())
    {
        int restartResult = mCVode->reStart(mTimeStart, mModel);
		if (restartResult != 0)
        {
            throw SBWApplicationException("Error in reStart call to CVODE");
        }
    }

    double tout = mTimeStart;

    //The simulation is executed right here..
    Log(lDebug)<<"Will run the OneStep function "<<mNumPoints<<" times";
    for (int i = 1; i < mNumPoints; i++)
    {
        Log(lDebug)<<"Step "<<i;
        mCVode->OneStep(tout, hstep);
        tout = mTimeStart + i * hstep;
        AddNthOutputToResult(results, i, tout);
    }
    Log(lDebug)<<"Simulation done..";
    Log(lDebug2)<<"Result: (point, time, value)";
    if(results.size())
    {
        for (int i = 0; i < mNumPoints; i++)
        {
            Log(lDebug2)<<i<<tab<<results(i,0)<<tab<<setprecision(16)<<results(i,1);
        }
    }
    return results;
}

void RoadRunner::DumpResults(TextWriter& writer, DoubleMatrix& data, const StringList& colLabels)
{
    for (int i = 0; i < colLabels.Count(); i++)
	{
        writer.Write(colLabels[i] + "\t");
    }

    writer.WriteLine();

    for (u_int i = 0; i < data.RSize(); i++)
    {
        for (u_int j = 0; j < data.CSize(); j++)
        {
            string val = ToString(data(i,j));
            writer.Write(val + "\t");
        }
        writer.WriteLine();
    }
}

bool RoadRunner::Simulate()
{
    if(!mModel)
    {
        Log(lError)<<"No model is loaded, can't simulate..";
        throw(Exception("There is no model loaded, can't simulate"));
    }

 	mRawSimulationData = simulate();

    //Populate simulation result
    PopulateResult();
    return true;
}

bool RoadRunner::PopulateResult()
{
    ArrayList  l = getAvailableTimeCourseSymbols();
    StringList list = getTimeCourseSelectionList();
    mSimulationData.SetColumnNames(list);
    mSimulationData.SetData(mRawSimulationData);
    return true;
}

bool RoadRunner::SimulateSBMLFile(const string& fileName, const bool& useConservationLaws)
{
    ComputeAndAssignConservationLaws(useConservationLaws);

    mModelXMLFileName = fileName;
    ifstream fs(mModelXMLFileName.c_str());
    if(!fs)
    {
        throw(Exception("Failed to open the model file:" + mModelXMLFileName));
    }

    Log(lInfo)<<"\n\n ===== Reading model file:"<<mModelXMLFileName<<" ==============";
    string sbml((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());
    fs.close();

    Log(lDebug5)<<"Loading SBML. SBML model code size: "<<sbml.size();

	loadSBML(sbml);

    mRawSimulationData = simulate();

    StringList list = getTimeCourseSelectionList();

    TextWriter writer(cout);
    DumpResults(writer, mRawSimulationData, list);
    return true;
}

bool RoadRunner::SimulateSBMLFile(const string& fileName, const bool& useConservationLaws, const double& startTime, const double& endTime, const int& mNumPoints)
{
//    var sim = new RoadRunner();
//    ComputeAndAssignConservationLaws(useConservationLaws);
//    sim.loadSBML(File.ReadAllText(fileName));
//
//    try
//    {
//        double[,] data = sim.simulateEx(startTime, endTime, mNumPoints);
//        ArrayList list = sim.getTimeCourseSelectionList();
//        TextWriter writer = Console.Error;
//
//        DumpResults(writer, data, list);
//    }
//    catch (Exception ex)
//    {
//        Debug.WriteLine(ex);
//    }
//
//    //Debug.WriteLine(sim.getCapabilities());
//
    return false;
}

bool RoadRunner::loadSBMLFromFile(const string& fileName)
{
	ifstream ifs(fileName.c_str());
    if(!ifs)
    {
        stringstream msg;
        msg<<"Failed opening file: "<<fileName;
        Log(lError)<<msg.str();
        return false;
    }

    std::string sbml((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

    Log(lDebug5)<<"Read SBML content from file:\n "<<sbml \
                << "\n============ End of SBML "<<endl;

    mModelXMLFileName = fileName;
    return loadSBML(sbml);
}

bool RoadRunner::loadSBML(const string& sbml)
{
    Log(lInfo)<<"Loading SBML into simulator";
    if (!sbml.size())
    {
        Log(lError)<<"No SBML content..!";
        return false;
    }

	// If the user loads the same model again, don't bother loading into NOM,
	// just reset the initial conditions
	if (mModelDLL != NULL && mModel != NULL && sbml == mCurrentSBML)
    {
        mCurrentSBML = sbml;
        return InitializeModel();
    }

    if(mModel != NULL)
    {
        unLoadModelDLL();
        delete mModel;
        mModel = NULL;
    }

    mCurrentSBML 	= sbml;
    string dllName  = GetDLLName();

     //Shall we compile model if it exists?
    bool compileIfDllExists = mSimulation ? mSimulation->CompileIfDllExists() : true;
    bool dllExists = FileExists(dllName);
    bool compile = true;
    if(dllExists && compileIfDllExists == false)
    {
        compile = false;
    }

    if(compile)
    {
        if(!GenerateAndCompileModel())
        {
            Log(lError)<<"Failed to generate and compile model";
            return false;
        }
    }

    if(!mModel)
    {
        unLoadModelDLL();
        Log(lError)<<"Failed to create ModelFromC";
        return false;
    }

    //Finally intitilaize the model..
    if(!InitializeModel())
    {
        Log(lError)<<"Failed Initializing C Model";
        return false;
    }

    _L  = mLS->getLinkMatrix();
    _L0 = mLS->getL0Matrix();
    _N  = mLS->getReorderedStoichiometryMatrix();
    _Nr = mLS->getNrMatrix();
    return true;
}

string RoadRunner::GetDLLName()
{
    string srcCodeFolder;
    srcCodeFolder = (mSimulation) ?
        mSimulation->GetTempDataFolder()
        :
        string(mTempFileFolder);


    string dllName  = srcCodeFolder + "\\" + ChangeFileExtensionTo(ExtractFileName(mModelXMLFileName), "dll");
    return dllName;
}

bool RoadRunner::GenerateModelCode(const string& sbml)
{
    if(sbml.size())
    {
        mCurrentSBML = sbml; //This should be used in stead of the file name below..
    }

    mModelGenerator->SetXMLModelFileName(mModelXMLFileName);

    string srcCodeFolder;
    if(mSimulation)
    {
        srcCodeFolder = mSimulation->GetTempDataFolder();
    }
    else
    {
        srcCodeFolder = mTempFileFolder;
    }

    mModelCode = mModelGenerator->generateModelCode(mCurrentSBML, ComputeAndAssignConservationLaws());

    if(!mModelCode.size())
    {
        Log(lError)<<"Failed to generate model code";
        return false;
    }

    if(!mModelGenerator->SaveSourceCodeToFolder(srcCodeFolder))
    {
        Log(lError)<<"Failed saving generated source code";
    }

    Log(lDebug5)<<" ------ Model Code --------\n"
                <<mModelCode
                <<" ----- End of Model Code -----\n";
    return true;
}

bool RoadRunner::CompileCurrentModel()
{
    CGenerator *codeGen = dynamic_cast<CGenerator*>(mModelGenerator);
    if(!codeGen)
    {
        //CodeGenerator has not been allocaed
        Log(lError)<<"Generate code before compiling....";
        return false;
    }

     //Compile the model
    if(!mCompiler.CompileC_DLL(codeGen->GetSourceCodeFileName()))
    {
        Log(lError)<<"Model failed compilation";
        return false;
    }
    Log(lDebug)<<"Model compiled successfully. ";
    Log(lDebug)<<mCompiler.GetDLLName()<<" was created";
    return true;
}

bool RoadRunner::unLoadModel()
{
    if(mModel)
    {
        delete mModel;
        mModel = NULL;
    }
    return unLoadModelDLL();
}

bool RoadRunner::unLoadModelDLL()
{
    //Make sure the dll is unloaded
    if(mModelDLL)	//Make sure the dll is unloaded
    {
        if(UnLoadDLL(mModelDLL))
        {
            mModelDLL = NULL;
            return true;
        }
        else
        {
            return false;
        }
    }
    return true;//No model is loaded..
}

bool RoadRunner::GenerateAndCompileModel()
{

    if(!GenerateModelCode(""))
    {
        Log(lError)<<"Failed generating model from SBML";
        return false;
    }

    //Make sure the dll is unloaded
    if(mModelDLL)	//Make sure the dll is unloaded
    {
        UnLoadDLL(mModelDLL);
    }

    string dllName  = GetDLLName();
    //Remove DLL
    if(FileExists(dllName) == true && remove(dllName.c_str()) != 0)
    {
        Log(lError)<<"Failed removing dll: "<<dllName;
        return false;
    }

    if(!CompileCurrentModel())
    {
        Log(lError)<<"Failed compiling model";
        return false;
    }

    //Load the DLL
    mModelDLL = LoadDLL(dllName);

    //Now create the Model using the compiled DLL
    mModel = CreateModel();

    if(!mModel)
    {
        Log(lError)<<"Failed to create Model";
        return false;
    }

    //Finally initialize the model..
    if(!InitializeModel())
    {
        Log(lError)<<"Failed Initializing Model";
        return false;
    }

    return true;
}

ModelFromC* RoadRunner::CreateModel()
{
    //Load dll
    if(!mModelDLL)
    {
        string   dllName  = GetDLLName();
        //Load the DLL
        mModelDLL = LoadDLL(dllName);
    }

    //Create a model
    if(mModelDLL)
    {
        CGenerator *codeGen = dynamic_cast<CGenerator*>(mModelGenerator);
        ModelFromC *rrCModel = new ModelFromC(codeGen, mModelDLL);     //Todo: memoryleak
        mModel = rrCModel;            //Should use an auto pointer?
    }
    else
    {
        Log(lError)<<"Failed to create DLL for model";
        mModel = NULL;
    }

    return mModel;
}

//Reset the simulator back to the initial conditions specified in the SBML model
void RoadRunner::reset()
{
    if (!mModelDLL)
    {
        // rather make sure that the simulator is!!!! in a stable state
        mModel = NULL;
        mCurrentSBML = "";
    }
    else
    {
        mModel->SetTime(0.0);

        // Reset the event flags
        mModel->resetEvents();
        mModel->setCompartmentVolumes();
        mModel->setInitialConditions();
        mModel->convertToAmounts();

        // in case we have ODE rules we should assign those as initial values
        mModel->InitializeRateRuleSymbols();
        mModel->InitializeRates();

        // and of course initial assignments should override anything
        mModel->evalInitialAssignments();
        mModel->convertToAmounts();

        // also we might need to set some initial assignment rules.
        mModel->convertToConcentrations();
        mModel->computeRules(mModel->y, *mModel->ySize);
        mModel->InitializeRates();
        mModel->InitializeRateRuleSymbols();
        mModel->evalInitialAssignments();
        mModel->computeRules(mModel->y, *mModel->ySize);

        mModel->convertToAmounts();

        if (mComputeAndAssignConservationLaws && !mConservedTotalChanged)
        {
            mModel->computeConservedTotals();
        }

        mCVode->AssignNewVector(mModel, true);
        mCVode->TestRootsAtInitialTime();

        //double hstep = (timeEnd - mTimeStart) / (mNumPoints - 1);
        //CvodeInterface.MaxStep = Math.Min(CvodeInterface.MaxStep, hstep);
        //if (CvodeInterface.MaxStep == 0)
        //    CvodeInterface.MaxStep = hstep;

        mModel->SetTime(0.0);
        mCVode->reStart(0.0, mModel);

        mCVode->assignments.clear();//Clear();

        try
        {
            mModel->testConstraints();
        }
        catch (const Exception& e)
        {
            mModel->Warnings.push_back("Constraint Violated at time = 0\n" + e.Message());
            Log(lWarning)<<"Constraint Violated at time = 0\n"<<e.Message();
        }
    }
}

DoubleMatrix RoadRunner::simulate()
{
    try
    {
        if (!mModel)
        {
            throw SBWApplicationException(emptyModelStr);
        }

        if (mTimeEnd <= mTimeStart)
        {
            throw SBWApplicationException("Error: time end must be greater than time start");
        }
        return runSimulation();
    }
    catch (const Exception& e)
    {
        throw SBWApplicationException("Unexpected error from simulate(): " + e.Message());
    }
}

// Help("Extension method to simulate (time start, time end, number of points). This routine resets the model to its initial condition before running the simulation (unlike simulate())"
DoubleMatrix RoadRunner::simulateEx(const double& startTime, const double& endTime, const int& numberOfPoints)
{
    try
    {
        if (!mModel)
        {
            throw SBWApplicationException(emptyModelStr);
        }

        reset(); // reset back to initial conditions

        if (endTime < 0 || startTime < 0 || numberOfPoints <= 0 || endTime <= startTime)
        {
            throw SBWApplicationException("Illegal input to simulateEx");
        }

        mTimeEnd            = endTime;
        mTimeStart          = startTime;
        mNumPoints          = numberOfPoints;
        mRawSimulationData  = runSimulation();
        PopulateResult();
        return mRawSimulationData;
    }
    catch(const Exception& e)
    {
        throw SBWApplicationException("Unexpected error from simulateEx()", e.Message());
    }
}

//Returns the currently selected columns that will be returned by calls to simulate() or simulateEx(,,).
StringList RoadRunner::getTimeCourseSelectionList()
{
    StringList oResult;

    if (!mModel)
    {
        oResult.Add("time");
        return oResult;
    }

    StringList oFloating    = mModelGenerator->getFloatingSpeciesConcentrationList();
    StringList oBoundary    = mModelGenerator->getBoundarySpeciesList();
    StringList oFluxes      = mModelGenerator->getReactionIds();
    StringList oVolumes     = mModelGenerator->getCompartmentList();
    StringList oRates       = getRateOfChangeIds();
    StringList oParameters  = getParameterIds();

    vector<TSelectionRecord>::iterator iter;
//    int size = selectionList.size();
    for(iter = selectionList.begin(); iter != selectionList.end(); iter++)
    {
        TSelectionRecord record = (*iter);
        switch (record.selectionType)
        {
            case TSelectionType::clTime:
                oResult.Add("time");
                break;
            case TSelectionType::clBoundaryAmount:
                oResult.Add(Format("[{0}]", oBoundary[record.index]));
                break;
            case TSelectionType::clBoundarySpecies:
                oResult.Add(oBoundary[record.index]);
                break;
            case TSelectionType::clFloatingAmount:
                oResult.Add(Format("[{0}]", oFloating[record.index]));
                break;
            case TSelectionType::clFloatingSpecies:
                oResult.Add(oFloating[record.index]);
                break;
            case TSelectionType::clVolume:
                oResult.Add(oVolumes[record.index]);
                break;
            case TSelectionType::clFlux:
                oResult.Add(oFluxes[record.index]);
                break;
            case TSelectionType::clRateOfChange:
                oResult.Add(oRates[record.index]);
                break;
            case TSelectionType::clParameter:
                oResult.Add(oParameters[record.index]);
                break;
            case TSelectionType::clEigenValue:
                oResult.Add("eigen_" + record.p1);
                break;
            case TSelectionType::clElasticity:
                oResult.Add(Format("EE:{0},{1}", record.p1, record.p2));
                break;
            case TSelectionType::clUnscaledElasticity:
                oResult.Add(Format("uEE:{0},{1}", record.p1, record.p2));
                break;
            case TSelectionType::clStoichiometry:
                oResult.Add(record.p1);
                break;
        }
    }
    return oResult;
}

// Help("Compute the steady state of the model, returns the sum of squares of the solution")
double RoadRunner::steadyState()
{
    if (!mModel)
    {
        throw SBWApplicationException(emptyModelStr);
    }

    if (UseKinsol)
    {
            //steadyStateSolver = NULL;//new KinSolveInterface(mModel);
            Log(lError)<<"Kinsol solver is not enabled...";
            return -1;
    }
    else
    {
        steadyStateSolver = new NLEQInterface(mModel);
    }

    //Get a std vector for the solver
    vector<double> someAmounts;
    CopyCArrayToStdVector(mModel->amounts, someAmounts, mModel->getNumIndependentVariables());

    double ss = steadyStateSolver->solve(someAmounts);
    if(ss < 0)
    {
        Log(lError)<<"Steady State solver failed...";
    }
    mModel->convertToConcentrations();

    delete steadyStateSolver;
    steadyStateSolver = NULL;

    return ss;
}

void RoadRunner::setParameterValue(const TParameterType& parameterType, const int& parameterIndex, const double& value)
{
    switch (parameterType)
    {
        case TParameterType::ptBoundaryParameter:
            mModel->bc[parameterIndex] = value;
        break;

        case TParameterType::ptGlobalParameter:
            mModel->gp[parameterIndex] = value;
        break;

        case TParameterType::ptFloatingSpecies:
            mModel->y[parameterIndex] = value;
        break;

        case TParameterType::ptConservationParameter:
            mModel->ct[parameterIndex] = value;
        break;

        case TParameterType::ptLocalParameter:
            throw Exception("Local parameters not permitted in setParameterValue (getCC, getEE)");
    }
}

double RoadRunner::getParameterValue(const TParameterType& parameterType, const int& parameterIndex)
{
    switch (parameterType)
    {
        case TParameterType::ptBoundaryParameter:
            return mModel->bc[parameterIndex];

        case TParameterType::ptGlobalParameter:
            return mModel->gp[parameterIndex];

        // Used when calculating elasticities
        case TParameterType::ptFloatingSpecies:
            return mModel->y[parameterIndex];

        case TParameterType::ptConservationParameter:
            return mModel->ct[parameterIndex];

        case TParameterType::ptLocalParameter:
            throw Exception("Local parameters not permitted in getParameterValue (getCC?)");

        default:
            return 0.0;
    }
}

// Help("This method turns on / off the computation and adherence to conservation laws."
//              + "By default roadRunner will discover conservation cycles and reduce the model accordingly.")
void RoadRunner::ComputeAndAssignConservationLaws(const bool& bValue)
{
    mComputeAndAssignConservationLaws = bValue;
    if(mModel != NULL)
    {
        //We need no recompile the model if this flag changes..
        GenerateAndCompileModel();
    }
}

// Help("Returns the names given to the rate of change of the floating species")
StringList RoadRunner::getRateOfChangeIds()
{
    if (!mModel)
    {
        throw SBWApplicationException(emptyModelStr);
    }

    StringList sp = mModelGenerator->getFloatingSpeciesConcentrationList(); // Reordered list
    for (int i = 0; i < sp.Count(); i++)
    {
        sp[i] = sp[i] + "'";
    }
    return sp;
}

// Help("Gets the list of compartment names")
StringList RoadRunner::getCompartmentIds()
{
    if (!mModel)
    {
        throw SBWApplicationException(emptyModelStr);
    }
    return mModelGenerator->getCompartmentList();
}

StringList RoadRunner::getParameterIds()
{
    if (!mModel)
    {
        throw SBWApplicationException(emptyModelStr);
    }
    StringList sp = mModelGenerator->getGlobalParameterList(); // Reordered list
    return sp;
}

// [Help("Get scaled elasticity coefficient with respect to a global parameter or species")]
double RoadRunner::getEE(const string& reactionName, const string& parameterName)
{
    return getEE(reactionName, parameterName, true);
}

// [Help("Get scaled elasticity coefficient with respect to a global parameter or species. Optionally the model is brought to steady state after the computation.")]
double RoadRunner::getEE(const string& reactionName, const string& parameterName, bool computeSteadyState)
{
    TParameterType parameterType;
    int reactionIndex;
    int parameterIndex;

    if (!mModel)
    {
        throw SBWApplicationException(emptyModelStr);
    }

    // Check the reaction name
    if (!mModelGenerator->GetReactionList().find(reactionName, reactionIndex))
    {
        throw SBWApplicationException(Format("Unable to locate reaction name: [{0}]", reactionName));
    }

    // Find out what kind of parameter we are dealing with
    if (mModelGenerator->GetFloatingSpeciesConcentrationList().find(parameterName, parameterIndex))
    {
        parameterType = TParameterType::ptFloatingSpecies;
    }
    else if (mModelGenerator->GetBoundarySpeciesList().find(parameterName, parameterIndex))
    {
        parameterType = TParameterType::ptBoundaryParameter;
    }
    else if (mModelGenerator->GetGlobalParameterList().find(parameterName, parameterIndex))
    {
        parameterType = TParameterType::ptGlobalParameter;
    }
    else if (mModelGenerator->GetConservationList().find(parameterName, parameterIndex))
    {
        parameterType = TParameterType::ptConservationParameter;
    }
    else
    {
        throw SBWApplicationException(Format("Unable to locate variable: [{0}]", parameterName));
    }

    mModel->computeReactionRates(mModel->GetTime(), mModel->y);
    double variableValue = mModel->rates[reactionIndex];
    double parameterValue = getParameterValue(parameterType, parameterIndex);
    if (variableValue == 0)
    {
        variableValue = 1e-12;
    }
    return getuEE(reactionName, parameterName, computeSteadyState) * parameterValue / variableValue;
}


//        [Help("Get unscaled elasticity coefficient with respect to a global parameter or species")]
double RoadRunner::getuEE(const string& reactionName, const string& parameterName)
{
    return getuEE(reactionName, parameterName, true);
}

//[Help("Get unscaled elasticity coefficient with respect to a global parameter or species. Optionally the model is brought to steady state after the computation.")]
double RoadRunner::getuEE(const string& reactionName, const string& parameterName, bool computeSteadystate)
{
    try
    {
        if (mModel)
        {
            TParameterType parameterType;
            double originalParameterValue;
            int reactionIndex;
            int parameterIndex;
            double f1;
            double f2;

            mModel->convertToConcentrations();
            mModel->computeReactionRates(mModel->GetTime(), mModel->y);

            // Check the reaction name
            if (!mModelGenerator->GetReactionList().find(reactionName, reactionIndex))
            {
                throw SBWApplicationException("Unable to locate reaction name: [" + reactionName + "]");
            }

            // Find out what kind of parameter we are dealing with
            if (mModelGenerator->GetFloatingSpeciesConcentrationList().find(parameterName, parameterIndex))
            {
                parameterType = TParameterType::ptFloatingSpecies;
                originalParameterValue = mModel->y[parameterIndex];
            }
            else if (mModelGenerator->GetBoundarySpeciesList().find(parameterName, parameterIndex))
            {
                parameterType = TParameterType::ptBoundaryParameter;
                originalParameterValue = mModel->bc[parameterIndex];
            }
            else if (mModelGenerator->GetGlobalParameterList().find(parameterName, parameterIndex))
            {
                parameterType = TParameterType::ptGlobalParameter;
                originalParameterValue = mModel->gp[parameterIndex];
            }
            else if (mModelGenerator->GetConservationList().find(parameterName, parameterIndex))
            {
                parameterType = TParameterType::ptConservationParameter;
                originalParameterValue = mModel->ct[parameterIndex];
            }
            else throw SBWApplicationException("Unable to locate variable: [" + parameterName + "]");

            double hstep = DiffStepSize*originalParameterValue;
            if (fabs(hstep) < 1E-12)
            {
                hstep = DiffStepSize;
            }

            try
            {
                mModel->convertToConcentrations();

                setParameterValue(parameterType, parameterIndex, originalParameterValue + hstep);
                mModel->computeReactionRates(mModel->GetTime(), mModel->y);
                double fi = mModel->rates[reactionIndex];

                setParameterValue(parameterType, parameterIndex, originalParameterValue + 2*hstep);
                mModel->computeReactionRates(mModel->GetTime(), mModel->y);
                double fi2 = mModel->rates[reactionIndex];

                setParameterValue(parameterType, parameterIndex, originalParameterValue - hstep);
                mModel->computeReactionRates(mModel->GetTime(), mModel->y);
                double fd = mModel->rates[reactionIndex];

                setParameterValue(parameterType, parameterIndex, originalParameterValue - 2*hstep);
                mModel->computeReactionRates(mModel->GetTime(), mModel->y);
                double fd2 = mModel->rates[reactionIndex];

                // Use instead the 5th order approximation double unscaledValue = (0.5/hstep)*(fi-fd);
                // The following separated lines avoid small amounts of roundoff error
                f1 = fd2 + 8*fi;
                f2 = -(8*fd + fi2);
            }
            catch(...)
            {}
            //finally
            {
                // What ever happens, make sure we restore the parameter level
                setParameterValue(parameterType, parameterIndex, originalParameterValue);
                mModel->computeReactionRates(mModel->GetTime(), mModel->y);
                if (computeSteadystate) steadyState();
            }
            return 1/(12*hstep)*(f1 + f2);
        }
        else throw SBWApplicationException(emptyModelStr);
    }
    catch (const Exception& e)
    {
        throw SBWApplicationException("Unexpected error from getuEE ()" +  e.Message());
    }
}

// Help("Updates the model based on all recent changes")
void RoadRunner::EvalModel()
{
    if (!mModel)
    {
        throw SBWApplicationException(emptyModelStr);
    }
    mModel->convertToAmounts();
    vector<double> args = mCVode->BuildEvalArgument();
    mModel->evalModel(mModel->GetTime(), args);
}

void RoadRunner::setTimeCourseSelectionList(const string& list)
{
    StringList aList(list,", ");
    setTimeCourseSelectionList(aList);
}

// Help("Set the columns to be returned by simulate() or simulateEx(), valid symbol names include" +
//              " time, species names, , volume, reaction rates and rates of change (speciesName')")
void RoadRunner::setTimeCourseSelectionList(const StringList& _selList)
{
    selectionList.clear();
    StringList newSelectionList(_selList);
    StringList fs = mModelGenerator->getFloatingSpeciesConcentrationList();
    StringList bs = mModelGenerator->getBoundarySpeciesList();
    StringList rs = mModelGenerator->getReactionIds();
    StringList vol= mModelGenerator->getCompartmentList();
    StringList gp = mModelGenerator->getGlobalParameterList();
//    StringList sr = mModelGenerator->ModifiableSpeciesReferenceList;

    for (int i = 0; i < _selList.Count(); i++)
    {
    	if (ToUpper(newSelectionList[i]) == ToUpper("time"))
        {
        	selectionList.push_back(TSelectionRecord(0, clTime));
        }

        // Check for species
        for (int j = 0; j < fs.Count(); j++)
        {
            if (newSelectionList[i] == fs[j])
            {
               	selectionList.push_back(TSelectionRecord(j, TSelectionType::clFloatingSpecies));
                break;
            }

            if (newSelectionList[i] == "[" + fs[j] + "]")
            {
               	selectionList.push_back(TSelectionRecord(j, clFloatingAmount));
                break;
            }

            // Check for species rate of change
            if (newSelectionList[i] == fs[j] + "'")
            {
                selectionList.push_back(TSelectionRecord(j, clRateOfChange));
                break;
            }
        }

        // Check fgr boundary species
        for (int j = 0; j < bs.Count(); j++)
        {
            if (newSelectionList[i] == bs[j])
            {
                selectionList.push_back(TSelectionRecord(j, clBoundarySpecies));
                break;
            }
            if (newSelectionList[i] == "[" + bs[j] + "]")
            {
                selectionList.push_back(TSelectionRecord(j, clBoundaryAmount));
                break;
            }
        }

        for (int j = 0; j < rs.Count(); j++)
        {
            // Check for reaction rate
            if (newSelectionList[i] == rs[j])
            {
                selectionList.push_back(TSelectionRecord(j, clFlux));
                break;
            }
        }

        for (int j = 0; j < vol.Count(); j++)
        {
            // Check for volume
            if (newSelectionList[i] == vol[j])
            {
                selectionList.push_back(TSelectionRecord(j, clVolume));
                break;
            }

            if (newSelectionList[i] == "[" + vol[j] + "]")
            {
                selectionList.push_back(TSelectionRecord(j, clVolume));
                break;
            }
        }

        for (int j = 0; j < gp.Count(); j++)
        {
            if (newSelectionList[i] == gp[j])
            {
                selectionList.push_back(TSelectionRecord(j, clParameter));
                break;
            }
        }

//        if (((string)newSelectionList[i]).StartsWith("EE:"))
//        {
//            string parameters = ((string)newSelectionList[i]).Substring(3);
//            var p1 = parameters.Substring(0, parameters.IndexOf(","));
//            var p2 = parameters.Substring(parameters.IndexOf(",") + 1);
//            selectionList[i].selectionType = TSelectionType::clElasticity;
//            selectionList[i].p1 = p1;
//            selectionList[i].p2 = p2;
//        }
//
//        if (((string)newSelectionList[i]).StartsWith("uEE:"))
//        {
//            string parameters = ((string)newSelectionList[i]).Substring(4);
//            var p1 = parameters.Substring(0, parameters.IndexOf(","));
//            var p2 = parameters.Substring(parameters.IndexOf(",") + 1);
//            selectionList[i].selectionType = TSelectionType::clUnscaledElasticity;
//            selectionList[i].p1 = p1;
//            selectionList[i].p2 = p2;
//        }
//        if (((string)newSelectionList[i]).StartsWith("eigen_"))
//        {
//            var species = ((string)newSelectionList[i]).Substring("eigen_".Length);
//            selectionList[i].selectionType = TSelectionType::clEigenValue;
//            selectionList[i].p1 = species;
//            mModelGenerator->floatingSpeciesConcentrationList.find(species, out selectionList[i].index);
//        }
//
//        int index;
//        if (sr.find((string)newSelectionList[i], out index))
//        {
//            selectionList[i].selectionType = TSelectionType::clStoichiometry;
//            selectionList[i].index = index;
//            selectionList[i].p1 = (string) newSelectionList[i];
//        }
    }
}


// Help(
//            "Carry out a single integration step using a stepsize as indicated in the method call (the intergrator is reset to take into account all variable changes). Arguments: double CurrentTime, double StepSize, Return Value: new CurrentTime."
//            )
double RoadRunner::oneStep(const double& currentTime, const double& stepSize)
{
    return oneStep(currentTime, stepSize, true);
}

//Help(
//   "Carry out a single integration step using a stepsize as indicated in the method call. Arguments: double CurrentTime, double StepSize, bool: reset integrator if true, Return Value: new CurrentTime."
//   )
double RoadRunner::oneStep(const double& currentTime, const double& stepSize, const bool& reset)
{
    if (!mModel)
    {
        throw SBWApplicationException(emptyModelStr);
    }

    if (reset)
    {
        mCVode->reStart(currentTime, mModel);
    }
    return mCVode->OneStep(currentTime, stepSize);
}

// Help("Compute the reduced Jacobian at the current operating point")
ls::DoubleMatrix RoadRunner::getReducedJacobian()
{
    try
    {
        if (mModel)
        {
            ls::DoubleMatrix uelast = getUnscaledElasticityMatrix();
            if(!_Nr)
            {
            	return ls::DoubleMatrix(0,0);
            }
            ls::DoubleMatrix I1 = mult((*_Nr), uelast);
            _L = mLS->getLinkMatrix();
            return mult(I1, (*_L));
        }
        throw SBWApplicationException(emptyModelStr);
    }
    catch (const Exception& e)
    {
        throw SBWApplicationException("Unexpected error from fullJacobian()", e.Message());
    }
}

// Returns eigenvalues, first column real part, second column imaginary part
// -------------------------------------------------------------------------
ls::DoubleMatrix RoadRunner::getEigenvalues()
{
    try
    {
	    if (!mModel)
	    {
            throw SBWApplicationException(emptyModelStr);
        }
        ls::DoubleMatrix result(getNumberOfIndependentSpecies(), 2);
        ls::DoubleMatrix mat = getReducedJacobian();
        LibLA LA;

        vector<Complex> oComplex = LA.getEigenValues(mat);
        for (int i = 0; i < oComplex.size(); i++)
        {
	        result[i][0] = oComplex[i].Real;
	        result[i][1] = oComplex[i].Imag;
        }
        return result;
    }
    catch (const Exception& e)
    {
        throw SBWApplicationException("Unexpected error from fullJacobian()", e.Message());
    }
}

// Help("Compute the full Jacobian at the current operating point")
DoubleMatrix RoadRunner::getFullJacobian()
{
    try
    {
        if (!mModel)
        {
	        throw SBWApplicationException(emptyModelStr);
        }
		DoubleMatrix* rsm;
        DoubleMatrix uelast = getUnscaledElasticityMatrix();
		if(mComputeAndAssignConservationLaws)
		{
			rsm    = mLS->getReorderedStoichiometryMatrix();
		}
		else
		{
			rsm = mLS->getStoichiometryMatrix();
		}        
        return mult(*rsm, uelast);
    }
    catch (const Exception& e)
    {
        throw SBWApplicationException("Unexpected error from fullJacobian()", e.Message());
    }
}

// ---------------------------------------------------------------------
// Start of Level 4 API Methods
// ---------------------------------------------------------------------
ls::DoubleMatrix* RoadRunner::getLinkMatrix()
{
    try
    {
       if (!mModel)
	   {
	       throw SBWApplicationException(emptyModelStr);
	   }
	   return _L;
    }
    catch (const Exception& e)
    {
         throw SBWApplicationException("Unexpected error from getLinkMatrix()", e.Message());
    }
}

ls::DoubleMatrix* RoadRunner::getNrMatrix()
{
    try
    {
       if (mModel)
	   {
		   return _Nr;
	   }
       throw SBWApplicationException(emptyModelStr);
    }
    catch (const Exception& e)
    {
         throw SBWApplicationException("Unexpected error from getNrMatrix()", e.Message());
    }
}

ls::DoubleMatrix* RoadRunner::getL0Matrix()
{
    try
    {
       if (mModel)
	   {
		   return _L0;
	   }
       throw SBWApplicationException(emptyModelStr);
    }
    catch (const Exception& e)
    {
         throw SBWApplicationException("Unexpected error from getL0Matrix()", e.Message());
    }
}

// Help("Returns the stoichiometry matrix for the currently loaded model")
DoubleMatrix RoadRunner::getStoichiometryMatrix()
{
    try
    {
		LibStructural::DoubleMatrix* aMat = mLS->getFullyReorderedStoichiometryMatrix();
        if (!mModel || !aMat)
        {
	        throw SBWApplicationException(emptyModelStr);
		}


        //Todo: Room to improve how matrices are handled across LibStruct/RoadRunner/C-API
        DoubleMatrix mat;

        mat.resize(aMat->numRows(), aMat->numCols());
        for(int row = 0; row < mat.RSize(); row++)
        {
            for(int col = 0; col < mat.CSize(); col++)
            {
                mat(row,col) = (*aMat)(row,col);
            }
        }
        return mat;
    }
    catch (const Exception& e)
    {
        throw SBWApplicationException("Unexpected error from getStoichiometryMatrix()" + e.Message());
    }
}

DoubleMatrix RoadRunner::getConservationMatrix()
{
    DoubleMatrix mat;

    try
    {
       if (mModel)
	   {
		   LibStructural::DoubleMatrix* aMat = mLS->getGammaMatrix();
            if (aMat)
            {
                mat.resize(aMat->numRows(), aMat->numCols());
                for(int row = 0; row < mat.RSize(); row++)
                {
                    for(int col = 0; col < mat.CSize(); col++)
                    {
                        mat(row,col) = (*aMat)(row,col);
                    }
                }
            }
            return mat;

	   }
       throw SBWApplicationException(emptyModelStr);
    }
    catch (const Exception& e)
    {
         throw SBWApplicationException("Unexpected error from getConservationMatrix()", e.Message());
    }
}

// Help("Returns the number of dependent species in the model")
int RoadRunner::getNumberOfDependentSpecies()
{
    try
    {
        if (mModel)
        {
            //return mStructAnalysis.GetInstance()->getNumDepSpecies();
            return mLS->getNumDepSpecies();
        }

        throw SBWApplicationException(emptyModelStr);
    }

    catch(Exception &e)
    {
        throw SBWApplicationException("Unexpected error from getNumberOfDependentSpecies()", e.Message());
    }
}

// Help("Returns the number of independent species in the model")
int RoadRunner::getNumberOfIndependentSpecies()
{
    try
    {
        if (mModel)
        {
            return mLS->getNumIndSpecies();
        }
        //return StructAnalysis.getNumIndSpecies();
        throw SBWApplicationException(emptyModelStr);
    }
    catch (Exception &e)
    {
        throw SBWApplicationException("Unexpected error from getNumberOfIndependentSpecies()", e.Message());
    }
}

double RoadRunner::getVariableValue(const TVariableType& variableType, const int& variableIndex)
{
    switch (variableType)
    {
        case vtFlux:
            return mModel->rates[variableIndex];

        case vtSpecies:
            return mModel->y[variableIndex];

        default:
            throw SBWException("Unrecognised variable in getVariableValue");
    }
}


///// <summary>
///// Fills the second argument with the Inverse of the first argument
///// </summary>
///// <param name="T2">The Matrix to calculate the Inverse for</param>
///// <param name="Inv">will be overriden wiht the inverse of T2 (must already be allocated)</param>
//void RoadRunner::GetInverse(ComplexMatrix& T2, ComplexMatrix& Inv)
//{
//    try
//    {
////        ComplexMatrix T8 = ls::GetInverse(ConvertComplex(T2.data));
////        for (int i1 = 0; i1 < Inv.nRows; i1++)
////        {
////            for (int j1 = 0; j1 < Inv.nCols; j1++)
////            {
////                Inv[i1, j1].Real = T8[i1][j1].Real;
////                Inv[i1, j1].Imag = T8[i1][j1].Imag;
////            }
////        }
//    }
//    catch (Exception)
//    {
//        throw SBWApplicationException("Could not calculate the Inverse");
//    }
//}

// Help(
//            "Derpar Continuation, stepSize = stepsize; independentVariable = index to parameter; parameterType = {'globalParameter', 'boundarySpecies'"
//            )
//        void RoadRunner::computeContinuation(double stepSize, int independentVariable, string parameterTypeStr)
//        {
//            if (!mModel) throw SBWApplicationException(emptyModelStr);
//            var derpar = new TDerpar(this, mModel->getNumTotalVariables, mModel->getNumIndependentVariables);
//            derpar.setup(mModel->amounts);
//            switch (parameterTypeStr)
//            {
//                case "globalParameter":
//                    mModel->amounts =
//                        (double[])
//                        derpar.evalOneStep(mModel->amounts, stepSize, independentVariable, TDerpar.GLOBAL_PARAMETER_TYPE).
//                            Clone();
//                    break;
//                case "boundarySpecies":
//                    mModel->amounts =
//                        (double[])
//                        derpar.evalOneStep(mModel->amounts, stepSize, independentVariable, TDerpar.BOUNDARY_SPECIES_TYPE).
//                            Clone();
//                    break;
//            }
//        }
//
//  Help("Returns the Symbols of all Flux Control Coefficients.")
ArrayList RoadRunner::getFluxControlCoefficientIds()
{
    ArrayList oResult;
    if (!mModel)
    {
        return oResult;
    }

    StringList oReactions       = getReactionIds();
    StringList oParameters      = mModelGenerator->getGlobalParameterList();
    StringList oBoundary        = mModelGenerator->getBoundarySpeciesList();
    StringList oConservation    = mModelGenerator->getConservationList();

    for(int i = 0; i < oReactions.Count(); i++)
    {
        string s = oReactions[i];

        ArrayList oCCReaction;
        ArrayList oInner;
        oCCReaction.Add(s);

        for(int i = 0; i < oParameters.Count(); i++)
        {
            oInner.Add("CC:" + s + "," + oParameters[i]);
        }

        for(int i = 0; i < oBoundary.Count(); i++)
        {
            oInner.Add("CC:" + s + "," + oBoundary[i]);
        }

        for(int i = 0; i < oConservation.Count(); i++)
        {
            oInner.Add("CC:" + s + "," + oConservation[i]);
        }

        oCCReaction.Add(oInner);
        oResult.Add(oCCReaction);
    }

    return oResult;
}


//  Help("Returns the Symbols of all Unscaled Flux Control Coefficients.")
ArrayList RoadRunner::getUnscaledFluxControlCoefficientIds()
{
    ArrayList oResult;// = new ArrayList();
    if (!mModel)
    {
        return oResult;
    }

    StringList oReactions = getReactionIds();
    StringList oParameters = mModelGenerator->getGlobalParameterList();
    StringList oBoundary = mModelGenerator->getBoundarySpeciesList();
    StringList oConservation = mModelGenerator->getConservationList();

    for(int i = 0; i < oReactions.Count(); i++)
    {
        string s = oReactions[i];

        ArrayList oCCReaction;
        ArrayList oInner;
        oCCReaction.Add(s);

        for(int i = 0; i < oParameters.Count(); i++)
        {
            oInner.Add("uCC:" + s + "," + oParameters[i]);
        }

        for(int i = 0; i < oBoundary.Count(); i++)
        {
            oInner.Add("uCC:" + s + "," + oBoundary[i]);
        }

        for(int i = 0; i < oConservation.Count(); i++)
        {
            oInner.Add("uCC:" + s + "," + oConservation[i]);
        }

        oCCReaction.Add(oInner);
        oResult.Add(oCCReaction);
    }

    return oResult;
}

// Help("Returns the Symbols of all Concentration Control Coefficients.")
ArrayList RoadRunner::getConcentrationControlCoefficientIds()
{
    ArrayList oResult;// = new ArrayList();
    if (!mModel)
    {
        return oResult;
    }

    StringList oFloating        = getFloatingSpeciesIds();
    StringList oParameters      = mModelGenerator->getGlobalParameterList();
    StringList oBoundary        = mModelGenerator->getBoundarySpeciesList();
    StringList oConservation    = mModelGenerator->getConservationList();

    for(int i = 0; i < oFloating.Count(); i++)
    {
        string s = oFloating[i];
        ArrayList oCCFloating;
        ArrayList oInner;
        oCCFloating.Add(s);

        for(int i = 0; i < oParameters.Count(); i++)
        {
            oInner.Add("CC:" + s + "," + oParameters[i]);
        }

        for(int i = 0; i < oBoundary.Count(); i++)
        {
            oInner.Add("CC:" + s + "," + oBoundary[i]);
        }

        for(int i = 0; i < oConservation.Count(); i++)
        {
            oInner.Add("CC:" + s + "," + oConservation[i]);
        }

        oCCFloating.Add(oInner);
        oResult.Add(oCCFloating);
    }

    return oResult;
}


// Help("Returns the Symbols of all Unscaled Concentration Control Coefficients.")
ArrayList RoadRunner::getUnscaledConcentrationControlCoefficientIds()
{
    ArrayList oResult;
    if (!mModel)
    {
        return oResult;
    }

    StringList oFloating        = getFloatingSpeciesIds();
    StringList oParameters      = mModelGenerator->getGlobalParameterList();
    StringList oBoundary        = mModelGenerator->getBoundarySpeciesList();
    StringList oConservation    = mModelGenerator->getConservationList();

    for(int i = 0; i < oFloating.Count(); i++)
    {
        string s = oFloating[i];
        ArrayList oCCFloating;
        ArrayList oInner;
        oCCFloating.Add(s);

        for(int i = 0; i < oParameters.Count(); i++)
        {
            oInner.Add("uCC:" + s + "," + oParameters[i]);
        }

        for(int i = 0; i < oBoundary.Count(); i++)
        {
            oInner.Add("uCC:" + s + "," + oBoundary[i]);
        }

        for(int i = 0; i < oConservation.Count(); i++)
        {
            oInner.Add("uCC:" + s + "," + oConservation[i]);
        }

        oCCFloating.Add(oInner);
        oResult.Add(oCCFloating);
    }

    return oResult;
}


// Help("Returns the Symbols of all Elasticity Coefficients.")
ArrayList RoadRunner::getElasticityCoefficientIds()
{
    ArrayList oResult;
    if (!mModel)
    {
        return oResult;
    }

    StringList reactionNames        = getReactionIds();
    StringList floatingSpeciesNames = mModelGenerator->getFloatingSpeciesConcentrationList();
    StringList boundarySpeciesNames = mModelGenerator->getBoundarySpeciesList();
    StringList conservationNames    = mModelGenerator->getConservationList();
    StringList globalParameterNames = mModelGenerator->getGlobalParameterList();

    for(int i = 0; i < reactionNames.Count(); i++)
    {
        string reac_name = reactionNames[i];
        ArrayList oCCReaction;
        oCCReaction.Add(reac_name);
        ArrayList oInner;

        for(int j = 0; j < floatingSpeciesNames.Count(); j++)
        {
            oInner.Add(Format("EE:{0},{1}", reac_name, floatingSpeciesNames[j]));
        }

        for(int j = 0; j < boundarySpeciesNames.Count(); j++)
        {
            oInner.Add(Format("EE:{0},{1}", reac_name, boundarySpeciesNames[j]));
        }

        for(int j = 0; j < globalParameterNames.Count(); j++)
        {
            oInner.Add(Format("EE:{0},{1}", reac_name, globalParameterNames[j]));
        }

        for(int j = 0; j < conservationNames.Count(); j++)
        {
            oInner.Add(Format("EE:{0},{1}", reac_name, conservationNames[j]));
        }

        oCCReaction.Add(oInner);
        oResult.Add(oCCReaction);
    }

    return oResult;
}

// Help("Returns the Symbols of all Unscaled Elasticity Coefficients.")
ArrayList RoadRunner::getUnscaledElasticityCoefficientIds()
{
    ArrayList oResult;
    if (!mModel)
    {
        return oResult;
    }

    StringList oReactions( getReactionIds() );
    StringList oFloating = mModelGenerator->getFloatingSpeciesConcentrationList();
    StringList oBoundary = mModelGenerator->getBoundarySpeciesList();
    StringList oGlobalParameters = mModelGenerator->getGlobalParameterList();
    StringList oConservation = mModelGenerator->getConservationList();

    for(int i = 0; i < oReactions.Count(); i++)
    {
        string reac_name = oReactions[i];
        ArrayList oCCReaction;
        StringList oInner;
        oCCReaction.Add(reac_name);

        for(int j = 0; j < oFloating.Count(); j++)
        {
            string variable = oFloating[j];
            oInner.Add(Format("uEE:{0},{1}", reac_name, variable));
        }

        for(int j = 0; j < oBoundary.Count(); j++)
        {
            string variable = oBoundary[j];
            oInner.Add(Format("uEE:{0},{1}", reac_name, variable));
        }

        for(int j = 0; j < oGlobalParameters.Count(); j++)
        {
            string variable = oGlobalParameters[j];
            oInner.Add(Format("uEE:{0},{1}", reac_name, variable));
        }

        for(int j = 0; j < oConservation.Count(); j++)
        {
            string variable = oConservation[j];
            oInner.Add(Format("uEE:{0},{1}", reac_name, variable));
        }

        oCCReaction.Add(oInner);
        oResult.Add(oCCReaction);
    }

    return oResult;
}

// Help("Returns the Symbols of all Floating Species Eigenvalues.")
StringList RoadRunner::getEigenValueIds()
{
    StringList result; //= new ArrayList();
    if (!mModel)
    {
        return result;
    }

    StringList floating = mModelGenerator->getFloatingSpeciesConcentrationList();

    //foreach (string s in oFloating)
    for(int i = 0; i < floating.Count(); i++)
    {
        result.Add("eigen_" + floating[i]);
    }

    return result;
}
//
// Help(
//            "Returns symbols of the currently loaded model, that can be used for steady state analysis. Format: array of arrays  { { \"groupname\", { \"item1\", \"item2\" ... } } }  or { { \"groupname\", { \"subgroup\", { \"item1\" ... } } } }."
//            )
ArrayList RoadRunner::getAvailableSteadyStateSymbols()
{
    ArrayList oResult;
    if (!mModel)
    {
    	return oResult;
    }

    oResult.Add("Floating Species", 					            getFloatingSpeciesIds() );
    oResult.Add("Boundary Species", 					            getBoundarySpeciesIds() );
    oResult.Add("Floating Species (amount)", 			            getFloatingSpeciesAmountIds() );
    oResult.Add("Boundary Species (amount)", 			            getBoundarySpeciesAmountIds() );
    oResult.Add("Global Parameters", 					            getParameterIds() );
    oResult.Add("Volumes", 							            	mModelGenerator->getCompartmentList() );
    oResult.Add("Fluxes", 							            	getReactionIds() );
    oResult.Add("Flux Control Coefficients", 			            getFluxControlCoefficientIds() );
    oResult.Add("Concentration Control Coefficients",             	getConcentrationControlCoefficientIds() );
    oResult.Add("Unscaled Concentration Control Coefficients",		getUnscaledConcentrationControlCoefficientIds());
    oResult.Add("Elasticity Coefficients", 							getElasticityCoefficientIds() );
    oResult.Add("Unscaled Elasticity Coefficients", 				getUnscaledElasticityCoefficientIds() );
    oResult.Add("Eigenvalues", 										getEigenValueIds() );

    return oResult;
}

// Help("Returns the selection list as returned by computeSteadyStateValues().")
ArrayList RoadRunner::getSteadyStateSelectionList()
{
    if (!mModel)
    {
        throw SBWApplicationException(emptyModelStr);
    }

    if (mSteadyStateSelection.size() == 0)
    {
        // default should be species only ...
        StringList floatingSpecies = getFloatingSpeciesIds();
        mSteadyStateSelection.resize(floatingSpecies.Count());
        for (int i = 0; i < floatingSpecies.Count(); i++)
        {
            TSelectionRecord aRec;
            aRec.selectionType = TSelectionType::clFloatingSpecies;
            aRec.p1 = floatingSpecies[i];
            aRec.index = i;
            mSteadyStateSelection[i] = aRec;
        }
    }

    StringList oFloating     = mModelGenerator->getFloatingSpeciesConcentrationList();
    StringList oBoundary     = mModelGenerator->getBoundarySpeciesList();
    StringList oFluxes       = mModelGenerator->getReactionIds();
    StringList oVolumes      = mModelGenerator->getCompartmentList();
    StringList oRates        = getRateOfChangeIds();
    StringList oParameters   = getParameterIds();

    ArrayList result;
    for(int i = 0; i < mSteadyStateSelection.size(); i++)
    {
        TSelectionRecord record = mSteadyStateSelection[i];
        switch (record.selectionType)
        {
            case TSelectionType::clTime:
                result.Add("time");
            break;
            case TSelectionType::clBoundaryAmount:
                result.Add(Format("[{0}]", oBoundary[record.index]));
            break;
            case TSelectionType::clBoundarySpecies:
                result.Add(oBoundary[record.index]);
            break;
            case TSelectionType::clFloatingAmount:
                result.Add("[" + (string)oFloating[record.index] + "]");
            break;
            case TSelectionType::clFloatingSpecies:
                result.Add(oFloating[record.index]);
            break;
            case TSelectionType::clVolume:
                result.Add(oVolumes[record.index]);
            break;
            case TSelectionType::clFlux:
                result.Add(oFluxes[record.index]);
            break;
            case TSelectionType::clRateOfChange:
                result.Add(oRates[record.index]);
            break;
            case TSelectionType::clParameter:
                result.Add(oParameters[record.index]);
            break;
            case TSelectionType::clEigenValue:
                result.Add("eigen_" + record.p1);
            break;
            case TSelectionType::clElasticity:
                result.Add("EE:" + record.p1 + "," + record.p2);
            break;
            case TSelectionType::clUnscaledElasticity:
                result.Add("uEE:" + record.p1 + "," + record.p2);
            break;
            case TSelectionType::clUnknown:
                result.Add(record.p1);
                break;
        }
    }
    return result ;
}

vector<TSelectionRecord> RoadRunner::getSteadyStateSelection(const StringList& newSelectionList)
{
    vector<TSelectionRecord> steadyStateSelection;
	steadyStateSelection.resize(newSelectionList.Count());
    StringList fs = mModelGenerator->getFloatingSpeciesConcentrationList();
    StringList bs = mModelGenerator->getBoundarySpeciesList();
    StringList rs = mModelGenerator->getReactionIds();
    StringList vol = mModelGenerator->getCompartmentList();
    StringList gp = mModelGenerator->getGlobalParameterList();

    for (int i = 0; i < newSelectionList.Count(); i++)
    {
        bool set = false;
        // Check for species
        for (int j = 0; j < fs.Count(); j++)
        {
            if ((string)newSelectionList[i] == (string)fs[j])
            {
                steadyStateSelection[i].index = j;
                steadyStateSelection[i].selectionType = TSelectionType::clFloatingSpecies;
                set = true;
                break;
            }

            if ((string)newSelectionList[i] == "[" + (string)fs[j] + "]")
            {
                steadyStateSelection[i].index = j;
                steadyStateSelection[i].selectionType = TSelectionType::clFloatingAmount;
                set = true;
                break;
            }

            // Check for species rate of change
            if ((string)newSelectionList[i] == (string)fs[j] + "'")
            {
                steadyStateSelection[i].index = j;
                steadyStateSelection[i].selectionType = TSelectionType::clRateOfChange;
                set = true;
                break;
            }
        }

        if (set)
        {
            continue;
        }

        // Check fgr boundary species
        for (int j = 0; j < bs.Count(); j++)
        {
            if ((string)newSelectionList[i] == (string)bs[j])
            {
                steadyStateSelection[i].index = j;
                steadyStateSelection[i].selectionType = TSelectionType::clBoundarySpecies;
                set = true;
                break;
            }
            if ((string)newSelectionList[i] == "[" + (string)bs[j] + "]")
            {
                steadyStateSelection[i].index = j;
                steadyStateSelection[i].selectionType = TSelectionType::clBoundaryAmount;
                set = true;
                break;
            }
        }

        if (set)
        {
            continue;
        }

        if ((string)newSelectionList[i] == "time")
        {
            steadyStateSelection[i].selectionType = TSelectionType::clTime;
            set = true;
        }

        for (int j = 0; j < rs.Count(); j++)
        {
            // Check for reaction rate
            if ((string)newSelectionList[i] == (string)rs[j])
            {
                steadyStateSelection[i].index = j;
                steadyStateSelection[i].selectionType = TSelectionType::clFlux;
                set = true;
                break;
            }
        }

        for (int j = 0; j < vol.Count(); j++)
        {
            // Check for volume
            if ((string)newSelectionList[i] == (string)vol[j])
            {
                steadyStateSelection[i].index = j;
                steadyStateSelection[i].selectionType = TSelectionType::clVolume;
                set = true;
                break;
            }
        }

        for (int j = 0; j < gp.Count(); j++)
        {
            // Check for volume
            if ((string)newSelectionList[i] == (string)gp[j])
            {
                steadyStateSelection[i].index = j;
                steadyStateSelection[i].selectionType = TSelectionType::clParameter;
                set = true;
                break;
            }
        }

        if (set)
        {
        	continue;
        }

        // it is another symbol
        steadyStateSelection[i].selectionType = TSelectionType::clUnknown;
        steadyStateSelection[i].p1 = (string)newSelectionList[i];
    }
    return steadyStateSelection;
}

// Help("sets the selection list as returned by computeSteadyStateValues().")
void RoadRunner::setSteadyStateSelectionList(const StringList& newSelectionList)
{
    if (!mModel)
    {
        throw SBWApplicationException(emptyModelStr);
    }

    vector<TSelectionRecord> ssSelection = getSteadyStateSelection(newSelectionList);

//    for(int i = 0; i < ssSelection.size(); i++)
//    {
//    	Log(lDebug)<<"Steady state selection: "<<ssSelection[i];
//    }
    mSteadyStateSelection = ssSelection;
}

// Help("performs steady state analysis, returning values as given by setSteadyStateSelectionList().")
vector<double> RoadRunner::computeSteadyStateValues()
{
    if (!mModel)
    {
        throw SBWApplicationException(emptyModelStr);
    }
    return computeSteadyStateValues(mSteadyStateSelection, true);
}

vector<double> RoadRunner::computeSteadyStateValues(const vector<TSelectionRecord>& selection, const bool& computeSteadyState)
{
    if (computeSteadyState)
    {
        steadyState();
    }

    vector<double> result; //= new double[oSelection.Length];
    for (int i = 0; i < selection.size(); i++)
    {
        result.push_back(computeSteadyStateValue(selection[i]));
    }
    return result;

}

// Help("performs steady state analysis, returning values as specified by the given selection list.")
//        double[] RoadRunner::computeSteadyStateValues(ArrayList oSelection)
//        {
//            if (!mModel) throw SBWApplicationException(emptyModelStr);
//            var selection = GetSteadyStateSelection(oSelection);
//            return computeSteadyStateValues(selection, true);
//        }
//
double RoadRunner::computeSteadyStateValue(const TSelectionRecord& record)
{
    if (!mModel)
    {
        throw SBWApplicationException(emptyModelStr);
    }

    if (record.selectionType == TSelectionType::clUnknown)
    {
        return computeSteadyStateValue(record.p1);
    }
    return GetValueForRecord(record);
}

// Help("Returns the value of the given steady state identifier.")
double RoadRunner::computeSteadyStateValue(const string& sId)
{
    if (!mModel)
    {
        throw SBWApplicationException(emptyModelStr);
    }

    string tmp("CC:");
    if(sId.compare(0, tmp.size(), tmp) == 0)
    {
        string sList = sId.substr(tmp.size());
        string sVariable = sList.substr(0, sList.find_first_of(","));
        string sParameter = sList.substr(sVariable.size() + 1);
        return getCC(sVariable, sParameter);
    }

    tmp = "uCC:";
    if (sId.compare(0, tmp.size(), tmp) == 0)
    {
        string sList = sId.substr(tmp.size());
        string sVariable = sList.substr(0, sList.find_first_of(","));
        string sParameter = sList.substr(sVariable.size() + 1);
        return getuCC(sVariable, sParameter);
    }

    tmp = "EE:";
    if (sId.compare(0, tmp.size(), tmp) == 0)
    {
        string sList = sId.substr(tmp.size());
        string sReaction = sList.substr(0, sList.find_first_of(","));
        string sVariable = sList.substr(sReaction.size() + 1);
        return getEE(sReaction, sVariable);
    }

    tmp = "uEE:";
    if (sId.compare(0, tmp.size(), tmp) == 0)
    {
        string sList = sId.substr(tmp.size());
        string sReaction = sList.substr(0, sList.find_first_of(","));
        string sVariable = sList.substr(sReaction.size() + 1);
        return getuEE(sReaction, sVariable);
    }
    else
    {
        tmp = "eigen_";
        if (sId.compare(0, tmp.size(), tmp) == 0)
        {
            string sSpecies = sId.substr(tmp.size());
            int nIndex;
            if (mModelGenerator->floatingSpeciesConcentrationList.find(sSpecies, nIndex))
            {
                //SBWComplex[] oComplex = SBW_CLAPACK.getEigenValues(getReducedJacobian());
                LibLA LA;

                ls::DoubleMatrix mat = getReducedJacobian();
                vector<Complex> oComplex = LA.getEigenValues(mat);

                if (oComplex.size() > nIndex)
                {
                    return oComplex[nIndex].Real;
                }
                return rr::DoubleNaN;
            }
            throw SBWApplicationException(Format("Found unknown floating species '{0}' in computeSteadyStateValue()", sSpecies));
        }
        try
        {
            return getValue(sId);
        }
        catch (Exception )
        {
            throw SBWApplicationException(Format("Found unknown symbol '{0}' in computeSteadyStateValue()", sId));
        }

    }
}

// Help("Returns the values selected with setTimeCourseSelectionList() for the current model time / timestep")
//        double[] RoadRunner::getSelectedValues()
//        {
//            if (!mModel) throw SBWApplicationException(emptyModelStr);
//
//            var result = new double[selectionList.Length];
//
//            for (int j = 0; j < selectionList.Length; j++)
//            {
//                result[j] = GetNthSelectedOutput(j, mModel->GetTime());
//            }
//            return result;
//        }
//
// Help("Returns any warnings that occured during the loading of the SBML")
//        string[] RoadRunner::getWarnings()
//        {
//            if (!mModel) throw SBWApplicationException(emptyModelStr);
//            return mModel->Warnings.ToArray();
//        }
//
// Help("When turned on, this method will cause rates, event assignments, rules and such to be multiplied " +
//              "with the compartment volume, if species are defined as initialAmounts. By default this behavior is off.")
//
//        void RoadRunner::ReMultiplyCompartments(bool bValue)
//        {
//            _ReMultiplyCompartments = bValue;
//        }
//
// Help("Performs a steady state parameter scan with the given parameters returning all elments from the selectionList: (Format: symnbol, startValue, endValue, stepSize)")
//        double[][] RoadRunner::steadyStateParameterScan(string symbol, double startValue, double endValue, double stepSize)
//        {
//            var results = new List<double[]>();
//
//            double initialValue = getValue(symbol);
//            double current = startValue;
//
//            while (current < endValue)
//            {
//                setValue(symbol, current);
//                try
//                {
//                    steadyState();
//                }
//                catch (Exception)
//                {
//                    //
//                }
//
//                var currentRow = new List<double> {current};
//                currentRow.AddRange(getSelectedValues());
//
//                results.Add(currentRow.ToArray());
//                current += stepSize;
//            }
//            setValue(symbol, initialValue);
//
//            return results.ToArray();
//        }
//
//
string RoadRunner::GetModelName()
{
    return mModelGenerator->mNOM.getModelName();
}

// Help("Returns the SBML with the current parameterset")
string RoadRunner::writeSBML()
{
    NOMSupport& NOM = mModelGenerator->mNOM;

    NOM.loadSBML(NOM.getParamPromotedSBML(mCurrentSBML));

    ModelState state(*mModel);
//    var state = new ModelState(model);

    StringList array = getFloatingSpeciesIds();
    for (int i = 0; i < array.Count(); i++)
    {
        NOM.setValue((string)array[i], state.mFloatingSpeciesConcentrations[i]);
    }

    array = getBoundarySpeciesIds();
    for (int i = 0; i < array.Count(); i++)
    {
        NOM.setValue((string)array[i], state.mBoundarySpeciesConcentrations[i]);
    }

    array = getCompartmentIds();
    for (int i = 0; i < array.Count(); i++)
    {
        NOM.setValue((string)array[i], state.mCompartmentVolumes[i]);
    }

    array = getGlobalParameterIds();
    for (int i = 0; i < min(array.Count(), state.mGlobalParameters.size()); i++)
    {
        NOM.setValue((string)array[i], state.mGlobalParameters[i]);
    }

    return NOM.getSBML();
}

// Get the number of local parameters for a given reaction
int RoadRunner::getNumberOfLocalParameters(const int& reactionId)
{
     if (!mModel)
     {
     	throw SBWApplicationException(emptyModelStr);
     }
     return getNumberOfLocalParameters(reactionId);	//Todo: this functions is calling itself ?
}

// Help("Sets the value of a global parameter by its index")
//        void RoadRunner::setLocalParameterByIndex(int reactionId, int index, double value)
//        {
//            if (!mModel) throw SBWApplicationException(emptyModelStr);
//
//            if ((reactionId >= 0) && (reactionId < mModel->getNumReactions) &&
//                (index >= 0) && (index < mModel->getNumLocalParameters(reactionId)))
//                mModel->lp[reactionId][index] = value;
//            else
//                throw SBWApplicationException(string.Format("Index in setLocalParameterByIndex out of range: [{0}]", index));
//        }
//

// Returns the value of a global parameter by its index
// ***** SHOULD WE SUPPORT LOCAL PARAMETERS? ******** (Sept 2, 2012, HMS
double RoadRunner::getLocalParameterByIndex	(const int& reactionId, const int& index)
{
    if(!mModel)
    {
       throw SBWApplicationException(emptyModelStr);
    }

    if(	reactionId >= 0 &&
    	reactionId < mModel->getNumReactions() &&
        index >= 0 &&
        index < mModel->getNumLocalParameters(reactionId))
    {
    	return -1;//mModel->lp[reactionId][index];
    }
    else
    {
     	throw SBWApplicationException(Format("Index in getLocalParameterByIndex out of range: [{0}]", index));
    }
}

// Help("Returns the value of a global parameter by its index")
//        double RoadRunner::getLocalParameterByIndex(int reactionId, int index)
//        {
//            if (!mModel)
//                throw SBWApplicationException(emptyModelStr);
//
//            if ((reactionId >= 0) && (reactionId < mModel->getNumReactions) &&
//                (index >= 0) && (index < mModel->getNumLocalParameters(reactionId)))
//                return mModel->lp[reactionId][index];
//
//            throw SBWApplicationException(String.Format("Index in getLocalParameterByIndex out of range: [{0}]", index));
//        }
//
// Help("Set the values for all global parameters in the model")
//        void RoadRunner::setLocalParameterValues(int reactionId, double[] values)
//        {
//            if (!mModel) throw SBWApplicationException(emptyModelStr);
//
//
//            if ((reactionId >= 0) && (reactionId < mModel->getNumReactions))
//                mModel->lp[reactionId] = values;
//            else
//                throw SBWApplicationException(String.Format("Index in setLocalParameterValues out of range: [{0}]", reactionId));
//        }
//
// Help("Get the values for all global parameters in the model")
//        double[] RoadRunner::getLocalParameterValues(int reactionId)
//        {
//            if (!mModel)
//                throw SBWApplicationException(emptyModelStr);
//
//            if ((reactionId >= 0) && (reactionId < mModel->getNumReactions))
//                return mModel->lp[reactionId];
//            throw SBWApplicationException(String.Format("Index in getLocalParameterValues out of range: [{0}]", reactionId));
//        }
//
// Help("Gets the list of parameter names")
//        ArrayList RoadRunner::getLocalParameterNames(int reactionId)
//        {
//            if (!mModel)
//                throw SBWApplicationException(emptyModelStr);
//
//            if ((reactionId >= 0) && (reactionId < mModel->getNumReactions))
//                return mModelGenerator->getLocalParameterList(reactionId);
//            throw (new SBWApplicationException("reaction Id out of range in call to getLocalParameterNames"));
//        }
//
// Help("Returns a list of global parameter tuples: { {parameter Name, value},...")
//        ArrayList RoadRunner::getAllLocalParameterTupleList()
//        {
//            if (!mModel)
//                throw SBWApplicationException(emptyModelStr);
//
//            var tupleList = new ArrayList();
//            for (int i = 0; i < mModelGenerator->getNumberOfReactions(); i++)
//            {
//                var tuple = new ArrayList();
//                ArrayList lpList = mModelGenerator->getLocalParameterList(i);
//                tuple.Add(i);
//                for (int j = 0; j < lpList.Count; j++)
//                {
//                    tuple.Add(lpList[j]);
//                    tuple.Add(mModel->lp[i][j]);
//                }
//                tupleList.Add(tuple);
//            }
//            return tupleList;
//        }
//
// Help("Get the number of reactions")
int RoadRunner::getNumberOfReactions()
{
    if (!mModel)
    {
        throw SBWApplicationException(emptyModelStr);
    }
    return mModel->getNumReactions();
}

// Help("Returns the rate of a reaction by its index")
double RoadRunner::getReactionRate(const int& index)
{
    if (!mModel)
    {
        throw SBWApplicationException(emptyModelStr);
    }

    if ((index >= 0) && (index < mModel->getNumReactions()))
    {
        mModel->convertToConcentrations();
        mModel->computeReactionRates(0.0, mModel->y);
        return mModel->rates[index];
    }
    else
    {
        throw SBWApplicationException(Format("Index in getReactionRate out of range: [{0}]", index));
    }
}

// Help("Returns the rate of changes of a species by its index")
double RoadRunner::getRateOfChange(const int& index)
{
    if (!mModel)
    {
        throw SBWApplicationException(emptyModelStr);
    }

    if ((index >= 0) && (index < mModel->getNumTotalVariables()))
    {
        mModel->computeAllRatesOfChange();
        return mModel->dydt[index];
    }

    throw SBWApplicationException(Format("Index in getRateOfChange out of range: [{0}]", index));
}

// Help("Returns the rates of changes given an array of new floating species concentrations")
vector<double> RoadRunner::getRatesOfChangeEx(const vector<double>& values)
{
    if (!mModel)
    {
        throw SBWApplicationException(emptyModelStr);
    }

    CopyStdVectorToCArray(values, mModel->y, *mModel->ySize);

    vector<double> temp;

    temp =  BuildModelEvalArgument();
    mModel->evalModel(0.0, temp);
    return CreateVector(mModel->dydt, *mModel->dydtSize);
}

// Help("Returns the rates of changes given an array of new floating species concentrations")
vector<double> RoadRunner::getReactionRatesEx(const vector<double>& values)
{
    if (!mModel)
    {
        throw SBWApplicationException(emptyModelStr);
    }

    mModel->computeReactionRates(0.0, CreateVector(values));
    return CreateVector(mModel->rates, *mModel->ratesSize);
}

// Help("Get the number of compartments")
int RoadRunner::getNumberOfCompartments()
{
    if (!mModel)
    {
        throw SBWApplicationException(emptyModelStr);
    }
    return mModel->getNumCompartments();
}

// Help("Sets the value of a compartment by its index")
void RoadRunner::setCompartmentByIndex(const int& index, const double& value)
{
    if (!mModel)
    {
         throw SBWApplicationException(emptyModelStr);
    }

    if ((index >= 0) && (index < mModel->getNumCompartments()))
    {
        mModel->c[index] = value;
    }
    else
    {
        throw SBWApplicationException(Format("Index in getCompartmentByIndex out of range: [{0}]", index));
    }
}

// Help("Returns the value of a compartment by its index")
double RoadRunner::getCompartmentByIndex(const int& index)
{
    if (!mModel)
    {
        throw SBWApplicationException(emptyModelStr);
    }

    if ((index >= 0) && (index < mModel->getNumCompartments()))
    {
        return mModel->c[index];
    }

    throw SBWApplicationException(Format("Index in getCompartmentByIndex out of range: [{0}]", index));
}
//
// Help("Returns the value of a compartment by its index")
//        void RoadRunner::setCompartmentVolumes(double[] values)
//        {
//            if (!mModel)
//                throw SBWApplicationException(emptyModelStr);
//            if (values.Length < mModel->getNumCompartments)
//                mModel->c = values;
//            else
//                throw (new SBWApplicationException(String.Format("Size of vector out not in range in setCompartmentValues: [{0}]", values.Length)));
//        }
//
// Help("Get the number of boundary species")
int RoadRunner::getNumberOfBoundarySpecies()
{
    if (!mModel)
    {
        throw Exception(emptyModelStr);
    }
    return mModel->getNumBoundarySpecies();
}

// Help("Sets the value of a boundary species by its index")
void RoadRunner::setBoundarySpeciesByIndex(const int& index, const double& value)
{
    if (!mModel)
    {
        throw Exception(emptyModelStr);
    }

    if ((index >= 0) && (index < mModel->getNumBoundarySpecies()))
    {
        mModel->bc[index] = value;
    }
    else
    {
        throw Exception(Format("Index in getBoundarySpeciesByIndex out of range: [{0}]", index));
    }
}

// Help("Returns the value of a boundary species by its index")
double RoadRunner::getBoundarySpeciesByIndex(const int& index)
{
    if (!mModel)
    {
        throw Exception(emptyModelStr);
    }
    if ((index >= 0) && (index < mModel->getNumBoundarySpecies()))
    {
        return mModel->bc[index];
    }
    throw Exception(Format("Index in getBoundarySpeciesByIndex out of range: [{0}]", index));
}

// Help("Returns an array of boundary species concentrations")
vector<double> RoadRunner::getBoundarySpeciesConcentrations()
{
    if (!mModel)
    {
        throw SBWApplicationException(emptyModelStr);
    }

    mModel->convertToConcentrations();
	return CreateVector(mModel->bc, *mModel->bcSize);
}

//
// Help("Returns an array of boundary species concentrations")
//        double[] RoadRunner::getBoundarySpeciesConcentrations()
//        {
//            if (!mModel)
//                throw SBWApplicationException(emptyModelStr);
//            return mModel->bc;
//        }
//

// Help("Set the concentrations for all boundary species in the model")
//        void RoadRunner::setBoundarySpeciesConcentrations(double[] values)
//        {
//            if (!mModel) throw SBWApplicationException(emptyModelStr);
//
//            mModel->bc = values;
//        }
//
// Help("Gets the list of boundary species names")
StringList RoadRunner::getBoundarySpeciesIds()
{
    if (!mModel)
    {
        throw SBWApplicationException(emptyModelStr);
    }
    return mModelGenerator->getBoundarySpeciesList();
}

// Help("Gets the list of boundary species amount names")
StringList RoadRunner::getBoundarySpeciesAmountIds()
{
    StringList result;// = new ArrayList();
    StringList list = getBoundarySpeciesIds();
//    foreach (string s in getBoundarySpeciesIds()) oResult.Add("[" + s + "]");
    for(int item = 0; item < list.Count(); item++)// (object item in floatingSpeciesNames)
    {
        result.Add(Format("[{0}]", list[item]));
    }

    return result;
}

// Help("Get the number of floating species")
int RoadRunner::getNumberOfFloatingSpecies()
{
    if (!mModel)
    {
        throw SBWApplicationException(emptyModelStr);
    }
    return mModel->getNumTotalVariables();
}

// Help("Sets the value of a floating species by its index")
void RoadRunner::setFloatingSpeciesByIndex(const int& index, const double& value)
{
    if (!mModel)
    {
        throw SBWApplicationException(emptyModelStr);
    }

    if ((index >= 0) && (index < mModel->getNumTotalVariables()))
    {
        mModel->setConcentration(index, value); // This updates the amount vector aswell
        if (!mConservedTotalChanged)
        {
            mModel->computeConservedTotals();
        }
    }
    else
    {
        throw SBWApplicationException(Format("Index in setFloatingSpeciesByIndex out of range: [{0}]", index));
    }
}

// Help("Returns the value of a floating species by its index")
double RoadRunner::getFloatingSpeciesByIndex(const int& index)
{
    if (!mModel)
    {
        throw SBWApplicationException(emptyModelStr);
    }

    if ((index >= 0) && (index < mModel->getNumTotalVariables()))
    {
        return mModel->getConcentration(index);
    }
    throw SBWApplicationException(Format("Index in getFloatingSpeciesByIndex out of range: [{0}]", index));
}

// Help("Returns an array of floating species concentrations")
vector<double> RoadRunner::getFloatingSpeciesConcentrations()
{
    if (!mModel)
    {
        throw SBWApplicationException(emptyModelStr);
    }

    mModel->convertToConcentrations();
    return CreateVector(mModel->y, *mModel->ySize);
}

// Help("returns an array of floating species initial conditions")
vector<double> RoadRunner::getFloatingSpeciesInitialConcentrations()
{
    if (!mModel)
    {
        throw SBWApplicationException(emptyModelStr);
    }
    vector<double> initYs;
    CopyCArrayToStdVector(mModel->init_y, initYs, *mModel->init_ySize);
    return initYs;
}


// Help("Set the concentrations for all floating species in the model")
void RoadRunner::setFloatingSpeciesConcentrations(const vector<double>& values)
{
    if (!mModel)
    {
        throw SBWApplicationException(emptyModelStr);
    }

    for (int i = 0; i < values.size(); i++)
    {
        mModel->setConcentration(i, values[i]);
        if ((*mModel->ySize) > i)
        {
            mModel->y[i] = values[i];
        }
    }
    mModel->convertToAmounts();
    if (!mConservedTotalChanged) mModel->computeConservedTotals();
}


// Help("Set the concentrations for all floating species in the model")
void RoadRunner::setBoundarySpeciesConcentrations(const vector<double>& values)
{
    if (!mModel)
    {
        throw SBWApplicationException(emptyModelStr);
    }

    for (int i = 0; i < values.size(); i++)
    {
        mModel->setConcentration(i, values[i]);
        if ((*mModel->bcSize) > i)
        {
            mModel->bc[i] = values[i];
        }
    }
    mModel->convertToAmounts();
}

// Help("Sets the value of a floating species by its index")
//        void RoadRunner::setFloatingSpeciesInitialConcentrationByIndex(int index, double value)
//        {
//            if (!mModel) throw SBWApplicationException(emptyModelStr);
//
//            if ((index >= 0) && (index < mModel->init_y.Length))
//            {
//                mModel->init_y[index] = value;
//                reset();
//            }
//            else
//                throw SBWApplicationException(String.Format("Index in setFloatingSpeciesInitialConcentrationByIndex out of range: [{0}]", index));
//        }
//
// Help("Sets the initial conditions for all floating species in the model")
//        void RoadRunner::setFloatingSpeciesInitialConcentrations(double[] values)
//        {
//            if (!mModel) throw SBWApplicationException(emptyModelStr);
//
//            mModel->init_y = values;
//            reset();
//        }
//
//
// This is a Level 1 method !
// Help("Returns a list of floating species names")
StringList RoadRunner::getFloatingSpeciesIds()
{
    if (!mModel)
    {
        throw SBWApplicationException(emptyModelStr);
    }

    return mModelGenerator->getFloatingSpeciesConcentrationList(); // Reordered list
}

// Help("Returns a list of floating species initial condition names")
StringList RoadRunner::getFloatingSpeciesInitialConditionIds()
{
    if (!mModel)
    {
        throw SBWApplicationException(emptyModelStr);
    }

    StringList floatingSpeciesNames = mModelGenerator->getFloatingSpeciesConcentrationList();
    StringList result;// = new ArrayList();
    for(int item = 0; item < floatingSpeciesNames.Count(); item++)// (object item in floatingSpeciesNames)
    {
        result.Add(Format("init({0})", floatingSpeciesNames[item]));
    }
    return result;
}

// Help("Returns the list of floating species amount names")
StringList RoadRunner::getFloatingSpeciesAmountIds()
{
    StringList oResult;// = new ArrayList();
    StringList list = getFloatingSpeciesIds();
//    foreach (string s in getFloatingSpeciesIds()) oResult.Add(String.Format("[{0}]", s));
    for(int i = 0; i < list.Count(); i++)
    {
        oResult.push_back(Format("[{0}]", list[i]));
    }
    return oResult;
}

// Help("Get the number of global parameters")
int RoadRunner::getNumberOfGlobalParameters()
{
    if (!mModel)
    {
        throw SBWApplicationException(emptyModelStr);
    }
    return mModelGenerator->getGlobalParameterList().Count();
}

// Help("Sets the value of a global parameter by its index")
void RoadRunner::setGlobalParameterByIndex(const int& index, const double& value)
{
    if (!mModel)
    {
        throw SBWApplicationException(emptyModelStr);
    }

    if ((index >= 0) && (index < mModel->getNumGlobalParameters() + *mModel->ctSize))
    {
        if (index >= mModel->getNumGlobalParameters())
        {
            mModel->ct[index - mModel->getNumGlobalParameters()] = value;
            mModel->updateDependentSpeciesValues(mModel->y);
            mConservedTotalChanged = true;
        }
        else
        {
            mModel->gp[index] = value;
        }
    }
    else
    {
        throw SBWApplicationException(Format("Index in getNumGlobalParameters out of range: [{0}]", index));
    }
}

// Help("Returns the value of a global parameter by its index")
double RoadRunner::getGlobalParameterByIndex(const int& index)
{
    if (!mModel)
    {
        throw SBWApplicationException(emptyModelStr);
    }

//    int sanity = (mModel->getNumGlobalParameters() + *mModel->ctSize) ;
    if ((index >= 0) && (index < (mModel->getNumGlobalParameters() + *mModel->ctSize)))
    {
        int arraySize = *mModel->gpSize + *mModel->ctSize;
        double* result = new double[ arraySize];
//        mModel->gp.CopyTo(result, 0);
        for(int i = 0; i < *mModel->gpSize; i++)
        {
            result[i] = mModel->gp[i];
        }

        int tempIndex = 0;
        for(int i = *mModel->gpSize; i < arraySize; i++)
        {
            result[i] = mModel->ct[tempIndex++];
        }

//        mModel->ct.CopyTo(result, *mModel->gpSize);
        return result[index];
    }

    throw SBWApplicationException(Format("Index in getNumGlobalParameters out of range: [{0}]", index));
}

// Help("Set the values for all global parameters in the model")
//        void RoadRunner::setGlobalParameterValues(double[] values)
//        {
//            if (!mModel) throw SBWApplicationException(emptyModelStr);
//            if (values.Length == mModel->gp.Length)
//                mModel->gp = values;
//            else
//            {
//                for (int i = 0; i < mModel->gp.Length; i++)
//                {
//                    mModel->gp[i] = values[i];
//                }
//                for (int i = 0; i < mModel->ct.Length; i++)
//                {
//                    mModel->gp[i] = values[i + mModel->gp.Length];
//                    mConservedTotalChanged = true;
//                }
//                mModel->updateDependentSpeciesValues(mModel->y);
//            }
//        }
//
// Help("Get the values for all global parameters in the model")
vector<double> RoadRunner::getGlobalParameterValues()
{
    if (!mModel)
    {
        throw SBWApplicationException(emptyModelStr);
    }

    if (*mModel->ctSize > 0)
    {
        vector<double> result; //= new double[mModel->gp.Length + mModel->ct.Length];
        result.resize(*mModel->gpSize + *mModel->ctSize);

        //mModel->gp.CopyTo(result, 0);
        CopyValues(result,mModel->gp, *mModel->gpSize, 0);

        //mModel->ct.CopyTo(result, mModel->gp.Length);
        CopyValues(result, mModel->ct, *mModel->ctSize, *mModel->gpSize);
        return result;
    }

    return CreateVector(mModel->gp, *mModel->gpSize);
}

// Help("Gets the list of parameter names")
StringList RoadRunner::getGlobalParameterIds()
{
    if (!mModel)
    {
        throw SBWApplicationException(emptyModelStr);
    }
    return mModelGenerator->getGlobalParameterList();
}
//
// Help("Returns a list of global parameter tuples: { {parameter Name, value},...")
//        ArrayList RoadRunner::getAllGlobalParameterTupleList()
//        {
//            if (!mModel)
//                throw SBWApplicationException(emptyModelStr);
//
//            var tupleList = new ArrayList();
//            ArrayList gp = mModelGenerator->getGlobalParameterList();
//            for (int i = 0; i < gp.Count; i++)
//            {
//                var tuple = new ArrayList {gp[i], mModel->gp[i]};
//                tupleList.Add(tuple);
//            }
//            return tupleList;
//        }
//
// Help("Returns a description of the module")
string RoadRunner::getDescription()
{
    return "Simulator API based on CVODE/NLEQ/C++ implementation";
}
//
//       void RoadRunner::TestChange()
//        {
//            var sbml = File.ReadAllText(@"C:\Users\fbergmann\Desktop\testModel.xml");
//            var sim = new RoadRunner();
//            sim.loadSBML(sbml);
//            sim.setmTimeStart(0);
//            sim.setTimeEnd(10);
//            sim.setmNumPoints(10);
//            var data = sim.simulate();
//            var writer = new StringWriter();
//            DumpResults(writer, data, sim.getTimeCourseSelectionList());
//            sim.changeInitialConditions(new double[] { 20, 0 });
//            sim.reset();
//            data = sim.simulate();
//            writer = new StringWriter();
//            DumpResults(writer, data, sim.getTimeCourseSelectionList());
//        }
//#endif
//    }

//---------------- MCA functions......
//    partial class RoadRunner
//    {
//        [Help("Get unscaled control coefficient with respect to a global parameter")]
double RoadRunner::getuCC(const string& variableName, const string& parameterName)
{
    try
    {
        if (mModel)
        {
            TParameterType parameterType;
            TVariableType variableType;
            double originalParameterValue;
            int variableIndex;
            int parameterIndex;
            double f1;
            double f2;

            mModel->convertToConcentrations();
            mModel->computeReactionRates(mModel->GetTime(), mModel->y);

            // Check the variable name
            if (mModelGenerator->reactionList.find(variableName, variableIndex))
            {
                variableType = TVariableType::vtFlux;
            }
            else if (mModelGenerator->floatingSpeciesConcentrationList.find(variableName, variableIndex))
            {
                variableType = TVariableType::vtSpecies;
            }
            else
            {
                throw SBWApplicationException("Unable to locate variable: [" + variableName + "]");
            }

            // Check for the parameter name
            if (mModelGenerator->globalParameterList.find(parameterName, parameterIndex))
            {
                parameterType = TParameterType::ptGlobalParameter;
                originalParameterValue = mModel->gp[parameterIndex];
            }
            else if (mModelGenerator->boundarySpeciesList.find(parameterName, parameterIndex))
            {
                parameterType = TParameterType::ptBoundaryParameter;
                originalParameterValue = mModel->bc[parameterIndex];
            }
            else if (mModelGenerator->conservationList.find(parameterName, parameterIndex))
            {
                parameterType = TParameterType::ptConservationParameter;
                originalParameterValue = mModel->ct[parameterIndex];
            }
            else throw SBWApplicationException("Unable to locate parameter: [" + parameterName + "]");

            // Get the original parameter value
            originalParameterValue = getParameterValue(parameterType, parameterIndex);

            double hstep = DiffStepSize*originalParameterValue;
            if (fabs(hstep) < 1E-12)
            {
                hstep = DiffStepSize;
            }

            try
            {
                mModel->convertToConcentrations();

                setParameterValue(parameterType, parameterIndex, originalParameterValue + hstep);
                steadyState();
                mModel->computeReactionRates(mModel->GetTime(), mModel->y);
                double fi = getVariableValue(variableType, variableIndex);

                setParameterValue(parameterType, parameterIndex, originalParameterValue + 2*hstep);
                steadyState();
                mModel->computeReactionRates(mModel->GetTime(), mModel->y);
                double fi2 = getVariableValue(variableType, variableIndex);

                setParameterValue(parameterType, parameterIndex, originalParameterValue - hstep);
                steadyState();
                mModel->computeReactionRates(mModel->GetTime(), mModel->y);
                double fd = getVariableValue(variableType, variableIndex);

                setParameterValue(parameterType, parameterIndex, originalParameterValue - 2*hstep);
                steadyState();
                mModel->computeReactionRates(mModel->GetTime(), mModel->y);
                double fd2 = getVariableValue(variableType, variableIndex);

                // Use instead the 5th order approximation double unscaledValue = (0.5/hstep)*(fi-fd);
                // The following separated lines avoid small amounts of roundoff error
                f1 = fd2 + 8*fi;
                f2 = -(8*fd + fi2);
            }
            //finally
            catch(...)
            {

            }
            {
                // What ever happens, make sure we restore the parameter level
                setParameterValue(parameterType, parameterIndex, originalParameterValue);
                steadyState();
            }
            return 1/(12*hstep)*(f1 + f2);
        }
        else throw SBWApplicationException(emptyModelStr);
    }
    catch (SBWException)
    {
        throw;
    }
    catch (const Exception& e)
    {
        throw SBWApplicationException("Unexpected error from getuCC ()", e.Message());
    }
}


//        [Help("Get scaled control coefficient with respect to a global parameter")]
double RoadRunner::getCC(const string& variableName, const string& parameterName)
{
    TVariableType variableType;
    TParameterType parameterType;
    int variableIndex;
    int parameterIndex;
    //double originalParameterValue;

    if (!mModel)
    {
        throw SBWApplicationException(emptyModelStr);
    }

    // Check the variable name
    if (mModelGenerator->reactionList.find(variableName, variableIndex))
    {
        variableType = TVariableType::vtFlux;
    }
    else if (mModelGenerator->floatingSpeciesConcentrationList.find(variableName, variableIndex))
    {
        variableType = TVariableType::vtSpecies;
    }
    else
    {
        throw SBWApplicationException("Unable to locate variable: [" + variableName + "]");
    }

    // Check for the parameter name
    if (mModelGenerator->globalParameterList.find(parameterName, parameterIndex))
    {
        parameterType = TParameterType::ptGlobalParameter;
        //originalParameterValue = mModel->gp[parameterIndex];
    }
    else if (mModelGenerator->boundarySpeciesList.find(parameterName, parameterIndex))
    {
        parameterType = TParameterType::ptBoundaryParameter;
        //originalParameterValue = mModel->bc[parameterIndex];
    }
    else if (mModelGenerator->conservationList.find(parameterName, parameterIndex))
    {
        parameterType = TParameterType::ptConservationParameter;
        //originalParameterValue = mModel->ct[parameterIndex];
    }

    else
    {
        throw SBWApplicationException("Unable to locate parameter: [" + parameterName + "]");
    }

    steadyState();

    double variableValue = getVariableValue(variableType, variableIndex);
    double parameterValue = getParameterValue(parameterType, parameterIndex);
    return getuCC(variableName, parameterName)*parameterValue/variableValue;
}

//        [Ignore]
//        // Get a single species elasticity value
//        // IMPORTANT:
//        // Assumes that the reaction rates have been precomputed at the operating point !!
double RoadRunner::getUnscaledSpeciesElasticity(int reactionId, int speciesIndex)
{
    double f1, f2, fi, fi2, fd, fd2;
    double originalParameterValue = mModel->getConcentration(speciesIndex);

    double hstep = DiffStepSize*originalParameterValue;
    if (fabs(hstep) < 1E-12)
    {
        hstep = DiffStepSize;
    }

    mModel->convertToConcentrations();
    mModel->setConcentration(speciesIndex, originalParameterValue + hstep);
    try
    {
        mModel->computeReactionRates(mModel->GetTime(), mModel->y);
        fi = mModel->rates[reactionId];

        mModel->setConcentration(speciesIndex, originalParameterValue + 2*hstep);
        mModel->computeReactionRates(mModel->GetTime(), mModel->y);
        fi2 = mModel->rates[reactionId];

        mModel->setConcentration(speciesIndex, originalParameterValue - hstep);
        mModel->computeReactionRates(mModel->GetTime(), mModel->y);
        fd = mModel->rates[reactionId];

        mModel->setConcentration(speciesIndex, originalParameterValue - 2*hstep);
        mModel->computeReactionRates(mModel->GetTime(), mModel->y);
        fd2 = mModel->rates[reactionId];

        // Use instead the 5th order approximation double unscaledElasticity = (0.5/hstep)*(fi-fd);
        // The following separated lines avoid small amounts of roundoff error
        f1 = fd2 + 8*fi;
        f2 = -(8*fd + fi2);
    }
    catch(const Exception& e)
    {
        Log(lError)<<"Something went wrong in "<<__FUNCTION__;
        Log(lError)<<"Exception "<<e.what()<< " thrown";
    }
//    finally
    {
        // What ever happens, make sure we restore the species level
        mModel->setConcentration(speciesIndex, originalParameterValue);
    }
    return 1/(12*hstep)*(f1 + f2);
}


//        [Help(
//            "Returns the elasticity of a given reaction to a given parameter. Parameters can be boundary species or global parameters"
//            )]
//        double getUnScaledElasticity(string reactionName, string parameterName)
//        {
//            if (!mModel) throw SBWApplicationException(emptyModelStr);
//            double f1, f2, fi, fi2, fd, fd2;
//            double hstep;
//
//            int reactionId = -1;
//            if (!(mModelGenerator->reactionList.find(reactionName, out reactionId)))
//                throw SBWApplicationException("Unrecognized reaction name in call to getUnScaledElasticity [" +
//                                                  reactionName + "]");
//
//            int index = -1;
//            // Find out what kind of parameter it is, species or global parmaeter
//            if (mModelGenerator->boundarySpeciesList.find(parameterName, out index))
//            {
//                double originalParameterValue = mModel->bc[index];
//                hstep = DiffStepSize*originalParameterValue;
//                if (Math.Abs(hstep) < 1E-12)
//                    hstep = DiffStepSize;
//
//                try
//                {
//                    mModel->convertToConcentrations();
//                    mModel->bc[index] = originalParameterValue + hstep;
//                    mModel->computeReactionRates(mModel->GetTime(), mModel->y);
//                    fi = mModel->rates[reactionId];
//
//                    mModel->bc[index] = originalParameterValue + 2*hstep;
//                    mModel->computeReactionRates(mModel->GetTime(), mModel->y);
//                    fi2 = mModel->rates[reactionId];
//
//                    mModel->bc[index] = originalParameterValue - hstep;
//                    mModel->computeReactionRates(mModel->GetTime(), mModel->y);
//                    fd = mModel->rates[reactionId];
//
//                    mModel->bc[index] = originalParameterValue - 2*hstep;
//                    mModel->computeReactionRates(mModel->GetTime(), mModel->y);
//                    fd2 = mModel->rates[reactionId];
//
//                    // Use instead the 5th order approximation double unscaledElasticity = (0.5/hstep)*(fi-fd);
//                    // The following separated lines avoid small amounts of roundoff error
//                    f1 = fd2 + 8*fi;
//                    f2 = -(8*fd + fi2);
//                }
//                finally
//                {
//                    mModel->bc[index] = originalParameterValue;
//                }
//            }
//            else
//            {
//                if (mModelGenerator->globalParameterList.find(parameterName, out index))
//                {
//                    double originalParameterValue = mModel->gp[index];
//                    hstep = DiffStepSize*originalParameterValue;
//                    if (Math.Abs(hstep) < 1E-12)
//                        hstep = DiffStepSize;
//
//                    try
//                    {
//                        mModel->convertToConcentrations();
//
//                        mModel->gp[index] = originalParameterValue + hstep;
//                        mModel->computeReactionRates(mModel->GetTime(), mModel->y);
//                        fi = mModel->rates[reactionId];
//
//                        mModel->gp[index] = originalParameterValue + 2*hstep;
//                        mModel->computeReactionRates(mModel->GetTime(), mModel->y);
//                        fi2 = mModel->rates[reactionId];
//
//                        mModel->gp[index] = originalParameterValue - hstep;
//                        mModel->computeReactionRates(mModel->GetTime(), mModel->y);
//                        fd = mModel->rates[reactionId];
//
//                        mModel->gp[index] = originalParameterValue - 2*hstep;
//                        mModel->computeReactionRates(mModel->GetTime(), mModel->y);
//                        fd2 = mModel->rates[reactionId];
//
//                        // Use instead the 5th order approximation double unscaledElasticity = (0.5/hstep)*(fi-fd);
//                        // The following separated lines avoid small amounts of roundoff error
//                        f1 = fd2 + 8*fi;
//                        f2 = -(8*fd + fi2);
//                    }
//                    finally
//                    {
//                        mModel->gp[index] = originalParameterValue;
//                    }
//                }
//                else if (mModelGenerator->conservationList.find(parameterName, out index))
//                {
//                    double originalParameterValue = mModel->gp[index];
//                    hstep = DiffStepSize*originalParameterValue;
//                    if (Math.Abs(hstep) < 1E-12)
//                        hstep = DiffStepSize;
//
//                    try
//                    {
//                        mModel->convertToConcentrations();
//
//                        mModel->ct[index] = originalParameterValue + hstep;
//                        mModel->computeReactionRates(mModel->GetTime(), mModel->y);
//                        fi = mModel->rates[reactionId];
//
//                        mModel->ct[index] = originalParameterValue + 2*hstep;
//                        mModel->computeReactionRates(mModel->GetTime(), mModel->y);
//                        fi2 = mModel->rates[reactionId];
//
//                        mModel->ct[index] = originalParameterValue - hstep;
//                        mModel->computeReactionRates(mModel->GetTime(), mModel->y);
//                        fd = mModel->rates[reactionId];
//
//                        mModel->ct[index] = originalParameterValue - 2*hstep;
//                        mModel->computeReactionRates(mModel->GetTime(), mModel->y);
//                        fd2 = mModel->rates[reactionId];
//
//                        // Use instead the 5th order approximation double unscaledElasticity = (0.5/hstep)*(fi-fd);
//                        // The following separated lines avoid small amounts of roundoff error
//                        f1 = fd2 + 8*fi;
//                        f2 = -(8*fd + fi2);
//                    }
//                    finally
//                    {
//                        mModel->ct[index] = originalParameterValue;
//                    }
//                }
//                else
//                    throw SBWApplicationException("Unrecognized parameter name in call to getUnScaledElasticity [" +
//                                                      parameterName + "]");
//            }
//            return 1/(12*hstep)*(f1 + f2);
//        }


//        [Help("Compute the unscaled species elasticity matrix at the current operating point")]
ls::DoubleMatrix RoadRunner::getUnscaledElasticityMatrix()
{
    ls::DoubleMatrix uElastMatrix(mModel->getNumReactions(), mModel->getNumTotalVariables());

    try
    {
        if (mModel)
        {
            mModel->convertToConcentrations();
            // Compute reaction velocities at the current operating point
            mModel->computeReactionRates(mModel->GetTime(), mModel->y);

            for (int i = 0; i < mModel->getNumReactions(); i++)
            {
                for (int j = 0; j < mModel->getNumTotalVariables(); j++)
                {
                    uElastMatrix[i][j] = getUnscaledSpeciesElasticity(i, j);
                }
            }

            return uElastMatrix;
        }
        else
        {
            throw SBWApplicationException(emptyModelStr);
        }
    }
    catch (const Exception& e)
    {
        throw SBWApplicationException("Unexpected error from unscaledElasticityMatrix()", e.Message());
    }
}

//        [Help("Compute the unscaled elasticity matrix at the current operating point")]
ls::DoubleMatrix RoadRunner::getScaledElasticityMatrix()
{
    try
    {
        if (mModel)
        {
            ls::DoubleMatrix uelast = getUnscaledElasticityMatrix();

            ls::DoubleMatrix result(uelast.RSize(), uelast.CSize());// = new double[uelast.Length][];
            mModel->convertToConcentrations();
            mModel->computeReactionRates(mModel->GetTime(), mModel->y);
            vector<double> rates;// = mModel->rates;
            if(!CopyCArrayToStdVector(mModel->rates, rates, *mModel->ratesSize))
            {
                throw SBWApplicationException("Failed to copy model->rates");
            }

            for (int i = 0; i < uelast.RSize(); i++)
            {
                // Rows are rates
                if (*mModel->ratesSize == 0 || rates[i] == 0)
                {
	                string name;
                	if(mModelGenerator && mModelGenerator->reactionList.size())
                    {
                		name = mModelGenerator->reactionList[i].name;
                    }
                    else
                    {
                    	name = "none";
                    }

                    throw SBWApplicationException("Unable to compute elasticity, reaction rate [" + name + "] set to zero");
                }

                for (int j = 0; j < uelast.CSize(); j++) // Columns are species
                {
                    result[i][j] = uelast[i][j]*mModel->getConcentration(j)/rates[i];
                }
            }
            return result;
        }
        else
        {
            throw SBWApplicationException(emptyModelStr);
        }
    }
    catch (const Exception& e)
    {
        throw SBWApplicationException("Unexpected error from scaledElasticityMatrix()", e.Message());
    }
}


//        [Help("Compute the unscaled elasticity for a given reaction and given species")]
//        double getUnscaledFloatingSpeciesElasticity(string reactionName, string speciesName)
//        {
//            try
//            {
//                if (mModel)
//                {
//                    int speciesIndex = 0;
//                    int reactionIndex = 0;
//
//                    mModel->convertToConcentrations();
//                    mModel->computeReactionRates(mModel->GetTime(), mModel->y);
//
//                    if (!mModelGenerator->floatingSpeciesConcentrationList.find(speciesName, out speciesIndex))
//                        throw SBWApplicationException(
//                            "Internal Error: unable to locate species name while computing unscaled elasticity");
//                    if (!mModelGenerator->reactionList.find(reactionName, out reactionIndex))
//                        throw SBWApplicationException(
//                            "Internal Error: unable to locate reaction name while computing unscaled elasticity");
//
//                    return getUnscaledSpeciesElasticity(reactionIndex, speciesIndex);
//                }
//                else throw SBWApplicationException(emptyModelStr);
//            }
//            catch (SBWException)
//            {
//                throw;
//            }
//            catch (const Exception& e)
//            {
//                throw SBWApplicationException("Unexpected error from scaledElasticityMatrix()", e.Message());
//            }
//        }
//
//        [Help("Compute the scaled elasticity for a given reaction and given species")]
double RoadRunner::getScaledFloatingSpeciesElasticity(const string& reactionName, const string& speciesName)
{
    try
    {
        if (!mModel)
        {
            throw SBWApplicationException(emptyModelStr);
        }
        int speciesIndex = 0;
        int reactionIndex = 0;

        mModel->convertToConcentrations();
        mModel->computeReactionRates(mModel->GetTime(), mModel->y);

        if (!mModelGenerator->floatingSpeciesConcentrationList.find(speciesName, speciesIndex))
        {
            throw SBWApplicationException("Internal Error: unable to locate species name while computing unscaled elasticity");
        }
        if (!mModelGenerator->reactionList.find(reactionName, reactionIndex))
        {
            throw SBWApplicationException("Internal Error: unable to locate reaction name while computing unscaled elasticity");
        }

        return getUnscaledSpeciesElasticity(reactionIndex, speciesIndex)*
               mModel->getConcentration(speciesIndex)/mModel->rates[reactionIndex];

    }
    catch (const Exception& e)
    {
        throw SBWApplicationException("Unexpected error from scaledElasticityMatrix()", e.Message());
    }
}


//        [Ignore]
//        // Changes a given parameter type by the given increment
//        void changeParameter(TParameterType parameterType, int reactionIndex, int parameterIndex,
//                                     double originalValue, double increment)
//        {
//            switch (parameterType)
//            {
//                case TParameterType::ptLocalParameter:
//                    mModel->lp[reactionIndex][parameterIndex] = originalValue + increment;
//                    break;
//                case TParameterType::ptGlobalParameter:
//                    mModel->gp[parameterIndex] = originalValue + increment;
//                    break;
//                case TParameterType::ptBoundaryParameter:
//                    mModel->bc[parameterIndex] = originalValue + increment;
//                    break;
//                case TParameterType::ptConservationParameter:
//                    mModel->ct[parameterIndex] = originalValue + increment;
//                    break;
//            }
//        }
//
//
//        [Help("Returns the unscaled elasticity for a named reaction with respect to a named parameter (local or global)"
//            )]
//        double getUnscaledParameterElasticity(string reactionName, string parameterName)
//        {
//            int reactionIndex;
//            int parameterIndex;
//            double originalParameterValue;
//            TParameterType parameterType;
//
//            if (!mModel) throw SBWApplicationException(emptyModelStr);
//            mModel->convertToConcentrations();
//            mModel->computeReactionRates(mModel->GetTime(), mModel->y);
//
//            if (!mModelGenerator->reactionList.find(reactionName, out reactionIndex))
//                throw SBWApplicationException(
//                    "Internal Error: unable to locate reaction name while computing unscaled elasticity");
//
//            // Look for the parameter name, check local parameters first, then global
//            if (mModelGenerator->localParameterList[reactionIndex].find(reactionName, parameterName,
//                                                                               out parameterIndex))
//                parameterType = TParameterType::ptLocalParameter;
//            else if (mModelGenerator->globalParameterList.find(parameterName, out parameterIndex))
//                parameterType = TParameterType::ptGlobalParameter;
//            else if (mModelGenerator->boundarySpeciesList.find(parameterName, out parameterIndex))
//                parameterType = TParameterType::ptBoundaryParameter;
//            else if (mModelGenerator->conservationList.find(parameterName, out parameterIndex))
//                parameterType = TParameterType::ptConservationParameter;
//            else
//                return 0.0;
//
//            double f1, f2, fi, fi2, fd, fd2;
//            originalParameterValue = 0.0;
//            switch (parameterType)
//            {
//                case TParameterType::ptLocalParameter:
//                    originalParameterValue = mModel->lp[reactionIndex][parameterIndex];
//                    break;
//                case TParameterType::ptGlobalParameter:
//                    originalParameterValue = mModel->gp[parameterIndex];
//                    break;
//                case TParameterType::ptBoundaryParameter:
//                    originalParameterValue = mModel->bc[parameterIndex];
//                    break;
//                case TParameterType::ptConservationParameter:
//                    originalParameterValue = mModel->ct[parameterIndex];
//                    break;
//            }
//
//            double hstep = DiffStepSize*originalParameterValue;
//            if (Math.Abs(hstep) < 1E-12)
//                hstep = DiffStepSize;
//
//            try
//            {
//                changeParameter(parameterType, reactionIndex, parameterIndex, originalParameterValue, hstep);
//                mModel->convertToConcentrations();
//                mModel->computeReactionRates(mModel->GetTime(), mModel->y);
//                fi = mModel->rates[reactionIndex];
//
//                changeParameter(parameterType, reactionIndex, parameterIndex, originalParameterValue, 2*hstep);
//                mModel->computeReactionRates(mModel->GetTime(), mModel->y);
//                fi2 = mModel->rates[reactionIndex];
//
//                changeParameter(parameterType, reactionIndex, parameterIndex, originalParameterValue, -hstep);
//                mModel->computeReactionRates(mModel->GetTime(), mModel->y);
//                fd = mModel->rates[reactionIndex];
//
//                changeParameter(parameterType, reactionIndex, parameterIndex, originalParameterValue, -2*hstep);
//                mModel->computeReactionRates(mModel->GetTime(), mModel->y);
//                fd2 = mModel->rates[reactionIndex];
//
//                // Use instead the 5th order approximation double unscaledElasticity = (0.5/hstep)*(fi-fd);
//                // The following separated lines avoid small amounts of roundoff error
//                f1 = fd2 + 8*fi;
//                f2 = -(8*fd + fi2);
//            }
//            finally
//            {
//                // What ever happens, make sure we restore the species level
//                switch (parameterType)
//                {
//                    case TParameterType::ptLocalParameter:
//                        mModel->lp[reactionIndex][parameterIndex] = originalParameterValue;
//                        break;
//                    case TParameterType::ptGlobalParameter:
//                        mModel->gp[parameterIndex] = originalParameterValue;
//                        break;
//                    case TParameterType::ptBoundaryParameter:
//                        mModel->bc[parameterIndex] = originalParameterValue;
//                        break;
//                    case TParameterType::ptConservationParameter:
//                        mModel->ct[parameterIndex] = originalParameterValue;
//                        break;
//                }
//            }
//            return 1/(12*hstep)*(f1 + f2);
//        }
//
//
// Use the formula: ucc = -L Jac^-1 Nr
// [Help("Compute the matrix of unscaled concentration control coefficients")]
ls::DoubleMatrix RoadRunner::getUnscaledConcentrationControlCoefficientMatrix()
{
	try
	{
		if (!mModel)
		{
            throw SBWApplicationException(emptyModelStr);
        }

        setTimeStart(0.0);
        setTimeEnd(50.0);
        setNumPoints(2);
        simulate(); //This will crash, because numpoints == 1, not anymore, numPoints = 2 if numPoints <= 1
        if (steadyState() > STEADYSTATE_THRESHOLD)
        {
            if (steadyState() > 1E-2)
            {
                throw SBWApplicationException("Unable to locate steady state during control coefficient computation");
            }
        }

        // Compute the Jacobian first
        DoubleMatrix uelast     = getUnscaledElasticityMatrix();
        DoubleMatrix *Nr         = getNrMatrix();
        DoubleMatrix T1 = mult(*Nr, uelast);
        DoubleMatrix *LinkMatrix = getLinkMatrix();
        DoubleMatrix Jac = mult(T1, *LinkMatrix);

        // Compute -Jac
        DoubleMatrix T2 = Jac * (-1.0);

        ComplexMatrix temp(T2); //Get a complex matrix from a double one. Imag part is zero
        ComplexMatrix Inv = GetInverse(temp);

        // &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
        // Sauro: mult which takes complex matrix need to be implemented
        DoubleMatrix T3 = mult(Inv, *Nr); // Compute ( - Jac)^-1 . Nr

        // Finally include the dependent set as well.
        DoubleMatrix T4 = mult(*LinkMatrix, T3); // Compute L (iwI - Jac)^-1 . Nr
		return T4;
    }
	catch (const Exception& e)
	{
		throw SBWApplicationException("Unexpected error from getUnscaledConcentrationControlCoefficientMatrix()", e.Message());
	}
}

//This just creates a copy?? remove and use = instead!?
ls::ComplexMatrix RoadRunner::ConvertComplex(ls::ComplexMatrix oMatrix)
{
	ComplexMatrix oResult;
	oResult.resize (oMatrix.RSize(), oMatrix.CSize()); //  = Complex[oMatrix.Length][];
	for (int i = 0; i < oMatrix.RSize(); i++)
	{
		for (int j = 0; j < oMatrix.CSize(); j++)
		{
			oResult[i][j].Real = oMatrix[i][j].Real;
			oResult[i][j].Imag = oMatrix[i][j].Imag;
		}
	}
	return oResult;
}

// [Help("Compute the matrix of scaled concentration control coefficients")]
ls::DoubleMatrix RoadRunner::getScaledConcentrationControlCoefficientMatrix()
{
	try
	{
		if (mModel)
		{
			DoubleMatrix ucc = getUnscaledConcentrationControlCoefficientMatrix();

			if (ucc.size() > 0 )
			{
				mModel->convertToConcentrations();
				mModel->computeReactionRates(mModel->GetTime(), mModel->y);
				for (int i = 0; i < ucc.RSize(); i++)
                {
					for (int j = 0; j < ucc.CSize(); j++)
					{
                    	if(mModel->getConcentration(i) != 0.0)
                        {
							ucc[i][j] = ucc[i][j]*mModel->rates[j]/mModel->getConcentration(i);
                        }
                        else
                        {
                        	throw(Exception("Dividing with zero"));
                        }
					}
                }
			}
			return ucc;
		}
		else
        {
        	throw SBWApplicationException(emptyModelStr);
        }
	}
	catch (const Exception& e)
	{
		throw SBWApplicationException("Unexpected error from getScaledConcentrationControlCoefficientMatrix()", e.Message());
	}
}


			//// Create an identity matrix of size uee.RSize
			//DoubleMatrix T2 (uee.RSize(), uee.RSize());
			//for(int i=0; i<T2.RSize(); i++)
   //             for(int j=0; j<T2.CSize(); j++) {
   //                if (i == j)
   //                  T2[j][j] = 1;
   //                else
   //                  T2[i][j] = 0;
   //             }


// Use the formula: ucc = elast CS + I
// [Help("Compute the matrix of unscaled flux control coefficients")]
ls::DoubleMatrix RoadRunner::getUnscaledFluxControlCoefficientMatrix()
{
	try
	{
		if (mModel)
		{
			DoubleMatrix ucc = getUnscaledConcentrationControlCoefficientMatrix();
			DoubleMatrix uee = getUnscaledElasticityMatrix();

			DoubleMatrix T1 = mult(uee, ucc);
			
			// Add an identity matrix I to T1, that is add a 1 to every diagonal of T1
			for (int i=0; i<T1.RSize(); i++)
				T1[i][i] = T1[i][i] + 1;
			return T1;//Matrix.convertToDouble(T1);
		}
		else throw SBWApplicationException(emptyModelStr);
	}
	catch (SBWException)
	{
		throw;
	}
	catch (const Exception& e)
	{
		throw SBWApplicationException("Unexpected error from getUnscaledFluxControlCoefficientMatrix()", e.Message());
	}
}

// [Help("Compute the matrix of scaled flux control coefficients")]
ls::DoubleMatrix RoadRunner::getScaledFluxControlCoefficientMatrix()
{
	try
	{
		if (!mModel)
		{
        	throw SBWApplicationException(emptyModelStr);
        }

        DoubleMatrix ufcc = getUnscaledFluxControlCoefficientMatrix();

        if (ufcc.RSize() > 0)
        {
            mModel->convertToConcentrations();
            mModel->computeReactionRates(mModel->GetTime(), mModel->y);
            for (int i = 0; i < ufcc.RSize(); i++)
            {
                for (int j = 0; j < ufcc.CSize(); j++)
                {
                    if(mModel->rates[j] !=0)
                    {
                    	ufcc[i][j] = ufcc[i][j]*mModel->rates[i]/mModel->rates[j];
                    }
                	else
                    {
                    	throw(Exception("Dividing with zero"));
                   	}
                }
            }
        }
        return ufcc;
    }
	catch (const Exception& e)
	{
		throw SBWApplicationException("Unexpected error from getScaledFluxControlCoefficientMatrix()", e.Message());
	}
}

//
//        // ----------------------------------------------------------------------------------------------
//
//
//        [Help(
//            "Compute the value for a particular unscaled concentration control coefficients with respect to a local parameter"
//            )]
//        double getUnscaledConcentrationControlCoefficient(string speciesName, string localReactionName,
//                                                                 string parameterName)
//        {
//            int parameterIndex;
//            int reactionIndex;
//            int speciesIndex;
//            double f1;
//            double f2;
//
//            try
//            {
//                if (mModel)
//                {
//                    mModel->convertToConcentrations();
//                    mModel->computeReactionRates(mModel->GetTime(), mModel->y);
//
//                    if (!mModelGenerator->reactionList.find(localReactionName, out reactionIndex))
//                        throw SBWApplicationException(
//                            "Internal Error: unable to locate reaction name while computing unscaled control coefficient");
//
//                    if (!mModelGenerator->floatingSpeciesConcentrationList.find(speciesName, out speciesIndex))
//                        throw SBWApplicationException(
//                            "Internal Error: unable to locate species name while computing unscaled control coefficient");
//
//                    // Look for the parameter name
//                    if (mModelGenerator->localParameterList[reactionIndex].find(parameterName,
//                                                                                       out parameterIndex))
//                    {
//                        double originalParameterValue = mModel->lp[reactionIndex][parameterIndex];
//                        double hstep = DiffStepSize*originalParameterValue;
//                        if (Math.Abs(hstep) < 1E-12)
//                            hstep = DiffStepSize;
//
//                        try
//                        {
//                            mModel->convertToConcentrations();
//                            mModel->lp[reactionIndex][parameterIndex] = originalParameterValue + hstep;
//                            mModel->computeReactionRates(mModel->GetTime(), mModel->y);
//                            double fi = mModel->getConcentration(speciesIndex);
//
//                            mModel->lp[reactionIndex][parameterIndex] = originalParameterValue + 2*hstep;
//                            mModel->computeReactionRates(mModel->GetTime(), mModel->y);
//                            double fi2 = mModel->getConcentration(speciesIndex);
//
//                            mModel->lp[reactionIndex][parameterIndex] = originalParameterValue - hstep;
//                            mModel->computeReactionRates(mModel->GetTime(), mModel->y);
//                            double fd = mModel->getConcentration(speciesIndex);
//
//                            mModel->lp[reactionIndex][parameterIndex] = originalParameterValue - 2*hstep;
//                            mModel->computeReactionRates(mModel->GetTime(), mModel->y);
//                            double fd2 = mModel->getConcentration(speciesIndex);
//
//                            // Use instead the 5th order approximation double unscaledElasticity = (0.5/hstep)*(fi-fd);
//                            // The following separated lines avoid small amounts of roundoff error
//                            f1 = fd2 + 8*fi;
//                            f2 = -(8*fd + fi2);
//                        }
//                        finally
//                        {
//                            // What ever happens, make sure we restore the species level
//                            mModel->lp[reactionIndex][parameterIndex] = originalParameterValue;
//                        }
//                        return 1/(12*hstep)*(f1 + f2);
//                    }
//                    else
//                        throw SBWApplicationException("Unable to locate local parameter [" + parameterName +
//                                                          "] in reaction [" + localReactionName + "]");
//                }
//                else throw SBWApplicationException(emptyModelStr);
//            }
//            catch (SBWException)
//            {
//                throw;
//            }
//            catch (const Exception& e)
//            {
//                throw SBWApplicationException("Unexpected error from getScaledFluxControlCoefficientMatrix()",
//                                                  e.Message());
//            }
//        }
//
//
//        [Help(
//            "Compute the value for a particular scaled concentration control coefficients with respect to a local parameter"
//            )]
//        double getScaledConcentrationControlCoefficient(string speciesName, string localReactionName,
//                                                               string parameterName)
//        {
//            int localReactionIndex;
//            int parameterIndex;
//            int speciesIndex;
//
//            try
//            {
//                if (mModel)
//                {
//                    double ucc = getUnscaledConcentrationControlCoefficient(speciesName, localReactionName,
//                                                                            parameterName);
//
//                    mModel->convertToConcentrations();
//                    mModel->computeReactionRates(mModel->GetTime(), mModel->y);
//
//                    mModelGenerator->reactionList.find(localReactionName, out localReactionIndex);
//                    mModelGenerator->floatingSpeciesConcentrationList.find(localReactionName, out speciesIndex);
//                    mModelGenerator->localParameterList[localReactionIndex].find(parameterName,
//                                                                                        out parameterIndex);
//
//                    return ucc*mModel->lp[localReactionIndex][parameterIndex]/mModel->getConcentration(speciesIndex);
//                }
//                else throw SBWApplicationException(emptyModelStr);
//            }
//            catch (SBWException)
//            {
//                throw;
//            }
//            catch (const Exception& e)
//            {
//                throw SBWApplicationException("Unexpected error from getScaledFluxControlCoefficientMatrix()",
//                                                  e.Message());
//            }
//        }
//
//
//        [Help(
//            "Compute the value for a particular concentration control coefficient, permitted parameters include global parameters, boundary conditions and conservation totals"
//            )]
//        double getUnscaledConcentrationControlCoefficient(string speciesName, string parameterName)
//        {
//            int speciesIndex;
//            int parameterIndex;
//            TParameterType parameterType;
//            double originalParameterValue;
//            double f1;
//            double f2;
//
//            try
//            {
//                if (mModel)
//                {
//                    mModel->convertToConcentrations();
//                    mModel->computeReactionRates(mModel->GetTime(), mModel->y);
//
//                    if (!mModelGenerator->floatingSpeciesConcentrationList.find(speciesName, out speciesIndex))
//                        throw SBWApplicationException(
//                            "Internal Error: unable to locate species name while computing unscaled control coefficient");
//
//                    if (mModelGenerator->globalParameterList.find(parameterName, out parameterIndex))
//                    {
//                        parameterType = TParameterType::ptGlobalParameter;
//                        originalParameterValue = mModel->gp[parameterIndex];
//                    }
//                    else if (mModelGenerator->boundarySpeciesList.find(parameterName, out parameterIndex))
//                    {
//                        parameterType = TParameterType::ptBoundaryParameter;
//                        originalParameterValue = mModel->bc[parameterIndex];
//                    }
//                    else if (mModelGenerator->conservationList.find(parameterName, out parameterIndex))
//                    {
//                        parameterType = TParameterType::ptConservationParameter;
//                        originalParameterValue = mModel->ct[parameterIndex];
//                    }
//                    else throw SBWApplicationException("Unable to locate parameter: [" + parameterName + "]");
//
//                    double hstep = DiffStepSize*originalParameterValue;
//                    if (Math.Abs(hstep) < 1E-12)
//                        hstep = DiffStepSize;
//
//                    try
//                    {
//                        setParameterValue(parameterType, parameterIndex, originalParameterValue + hstep);
//                        steadyState();
//                        mModel->convertToConcentrations();
//                        mModel->computeReactionRates(mModel->GetTime(), mModel->y);
//                        double fi = mModel->getConcentration(speciesIndex);
//
//                        setParameterValue(parameterType, parameterIndex, originalParameterValue + 2*hstep);
//                        steadyState();
//                        mModel->computeReactionRates(mModel->GetTime(), mModel->y);
//                        double fi2 = mModel->getConcentration(speciesIndex);
//
//                        setParameterValue(parameterType, parameterIndex, originalParameterValue - hstep);
//                        steadyState();
//                        mModel->computeReactionRates(mModel->GetTime(), mModel->y);
//                        double fd = mModel->getConcentration(speciesIndex);
//
//                        setParameterValue(parameterType, parameterIndex, originalParameterValue - 2*hstep);
//                        steadyState();
//                        mModel->computeReactionRates(mModel->GetTime(), mModel->y);
//                        double fd2 = mModel->getConcentration(speciesIndex);
//
//                        // Use instead the 5th order approximation double unscaledElasticity = (0.5/hstep)*(fi-fd);
//                        // The following separated lines avoid small amounts of roundoff error
//                        f1 = fd2 + 8*fi;
//                        f2 = -(8*fd + fi2);
//                    }
//                    finally
//                    {
//                        // What ever happens, make sure we restore the species level
//                        setParameterValue(parameterType, parameterIndex, originalParameterValue);
//                        steadyState();
//                    }
//                    return 1/(12*hstep)*(f1 + f2);
//                }
//                else throw SBWApplicationException(emptyModelStr);
//            }
//            catch (SBWException)
//            {
//                throw;
//            }
//            catch (const Exception& e)
//            {
//                throw SBWApplicationException("Unexpected error from getScaledFluxControlCoefficientMatrix()",
//                                                  e.Message());
//            }
//        }
//
//
//        [Help(
//            "Compute the value for a particular scaled concentration control coefficients with respect to a global or boundary species parameter"
//            )]
//        double getScaledConcentrationControlCoefficient(string speciesName, string parameterName)
//        {
//            int parameterIndex;
//            int speciesIndex;
//
//            try
//            {
//                if (mModel)
//                {
//                    double ucc = getUnscaledConcentrationControlCoefficient(speciesName, parameterName);
//
//                    mModel->convertToConcentrations();
//                    mModel->computeReactionRates(mModel->GetTime(), mModel->y);
//
//                    mModelGenerator->floatingSpeciesConcentrationList.find(speciesName, out speciesIndex);
//                    if (mModelGenerator->globalParameterList.find(parameterName, out parameterIndex))
//                        return ucc*mModel->gp[parameterIndex]/mModel->getConcentration(speciesIndex);
//                    else if (mModelGenerator->boundarySpeciesList.find(parameterName, out parameterIndex))
//                        return ucc*mModel->bc[parameterIndex]/mModel->getConcentration(speciesIndex);
//                    else if (mModelGenerator->conservationList.find(parameterName, out parameterIndex))
//                        return ucc*mModel->ct[parameterIndex]/mModel->getConcentration(speciesIndex);
//                    return 0.0;
//                }
//                else throw SBWApplicationException(emptyModelStr);
//            }
//            catch (SBWException)
//            {
//                throw;
//            }
//            catch (const Exception& e)
//            {
//                throw SBWApplicationException("Unexpected error from getScaledFluxControlCoefficientMatrix()",
//                                                  e.Message());
//            }
//        }
//
//
//        // ----------------------------------------------------------------------------------------------
//
//
//        [Help("Compute the value for a particular unscaled flux control coefficients with respect to a local parameter")
//        ]
//        double getUnscaledFluxControlCoefficient(string fluxName, string localReactionName, string parameterName)
//        {
//            int parameterIndex;
//            int localReactionIndex;
//            int fluxIndex;
//            double f1;
//            double f2;
//
//            try
//            {
//                if (mModel)
//                {
//                    mModel->convertToConcentrations();
//                    mModel->computeReactionRates(mModel->GetTime(), mModel->y);
//
//                    if (!mModelGenerator->reactionList.find(localReactionName, out localReactionIndex))
//                        throw SBWApplicationException(
//                            "Internal Error: unable to locate reaction name while computing unscaled control coefficient");
//
//                    if (!mModelGenerator->reactionList.find(fluxName, out fluxIndex))
//                        throw SBWApplicationException(
//                            "Internal Error: unable to locate reaction name while computing unscaled control coefficient");
//
//                    // Look for the parameter name
//                    if (mModelGenerator->localParameterList[localReactionIndex].find(parameterName,
//                                                                                            out parameterIndex))
//                    {
//                        double originalParameterValue = mModel->lp[localReactionIndex][parameterIndex];
//                        double hstep = DiffStepSize*originalParameterValue;
//                        if (Math.Abs(hstep) < 1E-12)
//                            hstep = DiffStepSize;
//
//                        try
//                        {
//                            mModel->convertToConcentrations();
//                            mModel->lp[localReactionIndex][parameterIndex] = originalParameterValue + hstep;
//                            steadyState();
//                            mModel->computeReactionRates(mModel->GetTime(), mModel->y);
//                            double fi = mModel->rates[fluxIndex];
//
//                            mModel->lp[localReactionIndex][parameterIndex] = originalParameterValue + 2*hstep;
//                            steadyState();
//                            mModel->computeReactionRates(mModel->GetTime(), mModel->y);
//                            double fi2 = mModel->rates[fluxIndex];
//
//                            mModel->lp[localReactionIndex][parameterIndex] = originalParameterValue - hstep;
//                            steadyState();
//                            mModel->computeReactionRates(mModel->GetTime(), mModel->y);
//                            double fd = mModel->rates[fluxIndex];
//
//                            mModel->lp[localReactionIndex][parameterIndex] = originalParameterValue - 2*hstep;
//                            steadyState();
//                            mModel->computeReactionRates(mModel->GetTime(), mModel->y);
//                            double fd2 = mModel->rates[fluxIndex];
//
//                            // Use instead the 5th order approximation double unscaledElasticity = (0.5/hstep)*(fi-fd);
//                            // The following separated lines avoid small amounts of roundoff error
//                            f1 = fd2 + 8*fi;
//                            f2 = -(8*fd + fi2);
//                        }
//                        finally
//                        {
//                            // What ever happens, make sure we restore the species level
//                            mModel->lp[localReactionIndex][parameterIndex] = originalParameterValue;
//                            steadyState();
//                        }
//                        return 1/(12*hstep)*(f1 + f2);
//                    }
//                    else
//                        throw SBWApplicationException("Unable to locate local parameter [" + parameterName +
//                                                          "] in reaction [" + localReactionName + "]");
//                }
//                else throw SBWApplicationException(emptyModelStr);
//            }
//            catch (SBWException)
//            {
//                throw;
//            }
//            catch (const Exception& e)
//            {
//                throw SBWApplicationException("Unexpected error from getScaledFluxControlCoefficientMatrix()",
//                                                  e.Message());
//            }
//        }
//
// -------------------------------------------------------------------------------
//
//        void RoadRunner::PrintTout(double start, double end, int mNumPoints)
//        {
//            double hstep = (end - start) / (mNumPoints - 1);
//            Debug.WriteLine("Using step " + hstep);
//            double tout = start;
//            for (int i = 1; i < mNumPoints; i++)
//            {
//                tout = start + i*hstep;
//                Debug.WriteLine(tout.ToString("G17"));
//            }
//        }

//        //void RoadRunner::TestDirectory(string directory, bool testSubDirs)
//        //{
//        //    //TestDirectory(directory, testSubDirs, "*sbml-l3v1.xml");
//        //    TestDirectory(directory, testSubDirs, "*.xml");
//        //}
//
//    //    void RoadRunner::TestDirectory(string directory, bool testSubDirs, string pattern)
//    //{
//    //        var files = Directory.GetFiles(directory, pattern,
//    //            (testSubDirs ? SearchOption.AllDirectories : SearchOption.TopDirectoryOnly));
//    //        foreach (var item in files)
//    //        {
//    //            try
//    //            {
//    //                var rr = new RoadRunner();
//    //                rr.setTolerances(1e-6, 1e-3);
//    //                rr.loadSBMLFromFile(item);
//    //                rr.simulateEx(0, 10, 1000);
//    //                Debug.WriteLine(string.Format("File: {0} passed", Path.GetFileName(item)));
//    //            }
//    //            catch (Exception ex)
//    //            {
//    //                Debug.WriteLine(string.Format("File: {0} failed: ", Path.GetFileName(item), ex.Message));
//    //            }
//    //        }
//    //    }
// Help("Returns the initially loaded model as SBML")
string RoadRunner::getSBML()
{
    return mCurrentSBML;
}
//
// Help("get the currently set time start")
//        double RoadRunner::getmTimeStart()
//        {
//            return mTimeStart;
//        }
//
// Help("get the currently set time end")
//        double RoadRunner::getTimeEnd()
//        {
//            return mTimeEnd;
//        }
//
// Help("get the currently set number of points")
//        int RoadRunner::getmNumPoints()
//        {
//            return mNumPoints;
//        }
//
// Help("Set the time start for the simulation")
void RoadRunner::setTimeStart(const double& startTime)
{
    if (!mModel)
    {
        throw SBWApplicationException(emptyModelStr);
    }

    if (startTime < 0)
    {
        throw SBWApplicationException("Time Start most be greater than zero");
    }

    mTimeStart = startTime;
}

//Help("Set the time end for the simulation")
void RoadRunner::setTimeEnd(const double& endTime)
{
    if (!mModel)
    {
        throw SBWApplicationException(emptyModelStr);
    }

    if (endTime <= 0)
    {
        throw SBWApplicationException("Time End most be greater than zero");
    }

    mTimeEnd = endTime;
}

//Help("Set the number of points to generate during the simulation")
void RoadRunner::setNumPoints(const int& pts)
{
    if(!mModel)
    {
        throw SBWApplicationException(emptyModelStr);
    }

    mNumPoints = (pts <= 0) ? 2 : pts;
}


// [Help("get the currently set time start")]
double RoadRunner::getTimeStart()
{
    return mTimeStart;
}

// [Help("get the currently set time end")]
double RoadRunner::getTimeEnd()
{
   return mTimeEnd;
}

// [Help("get the currently set number of points")]
int RoadRunner::getNumPoints()
{
   return mNumPoints;
}


// Help(
//            "Change the initial conditions to another concentration vector (changes only initial conditions for floating Species)")
void RoadRunner::changeInitialConditions(const vector<double>& ic)
{
    if (!mModel)
    {
        throw SBWApplicationException(emptyModelStr);
    }

    for (int i = 0; i < ic.size(); i++)
    {
        mModel->setConcentration(i, ic[i]);
        if ((*mModel->init_ySize) > i)
        {
            mModel->init_y[i] = ic[i];
        }
    }
    mModel->convertToAmounts();
    mModel->computeConservedTotals();
}

// Help("Returns the current vector of reactions rates")
vector<double> RoadRunner::getReactionRates()
{
    if (!mModel)
    {
        throw SBWApplicationException(emptyModelStr);
    }
    mModel->convertToConcentrations();
    mModel->computeReactionRates(0.0, mModel->y);

    vector<double> _rates;
    CopyCArrayToStdVector(mModel->rates, _rates, *mModel->ratesSize);
    return _rates;
}

// Help("Returns the current vector of rates of change")
vector<double> RoadRunner::getRatesOfChange()
{
    if (!mModel)
    {
        throw SBWApplicationException(emptyModelStr);
    }

    mModel->computeAllRatesOfChange();
    vector<double> result;
    CopyCArrayToStdVector(mModel->dydt, result, *mModel->dydtSize);

    return result;
}

// Help("Returns a list of reaction names")
StringList RoadRunner::getReactionIds()
{
    if (!mModel)
    {
        throw SBWApplicationException(emptyModelStr);
    }

    return mModelGenerator->getReactionIds();
}

// ---------------------------------------------------------------------
// Start of Level 2 API Methods
// ---------------------------------------------------------------------
// Help("Get Simulator Capabilities")
string RoadRunner::getCapabilities()
{
    CapsSupport current = CapsSupport(this);
    return current.AsXMLString();
}
//
//        void RoadRunner::setTolerances(double aTol, double rTol)
//        {
//            CvodeInterface.relTol = rTol;
//            CvodeInterface.absTol = aTol;
//        }
//
//        void RoadRunner::setTolerances(double aTol, double rTol, int maxSteps)
//        {
//            setTolerances(aTol, rTol);
//            CvodeInterface.MaxNumSteps = maxSteps;
//        }
//
//        void RoadRunner::CorrectMaxStep()
//        {
//            double maxStep = (mTimeEnd - mTimeStart) / (mNumPoints);
//            maxStep = Math.Min(CvodeInterface.MaxStep, maxStep);
//            CvodeInterface.MaxStep = maxStep;
//        }
//

// Help("Set Simulator Capabilites")
void RoadRunner::setCapabilities(const string& capsStr)
{
//    var cs = new CapsSupport(capsStr);
//    cs.Apply();
//
//    //CorrectMaxStep();
//
//    if (mModel)
//    {
//        if(!mCVode)
//        {
//            mCVode = new CvodeInterface(model);
//        }
//        for (int i = 0; i < model.getNumIndependentVariables; i++)
//        {
//            mCVode->setAbsTolerance(i, CvodeInterface->absTol);
//        }
//        mCVode->reStart(0.0, model);
//    }
//
//    if (cs.HasSection("integration") && cs["integration"].HasCapability("usekinsol"))
//    {
//        CapsSupport.Capability cap = cs["integration", "usekinsol"];
//        UseKinsol = cap.IntValue;
//    }
}

// Help("Sets the value of the given species or global parameter to the given value (not of local parameters)")
bool RoadRunner::setValue(const string& sId, const double& dValue)
{
    if (!mModel)
    {
        Log(lError)<<emptyModelStr;
        return false;
    }

    int nIndex = -1;
    if (mModelGenerator->globalParameterList.find(sId, nIndex))
    {
        mModel->gp[nIndex] = dValue;
        return true;
    }

    if (mModelGenerator->boundarySpeciesList.find(sId, nIndex))
    {
        mModel->bc[nIndex] = dValue;
        return true;
    }

    if (mModelGenerator->compartmentList.find(sId, nIndex))
    {
        mModel->c[nIndex] = dValue;
        return true;
    }

    if (mModelGenerator->floatingSpeciesConcentrationList.find(sId, nIndex))
    {
        mModel->setConcentration(nIndex, dValue);
        mModel->convertToAmounts();
        if (!mConservedTotalChanged)
        {
            mModel->computeConservedTotals();
        }
        return true;
    }

    if (mModelGenerator->conservationList.find(sId, nIndex))
    {
        mModel->ct[nIndex] = dValue;
        mModel->updateDependentSpeciesValues(mModel->y);
        mConservedTotalChanged = true;
        return true;
    }

    StringList initialConditions;
    initialConditions = getFloatingSpeciesInitialConditionIds();

    if (initialConditions.Contains(sId))
    {
        int index = initialConditions.IndexOf(sId);
        mModel->init_y[index] = dValue;
        reset();
        return true;
    }

    Log(lError)<<Format("Given Id: '{0}' not found.", sId) + "Only species and global parameter values can be set";
    return false;
}
//        void RoadRunner::setValue(string sId, double dValue)
//        {
//            if (!mModel)
//                throw SBWApplicationException(emptyModelStr);
//
//
//            int nIndex = -1;
//            if (mModelGenerator->globalParameterList.find(sId, out nIndex))
//            {
//                model.gp[nIndex] = dValue;
//                return;
//            }
//            if (mModelGenerator->boundarySpeciesList.find(sId, out nIndex))
//            {
//                model.bc[nIndex] = dValue;
//                return;
//            }
//            if (mModelGenerator->compartmentList.find(sId, out nIndex))
//            {
//                model.c[nIndex] = dValue;
//                return;
//            }
//            if (mModelGenerator->floatingSpeciesConcentrationList.find(sId, out nIndex))
//            {
//                model.setConcentration(nIndex, dValue);
//                model.convertToAmounts();
//                if (!mConservedTotalChanged) model.computeConservedTotals();
//                return;
//            }
//            if (mModelGenerator->conservationList.find(sId, out nIndex))
//            {
//                model.ct[nIndex] = dValue;
//                model.updateDependentSpeciesValues(model.y);
//                mConservedTotalChanged = true;
//                return;
//            }
//
//            var initialConditions =
//                new List<string>((string[])getFloatingSpeciesInitialConditionIds().ToArray(typeof(string)));
//            if (initialConditions.Contains(sId))
//            {
//                int index = initialConditions.IndexOf(sId);
//                model.init_y[index] = dValue;
//                reset();
//                return;
//            }
//
//
//            throw SBWApplicationException(String.Format("Given Id: '{0}' not found.", sId),
//                                              "Only species and global parameter values can be set");
//        }
//


// Help("Gets the Value of the given species or global parameter (not of local parameters)")
double RoadRunner::getValue(const string& sId)
{
    if (!mModel)
        throw SBWApplicationException(emptyModelStr);

    int nIndex = 0;
    if (mModelGenerator->globalParameterList.find(sId, nIndex))
    {
        return mModel->gp[nIndex];
    }
    if (mModelGenerator->boundarySpeciesList.find(sId, nIndex))
    {
        return mModel->bc[nIndex];
    }

    if (mModelGenerator->floatingSpeciesConcentrationList.find(sId, nIndex))
    {
        return mModel->y[nIndex];
    }

    if (mModelGenerator->floatingSpeciesConcentrationList.find(sId.substr(0, sId.size() - 1), nIndex))
    {
        mModel->computeAllRatesOfChange();

        //fs[j] + "'" will be interpreted as rate of change
        return mModel->dydt[nIndex];
    }

    if (mModelGenerator->compartmentList.find(sId, nIndex))
    {
        return mModel->c[nIndex];
    }
    if (mModelGenerator->reactionList.find(sId, nIndex))
    {
        return mModel->rates[nIndex];
    }

    if (mModelGenerator->conservationList.find(sId, nIndex))
    {
        return mModel->ct[nIndex];
    }

    StringList initialConditions = getFloatingSpeciesInitialConditionIds();
    if (initialConditions.Contains(sId))
    {
        int index = initialConditions.IndexOf(sId);
        return mModel->init_y[index];
    }

    string tmp("EE:");
    if (sId.compare(0, tmp.size(), tmp) == 0)
    {
        string parameters = sId.substr(3);
        string p1 = parameters.substr(0, parameters.find_first_of(","));
        string p2 = parameters.substr(parameters.find_first_of(",") + 1);
        return getEE(p1, p2, false);
    }

    tmp = ("uEE:");
    if (sId.compare(0, tmp.size(), tmp) == 0)
    {
        string parameters = sId.substr(4);
        string p1 = parameters.substr(0, parameters.find_first_of(","));
        string p2 = parameters.substr(parameters.find_first_of(",") + 1);
        return getuEE(p1, p2, false);
    }

    tmp = ("eigen_");
    if (sId.compare(0, tmp.size(), tmp) == 0)
    {
        string species = sId.substr(tmp.size());
        int index;
        mModelGenerator->floatingSpeciesConcentrationList.find(species, index);

        LibLA LA;

        ls::DoubleMatrix mat = getReducedJacobian();
        vector<Complex> oComplex = LA.getEigenValues(mat);

        if (oComplex.size() > selectionList[index].index)
        {
            return oComplex[selectionList[index].index].Real;
        }
        return std::numeric_limits<double>::quiet_NaN();
    }

    throw SBWApplicationException("Given Id: '" + sId + "' not found.",
                                      "Only species, global parameter values and fluxes can be returned");
}


//
// Help("Gets the Value of the given species or global parameter (not of local parameters)")
//        double RoadRunner::getValue(string sId)
//        {
//            if (!mModel)
//                throw SBWApplicationException(emptyModelStr);
//
//            int nIndex = 0;
//            if (mModelGenerator->globalParameterList.find(sId, out nIndex))
//            {
//                return model.gp[nIndex];
//            }
//            if (mModelGenerator->boundarySpeciesList.find(sId, out nIndex))
//            {
//                return model.bc[nIndex];
//            }
//            if (mModelGenerator->floatingSpeciesConcentrationList.find(sId, out nIndex))
//            {
//                return model.y[nIndex];
//            }
//
//            if (mModelGenerator->floatingSpeciesConcentrationList.find(sId.Substring(0, sId.Length - 1),
//                                                                              out nIndex))
//            {
//                //fs[j] + "'" will be interpreted as rate of change
//                return model.dydt[nIndex];
//            }
//
//            if (mModelGenerator->compartmentList.find(sId, out nIndex))
//            {
//                return model.c[nIndex];
//            }
//            if (mModelGenerator->reactionList.find(sId, out nIndex))
//            {
//                return model.rates[nIndex];
//            }
//
//            if (mModelGenerator->conservationList.find(sId, out nIndex))
//            {
//                return model.ct[nIndex];
//            }
//
//            var initialConditions =
//                new List<string>((string[])getFloatingSpeciesInitialConditionIds().ToArray(typeof(string)));
//            if (initialConditions.Contains(sId))
//            {
//                int index = initialConditions.IndexOf(sId);
//                return model.init_y[index];
//            }
//
//            if (sId.StartsWith("EE:"))
//            {
//                string parameters = sId.Substring(3);
//                var p1 = parameters.Substring(0, parameters.IndexOf(","));
//                var p2 = parameters.Substring(parameters.IndexOf(",") + 1);
//                return getEE(p1, p2, false);
//            }
//
//
//            if (sId.StartsWith("uEE:"))
//            {
//                string parameters = sId.Substring(4);
//                var p1 = parameters.Substring(0, parameters.IndexOf(","));
//                var p2 = parameters.Substring(parameters.IndexOf(",") + 1);
//                return getuEE(p1, p2, false);
//            }
//
//            if (sId.StartsWith("eigen_"))
//            {
//                var species = (sId).Substring("eigen_".Length);
//                int index;
//                mModelGenerator->floatingSpeciesConcentrationList.find(species, out index);
//                Complex[] oComplex = LA.GetEigenValues(getReducedJacobian());
//                if (oComplex.Length > selectionList[index].index)
//                {
//                    return oComplex[selectionList[index].index].Real;
//                }
//                return Double.NaN;
//            }
//
//            throw SBWApplicationException("Given Id: '" + sId + "' not found.",
//                                              "Only species, global parameter values and fluxes can be returned");
//        }



// Help(
//            "Returns symbols of the currently loaded model,
//              that can be used for the selectionlist format array of arrays  { { \"groupname\", { \"item1\", \"item2\" ... } } }."
//            )
ArrayList RoadRunner::getAvailableTimeCourseSymbols()
{
    ArrayList oResult;

    if (!mModel)
    {
        return oResult;
    }

    oResult.Add("Floating Species",                 getFloatingSpeciesIds() );
    oResult.Add("Boundary Species",                 getBoundarySpeciesIds() );
    oResult.Add("Floating Species (amount)",        getFloatingSpeciesAmountIds() );
    oResult.Add("Boundary Species (amount)",        getBoundarySpeciesAmountIds() );
    oResult.Add("Global Parameters",                getParameterIds() );
    oResult.Add("Fluxes",                           getReactionIds() );
    oResult.Add("Rates of Change",                  getRateOfChangeIds() );
    oResult.Add("Volumes",                          mModelGenerator->getCompartmentList() );
    oResult.Add("Elasticity Coefficients",          getElasticityCoefficientIds() );
    oResult.Add("Unscaled Elasticity Coefficients", getUnscaledElasticityCoefficientIds() );
    oResult.Add("Eigenvalues",                      getEigenValueIds() );
    return oResult;
}


//bool RoadRunner::IsNleqAvailable()
//{
//    return NLEQInterface.IsAvailable;
//}

//        void RoadRunner::Test()
//        {
//
//            double[,] results;
//            RoadRunner sim;
//            sim = new RoadRunner();
//            //RoadRunner.ReMultiplyCompartments(false);
//            //RoadRunner.ComputeAndAssignConservationLaws(false);
//            sim.setTolerances(1E-4, 1E-4, 100);
//            //sim.loadSBMLFromFile(@"C:\Development\sbwBuild\source\Translators\TestModels\MathMLTests.xml");
//            //sim.loadSBMLFromFile(@"C:\Development\trunk-sbml\trunk\test-suite\cases\semantic\00938\00938-sbml-l3v1.xml");
//            //sim.loadSBMLFromFile(@"C:\Development\test-suite\cases\semantic\00978\00978-sbml-l3v1.xml");
//            string test = "01104";
//            sim.loadSBMLFromFile(string.Format(@"C:\Development\test-suite\cases\semantic\{0}\{0}-sbml-l3v1.xml", test));
//
//            //sim.loadSBMLFromFile(@"C:\Development\test-suite\cases\semantic\00951\00951-sbml-l3v1.xml");
//            //sim.setTimeCourseSelectionList(new ArrayList(new string[] {
//            //    "time", "x", "y", "p", "q"
//            //    }));
//            //results = sim.simulateEx(0, 2, 11);
//            ////var writer = new StringWriter();
//
//            //sim.loadSBMLFromFile(@"C:\Users\fbergmann\Desktop\max.xml");
//            sim.setTimeCourseSelectionList(new ArrayList(new string[] {
//                "time", "Xref"
//            }));
//            results = sim.simulateEx(0, 10, 11);
//
//            DumpResults(Console.Out, results, sim.getTimeCourseSelectionList());
//
//
//
//            //double[,] results;
//            //RoadRunner sim;
//            //sim = new RoadRunner();
//            ////sim.loadSBMLFromFile(@"C:\Development\sbwBuild\source\Translators\TestModels\MathMLTests.xml");
//            ////sim.loadSBMLFromFile(@"C:\Development\trunk-sbml\trunk\test-suite\cases\semantic\00938\00938-sbml-l3v1.xml");
//            //sim.loadSBMLFromFile(@"C:\Development\test-suite\cases\semantic\00952\00952-sbml-l3v1.xml");
//            ////sim.loadSBMLFromFile(@"C:\Development\test-suite\cases\semantic\00951\00951-sbml-l3v1.xml");
//            //sim.setTimeCourseSelectionList(new ArrayList(new string[] {
//            //    "time", "S", "Q", "R", "reset"
//            //    }));
//            //results = sim.simulateEx(0, 1, 11);
//            ////var writer = new StringWriter();
//            //DumpResults(Console.Out, results, sim.getTimeCourseSelectionList());
//
//            //sim = new RoadRunner();
//            //sim.setTolerances(1e-10, 1e-9);
//            //sim.loadSBMLFromFile(@"C:\Development\test-suite\cases\semantic\00374\00374-sbml-l2v4.xml");
//            //sim.setTimeCourseSelectionList(new ArrayList(new string[] {
//            //    "time", "S1", "S2", "S3", "S4"
//            //    }));
//            //results = sim.simulateEx(0, 2, 51);
//            //DumpResults(Console.Out, results,sim.getTimeCourseSelectionList());
//
//            //sim = new RoadRunner();
//            //mComputeAndAssignConservationLaws = false;
//            //sim.loadSBMLFromFile(@"C:\Development\test-suite\cases\semantic\00424\00424-sbml-l3v1.xml");
//            //sim.setTimeCourseSelectionList(new ArrayList(new string[] {
//            //    "time", "S1", "S2", "S3"
//            //    }));
//
//            ////sim.CorrectMaxStep();
//            //CvodeInterface.MaxStep = 0.0001;
//            //sim.mCVode.reStart(0.0, sim.model);
//
//            ////sim.mCVode.
//            //results = sim.simulateEx(0, 5, 51);
//            //DumpResults(Console.Out, results, sim.getTimeCourseSelectionList());
//
//            //Debug.WriteLine(writer.GetStringBuilder().ToString());
//            //Debug.WriteLine(sim.getWarnings());
//            //Debug.WriteLine(sim.getCapabilities());
//        }
//#endif
//
//         void RoadRunner::emptyModel()
//        {
//            throw SBWApplicationException(emptyModelStr);
//        }
//
//
//
//        [Help(
//            "Compute the value for a particular flux control coefficient, permitted parameters include global parameters, boundary conditions and conservation totals"
//            )]
//        double getUnscaledFluxControlCoefficient(string reactionName, string parameterName)
//        {
//            int fluxIndex;
//            int parameterIndex;
//            TParameterType parameterType;
//            double originalParameterValue;
//            double f1;
//            double f2;
//
//            try
//            {
//                if (mModel)
//                {
//                    mModel->convertToConcentrations();
//                    mModel->computeReactionRates(mModel->GetTime(), mModel->y);
//
//                    if (!mModelGenerator->reactionList.find(reactionName, out fluxIndex))
//                        throw SBWApplicationException(
//                            "Internal Error: unable to locate species name while computing unscaled control coefficient");
//
//                    if (mModelGenerator->globalParameterList.find(parameterName, out parameterIndex))
//                    {
//                        parameterType = TParameterType::ptGlobalParameter;
//                        originalParameterValue = mModel->gp[parameterIndex];
//                    }
//                    else if (mModelGenerator->boundarySpeciesList.find(parameterName, out parameterIndex))
//                    {
//                        parameterType = TParameterType::ptBoundaryParameter;
//                        originalParameterValue = mModel->bc[parameterIndex];
//                    }
//                    else if (mModelGenerator->conservationList.find(parameterName, out parameterIndex))
//                    {
//                        parameterType = TParameterType::ptConservationParameter;
//                        originalParameterValue = mModel->ct[parameterIndex];
//                    }
//                    else throw SBWApplicationException("Unable to locate parameter: [" + parameterName + "]");
//
//                    double hstep = DiffStepSize*originalParameterValue;
//                    if (Math.Abs(hstep) < 1E-12)
//                        hstep = DiffStepSize;
//
//                    try
//                    {
//                        setParameterValue(parameterType, parameterIndex, originalParameterValue + hstep);
//                        steadyState();
//                        mModel->computeReactionRates(mModel->GetTime(), mModel->y);
//                        double fi = mModel->rates[fluxIndex];
//
//                        setParameterValue(parameterType, parameterIndex, originalParameterValue + 2*hstep);
//                        steadyState();
//                        mModel->computeReactionRates(mModel->GetTime(), mModel->y);
//                        double fi2 = mModel->rates[fluxIndex];
//
//                        setParameterValue(parameterType, parameterIndex, originalParameterValue - hstep);
//                        steadyState();
//                        mModel->computeReactionRates(mModel->GetTime(), mModel->y);
//                        double fd = mModel->rates[fluxIndex];
//
//                        setParameterValue(parameterType, parameterIndex, originalParameterValue - 2*hstep);
//                        steadyState();
//                        mModel->computeReactionRates(mModel->GetTime(), mModel->y);
//                        double fd2 = mModel->rates[fluxIndex];
//
//                        // Use instead the 5th order approximation double unscaledElasticity = (0.5/hstep)*(fi-fd);
//                        // The following separated lines avoid small amounts of roundoff error
//                        f1 = fd2 + 8*fi;
//                        f2 = -(8*fd + fi2);
//                    }
//                    finally
//                    {
//                        // What ever happens, make sure we restore the species level
//                        setParameterValue(parameterType, parameterIndex, originalParameterValue);
//                        steadyState();
//                    }
//                    return 1/(12*hstep)*(f1 + f2);
//                }
//                else throw SBWApplicationException(emptyModelStr);
//            }
//            catch (SBWException)
//            {
//                throw;
//            }
//            catch (const Exception& e)
//            {
//                throw SBWApplicationException("Unexpected error from getScaledFluxControlCoefficientMatrix()",
//                                                  e.Message());
//            }
//        }
//
//
//        [Help("Compute the value for a particular scaled flux control coefficients with respect to a local parameter")]
//        double getScaledFluxControlCoefficient(string reactionName, string localReactionName,
//                                                      string parameterName)
//        {
//            int parameterIndex;
//            int reactionIndex;
//
//            try
//            {
//                if (mModel)
//                {
//                    double ufcc = getUnscaledFluxControlCoefficient(reactionName, localReactionName, parameterName);
//
//                    mModel->convertToConcentrations();
//                    mModel->computeReactionRates(mModel->GetTime(), mModel->y);
//
//                    mModelGenerator->reactionList.find(reactionName, out reactionIndex);
//                    if (mModelGenerator->globalParameterList.find(parameterName, out parameterIndex))
//                        return ufcc*mModel->gp[parameterIndex]/mModel->rates[reactionIndex];
//                    else if (mModelGenerator->boundarySpeciesList.find(parameterName, out parameterIndex))
//                        return ufcc*mModel->bc[parameterIndex]/mModel->rates[reactionIndex];
//                    else if (mModelGenerator->conservationList.find(parameterName, out parameterIndex))
//                        return ufcc*mModel->ct[parameterIndex]/mModel->rates[reactionIndex];
//                    return 0.0;
//                }
//                else throw SBWApplicationException(emptyModelStr);
//            }
//            catch (SBWException)
//            {
//                throw;
//            }
//            catch (const Exception& e)
//            {
//                throw SBWApplicationException("Unexpected error from getScaledFluxControlCoefficientMatrix()",
//                                                  e.Message());
//            }
//        }
//
//
//        [Help(
//            "Compute the value for a particular scaled flux control coefficients with respect to a global or boundary species parameter"
//            )]
//        double getScaledFluxControlCoefficient(string reactionName, string parameterName)
//        {
//            int parameterIndex;
//            int reactionIndex;
//
//            try
//            {
//                if (mModel)
//                {
//                    double ufcc = getUnscaledFluxControlCoefficient(reactionName, parameterName);
//
//                    mModel->convertToConcentrations();
//                    mModel->computeReactionRates(mModel->GetTime(), mModel->y);
//
//                    mModelGenerator->reactionList.find(reactionName, out reactionIndex);
//                    if (mModelGenerator->globalParameterList.find(parameterName, out parameterIndex))
//                        return ufcc*mModel->gp[parameterIndex]/mModel->rates[reactionIndex];
//                    else if (mModelGenerator->boundarySpeciesList.find(parameterName, out parameterIndex))
//                        return ufcc*mModel->bc[parameterIndex]/mModel->rates[reactionIndex];
//                    else if (mModelGenerator->conservationList.find(parameterName, out parameterIndex))
//                        return ufcc*mModel->ct[parameterIndex]/mModel->rates[reactionIndex];
//                    return 0.0;
//                }
//                else throw SBWApplicationException(emptyModelStr);
//            }
//            catch (SBWException)
//            {
//                throw;
//            }
//            catch (const Exception& e)
//            {
//                throw SBWApplicationException("Unexpected error from getScaledFluxControlCoefficientMatrix()",
//                                                  e.Message());
//            }
//        }
//    }
//}

string RoadRunner::getCopyright()
{
    return "(c) 2009-2012 HM. Sauro and FT. Bergmann, BSD Licence";
}

string RoadRunner::getURL()
{
    return "http://www.sys-bio.org";
}

string	RoadRunner::getlibSBMLVersion()
{
	return mNOM.getlibSBMLVersion();
}

}//namespace rr
