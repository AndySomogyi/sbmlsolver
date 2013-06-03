#include <stdio.h>
#include <iomanip>
#include "lmfit/lmmin.h"
#include "../../../../Wrappers/C/rrc_api.h"
#include "rrParameters.h"

using namespace rrc;
using namespace rr;

/* Standard monitoring routine. */
void my_printout(int n_par,
				 const double *par,
                 int m_dat,
                 const void *data,
                 const double *fvec,
                 int printflags,
                 int iflag,
                 int iter,
                 int nfev);


/* data structure to transmit model data to function evalution */
typedef struct
{
	int			            nrOfSpecies;
	int			            nrOfParameters;			//Number of parameters to fit
    int 		            nrOfTimePoints;
    int						nrOfResiduePoints;		//This is the number "m_dat", typically nrOfSpecies * nrOfTimePoints
    double*					parameters;				//Vector holding the numbers for each parameter
    char**  	            speciesLabels;
    char**  	            parameterLabels;    	//Labels for the parameters to fit, eg k1, k2 etc
    double**	            experimentalData;		//Species experimental time data
	double		            timeStart;
	double		            timeEnd;
    RRHandle				rrHandle;
    double* 	            time;
//    double (*f)( double time, char* specie, const double *p);
} RRLMMinimizationStructure;

////Get model value for specie at time t and parameters p
double f(double time, char* specie, RRLMMinimizationStructure *ms)
{
    //set model parameters, labels contained in gParas
    for(int i = 0; i < ms->nrOfParameters; i++)
    {
    	setValue(ms->rrHandle, ms->parameterLabels[i], ms->parameters[i]);
    }

    reset(ms->rrHandle);
 	RRData* data;
    if(time != 0)
    {
		//propagate the model to time t
    	data = simulateEx(ms->rrHandle, 0, time, 1);
    }

    //Get the values at current time...
	double value;
	getValue(ms->rrHandle, specie, &value);
	return value;
}


/* function evaluation, determination of residues */
void rr_evaluate(
					const double   *par,  		//Parameter vector
				 	int 		  	m_dat,  	//Dimension of residue vector
                 	const void     *userData,  	//Data structure
                 	double 	   	   *fvec,   	//residue vector..
                 	int 		   *infoIndex   //Index into info message array
                 )
{

    RRLMMinimizationStructure *myData = (RRLMMinimizationStructure*) userData;
   	reset(myData->rrHandle);

    for(int i = 0; i < myData->nrOfParameters; i++)
    {
    	setValue(myData->rrHandle, myData->parameterLabels[i], par[i]);
        cout<<"k"<<i<<" = "<<par[i]<<endl;
    }

    RRData* rrData = simulateEx(myData->rrHandle, myData->timeStart, myData->timeEnd, myData->nrOfTimePoints);

    //calculate fvec for each specie
    int count = 0;
   	for(int i = 0; i < myData->nrOfSpecies; i++)
    {
    	fvec[count] = 0;
	    for (int j = 0; j < myData->nrOfTimePoints; j++ )
    	{
            double modelValue;
            if(!getResultElement(rrData, j, i + 1, &modelValue))
            {
            	exit(-1) ;
            }

			fvec[count] = myData->experimentalData[i][j] - modelValue;
            count++;
	    }
    }
    freeRRData(rrData);
}

int main()
{
	RRHandle rrHandle = createRRInstanceEx("r:\\temp");
    setLogLevel("Info");
    enableLoggingToConsole();

	//create 'experimental' data
    if(!loadSBMLFromFile(rrHandle, "r:\\models\\MinimizationTest_A.xml"))
    {
    	cerr<<"failed loading sbml...\n";
        cerr<<"Last error: "<<getLastError();
        return -1;
    }

    //Set selection list
    if(!setTimeCourseSelectionList(rrHandle, "time, s1, s2"))
    {
        cerr<<"Last error: "<<getLastError();
        return -1;
    }

    RRStringArray* list = getTimeCourseSelectionList(rrHandle);
	cout<<"Currrent selection is: "<< stringArrayToString(list)<<endl;

    lm_status_struct status;
    lm_control_struct control;


	//Setup the minimization data structure
    RRLMMinimizationStructure 		myData;
    myData.nrOfSpecies 	    		= 2;
    myData.nrOfParameters			= 3;

    //There are 3 k's (k1 == 1.2; k2 == 4.5; k3 == 3.40)
    myData.parameters 				= new double[myData.nrOfParameters];// = { .1, 12, -3 }; 		// arbitrary starting value
               myData.parameters[0] = .1;
    	       myData.parameters[1] = 1;
    	       myData.parameters[2] = .1;

    myData.nrOfTimePoints			= 500;
    myData.timeStart				= 0;
    myData.timeEnd					= 5;//79; //Something happens at time 2.8 ??
	myData.nrOfResiduePoints        = myData.nrOfSpecies * myData.nrOfTimePoints;
    myData.time 			    	= new double[myData.nrOfTimePoints];
    myData.experimentalData 	    = new double*[myData.nrOfSpecies];
    myData.experimentalData[0] 		= new double[myData.nrOfTimePoints];
    myData.experimentalData[1] 		= new double[myData.nrOfTimePoints];
    myData.speciesLabels 	    	= new char*[myData.nrOfSpecies];
    myData.parameterLabels       	= new char*[myData.nrOfParameters];
    myData.parameterLabels[0]		= "k1";
  	myData.parameterLabels[1]		= "k2";
    myData.parameterLabels[2]		= "k3";
    myData.speciesLabels[0] 		= "s1";
    myData.speciesLabels[1] 		= "s2";

	myData.rrHandle					= rrHandle;

	//create 'experimental' data points, time 0 -> 2

    RRData* rrData = simulateEx(rrHandle, myData.timeStart, myData.timeEnd, myData.nrOfTimePoints);
    cout<<"\"Experimental Data\"\n"<<rrDataToString(rrData);

    for(int i = 0; i < myData.nrOfTimePoints; i++)
    {
        double val;
        getResultElement(rrData, i, 0, &val);
        myData.time[i] = val;
        getResultElement(rrData, i, 1, &val);
        myData.experimentalData[0][i] = val;
        getResultElement(rrData, i, 2, &val);
        myData.experimentalData[1][i] = val;
    }
    freeRRData(rrData);

    /* auxiliary parameters */
    control 			= lm_control_double;	//Predefined
    control.printflags 	= 3; // monitor status (+1) and parameters (+2)

    /* perform the fit */
    printf( "Fitting:\n" );
    lmmin( 	myData.nrOfParameters,
    		myData.parameters,
            myData.nrOfResiduePoints,
            (const void*) &myData,
            rr_evaluate,
            &control,
            &status,
            my_printout);

    /* print results */
    printf("\nResults:\n" );
    printf("status after %d function evaluations:\n  %s\n", status.nfev, lm_infmsg[status.info] );
    printf("obtained parameters:\n");

    for ( int i=0; i< myData.nrOfParameters; ++i )
    {
		printf("  par[%i] = %12g\n", i, myData.parameters[i]);
    }

    printf("obtained norm:\n  %12g\n", status.fnorm );
    printf("fitting data as follows:\n");

    for (int j=0; j < myData.nrOfResiduePoints; ++j )
    {
        for(int i = 0; i < myData.nrOfSpecies; i++)
        {
            double modelValue = f(myData.time[j], myData.speciesLabels[i], &myData);

            cout<<"time "<<std::setw(10)<<myData.time[j]<<std::setw(10)<<
            "Exp "<<myData.speciesLabels[i]<<"="<<myData.experimentalData[i][j]<<std::setw(10)<<
            "Fit: "<<myData.speciesLabels[i]<<"="<<modelValue<<std::setw(10)<<
            "\tresidue="<<myData.experimentalData[i][j] - modelValue<<endl;
        }
    }

	ofstream fOut("lm.dat");
    fOut<<"time\t"<<myData.speciesLabels[0]<<"\t"<<myData.speciesLabels[1]<<"\ts1_mod\ts2_mod\ts1_res\ts2_res\n";
    for (int j=0; j < myData.nrOfTimePoints; ++j )
    {
            double modelValue1 = f(myData.time[j], myData.speciesLabels[0], &myData);
            double modelValue2 = f(myData.time[j], myData.speciesLabels[1], &myData);

            fOut<<myData.time[j]<<std::setw(20)<<
            myData.experimentalData[0][j]<<std::setw(20)<<
            myData.experimentalData[1][j]<<std::setw(20)<<
            modelValue1<<std::setw(20)<<
            modelValue2<<std::setw(20)<<
            myData.experimentalData[0][j] - modelValue1<<std::setw(20)<<
            myData.experimentalData[1][j] - modelValue2<<endl;
    }

    return 0;
}

void my_printout( 	int n_par,
					const double *par,
                    int m_dat,
                    const void *data,   //data  : for soft control of printout behaviour, add control variables to the data struct
                    const double *fvec,
                    int printflags,
                    int iflag,          //iflag : 0 (init) 1 (outer loop) 2(inner loop) -1(terminated)
                    int iter,           //iter  : outer loop counter
                    int nfev)			//nfev  : number of calls to *evaluate
{
    int i;

    if( !printflags )
        return;

    if( printflags & 1 )
    {
        /* location of printout call within lmdif */
        if (iflag == 2)
        {
            printf("trying step in gradient direction  ");
        }
        else if (iflag == 1)
        {
            printf("determining gradient (iteration %2d)", iter);
        }
        else if (iflag == 0)
        {
            printf("starting minimization              ");
        }
        else if (iflag == -1)
        {
            printf("terminated after %3d evaluations   ", nfev);
        }
    }

    if( printflags & 2 )
    {
        printf("  par: ");
        for (i = 0; i < n_par; ++i)
        {
            printf(" %18.11g", par[i]);
        }
        printf(" => norm: %18.11g", lm_enorm(m_dat, fvec));
    }

    if( printflags & 3 )
    {
        printf( "\n" );
    }

    if ( (printflags & 8) || ((printflags & 4) && iflag == -1) )
    {
        printf("  residuals:\n");
        for (i = 0; i < m_dat; ++i)
        {
        	printf("    fvec[%2d]=%12g\n", i, fvec[i] );
        }
    }
}
#if defined(CG_IDE)
#pragma comment(lib, "rrc_api.lib")
#pragma comment(lib, "roadrunner.lib")
#pragma comment(lib, "lmfit-static.lib")
#endif
