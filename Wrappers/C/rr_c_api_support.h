#ifndef rr_c_api_supportH
#define rr_c_api_supportH
#include <string>
#include <vector>
#include "rr-libstruct/lsMatrix.h"
#include "rrStringList.h"
#include "rrNewArrayList.h"
#include "rrParameter.h"
#include "rrUtils.h"
#include "rr_c_types.h"
//---------------------------------------------------------------------------

namespace rr
{
class RoadRunner;
class RoadRunnerList;
}

namespace rr_c_api
{
using std::vector;
using std::string;

extern char* gLastError;
extern char* gInstallFolder;

//Error/Warning Messages
extern const char* 	ALLOCATE_API_ERROR_MSG;
extern const char* 	INVALID_HANDLE_ERROR_MSG;

//Internal prototypes (not exported)
void                setError(const string& err);
bool                copyVector(const RRVector* source, vector<double>& dest);
//char*               createText(const string& str);
RRVectorHandle      createVectorFromVector_double(const vector<double>& vec);
vector<double>      createVectorFromRRVector(const RRVector* vec);
RRMatrixHandle      createMatrix(const ls::DoubleMatrix* mat);

//Lists and arrays
RRStringArrayHandle createList(const rr::StringList& aList);
RRListHandle 		createList(const rr::NewArrayList& aList);

//Parameters
RRParameterHandle	createParameter(const rr::BaseParameter& para);

//Cast void* handle to RoadRunner instance pointer, throw if it fails
rr::RoadRunner* 	castFrom(RRHandle rrHandle);

//Cast
rr::RoadRunnerList* getRRList(RRInstanceListHandle handle);
}

#endif
