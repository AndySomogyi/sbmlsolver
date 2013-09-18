
// Module Name
%module(docstring="The RoadRunner SBML Simulation Engine,
(c) 2009-2013 Herbert Sauro, Andy Somogyi and Totte Karlsson", "threads"=1) roadrunner


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



%feature("docstring") rr::RoadRunner "
    The main RoadRunner class.

    All three of the RoadRunner options default to the empty string, in this
    case, the default values are used.

    @param compiler: if LLVM build is enabled, the compiler defaults to LLVM.
    @param tempDir: typically ignored, only used by the old C RoadRunner.
    @param supportCodeDir: typically ignored, only used by the old C RoadRunner";

%feature("docstring") rr::RoadRunner::simulate "
    simulate the current SBML model.

    If options is null, then the current simulation settings (start time,
    end time, n steps) are used. If options is not null, then the
    current simulation settings are set to the values specified by
    options and they are used.

    @returns a numpy array with each selected output timeseries being a
    column vector, and the 0'th column is the simulation time.";


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


 //     virtual int getFloatingSpeciesAmounts(int len, int const *indx,
 //            double *values) = 0;


/* Typemap suite for (DATA_TYPE* ARGOUT_ARRAY1, DIM_TYPE DIM1)
 */

 /*
%typemap(in,numinputs=1,fragment="NumPy_Fragments")
(int len, int const* indx)
{
    
    npy_intp dims[1];
    if (!PyInt_Check($input))
    {
        const char* typestring = pytype_string($input);
        PyErr_Format(PyExc_TypeError,
                     "Int dimension expected.  '%s' given.",
                     typestring);
    SWIG_fail;
  }
    //$2 = (DIM_TYPE) PyInt_AsLong($input);
    //dims[0] = (npy_intp) $2;
    //array = PyArray_SimpleNew(1, dims, DATA_TYPECODE);
    //if (!array) SWIG_fail;
    //$1 = (DATA_TYPE*) array_data(array);
    $1 = 0;
    $2 = 0;
    $3 = &array;
}
%typemap(argout) (int len, int const* indx) 
{
    //$result = SWIG_Python_AppendOutput($result,(PyObject*)array$argnum
    int rows = $1;
    int nd = 1;
    npy_intp dims[1] = {rows};
    //double *data = (double*)malloc(sizeof(double)*rows*cols);
    //memcpy(data, ($1).getArray(), sizeof(double)*rows*cols);


        PyObject *pArray = PyArray_New(&PyArray_Type, nd, dims, NPY_DOUBLE, NULL, array$argnum, 0,
            NPY_CARRAY | NPY_OWNDATA, NULL);
    $result  = pArray;

}
 */



%init %{
import_array();
%}


%{

typedef int (rr::ExecutableModel::*getValuesPtr)(int, int const*, double*);

static PyObject* _ExecutableModel_getValues(rr::ExecutableModel *self, getValuesPtr func, 
                                            int len, int const *indx) {
    if (len <= 0) {
        len = self->getNumCompartments();
        indx = 0;
    }
    
    npy_intp dims[1] = {len};
    PyObject *array = PyArray_SimpleNew(1, dims, NPY_DOUBLE);
    
    if (!array) {
        // TODO error handling.
        return 0;
    }
    
    double *data = (double*)PyArray_DATA((PyArrayObject*)array);
    
    (self->*func)(len, indx, data);
    
    // TODO check result
    return array;
}
%}


%apply (int DIM1, int* IN_ARRAY1) {(int len, int const *indx)};

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


/*%ignore rr::ExecutableModel::getCompartmentVolumes
    virtual int 
            double *values) = 0;
    virtual int getFloatingSpeciesAmountRates(int len, int const *indx,
            double *values) = 0;
    virtual int getFloatingSpeciesConcentrations(int len, int const *indx,
            double *values) = 0;
virtual int getBoundarySpeciesConcentrations(int len, int const *indx,
            double *values) = 0;
    virtual int getGlobalParameterValues(int len, int const *indx,
            double *values) = 0;

    virtual int getReactionRates(int len, int const *indx,
                double *values) = 0;
*/

// ignore everything in ExecutableModel, and build the python bindings 
// in the extension below.




%ignore rr::ExecutableModel::getFloatingSpeciesAmounts(int, int const*, double *);
%ignore rr::ExecutableModel::setFloatingSpeciesAmounts;
%ignore rr::ExecutableModel::getFloatingSpeciesAmountRates(int, int const*, double *);
%ignore rr::ExecutableModel::getFloatingSpeciesConcentrations(int, int const*, double *);
%ignore rr::ExecutableModel::setFloatingSpeciesConcentrations;
%ignore rr::ExecutableModel::setFloatingSpeciesInitConcentrations;
%ignore rr::ExecutableModel::getFloatingSpeciesInitConcentrations(int, int const*, double *);
%ignore rr::ExecutableModel::getBoundarySpeciesAmounts(int, int const*, double *);
%ignore rr::ExecutableModel::getBoundarySpeciesConcentrations(int, int const*, double *);
%ignore rr::ExecutableModel::setBoundarySpeciesConcentrations;
%ignore rr::ExecutableModel::getGlobalParameterValues(int, int const*, double *);
%ignore rr::ExecutableModel::setGlobalParameterValues;
%ignore rr::ExecutableModel::getCompartmentVolumes(int, int const*, double *);
%ignore rr::ExecutableModel::setCompartmentVolumes;
%ignore rr::ExecutableModel::getConservedSums(int, int const*, double *);
%ignore rr::ExecutableModel::setConservedSums;
%ignore rr::ExecutableModel::getReactionRates(int, int const*, double *);
%ignore rr::ExecutableModel::evalReactionRates;
%ignore rr::ExecutableModel::convertToAmounts;
%ignore rr::ExecutableModel::computeConservedTotals;
%ignore rr::ExecutableModel::setRateRuleValues;
%ignore rr::ExecutableModel::getRateRuleValues;
%ignore rr::ExecutableModel::getStateVector;
%ignore rr::ExecutableModel::setStateVector;
%ignore rr::ExecutableModel::convertToConcentrations;
%ignore rr::ExecutableModel::updateDependentSpeciesValues;
%ignore rr::ExecutableModel::computeAllRatesOfChange;
%ignore rr::ExecutableModel::evalModel;
%ignore rr::ExecutableModel::testConstraints;
%ignore rr::ExecutableModel::print;
%ignore rr::ExecutableModel::getNumEvents;
%ignore rr::ExecutableModel::getEventTriggers;
%ignore rr::ExecutableModel::evalEvents;
%ignore rr::ExecutableModel::applyPendingEvents;
%ignore rr::ExecutableModel::evalEventRoots;
%ignore rr::ExecutableModel::getNextPendingEventTime;
%ignore rr::ExecutableModel::getPendingEventSize;
%ignore rr::ExecutableModel::resetEvents;
%ignore rr::ExecutableModel::getStoichiometry;

%include <rrRoadRunnerOptions.h>
%include <rrRoadRunner.h>
%include <rrLogger.h>
%include <rrCompiler.h>
%include <rrExecutableModel.h>


%extend rr::ExecutableModel
{
    PyObject *getFloatingSpeciesAmounts(int len, int const *indx) {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getFloatingSpeciesAmounts, 
                                          len, indx);
    }

    PyObject *getFloatingSpeciesAmounts() {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getFloatingSpeciesAmounts, 
                                          (int)0, (int const*)0);
    }

    PyObject *getFloatingSpeciesAmountRates(int len, int const *indx) {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getFloatingSpeciesAmountRates, 
                                          len, indx);
    }

    PyObject *getFloatingSpeciesAmountRates() {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getFloatingSpeciesAmountRates, 
                                          (int)0, (int const*)0);
    }

    PyObject *getFloatingSpeciesConcentrations(int len, int const *indx) {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getFloatingSpeciesConcentrations, 
                                          len, indx);
    }

    PyObject *getFloatingSpeciesConcentrations() {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getFloatingSpeciesConcentrations, 
                                          (int)0, (int const*)0);
    }
    PyObject *getBoundarySpeciesAmounts(int len, int const *indx) {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getBoundarySpeciesAmounts, 
                                          len, indx);
    }

    PyObject *getBoundarySpeciesAmounts() {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getBoundarySpeciesAmounts, 
                                          (int)0, (int const*)0);
    }
    PyObject *getBoundarySpeciesConcentrations(int len, int const *indx) {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getBoundarySpeciesConcentrations, 
                                          len, indx);
    }

    PyObject *getBoundarySpeciesConcentrations() {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getBoundarySpeciesConcentrations, 
                                          (int)0, (int const*)0);
    }
    PyObject *getGlobalParameterValues(int len, int const *indx) {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getGlobalParameterValues, 
                                          len, indx);
    }

    PyObject *getGlobalParameterValues() {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getGlobalParameterValues, 
                                          (int)0, (int const*)0);
    }
    PyObject *getCompartmentVolumes(int len, int const *indx) {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getCompartmentVolumes, 
                                          len, indx);
    }

    PyObject *getCompartmentVolumes() {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getCompartmentVolumes, 
                                          (int)0, (int const*)0);
    }
    PyObject *getConservedSums(int len, int const *indx) {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getConservedSums, 
                                          len, indx);
    }

    PyObject *getConservedSums() {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getConservedSums, 
                                          (int)0, (int const*)0);
    }
    PyObject *getReactionRates(int len, int const *indx) {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getReactionRates, 
                                          len, indx);
    }

    PyObject *getReactionRates() {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getReactionRates, 
                                          (int)0, (int const*)0);
    }
    

 
    
}
