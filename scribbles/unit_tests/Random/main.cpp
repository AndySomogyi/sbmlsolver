#pragma hdrstop
#include <tchar.h>
#include <iostream>
#include <iomanip>
#include "rrRandom.h"
#pragma argsused
using namespace std;
using namespace rr;

int _tmain(int argc, _TCHAR* argv[])
{

	Random random;
    cout<<"1000 random numbers in [0, 1)"<<endl;
  	for (int i = 0; i < 1000; ++i)
    {
    	cout.width(15);
    	cout<<left<<i<<fixed<<setprecision(8)<<random();

//    	if ((i % 5) == 4)
        {
        	cout<<endl;
        }
  	}
	return 0;
}

