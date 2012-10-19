#ifdef USE_PCH
#include "rr_pch.h"
#endif
#pragma hdrstop
#include <iomanip>
#include <math.h>
#include <map>
#include <algorithm>
#include "rrRoadRunner.h"
#include "rrModelFromC.h"
#include "rrCvodedll.h"
#include "rrException.h"
#include "rrModelState.h"
#include "rrLogger.h"
#include "rrStringUtils.h"
#include "rrException.h"
#include "rrCvodeInterface.h"
#include "rrCvodeDll.h"
#include "rrUtils.h"
#include "rrEvent.h"
//---------------------------------------------------------------------------

using namespace std;
namespace rr
{
//helper function for call back functions..
vector<double> BuildEvalArgument(ModelFromC* model);

//Static stuff...
//double      CvodeInterface::lastTimeValue = 0;
int         CvodeInterface::mCount = 0;
int         CvodeInterface::mRootCount = 0;
int         CvodeInterface::errorFileCounter = 0;
string      CvodeInterface::tempPathstring = "c:\\";
ModelFromC* CvodeInterface::model = NULL;

// -------------------------------------------------------------------------
// Constructor
// Model contains all the symbol tables associated with the model
// ev is the model function
// -------------------------------------------------------------------------
CvodeInterface::CvodeInterface(RoadRunner* rr, ModelFromC *aModel, const double& _absTol, const double& _relTol)
:
defaultReltol(_relTol),
defaultAbsTol(_absTol),
defaultMaxNumSteps(10000),
gdata(NULL),
_amounts(NULL),
cvodeLogFile("cvodeLogFile"),
followEvents(true),
mRandom(),
defaultMaxAdamsOrder(12),
defaultMaxBDFOrder(5),
MaxAdamsOrder(defaultMaxAdamsOrder),
MaxBDFOrder(defaultMaxBDFOrder),
InitStep(0.0),
MinStep(0.0),
MaxStep(0.0),
MaxNumSteps(defaultMaxNumSteps),
relTol(_relTol),
absTol(_absTol),
//errorFileCounter,
//_rootsFound),
abstolArray(NULL),
fileHandle(NULL),
//modelDelegate(&CvodeInterface::ModelFcn)
mRR(rr),
cvodeMem(NULL),
mOneStepCount(0),
lastTimeValue(0),
lastEvent(0)

{
    InitializeCVODEInterface(aModel);
}

CvodeInterface::~CvodeInterface()
{
    FreeCvode_Mem((void**) cvodeMem);
    FreeCvode_Vector(_amounts);
    FreeCvode_Vector(abstolArray);
    fileClose(fileHandle);
}

////        public void ModelFcn(int n, double time, IntPtr y, IntPtr ydot, IntPtr fdata)
////        {
////            var oldState = new ModelState(model);
////
////            var dCVodeArgument = new double[model.amounts.Length + model.rateRules.Length];
////            Marshal.Copy(y, dCVodeArgument, 0, Math.Min(n, dCVodeArgument.Length));
////
////#if (PRINT_STEP_DEBUG)
////                    System.Diagnostics.Debug.Write("CVode In: (" + nCount + ")" );
////                    for (int i = 0; i < dCVodeArgument.Length; i++)
////                    {
////                        System.Diagnostics.Debug.Write(dCVodeArgument[i].ToString() + ", ");
////                    }
////                    System.Diagnostics.Debug.WriteLine("");
////#endif
////
////            model.evalModel(time, dCVodeArgument);
////
////            model.rateRules.CopyTo(dCVodeArgument, 0);
////            model.dydt.CopyTo(dCVodeArgument, model.rateRules.Length);
////
////#if (PRINT_STEP_DEBUG)
////                    System.Diagnostics.Debug.Write("CVode Out: (" + nCount + ")");
////                    for (int i = 0; i < dCVodeArgument.Length; i++)
////                    {
////                        System.Diagnostics.Debug.Write(dCVodeArgument[i].ToString() + ", ");
////                    }
////                    System.Diagnostics.Debug.WriteLine("");
////#endif
////
////            Marshal.Copy(dCVodeArgument, 0, ydot, Math.Min(dCVodeArgument.Length, n));
////
////            nCount++;
////
////            oldState.AssignToModel(model);
////        }

void ModelFcn(int n, double time, cvode_precision* y, cvode_precision* ydot, void* fdata)
{
    ModelFromC *model = CvodeInterface::model;
    ModelState oldState(*model);

    int size = *model->amountsSize + (*model->rateRulesSize);
    vector<double> dCVodeArgument(size);

    for(int i = 0; i < min((int) dCVodeArgument.size(), n); i++)
    {
        dCVodeArgument[i] = y[i];
    }

    stringstream msg;
    msg<<left<<setw(20)<<"Count = "<<(CvodeInterface::mCount)<<"\t";

    for (u_int i = 0; i < dCVodeArgument.size(); i++)
    {
        //msg<<left<<setw(20)<<setprecision(4)<<dCVodeArgument[i];
    }

    model->evalModel(time, dCVodeArgument);

    CopyCArrayToStdVector(model->rateRules,    dCVodeArgument, (*model->rateRulesSize));

    for(u_int i = 0 ; i < (*model->dydtSize); i++)
    {
        dCVodeArgument.push_back(model->dydt[i]);
    }

    msg<<"\tcount = "<<CvodeInterface::mCount << "\t" ;
    for (u_int i = 0; i < dCVodeArgument.size(); i++)
    {
        //msg<<setw(20)<<left<<setprecision(4)<<dCVodeArgument[i];
    }

    Log(lDebug4)<<msg.str();

    for (int i = 0; i < min((int) dCVodeArgument.size(), n); i++)
    {
        ydot[i]= dCVodeArgument[i];
    }

    CvodeInterface::mCount++;
    oldState.AssignToModel(*model);
}

////        void CvodeInterface::EventFcn(double time, IntPtr y, IntPtr gdot, IntPtr fdata)
void EventFcn(double time, cvode_precision* y, cvode_precision* gdot, void* fdata)
{
    ModelFromC *model = CvodeInterface::model;
    ModelState* oldState = new ModelState(*model);

    vector<double> args = BuildEvalArgument(model);
    model->evalModel(time, args);
    model->mCvodeInterface->AssignResultsToModel(); //AssignResultsToModel();

    args = BuildEvalArgument(model);
    model->evalEvents(time, args);

//    Marshal.Copy(model.eventTests, 0, gdot, model.getNumEvents());

    for(int i = 0; i < model->getNumEvents(); i++)
    {
        gdot[i] = model->eventTests[i];
    }

    Log(lDebug3)<<"S1 Value"<<model->amounts[0];
    Log(lDebug3)<<"Rootfunction Out: t="<<time<<" ("<< CvodeInterface::mRootCount <<"): ";
    for (int i = 0; i < *model->eventTestsSize; i++)
    {
        Log(lDebug3)<<ToString(model->eventTests[i])<<" p="<<model->previousEventStatusArray[i]<<" c="<<ToString(model->eventStatusArray[i])<<", ";
    }
    CvodeInterface::mRootCount++;
    oldState->AssignToModel(*model);
    delete oldState;
}

////        void CvodeInterface::EventFcn(double time, IntPtr y, IntPtr gdot, IntPtr fdata)
////        {
////
////            var oldState = new ModelState(model);
////
////            model.evalModel(time, BuildEvalArgument());
////            AssignResultsToModel();
////            model.evalEvents(time, BuildEvalArgument());
////
////            Marshal.Copy(model.eventTests, 0, gdot, model.getNumEvents());
////
////#if (PRINT_EVENT_DEBUG)
////                    System.Diagnostics.Debug.Write("Rootfunction Out: t=" + time.ToString("F14") + " (" + nRootCount + "): ");
////                    for (int i = 0; i < model.eventTests.Length; i++)
////                    {
////                        System.Diagnostics.Debug.Write(model.eventTests[i].ToString() + " p=" + model.previousEventStatusArray[i] + " c=" + model.eventStatusArray[i] + ", ");
////                    }
////                    System.Diagnostics.Debug.WriteLine("");
////#endif
////
////            nRootCount++;
////
////            oldState.AssignToModel(model);
////        }
////
////        #endregion

bool CvodeInterface::HaveVariables()
{
    return (numAdditionalRules + numIndependentVariables > 0) ? true : false;
}

void CvodeInterface::InitializeCVODEInterface(ModelFromC *oModel)
{
    if(!oModel)
    {
        throw SBWApplicationException("Fatal Error while initializing CVODE");
    }

    try
    {
        model = oModel;
        numIndependentVariables = oModel->getNumIndependentVariables();
        numAdditionalRules = (*oModel->rateRulesSize);

        if (numAdditionalRules + numIndependentVariables > 0)
        {
            int allocatedMemory = numIndependentVariables + numAdditionalRules;
            _amounts =     NewCvode_Vector(allocatedMemory);
            abstolArray = NewCvode_Vector(allocatedMemory);
            for (int i = 0; i < allocatedMemory; i++)
            {
                Cvode_SetVector((N_Vector) abstolArray, i, defaultAbsTol);
            }

            AssignNewVector(oModel, true);

            cvodeMem = (void*) Create_BDF_NEWTON_CVode();
            SetMaxOrder(cvodeMem, MaxBDFOrder);
            CVodeSetInitStep(cvodeMem, InitStep);
            SetMinStep(cvodeMem, MinStep);
            SetMaxStep(cvodeMem, MaxStep);

            SetMaxNumSteps(cvodeMem, MaxNumSteps);

            fileHandle = fileOpen(tempPathstring + cvodeLogFile + ToString(errorFileCounter) + ".txt");
            SetErrFile(cvodeMem, fileHandle);
            errCode = AllocateCvodeMem(cvodeMem, allocatedMemory, ModelFcn, (cvode_precision) 0.0, (N_Vector) _amounts, relTol, (N_Vector) abstolArray);

            if (errCode < 0)
            {
                HandleCVODEError(errCode);
            }

            if (oModel->getNumEvents() > 0)
            {
                errCode = CVRootInit(cvodeMem, oModel->getNumEvents(), EventFcn, gdata);
                Log(lDebug2)<<"CVRootInit executed.....";
            }

            errCode = CvDense(cvodeMem, allocatedMemory); // int = size of systems
            if (errCode < 0)
            {
                HandleCVODEError(errCode);
            }

            oModel->resetEvents();
        }
        else if (model->getNumEvents() > 0)
        {
            int allocated = 1;
            _amounts =  NewCvode_Vector(allocated);
            abstolArray =  NewCvode_Vector(allocated);
            Cvode_SetVector( (N_Vector) _amounts, 0, 10);
            Cvode_SetVector( (N_Vector) abstolArray, 0, defaultAbsTol);

            cvodeMem = (long*) Create_BDF_NEWTON_CVode();
            SetMaxOrder(cvodeMem, MaxBDFOrder);
            SetMaxNumSteps(cvodeMem, MaxNumSteps);

            fileHandle = fileOpen(tempPathstring + cvodeLogFile + ToString(errorFileCounter) + ".txt");
            SetErrFile(cvodeMem, fileHandle);

            errCode = AllocateCvodeMem(cvodeMem, allocated, ModelFcn, 0.0, (N_Vector) _amounts, relTol, (N_Vector) abstolArray);
            if (errCode < 0)
            {
                HandleCVODEError(errCode);
            }

            if (oModel->getNumEvents() > 0)
            {
                errCode = CVRootInit(cvodeMem, oModel->getNumEvents(), EventFcn, gdata);
                Log(lDebug2)<<"CVRootInit executed.....";
            }

            errCode = CvDense(cvodeMem, allocated); // int = size of systems
            if (errCode < 0)
            {
                HandleCVODEError(errCode);
            }

            oModel->resetEvents();
        }
    }
    catch (RRException ex)
    {
        throw SBWApplicationException("Fatal Error while initializing CVODE");//, ex.mMessage);
    }
}

////    class CvodeInterface : IDisposable
////    {
////        /// <summary>
////        /// Point to the CVODE DLL to use
////        /// </summary>
////        const string CVODE = "cvodedll";
////
////        static int nOneStepCount;
////
////        #region Default CVODE Settings
////
////        const double defaultReltol = 1E-6;
////        const double defaultAbsTol = 1E-12;
////        const int defaultMaxNumSteps = 10000;
////        static int defaultMaxAdamsOrder = 12;
////        static int defaultMaxBDFOrder = 5;
////
////        static int MaxAdamsOrder = defaultMaxAdamsOrder;
////        static int MaxBDFOrder = defaultMaxBDFOrder;
////
////        static double InitStep = 0.0;
////        static double MinStep = 0.0;
////        static double MaxStep = 0.0;
////
////
////        //static double defaultReltol = 1E-15;
////        //static double defaultAbsTol = 1E-20;
////
////        static int MaxNumSteps = defaultMaxNumSteps;
////        static double relTol = defaultReltol;
////        static double absTol = defaultAbsTol;
////
////        #endregion
////
////        #region Class Variables
////
////        // Error codes
////        const int CV_ROOT_RETURN = 2;
////        const int CV_TSTOP_RETURN = 1;
////        const int CV_SUCCESS = 0;
////        const int CV_MEM_NULL = -1;
////        const int CV_ILL_INPUT = -2;
////        const int CV_NO_MALLOC = -3;
////        const int CV_TOO_MUCH_WORK = -4;
////        const int CV_TOO_MUCH_ACC = -5;
////        const int CV_ERR_FAILURE = -6;
////        const int CV_CONV_FAILURE = -7;
////        const int CV_LINIT_FAIL = -8;
////        const int CV_LSETUP_FAIL = -9;
////        const int CV_LSOLVE_FAIL = -10;
////
////        const int CV_MEM_FAIL = -11;
////
////        const int CV_RTFUNC_NULL = -12;
////        const int CV_NO_SLDET = -13;
////        const int CV_BAD_K = -14;
////        const int CV_BAD_T = -15;
////        const int CV_BAD_DKY = -16;
////
////        const int CV_PDATA_NULL = -17;
////        static readonly string tempPathstring = Path.GetTempPath();
////
////
////        static int errorFileCounter;
////        static double lastTimeValue;
////        readonly IntPtr gdata = IntPtr.Zero;
////        IntPtr _amounts;
////        IntPtr _rootsFound;
////        IntPtr abstolArray;
////        string cvodeLogFile = "cvodeLogFile";
////        IntPtr cvodeMem;
////        int errCode;
////
////        IntPtr fileHandle;
////        IModel model;
////        int numIndependentVariables;
////
////        #endregion
////
////        #region Library Imports
////
////        [DllImport(CVODE, EntryPoint = "fileOpen", ExactSpelling = false,
////            CharSet = CharSet.Ansi, SetLastError = true)]
////        static extern IntPtr fileOpen(string fileName);
////
////        [DllImport(CVODE, EntryPoint = "fileClose", ExactSpelling = false,
////            CharSet = CharSet.Unicode, SetLastError = true)]
////        static extern void fileClose(IntPtr fp);
////
////
////        [DllImport(CVODE, EntryPoint = "NewCvode_Vector", ExactSpelling = false,
////            CharSet = CharSet.Unicode, SetLastError = true)]
////        static extern IntPtr NewCvode_Vector(int n);
////
////        [DllImport(CVODE, EntryPoint = "FreeCvode_Vector", ExactSpelling = false,
////            CharSet = CharSet.Unicode, SetLastError = true)]
////        static extern void FreeCvode_Vector(IntPtr vect);
////
////        [DllImport(CVODE, EntryPoint = "FreeCvode_Mem", ExactSpelling = false,
////            CharSet = CharSet.Unicode, SetLastError = true)]
////        static extern void FreeCvode_Mem(IntPtr p);
////
////        // void *p
////
////        [DllImport(CVODE, EntryPoint = "Cvode_SetVector", ExactSpelling = false,
////            CharSet = CharSet.Unicode, SetLastError = true)]
////        static extern void Cvode_SetVector(IntPtr v, int Index, double Value);
////
////        [DllImport(CVODE, EntryPoint = "Cvode_GetVector", ExactSpelling = false,
////            CharSet = CharSet.Unicode, SetLastError = true)]
////        static extern double Cvode_GetVector(IntPtr v, int Index);
////
////        [DllImport(CVODE, EntryPoint = "Create_BDF_NEWTON_CVode", ExactSpelling = false,
////            CharSet = CharSet.Unicode, SetLastError = true)]
////        static extern IntPtr Create_BDF_NEWTON_CVode();
////
////        [DllImport(CVODE, EntryPoint = "Create_ADAMS_FUNCTIONAL_CVode", ExactSpelling = false,
////            CharSet = CharSet.Unicode, SetLastError = true)]
////        static extern IntPtr Create_ADAMS_FUNCTIONAL_CVode();
////
////        [DllImport(CVODE, EntryPoint = "AllocateCvodeMem", ExactSpelling = false,
////            CharSet = CharSet.Unicode, SetLastError = true)]
////        static extern int AllocateCvodeMem(IntPtr cvode_mem, int n, TCallBackModelFcn fcn, double t0, IntPtr y,
////                                                  double reltol, IntPtr abstol);
////
////        [DllImport(CVODE, EntryPoint = "CvDense", ExactSpelling = false,
////            CharSet = CharSet.Unicode, SetLastError = true)]
////        static extern int CvDense(IntPtr cvode_mem, int n);
////
////        // int = size of systems
////
////        [DllImport(CVODE, EntryPoint = "CVReInit", ExactSpelling = false,
////            CharSet = CharSet.Unicode, SetLastError = true)]
////        static extern int CVReInit(IntPtr cvode_mem, double t0, IntPtr y0, double reltol, IntPtr abstol);
////
////        [DllImport(CVODE, EntryPoint = "Run_Cvode")]
////        //static extern int  RunCvode (IntPtr cvode_mem, double tout, IntPtr  y, ref double t, string ErrMsg);
////        static extern int RunCvode(IntPtr cvode_mem, double tout, IntPtr y, ref double t);
////
////        //static extern int  RunCvode (IntPtr cvode_mem, double tout, IntPtr y, ref double t);  // t = double *
////
////
////        [DllImport(CVODE, EntryPoint = "CVGetRootInfo", ExactSpelling = false,
////            CharSet = CharSet.Unicode, SetLastError = true)]
////        static extern int CVGetRootInfo(IntPtr cvode_mem, IntPtr rootsFound);
////
////        [DllImport(CVODE, EntryPoint = "CVRootInit", ExactSpelling = false,
////            CharSet = CharSet.Unicode, SetLastError = true)]
////        static extern int CVRootInit(IntPtr cvode_mem, int numRoots, TCallBackRootFcn rootfcn, IntPtr gdata);
////
////        [DllImport(CVODE, EntryPoint = "SetMaxNumSteps", ExactSpelling = false,
////            CharSet = CharSet.Unicode, SetLastError = true)]
////        static extern int SetMaxNumSteps(IntPtr cvode_mem, int mxsteps);
////
////
////        [DllImport(CVODE, EntryPoint = "SetMinStep", ExactSpelling = false,
////            CharSet = CharSet.Unicode, SetLastError = true)]
////        static extern int SetMinStep(IntPtr cvode_mem, double minStep);
////        [DllImport(CVODE, EntryPoint = "SetMaxStep", ExactSpelling = false,
////                    CharSet = CharSet.Unicode, SetLastError = true)]
////        static extern int SetMaxStep(IntPtr cvode_mem, double maxStep);
////        [DllImport(CVODE, EntryPoint = "SetInitStep", ExactSpelling = false,
////                    CharSet = CharSet.Unicode, SetLastError = true)]
////        static extern int SetInitStep(IntPtr cvode_mem, double initStep);
////
////        [DllImport(CVODE, EntryPoint = "SetMaxOrder", ExactSpelling = false,
////            CharSet = CharSet.Unicode, SetLastError = true)]
////        static extern int SetMaxOrder(IntPtr cvode_mem, int mxorder);
////
////        [DllImport(CVODE, EntryPoint = "SetMaxErrTestFails", ExactSpelling = false,
////            CharSet = CharSet.Unicode, SetLastError = true)]
////        static extern int SetMaxErrTestFails(IntPtr cvode_mem, int maxnef);
////
////        [DllImport(CVODE, EntryPoint = "SetMaxConvFails", ExactSpelling = false,
////            CharSet = CharSet.Unicode, SetLastError = true)]
////        static extern int SetMaxConvFails(IntPtr cvode_mem, int maxncf);
////
////        [DllImport(CVODE, EntryPoint = "SetMaxNonLinIters", ExactSpelling = false,
////            CharSet = CharSet.Unicode, SetLastError = true)]
////        static extern int SetMaxNonLinIters(IntPtr cvode_mem, int maxcor);
////
////        [DllImport(CVODE, EntryPoint = "SetErrFile", ExactSpelling = false,
////            CharSet = CharSet.Unicode, SetLastError = true)]
////        static extern int SetErrFile(IntPtr cvode_mem, IntPtr errfp);
////
////        #endregion
////
////        #region CVODE Callback functions
////
////        #region Delegates
////
////        delegate void TCallBackModelFcn(int n, double time, IntPtr y, IntPtr ydot, IntPtr fdata);
////
////        delegate void TCallBackRootFcn(double t, IntPtr y, IntPtr gdot, IntPtr gdata);
////
////        #endregion
////
////        static TCallBackModelFcn modelDelegate;
////        static TCallBackRootFcn eventDelegate;
////
////        static int nCount;
////        static int nRootCount;
////



////        double[] CvodeInterface::GetCopy(double[] oVector)
////        {
////            var oResult = new double[oVector.Length];
////            oVector.CopyTo(oResult, 0);
////            return oResult;
////        }
////
////        bool[] CvodeInterface::GetCopy(bool[] oVector)
////        {
////            var oResult = new bool[oVector.Length];
////            oVector.CopyTo(oResult, 0);
////            return oResult;
////        }
////
////
////
////        #region ErrorHandling
////
////        static CvodeErrorCodes[] errorCodes = InitilizeErrorCodes();
////
////        static CvodeErrorCodes[] CvodeInterface::InitilizeErrorCodes()
////        {
////            var oErrorCodes = new CvodeErrorCodes[28];
////            oErrorCodes[0] = new CvodeErrorCodes(CV_SUCCESS, "Success");
////            oErrorCodes[1] = new CvodeErrorCodes(-1, "The solver took mxstep steps but could not reach tout.");
////            oErrorCodes[2] = new CvodeErrorCodes(-2,
////                                                 "The solver could not satisfy the accuracy demanded by the user for some step.");
////            oErrorCodes[3] = new CvodeErrorCodes(-3,
////                                                 "Error test failures occurred too many times during one time step or minimum step size was reached.");
////            oErrorCodes[4] = new CvodeErrorCodes(-4,
////                                                 "Convergence test failures occurred too many times during one time step or minimum step size was reached.");
////            oErrorCodes[5] = new CvodeErrorCodes(-5, "The linear solver's initialization function failed.");
////            oErrorCodes[6] = new CvodeErrorCodes(-6,
////                                                 "The linear solver's setup function failed in an unrecoverable manner.");
////            oErrorCodes[7] = new CvodeErrorCodes(-7,
////                                                 "The linear solver's solve function failed in an unrecoverable manner.");
////            oErrorCodes[8] = new CvodeErrorCodes(-8, "The right-hand side function failed in an unrecoverable manner.");
////            oErrorCodes[9] = new CvodeErrorCodes(-9, "The right-hand side function failed at the first call.");
////            oErrorCodes[10] = new CvodeErrorCodes(-10, "The right-hand side function had repetead recoverable errors.");
////            oErrorCodes[11] = new CvodeErrorCodes(-11,
////                                                  "The right-hand side function had a recoverable error, but no recovery is possible.");
////            oErrorCodes[12] = new CvodeErrorCodes(-12, "The rootfinding function failed in an unrecoverable manner.");
////            oErrorCodes[13] = new CvodeErrorCodes(-13, "");
////            oErrorCodes[14] = new CvodeErrorCodes(-14, "");
////            oErrorCodes[15] = new CvodeErrorCodes(-15, "");
////            oErrorCodes[16] = new CvodeErrorCodes(-16, "");
////            oErrorCodes[17] = new CvodeErrorCodes(-17, "");
////            oErrorCodes[18] = new CvodeErrorCodes(-18, "");
////            oErrorCodes[19] = new CvodeErrorCodes(-19, "");
////            oErrorCodes[20] = new CvodeErrorCodes(-20, "A memory allocation failed");
////            oErrorCodes[21] = new CvodeErrorCodes(-21, "The cvode mem argument was NULL.");
////            oErrorCodes[22] = new CvodeErrorCodes(-22, "One of the function inputs is illegal.");
////            oErrorCodes[23] = new CvodeErrorCodes(-23,
////                                                  "The cvode memory block was not allocated by a call to CVodeMalloc.");
////            oErrorCodes[24] = new CvodeErrorCodes(-24, "The derivative order k is larger than the order used.");
////            oErrorCodes[25] = new CvodeErrorCodes(-25, "The time t s outside the last step taken.");
////            oErrorCodes[26] = new CvodeErrorCodes(-26, "The output derivative vector is NULL.");
////            oErrorCodes[27] = new CvodeErrorCodes(-27, "The output and initial times are too close to each other.");
////            return oErrorCodes;
////        }
////

void CvodeInterface::HandleCVODEError(int errCode)
{
    if (errCode < 0)
    {
        string msg = "";
        string errorFile = tempPathstring + cvodeLogFile + ToString(errorFileCounter) + ".txt";

        // and open a new file handle
        errorFileCounter++;
        FILE* newHandle = fileOpen(tempPathstring + cvodeLogFile + ToString(errorFileCounter) + ".txt");
        if (newHandle != NULL && cvodeMem != NULL)
        {
            SetErrFile(cvodeMem, newHandle);
        }
        // close error file used by the cvode library
        if (fileHandle != NULL)
        {
            fileClose(fileHandle);
        }

        fileHandle = newHandle;

        //throw CvodeException("Error in RunCVode: " + errorCodes[-errCode].msg + msg);
        Log(lError)<<"Error in RunCVode: "<<errCode<<msg;
        throw CvodeException("Error in RunCVode: " + ToString(errCode) + msg);
    }
}

double CvodeInterface::OneStep(double timeStart, double hstep)
{
    Log(lDebug3)<<"---------------------------------------------------";
    Log(lDebug3)<<"--- O N E     S T E P      ( "<<mOneStepCount<< " ) ";
    Log(lDebug3)<<"---------------------------------------------------";

    mOneStepCount++;
    mCount = 0;

    double timeEnd = 0.0;
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
            if (!HaveVariables() && model->getNumEvents() == 0)
            {
                model->convertToAmounts();
                vector<double> args = BuildEvalArgument();
                model->evalModel(tout, args);
                return tout;
            }

            if (lastTimeValue > timeStart)
            {
                reStart(timeStart, model);
            }

            double nextTargetEndTime = tout;
            if (assignmentTimes.size() > 0 && assignmentTimes[0] < nextTargetEndTime)
            {
                nextTargetEndTime = assignmentTimes[0];
                assignmentTimes.erase(assignmentTimes.begin());
            }

            int nResult = Run_Cvode(cvodeMem, nextTargetEndTime,  _amounts, &timeEnd);//, err); // t = double *

            if (nResult == CV_ROOT_RETURN && followEvents)
            {
                Log(lDebug1)<<("---------------------------------------------------");
                Log(lDebug1)<<"--- E V E N T      ( " << mOneStepCount << " ) ";
                Log(lDebug1)<<("---------------------------------------------------");

//                bool tooCloseToStart = Math.Abs(timeEnd - timeStart) > absTol;
//                bool tooCloseToStart = fabsl(timeEnd - timeStart) > absTol;
                bool tooCloseToStart = fabs(timeEnd - lastEvent) > relTol;

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
                    HandleRootsFound(timeEnd, tout);
                    reStart(timeEnd, model);
                    lastEvent = timeEnd;
                }
            }
            else if (nResult == CV_SUCCESS || !followEvents)
            {
                //model->resetEvents();
                model->SetTime(tout);
                AssignResultsToModel();
            }
            else
            {
                HandleCVODEError(nResult);
            }

            lastTimeValue = timeEnd;

            try
            {
                model->testConstraints();
            }
            catch (Exception e)
            {
                model->Warnings.push_back("Constraint Violated at time = " + ToString(timeEnd) + "\n" + e.Message());
            }

            AssignPendingEvents(timeEnd, tout);

            if (tout - timeEnd > 1E-16)
            {
                timeStart = timeEnd;
            }
            Log(lDebug3)<<"tout: "<<tout<<tab<<"timeEnd: "<<timeEnd;
        }
        return (timeEnd);
    }
    catch (Exception)
    {
        InitializeCVODEInterface(model);
        throw;
    }
}

////        public double OneStep(double timeStart, double hstep)
////        {
////#if (PRINT_DEBUG)
////                    System.Diagnostics.Debug.WriteLine("---------------------------------------------------");
////                    System.Diagnostics.Debug.WriteLine("--- O N E     S T E P      ( " + nOneStepCount + " ) ");
////                    System.Diagnostics.Debug.WriteLine("---------------------------------------------------");
////#endif
////            nOneStepCount++;
////            nCount = 0;
////
////            double timeEnd = 0.0;
////            double tout = timeStart + hstep;
////            int strikes = 3;
////            try
////            {
////                // here we stop for a too small timestep ... this seems troublesome to me ...
////                while (tout - timeEnd > 1E-16)
////                {
////                    if (hstep < 1E-16) return tout;
////
////                    // here we bail in case we have no ODEs set up with CVODE ... though we should
////                    // still at least evaluate the model function
////                    if (!HaveVariables && model.getNumEvents == 0)
////                    {
////                        model.convertToAmounts();
////                        model.evalModel(tout, BuildEvalArgument());
////                        return tout;
////                    }
////
////
////                    if (lastTimeValue > timeStart)
////                        reStart(timeStart, model);
////
////                    double nextTargetEndTime = tout;
////                    if (assignmentTimes.Count > 0 && assignmentTimes[0] < nextTargetEndTime)
////                    {
////                        nextTargetEndTime = assignmentTimes[0];
////                        assignmentTimes.RemoveAt(0);
////                    }
////
////                    int nResult = RunCvode(cvodeMem, nextTargetEndTime, _amounts, ref timeEnd); // t = double *
////
////                    if (nResult == CV_ROOT_RETURN && followEvents)
////                    {
////#if (PRINT_DEBUG)
////                                System.Diagnostics.Debug.WriteLine("---------------------------------------------------");
////                                System.Diagnostics.Debug.WriteLine("--- E V E N T      ( " + nOneStepCount + " ) ");
////                                System.Diagnostics.Debug.WriteLine("---------------------------------------------------");
////#endif
////
////                        //bool tooCloseToStart = Math.Abs(timeEnd - timeStart) > absTol;
////                        bool tooCloseToStart = Math.Abs(timeEnd - lastEvent) > relTol;
////                        if (tooCloseToStart)
////                            strikes = 3;
////                        else
////                            strikes--;
////
////                        if (tooCloseToStart || strikes > 0)
////                        {
////                            HandleRootsFound(ref timeEnd, tout);
////                            reStart(timeEnd, model);
////                            lastEvent = timeEnd;
////                        }
////                    }
////                    else if (nResult == CV_SUCCESS || !followEvents)
////                    {
////                        //model.resetEvents();
////                        model.time = tout;
////                        AssignResultsToModel();
////                    }
////                    else
////                    {
////                        HandleCVODEError(nResult);
////                    }
////
////                    lastTimeValue = timeEnd;
////
////                    try
////                    {
////                        model.testConstraints();
////                    }
////                    catch (Exception e)
////                    {
////                        model.Warnings.Add("Constraint Violated at time = " + timeEnd + "\n" +
////                                                             e.Message);
////                    }
////
////                    AssignPendingEvents(timeEnd, tout);
////
////                    if (tout - timeEnd > 1E-16)
////                        timeStart = timeEnd;
////                }
////                return (timeEnd);
////            }
////            catch (NullReferenceException ex)
////            {
////                throw new SBWApplicationException("Internal error, please reload the model", ex.StackTrace);
////            }
////            catch (Exception)
////            {
////                InitializeCVODEInterface(model);
////                throw;
////            }
////        }


void CvodeInterface::AssignPendingEvents(const double& timeEnd, const double& tout)
{
    for (int i = assignments.size() - 1; i >= 0; i--)
    {
        if (timeEnd >= assignments[i].GetTime())
        {
            model->SetTime(tout);
            AssignResultsToModel();
            model->convertToConcentrations();
            model->updateDependentSpeciesValues(model->y);
            assignments[i].AssignToModel();

            if (mRR && !mRR->mConservedTotalChanged)
            {
                 model->computeConservedTotals();
            }
            model->convertToAmounts();
            vector<double> args = BuildEvalArgument();
            model->evalModel(timeEnd, args);
            reStart(timeEnd, model);
            assignments.erase(assignments.begin() + i);
        }
    }
}

//vector<int> CvodeInterface::RetestEvents(const double& timeEnd, vector<int>& handledEvents)
//{
//    return RetestEvents(timeEnd, handledEvents, false);
//}

vector<int> CvodeInterface::RetestEvents(const double& timeEnd, const vector<int>& handledEvents, vector<int>& removeEvents)
{
    return RetestEvents(timeEnd, handledEvents, false, removeEvents);
}

vector<int> CvodeInterface::RetestEvents(const double& timeEnd, vector<int>& handledEvents, const bool& assignOldState)
{
    vector<int> removeEvents;
    return RetestEvents(timeEnd, handledEvents, assignOldState, removeEvents);
}

vector<int> CvodeInterface::RetestEvents(const double& timeEnd, const vector<int>& handledEvents, const bool& assignOldState, vector<int>& removeEvents)
{
    vector<int> result;
//    vector<int> removeEvents;// = new vector<int>();    //Todo: this code was like this originally.. which removeEvents to use???

    if (mRR && !mRR->mConservedTotalChanged)
    {
        model->computeConservedTotals();
    }

    model->convertToAmounts();
    vector<double> args = BuildEvalArgument();
    model->evalModel(timeEnd, args);

    ModelState *oldState = new ModelState(*model);

    args = BuildEvalArgument();
    model->evalEvents(timeEnd, args);

    for (int i = 0; i < model->getNumEvents(); i++)
    {
        bool containsI = (std::find(handledEvents.begin(), handledEvents.end(), i) != handledEvents.end()) ? true : false;
        if (model->eventStatusArray[i] == true && oldState->mEventStatusArray[i] == false && !containsI)
        {
            result.push_back(i);
        }

        if (model->eventStatusArray[i] == false && oldState->mEventStatusArray[i] == true && !model->eventPersistentType[i])
        {
            removeEvents.push_back(i);
        }
    }

    if (assignOldState)
    {
        oldState->AssignToModel(*model);
    }

    delete oldState;
    return result;
}

////        private List<int> RetestEvents(double timeEnd, List<int> handledEvents, bool assignOldState, out List<int> removeEvents)
////        {
////            var result = new List<int>();
////            removeEvents = new List<int>();
////
////            if (!RoadRunner._bConservedTotalChanged) model.computeConservedTotals();
////            model.convertToAmounts();
////            model.evalModel(timeEnd, BuildEvalArgument());
////
////            var oldState = new ModelState(model);
////            model.evalEvents(timeEnd, BuildEvalArgument());
////
////            for (int i = 0; i < model.getNumEvents; i++)
////            {
////                if (model.eventStatusArray[i] == true && oldState.EventStatusArray[i] == false && !handledEvents.Contains(i))
////                    result.Add(i);
////                if (model.eventStatusArray[i] == false && oldState.EventStatusArray[i] == true && !model.eventPersistentType[i])
////                {
////                    removeEvents.Add(i);
////                }
////            }
////            if (assignOldState)
////                oldState.AssignToModel(model);
////
////            return result;
////        }
////
////


void CvodeInterface::HandleRootsFound(double &timeEnd, const double& tout)
{
    vector<int> rootsFound(model->getNumEvents());

    // Create some space for the CVGetRootInfo call
    _rootsFound = new int[model->getNumEvents()];
    CVGetRootInfo(cvodeMem, _rootsFound);    //This is a DLL Call.. Todo: implement..
    CopyCArrayToStdVector(_rootsFound, rootsFound, model->getNumEvents());
    delete [] _rootsFound;
    HandleRootsForTime(timeEnd, rootsFound);
}

void CvodeInterface::TestRootsAtInitialTime()
{
    vector<int> dummy;
    vector<int> events = RetestEvents(0, dummy, true); //Todo: dummy is passed but is not used..?
    if (events.size() > 0)
    {
        vector<int> rootsFound(model->getNumEvents());//         = new int[model->getNumEvents];
        vector<int>::iterator iter;
        for(iter = rootsFound.begin(); iter != rootsFound.end(); iter ++) //int item in events)
        {
            (*iter) = 1;
        }
        HandleRootsForTime(0, rootsFound);
    }
}

void CvodeInterface::RemovePendingAssignmentForIndex(const int& eventIndex)
{
    for (int j = assignments.size() - 1; j >= 0; j--)
    {
        if (assignments[j].GetIndex() == eventIndex)
        {
            assignments.erase(assignments.begin() + j);
        }
    }
}

void CvodeInterface::SortEventsByPriority(vector<Event>& firedEvents)
{
    if ((firedEvents.size() > 1))
    {
		Log(lDebug3)<<"Sorting event priorities";
        for(int i = 0; i < firedEvents.size(); i++)
        {
        	firedEvents[i].SetPriority(model->eventPriorities[firedEvents[i].GetID()]);
        	Log(lDebug3)<<firedEvents[i];
        }
        sort(firedEvents.begin(), firedEvents.end(), SortByPriority());

		Log(lDebug3)<<"After sorting event priorities";
        for(int i = 0; i < firedEvents.size(); i++)
        {
        	Log(lDebug3)<<firedEvents[i];
        }
    }
}

void CvodeInterface::SortEventsByPriority(vector<int>& firedEvents)
{
    if ((firedEvents.size() > 1))
    {
		model->computeEventPriorites();
        vector<Event> dummy;
        for(int i = 0; i < firedEvents.size(); i++)
        {
        	dummy.push_back(firedEvents[i]);
        }

		Log(lDebug3)<<"Sorting event priorities";
        for(int i = 0; i < firedEvents.size(); i++)
        {
        	dummy[i].SetPriority(model->eventPriorities[dummy[i].GetID()]);
        	Log(lDebug3)<<dummy[i];
        }
        sort(dummy.begin(), dummy.end(), SortByPriority());

        for(int i = 0; i < firedEvents.size(); i++)
        {
        	firedEvents[i] = dummy[i].GetID();
        }

		Log(lDebug3)<<"After sorting event priorities";
        for(int i = 0; i < firedEvents.size(); i++)
        {
        	Log(lDebug3)<<firedEvents[i];
        }
    }
}

////        private void SortEventsByPriority(List<int> firedEvents)
////        {
////            if ((firedEvents.Count > 1))
////            {
////                model.computeEventPriorites();
////                firedEvents.Sort(new Comparison<int>((index1, index2) =>
////                                                         {
////                                                             double priority1 = model.eventPriorities[index1];
////                                                             double priority2 = model.eventPriorities[index2];
////
////                                                             // random toss in case we have same priorites
////                                                             if (priority1 == priority2 && priority1 != 0 &&
////                                                                 index1 != index2)
////                                                             {
////                                                                 if (Random.NextDouble() > 0.5)
////                                                                     return -1;
////                                                                 else
////                                                                     return 1;
////                                                             }
////
////                                                             return -1 * priority1.CompareTo(priority2);
////                                                         }));
////            }
////        }


////        private void HandleRootsForTime(double timeEnd, int[] rootsFound)
////        {
////            AssignResultsToModel();
////            model.convertToConcentrations();
////            model.updateDependentSpeciesValues(model.y);
////
////            model.evalEvents(timeEnd, BuildEvalArgument());
////
////
////            var firedEvents = new List<int>();
////            var preComputedAssignments = new Dictionary<int, double[]>();
////
////
////            for (int i = 0; i < model.getNumEvents; i++)
////            {
////                // We only fire an event if we transition from false to true
////                if (rootsFound[i] == 1)
////                {
////                    if (model.eventStatusArray[i])
////                    {
////                        firedEvents.Add(i);
////                        if (model.eventType[i])
////                            preComputedAssignments[i] = model.computeEventAssignments[i]();
////                    }
////                }
////                else
////                {
////                    // if the trigger condition is not supposed to be persistent, remove the event from the firedEvents list;
////                    if (!model.eventPersistentType[i])
////                    {
////                        RemovePendingAssignmentForIndex(i);
////                    }
////                }
////            }
////            var handled = new List<int>();
////            while (firedEvents.Count > 0)
////            {
////                SortEventsByPriority(firedEvents);
////                // Call event assignment if the eventstatus flag for the particular event is false
////                for (int i = 0; i < firedEvents.Count; i++)
////                {
////                    var currentEvent = firedEvents[i];
////                    // We only fire an event if we transition from false to true
////
////                    model.previousEventStatusArray[currentEvent] = model.eventStatusArray[currentEvent];
////                    double eventDelay = model.eventDelay[currentEvent]();
////                    if (eventDelay == 0)
////                    {
////                        if (model.eventType[currentEvent] && preComputedAssignments.ContainsKey(currentEvent))
////                            model.performEventAssignments[currentEvent](preComputedAssignments[currentEvent]);
////                        else
////                            model.eventAssignments[currentEvent]();
////
////                        handled.Add(currentEvent);
////                        List<int> removeEvents;
////                        var additionalEvents = RetestEvents(timeEnd, handled, out removeEvents);
////                        firedEvents.AddRange(additionalEvents);
////
////                        foreach (var newEvent in additionalEvents)
////                        {
////                            if (model.eventType[newEvent])
////                                preComputedAssignments[newEvent] = model.computeEventAssignments[newEvent]();
////                        }
////
////                        model.eventStatusArray[currentEvent] = false;
////                        firedEvents.RemoveAt(i);
////
////                        foreach (var item in removeEvents)
////                        {
////                            if (firedEvents.Contains(item))
////                            {
////                                firedEvents.Remove(item);
////                                RemovePendingAssignmentForIndex(item);
////                            }
////                        }
////
////                        break;
////                    }
////                    else
////                    {
////                        if (!assignmentTimes.Contains(timeEnd + eventDelay))
////                            assignmentTimes.Add(timeEnd + eventDelay);
////
////
////                        var pending = new PendingAssignment(
////                                                                    timeEnd + eventDelay,
////                                                                    model.computeEventAssignments[currentEvent],
////                                                                    model.performEventAssignments[currentEvent],
////                                                                    model.eventType[currentEvent], currentEvent);
////
////                        if (model.eventType[currentEvent] && preComputedAssignments.ContainsKey(currentEvent))
////                            pending.ComputedValues = preComputedAssignments[currentEvent];
////
////                        assignments.Add(pending);
////                        model.eventStatusArray[currentEvent] = false;
////                        firedEvents.RemoveAt(i);
////                        break;
////                    }
////
////#if (PRINT_DEBUG)
////                            System.Diagnostics.Debug.WriteLine("time: " + model.time.ToString("F4") + " Event " + (i + 1).ToString());
////#endif
////                }
////            }
////
////            if (!RoadRunner._bConservedTotalChanged) model.computeConservedTotals();
////            model.convertToAmounts();
////
////
////            model.evalModel(timeEnd, BuildEvalArgument());
////            double[] dCurrentValues = model.GetCurrentValues();
////            for (int k = 0; k < numAdditionalRules; k++)
////                Cvode_SetVector(_amounts, k, dCurrentValues[k]);
////
////            for (int k = 0; k < numIndependentVariables; k++)
////                Cvode_SetVector(_amounts, k + numAdditionalRules, model.amounts[k]);
////
////            CVReInit(cvodeMem, timeEnd, _amounts, relTol, abstolArray);
////            assignmentTimes.Sort();
////        }
////

void CvodeInterface::HandleRootsForTime(const double& timeEnd, vector<int>& rootsFound)
{
    AssignResultsToModel();
    model->convertToConcentrations();
    model->updateDependentSpeciesValues(model->y);
    vector<double> args = BuildEvalArgument();
    model->evalEvents(timeEnd, args);

//    vector<Event> firedEvents;
    vector<int> firedEvents;
    map<int, double* > preComputedAssignments;

	for (int i = 0; i < model->getNumEvents(); i++)
    {
        // We only fire an event if we transition from false to true
        if (rootsFound[i] == 1)
        {
            if (model->eventStatusArray[i])
            {
                firedEvents.push_back(i);
                if (model->eventType[i])
                {
                    preComputedAssignments[i] = model->computeEventAssignments[i]();
                }
            }
        }
        else
        {
            // if the trigger condition is not supposed to be persistent, remove the event from the firedEvents list;
            if (!model->eventPersistentType[i])
            {
                RemovePendingAssignmentForIndex(i);
            }
        }
    }

    vector<int> handled;
    while (firedEvents.size() > 0)
    {
        SortEventsByPriority(firedEvents);
        // Call event assignment if the eventstatus flag for the particular event is false
        for (u_int i = 0; i < firedEvents.size(); i++)
        {
            int currentEvent = firedEvents[i];//.GetID();

            // We only fire an event if we transition from false to true
            model->previousEventStatusArray[currentEvent] = model->eventStatusArray[currentEvent];
            double eventDelay = model->eventDelays[currentEvent]();
            if (eventDelay == 0)
            {
                if (model->eventType[currentEvent] && preComputedAssignments.count(currentEvent) > 0)
                {
                    model->performEventAssignments[currentEvent](preComputedAssignments[currentEvent]);
                }
                else
                {
                    model->eventAssignments[currentEvent]();
                }

                handled.push_back(currentEvent);
                vector<int> removeEvents;
                vector<int> additionalEvents = RetestEvents(timeEnd, handled, removeEvents);

                std::copy (additionalEvents.begin(), additionalEvents.end(), firedEvents.end());

                for (int j = 0; j < additionalEvents.size(); j++)
                {
                    int newEvent = additionalEvents[j];
                    if (model->eventType[newEvent])
                    {
                        preComputedAssignments[newEvent] = model->computeEventAssignments[newEvent]();
                    }
                }

                model->eventStatusArray[currentEvent] = false;
                Log(lDebug3)<<"Fired Event with ID:"<<currentEvent;
                firedEvents.erase(firedEvents.begin() + i);

                for (int i = 0; i < removeEvents.size(); i++)
                {
                    int item = removeEvents[i];
                    if (find(firedEvents.begin(), firedEvents.end(), item) != firedEvents.end())
                    {
                        firedEvents.erase(find(firedEvents.begin(), firedEvents.end(), item));
                        RemovePendingAssignmentForIndex(item);
                    }
                }

                break;
            }
            else
            {
                if (find(assignmentTimes.begin(), assignmentTimes.end(), timeEnd + eventDelay) == assignmentTimes.end())
                {
                    assignmentTimes.push_back(timeEnd + eventDelay);
                }

                PendingAssignment *pending = new PendingAssignment(  timeEnd + eventDelay,
                                                                    model->computeEventAssignments[currentEvent],
                                                                    model->performEventAssignments[currentEvent],
                                                                    model->eventType[currentEvent],
                                                                    currentEvent);

                if (model->eventType[currentEvent] && preComputedAssignments.count(currentEvent) == 1)
                {
                    pending->ComputedValues = preComputedAssignments[currentEvent];
                }

                assignments.push_back(*pending);
                model->eventStatusArray[currentEvent] = false;
                firedEvents.erase(firedEvents.begin() + i);
                break;
            }
        }
    }

    if (mRR && !mRR->mConservedTotalChanged)
    {
        model->computeConservedTotals();
    }
    model->convertToAmounts();


    args = BuildEvalArgument();
    model->evalModel(timeEnd, args);

    vector<double> dCurrentValues = model->GetCurrentValues();
    for (int k = 0; k < numAdditionalRules; k++)
    {
        Cvode_SetVector((N_Vector) _amounts, k, dCurrentValues[k]);
    }

    for (int k = 0; k < numIndependentVariables; k++)
    {
        Cvode_SetVector((N_Vector) _amounts, k + numAdditionalRules, model->amounts[k]);
    }

    CVReInit(cvodeMem, timeEnd, _amounts, relTol, abstolArray);
    sort(assignmentTimes.begin(), assignmentTimes.end());
}

void CvodeInterface::AssignResultsToModel()
{
    model->updateDependentSpeciesValues(model->y);
    vector<double> dTemp(numAdditionalRules);

    for (int i = 0; i < numAdditionalRules; i++)
    {
        dTemp[i] = Cvode_GetVector((_generic_N_Vector*) _amounts, i);
        model->amounts[i] = dTemp[i];
    }

    for (int i = 0; i < numIndependentVariables; i++) //
    {
        double val = Cvode_GetVector((_generic_N_Vector*) _amounts, i + numAdditionalRules);
        model->amounts[i] = (val);
        Log(lDebug5)<<"Amount "<<setprecision(16)<<val;
    }

    vector<double> args = BuildEvalArgument();
    model->computeRules(args);
    model->AssignRates(dTemp);
    model->computeAllRatesOfChange();
}

// Restart the simulation using a different initial condition
void CvodeInterface::AssignNewVector(ModelFromC *oModel, bool bAssignNewTolerances)
{
    vector<double> dTemp = model->GetCurrentValues();
    double dMin = absTol;

    for (int i = 0; i < numAdditionalRules; i++)
    {
        if (dTemp[i] > 0 && dTemp[i]/1000. < dMin)
        {
            dMin = dTemp[i]/1000.0;
        }
    }

    for (int i = 0; i < numIndependentVariables; i++)
    {
        if (oModel->GetAmounts(i) > 0 && oModel->GetAmounts(i)/1000.0 < dMin)    //Todo: was calling oModel->amounts[i]  is this in fact GetAmountsForSpeciesNr(i) ??
        {
            dMin = oModel->amounts[i]/1000.0;
        }
    }

    for (int i = 0; i < numAdditionalRules; i++)
    {
        if (bAssignNewTolerances)
        {
            setAbsTolerance(i, dMin);
        }
        Cvode_SetVector(_amounts, i, dTemp[i]);
    }

    for (int i = 0; i < numIndependentVariables; i++)
    {
        if (bAssignNewTolerances)
        {
            setAbsTolerance(i + numAdditionalRules, dMin);
        }
        Cvode_SetVector(_amounts, i + numAdditionalRules, oModel->GetAmounts(i));
    }

    if (!HaveVariables() && model->getNumEvents() > 0)
    {
        if (bAssignNewTolerances)
        {
            setAbsTolerance(0, dMin);
        }
        Cvode_SetVector(_amounts, 0, 1.0);
    }

    if (bAssignNewTolerances)
    {
        Log(lDebug1)<<"Set tolerance to: "<<setprecision(16)<< dMin;
    }
}

void CvodeInterface::AssignNewVector(ModelFromC *model)
{
    AssignNewVector(model, false);
}

void CvodeInterface::setAbsTolerance(int index, double dValue)
{
    double dTolerance = dValue;
    if (dValue > 0 && absTol > dValue)
    {
        dTolerance = dValue;
    }
    else
    {
        dTolerance = absTol;
    }

    Cvode_SetVector(abstolArray, index, dTolerance);
}

int CvodeInterface::reStart(double timeStart, ModelFromC* model)
{
    AssignNewVector(model);
    SetInitStep(cvodeMem, InitStep);
    SetMinStep(cvodeMem, MinStep);
    SetMaxStep(cvodeMem, MaxStep);
    return CVReInit(cvodeMem, timeStart, _amounts, relTol, abstolArray);
}

////        double CvodeInterface::getValue(int index)
////        {
////            return (Cvode_GetVector(_amounts, index + numAdditionalRules));
////        }
////



vector<double> BuildEvalArgument(ModelFromC* model)
{
    vector<double> dResult;
    dResult.resize(*model->amountsSize + *model->rateRulesSize);

    vector<double> dCurrentValues = model->GetCurrentValues();
    for(int i = 0; i < dCurrentValues.size(); i++)
    {
        dResult[i] = dCurrentValues[i];
    }

    for(int i = 0; i < *model->amountsSize; i++)
    {
        dResult[i + *model->rateRulesSize] = model->amounts[i];
    }

    Log(lDebug4)<<"Size of dResult in BuildEvalArgument: "<<dResult.size();
    return dResult;

}

////        internal double[] BuildEvalArgument()
////        {
////            var dResult = new double[model.amounts.Length + model.rateRules.Length];
////            double[] dCurrentValues = model.GetCurrentValues();
////            dCurrentValues.CopyTo(dResult, 0);
////            model.amounts.CopyTo(dResult, model.rateRules.Length);
////            return dResult;
////        }

vector<double> CvodeInterface::BuildEvalArgument()
{
    vector<double> dResult;
    dResult.resize(*model->amountsSize + *model->rateRulesSize);

    vector<double> dCurrentValues = model->GetCurrentValues();
    for(int i = 0; i < dCurrentValues.size(); i++)
    {
        dResult[i] = dCurrentValues[i];
    }

    for(int i = 0; i < *model->amountsSize; i++)
    {
        dResult[i + *model->rateRulesSize] = model->amounts[i];
    }

    Log(lDebug4)<<"Size of dResult in BuildEvalArgument: "<<dResult.size();
    return dResult;
}

////        void CvodeInterface::release()
////        {
////            FreeCvode_Mem(cvodeMem);
////            FreeCvode_Vector(_amounts);
////            fileClose(fileHandle);
////        }
////
////        /// <summary>
////        /// Random number generator used to implement the random choosing
////        /// of event priorities.
////        /// <remarks>ReInitialize with specific seed in order to produce
////        /// repeatable runs.</remarks>
////        /// </summary>
////        Random CvodeInterface::Random { get; set; }
////    }
////}



} //namespace rr


