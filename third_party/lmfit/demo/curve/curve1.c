/*
 * Project:  LevenbergMarquardtLeastSquaresFitting
 *
 * File:     curve1.c
 *
 * Contents: Example for one-dimensional curve fitting,
 *           using the simplified interface from lmcurve.h.
 *
 * Author:   Joachim Wuttke 2004-10
 *
 * Homepage: joachimwuttke.de/lmfit
 */

 #include <stdio.h>
 #include "../lib/lmcurve.h"

/* model function: a quadratic function p0 + p1 * (t-p2)^2 */
double f( double t, const double *p )
{
    return p[0] + p[1]*(t-p[2])*(t-p[2]);
}

int main()
{
    /* parameter vector */
    int n_par = 3; // number of parameters in model function f
    double par[3] = { 1, 0, 1 }; // relatively bad starting value

    /* data pairs: slightly distorted standard parabola */
    int m_dat = 11; // number of data pairs
    int i;
    double x[11] = { -5., -4., -3., -2., -1., 0., 1., 2., 3., 4., 5. };
    double y[11] = { 25.5, 16.6, 9.9, 4.4, 1.1, 0, 1.1, 4.2, 9.3, 16.4, 25.5 };

    /* auxiliary parameters */
    lm_status_struct status;
    lm_control_struct control = lm_control_double;
    control.printflags = 3; // monitor status (+1) and parameters (+2)

    /* perform the fit */
    printf( "Fitting:\n" );
    lmcurve_fit( n_par, par, m_dat, x, y, f, &control, &status );

    /* print results */
    printf( "\nResults:\n" );
    printf( "status after %d function evaluations:\n  %s\n",
            status.nfev, lm_infmsg[status.info] );

    printf("obtained parameters:\n");
    for ( i = 0; i < n_par; ++i)
	printf("  par[%i] = %12g\n", i, par[i]);
    printf("obtained norm:\n  %12g\n", status.fnorm );

    printf("fitting data as follows:\n");
    for ( i = 0; i < m_dat; ++i)
        printf( "  t[%2d]=%12g y=%12g fit=%12g residue=%12g\n",
                i, x[i], y[i], f(x[i],par), y[i] - f(x[i],par) );

    return 0;
}
