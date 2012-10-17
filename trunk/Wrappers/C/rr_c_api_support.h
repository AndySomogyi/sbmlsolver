//---------------------------------------------------------------------------
#ifndef rr_c_api_supportH
#define rr_c_api_supportH
#include <string>
#include <vector>
#include "rr-libstruct/lsMatrix.h"
#include "rrStringList.h"
#include "rrArrayList.h"
#include "rrUtils.h"
#include "rr_c_types.h"
//---------------------------------------------------------------------------

using std::vector;
using std::string;

namespace rr_c_api
{
//Error/Warning Messages
extern const char* 	ALLOCATE_API_ERROR_MSG;

//Internal prototypes (not exported)
void                setError(const string& err);
bool                copyVector(const RRVector* source, vector<double>& dest);

char*               createText(const char* str);        //To be deleted by client using freeText
char*               createText(const string& str);      //To be deleted by client using freeText
RRVectorHandle      createVectorFromVector_double(const vector<double>& vec);
vector<double>      createVectorFromRRVector(const RRVector* vec);
RRMatrixHandle      createMatrix(const ls::DoubleMatrix* mat);

//Lists and arrays
RRStringArrayHandle createList(const rr::StringList& aList);
RRListHandle 		createList(const rr::ArrayList& aList);
}

#endif
