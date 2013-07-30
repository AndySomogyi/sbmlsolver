/*
 * rrLLVMModelGenerator.cpp
 *
 * Created on: Jun 3, 2013
 *
 * Author: Andy Somogyi,
 *     email decode: V1 = "."; V2 = "@"; V3 = V1;
 *     andy V1 somogyi V2 gmail V3 com
 */
#include "rrLLVMModelGenerator.h"
#include "rrLLVMExecutableModel.h"
#include "rrLLVMModelGeneratorContext.h"
#include "rrLLVMIncludes.h"





/*
 This function is obsolete.. initialize all model data in roadrunner instead..
 all this does is print the size of the struct.
 cInitModel(&mData);

 csetCompartmentVolumes(&mData);
 cinitializeInitialConditions(&mData);
 csetParameterValues(&mData);
 csetBoundaryConditions(&mData);

 csetInitialConditions(&mData);

 cconvertToAmounts(&mData);
 cevalInitialAssignments(&mData);
 ccomputeRules(&mData, y);
 cevalInitialAssignments(&mData);
 double* values = cGetCurrentValues(&mData);
 cresetEvents(&mData);
 cInitializeRateRuleSymbols(&mData);
 cInitializeRates(&mData);
 cconvertToConcentrations(&mData);
 cconvertToConcentrations(&mData);
 ccomputeConservedTotals(&mData);
 cevalEvents(&mData, timeIn, oAmounts);
 ctestConstraints(&mData);
 cupdateDependentSpeciesValues(&mData, y_vec);
 cAssignRates_b(&mData, local_rates);
 ccomputeAllRatesOfChange(&mData);
 */


namespace rr
{

LLVMModelGenerator::LLVMModelGenerator()
{
}

LLVMModelGenerator::~LLVMModelGenerator()
{
}

bool LLVMModelGenerator::setTemporaryDirectory(const string& path)
{
    return true;
}

string LLVMModelGenerator::getTemporaryDirectory()
{
    return LLVMCompiler::gurgle();
}

ExecutableModel* LLVMModelGenerator::createModel(const string& sbml,
        ls::LibStructural*, bool forceReCompile,
        bool computeAndAssignConsevationLaws)
{
    return createModel(sbml, computeAndAssignConsevationLaws);
}

ExecutableModel* LLVMModelGenerator::createModel(const std::string& sbml,
        bool computeAndAssignConsevationLaws)
{
    LLVMModelGeneratorContext context(sbml, computeAndAssignConsevationLaws);

    LLVMEvalInitialConditionsCodeGen::FunctionPtr evalInitialConditionsPtr =
            LLVMEvalInitialConditionsCodeGen(context).createFunction();

    LLVMEvalReactionRatesCodeGen::FunctionPtr evalReactionRatesPtr =
            LLVMEvalReactionRatesCodeGen(context).createFunction();

    LLVMGetBoundarySpeciesAmountCodeGen::FunctionPtr getBoundarySpeciesAmountPtr =
            LLVMGetBoundarySpeciesAmountCodeGen(context).createFunction();

    LLVMGetFloatingSpeciesAmountCodeGen::FunctionPtr getFloatingSpeciesAmountPtr =
            LLVMGetFloatingSpeciesAmountCodeGen(context).createFunction();

    LLVMGetBoundarySpeciesConcentrationCodeGen::FunctionPtr getBoundarySpeciesConcentrationPtr =
            LLVMGetBoundarySpeciesConcentrationCodeGen(context).createFunction();

    LLVMGetFloatingSpeciesConcentrationCodeGen::FunctionPtr getFloatingSpeciesConcentrationPtr =
            LLVMGetFloatingSpeciesConcentrationCodeGen(context).createFunction();

    LLVMGetCompartmentVolumeCodeGen::FunctionPtr getCompartmentVolumePtr =
            LLVMGetCompartmentVolumeCodeGen(context).createFunction();

    LLVMGetGlobalParameterCodeGen::FunctionPtr getGlobalParameterPtr =
            LLVMGetGlobalParameterCodeGen(context).createFunction();


    // if anything up to this point throws an exception, thats OK, because
    // we have not allocated any memory yet that is not taken care of by
    // something else.
    // Now that everything that could have thrown would have thrown, we
    // can now create the model and set its fields.

    LLVMExecutableModel *exe = new LLVMExecutableModel();

    // * MOVE * the bits over from the context to the exe model.
    context.stealThePeach(&exe->symbols, &exe->context, &exe->executionEngine,
            &exe->errStr);

    exe->symbols->initAllocModelDataBuffers(exe->modelData);
    modeldata_clone(&exe->modelDataCopy, &exe->modelData);

    exe->evalInitialConditionsPtr = evalInitialConditionsPtr;
    exe->evalReactionRatesPtr = evalReactionRatesPtr;

    exe->getBoundarySpeciesAmountPtr = getBoundarySpeciesAmountPtr;
    exe->getFloatingSpeciesAmountPtr = getFloatingSpeciesAmountPtr;
    exe->getBoundarySpeciesConcentrationPtr = getBoundarySpeciesConcentrationPtr;
    exe->getFloatingSpeciesConcentrationPtr = getFloatingSpeciesConcentrationPtr;
    exe->getCompartmentVolumePtr = getCompartmentVolumePtr;
    exe->getGlobalParameterPtr = getGlobalParameterPtr;


    return exe;
}

Compiler* LLVMModelGenerator::getCompiler()
{
    return &compiler;
}

bool LLVMModelGenerator::setCompiler(const string& compiler)
{
    return true;
}

/************ LLVM Utility Functions, TODO: Move To Separate File ************/

/**
 * C++ 11 style to_string for LLVM types
 */
std::string to_string(const llvm::Value *value)
{
    std::string str;
    llvm::raw_string_ostream stream(str);
    value->print(stream);
    return str;
}

/*
 *** basic outline of a simulation run is as follows ***



 bool RoadRunner::loadSBML(const string& sbml, const bool& forceReCompile)
{

    delete mModel;
    mModel = mModelGenerator->createModel(sbml, &mLS, &mNOM, forceReCompile, computeAndAssignConservationLaws());

    if(!initializeModel())

    createDefaultSelectionLists();
}


DoubleMatrix RoadRunner::runSimulation()
{

    vector<double> y;
    y = buildModelEvalArgument();
    mModel->evalModel(mTimeStart, &y[0]);
    addNthOutputToResult(results, 0, mTimeStart);

    //Todo: Don't understand this code.. MTK
    if (mCVode->haveVariables())
    {
        mCVode->reStart(mTimeStart, mModel);
    }

    double tout = mTimeStart;

    //The simulation is executed right here..
    Log(lDebug)<<"Will run the OneStep function "<<mNumPoints<<" times";
    for (int i = 1; i < mNumPoints; i++)
    {
        Log(lDebug)<<"Step "<<i;
        mCVode->oneStep(tout, hstep);
        tout = mTimeStart + i * hstep;
        addNthOutputToResult(results, i, tout);
    }
}

double CvodeInterface::oneStep(const double& _timeStart, const double& hstep)
{
    Log(lDebug3)<<"---------------------------------------------------";
    Log(lDebug3)<<"--- O N E     S T E P      ( "<<mOneStepCount<< " ) ";
    Log(lDebug3)<<"---------------------------------------------------";

    mOneStepCount++;
    mCount = 0;

    double timeEnd = 0.0;
    double timeStart = _timeStart;
    double tout = timeStart + hstep;
    int strikes = 3;
    try
    {
        // here we stop for a too small timestep ... this seems troublesome to me ...
        while (tout - timeEnd > 1E-16)
        {
            if (hstep < 1E-16)
            {
                return tout;
            }

            // here we bail in case we have no ODEs set up with CVODE ... though we should
            // still at least evaluate the model function
            if (!haveVariables() && mTheModel->getNumEvents() == 0)
            {
                mTheModel->convertToAmounts();
                mTheModel->evalModel(tout, 0);
                return tout;
            }

            if (mLastTimeValue > timeStart)
            {
                reStart(timeStart, mTheModel);
            }

            double nextTargetEndTime = tout;
            if (mAssignmentTimes.size() > 0 && mAssignmentTimes[0] < nextTargetEndTime)
            {
                nextTargetEndTime = mAssignmentTimes[0];
                mAssignmentTimes.erase(mAssignmentTimes.begin());
            }

            int nResult = CVode(mCVODE_Memory, nextTargetEndTime,  mAmounts, &timeEnd, CV_NORMAL);

            if (nResult == CV_ROOT_RETURN && mFollowEvents)
            {
                Log(lDebug1)<<("---------------------------------------------------");
                Log(lDebug1)<<"--- E V E N T      ( " << mOneStepCount << " ) ";
                Log(lDebug1)<<("---------------------------------------------------");

                bool tooCloseToStart = fabs(timeEnd - mLastEvent) > mRelTol;

                if(tooCloseToStart)
                {
                    strikes =  3;
                }
                else
                {
                    strikes--;
                }

                if (tooCloseToStart || strikes > 0)
                {
                    handleRootsFound(timeEnd, tout);
                    reStart(timeEnd, mTheModel);
                    mLastEvent = timeEnd;
                }
            }
            else if (nResult == CV_SUCCESS || !mFollowEvents)
            {
                //mTheModel->resetEvents();
                mTheModel->setTime(tout);
                assignResultsToModel();
            }
            else
            {
                handleCVODEError(nResult);
            }

            mLastTimeValue = timeEnd;

            try
            {
                mTheModel->testConstraints();
            }
            catch (const Exception& e)
            {
                Log(lWarning)<<"Constraint Violated at time = " + toString(timeEnd)<<": " + e.Message();

            }

            assignPendingEvents(timeEnd, tout);

            if (tout - timeEnd > 1E-16)
            {
                timeStart = timeEnd;
            }
            Log(lDebug3)<<"tout: "<<tout<<gTab<<"timeEnd: "<<timeEnd;
        }
        return (timeEnd);
    }
    catch(const Exception& ex)
    {
        Log(lError)<<"Problem in OneStep: "<<ex.getMessage()<<endl;
        initializeCVODEInterface(mTheModel);    //tk says ??? tk
        throw;
    }
}

 */


} /* namespace rr */


