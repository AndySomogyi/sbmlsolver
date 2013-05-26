#ifndef lmUtilsH
#define lmUtilsH
#include "../../Wrappers/C/rrc_types.h"
//---------------------------------------------------------------------------

/* data structure to transmit model data to function evalution */
typedef struct
{
	int			            nrOfSpecies;
	int			            nrOfParameters;				//Number of parameters to fit
    int 		            nrOfTimePoints;
    int						nrOfResiduePoints;			//This is the number "m_dat", typically nrOfSpecies * nrOfTimePoints
    double*					parameters;					//Vector holding the numbers for each parameter
    char**  	            speciesLabels;
    char**  	            parameterLabels;    		//Labels for the parameters to fit, eg k1, k2 etc
    double**	            experimentalData;			//Species experimental time data
    double**	            experimentalDataWeights;	//experimental weights..
	double		            timeStart;
	double		            timeEnd;
    rrc::RRHandle	 		rrHandle;
    double* 	            time;
//    double (*f)( double time, char* specie, const double *p);
} lmDataStructure;

void my_printout( 	int n_par,
					const double *par,
                    int m_dat,
                    const void *data,   //data  : for soft control of printout behaviour, add control variables to the data struct
                    const double *fvec,
                    int printflags,
                    int iflag,          //iflag : 0 (init) 1 (outer loop) 2(inner loop) -1(terminated)
                    int iter,           //iter  : outer loop counter
                    int nfev);			//nfev  : number of calls to *evaluate


//Evaluate is the callback function that lmfit call to evaulate residuals
void evaluate(   	const double   *par,  		//Parameter vector
				 	int 		  	m_dat,  	//Dimension of residue vector
                 	const void     *userData,  	//Data structure
                 	double 	   	   *fvec,   	//residue vector..
                 	int 		   *infoIndex   //Index into info message array
                 );

#endif
