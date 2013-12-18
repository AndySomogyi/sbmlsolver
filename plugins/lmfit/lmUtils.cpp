#pragma hdrstop
#include <sstream>
#include <string>
#include "rrUtils.h"
#include "rrLogger.h"
#include "lmUtils.h"
#include "lib/lmmin.h"
//---------------------------------------------------------------------------

namespace lmfit
{
using namespace std;
using namespace rr;

void my_printout(   int n_par,
                    const double *par,
                    int m_dat,
                    const void *data,   //data  : for soft control of printout behaviour, add control variables to the data struct
                    const double *fvec,
                    int printflags,
                    int iflag,          //iflag : 0 (init) 1 (outer loop) 2(inner loop) -1(terminated)
                    int iter,           //iter  : outer loop counter
                    int nfev)            //nfev  : number of calls to *evaluate
{
    if( !printflags )
        return;

    if( printflags & 1 )
    {
        /* location of printout call within lmdif */
        if (iflag == 2)
        {
            Log(lDebug) << "trying step in gradient direction  ";
        }
        else if (iflag == 1)
        {
            Log(lDebug) << "determining gradient (iteration " << iter << ")";
        }
        else if (iflag == 0)
        {
            Log(lInfo) << "starting minimization              ";
        }
        else if (iflag == -1)
        {
            Log(lInfo) << "terminated after " << nfev << " evaluations   ";
        }
    }

    if( printflags & 2 )
    {
        Log(lDebug) << "par: ";
        for (int i = 0; i < n_par; ++i)
        {
            Log(lDebug) << par[i];
        }
        Log(lDebug) << " => norm: " << lm_enorm(m_dat, fvec);
    }

    if( printflags & 3 )
    {
        Log(lDebug)<<( "\n" );
    }

    if ( (printflags & 8) || ((printflags & 4) && iflag == -1) )
    {
        Log(lDebug) << "residuals:\n";
        for (int i = 0; i < m_dat; ++i)
        {
            Log(lDebug) << "    fvec[" << i << "]=" << fvec[i];
        }
    }
}

void ui_printout(   int n_par,
                    const double *par,
                    int m_dat,
                    const void *userData,   //data  : for soft control of printout behaviour, add control variables to the data struct
                    const double *fvec,
                    int printflags,
                    int iflag,          //iflag : 0 (init) 1 (outer loop) 2(inner loop) -1(terminated)
                    int iter,           //iter  : outer loop counter
                    int nfev)            //nfev  : number of calls to *evaluate
{
    if( !printflags )
        return;

    const lmDataStructure *myData = (const lmDataStructure*) userData;

    stringstream ss;

    if( printflags & 1 )
    {
        /* location of printout call within lmdif */
        if (iflag == 2)
        {
            ss << "trying step in gradient direction  ";
        }
        else if (iflag == 1)
        {
            ss << "determining gradient (iteration " << iter << ")";
        }
        else if (iflag == 0)
        {
            ss << "starting minimization              ";
        }
        else if (iflag == -1)
        {
            ss << "terminated after " << nfev << " evaluations   ";
        }
    }

    if( printflags & 2 )
    {
        ss << "par: ";
        for (int i = 0; i < n_par; ++i)
        {
            ss << par[i];
        }
        ss << " => norm: " << lm_enorm(m_dat, fvec);
    }

    if( printflags & 3 )
    {
        ss <<( "\n" );
    }

    if ( (printflags & 8) || ((printflags & 4) && iflag == -1) )
    {
        ss << "residuals:\n";
        for (int i = 0; i < m_dat; ++i)
        {
            ss << "    fvec[" << i << "]=" << fvec[i];
        }
    }

    //Event to UI, with progress data
    string msg = ss.str();
    if(myData->mProgressEvent)
    {
        char* passMsg = createText(msg);
        myData->mProgressEvent((void*) passMsg, myData->mProgressEventContextData);
        delete [] passMsg;
    }

}

}

