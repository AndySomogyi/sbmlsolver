//---------------------------------------------------------------------------

#pragma hdrstop
#include <iostream>

using namespace std;
//---------------------------------------------------------------------------
extern "C"
{
#include "00957-sbml-l2v4.h"
}
#pragma argsused
int main()
{
    cout<<"Hello";

    InitModel();
    double y[10];
    y[0] =0;
    computeRules(y);
    evalEvents(0,y);
    return 0;
}
//---------------------------------------------------------------------------
