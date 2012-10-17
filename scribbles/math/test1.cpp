
#pragma hdrstop
#include <string>
#include <iostream>
#include <strstream>
#include <conio.h>
#include <iomanip>
#include <math.h>
#include "rr_support/rrSupport.h"
#include "rrStringUtils.h"

//---------------------------------------------------------------------------
using namespace std;
using namespace rr;
int main()
{
//    strstream outPut;
//
//    outPut.precision(5);
//    outPut.setf(ios::left);
//	//cout<<outPut.str()<<endl;
//
//    //Test and function
//    cout<<" And is "<< ToString( spf_and(3, (double) 1, (double) 0, (double) 1) )<<endl;
//
//    //Power function
//    double base = 25;
//    double exp = -1;
//    cout<<base<<" to the power of "<<exp<<" = "<<spf_pow(base, exp)<<endl;
//    cout<<"CG: "<<base<<" to the power of "<<exp<<" = "<<pow(base, exp)<<endl;

    double val =.0000000001;
    char sBuffer[256];
    sprintf(sBuffer, "%G", val);
    string str = string(sBuffer);
    cout<<"A double: "<<str;
    //////////////////////////////////////////////////
    cout<<"Hit any key to exit...";
    cin.ignore(0,'\n');
    getch();

    return 0;
}

