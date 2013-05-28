#pragma hdrstop
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include "rrBaseParameter.h"
#include "rrException.h"
#include "rrArrayListItem.h"
#include "rrc_cpp_support.h"
#include "rrc_types.h"
#include "rrc_utilities.h"

namespace rrc
{
using namespace rr;

BaseParameter* castToParameter(RRParameterHandle handle)
{
	BaseParameter* para = (BaseParameter*) handle;
    if(para) //Will only fail if handle is NULL...
    {
    	return para;
    }
    else
    {
    	Exception ex("Failed to cast to a valid Parameter handle");
    	throw(ex);
    }
}

MinimizationData* castToMinimizationData(RRMinimizationDataHandle handle)
{
	MinimizationData* para = (MinimizationData*) handle;
    if(para) //Will only fail if handle is NULL...
    {
    	return para;
    }
    else
    {
    	Exception ex("Failed to cast to a valid MinimizationData handle");
    	throw(ex);
    }
}

RoadRunnerList* getRRList(RRInstanceListHandle listHandle)
{
	RoadRunnerList* handle = (RoadRunnerList*) listHandle->RRList;
    if(handle)
    {
    	return handle;
    }
    else
    {
    	Exception ex("Failed to create a valid RoadRunnerList handle");
    	throw(ex);
    }
}

void setError(const string& err)
{
    if(gLastError)
    {
        delete [] gLastError;
    }

    gLastError = createText(err);
}


RoadRunner* castFrom(RRHandle handle)
{
	RoadRunner* rr = (RoadRunner*) handle;
    if(rr) //Will only fail if handle is NULL...
    {
    	return rr;
    }
    else
    {
    	Exception ex("Failed to cast to a valid RoadRunner handle");
    	throw(ex);
    }
}

Plugin* castToPlugin(RRPluginHandle handle)
{
	Plugin* plugin = (Plugin*) handle;
    if(plugin) //Will only fail if handle is NULL...
    {
    	return plugin;
    }
    else
    {
    	Exception ex("Failed to cast to a valid Plugin handle");
    	throw(ex);
    }
}


//char* createText(const string& str)
//{
//	if(str.size() == 0)
//    {
//    	return NULL;
//    }
//
//	char* text = new char[str.size() + 1];
//	std::copy(str.begin(), str.end(), text);
//	text[str.size()] = '\0'; //terminating 0!
//	return text;
//}

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
        list->String[i] = createText(sList[i]);
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

RRList* createArrayList(const rr::NewArrayList& aList)
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
        NewArrayListItemObject* ptr = const_cast<NewArrayListItemObject*>(&aList[i]);
        if(dynamic_cast<ArrayListItem<int>*>(ptr))
        {
            int val = (int) *(dynamic_cast<NewArrayListItem<int>*>(ptr));
            myItem = createIntegerItem (val);
			addItem (theList, &myItem);
        }
        else if(dynamic_cast<NewArrayListItem<double>*>(ptr))
        {
            double val = (double) *(dynamic_cast<NewArrayListItem<double>*>(ptr));
            myItem = createDoubleItem (val);
			addItem (theList, &myItem);
        }
        else if(dynamic_cast<NewArrayListItem<string>*>(ptr))
        {
            string item = (string) *(dynamic_cast<NewArrayListItem<string>*>(ptr));
            char* str = (char *) new char[item.size() + 1];
            strcpy (str, item.c_str());
			myItem = createStringItem (str);
   			addItem (theList, &myItem);
        }
        else if(dynamic_cast<NewArrayListItem<StringList>*>(ptr))
        {
            StringList list 			= (StringList) *(dynamic_cast<NewArrayListItem<StringList>*>(ptr));
			NewArrayList  aList;
            for(int i = 0; i < list.Count(); i++)
            {
            	aList.Add(list[i]);
            }
			RRListHandle myList 			= createArrayList (aList);
			myItem 						    = createListItem(myList);
   			addItem (theList, &myItem);
        }

        else if(dynamic_cast<NewArrayListItem<NewArrayList>*>(ptr))
        {
            NewArrayList list = (NewArrayList) *(dynamic_cast<NewArrayListItem<NewArrayList>*>(ptr));
			RRListHandle myList 			= createArrayList (list);
			RRListItemHandle myListItem 	= createListItem (myList);
			addItem (theList, &myListItem);
        }
    }
    return theList;
}

//RRParameterHandle createParameter(const rr::BaseParameter& para)
//{
//    if(para.getType() == "bool")
//    {
//    	Parameter<bool> *thePara = dynamic_cast< Parameter<bool>* >(const_cast< BaseParameter* >(&para));
//
//	    RRParameter* aPara 	= new RRParameter;
//        aPara->ParaType 	= ptBool;
//        aPara->data.bValue 	= thePara->getValue();
//        aPara->mName		= createText(thePara->getName());
//        aPara->mHint		= createText(thePara->getHint());
//        return aPara;
//    }
//
//    if(para.getType() == "integer")
//    {
//    	Parameter<int> *thePara = dynamic_cast< Parameter<int>* >(const_cast< BaseParameter* >(&para));
//
//	    RRParameter* aPara 	= new RRParameter;
//        aPara->ParaType 	= ptInteger;
//        aPara->data.iValue 	= thePara->getValue();
//        aPara->mName		= createText(thePara->getName());
//        aPara->mHint		= createText(thePara->getHint());
//        return aPara;
//    }
//
//    if(para.getType() == "double")
//    {
//    	Parameter<double> *thePara = dynamic_cast< Parameter<double>* >(const_cast< BaseParameter* >(&para));
//
//	    RRParameter* aPara 	= new RRParameter;
//        aPara->ParaType 	= ptInteger;
//        aPara->data.iValue 	= thePara->getValue();
//        aPara->mName		= createText(thePara->getName());
//        aPara->mHint		= createText(thePara->getHint());
//        return aPara;
//    }
//
//    if(para.getType() == "vector")
//    {
//    	Parameter<RRVector*> *thePara = dynamic_cast< Parameter<RRVector*>* >(const_cast< BaseParameter* >(&para));
//
//	    RRParameter* aPara 	= new RRParameter;
//        aPara->ParaType 	= ptVector;
//        aPara->data.vValue 	= thePara->getValue();
//        aPara->mName		= createText(thePara->getName());
//        aPara->mHint		= createText(thePara->getHint());
//        return aPara;
//    }
//	return NULL;
//}


RRDataHandle createRRData(const RoadRunnerData& result)
{
    RRData* rrCData  = new RRData;
    memset(rrCData, 0, sizeof(RRData));

    rrCData->ColumnHeaders = new char*[result.cSize()];
    for(int i = 0; i < result.cSize(); i++)
    {
        rrCData->ColumnHeaders[i] = createText(result.getColumnNames()[i]);
    }

    rrCData->RSize = result.rSize();
    rrCData->CSize = result.cSize();
    int size = rrCData->RSize*rrCData->CSize;
    rrCData->Data = new double[size];

    if(result.hasWeights())
    {
    	rrCData->Weights = new double[size];
    }

    int index = 0;
    //The data layout is simple row after row, in one single long row...
    for(int row = 0; row < rrCData->RSize; row++)
    {
        for(int col = 0; col < rrCData->CSize; col++)
        {
            rrCData->Data[index] = result(row, col);
            if(result.hasWeights())
            {
            	rrCData->Weights[index] = result.weight(row, col);
            }
            ++index;
        }
    }
	return rrCData;
}

}
