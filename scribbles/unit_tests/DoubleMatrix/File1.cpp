#pragma hdrstop
#pragma argsused

#include <tchar.h>
#include <stdio.h>
#include <iostream>

#include "rrDoubleMatrix.h"
using namespace rr;
using namespace std;

DoubleMatrix Test();

int _tmain(int argc, _TCHAR* argv[])
{
	DoubleMatrix testing;
	testing = Test();

	cout<<testing;
	return 0;
}

DoubleMatrix Test()
{
	DoubleMatrix test(3,3);

	cout<<test;
	return test;
}
