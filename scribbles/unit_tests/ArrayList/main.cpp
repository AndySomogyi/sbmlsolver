//---------------------------------------------------------------------------
#pragma hdrstop
#include <iostream>
#include "rrArrayList2.h"
//---------------------------------------------------------------------------
#pragma argsused

using namespace rr;
using namespace std;
int main()
{
    ArrayListItem<string> str1("34343");
    ArrayListItem<string> str2("12");

    str2 = str1;
    cout<<str1[2];
    cout<<str2;

    ArrayList2 aList1;
    ArrayList2 aList2;

    double test = 2;
    aList1.Add(test);
    aList1.Add("tests");
    aList2.Add(34);
    aList2.Add("in me");
    aList1.Add(aList2);
    aList1.Add(str1);
    cout<<aList1;

    aList1.Add(1234);
    aList1.Add("sdfsdfd");

    cout<<"\n"<<aList1;

    for(u_int i = 0; i < aList1.Count(); i++)
    {
        cout<<aList1[i]<<endl;
    }

    return 0;
}

