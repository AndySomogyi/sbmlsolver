//---------------------------------------------------------------------------
#pragma hdrstop
#include <iostream>
#include <tchar.h>
//---------------------------------------------------------------------------
#include "rrException.h"
#include "rrDoubleMatrix.h"
#pragma argsused
using namespace rr;
using namespace std;

int main ()
{
//  	try
//  	{
//  		throw SBWApplicationException("Testing an exception");
//  	}
//  	catch (exception& e)
//  	{
//    	cout << e.what() << endl;
//  	}

    int rows = 5;
    int cols = 5;
    DoubleMatrix mat(5,5);

//    double **test = new double**[cols];
//    for(int col = 0; col < cols; col++)
//    {
//    	*test[col] = new double[rows];
//    }
//
//    int count = 1;
//    for(int row = 0; row < rows; row++)
//    {
//	    for(int col = 0; col < cols; col++)
//        {
//	        *test[row][col] = count +10;
//       		mat(row,col) = count++;
//        }
//    }
//	cout<<mat;

	return 0;
}


