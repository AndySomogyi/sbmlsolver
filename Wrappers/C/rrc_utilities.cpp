/**
 * @file utilities.cpp
 * @brief roadRunner C API 2012
 * @author Totte Karlsson & Herbert M Sauro
 *
 * <--------------------------------------------------------------
 * This file is part of cRoadRunner.
 * See http://code.google.com/p/roadrunnerlib for more details.
 *
 * Copyright (C) 2012-2013
 *   University of Washington, Seattle, WA, USA
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * In plain english this means:
 *
 * You CAN freely download and use this software, in whole or in part, for personal,
 * company internal, or commercial purposes;
 *
 * You CAN use the software in packages or distributions that you create.
 *
 * You SHOULD include a copy of the license in any redistribution you may make;
 *
 * You are NOT required include the source of software, or of any modifications you may
 * have made to it, in any redistribution you may assemble that includes it.
 *
 * YOU CANNOT:
 *
 * redistribute any piece of this software without proper attribution;
*/

#pragma hdrstop
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include "rrRoadRunner.h"
#include "rrRoadRunnerList.h"
#include "rrUtils.h"
#include "rrException.h"
#include "rrc_utilities.h" 		// Need to include this before the support header..
#include "rrc_macros.h"
//---------------------------------------------------------------------------

namespace rrc
{
using namespace std;
using namespace rr;
using namespace rrc;

const char* ALLOCATE_API_ERROR_MSG 		= "Allocate a handle to the roadrunner API before calling any API function";
const char* INVALID_HANDLE_ERROR_MSG 	= "The HANDLE passed to this function was invalid";
char* 		gLastError      			= NULL;
char* 		gInstallFolder 				= NULL;

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

RRDataHandle rrCallConv createRRData(const RoadRunnerData& result)
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

char* rrCallConv getFileContent(const char* fName)
{
	try
    {
    	string fContent = getFileContent(fName);
        return createText(fContent);
    }
    catch_ptr_macro
}

bool rrCallConv compileSource(RRHandle handle, const char* sourceFileName)
{
	try
    {
        RoadRunner* rri = castFrom(handle);
        return rri->compileSource(sourceFileName);
    }
    catch_bool_macro
}

// -------------------------------------------------------------------
// List Routines
// -------------------------------------------------------------------
RRListHandle rrCallConv createRRList()
{
	RRListHandle list = new RRList;
	list->Count = 0;
	list->Items = NULL;
	return list;
}

void rrCallConv freeRRList (RRListHandle theList)
{
	if(!theList)
    {
    	return;
    }
    int itemCount = theList->Count;
    for(int i = 0; i < itemCount; i++)
    {
        if(theList->Items[i]->ItemType == litString)
        {
              delete [] theList->Items[i]->data.sValue;
        }

        if(theList->Items[i]->ItemType == litList)
        {
            freeRRList ((RRList *) theList->Items[i]->data.lValue);
        }
        delete theList->Items[i];
    }
	delete [] theList->Items;
    delete theList;
}

RRListItemHandle rrCallConv createIntegerItem (int value)
{
	RRListItemHandle item =  new RRListItem;
	item->ItemType = litInteger;
	item->data.iValue = value;
	return item;
}

RRListItemHandle rrCallConv createDoubleItem (double value)
{
	RRListItemHandle item = new RRListItem;
	item->ItemType = litDouble;
	item->data.dValue = value;
	return item;
}

RRListItemHandle rrCallConv createStringItem (char* value)
{
	RRListItemHandle item = new RRListItem;
	item->ItemType = litString;
	item->data.sValue = createText(value);
	return item;
}

RRListItemHandle rrCallConv createListItem (RRList* value)
{
	RRListItemHandle item = new RRListItem;
	item->ItemType = litList;
	item->data.lValue = value;
	return item;
}

// Add an item to a given list, returns the index to
// the item in the list. Returns -1 if it fails.
int rrCallConv addItem (RRListHandle list, RRListItemHandle *item)
{
	int n = list->Count;

	RRListItemHandle *newItems = new RRListItemHandle [n+1];
    if(!newItems)
    {
    	setError("Failed allocating memory in addItem()");
    	return -1;
    }

    for(int i = 0; i < n; i++)
    {
    	newItems[i] = list->Items[i];
    }

    newItems[n] = *item;
    RRListItemHandle *oldItems = list->Items;
    list->Items = newItems;

    delete [] oldItems;

	list->Count = n+1;
	return n;
}

bool rrCallConv isListItemInteger (RRListItemHandle item)
{
	return (item->ItemType == litInteger) ? true : false;
}

bool rrCallConv isListItemDouble (RRListItemHandle item)
{
	return (item->ItemType == litDouble) ? true : false;
}

bool rrCallConv isListItemString (RRListItemHandle item)
{
	return (item->ItemType == litString) ? true : false;
}

bool rrCallConv isListItemList (RRListItemHandle item)
{
	return (item->ItemType == litList) ? true : false;
}

RRListItemHandle rrCallConv getListItem (RRListHandle list, int index)
{
	return (index >= list->Count) ? NULL : list->Items[index];
}

bool rrCallConv getIntegerListItem (RRListItemHandle item, int *value)
{
    if (item->ItemType == litInteger)
    {
        *value = item->data.iValue;
        return true;
    }
    return false;
}

bool rrCallConv getDoubleListItem (RRListItemHandle item, double *value)
{
    if (item->ItemType == litDouble)
    {
    	*value = item->data.dValue;
     	return true;
    }

    return false;
}

char* rrCallConv getStringListItem (RRListItemHandle item)
{
	return (item->ItemType == litString) ? item->data.sValue : NULL;
}

RRListHandle rrCallConv getList (RRListItemHandle item)
{
	return (item->ItemType == litList) ? item->data.lValue : NULL;
}

bool rrCallConv isListItem (RRListItemHandle item, ListItemType itemType)
{
	return  (item->ItemType == itemType) ? true : false;
}

int rrCallConv getListLength (RRListHandle myList)
{
	return myList->Count;
}

char* rrCallConv listToString (RRListHandle list)
{
	try
    {
        if(!list)
        {
            return NULL;
        }

        //Types of list items
        char*           cVal;
        int             intVal;
        double          dVal;
        RRList*        lVal; 		//list is nested list
		stringstream resStr;
        resStr<<"{";
	    for(int i = 0; i < list->Count; i++)
        {
			switch(list->Items[i]->ItemType)
            {
                case litString:
					cVal = list->Items[i]->data.sValue;
                    resStr<<"\"";
                    if(cVal)
                    {
                    	resStr<<cVal;
                    }
                    resStr<<"\"";
                break;

                case litInteger:
					intVal = list->Items[i]->data.iValue;
                    resStr<< (intVal);
                break;

                case litDouble:
					dVal =  list->Items[i]->data.dValue;
                    resStr<< (dVal);
                break;

                case litList:
					lVal = list->Items[i]->data.lValue;
                    if(lVal)
                    {
                    	char* text = listToString(lVal);
                    	resStr<<text;
                        freeText(text);
                    }
                    else
                    {
						resStr<<"{}";
                    }
                break;
            }

            if(i < list->Count -1)
            {
                resStr<<",";
            }
        }
        resStr<<"}";
        return createText(resStr.str());

    }
    catch(Exception& ex)
    {
        stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
}


// Free Functions =====================================================
bool rrCallConv freeMatrix(RRMatrixHandle matrix)
{
	try
    {
        if(matrix)
        {
            delete [] (matrix->Data);
            delete matrix;
        }
        return true;
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
	    return false;
    }
}

bool rrCallConv freeRRData(RRDataHandle handle)
{
	try
    {
    	delete [] handle->Data;

        for(int i = 0; i < handle->CSize; i++)
        {
        	freeText(handle->ColumnHeaders[i]);
        }

        delete [] handle->ColumnHeaders;

        delete handle;
        return true;
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
	    return false;
    }
}

bool rrCallConv freeText(char* text)
{
	try
    {
        delete [] text;
        return true;
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
    	return false;
    }
}

bool rrCallConv freeStringArray(RRStringArrayHandle sl)
{
	try
    {
    	delete sl;
    	return true;
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
	    return false;
    }
}

bool rrCallConv freeVector(RRVectorHandle vector)
{
	try
    {
        if(vector)
        {
    	   delete [] vector->Data;
        }
        return true;
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
	    return false;
    }
}

bool rrCallConv freeCCode(RRCCodeHandle code)
{
	try
    {
        if(code)
        {
            delete code->Header;
            delete code->Source;
        }
		return true;
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
	    return false;
    }
}

/////////////////////////////////////////////////////////////
void rrCallConv pause()
{
    rr::pause(true, "Hit any key to continue..\n");
}

RRVectorHandle rrCallConv createVector (int size)
{
   RRVectorHandle list = new RRVector;
   list->Count = size;
   list->Data = new double[list->Count];
   return list;
}

int rrCallConv getVectorLength (RRVectorHandle vector)
{
	if (vector == NULL)
    {
		setError ("Vector argument is null in getVectorLength");
		return -1;
	}
	else
    {
		return vector->Count;
    }
}

bool rrCallConv getVectorElement (RRVectorHandle vector, int index, double *value)
{
	if (vector == NULL)
    {
		setError ("Vector argument is null in getVectorElement");
		return false;
	}

	if ((index < 0) || (index >= vector->Count))
    {
		stringstream msg;
		msg << "Index out range in getVectorElement: " << index;
        setError(msg.str());
		return false;
	}

	*value = vector->Data[index];
	return true;
}

bool rrCallConv setVectorElement (RRVectorHandle vector, int index, double value)
{
	if (vector == NULL)
    {
		setError ("Vector argument is null in setVectorElement");
		return false;
	}

	if ((index < 0) || (index >= vector->Count))
    {
		stringstream msg;
		msg << "Index out range in setVectorElement: " << index;
        setError(msg.str());
		return false;
	}

	vector->Data[index] = value;
	return true;
}

// Matrix Routines
// ------------------------------------------------------------------------------------

RRMatrixHandle rrCallConv createRRMatrix (int r, int c)
{
   	RRMatrixHandle matrix = new RRMatrix;
   	matrix->RSize = r;
   	matrix->CSize = c;
   	int dim =  matrix->RSize * matrix->CSize;
   	if(dim)
   	{
		matrix->Data =  new double[dim];
		return matrix;
   	}
   	else
	{
        delete matrix;
		setError ("Dimensions for new RRMatrix in createRRMatrix are zero");
        return NULL;
   }
}

int rrCallConv getMatrixNumRows (RRMatrixHandle m)
{
	if (m == NULL) {
		setError ("Matrix argument is null in getMatrixNumRows");
		return -1;
	}
	return m->RSize;
}

int  rrCallConv getMatrixNumCols (RRMatrixHandle m)
{
	if (m == NULL) {
		setError ("Matrix argument is null in getMatrixNumCols");
		return -1;
	}

	return m->CSize;
}

bool rrCallConv getMatrixElement (RRMatrixHandle m, int r, int c, double* value)
{
	if (m == NULL)
    {
		setError ("Matrix argument is null in getMatrixElement");
		return false;
	}

	if ((r < 0) || (c < 0) || (r >= m->RSize) || (c >= m->CSize))
    {
		stringstream msg;
		msg << "Index out range in getMatrixElement: " << r << ", " << c;
        setError(msg.str());
		return false;
	}

	*value = m->Data[r*m->CSize + c];
	return true;
}

bool rrCallConv setMatrixElement (RRMatrixHandle m, int r, int c, double value)
{
	if (m == NULL)
    {
		setError ("Matrix argument is null in setMatrixElement");
	    return false;
	}

	if ((r < 0) || (c < 0) || (r >= m->RSize) || (c >= m->CSize))
    {
		stringstream msg;
		msg << "Index out range in setMatrixElement: " << r << ", " << c;
        setError(msg.str());
		return false;
	}

	m->Data[r*m->CSize + c] = value;
	return true;
}

int rrCallConv  getRRDataNumRows (RRDataHandle result)
{
	if (result == NULL)
    {
       setError ("result argument is null in getResultNumRows");
       return -1;
	}
	return result->RSize;
}

int  rrCallConv  getRRDataNumCols (RRDataHandle result)
{
	if (result == NULL)
    {
       setError ("result argument is null in getResultNumCols");
       return -1;
	}
	return result->CSize;
}

bool  rrCallConv getRRDataElement(RRDataHandle result, int r, int c, double *value)
{
	if (result == NULL)
    {
	   setError ("result argument is null in getResultElement");
       return false;
	}

	if ((r < 0) || (c < 0) || (r >= result->RSize) || (c >= result->CSize))
    {
		stringstream msg;
		msg << "Index out range in getResultElement: " << r << ", " << c;
        setError(msg.str());
		return false;
    }

	*value = result->Data[r*result->CSize + c];
	return true;
}

char*  rrCallConv getRRDataColumnLabel (RRDataHandle result, int column)
{
	if (result == NULL)
    {
	   setError ("result argument is null in getResultColumnLabel");
       return NULL;
	}

	if ((column < 0) || (column >= result->CSize))
    {
		stringstream msg;
		msg << "Index out range in getResultColumnLabel: " << column;
        setError(msg.str());
		return NULL;
    }

	return result->ColumnHeaders[column];
}

char* rrCallConv getCCodeHeader(RRCCodeHandle code)
{
	if (code == NULL)
    {
    	setError ("code argument is null in getCCodeHeader");
		return NULL;
    }
	return code->Header;
}

char* rrCallConv getCCodeSource(RRCCodeHandle code)
{
	if (code == NULL)
    {
        setError ("code argument is null in getCCodeSource");
		return NULL;
    }
	return code->Source;
}

//====================== DATA WRITING ROUTINES ======================
bool rrCallConv writeRRData(RRHandle rrHandle, const char* fileNameAndPath)
{
	try
    {
        RoadRunner *rr = castFrom(rrHandle);
        RoadRunnerData data;
        data = rr->getSimulationResult();

        data.writeTo(fileNameAndPath);
		return true;
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
	    return false;
    }
}

bool rrCallConv writeMultipleRRData(RRInstanceListHandle rrHandles, const char* fileNameAndPath)
{
	try
    {
        RoadRunnerList *rrs = getRRList(rrHandles);

        int rrCount = rrs->count();
        RoadRunnerData allData;
        for(int i = rrCount -1 ; i >-1 ; i--) //"Backwards" because bad plotting program..
        {
            RoadRunner* rr = (*rrs)[i];
            if(rr)
            {
            	RoadRunnerData data = rr->getSimulationResult();
	            allData.append(data);
            }
        }

        allData.writeTo(fileNameAndPath);
		return true;
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
	    return false;
    }
}

// Utility functions ==========================================================
int rrCallConv getNumberOfStringElements (const RRStringArrayHandle list)
{
	if (!list)
		return (-1);
	else
	    return (list->Count);
}

char* rrCallConv getStringElement (RRStringArrayHandle list, int index)
{
	try
	{
	  if (list == NULL)
	  {
	     return NULL;
	  }

	  if ((index < 0) || (index >= list->Count))
	  {
         setError("Index out of range");
         return NULL;
	  }

	  return createText(list->String[index]);
	}
	catch(Exception& ex)
	{
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
		return NULL;
    }
}

char* rrCallConv stringArrayToString (const RRStringArrayHandle list)
{
	try
    {
        if(!list)
        {
            return NULL;
        }

		stringstream resStr;
	    for(int i = 0; i < list->Count; i++)
        {
        	resStr<<list->String[i];;
            if(i < list->Count -1)
            {
            	resStr <<" ";
            }
        }

    	return createText(resStr.str());
    }
    catch(Exception& ex)
    {
        stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
		return NULL;
    }
}

char* rrCallConv rrDataToString(const RRDataHandle result)
{
	try
    {
        if(!result)
        {
            return NULL;
        }
		stringstream resStr;
		//RRData is a 2D matrix, and column headers (strings)
        //First header....
	    for(int i = 0; i < result->CSize; i++)
        {
        	resStr<<result->ColumnHeaders[i];
            if(i < result->CSize -1)
            {
            	resStr <<"\t";
            }
        }
        resStr<<endl;

        //Then the data
        int index = 0;
	    for(int j = 0; j < result->RSize; j++)
   	    {
		    for(int i = 0; i < result->CSize; i++)
    	    {
        		resStr<<result->Data[index++];
	            if(i < result->CSize -1)
    	        {
        	    	resStr <<"\t";
            	}
            }
	    	resStr <<"\n";
        }
        return createText(resStr.str());
    }
    catch_ptr_macro
}

char* rrCallConv matrixToString(const RRMatrixHandle matrixHandle)
{
	try
    {
        if(!matrixHandle)
        {
            return NULL;
        }

        RRMatrix& mat = *matrixHandle;
        stringstream ss;
        //ss<<"\nmatrix dimension: "<<mat.RSize<<"x"<<mat.CSize<<" --\n";
        ss<<"\n";
        for(int row = 0; row < mat.RSize; row++)
        {
            for(int col = 0; col < mat.CSize; col++)
            {
                ss<<mat.Data[row*mat.CSize + col];
                if(col < mat.CSize + 1)
                {
                    ss<<"\t";
                }
            }
            ss<<endl;
        }
        return createText(ss.str());
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
    	return NULL;
    }
}

char* rrCallConv vectorToString(RRVectorHandle vecHandle)
{
	try
    {
        if(!vecHandle)
        {
            setError("Null vector in vectorToString");
            return NULL;
        }

        RRVector& vec = *vecHandle;

        stringstream ss;
        ss<<"vector dimension: "<<vec.Count<<" \n";

        for(int index = 0; index < vec.Count; index++)
        {
            ss<<vec.Data[index];
            if(index < vec.Count + 1)
            {
                ss<<"\t";
            }
        }
        ss<<endl;
        return createText(ss.str());
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
    	return NULL;
    }
}

}



