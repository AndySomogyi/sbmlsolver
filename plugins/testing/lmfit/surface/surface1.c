/*
 * Project:  LevenbergMarquardtLeastSquaresFitting
 * File:     surface1.c
 * Contents: Example for fitting data y_i(t_i) by a function f(t;p),
 *           where each t_i is a vector of dimension k=2.
 * Author:   Joachim Wuttke 2010, following a suggestion by Mario Rudolphi
 * Homepage: joachimwuttke.de/lmfit
 * Licence:  I don't care
 */

#include <stdio.h>
#include "lmfit/lmmin.h"

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

/* model function: a plane p0 + p1*tx + p2*tz */
double f( double tx, double tz, const double *p )
{
    return p[0] + p[1]*tx + p[2]*tz;
}

/* data structure to transmit model data to function evalution */
typedef struct
{
    double *tx, *tz;
    double *y;
    double (*f)( double tx, double tz, const double *p );
} data_struct;

/* function evaluation, determination of residues */
void evaluate_surface(	const double *par,  //Parameter vector
						int m_dat,          //Dimension of residue vector
                        const void *data,   //Data structure
                       	double *fvec,       //residue vector..
                        int *info )         //
{
    int i;

    /* for readability, perform explicit type conversion */
    data_struct *mydata;
    mydata = (data_struct*) data;

    for ( i = 0; i < m_dat; i++ )
    {
		fvec[i] = mydata->y[i] - mydata->f( mydata->tx[i], mydata->tz[i], par );
    }
}

int main()
{
	int i;
    double ff;

    /* parameter vector */
    int n_par = 3; // number of parameters in model function f
    double par[3] = { -1, 0, 1 }; // arbitrary starting value

    lm_status_struct status;
    lm_control_struct control;

    /* data points */
    int m_dat = 4;
    double tx[4] = { -1, -1,  1,  1 };
    double tz[4] = { -1,  1, -1,  1 };
    double y[4]  = {  0,  1,  1,  2 };

    data_struct data;// = { tx, tz, y, f };
    data.tx = tx;
    data.tz = tz;
    data.y = y;
    data.f = f;

    /* auxiliary parameters */
    control = lm_control_double;
    control.printflags = 3; // monitor status (+1) and parameters (+2)

    /* perform the fit */
    printf( "Fitting:\n" );
    lmmin( 	n_par,
    		par,
            m_dat,
            (const void*) &data,
            evaluate_surface,
            &control,
            &status,
            my_printout);

    /* print results */
    printf( "\nResults:\n" );
    printf( "status after %d function evaluations:\n  %s\n", status.nfev, lm_infmsg[status.info] );

    printf("obtained parameters:\n");

    for ( i=0; i<n_par; ++i )
    {
		printf("  par[%i] = %12g\n", i, par[i]);
    }

    printf("obtained norm:\n  %12g\n", status.fnorm );
    printf("fitting data as follows:\n");

    for ( i=0; i<m_dat; ++i )
    {
        ff = f(tx[i], tz[i], par);
        printf( "  t[%2d]=%12g,%12g y=%12g fit=%12g residue=%12g\n",
                i, tx[i], tz[i], y[i], ff, y[i] - ff );
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
#pragma comment(lib, "lmfit-static.lib")
#endif
