
// Module Name
%module(directors="1", docstring="The RoadRunner SBML Simulation Engine, (c) 2009-2014 Andy Somogyi and Herbert Sauro","threads"=1) roadrunner

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
    // see discussion on import array,
    // https://numpy.org/doc/stable/reference/c-api/array.html#importing-the-api
    #define PY_ARRAY_UNIQUE_SYMBOL RoadRunner_ARRAY_API
    //Can't require new wrappers on MacOS 10.9
    //#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
    #include <numpy/arrayobject.h>
    #ifdef _MSC_VER
    #pragma warning(disable: 26812)
    #pragma warning(disable: 26451)
    #endif
    #include <lsMatrix.h>
    #ifdef _MSC_VER
    #pragma warning(disable: 26812)
    #pragma warning(disable: 26451)
    #endif
    #include <lsLibla.h>
    #include <lsLA.h>
    #include <lsLUResult.h>
    #include <lsUtils.h>
    #include <rrCompiler.h>
    #include <ExecutableModelFactory.h>
    #include <rrExecutableModel.h>
    #include <rrRoadRunnerOptions.h>
    #include <rrRoadRunner.h>
    #include <SteadyStateSolver.h>
    #include <rrLogger.h>
    #include <rrConfig.h>
    #include <conservation/ConservationExtension.h>
    #include "conservation/ConservedMoietyConverter.h"
    #include "SBMLValidator.h"
    #include "rrSBMLReader.h"
    #include <memory>
    #include <cstddef>
    #include <map>
    #include <rrVersionInfo.h>
    #include <rrException.h>
    #include <assert.h>
    #include <math.h>
    #include <cmath>
    #include "PyUtils.h"
    #include "PyLoggerStream.h"

    #include "Registrable.h"
    #include "RegistrationFactory.h"

    // Steady State Solvers
    #include "KinsolSteadyStateSolver.h"
    #include "NewtonIteration.h"
    #include "BasicNewtonIteration.h"
    #include "LinesearchNewtonIteration.h"
    #include "NLEQ1Solver.h"
    #include "NLEQ2Solver.h"

    // sundials Sensitivity solvers
    #include "SensitivitySolver.h"
    #include "ForwardSensitivitySolver.h"
    #include "Matrix.h"
    #include "Matrix3D.h"

    // Integrators
    #include "CVODEIntegrator.h"
    #include "GillespieIntegrator.h"
    #include "RK4Integrator.h"
    #include "RK45Integrator.h"
    #include "EulerIntegrator.h"


    // make a python obj out of the C++ ExecutableModel, this is used by the PyEventListener
    // class. This function is defined later in this compilation unit.
    PyObject *ExecutableModel_NewPythonObj(rr::ExecutableModel*);
    PyObject *Integrator_NewPythonObj(rr::Integrator*);

    #include "PyEventListener.h"
    #include "PyIntegratorListener.h"
    #include "tr1proxy/cxx11_ns.h"

    /**
     * Note, avoid "using" declarations, which can confuse
     * swig. Best to just be explicit about namespaces.
     */


// Windows is just so special...
#ifdef _WIN32
    #define isnan _isnan
#else
    #include <signal.h>
    #define isnan std::isnan
#endif

    using namespace rr;

#define VERIFY_PYARRAY(p) { \
    assert(p && "PyArray is NULL"); \
    assert((PyArray_NBYTES(p) > 0 ? PyArray_ISCARRAY(p) : true) &&  "PyArray must be C format"); \
}

    class DictionaryHolder {
    public:
        rr::Dictionary* dict;

        DictionaryHolder() { dict = NULL; }

        ~DictionaryHolder() {
            rrLog(Logger::LOG_TRACE) << __FUNC__ << ", deleting dictionary " << (void*)dict;
            delete dict;
        }
    };
%}


%naturalvar;

// C++ std::string handling
%include "std_string.i"

// C++ std::map handling
%include "std_unordered_map.i"

%include "std_vector.i"

%include "std_list.i"

//enables better handling of STL exceptions
%include "exception.i"

// correct mapping of unsigned integers
%include "rr_stdint.i"

// all the documentation goes here.
%include "rr_docstrings.txt"


// the cmake _CMakeLists.txt file in this directory sets the value of the
// SWIG_SHARED_PTR_SUBNAMESPACE as a pre-processor symbol based on the
// USE_TR1_CXX_NS CMake option. SWIG has no way of getting this info
// from the compiler so have to reley on the CMake system.
%include "std_shared_ptr.i"

%shared_ptr(rr::PyIntegratorListener)

%include "rrExporter.h"


%template(IntVector) std::vector<int>;
%template() std::vector<std::string>;
%template() std::list<std::string>;
//%template(DoubleMap) std::unordered_map< std::string,double,std::hash< std::string >,std::equal_to< std::string >,std::allocator< std::pair< std::string const,double > > > >;



//%template(SelectionRecordVector) std::vector<rr::SelectionRecord>;
//%apply std::vector<rr::SelectionRecord> {std::vector<SelectionRecord>, std::vector<rr::SelectionRecord>, std::vector<SelectionRecord>};

%apply std::list<std::string>& OUTPUT {std::list<std::string>};

%template(DictionaryVector) std::vector<const rr::Dictionary*>;
%apply std::vector<const rr::Dictionary*> {std::vector<const Dictionary*>, std::vector<const rr::Dictionary*>, std::vector<const Dictionary*>};

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
    // %typemap(out) ls::DoubleMatrix
    const ls::DoubleMatrix* mat = &($1);
    $result = doublematrix_to_py(mat, false, true);
}


/**
 * Convert from C --> Python
 * reference roadrunner owned data.
 */
%typemap(out) const ls::DoubleMatrix* {
    // %typemap(out) const ls::DoubleMatrix*
    const ls::DoubleMatrix* mat = ($1);
    $result = doublematrix_to_py(mat, false, false);
}

%apply const ls::DoubleMatrix* {ls::DoubleMatrix*, DoubleMatrix*, const DoubleMatrix* };

/**
 * Converts a rr::Matrix<double> to a numpy array,
 * using the same functions/methods as for ls::Matrix<double> (its superclass)
 * (proxy via rrDoubleMatrix_to_py)
 */
%typemap(out) rr::Matrix<double> {
    // marker for rrDoubleMatrix typemap. Look for me in TestModelFactoryPYTHON_wrap.cxx
    const rr::Matrix<double>* mat = &($1);
    $result = rrDoubleMatrix_to_py(mat, true);
}

%apply rr::Matrix<double> {
    const rr::Matrix<double>,
    const rr::Matrix<double>&,
    rr::Matrix<double>&
};


/**
 * Note - you do not need to %include "Matrix3D.h"
 * since we convert it to a Tuple[np.ndarray, np.ndarray]
 */
//%include "Matrix3D.h"
%typemap(out) rr::Matrix3D<double, double> {
    // marker for a rr::Matrix3D<double, double> typemap
    Matrix3DToNumpy matrix3DtoNumpy($1);
    PyObject* npArray3D = matrix3DtoNumpy.convertData();
    PyObject* idx = matrix3DtoNumpy.convertIndex();
    PyObject* colnames = matrix3DtoNumpy.convertColNames();
    PyObject* rownames = matrix3DtoNumpy.convertRowNames();

    $result = PyTuple_Pack(4, idx, npArray3D, rownames, colnames);
}




/* Convert from C --> Python */
%typemap(out) std::vector<double> {

    size_t len = $1.size();
    npy_intp dims[1] = {static_cast<npy_intp>(len)};

    PyObject *array = PyArray_SimpleNew(1, dims, NPY_DOUBLE);
//    VERIFY_PYARRAY(array);

    if (!array) {
        // TODO error handling.
        return 0;
    }

    double *data = (double*)PyArray_DATA((PyArrayObject*)array);

    std::vector<double>& vec = $1;

    memcpy(data, &vec[0], sizeof(double)*len);

    $result  = array;
}

%typedef ls::Complex std::complex<double>;

/* Convert from C --> Python */
%typemap(out) std::vector<std::complex<double>> {

    typedef std::complex<double> cpx;

    std::vector<cpx>& vec = $1;

    bool iscpx = false;

    size_t len = $1.size();
    npy_intp dims[1] = {static_cast<npy_intp>(len)};

    PyObject *array = PyArray_SimpleNew(1, dims, NPY_COMPLEX128);
    VERIFY_PYARRAY(array);

    if (!array) {
        // TODO error handling.
        return 0;
    }

    cpx *data = (cpx*)PyArray_DATA((PyArrayObject*)array);

    memcpy(data, &vec[0], sizeof(std::complex<double>)*len);

    $result  = array;
}



/**
 * converts a C++ rr::Setting to a Python variable, depending on its type
 * The "work" of this typemap is offloaded to the rr::Variant_to_py function.
 */
%typemap(out) rr::Setting{
    try {
        // I'm a marker rr::Setting(out). Look for me in the swig_wrap.cxx file
        // to verify that this type map is being properly applied
        $result = rr::Variant_to_py($1);
    } catch (const std::exception& e){
        SWIG_exception(SWIG_RuntimeError, e.what());
    }
}

/**
 * Apply the %typemap(out) rr::Setting to
 * other Setting types. Note that both
 * rr:: qualified and unqualified are
 * necessary!
 */
%apply rr::Setting{
    // both syntax's of const are needed!
    const rr::Setting&,
    rr::Setting const &,
    Setting const &,
    rr::Setting&,
    Setting&
};

// converts a Python Variable to a C++ rr::Setting
%typemap(in) const rr::Setting&(PyObject* settingObjFromPython){
    try {
        // I'm a marker rr::Setting(in). Look for me in the swig_wrap.cxx file
        // to verify that this type map is being properly applied
        $1 = rr::Variant_from_py($input);

    } catch (const std::exception& e){
        SWIG_exception(SWIG_RuntimeError, e.what());
    }
}
%apply const rr::Setting&{
    // both syntax's of const are needed!
    const rr::Setting&,
    rr::Setting const &,
    const Setting&,
    Setting const &,
    rr::Setting&,
    Setting&,
    rr::Setting,
    Setting
};


/**
 * @brief typemap to convert a string : Variant map into a Python dict.
 * Used in the "settings" map of TestModelFactory (tmf).
 *
 * @note std::unordered_map<std::string, rr:Variant> gets expanded by swig to
 * the full version below. The full version is needed for swig to recognize the type
 * and use this typemap
 */
%typemap(out) std::unordered_map< std::string, rr::Setting> {
    // I'm a marker for %typemap(out) std::unordered_map< std::string, rr::Setting>
    // Look me up in the swig generated wrapper cxx file to
    // make sure this typemap is being properly applied
    $result = PyDict_New();
    if (!$result){
        std::cerr << "Could not create Python Dict" << std::endl;
    }

    for (const auto& item: *$1){
        int err = PyDict_SetItem($result, PyUnicode_FromString(item.first.c_str()), Variant_to_py(item.second));
        if (err < 0){
            std::cout << "Could not create item in Python Dict" << std::endl;
        }
    }
}


/**
 * @brief Apply the %typemap(out) std::unordered_map< std::string, rr::Setting>
 * to the following types.
 * @note swig is sensitive to rr:: qualified and unqualified types, even though they are the same
 */
%apply std::unordered_map< std::string, rr::Setting>{
    std::unordered_map< std::string,rr::Setting,std::hash< std::string >,std::equal_to< std::string >,std::allocator< std::pair< std::string const,rr::Setting > > >,   // with rr::,       no reference
    std::unordered_map< std::string,Setting,std::hash< std::string >,std::equal_to< std::string >,std::allocator< std::pair< std::string const,Setting > > >,           // no rr::          no reference
     std::unordered_map< std::string,rr::Setting,std::hash< std::string >,std::equal_to< std::string >,std::allocator< std::pair< std::string const,rr::Setting > > > &, // with rr::        reference
     std::unordered_map< std::string,Setting,std::hash< std::string >,std::equal_to< std::string >,std::allocator< std::pair< std::string const,Setting > > > &, // without rr::     reference

    const std::unordered_map< std::string,rr::Setting,std::hash< std::string >,std::equal_to< std::string >,std::allocator< std::pair< std::string const,rr::Setting > > >,   // with rr::,       no reference
    const std::unordered_map< std::string,Setting,std::hash< std::string >,std::equal_to< std::string >,std::allocator< std::pair< std::string const,Setting > > >          // no rr::          no reference
    // note: it seems that this typemap cannot be applied to constant references,

    // const std::unordered_map< std::string,rr::Setting,std::hash< std::string >,std::equal_to< std::string >,std::allocator< std::pair< std::string const,rr::Setting > > > &, // with rr::        reference
    // const std::unordered_map< std::string,Setting,std::hash< std::string >,std::equal_to< std::string >,std::allocator< std::pair< std::string const,Setting > > > & // without rr::     reference
};

// C++ std::unordered_map<std::string, double> to Python Dictionary
//%template(StringDoubleMap) std::unordered_map<std::string, double>;

%typemap(out) std::unordered_map<std::string, double> {
    // Marker for StringDoubleMap
    $result = PyDict_New();
    if (!$result){
        std::cerr << "Could not create Python Dict" << std::endl;
    }

    for (const auto& item: *(&$1)){
        int err = PyDict_SetItem($result, PyUnicode_FromString(item.first.c_str()), PyFloat_FromDouble(item.second));
        if (err < 0){
            std::cout << "Could not create item in Python Dict" << std::endl;
        }
    }
}


/**
 * input map, convert an incomming object to a roadrunner Dictionary*
 */
%typemap(in) const rr::Dictionary* (DictionaryHolder holder, void* argp) {
    // I'm a marker for %typemap(in) const rr::Dictionary* (DictionaryHolder holder, void* argp)
    // Look me up in the swig generated wrapper cxx file to
    // make sure this typemap is being properly applied

    try {
        // check if null, this is fine,
        if($input == NULL) {
            $1 = NULL;
        }
        else {
            // first check if its a roadrunner type
            int res = SWIG_ConvertPtr($input, &argp,SWIGTYPE_p_rr__Dictionary, 0 |  0 );
            if (SWIG_IsOK(res)) {
                $1 = reinterpret_cast< rr::Dictionary * >(argp);
            } else {
                holder.dict = Dictionary_from_py($input);
                $1 = holder.dict;
            }
        }
    } catch (const std::exception& e) {
        SWIG_exception(SWIG_RuntimeError, e.what());
    }
}

%typemap(typecheck) const rr::Dictionary* = PyObject*;
%apply const rr::Dictionary* {const Dictionary*, rr::Dictionary*, Dictionary*};

%include "numpy.i"


%init %{
import_array();
rr::pyutil_init(m);
%}

%{


#ifndef _WIN32

    static void rr_sighandler(int sig) {
        std::cout << "handling signal " << sig << std::endl;
        rrLog(rr::Logger::LOG_WARNING) << "signal handler : " << sig;
    }

    static unsigned long sigtrap() {
        signal(SIGTRAP, rr_sighandler);
        return raise(SIGTRAP);
    }

#else

    static unsigned long sigtrap() {
        rrLog(rr::Logger::LOG_WARNING) << "sigtrap not supported on Windows";
        return 0;
    }

#endif
%}


size_t sigtrap();



%{

typedef int (rr::ExecutableModel::*getValuesPtr)(size_t, int const*, double*);
typedef string (ExecutableModel::*getNamePtr)(int);
typedef int (ExecutableModel::*getNumPtr)();


static PyObject* _ExecutableModel_getValues(rr::ExecutableModel *self, getValuesPtr func,
                                            getNumPtr numPtr, size_t len, int const *indx) {
    if (len <= 0) {
        len = (self->*numPtr)();
        indx = 0;
    }

    npy_intp dims[1] = {static_cast<npy_intp>(len)};
    PyObject *array = PyArray_SimpleNew(1, dims, NPY_DOUBLE);
//    VERIFY_PYARRAY(array);

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



// make a python obj out of the C++ ExecutableModel, this is used by the PyEventListener
// class. This function is defined later in this compilation unit.

PyObject *ExecutableModel_NewPythonObj(rr::ExecutableModel* e) {
    return SWIG_NewPointerObj(SWIG_as_voidptr(e), SWIGTYPE_p_rr__ExecutableModel, 0 |  0 );
}

PyObject *Integrator_NewPythonObj(rr::Integrator* i) {
    return SWIG_NewPointerObj(SWIG_as_voidptr(i), SWIGTYPE_p_rr__Integrator, 0 |  0 );
}



%}


%apply (int DIM1, int* IN_ARRAY1) {(size_t len, int const *indx)};

%apply (int DIM1, double* IN_ARRAY1) {(size_t len, double const *values)};

// typemap for the set***Values methods
%apply (int DIM1, int* IN_ARRAY1) {(size_t leni, int const* indx)};
%apply (int DIM1, double* IN_ARRAY1) {(size_t lenv, const  double* values)};

%apply int { size_t }

#define LIB_EXTERN
#define RR_DECLSPEC
#define PUGIXML_CLASS



%warnfilter(509) rr::RoadRunner::setSelections;
%warnfilter(509) rr::RoadRunner::setSteadyStateSelections;

// Many of the RoadRunner methods will be ignored for the time being
// as currently we do not have a clean mapping to Python.


%ignore rr::RoadRunner::RoadRunner(const std::string&, const std::string&, const std::string&);

%ignore rr::RoadRunner::addCapabilities;
%ignore rr::RoadRunner::getFloatingSpeciesIds;
%ignore rr::RoadRunner::getFloatingSpeciesConcentrationIds;
%ignore rr::RoadRunner::getRateOfChangeIds;
//%ignore rr::RoadRunner::getuCC;
%ignore rr::RoadRunner::addCapability;
%ignore rr::RoadRunner::getFloatingSpeciesInitialConcentrationByIndex;
//%ignore rr::RoadRunner::getRatesOfChange;
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
//%ignore rr::RoadRunner::getInstanceID;
//%ignore rr::RoadRunner::getScaledFluxControlCoefficientMatrix;
%ignore rr::RoadRunner::setCompartmentByIndex;
%ignore rr::RoadRunner::getBoundarySpeciesIds;
%ignore rr::RoadRunner::getBoundarySpeciesConcentrationIds;
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
//%ignore rr::RoadRunner::getConservedMoietyIds;
//%ignore rr::RoadRunner::getNrMatrix;
//%ignore rr::RoadRunner::getTimeCourseSelectionList;
%ignore rr::RoadRunner::setSimulationSettings;
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

%ignore rr::RoadRunner::getNumberOfFloatingSpecies;
//%ignore rr::RoadRunner::getUnscaledElasticityMatrix;
%ignore rr::RoadRunner::setTimeStart;

%ignore rr::RoadRunner::getNumberOfGlobalParameters;
//%ignore rr::RoadRunner::getUnscaledFluxControlCoefficientMatrix;
//%ignore rr::RoadRunner::setValue;
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
%ignore rr::RoadRunner::getRateOfChange;
//%ignore rr::RoadRunner::getlibSBMLVersion;
//%ignore rr::RoadRunner::writeSBML;
%ignore rr::RoadRunner::getSimulateOptions;
%ignore rr::RoadRunner::setSimulateOptions;
%ignore rr::RoadRunner::getIds(int types, std::list<std::string> &);

%ignore rr::RoadRunner::getOptions;

%ignore rr::RoadRunner::simulate;

%rename (_load) rr::RoadRunner::load;
%rename (_getValue) rr::RoadRunner::getValue;

%ignore rr::Config::getInt;
%ignore rr::Config::getString;
%ignore rr::Config::getBool;
%ignore rr::Config::getDouble;

%ignore *::setItem;
%ignore *::getItem;
%ignore *::hasKey;
%ignore *::deleteItem;
%ignore *::getKeys;

// ignore SimulateOptions key access methods,
// these are replaced by python dictionary protocol.
//%rename (__setitem__) rr::Dictionary::setItem;
//%rename (__getitem__) rr::Dictionary::getItem;
//%rename (__contains__) rr::Dictionary::hasKey;
//%rename (__delitem__) rr::Dictionary::deleteItem;
//%rename (keys) rr::Dictionary::getKeys;

// do not create these, pure interface.
%nodefaultctor rr::Dictionary;
%nodefaultdtor Dictionary;
%nodefaultctor rr::BasicDictionary;
%nodefaultdtor DictionaryImpl;


// ignore SimulateOptions key access methods,
// these are replaced by python dictionary protocol.
//%ignore rr::Integrator::setValue;
//%ignore rr::Integrator::getValue;
%ignore rr::Integrator::hasKey;
%ignore rr::Integrator::deleteValue;
%ignore rr::Integrator::getKeys;
%ignore rr::Integrator::setSimulateOptions;
%rename (__str__) rr::Integrator::toString;
//%rename (__repr__) rr::Integrator::toRepr;

%ignore rr::integratorFactoryMutex;
%ignore rr::integratorRegistrationMutex;
%ignore rr::steadyStateSolverFactoryMutex;
%ignore rr::steadyStateSolverRegistrationMutex;


%rename (__str__) rr::SteadyStateSolver::toString;

// rename these, the injected python code will take care of
// making these properties.
%ignore rr::RoadRunner::getSelections();
%ignore rr::RoadRunner::setSelections(const std::vector<rr::SelectionRecord>&);
%rename (_setSelections) setSelections(const std::vector<std::string>&);
%rename (_getModel) getModel();

// hide SelectionRecord details from python api,
// only deal with strings here.
%ignore rr::RoadRunner::getSteadyStateSelections();
%ignore rr::RoadRunner::setSteadyStateSelections(const std::vector<rr::SelectionRecord>&);
//%ignore rr::RoadRunner::getSteadyStateValues();
//%rename (getSteadyStateValues) rr::RoadRunner::getSteadyStateValuesNamedArray();
//%rename (steadyState) rr::RoadRunner::steadyState;
//%rename (steadyState) rr::RoadRunner::steadyStateNamedArray;

%rename (_setSteadyStateSelections) setSteadyStateSelections(const std::vector<std::string>&);
%rename (_getConservedMoietyAnalysis) rr::RoadRunner::getConservedMoietyAnalysis();
%rename (_setConservedMoietyAnalysis) rr::RoadRunner::setConservedMoietyAnalysis(bool);

//Model editing proxies so that we can regenerate properties each time we regenerate the model
%rename (_addParameter) rr::RoadRunner::addParameter(const std::string&, double, bool);
%rename (_addSpeciesConcentration) rr::RoadRunner::addSpeciesConcentration(const std::string&, const std::string&, double, bool, bool, const std::string&, bool);
%rename (_addSpeciesAmount) rr::RoadRunner::addSpeciesAmount(const std::string&, const std::string&, double, bool, bool, const std::string&, bool);
%rename (_addCompartment) rr::RoadRunner::addCompartment(const std::string&, double, bool);
%rename (_addReaction) rr::RoadRunner::addReaction(const std::string&, std::vector<std::string>, std::vector<std::string>, const std::string&, bool);


// Swig wraps C++ vectors to tuples, need to wrap lists instead on some methods
%rename (_getIndependentFloatingSpeciesIds) rr::RoadRunner::getIndependentFloatingSpeciesIds();
%rename (_getDependentFloatingSpeciesIds) rr::RoadRunner::getDependentFloatingSpeciesIds();

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


%ignore rr::ExecutableModel::getFloatingSpeciesAmounts(size_t, int const*, double *);
%ignore rr::ExecutableModel::setFloatingSpeciesAmounts(size_t len, int const *indx, const double *values);
%ignore rr::ExecutableModel::getFloatingSpeciesAmountRates(size_t, int const*, double *);
%ignore rr::ExecutableModel::getFloatingSpeciesConcentrationRates(size_t, int const*, double *);

%ignore rr::ExecutableModel::getFloatingSpeciesConcentrations(size_t, int const*, double *);
%ignore rr::ExecutableModel::setFloatingSpeciesConcentrations(size_t len, int const *indx, const double *values);
%ignore rr::ExecutableModel::setFloatingSpeciesInitConcentrations(size_t len, int const *indx, const double *values);
%ignore rr::ExecutableModel::getFloatingSpeciesInitConcentrations(size_t, int const*, double *);
%ignore rr::ExecutableModel::getBoundarySpeciesAmounts(size_t, int const*, double *);
%ignore rr::ExecutableModel::getBoundarySpeciesConcentrations(size_t, int const*, double *);
%ignore rr::ExecutableModel::setBoundarySpeciesConcentrations(size_t len, int const *indx, const double *values);
%ignore rr::ExecutableModel::getGlobalParameterValues(size_t, int const*, double *);
%ignore rr::ExecutableModel::setGlobalParameterValues(size_t len, int const *indx, const double *values);
%ignore rr::ExecutableModel::getCompartmentVolumes(size_t, int const*, double *);
%ignore rr::ExecutableModel::setCompartmentVolumes(size_t len, int const *indx, const double *values);
%ignore rr::ExecutableModel::getConservedMoietyValues(size_t, int const*, double *);
%ignore rr::ExecutableModel::setConservedMoietyValues(size_t len, int const *indx, const double *values);
%ignore rr::ExecutableModel::getReactionRates(size_t, int const*, double *);
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
%ignore rr::ExecutableModel::getStateVectorId(int index);
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
%ignore rr::ExecutableModel::saveState;

%ignore rr::ExecutableModel::getFloatingSpeciesId(int index);
%ignore rr::ExecutableModel::getBoundarySpeciesId(int index);
%ignore rr::ExecutableModel::getGlobalParameterId(int index);
%ignore rr::ExecutableModel::getCompartmentId(int index);
%ignore rr::ExecutableModel::getConservedMoietyId(int index);
%ignore rr::ExecutableModel::getReactionId(size_t index);

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

%ignore rr::ExecutableModel::setFloatingSpeciesInitConcentrations(size_t len, int const *indx, double const *values);
%ignore rr::ExecutableModel::getFloatingSpeciesInitConcentrations(size_t len, int const *indx, double *values);
%ignore rr::ExecutableModel::setFloatingSpeciesInitAmounts(size_t len, int const *indx, double const *values);
%ignore rr::ExecutableModel::getFloatingSpeciesInitAmounts(size_t len, int const *indx, double *values);
%ignore rr::ExecutableModel::setCompartmentInitVolumes(size_t len, int const *indx, double const *values);
%ignore rr::ExecutableModel::getCompartmentInitVolumes(size_t len, int const *indx, double *values);
%ignore rr::ExecutableModel::getIds(int, std::list<std::string> &);

// deprecated, model knows how to reset itself with reset.
%ignore rr::ExecutableModel::evalInitialConditions;

// map the events to python using the PyEventListener class
%ignore rr::ExecutableModel::setEventListener(int, rr::EventListenerPtr);
%ignore rr::ExecutableModel::getEventListener(int);
%ignore rr::EventListenerPtr;
%ignore rr::EventListenerException;

// ignore the EventListener virtuals, but leave the enum
%ignore rr::EventListener::onTrigger(ExecutableModel* model, size_t eventIndex, const std::string& eventId);
%ignore rr::EventListener::onAssignment(ExecutableModel* model, size_t eventIndex, const std::string& eventId);

// ignore the C++ class, only deal with the python version
%ignore rr::IntegratorListener;

%ignore rr::Integrator::setListener(rr::IntegratorListenerPtr);
%ignore rr::Integrator::getListener();

//%ignore rr::Integrator::addIntegratorListener;
//%ignore rr::Integrator::removeIntegratorListener;

// ignore integrator registration

%ignore rr::IntegratorRegistrar;
%ignore rr::IntegratorFactory;

%rename rr::conversion::ConservedMoietyConverter PyConservedMoietyConverter;

%ignore rr::ostream;
%ignore ostream;
%ignore std::ostream;
%ignore operator<<(ostream&, const rr::SelectionRecord& rec);
%ignore operator<<(rr::ostream&, const rr::SelectionRecord& rec);



//%ignore rr::DictionaryImpl;

// Warning 389: operator[] ignored (consider using %extend)
// Warning 401: Nothing known about base class 'Configurable'. Ignored.


// Warning 315: Nothing known about 'std::ostream'.
namespace std { class ostream{}; }


// Warning 401: Nothing known about base class 'Configurable'. Ignored.


/**
 * include the roadrunner files here, this is where the wrappers are generated.
 */

//
/**
 * this returns a new object
 */
%newobject rr::ExecutableModelFactory::createModel;



%include <Dictionary.h>
%include <rrRoadRunnerOptions.h>
%include <rrCompiler.h>
%include <rrExecutableModel.h>
%include <ExecutableModelFactory.h>
%include <rrVersionInfo.h>


// including rrLogger.h causes rr not to compile?
// haven't worked out why but seems roadrunner python
// works without it.
%include <rrLogger.h>

%thread;
%include <rrRoadRunner.h>
%nothread;

%include <rrSelectionRecord.h>
%include <conservation/ConservedMoietyConverter.h>

%include "RegistrationFactory.h"
%include "Registrable.h"
/**
 * Solve base class tells swig to properly handle
 * cross language polymorphism.
 */
%include <Solver.h>
%feature("director") Solver;
%include <Integrator.h>
%include <SteadyStateSolver.h>
%include <SensitivitySolver.h>

%include "PyEventListener.h"
%include "PyIntegratorListener.h"
%include <rrConfig.h>
%include <SBMLValidator.h>
%include <rrSBMLReader.h>

%extend rr::RoadRunner
{
    // attributes

    /**
     * make some of these const so SWIG would not allow setting.
     */
    const rr::SimulateOptions *__simulateOptions;

    rr::RoadRunnerOptions *options;

    std::string __repr__() {
        std::stringstream s;
        s << "<roadrunner.RoadRunner() { this = " << (void*)$self << " }>";
        return s.str();
    }

    std::string __str__() {
        return $self->getInfo();
    }

    PyObject* _simulate(const rr::SimulateOptions* opt) {
        // its not const correct...
        ls::DoubleMatrix *result = const_cast<ls::DoubleMatrix*>($self->simulate(opt));

        return doublematrix_to_py(result, opt->structured_result, opt->copy_result);
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

        size_t size = ids.size();

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

    /**
     * returns the SelectionRecord vector python list of strings.
     */
    PyObject *_getSelections() {

        const std::vector<rr::SelectionRecord>& selections = ($self)->getSelections();

        size_t size = selections.size();

        PyObject *pysel = PyList_New(size);

        unsigned j = 0;
        for (std::vector<rr::SelectionRecord>::const_iterator i = selections.begin();
             i != selections.end(); ++i) {
            std::string str = i->to_string();

            PyObject *pystr = PyString_FromString(str.c_str());
            PyList_SET_ITEM(pysel, j++, pystr);
        }

        return pysel;
    }

    /**
     * returns the SelectionRecord vector python list of strings.
     */
    PyObject *_getSteadyStateSelections() {

        const std::vector<rr::SelectionRecord>& selections = ($self)->getSteadyStateSelections();

        size_t size = selections.size();

        PyObject *pysel = PyList_New(size);

        unsigned j = 0;
        for (std::vector<rr::SelectionRecord>::const_iterator i = selections.begin();
             i != selections.end(); ++i) {
            std::string str = i->to_string();

            PyObject *pystr = PyString_FromString(str.c_str());
            PyList_SET_ITEM(pysel, j++, pystr);
        }

        return pysel;
    }



   %pythoncode %{
        def __getattr__(self, name):
            if name != "this" and name in self.getIds(_roadrunner.SelectionRecord_ALL):
                return self[name]
            else:
                raise AttributeError(name)

        def getValue(self, *args):
            import re
            reg = re.compile(r'eigen\s*\(\s*(\w*)\s*\)\s*$')
            regarr = re.split(reg, args[0])

            if len(regarr) > 1:
               eig_r = _roadrunner.RoadRunner__getValue(self, 'eigenReal(' + str(regarr[1]) + ')')
               eig_i = _roadrunner.RoadRunner__getValue(self, 'eigenImag(' + str(regarr[1]) + ')')
               return complex(eig_r, eig_i)
            else:
                return _roadrunner.RoadRunner__getValue(self, *args)

        def setValues(self, keys, values):
            for key, val in zip(keys, values):
                _roadrunner.RoadRunner_setValue(self, key, val)

        def getModel(self):
            return self._getModel()

        def _setConservedMoietyAnalysisProxy(self, value):
            self._setConservedMoietyAnalysis(value)
            self._makeProperties()

        selections = property(_getSelections, _setSelections)
        timeCourseSelections = property(_getSelections, _setSelections)
        steadyStateSelections = property(_getSteadyStateSelections, _setSteadyStateSelections)
        conservedMoietyAnalysis = property(_getConservedMoietyAnalysis, _setConservedMoietyAnalysis)
        model = property(_getModel)
        integrator = property(getIntegrator, setIntegrator)

        # static list of properties added to the RoadRunner class object
        _properties = []

        def _makeProperties(self):
            """creates dynamic properties for model components, like floating species
            concentrations or amounts etc.
            """
            #global _properties

            # always clear the old properties
            for s in self._properties:
                if hasattr(RoadRunner, s):
                    delattr(RoadRunner, s)

            # properties now empty
            RoadRunner._properties = []

            # check if we should make new properties
            if Config.getValue(Config.ROADRUNNER_DISABLE_PYTHON_DYNAMIC_PROPERTIES):
                return

            # can't make properties without a model.
            if self.getModel() is None:
                return

            def makeProperty(name, sel):
                prop = property(
                        lambda self: self.getModel().__getitem__(sel),
                        lambda self, val: self.getModel().__setitem__(sel, val)
                )
                setattr(RoadRunner, name, prop)
                RoadRunner._properties.append(name)

            model = self.getModel()
            for s in model.getFloatingSpeciesIds():
                makeProperty(s, "[" + s + "]")  # concentrations for backwards compatibility
                makeProperty(s + "_conc", "[" + s + "]")  # concentrations
                makeProperty(s + "_amt", s)     # amounts


            for s in model.getBoundarySpeciesIds():
                makeProperty(s, "[" + s + "]")  # concentrations for backwards compatibility
                makeProperty(s + "_conc", "[" + s + "]")  # concentrations
                makeProperty(s + "_amt", s)     # amounts


            for s in model.getGlobalParameterIds() + model.getCompartmentIds() + model.getReactionIds() + model.getConservedMoietyIds():
                makeProperty(s, s)



        # Set up the python dyanic properties for model access,
        # save the original init method
        _swig_init = __init__

        def _new_init(self, *args):
            # /if called with https, use Python for transport
            if len(args) >= 1:
                p = args[0]
                if hasattr(p,'startswith') and p.startswith('https://'):
                    try:
                        # Python3
                        from urllib.request import urlopen
                    except ImportError:
                        # Python2
                        from urllib2 import urlopen
                    sbml = urlopen(p).read()
                    try:
                        sbml = str(sbml.decode())
                    except:
                        pass
                    RoadRunner._swig_init(self, sbml)
                    RoadRunner._makeProperties(self)
                    return
            # Otherwise, use regular init
            RoadRunner._swig_init(self, *args)
            RoadRunner._makeProperties(self)

        # set the ctor to use the new init
        __init__ = _new_init

        def load(self, *args):
            self._load(*args)
            RoadRunner._makeProperties(self)

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

        def simulate(self, start=None, end=None, points=None, selections=None, output_file=None, steps=None):
            '''
            Simulate and optionally plot current SBML model. This is the one stop shopping method
            for simulation and plotting.

            simulate accepts up to five positional arguments.

            The first five (optional) arguments are treated as:

                1: Start Time, if this is a number.

                2: End Time, if this is a number.

                3: Number of points, if this is a number.

                4: List of Selections. A list of variables to include in the output, e.g. ``['time','A']`` for a model with species ``A``. More below.

                5: output file path. The file to which simulation results will be written. If this is specified and
                nonempty, simulation output will be written to output_file every Config::K_ROWS_PER_WRITE generated.
                Note that simulate() will not return the result matrix if it is writing to output_file.
                It will also not keep any simulation data, so in that case one should not call ``r.plot()``
                without arguments. This should be specified when one cannot, or does not want to, keep the
                entire result matrix in memory.


            All five of the positional arguments are optional. If any of the positional arguments are
            a list of string instead of a number, then they are interpreted as a list of selections.

            There are a number of ways to call simulate.

            1: With no arguments. In this case, the current set of options from the previous
              ``simulate`` call will be used. If this is the first time ``simulate`` is called,
              then a default set of values is used. The default set of values are (start = 0, end = 5, points = 51).

            2: With up to five positions arguments, described above.

            Finally, you can pass steps keyword argument instead of points.

            steps (Optional) Number of steps at which the output is sampled where the samples are evenly spaced. Steps = points-1. Steps and points may not both be specified.

            '''

            # fix issue #401 - this will check if a list was positioned at 3rd position. This allows users to
            # omit positional arguement points. This is un-Pythonic, but implemented for the sake of novice users.
            if type(points) == list:
                selections = points
                points = None

            # check for errors
            import collections
            import sys
            import warnings
            if selections is not None:
                # check that selections is a sequence
                if not isinstance(selections, collections.Sequence):
                    raise ValueError('Expected a sequence type for selections')

                # check that selections contains only strings
                for x in selections:
                    if sys.version_info >= (3,0):
                        if not isinstance(x, str):
                            raise ValueError('Expected selections to be a sequence of strings')
                    else:
                        if not isinstance(x, basestring):
                            raise ValueError('Expected selections to be a sequence of strings')

            if points is not None and steps is not None:
                raise ValueError('Cannot specify both points and steps in simulate call')

            if points is not None:
                if points < 2:
                    raise ValueError('Number of points cannot be less than 2')

            if steps is not None:
                if steps < 1:
                    raise ValueError('Number of steps cannot be less than 1')

            has_output_file = False
            if output_file is not None:
                if not isinstance(output_file, str):
                    raise ValueError('Output file path must be a string')

                has_output_file = bool(output_file)  # if not empty string


            # end error checking

            o = self.__simulateOptions
            originalSteps = o.steps
            originalVSS = True;

            if self.getIntegrator().hasValue('variable_step_size'):
                originalVSS = self.getIntegrator().getValue('variable_step_size')
                if end is not None and (points is not None or steps is not None):
                   self.getIntegrator().setValue('variable_step_size', False)
                else:
                   if originalVSS == True:
                        o.steps = 0

            if start is not None:
                o.start = start

            if end is not None:
                o.end = end

            if points is not None:
                o.steps = points - 1

            if selections is not None:
                self.timeCourseSelections = selections

            if has_output_file:
                o.output_file = output_file
            else:
                o.output_file = ""

            if steps is not None:
                o.steps = steps

            result = self._simulate(o)

            #Check to make sure we made it to the 'end'.
            selections_lower = [x.lower() for x in self.timeCourseSelections]
            if end is not None and "time" in selections_lower:
                lastresult_time = result[len(result)-1][selections_lower.index("time")]
                if end - lastresult_time > end/10000:
                    warnings.warn("Simulation requested end time point (" + str(end) + ") not reached, because the maximum number of steps reached.  Possible solutions include:\n  * Setting an explicit number of points (i.e. r.simulate(" + str(start) + ", " + str(end) + ", 1001)\n  * Setting r.integrator.variable_step_size to 'False'\n  * Setting r.integrator.max_output_rows to a larger number ")

            #Reset any integrator variables we might have changed
            o.steps = originalSteps
            if self.getIntegrator().hasValue('variable_step_size'):
                self.getIntegrator().setValue('variable_step_size', originalVSS)

            if has_output_file:
                # result should be empty here.
                return 'Your results have been written to "{}".'.format(output_file)\
                    + 'To obtain the results directly, pass None or "" to the output_file keyword argument'

            return result

        def __simulateOld(self, *args, **kwargs):
            """
            DEPRECATED!!!!!!!!!!!!!!!!
            WILL BE REMOVED

            Simulate the current SBML model.

            simulate accepts a up to four positional arguments. The first four (optional) arguments are treated as:

            1: start (the simulation starting time)

            2: end (the simulation end time)

            3: steps (the number of output points)

            4: List of Selections.

            All four of the positional arguments are optional. If any of the positional arguments are
            supplied as a list of strings, then they are interpreted as a list of selections.


            There is only one correct way to call simulate. If one positional argument is specified,
            it is the start time. If two are specified, they are the start and end time.
            The third argument is the number of output points!
            The fourth argument, if it is supplied, must be a list of strings that correspond to
            proper timecourse selections as in timeCourseSelections.
            The fifth argument, if supplied via keyword, is the number of intervals, not the
            number of points. Specifying intervals and points is an error.

            Do NOT pass a `SimulateOptions` object to this function. SimulateOptions is DEPRECATED.

            Keyword arguments:

            integrator
                DEPRECATED: use setIntegrator method

            sel or selections
                A list of strings specifying what values to display in the output.

            plot
                DEPRECATED: use plot method.

            end
                The simulation end time. Note, setting the end time automatically sets
                the duration accordingly and visa versa.

            resetModel
                DEPRECATED

            reset
                DEPRECATED: use reset method.

            start
                The start time of the simulation time-series data. Often this is 0,
                but not necessarily.

            steps
                The number of steps at which the output is sampled. The samples are evenly spaced.
                When a simulation system calculates the data points to record, it will typically
                divide the duration by the number of time steps. Thus, for N steps, the output
                will have N+1 data rows.

            stiff
                DEPRECATED: use solver wrappers (this setting only available for some solvers).

            seed
                DEPRECATED: use solver wrappers (this setting only available for some solvers).


            :returns: a numpy array with each selected output time series being a
             column vector, and the 0'th column is the simulation time (if time is selected as an
             output).
            :rtype: numpy.ndarray
            """

            doPlot = False
            showPlot = True

            # cleanup tasks
            post_tasks = []

            # user specified number of steps via 3rd arg or steps=xxx
            haveSteps = False

            # variableStep = True was specified in args
            haveVariableStep = False
            o = self.__simulateOptions

            def steps_restore(v):
                def f():
                    o.steps = v
                return f
            post_tasks.append(steps_restore(o.steps))

            o.steps = 50
            if self.getIntegrator().hasValue('variable_step_size'):
                if self.getIntegrator().getValue('variable_step_size') == True:
                    o.steps = 0
            stepsSpecified = False

            # did the options originally have a seed, if so, don't delete it when we're done
            hadSeed = "seed" in o

            # check if we have just a sim options
            if len(args) >= 1:
                if type(args[0]) == type(self.__simulateOptions):
                    o = args[0]
                elif type(args[0]) == list:
                    # its a selection list
                    self.timeCourseSelections = args[0]
                elif isinstance(args[0], (int, float)):
                    # treat it as a number
                    o.start = args[0]
                else:
                    raise ValueError("argument 1 must be either a number, list or "
                                     "SimulateOptions object, recieved: {0}".format(str(args[0])))

            # second arg is treated as sim end time
            if len(args) >= 2:
                if type(args[1]) == list:
                    # its a selection list
                    self.timeCourseSelections = args[1]
                elif isinstance(args[1], (int, float)):
                    # treat it as a number
                    o.end = args[1]
                else:
                    raise ValueError("argument 2 must be either a number, list or "
                                     "SimulateOptions object, recieved: {0}".format(str(args[1])))


            # third arg is treated as number of points
            if len(args) >= 3:
                if type(args[2]) == list:
                    # its a selection list
                    self.timeCourseSelections = args[2]
                elif isinstance(args[2], (int, float)):
                    # treat it as a number
                    o.steps = args[2]-1
                    stepsSpecified = True
                    if o.steps < 1:
                      raise RuntimeError('Number of points must be 2 or more')
                    haveSteps = True
                else:
                    raise ValueError("argument 3 must be either a number, list or "
                                     "SimulateOptions object, recieved: {0}".format(str(args[2])))

            # forth arg may be a list (currently)
            if len(args) >= 4:
                if type(args[3]) == list:
                    # its a selection list
                    self.timeCourseSelections = args[3]
                else:
                    raise ValueError("argument 4 (if given) must be a list of timeCourseSelections "
                                     ", recieved: {0}".format(str(args[3])))


            # go through the list of keyword args
            for k,v in kwargs.items():

                # changing integrators.
                if k == "integrator":
                    if type(v) == str:
                        # this automatically sets the variable / fixed time step
                        # according to integrator type, raises exception if invalid
                        # integrator string.
                        self.setIntegrator(v)
                    else:
                        raise Exception("{0} is invalid argument for integrator, integrator name must be a string.".format(v))
                    continue

                # specifying timeCourseSelections:
                if k == "timeCourseSelections" or k == "sel":
                    self.timeCourseSelections = v
                    continue

                if k == "steps":
                    o.steps = v
                    stepsSpecified = True
                    continue

                if k == "start":
                    o.start = v
                    continue

                if k == "end":
                    o.end = v
                    continue

                # reset model, also accept 'reset'
                if k == "reset" or k == "resetModel":
                    o.resetModel = v
                    continue

                # check if variableStep was explicitly specified, this overrides the steps
                # positional arg
                if k == "variableStep":
                    raise KeyError('Do NOT pass variableStep to simulate. Use integrator wrappers: r.getIntegrator().setValue("variable_step_size", True)')
                    haveVariableStep = True
                    self.getIntegrator().setValue('variable_step_size', v)
                    if not stepsSpecified:
                        o.steps = 0
                    continue

                if k == "plot":
                    raise RuntimeError('plot argument is deprecated, use plot method')
                    continue

                if k == "show":
                    showPlot = v
                    continue

                if k == "stiff" and self.getIntegrator().hasValue('stiff'):
                    raise KeyError('Do NOT pass stiff to simulate. Use the integrator wrappers: r.getIntegrator().setValue("stiff", True)')
                    def stiff_restore(v):
                        def f():
                            self.getIntegrator().setValue('stiff', v)
                        return f
                    self.getIntegrator().setValue('stiff', kwargs[k])
                    post_tasks.append(stiff_restore(self.getIntegrator().getValue('stiff')))
                    continue

                # if its not one of these, just set the item on the dict, and
                # if the inegrator cares about it, it will use it.
                # if its one of these, set it.
                raise KeyError('No such argument: {}'.format(k))



            # if we are doing a stochastic sim,
            # explicit options of variableStep trumps everything,
            # if not explicit, variableStep is if number of steps was specified,
            # if no steps, varStep = true, false otherwise.
            if self.getIntegrator().getIntegrationMethod() == \
                Integrator.Stochastic and not haveVariableStep:
                self.getIntegrator().setValue('variable_step_size', not haveSteps)

            # the options are set up, now actually run the simuation...
            result = self._simulate(o)

            if not hadSeed:
                del o["seed"]

            if doPlot:
                self.plot(result=None, loc='upper left', show=showPlot)

            # revert any settings we changed
            for x in post_tasks:
                x()

            return result

        def resetToOrigin(self):
            """ Reset model to state when first loaded.

            This resets the model back to the state when it was FIRST loaded,
            this includes all init() and parameters such as k1 etc.

            identical to:
                r.reset(SelectionRecord.ALL)
            """
            self.reset(SelectionRecord.ALL)

        def resetAll(self):
            """ Reset all model variables to CURRENT init(X) values.

            This resets all variables and parameters in the model (S1, S2, k1, etc.) to the CURRENT init(X) values.
            """
            self.reset(SelectionRecord.TIME |
                       SelectionRecord.RATE |
                       SelectionRecord.FLOATING |
                       SelectionRecord.BOUNDARY |
                       SelectionRecord.COMPARTMENT |
                       SelectionRecord.GLOBAL_PARAMETER)

        def resetParameter(self):
            """ Reset parameters to CURRENT init(X) values.

            This resets all parameters to the CURRENT init(X) values.
            """
            self.reset(SelectionRecord.GLOBAL_PARAMETER)

        def getAvailableIntegrators(self):
            """
            get a list of available integrator names.
            """
            return self.getExistingIntegratorNames()


        def plot(self, result=None, loc='upper left', show=True):
            """
            RoadRunner.plot([show])

            Plot the previously run simulation result using Matplotlib.

            This takes the contents of the simulation result and builds a
            legend from the selection list.


            If the optional prameter 'show' [default is True] is given, the pylab
            show() method is called.
            """

            try:
                import matplotlib.pyplot as p
            except ImportError:
                raise ImportError('Could not import matplotlib - please install matplotlib to enable plotting functionality')

            result = self.getSimulationData()

            if result is None:
                raise Exception("no simulation result")

            rval = []

            # check if standard numpy array
            if result.dtype.names is None:

                timeCourseSelections = self.timeCourseSelections

                if len(result.shape) != 2 or result.shape[1] != len(timeCourseSelections):
                    raise Exception("simulation result columns not equal to number of selections, likely a simulation has not been run")

                times = result[:,0]

                for i in range(1, len(timeCourseSelections)):
                    series = result[:,i]
                    name = timeCourseSelections[i]
                    rval.append(p.plot(times, series, label=str(name), linewidth=2))

            # result is structured array
            else:
                if len(result.dtype.names) < 1:
                    raise Exception('no columns to plot')

                time = result.dtype.names[0]

                for name in result.dtype.names[1:]:
                    rval.append(p.plot(result[time], result[name], label=name, linewidth=2))

            if show:
                p.show()

            return rval

        def plotLegend(self):
            try:
                import matplotlib.pyplot as p
            except ImportError:
                raise ImportError('Could not import matplotlib - please install matplotlib to enable plotting functionality')
            p.legend()

        def showPlot(self):
            try:
                import matplotlib.pyplot as p
            except ImportError:
                raise ImportError('Could not import matplotlib - please install matplotlib to enable plotting functionality')
            p.show()

        def getIndependentFloatingSpeciesIds(self):
            return list(self._getIndependentFloatingSpeciesIds())

        def getDependentFloatingSpeciesIds(self):
            return list(self._getDependentFloatingSpeciesIds())

        def getReactionRates(self):
            return self.getModel().getReactionRates()

        integrator = property(getIntegrator, setIntegrator)
        #if _newclass:
            #integrator = property(getIntegrator, setIntegrator)

        def setIntegratorSetting(self, integratorName, settingName, value):
            import sys
            if sys.version_info >= (3,0):
                if not isinstance(integratorName, str):
                    raise ValueError('Expected integratorName to be a string')
                if not isinstance(settingName, str):
                    raise ValueError('Expected settingName to be a string')
            else:
                if not isinstance(integratorName, basestring):
                    raise ValueError('Expected integratorName to be a string')
                if not isinstance(settingName, basestring):
                    raise ValueError('Expected settingName to be a string')

            # store original integrator and switch back to it afterwards
            origIntegrator = self.getIntegrator().getName()

            self.setIntegrator(integratorName)
            self.getIntegrator().setValue(settingName, value)
            self.setIntegrator(origIntegrator)

        @property
        def steadyStateSolver(self):
            '''The current steady state solver'''
            return self.getSteadyStateSolver()

        @steadyStateSolver.setter
        def steadyStateSolver(self, v):
            self.setSteadyStateSolver(v)

        def addParameter(self, *args):
            self._addParameter(*args)
            self._makeProperties()

        def addReaction(self, *args):
            self._addReaction(*args)
            self._makeProperties()

        def addSpecies(self, sid, compartment, initAmount = 0.0, initConcentration = 0.0, hasOnlySubstanceUnits=False, boundaryCondition=False, substanceUnits = "", forceRegenerate = True):
            if initConcentration==0.0:
                self._addSpeciesAmount(sid, compartment, initAmount, hasOnlySubstanceUnits, boundaryCondition, substanceUnits, forceRegenerate)
            elif initAmount==0.0:
                self._addSpeciesConcentration(sid, compartment, initConcentration, hasOnlySubstanceUnits, boundaryCondition, substanceUnits, forceRegenerate)
            else:
                raise AttributeError("When calling 'addSpecies' you may only define initAmount or initConcentration, not both.")
                
            self._makeProperties()

        def addSpeciesAmount(self, sid, compartment, initAmount = 0.0, hasOnlySubstanceUnits=False, boundaryCondition=False, substanceUnits = "", forceRegenerate = True):
            self._addSpeciesAmount(sid, compartment, initAmount, hasOnlySubstanceUnits, boundaryCondition, substanceUnits, forceRegenerate)
            self._makeProperties()

        def addSpeciesConcentration(self, sid, compartment, initConcentration = 0.0, hasOnlySubstanceUnits=False, boundaryCondition=False, substanceUnits = "", forceRegenerate = True):
            self._addSpeciesConcentration(sid, compartment, initConcentration, hasOnlySubstanceUnits, boundaryCondition, substanceUnits, forceRegenerate)
            self._makeProperties()

        def addCompartment(self, *args):
            self._addCompartment(*args)
            self._makeProperties()

        def _diffstep_getter(self):
            '''Differential step size used in MCA'''
            return self.getDiffStepSize()

        def _diffstep_stter(self, v):
            self.setDiffStepSize(v)

        diffstep = property(_diffstep_getter, _diffstep_stter)

        def _steadyStateThresh_getter(self):
            '''Steady state threshold used in MCA'''
            return self.getSteadyStateThreshold()

        def _steadyStateThresh_setter(self, v):
            self.setSteadyStateThreshold(v)

        steadyStateThresh = property(_steadyStateThresh_getter, _steadyStateThresh_setter)
    %}
}

%{
    rr::SimulateOptions* rr_RoadRunner___simulateOptions_get(RoadRunner* r) {
        rrLog(Logger::LOG_WARNING) << "DO NOT USE simulateOptions, it is DEPRECATED";
        return &r->getSimulateOptions();
    }

    void rr_RoadRunner___simulateOptions_set(RoadRunner* r, const rr::SimulateOptions* opt) {
        rrLog(Logger::LOG_WARNING) << "DO NOT USE simulateOptions, it is DEPRECATED";
        r->setSimulateOptions(*opt);
    }


    rr::RoadRunnerOptions* rr_RoadRunner_options_get(RoadRunner* r) {
        rrLog(Logger::LOG_WARNING) << "DO NOT USE options, it is DEPRECATED";
        return &r->getOptions();
    }

    void rr_RoadRunner_options_set(RoadRunner* r, const rr::RoadRunnerOptions* opt) {
        rrLog(Logger::LOG_WARNING) << "DO NOT USE options, it is DEPRECATED";
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
    bool structuredResult;
    bool copyResult;

    std::string __repr__() {
        return ($self)->toRepr();
    }


    std::string __str__() {
        return ($self)->toString();
    }

    /**
     * makes a copy of this object.
     * Python normally just keeps references to objects, and this forces a true
     * copy. Note, we heed to add the SWIG_POINTER_OWN to the function below
     * so that when the returned object is destroyed (by Python), the C++
     * object will also be deleted.
     */
    PyObject *copy() {
        SimulateOptions *pThis = $self;
        SimulateOptions *other = new SimulateOptions(*pThis);
        return SWIG_NewPointerObj(SWIG_as_voidptr(other), SWIGTYPE_p_rr__SimulateOptions, SWIG_POINTER_OWN );
    }

    %pythoncode %{
        def getListener(self):
            return self._getListener()

        def setListener(self, listener):
            if listener is None:
                self._clearListener()
            else:
                self._setListener(listener)
    %}

}

%{
    double rr_SimulateOptions_end_get(SimulateOptions* opt) {
        return opt->start + opt->duration;
    }

    void rr_SimulateOptions_end_set(SimulateOptions* opt, double end) {
        opt->duration = end - opt->start;
    }

    bool rr_SimulateOptions_structuredResult_get(SimulateOptions* opt) {
        return opt->structured_result;
    }

    void rr_SimulateOptions_structuredResult_set(SimulateOptions* opt, bool value) {
        opt->structured_result = value;
    }

    bool rr_SimulateOptions_copyResult_get(SimulateOptions* opt) {
        return opt->getItem("copyResult");
    }

    void rr_SimulateOptions_copyResult_set(SimulateOptions* opt, bool value) {
        opt->setItem("copyResult", value);
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

        size_t size = ids.size();

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

    /**
     * creates a function signature of
     * SWIGINTERN PyObject *rr_ExecutableModel_getEventIds(rr::ExecutableModel *self);
     */
    PyObject *getEventIds() {
        std::list<std::string> ids;

        ($self)->getEventIds(ids);

        size_t size = ids.size();

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

    PyObject *getFloatingSpeciesAmounts(size_t len, int const *indx) {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getFloatingSpeciesAmounts,
                                          &rr::ExecutableModel::getNumFloatingSpecies, len, indx);
    }

    PyObject *getFloatingSpeciesAmounts() {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getFloatingSpeciesAmounts,
                                          &rr::ExecutableModel::getNumFloatingSpecies, (size_t)0, (int const*)0);
    }

    PyObject *getFloatingSpeciesConcentrationRates(size_t len, int const *indx) {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getFloatingSpeciesConcentrationRates,
                                         &rr::ExecutableModel::getNumIndFloatingSpecies,  len, indx);
    }

    PyObject *getFloatingSpeciesConcentrationRates() {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getFloatingSpeciesConcentrationRates,
                                          &rr::ExecutableModel::getNumIndFloatingSpecies, (size_t)0, (int const*)0);
    }

    PyObject *getFloatingSpeciesConcentrations(size_t len, int const *indx) {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getFloatingSpeciesConcentrations,
                                         &rr::ExecutableModel::getNumFloatingSpecies,  len, indx);
    }

    PyObject *getFloatingSpeciesConcentrations() {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getFloatingSpeciesConcentrations,
                                          &rr::ExecutableModel::getNumFloatingSpecies, (size_t)0, (int const*)0);
    }

    PyObject *getBoundarySpeciesAmounts(size_t len, int const *indx) {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getBoundarySpeciesAmounts,
                                         &rr::ExecutableModel::getNumBoundarySpecies,  len, indx);
    }

    PyObject *getBoundarySpeciesAmounts() {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getBoundarySpeciesAmounts,
                                          &rr::ExecutableModel::getNumBoundarySpecies, (size_t)0, (int const*)0);
    }

    PyObject *getBoundarySpeciesConcentrations(size_t len, int const *indx) {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getBoundarySpeciesConcentrations,
                                         &rr::ExecutableModel::getNumBoundarySpecies,  len, indx);
    }

    PyObject *getBoundarySpeciesConcentrations() {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getBoundarySpeciesConcentrations,
                                          &rr::ExecutableModel::getNumBoundarySpecies, (size_t)0, (int const*)0);
    }
    PyObject *getGlobalParameterValues(size_t len, int const *indx) {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getGlobalParameterValues,
                                         &rr::ExecutableModel::getNumGlobalParameters,  len, indx);
    }

    PyObject *getGlobalParameterValues() {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getGlobalParameterValues,
                                          &rr::ExecutableModel::getNumGlobalParameters, (size_t)0, (int const*)0);
    }

    PyObject *getCompartmentVolumes(size_t len, int const *indx) {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getCompartmentVolumes,
                                         &rr::ExecutableModel::getNumCompartments,  len, indx);
    }

    PyObject *getCompartmentVolumes() {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getCompartmentVolumes,
                                          &rr::ExecutableModel::getNumCompartments, (size_t)0, (int const*)0);
    }

    PyObject *getConservedMoietyValues(size_t len, int const *indx) {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getConservedMoietyValues,
                                         &rr::ExecutableModel::getNumConservedMoieties,  len, indx);
    }

    PyObject *getConservedMoietyValues() {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getConservedMoietyValues,
                                          &rr::ExecutableModel::getNumConservedMoieties, (size_t)0, (int const*)0);
    }

    PyObject *getReactionRates(size_t len, int const *indx) {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getReactionRates,
                                         &rr::ExecutableModel::getNumReactions,  len, indx);
    }

    PyObject *getReactionRates() {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getReactionRates,
                                          &rr::ExecutableModel::getNumReactions, (size_t)0, (int const*)0);
    }

    PyObject *getFloatingSpeciesInitConcentrations() {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getFloatingSpeciesInitConcentrations,
                                          &rr::ExecutableModel::getNumFloatingSpecies, (size_t)0, (int const*)0);
    }

    PyObject *getFloatingSpeciesInitAmounts() {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getFloatingSpeciesInitAmounts,
                                          &rr::ExecutableModel::getNumFloatingSpecies, (size_t)0, (int const*)0);
    }

    PyObject *getCompartmentInitVolumes() {
        return _ExecutableModel_getValues($self, &rr::ExecutableModel::getCompartmentInitVolumes,
                                          &rr::ExecutableModel::getNumCompartments, (size_t)0, (int const*)0);
    }


    /***
     ** get ids section
     ***/


    PyObject *getFloatingSpeciesIds() {
        return rr_ExecutableModel_getIds($self, rr::SelectionRecord::FLOATING_AMOUNT);
    }

    PyObject *getFloatingSpeciesConcentrationIds() {
        return rr_ExecutableModel_getIds($self, rr::SelectionRecord::FLOATING_CONCENTRATION);
    }

    PyObject *getBoundarySpeciesIds() {
        return rr_ExecutableModel_getIds($self, rr::SelectionRecord::BOUNDARY_AMOUNT);
    }

    PyObject *getBoundarySpeciesConcentrationIds() {
        return rr_ExecutableModel_getIds($self, rr::SelectionRecord::BOUNDARY_CONCENTRATION);
    }

    PyObject *getGlobalParameterIds() {
        return rr_ExecutableModel_getIds($self, rr::SelectionRecord::GLOBAL_PARAMETER);
    }

    PyObject *getCompartmentIds() {
        return rr_ExecutableModel_getIds($self, rr::SelectionRecord::COMPARTMENT);
    }

    PyObject *getConservedMoietyIds() {
        return rr_ExecutableModel_getIds($self, rr::SelectionRecord::CONSERVED_MOIETY);
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

    PyObject *getEventIds() {
        return rr_ExecutableModel_getEventIds($self);
    }

    %pythoncode %{
        def getAllTimeCourseComponentIds(self):
            """

            ExecutableModel.getAllTimeCourseComponentIds([index])

            Return a list of all component ids. The list includes ids of amount/concentration of
            floating species, boundary species, global parameters, compartments, and reactions, as well as `time`.

            :returns: a list of all component ids widely used in time course selections.

            """

            return (['time'] + self.getFloatingSpeciesIds() + self.getBoundarySpeciesIds()
            + self.getFloatingSpeciesConcentrationIds() + self.getBoundarySpeciesConcentrationIds()
            + self.getGlobalParameterIds() + self.getCompartmentIds() + self.getReactionIds())
    %}




    /***
     ** set values section
     ***/

    int setFloatingSpeciesAmounts(size_t len, double const *values) {
        return $self->setFloatingSpeciesAmounts(len, 0, values);
    }

    int setFloatingSpeciesConcentrations(size_t len, double const *values) {
        return $self->setFloatingSpeciesConcentrations(len, 0, values);
    }

    int setBoundarySpeciesConcentrations(size_t len, double const *values) {
        return $self->setBoundarySpeciesConcentrations(len, 0, values);
    }

    int setGlobalParameterValues(size_t len, double const *values) {
        return $self->setGlobalParameterValues(len, 0, values);
    }

    int setCompartmentVolumes(size_t len, double const *values) {
        return $self->setCompartmentVolumes(len, 0, values);
    }

    int setConservedMoietyValues(size_t len, double const *values) {
        return $self->setConservedMoietyValues(len, 0, values);
    }

    int setFloatingSpeciesInitConcentrations(size_t len, double const *values) {
        return $self->setFloatingSpeciesInitConcentrations(len, 0, values);
    }

    int setFloatingSpeciesInitAmounts(size_t len, double const *values) {
        return $self->setFloatingSpeciesInitAmounts(len, 0, values);
    }

    int setCompartmentInitVolumes(size_t len, double const *values) {
        return $self->setCompartmentInitVolumes(len, 0, values);
    }


    int setFloatingSpeciesAmounts(size_t leni, int const* indx, int lenv, double const *values) {
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


    PyObject* getCurrentStoichiometryMatrix() {
        int rows = 0;
        int cols = 0;
        double* data = 0;

        $self->getStoichiometryMatrix(&rows, &cols, &data);

        int nd = 2;
        npy_intp dims[2] = {rows, cols};

        PyObject *pArray = PyArray_New(&PyArray_Type, nd, dims, NPY_DOUBLE, NULL, data, 0,
                NPY_ARRAY_CARRAY | NPY_ARRAY_OWNDATA, NULL);

//        VERIFY_PYARRAY(pArray);

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

%extend rr::Logger {
    static void enablePythonLogging() {
        PyLoggerStream::enablePythonLogging();
    }

    static void disablePythonLogging() {
        PyLoggerStream::disablePythonLogging();
    }
}

%extend rr::Solver {
    %pythoncode %{
        def __dir__(self):
            x = dir(type(self))
            x += self.getSettings()
            return x

        def __getattr__(self, name):
            if name in self.getSettings():
                return Solver.getValue(self, name)
            else:
                return self.__dict__[name]

        def __setattr__(self, name, value):
            if(name != 'this' and name in self.getSettings()):
                self.setValue(name, value)
            else:
                self.__dict__[name] = value

        def getSetting(self, k):
            return self.getValue(k)

        def setSetting(self, k, v):
            return self.setValue(k, v)

        def setValues(self, keys, values):
            for key, val in zip(keys, values):
                _roadrunner.Solver_setValue(self, key, val)
    %}
}

%extend rr::Integrator {

    void _setListener(const rr::PyIntegratorListenerPtr &listener) {

        rrLog(rr::Logger::LOG_INFORMATION) << __FUNC__ << ", use count: " << listener.use_count();

        cxx11_ns::shared_ptr<rr::IntegratorListener> i =
            cxx11_ns::dynamic_pointer_cast<rr::IntegratorListener>(listener);

        rrLog(rr::Logger::LOG_INFORMATION) << __FUNC__ << ", after cast use count: " << listener.use_count();

        ($self)->setListener(i);
    }

    rr::PyIntegratorListenerPtr _getListener() {

        rrLog(rr::Logger::LOG_INFORMATION) << __FUNC__;

        rr::IntegratorListenerPtr l = ($self)->getListener();

        rr::PyIntegratorListenerPtr ptr =
            cxx11_ns::dynamic_pointer_cast<rr::PyIntegratorListener>(l);

        rrLog(rr::Logger::LOG_INFORMATION) << __FUNC__ << ", use count: " << ptr.use_count();

        return ptr;
    }

    void _clearListener() {
        rr::IntegratorListenerPtr current = ($self)->getListener();

        rrLog(rr::Logger::LOG_INFORMATION) << __FUNC__ << ", current use count before clear: " << current.use_count();

        ($self)->setListener(rr::IntegratorListenerPtr());

        rrLog(rr::Logger::LOG_INFORMATION) << __FUNC__ << ", current use count after clear: " << current.use_count();
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

        listener = property(getListener, setListener)

        def __dir__(self):
            x = dir(type(self))
            x += self.getSettings()
            return x

        def __getattr__(self, name):
            if(name in self.getSettings()):
                return Solver.getValue(self, name)
            else:
                return self.__dict__[name]

        def __setattr__(self, name, value):
            if(name != 'this' and name in self.getSettings()):
                self.setValue(name, value)
            else:
                self.__dict__[name] = value

        def __repr__(self):
            return self.toRepr()

        def getSetting(self, k):
            return self.getValue(k)

        def setSetting(self, k, v):
            return self.setValue(k, v)
    %}
}

%extend rr::SteadyStateSolver {
    %pythoncode %{
        def __dir__(self):
            x = dir(type(self))
            x += self.getSettings()
            return x

        def __getattr__(self, name):
            if(name in self.getSettings()):
                return Solver.getValue(self, name)
            else:
                return self.__dict__[name]

        def __setattr__(self, name, value):
            if(name != 'this' and name in self.getSettings()):
                self.setValue(name, value)
            else:
                self.__dict__[name] = value

        def __repr__(self):
            return self.toRepr()

        def getSetting(self, k):
            return self.getValue(k)

        def setSetting(self, k, v):
            return self.setValue(k, v)
    %}
}

%extend rr::PyIntegratorListener {
    %pythoncode %{
        onTimeStep = property(getOnTimeStep, setOnTimeStep)

        onEvent = property(getOnEvent, setOnEvent)
     %}
}

%extend rr::PyEventListener {
    %pythoncode %{
        onTrigger = property(getOnTrigger, setOnTrigger)

        onAssignment = property(getOnAssignment, setOnAssignment)
     %}
}


// Copy this code ad verbatim to the Python module
%pythoncode %{
RoadRunner.registerSolvers()
integrators = list(RoadRunner.getRegisteredIntegratorNames())
steadyStateSolvers = list(RoadRunner.getRegisteredSteadyStateSolverNames())
solvers = integrators + steadyStateSolvers
%}


// no need to port the mutexes
%ignore rr::sensitivitySolverMutex;
%ignore rr::sensitivityRegistrationMutex;
%ignore rr::steadyStateSolverFactoryMutex;
%ignore rr::steadyStateSolverRegistrationMutex;
%ignore rr::integratorFactoryMutex;
%ignore rr::integratorRegistrationMutex;


// sundials steady state solvers
%include "KinsolSteadyStateSolver.h"
%include "NewtonIteration.h"
%include "BasicNewtonIteration.h"
%include "LinesearchNewtonIteration.h"
%include "NLEQSolver.h"
%include "NLEQ1Solver.h"
%include "NLEQ2Solver.h"

// sundials Sensitivity solvers
%include "ForwardSensitivitySolver.h"


// Integrators
%include "CVODEIntegrator.h"
%include "GillespieIntegrator.h"
%include "RK4Integrator.h"
%include "RK45Integrator.h"
%include "EulerIntegrator.h"















