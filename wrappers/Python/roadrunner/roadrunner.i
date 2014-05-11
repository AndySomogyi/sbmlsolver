
// Module Name
%module(docstring="The RoadRunner SBML Simulation Engine, (c) 2009-2014 Andy Somogyi and Herbert Sauro",
        "threads"=1 /*, directors="1"*/) roadrunner

// most methods should leave the GIL locked, no point to extra overhead
// for fast methods. Only Roadrunner with long methods like simulate
// and load release the GIL.
%nothread;

//%feature("director") PyEventListener;

// ************************************************************
// Module Includes
// ************************************************************

// These are copied directly to the .cxx file and are not parsed
// by SWIG.  Include include files or definitions that are required
// for the module to build correctly.

%{
    #define SWIG_FILE_WITH_INIT
    #include <numpy/arrayobject.h>
    #include <lsMatrix.h>
    #include <lsLibla.h>
    #include <lsLA.h>
    #include <lsLUResult.h>
    #include <lsUtils.h>
    #include <rrCompiler.h>
    #include <ModelGenerator.h>
    #include <rrExecutableModel.h>
    #include <rrRoadRunnerData.h>
    #include <rrRoadRunnerOptions.h>
    #include <rrRoadRunner.h>
    #include <rrLogger.h>
    #include <rrConfig.h>
    #include <conservation/ConservationExtension.h>
    #include "conservation/ConservedMoietyConverter.h"
    #include <cstddef>
    #include <map>
    #include <rrVersionInfo.h>
    #include <rrException.h>
    #include <assert.h>
    #include <math.h>
    #include <cmath>
    #include <PyUtils.h>

    // make a python obj out of the C++ ExecutableModel, this is used by the PyEventListener
    // class. This function is defined later in this compilation unit.
    PyObject *ExecutableModel_NewPythonObj(rr::ExecutableModel*);
    PyObject *Integrator_NewPythonObj(rr::Integrator*);


    #include "PyEventListener.h"
    #include "PyIntegratorListener.h"

// Windows is just so special...
#ifdef _WIN32
    #define INFINITY (DBL_MAX + DBL_MAX)
    #define NAN (INFINITY - INFINITY)
    #define isnan _isnan
#else
    #include <signal.h>
    #define isnan std::isnan
#endif


    using namespace rr;
%}

%naturalvar;

// C++ std::string handling
%include "std_string.i"

// C++ std::map handling
%include "std_map.i"

// C++ std::map handling
%include "std_vector.i"

%include "std_list.i"

//enables better handling of STL exceptions
%include "exception.i"

// correct mapping of unsigned integers
%include "rr_stdint.i"

// all the documentation goes here.
%include "rr_docstrings.i"

 // the integrator listener is a shared ptr
#define SWIG_SHARED_PTR_SUBNAMESPACE tr1
%include "std_shared_ptr.i"

%shared_ptr(rr::PyIntegratorListener)



%template(IntVector) std::vector<int>;
%template(StringVector) std::vector<std::string>;
%template(StringList) std::list<std::string>;

%apply std::vector<std::string> {vector<std::string>, vector<string>, std::vector<string> };

%template(SelectionRecordVector) std::vector<rr::SelectionRecord>;
%apply std::vector<rr::SelectionRecord> {std::vector<SelectionRecord>, std::vector<rr::SelectionRecord>, vector<SelectionRecord>};

%apply std::list<std::string>& OUTPUT {std::list<std::string>};

%exception {
  try {
    $action
  } catch (const std::exception& e) {
    SWIG_exception(SWIG_RuntimeError, e.what());
  }
}

/**
 *  Convert from C --> Python
 *  copy data
 */
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


/**
 * Convert from C --> Python
 * reference roadrunner owned data.
 */
%typemap(out) const ls::DoubleMatrix* {

    int rows = ($1)->numRows();
    int cols = ($1)->numCols();
    int nd = 2;
    npy_intp dims[2] = {rows, cols};
    double *data = ($1)->getArray();

    PyObject *pArray = PyArray_New(&PyArray_Type, nd, dims, NPY_DOUBLE, NULL, data, 0,
            NPY_CARRAY, NULL);
    $result  = pArray;
}

%apply const ls::DoubleMatrix* {ls::DoubleMatrix*, DoubleMatrix*, const DoubleMatrix* };


/* Convert from C --> Python */
%typemap(out) const rr::RoadRunnerData* {

    /* int rows = 0; */
    /* int cols = 0; */
    /* int nd = 2; */
    /* double *data = 0; */

    /* if ($1) */
    /* { */
    /*     ls::DoubleMatrix& mat = const_cast<ls::DoubleMatrix&>(($1)->getData()); */
    /*     rows = mat.numRows(); */
    /*     cols = mat.numCols(); */
    /*     data = mat.getArray(); */
    /* } */

    /* npy_intp dims[2] = {rows, cols}; */

    /* PyObject *pArray = PyArray_New(&PyArray_Type, nd, dims, NPY_DOUBLE, NULL, data, 0, */
    /*         NPY_CARRAY, NULL); */


    $result  = RoadRunnerData_to_py($1);
}

%apply const rr::RoadRunnerData* {rr::RoadRunnerData*, RoadRunnerData*, const RoadRunnerData* };

/* Convert from C --> Python */
%typemap(out) std::vector<double> {

    int len = $1.size();
    npy_intp dims[1] = {len};

    PyObject *array = PyArray_SimpleNew(1, dims, NPY_DOUBLE);

    if (!array) {
        // TODO error handling.
        return 0;
    }

    double *data = (double*)PyArray_DATA((PyArrayObject*)array);

    std::vector<double>& vec = $1;

    memcpy(data, &vec[0], sizeof(double)*len);

    $result  = array;
}

%typemap(out) const rr::Variant& {
    try {
        const rr::Variant& temp = *($1);
        $result = Variant_to_py(temp);
    } catch (const std::exception& e) {
        SWIG_exception(SWIG_RuntimeError, e.what());
    }
}



%typemap(in) const rr::Variant& (rr::Variant temp) {

    try {
        temp = Variant_from_py($input);
        $1 = &temp;
    } catch (const std::exception& e) {
        SWIG_exception(SWIG_RuntimeError, e.what());
    }
}

%apply const rr::Variant& {rr::Variant&, Variant&, const Variant&};





/*
%typemap(out) std::vector<std::string> {

    int len = $1.size();

    PyObject* pyList = PyList_New(len);

    for(int i = 0; i < len; i++)
    {
        const std::string& str  = $1.at(i);
        PyObject* pyStr = PyString_FromString(str.c_str());
        PyList_SET_ITEM(pyList, i, pyStr);
    }

    $result = pyList;
}
*/

//%apply std::vector<std::string> {vector<std::string>, vector<string>, std::vector<string> };






%include "numpy.i"


%init %{
import_array();
%}

%{


#ifndef _WIN32

    static void rr_sighandler(int sig) {
        std::cout << "handling signal " << sig << std::endl;
        Log(rr::Logger::LOG_WARNING) << "signal handler : " << sig;
    }

    static unsigned long sigtrap() {
        signal(SIGTRAP, rr_sighandler);
        return raise(SIGTRAP);
    }

#else

    static unsigned long sigtrap() {
        Log(rr::Logger::LOG_WARNING) << "sigtrap not supported on Windows";
        return 0;
    }

#endif
%}





size_t sigtrap();



%{

typedef int (rr::ExecutableModel::*getValuesPtr)(int, int const*, double*);
typedef string (ExecutableModel::*getNamePtr)(int);
typedef int (ExecutableModel::*getNumPtr)();


static PyObject* _ExecutableModel_getValues(rr::ExecutableModel *self, getValuesPtr func,
                                            getNumPtr numPtr, int len, int const *indx) {
    if (len <= 0) {
        len = (self->*numPtr)();
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



static std::string strvec_to_pystring(const std::vector<std::string>& strvec) {
    std::stringstream s;
    s << "[";

    for (int i = 0; i < strvec.size(); ++i) {
        s << "'" << strvec[i] << "'";
        if (i + 1 < strvec.size()) {
            s << ",";
        }
    }

    s << "]";

    return s.str();
}

/**
 * create a numpy structured array with the column names set from
 * the column names of the RoadRunnerData object.
 */
static PyObject *RoadRunnerData_to_py(rr::RoadRunnerData* pData) {

    // a valid array descriptor:
    // In [87]: b = array(array([0,1,2,3]),
    //      dtype=[('r', 'f8'), ('g', 'f8'), ('b', 'f8'), ('a', 'f8')])

    // could be Null
    if (pData == 0) {
        Py_RETURN_NONE;
    }

    if (pData->structuredResult) {

        const std::vector<std::string> &names = pData->getColumnNames();
        ls::DoubleMatrix& mat = const_cast<ls::DoubleMatrix&>(pData->getData());

        int rows = mat.numRows();
        int cols = mat.numCols();

        if (cols == 0) {
            Py_RETURN_NONE;
        }

        double* mData = mat.getArray();

        PyObject* list = PyList_New(names.size());

        for(int i = 0; i < names.size(); ++i)
            {
                PyObject *col = PyString_FromString(names[i].c_str());
                PyObject *type = PyString_FromString("f8");
                PyObject *tup = PyTuple_Pack(2, col, type);

                Py_DECREF(col);
                Py_DECREF(type);

                void PyList_SET_ITEM(list, i, tup);
            }

        PyArray_Descr* descr = 0;
        PyArray_DescrConverter(list, &descr);

        // done with list
        Py_CLEAR(list);

        npy_intp dims[] = {rows};

        // steals a reference to descr
        PyObject *result = PyArray_SimpleNewFromDescr(1, dims,  descr);

        if (result) {

            assert(PyArray_NBYTES(result) == rows*cols*sizeof(double) && "invalid array size");

            double* data = (double*)PyArray_BYTES(result);

            memcpy(data, mData, rows*cols*sizeof(double));
        }

        return result;
    }
    else {

        ls::DoubleMatrix& mat = const_cast<ls::DoubleMatrix&>(pData->getData());
        int rows = mat.numRows();
        int cols = mat.numCols();
        int nd = 2;

        if (cols == 0) {
            Py_RETURN_NONE;
        }

        npy_intp dims[2] = {rows, cols};
        double *data = (double*)malloc(sizeof(double)*rows*cols);
        memcpy(data, mat.getArray(), sizeof(double)*rows*cols);

        PyObject *pArray = PyArray_New(&PyArray_Type, nd, dims, NPY_DOUBLE, NULL, data, 0,
                                       NPY_CARRAY | NPY_OWNDATA, NULL);
        return pArray;
    }
};


// make a python obj out of the C++ ExecutableModel, this is used by the PyEventListener
// class. This function is defined later in this compilation unit.

PyObject *ExecutableModel_NewPythonObj(rr::ExecutableModel* e) {
    return SWIG_NewPointerObj(SWIG_as_voidptr(e), SWIGTYPE_p_rr__ExecutableModel, 0 |  0 );
}

PyObject *Integrator_NewPythonObj(rr::Integrator* i) {
    return SWIG_NewPointerObj(SWIG_as_voidptr(i), SWIGTYPE_p_rr__Integrator, 0 |  0 );
}



%}


%apply (int DIM1, int* IN_ARRAY1) {(int len, int const *indx)};

%apply (int DIM1, double* IN_ARRAY1) {(int len, double const *values)};

// typemap for the set***Values methods
%apply (int DIM1, int* IN_ARRAY1) {(int leni, int const* indx)};
%apply (int DIM1, double* IN_ARRAY1) {(int lenv, const  double* values)};

// typemap for getStateVector, getStateVectorRate
%apply (int DIM1, double* IN_ARRAY1)      {(int in_len, double const *in_values)};
%apply (int DIM1, double* INPLACE_ARRAY1) {(int out_len, double* out_values)};

#define LIB_EXTERN
#define RR_DECLSPEC
#define PUGIXML_CLASS



%warnfilter(509) rr::RoadRunner::setSelections;
%warnfilter(509) rr::RoadRunner::setSteadyStateSelections;

// Many of the RoadRunner methods will be ignored for the time being
// as currently we do not have a clean mapping to Python.


%ignore rr::RoadRunner::addCapabilities;
%ignore rr::RoadRunner::getFloatingSpeciesIds;
%ignore rr::RoadRunner::getRateOfChangeIds;
//%ignore rr::RoadRunner::getuCC;
%ignore rr::RoadRunner::addCapability;
%ignore rr::RoadRunner::getFloatingSpeciesInitialConcentrationByIndex;
%ignore rr::RoadRunner::getRatesOfChange;
//%ignore rr::RoadRunner::getuEE;
%ignore rr::RoadRunner::changeInitialConditions;
%ignore rr::RoadRunner::getFloatingSpeciesInitialConcentrations;
%ignore rr::RoadRunner::getRatesOfChangeEx;
%ignore rr::RoadRunner::initializeModel;
%ignore rr::RoadRunner::computeAndAssignConservationLaws;
%ignore rr::RoadRunner::getFloatingSpeciesInitialConditionIds;
%ignore rr::RoadRunner::getReactionIds;
//%ignore rr::RoadRunner::isModelLoaded;
//%ignore rr::RoadRunner::computeSteadyStateValue;
//%ignore rr::RoadRunner::getFullJacobian;
%ignore rr::RoadRunner::getReactionRate;
//%ignore rr::RoadRunner::loadSBML;
%ignore rr::RoadRunner::computeSteadyStateValues;
%ignore rr::RoadRunner::getFullReorderedJacobian;
%ignore rr::RoadRunner::getReactionRates;
//%ignore rr::RoadRunner::loadSBMLFromFile;
%ignore rr::RoadRunner::correctMaxStep;
//%ignore rr::RoadRunner::getFullyReorderedStoichiometryMatrix;
%ignore rr::RoadRunner::getReactionRatesEx;
%ignore rr::RoadRunner::loadSimulationSettings;
//%ignore rr::RoadRunner::createDefaultSelectionLists;
%ignore rr::RoadRunner::getGlobalParameterByIndex;
//%ignore rr::RoadRunner::getReducedJacobian;
//%ignore rr::RoadRunner::oneStep;
//%ignore rr::RoadRunner::createTimeCourseSelectionList;
%ignore rr::RoadRunner::getGlobalParameterIds;
//%ignore rr::RoadRunner::getReorderedStoichiometryMatrix;
//%ignore rr::RoadRunner::reset;
%ignore rr::RoadRunner::evalModel;
%ignore rr::RoadRunner::getGlobalParameterValues;
//%ignore rr::RoadRunner::getSBML;
%ignore rr::RoadRunner::setBoundarySpeciesByIndex;
%ignore rr::RoadRunner::getBoundarySpeciesAmountIds;
//%ignore rr::RoadRunner::getInfo;
//%ignore rr::RoadRunner::getScaledConcentrationControlCoefficientMatrix;
%ignore rr::RoadRunner::setBoundarySpeciesConcentrations;
%ignore rr::RoadRunner::getBoundarySpeciesByIndex;
//%ignore rr::RoadRunner::getInstanceCount;
//%ignore rr::RoadRunner::getScaledFloatingSpeciesElasticity;
%ignore rr::RoadRunner::setCapabilities;
%ignore rr::RoadRunner::getBoundarySpeciesConcentrations;
//%ignore rr::RoadRunner::getInstanceID;
//%ignore rr::RoadRunner::getScaledFluxControlCoefficientMatrix;
%ignore rr::RoadRunner::setCompartmentByIndex;
%ignore rr::RoadRunner::getBoundarySpeciesIds;
//%ignore rr::RoadRunner::getIntegrator;
//%ignore rr::RoadRunner::getScaledReorderedElasticityMatrix;
%ignore rr::RoadRunner::setCompiler;
//%ignore rr::RoadRunner::getCC;
//%ignore rr::RoadRunner::getL0Matrix;
//%ignore rr::RoadRunner::getSelectedValues;
%ignore rr::RoadRunner::setFloatingSpeciesByIndex;
//%ignore rr::RoadRunner::getCapabilitiesAsXML;
//%ignore rr::RoadRunner::getLinkMatrix;
//%ignore rr::RoadRunner::getSelectionList;
%ignore rr::RoadRunner::setFloatingSpeciesConcentrations;
%ignore rr::RoadRunner::getCapability;
%ignore rr::RoadRunner::getListOfCapabilities;
//%ignore rr::RoadRunner::getSimulationResult;
%ignore rr::RoadRunner::setFloatingSpeciesInitialConcentrationByIndex;
%ignore rr::RoadRunner::getCompartmentByIndex;
%ignore rr::RoadRunner::getListOfParameters;
//%ignore rr::RoadRunner::getSteadyStateSelection;
%ignore rr::RoadRunner::setFloatingSpeciesInitialConcentrations;
%ignore rr::RoadRunner::getCompartmentIds;
//%ignore rr::RoadRunner::getModel;
//%ignore rr::RoadRunner::getSteadyStateSelectionList;
%ignore rr::RoadRunner::setGlobalParameterByIndex;
//%ignore rr::RoadRunner::getCompiler;
//%ignore rr::RoadRunner::getModelGenerator;
//%ignore rr::RoadRunner::getStoichiometryMatrix;
%ignore rr::RoadRunner::setNumPoints;
//%ignore rr::RoadRunner::getConservationMatrix;
%ignore rr::RoadRunner::getModelName;
%ignore rr::RoadRunner::getTempFolder;
%ignore rr::RoadRunner::setParameterValue;
%ignore rr::RoadRunner::getConservedMoietyIds;
//%ignore rr::RoadRunner::getNrMatrix;
//%ignore rr::RoadRunner::getTimeCourseSelectionList;
%ignore rr::RoadRunner::setSimulationSettings;
%ignore rr::RoadRunner::getConservedMoietyValues;
%ignore rr::RoadRunner::getNumPoints;
%ignore rr::RoadRunner::getTimeEnd;
//%ignore rr::RoadRunner::setSteadyStateSelectionList;
//%ignore rr::RoadRunner::getCopyright;
%ignore rr::RoadRunner::getNumberOfBoundarySpecies;
%ignore rr::RoadRunner::getTimeStart;
%ignore rr::RoadRunner::setTempFileFolder;
//%ignore rr::RoadRunner::getDescription;
%ignore rr::RoadRunner::getNumberOfCompartments;
//%ignore rr::RoadRunner::getURL;
//%ignore rr::RoadRunner::setTimeCourseSelectionList;
//%ignore rr::RoadRunner::getEE;
%ignore rr::RoadRunner::getNumberOfDependentSpecies;
//%ignore rr::RoadRunner::getUnscaledConcentrationControlCoefficientMatrix;
%ignore rr::RoadRunner::setTimeEnd;
//%ignore rr::RoadRunner::getEigenvalueIds;
%ignore rr::RoadRunner::getNumberOfFloatingSpecies;
//%ignore rr::RoadRunner::getUnscaledElasticityMatrix;
%ignore rr::RoadRunner::setTimeStart;
//%ignore rr::RoadRunner::getEigenvalues;
%ignore rr::RoadRunner::getNumberOfGlobalParameters;
//%ignore rr::RoadRunner::getUnscaledFluxControlCoefficientMatrix;
//%ignore rr::RoadRunner::setValue;
%ignore rr::RoadRunner::getEigenvaluesCpx;
%ignore rr::RoadRunner::getNumberOfIndependentSpecies;
//%ignore rr::RoadRunner::getUnscaledSpeciesElasticity;
//%ignore rr::RoadRunner::simulate;
//%ignore rr::RoadRunner::getExtendedVersionInfo;
%ignore rr::RoadRunner::getNumberOfReactions;
//%ignore rr::RoadRunner::getValue;
//%ignore rr::RoadRunner::steadyState;
%ignore rr::RoadRunner::getValue(const SelectionRecord&);
//%ignore rr::RoadRunner::this;
%ignore rr::RoadRunner::getFloatingSpeciesByIndex;
%ignore rr::RoadRunner::getParameterValue;
//%ignore rr::RoadRunner::getVersion;
%ignore rr::RoadRunner::unLoadModel;
%ignore rr::RoadRunner::getFloatingSpeciesConcentrations;
%ignore rr::RoadRunner::getRateOfChange;
//%ignore rr::RoadRunner::getlibSBMLVersion;
//%ignore rr::RoadRunner::writeSBML;
%ignore rr::RoadRunner::getSimulateOptions;
%ignore rr::RoadRunner::setSimulateOptions;
%ignore rr::RoadRunner::getIds(int types, std::list<std::string> &);

%ignore rr::RoadRunner::getOptions;

%rename (_simulate) rr::RoadRunner::simulate;


%ignore rr::Config::getInt;
%ignore rr::Config::getString;
%ignore rr::Config::getBool;
%ignore rr::Config::getDouble;



// rename these, the injected python code will take care of
// making these properties.
%rename (_getSelections) rr::RoadRunner::getSelections();
%rename (_setSelections) setSelections(const std::vector<rr::SelectionRecord>&);
%rename (_setSelections) setSelections(const std::vector<std::string>&);
%rename (_getModel) getModel();

%rename (_getSteadyStateSelections) rr::RoadRunner::getSteadyStateSelections();
%rename (_setSteadyStateSelections) setSteadyStateSelections(const std::vector<rr::SelectionRecord>&);
%rename (_setSteadyStateSelections) setSteadyStateSelections(const std::vector<std::string>&);
%rename (_getConservedMoietyAnalysis) getConservedMoietyAnalysis();
%rename (_setConservedMoietyAnalysis) setConservedMoietyAnalysis(bool);

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


%ignore rr::ExecutableModel::getFloatingSpeciesAmounts(int, int const*, double *);
%ignore rr::ExecutableModel::setFloatingSpeciesAmounts(int len, int const *indx, const double *values);
%ignore rr::ExecutableModel::getFloatingSpeciesAmountRates(int, int const*, double *);
%ignore rr::ExecutableModel::getFloatingSpeciesConcentrationRates(int, int const*, double *);

%ignore rr::ExecutableModel::getFloatingSpeciesConcentrations(int, int const*, double *);
%ignore rr::ExecutableModel::setFloatingSpeciesConcentrations(int len, int const *indx, const double *values);
%ignore rr::ExecutableModel::setFloatingSpeciesInitConcentrations(int len, int const *indx, const double *values);
%ignore rr::ExecutableModel::getFloatingSpeciesInitConcentrations(int, int const*, double *);
%ignore rr::ExecutableModel::getBoundarySpeciesAmounts(int, int const*, double *);
%ignore rr::ExecutableModel::getBoundarySpeciesConcentrations(int, int const*, double *);
%ignore rr::ExecutableModel::setBoundarySpeciesConcentrations(int len, int const *indx, const double *values);
%ignore rr::ExecutableModel::getGlobalParameterValues(int, int const*, double *);
%ignore rr::ExecutableModel::setGlobalParameterValues(int len, int const *indx, const double *values);
%ignore rr::ExecutableModel::getCompartmentVolumes(int, int const*, double *);
%ignore rr::ExecutableModel::setCompartmentVolumes(int len, int const *indx, const double *values);
%ignore rr::ExecutableModel::getConservedMoietyValues(int, int const*, double *);
%ignore rr::ExecutableModel::setConservedMoietyValues(int len, int const *indx, const double *values);
%ignore rr::ExecutableModel::getReactionRates(int, int const*, double *);
%ignore rr::ExecutableModel::evalReactionRates;
%ignore rr::ExecutableModel::convertToAmounts;
%ignore rr::ExecutableModel::computeConservedTotals;
%ignore rr::ExecutableModel::setRateRuleValues;
%ignore rr::ExecutableModel::getRateRuleValues;
%ignore rr::ExecutableModel::getStateVector(double *stateVector);
%ignore rr::ExecutableModel::setStateVector;
%ignore rr::ExecutableModel::convertToConcentrations;
%ignore rr::ExecutableModel::updateDependentSpeciesValues;
%ignore rr::ExecutableModel::computeAllRatesOfChange;
%ignore rr::ExecutableModel::getStateVectorRate(double time, const double *y, double* dydt);
%ignore rr::ExecutableModel::getStateVectorRate(double time, const double *y);
%ignore rr::ExecutableModel::testConstraints;
%ignore rr::ExecutableModel::print;
//%ignore rr::ExecutableModel::getNumEvents;
%ignore rr::ExecutableModel::getEventTriggers;
%ignore rr::ExecutableModel::evalEvents;
%ignore rr::ExecutableModel::applyPendingEvents;
%ignore rr::ExecutableModel::evalEventRoots;
%ignore rr::ExecutableModel::getNextPendingEventTime;
%ignore rr::ExecutableModel::getPendingEventSize;
%ignore rr::ExecutableModel::resetEvents;

%ignore rr::ExecutableModel::getFloatingSpeciesId(int index);
%ignore rr::ExecutableModel::getBoundarySpeciesId(int index);
%ignore rr::ExecutableModel::getGlobalParameterId(int index);
%ignore rr::ExecutableModel::getCompartmentId(int index);
%ignore rr::ExecutableModel::getConservedMoietyId(int index);
%ignore rr::ExecutableModel::getReactionId(int index);

%ignore rr::ExecutableModel::getFloatingSpeciesIndex(const std::string& eid);
%ignore rr::ExecutableModel::getBoundarySpeciesIndex(const std::string &eid);
%ignore rr::ExecutableModel::getGlobalParameterIndex(const std::string& eid);
%ignore rr::ExecutableModel::getCompartmentIndex(const std::string& eid);
%ignore rr::ExecutableModel::getConservedMoietyIndex(const std::string& eid);
%ignore rr::ExecutableModel::getReactionIndex(const std::string& eid);

%ignore rr::ExecutableModel::getStoichiometryMatrix(int*, int*, double**);

%ignore rr::ExecutableModel::getConservedSumChanged();
%ignore rr::ExecutableModel::computeConservedTotals();
%ignore rr::ExecutableModel::setConservedSumChanged(bool);
%ignore rr::ExecutableModel::convertToAmounts() ;

%ignore rr::ExecutableModel::setFloatingSpeciesInitConcentrations(int len, int const *indx, double const *values);
%ignore rr::ExecutableModel::getFloatingSpeciesInitConcentrations(int len, int const *indx, double *values);
%ignore rr::ExecutableModel::setFloatingSpeciesInitAmounts(int len, int const *indx, double const *values);
%ignore rr::ExecutableModel::getFloatingSpeciesInitAmounts(int len, int const *indx, double *values);
%ignore rr::ExecutableModel::setCompartmentInitVolumes(int len, int const *indx, double const *values);
%ignore rr::ExecutableModel::getCompartmentInitVolumes(int len, int const *indx, double *values);
%ignore rr::ExecutableModel::getIds(int, std::list<std::string> &);

// map the events to python using the PyEventListener class
%ignore rr::ExecutableModel::setEventListener(int, rr::EventListenerPtr);
%ignore rr::ExecutableModel::getEventListener(int);
%ignore rr::EventListenerPtr;
%ignore rr::EventListenerException;

// ignore the EventListener virtuals, but leave the enum
%ignore rr::EventListener::onTrigger(ExecutableModel* model, int eventIndex, const std::string& eventId);
%ignore rr::EventListener::onAssignment(ExecutableModel* model, int eventIndex, const std::string& eventId);

// ignore the C++ class, only deal with the python version
%ignore rr::IntegratorListener;

%ignore rr::Integrator::setListener(rr::IntegratorListenerPtr);
%ignore rr::Integrator::getListener();

//%ignore rr::Integrator::addIntegratorListener;
//%ignore rr::Integrator::removeIntegratorListener;

%rename rr::conversion::ConservedMoietyConverter PyConservedMoietyConverter;

%ignore rr::ostream;
%ignore ostream;
%ignore std::ostream;
%ignore operator<<(ostream&, const rr::SelectionRecord& rec);
%ignore operator<<(rr::ostream&, const rr::SelectionRecord& rec);

// ignore all instances of the Configurable methods.
%ignore *::createConfigNode;
%ignore *::loadConfig;

// Warning 389: operator[] ignored (consider using %extend)
// Warning 401: Nothing known about base class 'Configurable'. Ignored.


// Warning 315: Nothing known about 'std::ostream'.
namespace std { class ostream{}; }


// Warning 401: Nothing known about base class 'Configurable'. Ignored.


/**
 * include the roadrunner files here, this is where the wrappers are generated.
 */

//%import(module="roadrunner") "Configurable.h"
//%ignore rr::Configurable;

//namespace rr { class Configurable{}; }

%ignore rr::Configurable;
%include <Configurable.h>

%include <rrRoadRunnerOptions.h>
%include <rrLogger.h>
%include <rrCompiler.h>
%include <rrExecutableModel.h>
%include <ModelGenerator.h>
%include <rrVersionInfo.h>

%thread;
%include <rrRoadRunner.h>
%nothread;

%include <rrSelectionRecord.h>
%include <conservation/ConservedMoietyConverter.h>
%include <Integrator.h>

%include "PyEventListener.h"
%include "PyIntegratorListener.h"
%include <rrConfig.h>

%extend std::vector<rr::SelectionRecord>
{
    std::string __repr__() {
        std::stringstream s;
        std::vector<rr::SelectionRecord> &p = *($self);

        s << "[";

        for (int i = 0; i < p.size(); ++i)
        {
            s << "\"" << p[i].to_string() << "\"";

            if (i + 1 < p.size())
            {
                s << ", ";
            }
        }

        s << "]";

        return s.str();
    }
}



%extend rr::RoadRunner
{
    // attributes

    /**
     * make some of these const so SWIG would not allow setting.
     */
    const rr::SimulateOptions *simulateOptions;

    rr::RoadRunnerOptions *options;

    std::string __repr__() {
        std::stringstream s;
        s << "<roadrunner.RoadRunner() { this = " << (void*)$self << " }>";
        return s.str();
    }

    double getValue(const rr::SelectionRecord* pRecord) {
        return $self->getValue(*pRecord);
    }

    double __getitem__(const std::string& id) {
        return ($self)->getValue(id);
    }

    void __setitem__(const std::string& id, double value) {
        ($self)->setValue(id, value);
    }

    PyObject *getIds(int types) {
        std::list<std::string> ids;

        ($self)->getIds(types, ids);

        unsigned size = ids.size();

        PyObject* pyList = PyList_New(size);

        unsigned j = 0;

        for (std::list<std::string>::const_iterator i = ids.begin(); i != ids.end(); ++i)
        {
            const std::string& id  = *i;
            PyObject* pyStr = PyString_FromString(id.c_str());
            PyList_SET_ITEM(pyList, j++, pyStr);
        }

        return pyList;
    }


   %pythoncode %{
        def getModel(self):
            if self.options.disablePythonDynamicProperties:
                return self._getModel()
            else:
                m = self._getModel();
                m._makeProperties()
                return m

        __swig_getmethods__["selections"] = _getSelections
        __swig_setmethods__["selections"] = _setSelections
        __swig_getmethods__["steadyStateSelections"] = _getSteadyStateSelections
        __swig_setmethods__["steadyStateSelections"] = _setSteadyStateSelections
        __swig_getmethods__["conservedMoietyAnalysis"] = _getConservedMoietyAnalysis
        __swig_setmethods__["conservedMoietyAnalysis"] = _setConservedMoietyAnalysis
        __swig_getmethods__["model"] = _getModel
        __swig_getmethods__["integrator"] = getIntegrator

        if _newclass:
            selections = property(_getSelections, _setSelections)
            steadyStateSelections = property(_getSteadyStateSelections, _setSteadyStateSelections)
            conservedMoietyAnalysis=property(_getConservedMoietyAnalysis, _setConservedMoietyAnalysis)
            model = property(getModel)
            integrator = property(getIntegrator)


        def keys(self, types=_roadrunner.SelectionRecord_ALL):
            return self.getIds(types)

        def values(self, types=_roadrunner.SelectionRecord_ALL):
            return [self.getValue(k) for k in self.keys(types)]

        def items(self, types=_roadrunner.SelectionRecord_ALL):
            return [(k, self.getValue(k)) for k in self.keys(types)]

        def __len__(self):
            return len(self.keys())

        def iteritems(self, types=_roadrunner.SelectionRecord_ALL):
            """
            return an iterator over (key, value) pairs
            """
            return self.items(types).__iter__()

        def iterkeys(self, types=_roadrunner.SelectionRecord_ALL):
            """
            return an iterator over the mapping's keys
            """
            return self.keys(types).__iter__()

        def itervalues(self, types=_roadrunner.SelectionRecord_ALL):
            """
            return an iterator over the mapping's values
            """
            return self.values(types).__iter__()


        def simulate(self, *args, **kwargs):
            """
            Simulate the optionally plot current SBML model.

            There are a number of ways to call simulate.

            1. With no arguments. In this case, the current set of `SimulateOptions` will
            be used for the simulation. The current set may be changed either directly
            via setSimulateOptions() or with one of the two alternate ways of calling
            simulate.

            2: With single `SimulateOptions` argument. In this case, all of the settings
            in the given options are copied and will be used for the current and future
            simulations.

            3: With the three positions arguments, `timeStart`, `timeEnd`, `steps`. In this case
            these three values are copied and will be used for the current and future simulations.

            4: With keyword arguments where keywords are the property names of the SimulateOptions
            class. To reset the model, simulate from 0 to 10 in 1000 steps and plot we can::

                rr.simulate(end=10, start=0, steps=1000, resetModel=True, plot=True)

            The options given in the 2nd and 3rd forms will remain in effect until changed. So, if
            one calls::

                rr.simulate (0, 3, 100)

            The start time of 0, end time of 3 and steps of 100 will remain in effect, so that if this
            is followed by a call to::

                rr.simulate()

            This simulation will use the previous values.

            :returns: a numpy array with each selected output time series being a
             column vector, and the 0'th column is the simulation time.
            :rtype: numpy.ndarray
            """


            doPlot = False
            show = True
            o = self.simulateOptions

            # check if we have just a sim options
            if len(args) >= 1:
                if type(args[0]) == type(self.simulateOptions):
                    o = args[0]
                else:
                    o.start = args[0]

            # second arg is treated as sim end time
            if len(args) >= 2:
                o.end = args[1]

            # third arg is steps
            if len(args) >= 3:
                o.steps = args[2]

            for k,v in kwargs.iteritems():

                if k == "integrator" and type(v) == str:
                    if v.lower() == "gillespie":
                        o.integrator = SimulateOptions.GILLESPIE
                    elif v.lower() == "cvode":
                        o.integrator = SimulateOptions.CVODE
                    else:
                        raise Exception("{0} is invalid argument for integrator".format(v))
                    continue

                if SimulateOptions.__dict__.has_key(k):
                    setattr(o, k, v)
                    continue

                if k == "plot":
                    doPlot = v
                    continue

                if k == "show":
                    show = v
                    continue

                raise Exception("{0} is not a valid keyword argument".format(k))

            result = self._simulate(o)

            if doPlot:
                self.plot(show)

            return result

        def plot(self, show=True):
            """
            RoadRunner.plot([show])

            Plot the previously run simulation result using Matplotlib.

            This takes the contents of the simulation result and builds a
            legend from the selection list.


            If the optional prameter 'show' [default is True] is given, the pylab
            show() method is called.
            """

            import pylab as p

            result = self.getSimulationResult()

            if result is None:
                raise Exception("no simulation result")

            # check if standard numpy array
            if result.dtype.names is None:

                selections = self.selections

                if len(result.shape) != 2 or result.shape[1] != len(selections):
                    raise Exception("simulation result columns not equal to number of selections, likely a simulation has not been run")

                times = result[:,0]

                for i in range(1, len(selections)):
                    series = result[:,i]
                    name = selections[i]
                    p.plot(times, series, label='$' + str(name) + '$')

            # result is structured array
            else:
                if len(result.dtype.names) < 1:
                    raise Exception('no columns to plot')

                time = result.dtype.names[0]

                for name in result.dtype.names[1:]:
                    p.plot(result[time], result[name], label='$' + name + '$')

            p.legend()

            if show:
                p.show()


    %}
}

%{
    rr::SimulateOptions* rr_RoadRunner_simulateOptions_get(RoadRunner* r) {
        return &r->getSimulateOptions();
    }

    void rr_RoadRunner_simulateOptions_set(RoadRunner* r, const rr::SimulateOptions* opt) {
        r->setSimulateOptions(*opt);
    }


    rr::RoadRunnerOptions* rr_RoadRunner_options_get(RoadRunner* r) {
        return &r->getOptions();
    }

    void rr_RoadRunner_options_set(RoadRunner* r, const rr::RoadRunnerOptions* opt) {
        rr::RoadRunnerOptions *rropt = &r->getOptions();
        *rropt = *opt;
    }
%}


%extend rr::SelectionRecord {

    std::string __repr__() {
        return $self->to_repr();
    }

    std::string __str__() {
        return $self->to_string();
    }
}

%extend rr::SimulateOptions
{
    double end;
    bool resetModel;
    bool stiff;
    bool multiStep;
    bool structuredResult;
    bool variableStep;
    rr::SimulateOptions::Integrator integrator;

    std::string __repr__() {
        std::stringstream s;
        s << "<roadrunner.SimulateOptions() { this = " << (void*)$self << " }>";
        return s.str();
    }

    int test() {
        std::cout << "sizeof: " << sizeof(rr::SimulateOptions) << std::endl;
        std::cout << "integrator: " << $self->integrator;
        return 0;
    }

    std::string __str__() {
        std::stringstream s;
        s << "{ 'flags' : " << $self->flags;
        s << ", 'integrator' : " << $self->integrator;
        s << ", 'integratorFlags' : " << $self->integratorFlags;
        s << ", 'steps' : " << $self->steps;
        s << ", 'start' : " << $self->start;
        s << ", 'duration' : " << $self->duration;
        s << ", 'absolute' : " << $self->absolute;
        s << ", 'relative' : " << $self->relative;
        s << ", 'variables' : " << strvec_to_pystring($self->variables);
        s << ", 'amounts' : " << strvec_to_pystring($self->amounts);
        s << ", 'concentrations' : " << strvec_to_pystring($self->concentrations);
        s << "}";
        return s.str();
    }
}

%{
    double rr_SimulateOptions_end_get(SimulateOptions* opt) {
        return opt->start + opt->duration;
    }

    void rr_SimulateOptions_end_set(SimulateOptions* opt, double end) {
        opt->duration = end - opt->start;
    }

    bool rr_SimulateOptions_resetModel_get(SimulateOptions* opt) {
        return opt->flags & SimulateOptions::RESET_MODEL;
    }

    void rr_SimulateOptions_resetModel_set(SimulateOptions* opt, bool value) {
        if (value) {
            opt->flags |= SimulateOptions::RESET_MODEL;
        } else {
            opt->flags &= !SimulateOptions::RESET_MODEL;
        }
    }

    bool rr_SimulateOptions_structuredResult_get(SimulateOptions* opt) {
        return opt->flags & SimulateOptions::STRUCTURED_RESULT;
    }

    void rr_SimulateOptions_structuredResult_set(SimulateOptions* opt, bool value) {
        if (value) {
            opt->flags |= SimulateOptions::STRUCTURED_RESULT;
        } else {
            opt->flags &= !SimulateOptions::STRUCTURED_RESULT;
        }
    }

    bool rr_SimulateOptions_stiff_get(SimulateOptions* opt) {
        return opt->integratorFlags & SimulateOptions::STIFF;
    }

    void rr_SimulateOptions_stiff_set(SimulateOptions* opt, bool value) {
        if (value) {
            opt->integratorFlags |= SimulateOptions::STIFF;
        } else {
            opt->integratorFlags &= ~SimulateOptions::STIFF;
        }
    }

    bool rr_SimulateOptions_multiStep_get(SimulateOptions* opt) {
        return opt->integratorFlags & SimulateOptions::MULTI_STEP;
    }

    void rr_SimulateOptions_multiStep_set(SimulateOptions* opt, bool value) {
        if (value) {
            opt->integratorFlags |= SimulateOptions::MULTI_STEP;
        } else {
            opt->integratorFlags &= ~SimulateOptions::MULTI_STEP;
        }
    }

    bool rr_SimulateOptions_variableStep_get(SimulateOptions* opt) {
        return opt->integratorFlags & SimulateOptions::VARIABLE_STEP;
    }

    void rr_SimulateOptions_variableStep_set(SimulateOptions* opt, bool value) {
        if (value) {
            opt->integratorFlags |= SimulateOptions::VARIABLE_STEP;
        } else {
            opt->integratorFlags &= ~SimulateOptions::VARIABLE_STEP;
        }
    }

    rr::SimulateOptions::Integrator rr_SimulateOptions_integrator_get(SimulateOptions* opt) {
        return opt->integrator;
    }

    void rr_SimulateOptions_integrator_set(SimulateOptions* opt, rr::SimulateOptions::Integrator value) {

        // set the value
        opt->integrator = value;

        // adjust the value of the VARIABLE_STEP based on wether we are choosing
        // stochastic or deterministic integrator.
        bool vs = false;

        if (rr::SimulateOptions::getIntegratorType(value) == rr::SimulateOptions::STOCHASTIC) {
            vs = rr::Config::getBool(rr::Config::SIMULATEOPTIONS_STOCHASTIC_VARIABLE_STEP);
        }

        else if (rr::SimulateOptions::getIntegratorType(value) == rr::SimulateOptions::DETERMINISTIC) {
            vs = rr::Config::getBool(rr::Config::SIMULATEOPTIONS_DETERMINISTIC_VARIABLE_STEP);
        }

        if (vs) {
            opt->integratorFlags |= rr::SimulateOptions::VARIABLE_STEP;
        } else {
            opt->integratorFlags &= ~rr::SimulateOptions::VARIABLE_STEP;
        }
    }

%}



%extend rr::RoadRunnerOptions
{
    bool disablePythonDynamicProperties;
}

%{


    bool rr_RoadRunnerOptions_disablePythonDynamicProperties_get(RoadRunnerOptions* opt) {
        return opt->flags & rr::RoadRunnerOptions::DISABLE_PYTHON_DYNAMIC_PROPERTIES;
    }

    void rr_RoadRunnerOptions_disablePythonDynamicProperties_set(RoadRunnerOptions* opt, bool value) {
        if (value) {
            opt->flags |= rr::RoadRunnerOptions::DISABLE_PYTHON_DYNAMIC_PROPERTIES;
        } else {
            opt->flags &= ~rr::RoadRunnerOptions::DISABLE_PYTHON_DYNAMIC_PROPERTIES;
        }
    }


%}

%extend rr::LoadSBMLOptions
{
    bool conservedMoieties;
    bool mutableInitialConditions;
    bool noDefaultSelections;
    bool readOnly;
    bool recompile;
}


%{
    bool rr_LoadSBMLOptions_conservedMoieties_get(rr::LoadSBMLOptions* opt) {
        return opt->modelGeneratorOpt & rr::LoadSBMLOptions::CONSERVED_MOIETIES;
    }


    void rr_LoadSBMLOptions_conservedMoieties_set(rr::LoadSBMLOptions* opt, bool value) {
        if (value) {
            opt->modelGeneratorOpt |= rr::LoadSBMLOptions::CONSERVED_MOIETIES;
        } else {
            opt->modelGeneratorOpt &= ~rr::LoadSBMLOptions::CONSERVED_MOIETIES;
        }
    }


    bool rr_LoadSBMLOptions_noDefaultSelections_get(rr::LoadSBMLOptions* opt) {
        return opt->loadFlags & rr::LoadSBMLOptions::NO_DEFAULT_SELECTIONS;
    }

    void rr_LoadSBMLOptions_noDefaultSelections_set(rr::LoadSBMLOptions* opt, bool value) {
        if (value) {
            opt->loadFlags |= rr::LoadSBMLOptions::NO_DEFAULT_SELECTIONS;
        } else {
            opt->loadFlags &= ~rr::LoadSBMLOptions::NO_DEFAULT_SELECTIONS;
        }
    }

    bool rr_LoadSBMLOptions_mutableInitialConditions_get(rr::LoadSBMLOptions* opt) {
        return opt->modelGeneratorOpt & rr::LoadSBMLOptions::MUTABLE_INITIAL_CONDITIONS;
    }


    void rr_LoadSBMLOptions_mutableInitialConditions_set(rr::LoadSBMLOptions* opt, bool value) {
        if (value) {
            opt->modelGeneratorOpt |= rr::LoadSBMLOptions::MUTABLE_INITIAL_CONDITIONS;
        } else {
            opt->modelGeneratorOpt &= ~rr::LoadSBMLOptions::MUTABLE_INITIAL_CONDITIONS;
        }
    }

    bool rr_LoadSBMLOptions_recompile_get(rr::LoadSBMLOptions* opt) {
        return opt->modelGeneratorOpt & rr::LoadSBMLOptions::RECOMPILE;
    }


    void rr_LoadSBMLOptions_recompile_set(rr::LoadSBMLOptions* opt, bool value) {
        if (value) {
            opt->modelGeneratorOpt |= rr::LoadSBMLOptions::RECOMPILE;
        } else {
            opt->modelGeneratorOpt &= ~rr::LoadSBMLOptions::RECOMPILE;
        }
    }

    bool rr_LoadSBMLOptions_readOnly_get(rr::LoadSBMLOptions* opt) {
        return opt->modelGeneratorOpt & rr::LoadSBMLOptions::READ_ONLY;
    }


    void rr_LoadSBMLOptions_readOnly_set(rr::LoadSBMLOptions* opt, bool value) {
        if (value) {
            opt->modelGeneratorOpt |= rr::LoadSBMLOptions::READ_ONLY;
        } else {
            opt->modelGeneratorOpt &= ~rr::LoadSBMLOptions::READ_ONLY;
        }

    }
%}




%extend rr::ExecutableModel
{

    /**
     * creates a function signature of
     * SWIGINTERN PyObject *rr_ExecutableModel_getIds(rr::ExecutableModel *self,int types);
     */
    PyObject *getIds(int types) {
        std::list<std::string> ids;

        ($self)->getIds(types, ids);

        unsigned size = ids.size();

        PyObject* pyList = PyList_New(size);

        unsigned j = 0;

        for (std::list<std::string>::const_iterator i = ids.begin(); i != ids.end(); ++i)
        {
            const std::string& id  = *i;
            PyObject* pyStr = PyString_FromString(id.c_str());
            PyList_SET_ITEM(pyList, j++, pyStr);
        }

        return pyList;
    }

    /***
     ** get values section
     ***/

    PyObject *getFloatingSpeciesAmounts(int len, int const *indx) {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getFloatingSpeciesAmounts,
                                          &rr::ExecutableModel::getNumFloatingSpecies, len, indx);
    }

    PyObject *getFloatingSpeciesAmounts() {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getFloatingSpeciesAmounts,
                                          &rr::ExecutableModel::getNumFloatingSpecies, (int)0, (int const*)0);
    }

    PyObject *getFloatingSpeciesAmountRates(int len, int const *indx) {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getFloatingSpeciesAmountRates,
                                         &rr::ExecutableModel::getNumIndFloatingSpecies,  len, indx);
    }

    PyObject *getFloatingSpeciesAmountRates() {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getFloatingSpeciesAmountRates,
                                          &rr::ExecutableModel::getNumIndFloatingSpecies, (int)0, (int const*)0);
    }

    PyObject *getFloatingSpeciesConcentrations(int len, int const *indx) {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getFloatingSpeciesConcentrations,
                                         &rr::ExecutableModel::getNumFloatingSpecies,  len, indx);
    }

    PyObject *getFloatingSpeciesConcentrations() {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getFloatingSpeciesConcentrations,
                                          &rr::ExecutableModel::getNumFloatingSpecies, (int)0, (int const*)0);
    }

    PyObject *getBoundarySpeciesAmounts(int len, int const *indx) {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getBoundarySpeciesAmounts,
                                         &rr::ExecutableModel::getNumBoundarySpecies,  len, indx);
    }

    PyObject *getBoundarySpeciesAmounts() {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getBoundarySpeciesAmounts,
                                          &rr::ExecutableModel::getNumBoundarySpecies, (int)0, (int const*)0);
    }

    PyObject *getBoundarySpeciesConcentrations(int len, int const *indx) {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getBoundarySpeciesConcentrations,
                                         &rr::ExecutableModel::getNumBoundarySpecies,  len, indx);
    }

    PyObject *getBoundarySpeciesConcentrations() {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getBoundarySpeciesConcentrations,
                                          &rr::ExecutableModel::getNumBoundarySpecies, (int)0, (int const*)0);
    }
    PyObject *getGlobalParameterValues(int len, int const *indx) {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getGlobalParameterValues,
                                         &rr::ExecutableModel::getNumGlobalParameters,  len, indx);
    }

    PyObject *getGlobalParameterValues() {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getGlobalParameterValues,
                                          &rr::ExecutableModel::getNumGlobalParameters, (int)0, (int const*)0);
    }

    PyObject *getCompartmentVolumes(int len, int const *indx) {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getCompartmentVolumes,
                                         &rr::ExecutableModel::getNumCompartments,  len, indx);
    }

    PyObject *getCompartmentVolumes() {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getCompartmentVolumes,
                                          &rr::ExecutableModel::getNumCompartments, (int)0, (int const*)0);
    }

    PyObject *getConservedMoietyValues(int len, int const *indx) {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getConservedMoietyValues,
                                         &rr::ExecutableModel::getNumConservedMoieties,  len, indx);
    }

    PyObject *getConservedMoietyValues() {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getConservedMoietyValues,
                                          &rr::ExecutableModel::getNumConservedMoieties, (int)0, (int const*)0);
    }

    PyObject *getReactionRates(int len, int const *indx) {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getReactionRates,
                                         &rr::ExecutableModel::getNumReactions,  len, indx);
    }

    PyObject *getReactionRates() {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getReactionRates,
                                          &rr::ExecutableModel::getNumReactions, (int)0, (int const*)0);
    }

    PyObject *getFloatingSpeciesInitConcentrations() {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getFloatingSpeciesInitConcentrations,
                                          &rr::ExecutableModel::getNumFloatingSpecies, (int)0, (int const*)0);
    }

    PyObject *getFloatingSpeciesInitAmounts() {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getFloatingSpeciesInitAmounts,
                                          &rr::ExecutableModel::getNumFloatingSpecies, (int)0, (int const*)0);
    }

    PyObject *getCompartmentInitVolumes() {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getCompartmentInitVolumes,
                                          &rr::ExecutableModel::getNumCompartments, (int)0, (int const*)0);
    }

    /***
     ** state vector section
     ***/



    /**
     * overload which returns a copy of the state vector
     */
    PyObject *getStateVector() {
        int len = ($self)->getStateVector(0);

        npy_intp dims[1] = {len};
        PyObject *array = PyArray_SimpleNew(1, dims, NPY_DOUBLE);

        if (!array) {
            // TODO error handling.
            return 0;
        }

        double *data = (double*)PyArray_DATA((PyArrayObject*)array);

        ($self)->getStateVector(data);

        return array;
    }


    void getStateVector(int out_len, double* out_values) {
        int len = ($self)->getStateVector(0);

        if (len > out_len) {
            PyErr_Format(PyExc_ValueError,
                         "given array of size %d, insufficient size for state vector %d.",
                         out_len, len);
            return;
        }

        ($self)->getStateVector(out_values);
    }

    /**
     * get a copy of the state vector rate using the current state.
     */
    PyObject *getStateVectorRate(double time = NAN) {
        int len = ($self)->getStateVector(0);

        npy_intp dims[1] = {len};
        PyObject *array = PyArray_SimpleNew(1, dims, NPY_DOUBLE);

        if (!array) {
            // TODO error handling.
            return 0;
        }

        if (isnan(time)) {
            time = ($self)->getTime();
        }

        double *data = (double*)PyArray_DATA((PyArrayObject*)array);

        ($self)->getStateVectorRate(time, 0, data);

        return array;
    }


    PyObject *getStateVectorRate(double time, PyObject *arg) {

        // length of state vector
        int len = ($self)->getStateVector(0);

        if (isnan(time)) {
            time = ($self)->getTime();
        }

        // check if the pyobj is an npy double array
        PyArrayObject *array  = obj_to_array_no_conversion(arg, NPY_DOUBLE);

        // need contigous and native
        // these set py error if they fail
        if (array && require_contiguous(array) && require_native(array))
        {
            // The number of dimensions in the array.
            int ndim = PyArray_NDIM(array);

            // pointer to the dimensions/shape of the array.
            npy_intp *pdims = PyArray_DIMS(array);

            if (ndim > 0 && pdims[ndim-1] == len) {

                // total number of elements in array
                npy_intp size = PyArray_SIZE(array);

                printf("size: %i\n", (int)size);

                // how many state vectors we have in given array
                int nvec = size / len;

                // pointer to start of data block
                double* stateVec = (double*)PyArray_DATA(array);

                // make result data, copy descriptor
                PyArray_Descr *dtype = PyArray_DESCR(array);
                Py_INCREF(dtype);

                // If strides is NULL, then the array strides are computed as
                // C-style contiguous (default)
                PyObject *result = PyArray_NewFromDescr(&PyArray_Type,
                                                        dtype,
                                                        ndim,
                                                        pdims,
                                                        NULL,
                                                        NULL,
                                                        0,
                                                        NULL);

                // out data
                double* stateVecRate = (double*)PyArray_DATA(result);

                // get the state vector rates, bump the data pointers
                // to the next state vec
                for (int i = 0; i < nvec; ++i) {
                    ($self)->getStateVectorRate(time, stateVec, stateVecRate);
                    stateVec += len;
                    stateVecRate += len;
                }

                return result;
            }

            PyErr_Format(PyExc_TypeError,
                         "Require an N dimensional array where N must be at least 1 and "
                         "the trailing dimension must be the same as the state vector size. "
                         "require trailing dimension of %i but recieved %d", len, (int)pdims[ndim-1]);


        }

        // error case, PyErr is set if we get here.
        return 0;
    }


    PyObject *getStateVectorRate(double time, PyObject *arg1, PyObject *arg2) {

        // length of state vector
        int len = ($self)->getStateVector(0);

        if (isnan(time)) {
            time = ($self)->getTime();
        }

        // check if the pyobj is an npy double array
        PyArrayObject *array1  = obj_to_array_no_conversion(arg1, NPY_DOUBLE);
        PyArrayObject *array2  = obj_to_array_no_conversion(arg2, NPY_DOUBLE);

        // need contigous and native
        // these set py error if they fail
        if (array1 && require_contiguous(array1) && require_native(array1) &&
            array2 && require_contiguous(array2) && require_native(array2))
        {
            // The number of dimensions in the array.
            int ndim1 = PyArray_NDIM(array1);
            int ndim2 = PyArray_NDIM(array2);

            // pointer to the dimensions/shape of the array.
            npy_intp *pdims1 = PyArray_DIMS(array1);
            npy_intp *pdims2 = PyArray_DIMS(array2);

            if (ndim1 > 0 && pdims1[ndim1-1] == len &&
                ndim2 > 0 && pdims2[ndim2-1] == len ) {

                // total number of elements in array
                npy_intp size = PyArray_SIZE(array1);

                if (size == PyArray_SIZE(array2)) {

                    printf("size: %i\n", (int)size);

                    // how many state vectors we have in given array
                    int nvec = size / len;

                    // pointer to start of data block
                    double* stateVec = (double*)PyArray_DATA(array1);

                    // out data
                    double* stateVecRate = (double*)PyArray_DATA(array2);

                    // get the state vector rates, bump the data pointers
                    // to the next state vec
                    for (int i = 0; i < nvec; ++i) {
                        ($self)->getStateVectorRate(time, stateVec, stateVecRate);
                        stateVec += len;
                        stateVecRate += len;
                    }

                    // only error free result case
                    // caller should decref the array so we need incrref it.
                    Py_INCREF(arg2);
                    return arg2;

                } else {
                    PyErr_Format(PyExc_TypeError,
                                 "Both arrays must be the same size and shape, array 1 size: %i, "
                                 "array 2 size: %i", (int)size, (int)PyArray_SIZE(array2));
                }
            } else {
                PyErr_Format(PyExc_TypeError,
                             "Require an N dimensional array where N must be at least 1 and "
                             "the trailing dimension must be the same as the state vector size. "
                             "require trailing dimension of %i but recieved %d", len, (int)pdims1[ndim1-1]);
            }
        }

        // error case, PyErr is set if we get here.
        return 0;
    }




    /***
     ** get ids section
     ***/


    PyObject *getFloatingSpeciesIds() {
        return rr_ExecutableModel_getIds($self, rr::SelectionRecord::FLOATING_AMOUNT);
    }

    PyObject *getBoundarySpeciesIds() {
        return rr_ExecutableModel_getIds($self, rr::SelectionRecord::BOUNDARY_AMOUNT);
    }

    PyObject *getGlobalParameterIds() {
        return rr_ExecutableModel_getIds($self, rr::SelectionRecord::GLOBAL_PARAMETER);
    }

    PyObject *getCompartmentIds() {
        return rr_ExecutableModel_getIds($self, rr::SelectionRecord::COMPARTMENT);
    }

    PyObject *getConservedMoietyIds() {
        return rr_ExecutableModel_getIds($self, rr::SelectionRecord::CONSREVED_MOIETY);
    }

    PyObject *getReactionIds() {
        return rr_ExecutableModel_getIds($self, rr::SelectionRecord::REACTION_RATE);
    }

    PyObject *getFloatingSpeciesInitAmountIds() {
        return rr_ExecutableModel_getIds($self, rr::SelectionRecord::INITIAL_FLOATING_AMOUNT);
    }

    PyObject *getFloatingSpeciesInitConcentrationIds() {
        return rr_ExecutableModel_getIds($self, rr::SelectionRecord::INITIAL_FLOATING_CONCENTRATION);
    }

    PyObject *getFloatingSpeciesAmountRateIds() {
        return rr_ExecutableModel_getIds($self, rr::SelectionRecord::FLOATING_AMOUNT_RATE);
    }

    PyObject *getStateVectorIds() {
        return rr_ExecutableModel_getIds($self, rr::SelectionRecord::STATE_VECTOR);
    }

    PyObject *getEventIds() {
        return rr_ExecutableModel_getIds($self, rr::SelectionRecord::EVENT);
    }



    /***
     ** set values section
     ***/

    int setFloatingSpeciesAmounts(int len, double const *values) {
        return $self->setFloatingSpeciesAmounts(len, 0, values);
    }

    int setFloatingSpeciesConcentrations(int len, double const *values) {
        return $self->setFloatingSpeciesConcentrations(len, 0, values);
    }

    int setBoundarySpeciesConcentrations(int len, double const *values) {
        return $self->setBoundarySpeciesConcentrations(len, 0, values);
    }

    int setGlobalParameterValues(int len, double const *values) {
        return $self->setGlobalParameterValues(len, 0, values);
    }

    int setCompartmentVolumes(int len, double const *values) {
        return $self->setCompartmentVolumes(len, 0, values);
    }

    int setConservedMoietyValues(int len, double const *values) {
        return $self->setConservedMoietyValues(len, 0, values);
    }

    int setFloatingSpeciesInitConcentrations(int len, double const *values) {
        return $self->setFloatingSpeciesInitConcentrations(len, 0, values);
    }

    int setFloatingSpeciesInitAmounts(int len, double const *values) {
        return $self->setFloatingSpeciesInitAmounts(len, 0, values);
    }

    int setCompartmentInitVolumes(int len, double const *values) {
        return $self->setCompartmentInitVolumes(len, 0, values);
    }


    int setFloatingSpeciesAmounts(int leni, int const* indx, int lenv, double const *values) {
        if (leni != lenv) {
            PyErr_Format(PyExc_ValueError,
                         "Arrays of lengths (%d,%d) given",
                         leni, lenv);
            return -1;
        }
        return $self->setFloatingSpeciesAmounts(leni, indx, values);
    }


    int setFloatingSpeciesConcentrations(int leni, int const* indx, int lenv, double const *values) {
        if (leni != lenv) {
            PyErr_Format(PyExc_ValueError,
                         "Arrays of lengths (%d,%d) given",
                         leni, lenv);
            return -1;
        }
        return $self->setFloatingSpeciesConcentrations(leni, indx, values);
    }

    int setBoundarySpeciesConcentrations(int leni, int const* indx, int lenv, double const *values) {
        if (leni != lenv) {
            PyErr_Format(PyExc_ValueError,
                         "Arrays of lengths (%d,%d) given",
                         leni, lenv);
            return -1;
        }
        return $self->setBoundarySpeciesConcentrations(leni, indx, values);
    }

    int setGlobalParameterValues(int leni, int const* indx, int lenv, double const *values) {
        if (leni != lenv) {
            PyErr_Format(PyExc_ValueError,
                         "Arrays of lengths (%d,%d) given",
                         leni, lenv);
            return -1;
        }
        return $self->setGlobalParameterValues(leni, indx, values);
    }

    int setCompartmentVolumes(int leni, int const* indx, int lenv, double const *values) {
        if (leni != lenv) {
            PyErr_Format(PyExc_ValueError,
                         "Arrays of lengths (%d,%d) given",
                         leni, lenv);
            return -1;
        }
        return $self->setCompartmentVolumes(leni, indx, values);
    }

    int setConservedMoietyValues(int leni, int const* indx, int lenv, double const *values) {
        if (leni != lenv) {
            PyErr_Format(PyExc_ValueError,
                         "Arrays of lengths (%d,%d) given",
                         leni, lenv);
            return -1;
        }
        return $self->setConservedMoietyValues(leni, indx, values);
    }

    int setFloatingSpeciesInitConcentrations(int leni, int const* indx, int lenv, double const *values) {
        if (leni != lenv) {
            PyErr_Format(PyExc_ValueError,
                         "Arrays of lengths (%d,%d) given",
                         leni, lenv);
            return -1;
        }
        return $self->setFloatingSpeciesInitConcentrations(leni, indx, values);
    }


    int setFloatingSpeciesInitAmounts(int leni, int const* indx, int lenv, double const *values) {
        if (leni != lenv) {
            PyErr_Format(PyExc_ValueError,
                         "Arrays of lengths (%d,%d) given",
                         leni, lenv);
            return -1;
        }
        return $self->setFloatingSpeciesInitAmounts(leni, indx, values);
    }


    int setCompartmentInitVolumes(int leni, int const* indx, int lenv, double const *values) {
        if (leni != lenv) {
            PyErr_Format(PyExc_ValueError,
                         "Arrays of lengths (%d,%d) given",
                         leni, lenv);
            return -1;
        }
        return $self->setCompartmentInitVolumes(leni, indx, values);
    }


    int setFloatingSpeciesInitConcentrations(int leni, int const* indx, int lenv, double const *values) {
        if (leni != lenv) {
            PyErr_Format(PyExc_ValueError,
                         "Arrays of lengths (%d,%d) given",
                         leni, lenv);
            return -1;
        }
        return $self->setFloatingSpeciesInitConcentrations(leni, indx, values);
    }

    int setFloatingSpeciesInitAmounts(int leni, int const* indx, int lenv, double const *values) {
        if (leni != lenv) {
            PyErr_Format(PyExc_ValueError,
                         "Arrays of lengths (%d,%d) given",
                         leni, lenv);
            return -1;
        }
        return $self->setFloatingSpeciesInitAmounts(leni, indx, values);
    }


    int setCompartmentInitVolumes(int leni, int const* indx, int lenv, double const *values) {
        if (leni != lenv) {
            PyErr_Format(PyExc_ValueError,
                         "Arrays of lengths (%d,%d) given",
                         leni, lenv);
            return -1;
        }
        return $self->setCompartmentInitVolumes(leni, indx, values);
    }


    PyObject* getStoichiometryMatrix() {
        int rows = 0;
        int cols = 0;
        double* data = 0;

        $self->getStoichiometryMatrix(&rows, &cols, &data);

        int nd = 2;
        npy_intp dims[2] = {rows, cols};

        PyObject *pArray = PyArray_New(&PyArray_Type, nd, dims, NPY_DOUBLE, NULL, data, 0,
                NPY_CARRAY | NPY_OWNDATA, NULL);

        return pArray;
    }


    /**
     * get values.
     */

    double __getitem__(const std::string& id) {
        return ($self)->getValue(id);
    }

    void __setitem__(const std::string& id, double value) {
        ($self)->setValue(id, value);
    }


    std::string __repr__() {
        std::stringstream s;
        s << "<roadrunner.ExecutableModel() { this = " << (void*)$self << " }>";
        return s.str();
    }

    /**
     * events section
     *
     * TODO, the returned event is not valid after the model object is freed.
     * is this OK???
     */
    rr::PyEventListener *getEvent(int index) {
        ExecutableModel *p = $self;
        EventListenerPtr e = p->getEventListener(index);

        if(e) {
            PyEventListener *impl = dynamic_cast<PyEventListener*>(e.get());
            return impl;
        } else {
            PyEventListener *impl = new PyEventListener();
            p->setEventListener(index, EventListenerPtr(impl));
            return impl;
        }
    }

    rr::PyEventListener *getEvent(const std::string& eventId) {
        int index = ($self)->getEventIndex(eventId);

        if (index >= 0) {
            ExecutableModel *p = $self;
            EventListenerPtr e = p->getEventListener(index);

            if(e) {
                PyEventListener *impl = dynamic_cast<PyEventListener*>(e.get());
                return impl;
            } else {
                PyEventListener *impl = new PyEventListener();
                p->setEventListener(index, EventListenerPtr(impl));
                return impl;
            }

        } else {
            throw std::out_of_range(std::string("could not find index for event ") + eventId);
        }
    }

    %pythoncode %{
        def _makeProperties(self) :

            def mk_fget(sel): return lambda self: self.__getitem__(sel)
            def mk_fset(sel): return lambda self, val: self.__setitem__(sel, val)

            for s in self.getFloatingSpeciesIds():
                sel = "[" + s + "]"
                fget = mk_fget(sel)
                fset = mk_fset(sel)
                self.__class__.__swig_getmethods__[s] = fget
                self.__class__.__swig_setmethods__[s] = fset
                setattr(self.__class__, s, property(fget, fset))

                fget = mk_fget(s)
                fset = mk_fset(s)
                name = s + "_amt"
                self.__class__.__swig_getmethods__[name] = fget
                self.__class__.__swig_setmethods__[name] = fset
                setattr(self.__class__, name, property(fget, fset))

            ids = self.getGlobalParameterIds() + self.getCompartmentIds() + \
                self.getReactionIds()

            for s in ids:
                fget = mk_fget(s)
                fset = mk_fset(s)
                self.__class__.__swig_getmethods__[s] = fget
                self.__class__.__swig_setmethods__[s] = fset
                setattr(self.__class__, s, property(fget, fset))

        def keys(self, types=_roadrunner.SelectionRecord_ALL):
            return self.getIds(types)

        def values(self, types=_roadrunner.SelectionRecord_ALL):
            return [self.getValue(k) for k in self.keys(types)]

        def items(self, types=_roadrunner.SelectionRecord_ALL):
            return [(k, self.getValue(k)) for k in self.keys(types)]

        def __len__(self):
            return len(self.keys())

        def iteritems(self, types=_roadrunner.SelectionRecord_ALL):
            """
            return an iterator over (key, value) pairs
            """
            return self.items(types).__iter__()

        def iterkeys(self, types=_roadrunner.SelectionRecord_ALL):
            """
            return an iterator over the mapping's keys
            """
            return self.keys(types).__iter__()

        def itervalues(self, types=_roadrunner.SelectionRecord_ALL):
            """
            return an iterator over the mapping's values
            """
            return self.values(types).__iter__()
    %}
}

%extend rr::Integrator {

    void _setListener(const rr::PyIntegratorListenerPtr &listener) {

        Log(rr::Logger::LOG_INFORMATION) << __FUNC__ << ", use count: " << listener.use_count();

        cxx11_ns::shared_ptr<rr::IntegratorListener> i =
            cxx11_ns::dynamic_pointer_cast<rr::IntegratorListener>(listener);

        Log(rr::Logger::LOG_INFORMATION) << __FUNC__ << ", after cast use count: " << listener.use_count();

        ($self)->setListener(i);
    }

    rr::PyIntegratorListenerPtr _getListener() {

        Log(rr::Logger::LOG_INFORMATION) << __FUNC__;

        rr::IntegratorListenerPtr l = ($self)->getListener();

        rr::PyIntegratorListenerPtr ptr =
            cxx11_ns::dynamic_pointer_cast<rr::PyIntegratorListener>(l);

        Log(rr::Logger::LOG_INFORMATION) << __FUNC__ << ", use count: " << ptr.use_count();

        return ptr;
    }

    void _clearListener() {
        rr::IntegratorListenerPtr current = ($self)->getListener();

        Log(rr::Logger::LOG_INFORMATION) << __FUNC__ << ", current use count before clear: " << current.use_count();

        ($self)->setListener(rr::IntegratorListenerPtr());

        Log(rr::Logger::LOG_INFORMATION) << __FUNC__ << ", current use count after clear: " << current.use_count();
    }

    // we want to get the listener back as a PyIntegratorListener, however
    // swig won't let us ignore by return value and if we ignore getListener,
    // it ignores any extended version. So, we have to make an extended
    // _getListener() above, and call it from python like this.
    %pythoncode %{
        def getListener(self):
            return self._getListener()

        def setListener(self, listener):
            if listener is None:
                self._clearListener()
            else:
                self._setListener(listener)

        __swig_getmethods__["listener"] = getListener
        __swig_setmethods__["listener"] = setListener
        if _newclass: listener = property(getListener, setListener)
    %}
}

%extend rr::PyIntegratorListener {
    %pythoncode %{
        __swig_getmethods__["onTimeStep"] = getOnTimeStep
        __swig_setmethods__["onTimeStep"] = setOnTimeStep
        if _newclass: onTimeStep = property(getOnTimeStep, setOnTimeStep)

        __swig_getmethods__["onEvent"] = getOnEvent
        __swig_setmethods__["onEvent"] = setOnEvent
        if _newclass: onEvent = property(getOnEvent, setOnEvent)
     %}
}

%extend rr::PyEventListener {
    %pythoncode %{
        __swig_getmethods__["onTrigger"] = getOnTrigger
        __swig_setmethods__["onTrigger"] = setOnTrigger
        if _newclass: onTrigger = property(getOnTrigger, setOnTrigger)

        __swig_getmethods__["onAssignment"] = getOnAssignment
        __swig_setmethods__["onAssignment"] = setOnAssignment
        if _newclass: onAssignment = property(getOnAssignment, setOnAssignment)
     %}
}




