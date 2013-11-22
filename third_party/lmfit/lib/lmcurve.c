/*
 * Project:  LevenbergMarquardtLeastSquaresFitting
 *
 * File:     lmcurve.c
 *
 * Contents: Simplified wrapper for one-dimensional curve fitting,
 *           using Levenberg-Marquardt least-squares minimization.
 *
 * Usage:    see application sample demo/curve1.c
 *
 * Author:   Joachim Wuttke 2010
 *
 * Homepage: joachimwuttke.de/lmfit
 */


#include "lmmin.h"


typedef struct {
    const double *t;
    const double *y;
    double (*f) (double t, const double *par);
} lmcurve_data_struct;

typedef struct {
    const double *t;
    const double *y;
    void* userData;
    double (*f)( double t, const double *par, void* userData );
} lmcurve_data_struct2;


void lmcurve_evaluate( const double *par, int m_dat, const void *data,
                       double *fvec, int *info )
{
    int i;
    for ( i = 0; i < m_dat; i++ )
        fvec[i] =
            ((lmcurve_data_struct*)data)->y[i] -
            ((lmcurve_data_struct*)data)->f(
                ((lmcurve_data_struct*)data)->t[i], par );
    // *info = *info; /* to prevent a 'unused variable' warning */
}

void lmcurve_evaluate2( const double *par, int m_dat, const void *data,
                       double *fvec, int *info )
{
    int i;
    for ( i = 0; i < m_dat; i++ )
        fvec[i] =
            ((lmcurve_data_struct2*)data)->y[i] -
            ((lmcurve_data_struct2*)data)->f(
                ((lmcurve_data_struct2*)data)->t[i], par,
                ((lmcurve_data_struct2*)data)->userData);
    // *info = *info; /* to prevent a 'unused variable' warning */
}


void lmcurve_fit( int n_par, double *par, int m_dat,
                  const double *t, const double *y,
                  double (*f)( double t, const double *par ),
                  const lm_control_struct *control, lm_status_struct *status )
{
//    lmcurve_data_struct data = { t, y, f };
    lmcurve_data_struct data;
    data.t = t;
    data.y = y;
    data.f = f;

    lmmin( n_par, par, m_dat, (const void*) &data,
           lmcurve_evaluate, control, status, lm_printout_std );
}

void lmcurve_fit2( int n_par, double *par, int m_dat,
                  const double *t, const double *y, void* userData,
                  double (*f)( double t, const double *par, void* userData ),
                  const lm_control_struct *control, lm_status_struct *status )
{
//    lmcurve_data_struct data = { t, y, f };
    lmcurve_data_struct2 data;
    data.t = t;
    data.y = y;
    data.userData = userData;
    data.f = f;

    lmmin( n_par, par, m_dat, (const void*) &data,
           lmcurve_evaluate, control, status, lm_printout_std );
}
