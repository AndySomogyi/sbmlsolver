#pragma hdrstop
#include "rrRoadRunnerData.h"
#include "rrLogger.h"
#include "lm_thread.h"
#include "rrNoise.h"
#include "lm.h"
#include "lmfit-3.5/lib/lmcurve.h"
#include "../../Wrappers/C/rrc_api.h"
#include "../../Wrappers/C/rrc_support.h"

//---------------------------------------------------------------------------
using namespace rr;
LMFitThread::LMFitThread(LM& host)
:
threadEnterCB(NULL),
threadExitCB(NULL),
mUserData(NULL),
mTheHost(host),
mResult(mTheHost.getMinimizationData())
{}

void LMFitThread::assignCallBacks(ThreadCB fn1, ThreadCB fn2, void* userData)
{
	threadEnterCB 	= fn1;
	threadExitCB  	= fn2;
    mUserData 		= userData;
}

void LMFitThread::start(RoadRunnerData* inputData)
{
//	mInputData = inputData;

    if(mThread.isRunning())
    {
    	Log(lError)<<"Tried to start a working thread!";
        return;
    }

	mThread.start(*this);	//Will spawn the run function below in a thread
}

double f(double t, const double* paras);
void LMFitThread::run()
{
	if(threadEnterCB)
    {
		threadEnterCB(mUserData);	//Tell anyone who wants to know
    }

//    RoadRunnerData& inputData = *(mInputData);

	//Create data vector appropriate for lmcurve_fit
    int n_par = 1;
    double par[1] = {-1};
//    int count = inputData.rSize();
//    double *x = new double[count];
//    double *y = new double[count];
//
//    //populate x and y
//    for(int i = 0; i < count; i++)
//    {
//    	x[i] = inputData(i,0);
//    	y[i] = inputData(i,1);
//    }
//
//    //Aux parameters
//    lm_status_struct status;
//    lm_control_struct control = lm_control_double;
//    control.printflags = 3;
//
//   	if(rri)
//    {
//    	delete rri;
//    }
//
//    rri = new RoadRunner;
//    rri->loadSBML(mTheHost.mSBML.getValue(), false);
//
//    lmcurve_fit(	n_par,
//    				par,
//                    count,
//                    x,
//                    y,
//                    f,
//                    &control,
//                    &status
//    			);
//
//
//    delete rri;
	if(threadExitCB)
    {
		threadExitCB(mUserData);
    }
}

double f(double time, const double* paras)
{
	//set model parameters, contained in paras

	// propagate the model to time t
    //


    //Get the values at current time...
	vector<SelectionRecord> sel = rri->getSelectionList();

    //for each specie, sum up current values
    double value = 0;
    for(int i = 0; i < sel.size(); i++)
    {
    	value += rri->getValueForRecord(sel[i]);
    }

    return value;
}


