#pragma hdrstop
#include "rrRoadRunnerData.h"
#include "rrLogger.h"
#include "lm_thread.h"
#include "rrNoise.h"
#include "lm.h"
#include "lmfit/lmmin.h"
#include "rrStringUtils.h"
#include "../../Wrappers/C/rrc_api.h"
#include "../../Wrappers/C/rrc_utilities.h"

//---------------------------------------------------------------------------
using namespace rr;
LMFitThread::LMFitThread(LM& host)
:
threadEnterCB(NULL),
threadExitCB(NULL),
mUserData(NULL),
mTheHost(host),
mMinData(mTheHost.getMinimizationData()),
mRRI(NULL)
{
	memset(&mLMData, 0, sizeof(lmDataStructure));
}

bool LMFitThread::isRuning()
{
	return mThread.isRunning();
}

void LMFitThread::assignCallBacks(ThreadCB fn1, ThreadCB fn2, void* userData)
{
	threadEnterCB 	= fn1;
	threadExitCB  	= fn2;
    mUserData 		= userData;
}

void LMFitThread::start()
{
    if(mThread.isRunning())
    {
    	Log(lError)<<"Tried to start an already working thread!";
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

    setupRoadRunner();

    StringList species = mMinData.getExperimentalDataSelectionList();
    Log(lInfo)<<"The following species are selected: "<<species.AsString();

    Parameters Paras =  mMinData.getParameters();
    Log(lInfo)<<"The following parameters are to be minimized";
    for(int i = 0; i < Paras.count(); i++)
    {
    	Log(lInfo)<<Paras[i]->getName()<<" with initial value: "<<Paras[i]->getValueAsString();
    }

    //Some parameters to the Algorithm..
    lm_status_struct status;
    lm_control_struct control = lm_control_double;
    control.printflags = 3;

    //Setup the input data structure
    setup();

    lmmin( 	mLMData.nrOfParameters,
    		mLMData.parameters,
            mLMData.nrOfResiduePoints,
            (const void*) &mLMData,
            evaluate,
            &control,
            &status,
            my_printout);

    /* print results */
    Log(lInfo)<<"Results:";
    Log(lInfo)<<"Status after "<<status.nfev<<" function evaluations: "<<lm_infmsg[status.info] ;
    Log(lInfo)<<"Obtained parameters:";

    for (int i = 0; i < mLMData.nrOfParameters; ++i)
    {
		Log(lInfo)<<"Parameter "<<mLMData.parameterLabels[i]<<" = "<< mLMData.parameters[i];
    }

    Log(lInfo)<<"Obtained norm:  "<<status.fnorm;

    //Populate minDataObject with data to 'report' back
    RoadRunnerData data = createModelData();
	mMinData.setModelData(data);

    data = createResidualsData();
	mMinData.setResidualsData(data);

	if(threadExitCB)
    {
		threadExitCB(mUserData);
    }
}

bool LMFitThread::setup()
{
	//Setup the minimization data structure
    StringList species 		= mMinData.getExperimentalDataSelectionList();   //Model data selection..
    mLMData.nrOfSpecies 	= species.Count();
    Parameters parameters 	= mMinData.getParameters();
    mLMData.nrOfParameters	= parameters.count();

    mLMData.parameters 				= new double[mLMData.nrOfParameters];
	//Set initial parameter values
    for(int i = 0; i < mLMData.nrOfParameters; i++)
    {
    	Parameter<double> *par = (Parameter<double>*) parameters[i];
        if(par)
        {
    		mLMData.parameters[i] = par->getValue();
        }
        else
        {
            throw("Bad stuff..");
        }
    }

    RoadRunnerData& obsData 		= mMinData.getObservedDataReference();
    mLMData.nrOfTimePoints			= obsData.rSize();
    mLMData.timeStart				= obsData.getTimeStart();
    mLMData.timeEnd					= obsData.getTimeEnd();
	mLMData.nrOfResiduePoints       = mLMData.nrOfSpecies * mLMData.nrOfTimePoints;
    mLMData.time 			    	= new double[mLMData.nrOfTimePoints];

    mLMData.experimentalData 	    = new double*[mLMData.nrOfSpecies];

    if(obsData.hasWeights())
    {
    	mLMData.experimentalDataWeights = new double*[mLMData.nrOfSpecies];
    }

    mLMData.speciesLabels 	    	= new char*[mLMData.nrOfSpecies];

    //Each species data points and label
    for (int i = 0; i < mLMData.nrOfSpecies; i++)
    {
	    mLMData.experimentalData[i]  = new double[mLMData.nrOfTimePoints];
	    mLMData.speciesLabels[i] 	 = createText(species[i]);

        if(obsData.hasWeights())
        {
            mLMData.experimentalDataWeights[i] = new double[mLMData.nrOfTimePoints];
        }
    }

    //Populate Experimental Data
    for (int i = 0; i < mLMData.nrOfSpecies; i++)
    {
    	for(int timePoint = 0; timePoint < mLMData.nrOfTimePoints; timePoint++)
        {
        	if(i == 0)
            {
	        	mLMData.time[timePoint] = obsData(timePoint, 0);
            }
			mLMData.experimentalData[i][timePoint] = obsData(timePoint, i + 1);
        }
    }

    //Populate weights..
    if(obsData.hasWeights())
    {
        for (int i = 0; i < mLMData.nrOfSpecies; i++)
        {
            for(int timePoint = 0; timePoint < mLMData.nrOfTimePoints; timePoint++)
            {
                mLMData.experimentalDataWeights[i][timePoint] = obsData.weight(timePoint, i + 1);
            }
        }
    }

    mLMData.parameterLabels       	= new char*[mLMData.nrOfParameters];
    for (int i = 0; i < mLMData.nrOfParameters; i++)
    {
	    mLMData.parameterLabels[i] 	= createText(parameters[i]->getName());
    }

	mLMData.rrHandle				= mRRI;
    mRRI->setTimeCourseSelectionList(species.AsString());
}

bool LMFitThread::setupRoadRunner()
{
   	if(mRRI)
    {
    	delete mRRI;
    }

    mRRI = new RoadRunner;
	mRRI->loadSBML(mTheHost.mSBML.getValue(), false);
    mRRI->setTimeCourseSelectionList(mMinData.getExperimentalDataSelectionList());
    return true;
}

/* function evaluation, determination of residues */
void evaluate(const double   *par,  		//Parameter vector
			  int 		  	m_dat,  	//Dimension of residue vector
              const void     *userData,  	//Data structure
              double 	   	   *fvec,   	//residue vector..
              int 		   *infoIndex   //Index into info message array
)
{
    lmDataStructure *myData = (lmDataStructure*) userData;
   	reset(myData->rrHandle);

    for(int i = 0; i < myData->nrOfParameters; i++)
    {
    	setValue(myData->rrHandle, myData->parameterLabels[i], par[i]);
        Log(lDebug)<<"k"<<i<<" = "<<par[i]<<endl;
    }

    RRData* rrData = simulateEx(myData->rrHandle, myData->timeStart, myData->timeEnd, myData->nrOfTimePoints);

    if(!rrData)
    {
    	char* lastError = getLastError();
        Log(lError)<<"Error in simulateEx: "<<lastError;
        freeText(lastError);
        return;
    }
    //calculate fvec for each specie
    int count = 0;
   	for(int i = 0; i < myData->nrOfSpecies; i++)
    {
    	fvec[count] = 0;
	    for(int j = 0; j < myData->nrOfTimePoints; j++ )
    	{
            double modelValue;
            if(!getRRDataElement(rrData, j, i, &modelValue))
            {
            	throw("Bad stuff...") ;
            }

            if(!isNaN(myData->experimentalData[i][j]))
            {
				fvec[count] = myData->experimentalData[i][j] - modelValue;


                if(myData->experimentalDataWeights != NULL)
                {
                    if(myData->experimentalDataWeights[i][j] != 0) //Cause the first column is time... :( loks ugly
                    {
                        double weight =  myData->experimentalDataWeights[i][j];
                        fvec[count] = fvec[count] / weight;
                    }
                }
            }
            else
            {
            	fvec[count] = 0;
            }
           	count++;
	    }
    }
    freeRRData(rrData);
}

RoadRunnerData LMFitThread::createModelData()
{
	//We now have the parameters
    RoadRunnerData& modData = mMinData.getModelDataReference();
    StringList selList("time");
    selList.Append(mMinData.getModelDataSelectionList());

    modData.reSize(mLMData.nrOfTimePoints, selList.Count());
    mRRI->reset();
	mRRI->setTimeCourseSelectionList(selList);

    for(int i = 0; i < mLMData.nrOfParameters; i++)
    {
    	mRRI->setValue(mLMData.parameterLabels[i], mLMData.parameters[i]);
    }

    if(mRRI->simulate2Ex(mLMData.timeStart, mLMData.timeEnd, mLMData.nrOfTimePoints))
    {
	     modData = mRRI->getSimulationResult();
    }
	return modData;
}

RoadRunnerData LMFitThread::createResidualsData()
{
	//We now have the parameters
    RoadRunnerData& modData = mMinData.getModelDataReference();
    RoadRunnerData& resData = mMinData.getResidualsDataReference();
    RoadRunnerData& obsData = mMinData.getObservedDataReference();

    resData.reSize(modData.rSize(), modData.cSize());

	for(int sel = 0; sel < mLMData.nrOfSpecies + 1; sel++)	//selection 1 becuase of time column..
    {
    	for(int i = 0; i < mLMData.nrOfTimePoints; i++)
        {
        	if(sel == 0)
            {
        		resData(i, sel) = modData(i, sel);	//Time
            }
            else
            {
            	//The modData may contain data for other species than that was fitted..
                //We need to find out what coulmn correspond to what..
                string specie = obsData.getColumnName(sel);
                int colNr = modData.getColumnIndex(specie);
                if(colNr != -1)
                {
            		resData(i,sel) = obsData(i, sel) - modData(i, colNr);
                }
                else
                {
                	Log(lError)<<"Problem with column names when creating residual data!";
                }
            }
        }
    }

	return resData;
}


