#ifdef USE_PCH
#include "rr_pch.h"
#endif
#pragma hdrstop
#include <string>
#include <sstream>
#include "rrException.h"
#include "rr_c_api.h"
#include "rr_c_api_support.h"
#include "rrUtils.h"
#include "rrArrayListItem.h"
//---------------------------------------------------------------------------
#if defined (__CODEGEAR__)
#pragma package(smart_init)
#endif
using namespace std;
using namespace rr;
namespace rr_c_api
{
extern char* gLastError;
const char* ALLOCATE_API_ERROR_MSG = "Please allocate a handle to the roadrunner API before calling any API function";

void setError(const string& err)
{
    if(gLastError)
    {
        delete [] gLastError;
    }
    gLastError = new char[err.size() + 1];
    strcpy(gLastError, err.c_str());
}

char* createText(const char* str)
{
    char* newstr = new char[strlen(str) + 1];
    strcpy(newstr, str);
    return newstr;
}

char* createText(const string& str)
{
    char* newstr = new char[str.size() + 1];
    strcpy(newstr, str.c_str());
    return newstr;
}

RRMatrix* createMatrix(const ls::DoubleMatrix* mat)
{
	if(!mat)
    {
    	return NULL;
    }
    RRMatrixHandle matrix = new RRMatrix;

    matrix->RSize = mat->RSize();
    matrix->CSize = mat->CSize();
    int dim =  matrix->RSize * matrix->CSize;
    if(dim)
    {
    	matrix->Data =  new double[mat->RSize()*mat->CSize()];
    }
    else
    {
    	delete matrix;
        return NULL;
    }

    int index = 0;
    for(rr::u_int row = 0; row < mat->RSize(); row++)
    {
        for(rr::u_int col = 0; col < mat->CSize(); col++)
        {
            matrix->Data[index++] = (*mat)(row,col);
        }
    }
    return matrix;
}

vector<double> createVectorFromRRVector(const RRVector* vec)
{
    vector<double> aVec;

    if(!vec)
    {
        return aVec;
    }

    aVec.resize(vec->Count);
    for(int i = 0; i < aVec.size(); i++)
    {
        aVec[i] =  vec->Data[i];
    }

    return aVec;
}

RRVector* createVectorFromVector_double(const vector<double>& vec)
{
    RRVector* aVec = new RRVector;
    aVec->Count = vec.size();

    if(aVec->Count)
    {
        aVec->Data = new double[aVec->Count];
    }

    for(int i = 0; i < aVec->Count; i++)
    {
        aVec->Data[i] =  vec[i];
    }

    return aVec;
}

bool copyVector(const RRVector* src, vector<double>& dest)
{
    if(!src)
    {
        return false;
    }

    dest.resize(src->Count);

    for(int i = 0; i < src->Count; i++)
    {
        dest[i] = src->Data[i];
    }

    return true;
}

RRStringArrayHandle createList(const StringList& sList)
{
    if(!sList.Count())
    {
        return NULL;
    }

    RRStringArray* list = new RRStringArray;
    list->Count = sList.Count();

    list->String = new char*[list->Count];

    for(int i = 0; i < list->Count; i++)
    {
        list->String[i] = new char[sList[i].size()];
        strcpy(list->String[i], sList[i].c_str());
    }
    return list;
}

//RRList* createList(const ArrayList& aList)
//{
//    if(!aList.Count())
//    {
//        return NULL;
//    }
//
//    RRListItemHandle myItem;
//	// Setup a RRStringArrayList structure from aList
// 	RRListHandle theList = createRRList();
//
//    int itemCount = aList.Count();
//    for(int i = 0; i < itemCount; i++)
//    {
////        //Have to figure out subtype of item
////        ArrayListItem<string>* ptr = const_cast< ArrayListItemBase<string>* >(*aList[i]);
////        if(ptr->mValue)
////        {
////            string item =  *ptr->mValue;
////            char* str = (char *) new char[item.size() + 1];
////            strcpy(str, item.c_str());
////			myItem = createStringItem (str);
////   			addItem (theList, &myItem);
////        }
////        else if(ptr->mLinkedList)
////        {
////            //ArrayListItem<ArrayList2Item>* listItem = dynamic_cast<ArrayListItem<ArrayList2Item>*>(ptr);
////			RRListHandle myList = createList (*(ptr->mLinkedList));
////
////			RRListItemHandle myListItem = createListItem (myList);
////			addItem (theList, &myListItem);
////
////        }
//    }
//    return theList;
//}

RRList* createList(const rr::ArrayList& aList)
{
    if(!aList.Count())
    {
        return NULL;
    }

    RRListItemHandle myItem;
	// Setup a RRStringArrayList structure from aList
 	RRListHandle theList = createRRList();

    int itemCount = aList.Count();
    for(int i = 0; i < itemCount; i++)
    {
        //Have to figure out subtype of item
        ArrayListItemObject* ptr = const_cast<ArrayListItemObject*>(&aList[i]);
        if(dynamic_cast<ArrayListItem<int>*>(ptr))
        {
            int val = (int) *(dynamic_cast<ArrayListItem<int>*>(ptr));
            myItem = createIntegerItem (val);
			addItem (theList, &myItem);
        }
        else if(dynamic_cast<ArrayListItem<double>*>(ptr))
        {
            double val = (double) *(dynamic_cast<ArrayListItem<double>*>(ptr));
            myItem = createDoubleItem (val);
			addItem (theList, &myItem);
        }
        else if(dynamic_cast<ArrayListItem<string>*>(ptr))
        {
            string item = (string) *(dynamic_cast<ArrayListItem<string>*>(ptr));
            char* str = (char *) new char[item.size() + 1];
            strcpy (str, item.c_str());
			myItem = createStringItem (str);
   			addItem (theList, &myItem);
        }
        else if(dynamic_cast<ArrayListItem<StringList>*>(ptr))
        {
            StringList list 			= (StringList) *(dynamic_cast<ArrayListItem<StringList>*>(ptr));
			ArrayList  aList;
            for(int i = 0; i < list.Count(); i++)
            {
            	aList.Add(list[i]);
            }
			RRListHandle myList 			= createList (aList);
			myItem 						    = createListItem(myList);
   			addItem (theList, &myItem);
        }

        else if(dynamic_cast<ArrayListItem<ArrayList>*>(ptr))
        {
            ArrayList list = (ArrayList) *(dynamic_cast<ArrayListItem<ArrayList>*>(ptr));
			RRListHandle myList 			= createList (list);
			RRListItemHandle myListItem 	= createListItem (myList);
			addItem (theList, &myListItem);
        }

    }
    return theList;
}


}

