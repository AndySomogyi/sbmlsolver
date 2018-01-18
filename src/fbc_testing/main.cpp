/* $Id: modify.cpp 1662 2011-01-04 17:52:40Z lou $ */
// Copyright (C) 2007, International Business Machines
// Corporation and others.  All Rights Reserved.
// This code is licensed under the terms of the Eclipse Public License (EPL).


/*
  This example shows the creation of a model from arrays, solution
  and then changes to objective and adding a row
*/
#include "coin/ClpSimplex.hpp"
#include "coin/CoinHelperFunctions.hpp"
#include <stdexcept>

#include "rrSBMLReader.h"
#include "ExecutableModelFactory.h"

using namespace rr;

void test1() {
    ClpSimplex  model;

/*
 ************************************************************************
*
*  The data in this file represents the following problem:
*
*  Minimize or maximize Z = x1 + 2x5 - x8
*
*  Subject to:
*
*  2.5 <=   3x1 +  x2         -  2x4 - x5               -    x8
*                 2x2 + 1.1x3                                   <=  2.1
*                          x3              +  x6                 =  4.0
*  1.8 <=                      2.8x4             -1.2x7         <=  5.0
*  3.0 <= 5.6x1                      + x5               + 1.9x8 <= 15.0
*
*  where:
*
*  2.5 <= x1
*    0 <= x2 <= 4.1
*    0 <= x3
*    0 <= x4
*  0.5 <= x5 <= 4.0
*    0 <= x6
*    0 <= x7
*    0 <= x8 <= 4.3
*
*  x3, x4 are 0,1 variables.
*
************************************************************************
 */

    // model is as exmip1.mps from Data/samples
    int numberRows = 5;
    int numberColumns = 8;
    int numberElements = 14;
    // matrix data - column ordered
    int start[9] =          {0, 2, 4, 6, 8, 10, 11, 12, 14};
    int length[8] =         {2, 2, 2, 2, 2,  1,  1,  2};
    int rows[14] =          {0,   4, 0, 1,   1, 2,  0,   3,  0, 4, 2,    3,  0,   4};
    double elements[14] =   {3, 5.6, 1, 2, 1.1, 1, -2, 2.8, -1, 1, 1, -1.2, -1, 1.9};

    CoinPackedMatrix matrix(true, numberRows, numberColumns, numberElements, elements, rows, start, length);

    // rim data
    double objective[8] = {1, 0, 0, 0, 2, 0, 0, -1};
    double rowLower[5] = {2.5, -COIN_DBL_MAX, 4, 1.8, 3};
    double rowUpper[5] = {COIN_DBL_MAX, 2.1, 4, 5, 15};
    double colLower[8] = {2.5, 0, 0, 0, 0.5, 0, 0, 0};
    double colUpper[8] = {COIN_DBL_MAX, 4.1, 1, 1, 4, COIN_DBL_MAX, COIN_DBL_MAX, 4.3};
    // load problem
    model.loadProblem(matrix, colLower, colUpper, objective,
                      rowLower, rowUpper);
    // mark integer (really for Cbc/examples/modify.cpp
    model.setInteger(2);
    model.setInteger(3);

    // Solve
    model.initialSolve();

    // Solution
    const double * solution = model.primalColumnSolution();
    int i;
    for (i = 0; i < numberColumns; i++)
         if (solution[i])
              printf("Column %d has value %g\n", i, solution[i]);

    // Change objective
    double * objective2 = model.objective();
    objective2[0] = -100.0;

    // Solve - primal as primal feasible
    model.primal(1);

    // Solution (array won't have changed)
    for (i = 0; i < numberColumns; i++)
         if (solution[i])
              printf("Column %d has value %g\n", i, solution[i]);

    // Add constraint
    int column[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    double element2[8] = {1, 1, 1, 1, 1, 1, 1, 1};
    model.addRow(8, column, element2, 7.8, COIN_DBL_MAX);

    // Solve - dual as dual feasible
    model.dual();

    /* Solution
       This time we have changed arrays of solver so -
       array won't have changed as column array and we added a row
       - but be on safe side
    */
    solution = model.primalColumnSolution();
    for (i = 0; i < numberColumns; i++)
         if (solution[i])
              printf("Column %d has value %g\n", i, solution[i]);


}


void testread(const char* fname)
{
    try {
	std::string s = SBMLReader::read(fname);

	rr::ExecutableModel *e = ExecutableModelFactory::createModel(s);
        
    } catch (std::exception &e) {
        std::cout << "exception: " << e.what() << std::endl;
    }

}



int read_lp(const char* fname)
{

    ClpSimplex  model;


    model.readLp(fname);



    int cols = model.getNumCols();




    // Solve

    model.initialSolve();

    // Solve - primal as primal feasible
    model.primal(1);

    // Solution
    const double * solution = model.primalColumnSolution();
    for (int i = 0; i < cols; i++)
              printf("Column %d , %s, has value %g\n", i, model.getColumnName(i).c_str(), solution[i]);


    return 0;

    /* foo */

}

int main(int argc, const char *argv[])
{
	try {

		if(argc > 1) {

			testread(argv[1]);
		}


	}
	catch (CoinError& e) {
		e.print();
	}

	return 0;


    ClpSimplex  model;


/*
\\ Example one LP format

Maximize
objective1_objf:
+ 1.0 R2

￼Subject To
 A: +R1 +0R2 -X1 -X2 = 0
 B: 0R1 -R2  +X1 +X2 = 0
Bounds
 R1 <= 1.0
 0.0 <= R2 <= +inf
 0.0 <= X1 <= +inf
 0.0 <= X2 <= +inf
END


1  0 −1 −1
0 −1  1  1


    C0000000  R0000009   -1.           R0000012  1.
    C0000001  R0000010   -1.
    C0000002  R0000011  1.             R0000016   -1.
    C0000003  R0000008  1.             R0000009   -1.
    C0000004  R0000004  1.             R0000005   -1.
    C0000005  R0000009  1.             R0000010  1.
    C0000006  R0000000   -1.           R0000021   -1.
    C0000007  R0000012  1.             R0000013   -1.
    C0000008  R0000000  1.             R0000002   -1.
    C0000009  R0000000  1.             R0000001   -1.
    C0000010  R0000001  1.             R0000003  1.
    C0000011  R0000001   -1.           R0000002   -1.
    C0000012  R0000006  1.             R0000007   -1.
    C0000013  R0000003  1.             R0000004   -1.
    C0000014  R0000003  1.             R0000006   -1.
    C0000015  R0000005  1.             R0000008   -1.
    C0000016  R0000007  1.             R0000008   -1.
    C0000017  R0000013  1.             R0000016   -1.
    C0000018  R0000010  1.             R0000014   -1.
    C0000019  OBJROW     -1.           R0000000   -1.
    C0000019  R0000018  1.
    C0000020  R0000017   -1.           R0000017   -1.
    C0000020  R0000018  0.5            R0000018  1.
    C0000021  R0000016   -1.           R0000018  1.
    C0000022  R0000011   -1.           R0000017  1.
    C0000023  R0000014  1.             R0000015  1.
    C0000024  R0000000   -1.           R0000015   -1.
    C0000025  R0000000  1.

         0    1    2    3   4     5    6    7   8    9    10   11   12   13   14   15   16   17   18   19   20   21   22   23   24   25
       R16, R14, R15, R12, R07, R13, R01, R17, R03, R02, R05, R04, R10, R06, R09, R08, R11, R18, R19, R26, R25, R24, R23, R22, R21, R20
0 A [[   0,   0,   0,   0,   0,   0,   1,   0,  -1,  -1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  -1,   0,   0,   0,   0,   0],
1 B  [   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,  -1,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0],
2 C  [   0,   0,   0,   0,   0,   0,   0,   0,   1,   0,   0,  -1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0],
3 D  [   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   0,   0,  -1,  -1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0],
4 E  [   0,   0,   0,   0,  -1,   0,   0,   0,   0,   0,   0,   0,   0,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0],
5 F  [   0,   0,   0,   0,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  -1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0],
6 G  [   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  -1,   0,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0],
7 H  [   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   0,   0,   0,  -1,   0,   0,   0,   0,   0,   0,   0,   0,   0],
8 I  [   0,   0,   0,  -1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0],
9 J  [  -1,   0,   0,   1,   0,  -1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0],
10K  [   0,  -1,   0,   0,   0,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  -1,   0,   0,   0,   0,   0,   0,   0],
11L  [   0,   1,  -1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   0],
12M  [   1,   0,   0,   0,   0,   0,   0,  -1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0],
13N  [   0,   0,   0,   0,   0,   0,   0,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,  -1,   0,   0,   0,   0,   0,   0,   0,   0],
14O  [   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   0,   0,   0,   0,   0,   0,  -1],
15P  [   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  -1,   1],
16Q  [   0,   0,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   0,   0,   0,   0,   0,  -1,   0,   0],
17R  [   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  -1,  -1,   1,   0,   0],
18S  [   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  -1, 0.5,   1,   1,   0,   0,   0]]
19T  [   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  -1,   0,   0,   0,   0,   0]]
20U  [   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   0,   0,   0,   0,   0]]
21X  [   0,   0,   0,   0,   0,   0,  -1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0]]
22Y  [   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   0,   0,   0,   0,   0,   0]]
         2    2    2    2    2    2    2    2    2    2    2    2    2    2    2    2    2    2    2    2    4    2    2    2    2    2


 */

    // model is as exmip1.mps from Data/samples
    int numberRows = 23;
    int numberColumns = 26;
    int numberElements = 54;
    // matrix data - column ordered
    int start[27] =       {0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,44,46,48,50,52,54};
    int length[26] =      {2,2,2,2,2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2};

                          //R16,   R14,    R15,   R12,  R07,   R13,  R01,   R17,  R03,  R02,  R05,  R04,    R10,
    int rows[54] =       { 9,12, 10,11,  11,16,   8,9,  4,5,  9,10, 0,21, 12,13,  0,2,  0,1,  1,3,  1,2,    6,7,

    		              // R06,  R09,   R08,   R11,    R18,    R19,    R26,         R25,    R24,    R23,   R22,    R21,    R20
    			             3,4,  3,6,   5,8,   7,8,  13,16,  10,14,  18,22,  0,18,19,20,  17,18,  17,18, 16,17,  11,15,  14,15};


    //                      R16,   R14,  R15,   R12,   R07,  R13,  R01,   R17,  R03,   R02,   R05,  R04,    R10,
    double elements[54] = {-1,1,  -1,1,  -1,1,  -1,1,  -1,1, -1,1,  1,-1, -1,1, -1,1,  -1,1,  -1,1,  1,-1,  -1,1,


    // R06,  R09,    R08,   R11,   R18,  R19,  R26,          R25,   R24,   R23,    R22,    R21,   R20
	  -1,1,  -1,1,  -1,1,  -1,1,  -1,1, -1,1, -1,1,  -1,0.5,-1,1,  -1,1,  -1,1,   -1,1,   1,-1,  -1,1 };


    CoinPackedMatrix matrix(true, numberRows, numberColumns, numberElements, elements, rows, start, length);


    for(int i = 0; i < numberColumns; ++i) {
    	for(int j = 0; j < numberRows; j++) {
    		double val = matrix.getCoefficient(j, i);
    		if(val != 0)
    			printf("%i, %i, %f\n", i, j, val);
    	}
    }

    double objective[26] = {0};
    objective[19] = 1;
    double rowLower[23] = {0};
    double rowUpper[23] = {0};

/* 

0 R16 ≥ 0     && R16 ≤ 1000
1 R14 ≥ −1000 && R14 ≤ 1000
2 R15 ≥ 0     && R15 ≤ 1000
3 R12 ≥ 0     && R12 ≤ 1000
4 R07 ≥ 0     && R07 ≤ 1000
5 R13 ≥ 0     && R13 ≤ 1000
6 R01 ≥ 0     && R01 ≤ 1
7 R17 ≥ 0     && R17 ≤ 1000
8 R03 ≥ −1000 && R03 ≤ 1000
9 R02 ≥ −1000 && R02 ≤ 1000
10R05 ≥ 0     && R05 ≤ 1000
11R04 ≥ −1000 && R04 ≤ 1000
12R10 ≥ 0     && R10 ≤ 1000
13R06 ≥ 0     && R06 ≤ 1000
14R09 ≥ 0     && R09 ≤ 1000
15R08 ≥ 0     && R08 ≤ 1000
16R11 ≥ 0     && R11 ≤ 1000
17R18 ≥ 0     && R18 ≤ 1000
18R19 ≥ −1000 && R19 ≤ 1000
19R26 ≥ 0     && R26 ≤ 1000
20R25 ≥ 0     && R25 ≤ 1000
21R24 ≥ 0     && R24 ≤ 1000
22R23 ≥ −1000 && R23 ≤ 1000
23R22 ≥ 0     && R22 ≤ 1000
24R21 ≥ −1000 && R21 ≤ 1000
25R20 ≥ −1000 && R20 ≤ 1000

 */


    double colLower[26] = {0};
    
    colLower[1] = -1000;
    colLower[8] = -1000;
    colLower[9] = -1000;
    colLower[11] = -1000;
    colLower[18] = -1000;
    colLower[22] = -1000;
    colLower[24] = -1000;
    colLower[25] = -1000;

    double colUpper[26] = {1000.}; colUpper[6] = 1;

    // load problem
    model.loadProblem(matrix, colLower, colUpper, objective,
                      rowLower, rowUpper);

    model.setOptimizationDirection(-1);




    model.writeMps("test.mps");


    // Solve

    model.initialSolve();

    // Solve - primal as primal feasible
    model.primal(1);

    // Solution
    const double * solution = model.primalColumnSolution();
    for (int i = 0; i < numberColumns; i++)
              printf("Column %d has value %g\n", i, solution[i]);


    test1();

}
