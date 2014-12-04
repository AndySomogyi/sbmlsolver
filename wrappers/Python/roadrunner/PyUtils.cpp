/*
 * PyUtils.cpp
 *
 *  Created on: Apr 27, 2014
 *      Author: andy
 */

// see discission on import array,
// http://docs.scipy.org/doc/numpy/reference/c-api.array.html#miscellaneous
#define NO_IMPORT_ARRAY
#define PY_ARRAY_UNIQUE_SYMBOL RoadRunner_ARRAY_API

#include <stdexcept>
#include <string>

// wierdness on OSX clang, this needs to be included before python.h,
// otherwise compile pukes with:
// localefwd.h error: too many arguments provided to function-like macro invocation
#include <sstream>
#include <vector>
#include <PyUtils.h>
#include <rrLogger.h>
#include <Dictionary.h>
#include "rrRoadRunnerOptions.h"
#include <numpy/arrayobject.h>




using namespace std;

namespace rr
{


PyObject* Variant_to_py(const Variant& var)
{
    PyObject *result = 0;

    const std::type_info &type = var.typeInfo();

    if (var.isEmpty()) {
        Py_RETURN_NONE;
    }

    if (type == typeid(std::string)) {
        return PyString_FromString(var.convert<string>().c_str());
    }

    if (type == typeid(bool)) {
        return PyBool_FromLong(var.convert<bool>());
    }

    if (type == typeid(unsigned long)) {
        return PyLong_FromUnsignedLong(var.convert<unsigned long>());
    }

    if (type == typeid(long)) {
        return PyLong_FromLong(var.convert<long>());
    }

    if (type == typeid(int)) {
        return PyInt_FromLong(var.convert<long>());
    }

    if (type == typeid(unsigned int)) {
        return PyLong_FromUnsignedLong(var.convert<unsigned long>());
    }

    if (type == typeid(char)) {
        char c = var.convert<char>();
        return PyString_FromStringAndSize(&c, 1);
    }

    if (type == typeid(unsigned char)) {
        return PyInt_FromLong(var.convert<long>());
    }

    if (type == typeid(float) || type == typeid(double)) {
        return PyFloat_FromDouble(var.convert<double>());
    }


    throw invalid_argument("could not convert " + var.toString() + "to Python object");
}

Variant Variant_from_py(PyObject* py)
{
    Variant var;

    if(py == Py_None)
    {
        return var;
    }

    if (PyString_Check(py))
    {
        var = std::string(PyString_AsString(py));
        return var;
    }

    else if (PyBool_Check(py))
    {
        var = (bool)(py == Py_True);
        return var;
    }

    else if (PyLong_Check(py))
    {
        // need to check for overflow.
        var = (long)PyLong_AsLong(py);

        // Borrowed reference.
        PyObject* err = PyErr_Occurred();
        if (err) {
            std::stringstream ss;
            ss << "Could not convert Python long to C ";
            ss << sizeof(long) * 8 << " bit long: ";
            ss << std::string(PyString_AsString(err));

            // clear error, raise our own
            PyErr_Clear();

            invalid_argument(ss.str());
        }

        return var;
    }

    else if (PyInt_Check(py))
    {
        var = (int)PyInt_AsLong(py);
        return var;
    }

    else if (PyFloat_Check(py))
    {
        var = (double)PyFloat_AsDouble(py);
        return var;
    }

    string msg = "could not convert Python type to built in type";
    throw invalid_argument(msg);
}

PyObject* dictionary_keys(const Dictionary* dict)
{
    std::vector<std::string> keys = dict->getKeys();

    unsigned size = keys.size();

    PyObject* pyList = PyList_New(size);

    unsigned j = 0;

    for (std::vector<std::string>::const_iterator i = keys.begin(); i != keys.end(); ++i)
    {
        const std::string& key  = *i;
        PyObject* pyStr = PyString_FromString(key.c_str());
        PyList_SET_ITEM(pyList, j++, pyStr);
    }

    return pyList;
}

PyObject* dictionary_values(const Dictionary* dict)
{
    std::vector<std::string> keys = dict->getKeys();

    unsigned size = keys.size();

    PyObject* pyList = PyList_New(size);

    unsigned j = 0;

    for (std::vector<std::string>::const_iterator i = keys.begin(); i != keys.end(); ++i)
    {
        const std::string& key  = *i;
        PyObject* pyVal = Variant_to_py(dict->getItem(key));
        PyList_SET_ITEM(pyList, j++, pyVal);
    }

    return pyList;
}

PyObject* dictionary_items(const Dictionary* dict)
{
    std::vector<std::string> keys = dict->getKeys();

    unsigned size = keys.size();

    PyObject* pyList = PyList_New(size);

    unsigned j = 0;

    for (std::vector<std::string>::const_iterator i = keys.begin(); i != keys.end(); ++i)
    {
        const std::string& key  = *i;
        PyObject* pyStr = Variant_to_py(dict->getItem(key));

        PyObject *pyKey = PyString_FromString(key.c_str());
        PyObject *pyVal = Variant_to_py(dict->getItem(key));
        PyObject *tup = PyTuple_Pack(2, pyKey, pyVal);

        Py_DECREF(pyKey);
        Py_DECREF(pyVal);

        // list takes ownershipt of tuple
        PyList_SET_ITEM(pyList, j++, tup);
    }

    return pyList;
}

PyObject* dictionary_getitem(const Dictionary* dict, const char* key)
{
    return Variant_to_py(dict->getItem(key));
}

PyObject* dictionary_setitem(Dictionary* dict, const char* key, PyObject* value)
{
    dict->setItem(key, Variant_from_py(value));
    Py_RETURN_NONE;
}

void dictionary_delitem(Dictionary* dict, const char* key)
{
    dict->deleteItem(key);
}

PyObject* dictionary_contains(const Dictionary* dict, const char* key)
{
    bool contains = dict->hasKey(key);
    return PyBool_FromLong(contains);
}

#define VERIFY_PYARRAY(p) { \
    assert(p && "PyArray is NULL"); \
    assert((PyArray_NBYTES(p) > 0 ? PyArray_ISCARRAY(p) : true) &&  "PyArray must be C format"); \
}

/*  original %typemap(out) ls::DoubleMatrix {
 *
    PyObject *pArray = NULL;
    int rows = ($1).numRows();
    int cols = ($1).numCols();
    double *data = (double*)malloc(sizeof(double)*rows*cols);
    memcpy(data, ($1).getArray(), sizeof(double)*rows*cols);

    if(cols == 1) {
        int nd = 1;
        npy_intp dims[1] = {rows};
        pArray = PyArray_New(&PyArray_Type, nd, dims, NPY_DOUBLE, NULL, data, 0,
                             NPY_CARRAY | NPY_OWNDATA, NULL);

    }
    else {
        int nd = 2;
        npy_intp dims[2] = {rows, cols};
        pArray = PyArray_New(&PyArray_Type, nd, dims, NPY_DOUBLE, NULL, data, 0,
                             NPY_CARRAY | NPY_OWNDATA, NULL);
    }

    VERIFY_PYARRAY(pArray);
    $result  = pArray;
 */

/*  original %typemap(out) const ls::DoubleMatrix* {
 *
    PyObject *pArray = NULL;
    int rows = ($1)->numRows();
    int cols = ($1)->numCols();
    double *data = ($1)->getArray();

    if(cols == 1) {
        int nd = 1;
        npy_intp dims[1] = {rows};
        pArray = PyArray_New(&PyArray_Type, nd, dims, NPY_DOUBLE, NULL, data, 0,
                             NPY_CARRAY, NULL);
    }
    else {
        int nd = 2;
        npy_intp dims[2] = {rows, cols};
        pArray = PyArray_New(&PyArray_Type, nd, dims, NPY_DOUBLE, NULL, data, 0,
                             NPY_CARRAY, NULL);
    }

    VERIFY_PYARRAY(pArray);
    $result  = pArray;
 */


PyObject* doublematrix_to_py(const ls::DoubleMatrix* m, uint32_t flags)
{

    Log(Logger::LOG_NOTICE) << __PRETTY_FUNCTION__;

    ls::DoubleMatrix *mat = const_cast<ls::DoubleMatrix*>(m);


    // a valid array descriptor:
    // In [87]: b = array(array([0,1,2,3]),
    //      dtype=[('r', 'f8'), ('g', 'f8'), ('b', 'f8'), ('a', 'f8')])


    // are we returning a structured array?
    if (flags & SimulateOptions::STRUCTURED_RESULT) {

        // get the column names
        //const std::vector<SelectionRecord>& sel = ($self)->getSelections();
        //std::vector<string> names(sel.size());

        //for(int i = 0; i < sel.size(); ++i) {
        //    names[i] = sel[i].to_string();
        //}

        std::vector<string> names = mat->getColumnNames();


        int rows = mat->numRows();
        int cols = mat->numCols();

        if (cols == 0) {
            Py_RETURN_NONE;
        }

        if (cols != names.size()) {
            throw std::logic_error("column names size does not match matrix columns size");
        }

        double* mData = mat->getArray();

        PyObject* list = PyList_New(names.size());

        for(int i = 0; i < names.size(); ++i)
        {
            PyObject *col = PyString_FromString(names[i].c_str());
            PyObject *type = PyString_FromString("f8");
            PyObject *tup = PyTuple_Pack(2, col, type);

            Py_DECREF(col);
            Py_DECREF(type);

            // list takes ownershipt of tuple
            void PyList_SET_ITEM(list, i, tup);
        }

        PyArray_Descr* descr = 0;
        PyArray_DescrConverter(list, &descr);

        // done with list
        Py_CLEAR(list);
        npy_intp dims[] = {rows};

        // steals a reference to descr
        PyObject *pyres = PyArray_SimpleNewFromDescr(1, dims,  descr);
        VERIFY_PYARRAY(pyres);

        if (pyres) {

            assert(PyArray_NBYTES(pyres) == rows*cols*sizeof(double) && "invalid array size");

            double* data = (double*)PyArray_BYTES(pyres);

            memcpy(data, mData, rows*cols*sizeof(double));
        }

        return pyres;
    }
    // standard array result.
    // this version just wraps the roadrunner owned data.
    else {

        int rows = mat->numRows();
        int cols = mat->numCols();
        int nd = 2;
        npy_intp dims[2] = {rows, cols};
        PyObject *pArray = NULL;

        if (flags & SimulateOptions::COPY_RESULT) {

            /*

            Log(rr::Logger::LOG_DEBUG) << "copying result data";

            pArray = PyArray_SimpleNew(2, dims, NPY_DOUBLE);

            VERIFY_PYARRAY(pArray);
            assert(PyArray_NBYTES(pArray) == rows*cols*sizeof(double) && "invalid array size");

            double *pyData = (double*)PyArray_BYTES(pArray);
            double *mData = mat->getArray();

            memcpy(pyData, mData, rows*cols*sizeof(double));
            */

            Log(rr::Logger::LOG_DEBUG) << "copying result data";

            double *data = (double*)malloc(sizeof(double)*rows*cols);
            memcpy(data, mat->getArray(), sizeof(double)*rows*cols);

            if(cols == 1) {
                int nd = 1;
                npy_intp dims[1] = {rows};
                pArray = PyArray_New(&PyArray_Type, nd, dims, NPY_DOUBLE, NULL, data, 0,
                                     NPY_CARRAY | NPY_OWNDATA, NULL);

            }
            else {
                int nd = 2;
                npy_intp dims[2] = {rows, cols};
                pArray = PyArray_New(&PyArray_Type, nd, dims, NPY_DOUBLE, NULL, data, 0,
                                     NPY_CARRAY | NPY_OWNDATA, NULL);
            }

            VERIFY_PYARRAY(pArray);
        }
        else {

            Log(rr::Logger::LOG_DEBUG) << "wraping existing data";


            double *data = mat->getArray();

            if(cols == 1) {
                int nd = 1;
                npy_intp dims[1] = {rows};
                pArray = PyArray_New(&PyArray_Type, nd, dims, NPY_DOUBLE, NULL, data, 0,
                                     NPY_CARRAY, NULL);
            }
            else {
                int nd = 2;
                npy_intp dims[2] = {rows, cols};
                pArray = PyArray_New(&PyArray_Type, nd, dims, NPY_DOUBLE, NULL, data, 0,
                                     NPY_CARRAY, NULL);
            }

            VERIFY_PYARRAY(pArray);

        }
        return pArray;
    }
}





} /* namespace rr */

