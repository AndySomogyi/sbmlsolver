#pragma hdrstop
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include "rrRoadRunner.h"
#include "rrRoadRunnerList.h"
#include "rrUtils.h"
#include "rrException.h"
#include "rrc_utilities.h"         // Need to include this before the support header..
#include "rrc_cpp_support.h"
#include "rrc_macros.h"
//---------------------------------------------------------------------------

// max path stuff
#if defined(_WIN32)
#include <windef.h>
#define RR_MAX_PATH MAX_PATH
#else
#include <limits.h>
#define RR_MAX_PATH PATH_MAX
#endif

namespace rrc
{
using namespace std;
using namespace rr;
using namespace rrc;

const char* ALLOCATE_API_ERROR_MSG         = "Allocate a handle to the roadrunner API before calling any API function";
const char* INVALID_HANDLE_ERROR_MSG     = "The HANDLE passed to this function was invalid";
char*       gLastError                  = NULL;
char gInstallFolderBuffer[RR_MAX_PATH] = {0};
char* gInstallFolder = gInstallFolderBuffer;

char* rrcCallConv createText(const char* text)
{
	return rr::createText(text);
}

char* rrCallConv getFileContent(const char* fName)
{
    try
    {
        string fContent = getFileContent(fName);
        return rr::createText(fContent);
    }
    catch_ptr_macro
}

bool rrCallConv compileSource(RRHandle handle, const char* sourceFileName)
{
    return true;
}

// -------------------------------------------------------------------
// List Routines
// -------------------------------------------------------------------
RRListPtr rrCallConv createRRList()
{
    RRListPtr list = new RRList;
    list->Count = 0;
    list->Items = NULL;
    return list;
}

int rrCallConv getInstanceCount(RRInstanceListPtr iList)
{
    return iList->Count;
}

RRHandle rrCallConv getRRHandle(RRInstanceListPtr iList, int index)
{
    return iList->Handle[index];
}

void rrCallConv freeRRList (RRListPtr theList)
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

RRListItemPtr rrCallConv createIntegerItem (int value)
{
    RRListItemPtr item =  new RRListItem;
    item->ItemType = litInteger;
    item->data.iValue = value;
    return item;
}

RRListItemPtr rrCallConv createDoubleItem (double value)
{
    RRListItemPtr item = new RRListItem;
    item->ItemType = litDouble;
    item->data.dValue = value;
    return item;
}

RRListItemPtr rrCallConv createStringItem (char* value)
{
    RRListItemPtr item = new RRListItem;
    item->ItemType = litString;
    item->data.sValue = rr::createText(value);
    return item;
}

RRListItemPtr rrCallConv createListItem (RRList* value)
{
    RRListItemPtr item = new RRListItem;
    item->ItemType = litList;
    item->data.lValue = value;
    return item;
}

// Add an item to a given list, returns the index to
// the item in the list. Returns -1 if it fails.
int rrCallConv addItem (RRListPtr list, RRListItemPtr *item)
{
    int n = list->Count;

    RRListItemPtr *newItems = new RRListItemPtr [n+1];
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
    RRListItemPtr *oldItems = list->Items;
    list->Items = newItems;

    delete [] oldItems;

    list->Count = n+1;
    return n;
}

bool rrCallConv isListItemInteger (RRListItemPtr item)
{
    return (item->ItemType == litInteger) ? true : false;
}

bool rrCallConv isListItemDouble (RRListItemPtr item)
{
    return (item->ItemType == litDouble) ? true : false;
}

bool rrCallConv isListItemString (RRListItemPtr item)
{
    return (item->ItemType == litString) ? true : false;
}

bool rrCallConv isListItemList (RRListItemPtr item)
{
    return (item->ItemType == litList) ? true : false;
}

RRListItemPtr rrCallConv getListItem (RRListPtr list, int index)
{
    return (index >= list->Count) ? NULL : list->Items[index];
}

bool rrCallConv getIntegerListItem (RRListItemPtr item, int *value)
{
    if (item->ItemType == litInteger)
    {
        *value = item->data.iValue;
        return true;
    }
    return false;
}

bool rrCallConv getDoubleListItem (RRListItemPtr item, double *value)
{
    if (item->ItemType == litDouble)
    {
        *value = item->data.dValue;
         return true;
    }

    return false;
}

char* rrCallConv getStringListItem (RRListItemPtr item)
{
    return (item->ItemType == litString) ? item->data.sValue : NULL;
}

RRListPtr rrCallConv getList (RRListItemPtr item)
{
    return (item->ItemType == litList) ? item->data.lValue : NULL;
}

bool rrCallConv isListItem (RRListItemPtr item, ListItemType itemType)
{
    return  (item->ItemType == itemType) ? true : false;
}

int rrCallConv getListLength (RRListPtr myList)
{
    return myList->Count;
}

char* rrCallConv listToString (RRListPtr list)
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
        RRList*        lVal;         //list is nested list
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
                        rrc::freeText(text);
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
        return rr::createText(resStr.str());

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
bool rrCallConv freeMatrix(RRDoubleMatrixPtr matrix)
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
            rr::freeText(handle->ColumnHeaders[i]);
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

        return rr::freeText(text);
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return false;
    }
}

bool rrCallConv freeStringArray(RRStringArrayPtr sl)
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

bool rrCallConv freeVector(RRVectorPtr vector)
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

RRVectorPtr rrCallConv createVector (int size)
{
   RRVectorPtr list = new RRVector;
   list->Count = size;
   list->Data = new double[list->Count];
   return list;
}

int rrCallConv getVectorLength (RRVectorPtr vector)
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

bool rrCallConv getVectorElement (RRVectorPtr vector, int index, double *value)
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

bool rrCallConv setVectorElement (RRVectorPtr vector, int index, double value)
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

RRDoubleMatrixPtr rrCallConv createRRMatrix (int r, int c)
{
       RRDoubleMatrixPtr matrix = new RRDoubleMatrix;
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
        setError ("Dimensions for new RRDoubleMatrix in createRRMatrix are zero");
        return NULL;
   }
}

int rrCallConv getMatrixNumRows (RRDoubleMatrixPtr m)
{
    if (m == NULL) {
        setError ("Matrix argument is null in getMatrixNumRows");
        return -1;
    }
    return m->RSize;
}

int  rrCallConv getMatrixNumCols (RRDoubleMatrixPtr m)
{
    if (m == NULL) {
        setError ("Matrix argument is null in getMatrixNumCols");
        return -1;
    }

    return m->CSize;
}

bool rrCallConv getMatrixElement (RRDoubleMatrixPtr m, int r, int c, double* value)
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

bool rrCallConv setMatrixElement (RRDoubleMatrixPtr m, int r, int c, double value)
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

bool rrcCallConv getComplexMatrixElement (RRComplexMatrixHandle m, int r, int c, RRComplexHandle value)
{
    if (m == NULL)
    {
        setError ("Matrix argument is null in getComplexMatrixElement");
        return false;
    }

    if ((r < 0) || (c < 0) || (r >= m->RSize) || (c >= m->CSize))
    {
        stringstream msg;
        msg << "Index out range in getComplexMatrixElement: " << r << ", " << c;
        setError(msg.str());
        return false;
    }

    value->re = m->Data[r*m->CSize + c].re;
    value->imag = m->Data[r*m->CSize + c].imag;
    return true;
}

bool rrcCallConv setComplexMatrixElement (RRComplexMatrixHandle m, int r, int c, RRComplex* value)
{
    if (m == NULL)
    {
        setError ("Matrix argument is null in setComplexMatrixElement");
        return false;
    }

    if ((r < 0) || (c < 0) || (r >= m->RSize) || (c >= m->CSize))
    {
        stringstream msg;
        msg << "Index out range in setComplexMatrixElement: " << r << ", " << c;
        setError(msg.str());
        return false;
    }

    m->Data[r*m->CSize + c].re = value->re;
    m->Data[r*m->CSize + c].imag = value->imag;
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

bool rrCallConv writeMultipleRRData(RRInstanceListPtr rrHandles, const char* fileNameAndPath)
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
int rrCallConv getNumberOfStringElements (const RRStringArrayPtr list)
{
    if (!list)
        return (-1);
    else
        return (list->Count);
}

char* rrCallConv getStringElement (RRStringArrayPtr list, int index)
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

      return rr::createText(list->String[index]);
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
}

char* rrCallConv stringArrayToString (const RRStringArrayPtr list)
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

        return rr::createText(resStr.str());
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
        return rr::createText(resStr.str());
    }
    catch_ptr_macro
}

char* rrCallConv matrixToString(const RRDoubleMatrixPtr matrixHandle)
{
    try
    {
        if(!matrixHandle)
        {
            return NULL;
        }

        RRDoubleMatrix& mat = *matrixHandle;
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
        return rr::createText(ss.str());
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return NULL;
    }
}

char* rrCallConv complexMatrixToString(const RRComplexMatrixHandle matrixHandle)
{
    try
    {
        if(!matrixHandle)
        {
            return NULL;
        }

        RRComplexMatrix& mat = *matrixHandle;
        stringstream ss;
        //ss<<"\nmatrix dimension: "<<mat.RSize<<"x"<<mat.CSize<<" --\n";
        ss<<"\n";
        for(int row = 0; row < mat.RSize; row++)
        {
            for(int col = 0; col < mat.CSize; col++)
            {
                ss<<"("<<mat.Data[row*mat.CSize + col].re<<","<<mat.Data[row*mat.CSize + col].imag<<")";
                if(col < mat.CSize + 1)
                {
                    ss<<"\t";
                }
            }
            ss<<endl;
        }
        return rr::createText(ss.str());
    }
    catch_ptr_macro
}

char* rrCallConv vectorToString(RRVectorPtr vecHandle)
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

        for(int index = 0; index < vec.Count; index++)
        {
            ss<<vec.Data[index];
            if(index < vec.Count + 1)
            {
                ss<<"\t";
            }
        }
        ss<<endl;
        return rr::createText(ss.str());
    }
    catch_ptr_macro
}

char* rrCallConv complexVectorToString(RRComplexVectorHandle vecHandle)
{
    try
    {
        if(!vecHandle)
        {
            setError("Null vector in vectorToString");
            return NULL;
        }

        RRComplexVector& vec = *vecHandle;

        stringstream ss;
        for(int index = 0; index < vec.Count; index++)
        {
        	ss<<"("<<vec.Data[index].re<<","<<vec.Data[index].imag<<")";
            if(index < vec.Count + 1)
            {
                ss<<"\t";
            }
        }
        ss<<endl;
        return rr::createText(ss.str());
    }
    catch_ptr_macro
}

}



