/**
 * @file rr_c_types.h
 * @brief roadRunner C API 2012
 * @author Totte Karlsson & Herbert M Sauro
 *
 * <--------------------------------------------------------------
 * This file is part of cRoadRunner.
 * See http://code.google.com/p/roadrunnerwork/ for more details.
 *
 * Copyright (C) 2012
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

#ifndef rr_c_typesH
#define rr_c_typesH

#if defined(__cplusplus)
extern "C"
{
#else
#include <stdio.h>
#include "rr_support/stdbool.h"
#endif

/*!@brief Void pointer to RRHandle */
typedef void* RRHandle; /*! Void pointer to RRHandle *>

/*!@struct*/
/*!@brief Structure for a simple vector of doubles */
typedef struct RRVector
{
    int             Count;  /*!< The number of elements in the vector */
    double*         Data;   /*!< Points to an array of double items */
} *RRVectorHandle;          /*!< Pointer to RRVectorHandle struct */

/*!@struct*/
/*!@brief Structure for a simple vector of strings */
typedef struct RRStringArray
{
    int             Count;  /*!< The number of elements in the string array */
    char**          String; /*!< Points to an array of string items */
} *RRStringArrayHandle;      /*!< Pointer to RRStringListHandle struct */

/*!@struct*/
/*!@brief Structure for a simple Matrix type */
typedef struct RRMatrix
{
    int             RSize;  /*!< The number of rows in the matrix */
    int             CSize;  /*!< The number of columns in the matrix */
    double*         Data;   /*!< Items in the matrix stored as a linear array. Access an element using Data[i*CSize + j],
							  where i,j represent the row and column numberof the element. Indexing is from zero */
} *RRMatrixHandle;          /*!< Pointer to RRMatrixHandle struct */

/*!@struct*/
/*!@brief Structure for the result type from the simulate calls */
typedef struct RRResult
{
    int             RSize;  /*!< The number of rows in the result matrix */
    int             CSize;  /*!< The number of columns in the result matrix */
    double*         Data;   /*!< A pointer to the data stored in the matrix. Access an element using Data[i*CSize + j] */
    char**          ColumnHeaders;   /*!< Pointer to an array of column header strings */
} *RRResultHandle;          /*!< Pointer to RRResultHandle struct */

/*!@struct*/
/*!@brief Convenient structure for storing the header and main body source for the generate simulation C code */
typedef struct RRCCode
{
    char*   Header;  /*!< Header file *.h */
    char*   Source;  /*!< Main source code, *.c */

} *RRCCodeHandle;    /*!< Pointer to RRCCodeHandle struct */

/*!@enum*/
/*!@brief The list type supports strings, integers, double and lists */
enum ListItemType {litString, litInteger, litDouble, litList};

// The above enums correspond to the currently supported types in an RRArrayList
// char, int, double, RRArrayList
// The void pointer pValue need to be casted to corresponding type to retrieve its value


struct RRList;	//Forward declaration for RRListItem
/*!@struct*/
/*!@brief A single list element type */
typedef struct RRListItem
{
    ListItemType ItemType;  	 /*!< The type of the item in this list element */
    union
    {
       int 	     		iValue;  /*!< Integer value */
	   double    		dValue;  /*!< Double value */
	   char*     		sValue;  /*!< String value */
	   RRList*  		lValue;  /*!< List value */
	} data;                      /*!< Union */
} *RRListItemHandle;             /*!< Pointer to cRRArrayListItemHandle struct */


/*!@struct*/
/*!@brief A list type, stores int, double, strings and lists */
typedef struct RRList
{
    int                   Count; /*!< The number elements in this list */
    RRListItemHandle    *Items;  /*!< A pointer to a list of items */
}  *RRListHandle;                /*!< Pointer to cRRArrayListHandle struct */


#if defined( __cplusplus)
}
#endif


#endif
