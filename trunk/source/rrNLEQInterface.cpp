#ifdef USE_PCH
#include "rr_pch.h"
#endif
#pragma hdrstop
#include <math.h>
#include "rrException.h"
#include "rrNLEQInterface.h"
#include "rrModelFromC.h"
#include "rrStringUtils.h"
#include "rrUtils.h"
#include "rrRoadRunner.h"
#include "nleq/nleq1.h"
#include "rrLogger.h"
#include "rrUtils.h"
//---------------------------------------------------------------------------

namespace rr
{

string ErrorForStatus(const int& error);
ModelFromC* NLEQInterface::model = NULL;     // Model generated from the SBML
long		NLEQInterface::n	 = 0;

long  NLEQInterface::GetN()
{
	return NLEQInterface::n;
}

ModelFromC* NLEQInterface::GetModel()
{
    return NLEQInterface::model;
}

NLEQInterface::NLEQInterface(ModelFromC *_model)
:
nOpts(50),
defaultMaxInterations(100),
maxIterations(defaultMaxInterations),
defaultTolerance(1.e-4),
relativeTolerance(defaultTolerance)
{
	model = _model;
    n = model->getNumIndependentVariables();

    // Allocate space, see NLEQ docs for details
    LWRK = (n + 2 + 15)*n + 61;
    LIWK = n + 52;

    XScal = new double[n];
    // Set up default scaling factors
    for (int i = 0; i < n; i++)
    {
        XScal[i] = 1.0;
    }

    iopt = new long[nOpts];
    for (int i = 0; i < nOpts; i++)
    {
        iopt[i] = 0;
    }

    // Set for Highly nonlinear problem
    iopt[31 - 1] = 4;

    // Initialise all array elements to 0.0
    IWK = new long[LIWK];
    for (int i = 0; i < LIWK; i++)
    {
        IWK[i] = 0;
    }
    IWK[31 - 1] = maxIterations; // Max iterations


    RWK = new double[LWRK];
    for (int i = 0; i < LWRK; i++)
    {
        RWK[i] = 0.0;
    }

    RWK[22 - 1] = 1E-16; // Minimal allowed damping factor
}

bool NLEQInterface::IsAvailable()
{
    NLEQInterface *temp= new NLEQInterface(NULL);
    if(temp)
    {
        bool val = temp->getNumberOfModelEvaluations() ? true : false;
        delete temp;
        return val;
    }

    return false;
}

double NLEQInterface::solve(const vector<double>& yin)
{
    if (yin.size() == 0)
    {
        return 0;
    }

    // Set up a dummy Jacobian, actual Jacobian is computed
    // by NLEQ using finite differences
	//    double* Jacobian = new double[1];

    ierr = 0;
    IWK[31 - 1] = maxIterations; // Max iterations

    // Set up default scaling factors
    for (int i = 0; i < n; i++)
    {
        XScal[i] = 1.0;
    }

    for (int i = 0; i < nOpts; i++)
    {
        iopt[i] = 0;
    }

    iopt[31 - 1] = 3; // Set for Highly nonlinear problem

    // Initialise all array elements to 0.0
    for (int i = 0; i < LIWK; i++)
    {
        IWK[i] = 0;
    }

    IWK[31 - 1] = maxIterations; // Max iterations
    for (int i = 0; i < LWRK; i++)
    {
        RWK[i] = 0.0;
    }

    RWK[22 - 1] = 1E-20; // Minimal allowed damping factor

    // For some reason NLEQ modifies the tolerance value, use a local copy instead
    double tmpTol = relativeTolerance;

    //NLEQ1(ref n, fcn, null, model->amounts, XScal, ref tmpTol, iopt, ref ierr, ref LIWK, IWK, ref LWRK, RWK);

    NLEQ1( &n, 				&ModelFunction, NULL,
           model->amounts,	XScal,        	&tmpTol,
           iopt,           	&ierr,          &LIWK,
           IWK,           	&LWRK,          RWK);

    if (ierr == 2) // retry
    {
        for (int i = 0; i < nOpts; i++)
        {
            iopt[i] = 0;
        }

        iopt[31 - 1] = 3; // Set for Highly nonlinear problem
        iopt[0] = 1; // Try again but tell NLEQ not to reinitialize
        tmpTol = relativeTolerance;
//        NLEQ1(  &n,
//                &NLEQModelFcn,
//                NULL,//Jacobian,
//                model->amounts,
//                XScal,
//                &tmpTol,
//                iopt,
//                &ierr,
//                &LIWK,
//                IWK,
//                &LWRK,
//                RWK);
                // If we get the same error then give up
    }


    if(ierr > 0 )
    {
    	string err = ErrorForStatus(ierr);
        Log(lError)<<"Error :"<<err;
        throw RRException(err);
    }

    return ComputeSumsOfSquares();
}

void ModelFunction(int* nx, double* y, double* fval, int* pErr)
{
    ModelFromC* model = NLEQInterface::GetModel();
    if (model == NULL)
    {
        return;
    }

    try
    {
    	long n = NLEQInterface::GetN();
		for(long i = 0; i < n; i++)
        {
        	model->amounts[i] = y[i];
        }

        int size = *model->amountsSize + *model->rateRulesSize;
        vector<double> dTemp;
        dTemp.resize(size);

		for(int i = 0; i < *model->rateRulesSize; i++)
        {
        	dTemp[i] = model->rateRules[i];
        }

        for(int i = *model->rateRulesSize; i < *model->amountsSize + *model->rateRulesSize; i++)
        {
        	dTemp[i] = model->amounts[i];
        }

        model->evalModel(0.0, dTemp);

		for(int i = 0; i < n; i++)
        {
        	fval[i] = model->dydt[i];
        }

        pErr = 0;
    }
    catch (const Exception& ex)
    {
    	throw(ex);	//catch at a higher level
    }
}

////        private void ModelFcn(IntPtr nx, IntPtr y, IntPtr fval, IntPtr pErr)
////        {
////            if (model == null)
////            {
////                var temp = new double[n];
////                Marshal.Copy(temp, 0, fval, n);
////                Marshal.WriteInt32(pErr, 0);
////                return;
////            }
////
////
////            try
////            {
////                Marshal.Copy(y, model.amounts, 0, n);
////                var dTemp = new double[model.amounts.Length + model.rateRules.Length];
////                model.rateRules.CopyTo(dTemp, 0);
////                model.amounts.CopyTo(dTemp, model.rateRules.Length);
////                model.evalModel(0.0, dTemp);
////                //                bool bError = false;
////
////                //                for (int i = 0; i < model.amounts.Length; i++)
////                //                    if (model.amounts[i] < 0)
////                //                    {
////                //                        bError = true;
////                //                        break;
////                //                    }
////                //
////
////                Marshal.Copy(model.dydt, 0, fval, n);
////                //                if (bError)
////                //                    Marshal.WriteInt32(pErr, -1);
////                //                else
////                Marshal.WriteInt32(pErr, 0);
////            }
////            catch (Exception)
////            {
////            }
////        }
////

////        // NLEQ2 seems to have problems with some models so we drop back to NLEQ1 for now.
////
////        //        [DllImport ("nleq2", EntryPoint="NLEQ2", ExactSpelling=false,
////        //             CharSet=CharSet.Unicode, SetLastError=true, CallingConvention=CallingConvention.Cdecl
////        //             )]
////        [DllImport("NleqLib", EntryPoint = "NLEQ1")]
////        //         NLEQ is a FORTRAN routine, therefore everything must be a reference
////        public static extern IntPtr NLEQ1(
////            ref int n,
////            TCallBackModelFcn fcn,
////            [In, Out] double[,] Jacobian,
////            [In, Out] double[] x,
////            [In, Out] double[] xscal,
////            ref double rtol,
////            [In, Out] int[] iopt,
////            ref int ierr,
////            ref int LIWK,
////            [In, Out] int[] IWK,
////            ref int LRWK,
////            [In, Out] double[] RWK);
////
////        //                [DllImport ("nleq2", EntryPoint="NLEQ2")                     ]
////        ////         NLEQ is a FORTRAN routine, therefore everything must be a reference
////        //        public static extern IntPtr NLEQ2(
////        //            ref int n,
////        //            TCallBackModelFcn fcn,
////        //            [In, Out] double[,] Jacobian,
////        //            [In, Out] double[] x,
////        //            [In, Out] double[] xscal,
////        //            ref double rtol,
////        //            [In, Out] int[] iopt,
////        //            ref int ierr,
////        //            ref int LIWK,
////        //            [In, Out] int[] IWK,
////        //            ref int LRWK,
////        //            [In, Out] double[] RWK);

void NLEQInterface::setScalingFactors(const vector<double>& sx)
{
    for (int i = 0; i < n; i++)
    {
        XScal[i] = sx[i];
    }
}

int NLEQInterface::getNumberOfNewtonIterations()
{
    return IWK[0];
}

int NLEQInterface::getNumberOfCorrectorSteps()
{
    return IWK[2];
}

int NLEQInterface::getNumberOfModelEvaluations()
{
    return IWK[3];
}

int NLEQInterface::getNumberOfJacobianEvaluations()
{
    return IWK[4];
}

int NLEQInterface::getNumberOfModelEvaluationsForJacobian()
{
    return IWK[7];
}

bool NLEQInterface::Test(const string& fileName)
{
    RoadRunner *rr = new RoadRunner();
    if(!rr)
    {
        Log(lError)<<"Failed to create a RoadRunner object in NLEQInterface::Test function";
        return false;
    }

    if(rr->loadSBMLFromFile(fileName))
    {
        Log(lDebug)<<"Steady State Test:"<< rr->steadyState();
        return true;
    }
    else
    {
        Log(lDebug)<<"Failed loading SBMLin NLEQInterface::Test:";
        return false;
    }
}

string ErrorForStatus(const int& error)
{
        switch (error)
        {
            case 1:     return ("Jacobian matrix singular in NLEQ");
            case 2:     return ("Maximum iterations exceeded");
            case 3:     return ("Damping factor has became to small to continue");
            case 4:     return ("Warning: Superlinear or quadratic convergence slowed down near the solution");
            case 5:     return ("Warning: Error Tolerance reached but solution is suspect");
            case 10:    return ("Integer or real workspace too small in NLEQ");
            case 20:    return ("Bad input to size of model parameter");
            case 21:    return ("Nonpositive value for RTOL supplied to NLEQ");
            case 22:    return ("Negative scaling value via vector XSCAL supplied");
            case 30:    return ("One or more fields specified in IOPT are invalid (NLEQ)");
            case 80:    return ("Error signalled by linear solver routine N1FACT, in NLEQ");
            case 81:    return ("Error signalled by linear solver routine N1SOLV, in NLEQ");
            case 82:    return ("Possible negative concentrations in solution (NLEQ)");
            case 83:    return ("Error signalled by user routine JAC in NLEQ");
            default:    return (Format("Unknown error in NLEQ, errCode = {0}", error));
        }   
}

double NLEQInterface::ComputeSumsOfSquares()
{
    // Compute the sums of squares and return value to caller
    vector<double> dTemp;// = new double[model->amounts.Length + model->rateRules.Length];
//    dTemp.resize(model->amounts.size() + model->rateRules.size());

    //    dTemp = model->rateRules;//model->rateRules.CopyTo(dTemp, 0);
    CopyCArrayToStdVector(model->rateRules,   dTemp, (*model->rateRulesSize));//model->rateRules.CopyTo(dTemp, 0);
    //model->amounts.CopyTo(dTemp, model->rateRules.Length);
//    for(int i = 0; i < model->amounts.size(); i++)
    for(int i = 0; i < model->getNumIndependentVariables(); i++)
    {
        dTemp.push_back(model->amounts[i]);
    }

    model->evalModel(0.0, dTemp);
    double sum = 0;
    for (int i = 0; i < n; i++)
    {
        sum = sum + pow(model->dydt[i], 2.0);
    }
    return sqrt(sum);
}

}//end of namespace
