
// Module Name
%module("threads"=1) RoadRunner

//%module Example
// ************************************************************
// Module Includes
// ************************************************************

// These are copied directly to the .cxx file and are not parsed
// by SWIG.  Include include files or definitions that are required
// for the module to build correctly.

// C++ std::string handling
%include "std_string.i"

// C++ std::map handling
%include "std_map.i"

// C++ std::map handling
%include "std_vector.i"

//enables better handling of STL exceptions
%include "exception.i"

// correct mapping of unsigned integers
%include "rr_stdint.i"


%exception {
  try {
    $action
  } catch (const std::exception& e) {
    SWIG_exception(SWIG_RuntimeError, e.what());
  }
}

/* Convert from C --> Python */
%typemap(out) ls::DoubleMatrix {

    int rows = ($1).numRows();
    int cols = ($1).numCols();
    int nd = 2;
    npy_intp dims[2] = {rows, cols};
    double *data = (double*)malloc(sizeof(double)*rows*cols);
    memcpy(data, ($1).getArray(), sizeof(double)*rows*cols);

    PyObject *pArray = PyArray_New(&PyArray_Type, nd, dims, NPY_DOUBLE, NULL, data, 0,
            NPY_CARRAY | NPY_OWNDATA, NULL);
    $result  = pArray;
}


/* Convert from C --> Python */
%typemap(out) ls::DoubleMatrix* {

    int rows = ($1)->numRows();
    int cols = ($1)->numCols();
    int nd = 2;
    npy_intp dims[2] = {rows, cols};
    double *data = ($1)->getArray();

    PyObject *pArray = PyArray_New(&PyArray_Type, nd, dims, NPY_DOUBLE, NULL, data, 0,
            NPY_CARRAY, NULL);
    $result  = pArray;
}


/* Convert from C --> Python */
%typemap(out) RoadRunnerData* {

    int rows = 0;
    int cols = 0;
    int nd = 2;
    double *data = 0;

    if ($1)
    {
        ls::DoubleMatrix& mat = const_cast<ls::DoubleMatrix&>(($1)->getData());
        rows = mat.numRows();
        cols = mat.numCols();
        data = mat.getArray();
    }

    npy_intp dims[2] = {rows, cols};

    PyObject *pArray = PyArray_New(&PyArray_Type, nd, dims, NPY_DOUBLE, NULL, data, 0,
            NPY_CARRAY, NULL);
    $result  = pArray;
}

%{
    #define SWIG_FILE_WITH_INIT
    #include <numpy/arrayobject.h>
    #include <lsComplex.h>
    #include <lsMatrix.h>
    #include <lsLibla.h>
    #include <lsLA.h>
    #include <lsLUResult.h>
    #include <lsUtils.h>
    #include <rrCompiler.h>
    #include <rrExecutableModel.h>
    #include <rrRoadRunnerData.h>
    #include <rrRoadRunnerOptions.h>
    #include <rrRoadRunner.h>
    #include <rrLogger.h>
    #include <cstddef>
    #include <map>

    using namespace std;
    using namespace rr;
%}


%include "numpy.i"

%init %{
import_array();
%}


#define LIB_EXTERN
#define RR_DECLSPEC
#define PUGIXML_CLASS

// Ignore methods that return this proprietary homebrewd
// re-implmentation of boost::variant,
// next version will re-write these.
%ignore rr::RoadRunner::getAvailableTimeCourseSymbols();
%ignore rr::RoadRunner::getUnscaledFluxControlCoefficientIds();
%ignore rr::RoadRunner::getFluxControlCoefficientIds();
%ignore rr::RoadRunner::getUnscaledConcentrationControlCoefficientIds();
%ignore rr::RoadRunner::getConcentrationControlCoefficientIds();
%ignore rr::RoadRunner::getElasticityCoefficientIds();
%ignore rr::RoadRunner::getUnscaledElasticityCoefficientIds();
%ignore rr::RoadRunner::getAvailableSteadyStateSymbols();

%template (mapStringDouble) std::map<std::string, double>;

%ignore rr::LoggingBuffer;
%ignore rr::LogLevel;
%ignore rr::getLogger;
%ignore rr::ToUpperCase;
%ignore rr::GetHighestLogLevel;
%ignore rr::GetLogLevel;
%ignore rr::GetLogLevelAsString;
%ignore rr::getLogLevel;
%ignore rr::lShowAlways;
%ignore rr::lError;
%ignore rr::lWarning;
%ignore rr::lInfo;
%ignore rr::lDebug;
%ignore rr::lDebug1;
%ignore rr::lDebug2;
%ignore rr::lDebug3;
%ignore rr::lDebug4;
%ignore rr::lDebug5;
%ignore rr::lAny;
%ignore rr::lUser;

%include <rrRoadRunnerOptions.h>
%include <rrRoadRunner.h>
%include <rrLogger.h>
%include <rrCompiler.h>
%include <rrExecutableModel.h>



