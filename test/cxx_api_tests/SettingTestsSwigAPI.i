%module SettingTestsSwigAPI

%{
#define SWIG_FILE_WITH_INIT
#include <unordered_map>
#include "Setting.h"
#include "SettingTestsSwigAPI.h"
#include <variant> // for visit
    #define PY_ARRAY_UNIQUE_SYMBOL RoadRunner_ARRAY_API
    #include <numpy/arrayobject.h>
#include "PyUtils.h"
%}
#include <iostream>
%include "std_basic_string.i"
#include "PyUtils.h"
    #include <numpy/arrayobject.h>

// converts a C++ rr::Setting to a Python variable, depending on its type
%typemap(out) rr::Setting{
    try {
        // I'm a marker rr::Setting(out). Look for me in the swig_wrap.cxx file
        // to verify that this type map is being properly applied
        $result = rr::Variant_to_py($1);
    } catch (const std::exception& e){
        SWIG_exception(SWIG_RuntimeError, e.what());
    }
}
%apply rr::Setting{
    const rr::Setting&,
    rr::Setting&
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
    const rr::Setting&,
    rr::Setting&,
    rr::Setting
};


%include "SettingTestsSwigAPI.h"

















