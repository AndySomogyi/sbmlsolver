%module TestModelFactory

%{
#define SWIG_FILE_WITH_INIT
#define PY_ARRAY_UNIQUE_SYMBOL RoadRunner_ARRAY_API // for the NamedArrayObject (see roadrunner.i)

#include <map>
#include <unordered_map>
#include <utility>
#include <iostream>

#include "Variant.h" // for storing settings
#include "PyUtils.h" // for variant_to_py and back.

#include "TestModelFactory.h"
typedef std::unordered_map<std::string, std::pair<double, double>> ResultMap;
using namespace rr;
%}

// since we be using the ls::Matrix from a Python
// we'll be using the same API needed by the main roadrunner bindings
%include "numpy.i"
%init %{
import_array();
rr::pyutil_init(m);
%}

%include "std_string.i"
%include "std_vector.i"
%include "std_pair.i"
%include "std_unordered_map.i"
%include "rr_variant.i"

%typedef std::unordered_map<std::string, std::pair<double, double>> ResultMap;


%template(DoublePair) std::pair<double, double>;
%template(DoubleDoubleMap) std::unordered_map<double, double>;
%template(StringDoubleMap) std::unordered_map<std::string, double >;
%template(ResultMap) std::unordered_map<std::string, std::pair<double, double>>;

%template(StringVariantMap) std::unordered_map<std::string, rr::Variant>;


%include "TestModelFactory.h"

//class SimpleFlux : public SBMLTestModel, public TimeSeriesResult, public SteadyStateResult {
//public:
//
//    std::string str() override;
//
//    std::string modelName() override;
//
//    ResultMap stateVectorAtT10() override;
//
//    ResultMap steadyState() override;
//
//    std::unordered_map<std::string, rr::Variant> settings() override;
//};


%typemap(out) SBMLTestModel *TestModelFactory{
        const std::string lookup_typename = *arg1 + " *";
        swig_type_info * const outtype = SWIG_TypeQuery(lookup_typename.c_str());
        $result = SWIG_NewPointerObj(SWIG_as_voidptr($1), outtype, $owner);
}


