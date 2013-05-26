#include <stdio.h>
#include <iomanip>
#include "lmfit/lmmin.h"
#include "../../../../Wrappers/C/rrc_api.h"
#include "rrParameters.h"

using namespace rrc;
using namespace rr;
//Global roadrunner... because lm fit don't allow void* to be passed in...
RRHandle 	rrHandle;
Parameters  gParas;

/* Standard monitoring routine. */
void my_printout(	int n_par,
					const double *par,
                    int m_dat,
                    const void *data,
                    const double *fvec,
                    int printflags,
                    int iflag,
                    int iter,
                    int nfev);

double f(double time, char* specie, const double *p)
{
	//Get model value for specie at time t and parameters p

    //set model parameters, labels contained in gParas
    for(int i = 0; i < gParas.count(); i++)
    {
    	Parameter<double>* aPar = (Parameter<double>*) gParas[i];
    	setValue(rrHandle, aPar->getName().c_str(), p[i]);
		aPar->setValue(p[i]);
    }

    reset(rrHandle);
 	RRData* data;
    if(time != 0)
    {
		//propagate the model to time t
    	data = simulateEx(rrHandle, 0, time, 1);
    }

    //Get the values at current time...
	double value;
	getValue(rrHandle, specie, &value);
	return value;
}

/* data structure to transmit model data to function evalution */
typedef struct
{
	int			nrOfSpecies;
    double* 	time;
    char**  	speciesLabels;
    double**	specieVals;
    double (*f)( double time, char* specie, const double *p);
} data_struct;

/* function evaluation, determination of residues */
void rr_evaluate(const double *par,  //Parameter vector
				 int m_dat,          //Dimension of residue vector
                 const void *data,   //Data structure
                 double *fvec,       //residue vector..
                 int *info )         //
{
    data_struct *myData;
    myData = (data_struct*) data;

    //calculate fvec for each specie
   	for(int i = 0; i < myData->nrOfSpecies; i++)
    {
	    for (int j = 0; j < m_dat; j++ )
    	{
        	double modelValue = myData->f( myData->time[j], myData->speciesLabels[i], par);
			fvec[i] = myData->specieVals[i][j] - modelValue;
	    }
    }
}

int main()
{
	rrHandle = createRRInstanceEx("r:\\temp");
    setLogLevel("Debug");
    enableLoggingToConsole();

	//create 'experimental' data
    if(!loadSBMLFromFile(rrHandle, "r:\\models\\MinimizationTest_A.xml"))
    {
    	cerr<<"failed loading sbml...\n";
        cerr<<"Last error: "<<getLastError();
        return -1;
    }

    //Set selection list
    if(!setTimeCourseSelectionList(rrHandle, "time,s1,s2"))
    {
        cerr<<"Last error: "<<getLastError();
        return -1;
    }

    RRStringArray* list = getTimeCourseSelectionList(rrHandle);
	cout<<"Currrent selection is: "<< stringArrayToString(list)<<endl;

   	int numTimePoints = 11;
	//create 11 time points, time 0 -> 2
    RRData* rrData = simulateEx(rrHandle, 0, 2, numTimePoints);

	cout<<"\"Experimental Data\"\n"<<rrDataToString(rrData);

    //There are 3 k's (k1 == 1.2; k2 == 4.5; k3 == 3.40)
    int n_par = 3; 					// number of parameters in model function f
    double par[3] = { 4, 0, 1 }; 	// arbitrary starting value
//    double par[3] = { 1.2, 4.5, 3.4 }; 	// arbitrary starting value
	gParas.add(new Parameter<double>("k1", par[0]));
	gParas.add(new Parameter<double>("k2", par[1]));
	gParas.add(new Parameter<double>("k3", par[2]));
    lm_status_struct status;
    lm_control_struct control;

    /* data points */
    int m_dat = numTimePoints;
	int numSpecies = 2;
    //time

	//Setup the data structure for fitting
    data_struct data;// = { tx, tz, y, f };
    data.nrOfSpecies = numSpecies;
    data.time = new double[numTimePoints];
    data.specieVals = new double*[numSpecies];
    data.specieVals[0] = new double[numTimePoints];
    data.specieVals[1] = new double[numTimePoints];
    data.speciesLabels = new char*[numSpecies];
    data.speciesLabels[0] = "s1";
    data.speciesLabels[1] = "s2";
    data.f = f;

    for(int i = 0; i < m_dat; i++)
    {
    	//row*CSize + col
    	data.time[i] = rrData->Data[i*rrData->CSize + 0];
        data.specieVals[0][i] = rrData->Data[i*rrData->CSize + 1];
        data.specieVals[1][i] = rrData->Data[i*rrData->CSize + 2];
    }

    /* auxiliary parameters */
    control = lm_control_double;
    control.printflags = 3; // monitor status (+1) and parameters (+2)

    /* perform the fit */
    printf( "Fitting:\n" );
    lmmin( 	n_par,
    		par,
            m_dat,
            (const void*) &data,
            rr_evaluate,
            &control,
            &status,
            my_printout);

    /* print results */
    printf( "\nResults:\n" );
    printf( "status after %d function evaluations:\n  %s\n", status.nfev, lm_infmsg[status.info] );

    printf("obtained parameters:\n");

    for ( int i=0; i<n_par; ++i )
    {
		printf("  par[%i] = %12g\n", i, par[i]);
    }

    printf("obtained norm:\n  %12g\n", status.fnorm );
    printf("fitting data as follows:\n");


    data_struct *myData;
    myData = & data;

        for (int j=0; j < m_dat; ++j )
        {
		   	for(int i = 0; i < myData->nrOfSpecies; i++)
    		{
        		double modelValue = f(myData->time[j], myData->speciesLabels[i], par);

				cout<<"time "<<std::setw(10)<<myData->time[j]<<std::setw(10)<<
                "Exp "<<myData->speciesLabels[i]<<"="<<myData->specieVals[i][j]<<std::setw(10)<<
                "Fit: "<<myData->speciesLabels[i]<<"="<<modelValue<<std::setw(10)<<
                "\tresidue="<<myData->specieVals[i][j] - modelValue<<endl;
        	}
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
