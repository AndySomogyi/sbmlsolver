/*
 * PyUtils.cpp
 *
 *  Created on: Apr 27, 2014
 *      Author: andy
 */

// see discission on import array,
// http://docs.scipy.org/doc/numpy/reference/c-api.array.html#miscellaneous

/**
 * @brief define some macro's to allow numpy api to be used
 * in multiple compilation units.
 *
 * @details We need this because the swig generated roadrunnerPYTOHN_wrap.cxx
 * and PyUtils.cpp both use the numpy C API.
 *
 * By default the numpy C API is stored as an
 * array of pointers (void **) called PyArray_API. This
 * is initialized when import_array() is called. What we'll get is
 *
 *  static void** PyArray_API;
 *
 * There could be problems with having a static PyArray_API when multiple
 * compilations units are in effect. The PY_ARRAY_UNIQUE_SYMBOL macro
 * is defined to deal with this problem. This renames the PyArray_API
 * to whatever PY_ARRAY_UNIQUE_SYMBOL is defined to be, in our case
 * RoadRunner_ARRAY_API (see numpy's __multiarray_api.h for
 * macro definitions). PY_ARRAY_UNIQUE_SYMBOL also makes the
 * declaration of the numpy api non-static.
 *
 * When combined with NO_IMPORT_ARRAY (or equally, NO_IMPORT) the declaration
 * becomes
 *  `extern void** RoadRunner_ARRAY_API;`
 * making the API a global variable. Therefore we have access to the RoadRunner_ARRAY_API
 * from other compilation units. The second consequence of NO_IMPORT_ARRAY
 * is that it removes the definition of import_array, so that it cannot be called
 * from within this compilation unit.
 *
 * So, since import_array() is called from the swig
 * %init we define NO_IMPORT_ARRAY and PY_ARRAY_UNIQUE_SYMBOL
 * to no reimport array and get a handle on the name of the api.
 *
 */
#define NO_IMPORT_ARRAY
#define PY_ARRAY_UNIQUE_SYMBOL RoadRunner_ARRAY_API
//Still using some of deprecated wrappers
//#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION

#include <stdexcept>
#include <string>
#include <algorithm>

// wierdness on OSX clang, this needs to be included before python.h,
// otherwise compile pukes with:
// localefwd.h error: too many arguments provided to function-like macro invocation
#include <sstream>
#include <vector>
#include "PyUtils.h"
#include <rrLogger.h>
#include <Dictionary.h>
#include "rrConfig.h"
#include "rrRoadRunnerOptions.h"
#include <numpy/arrayobject.h>
#include "structmember.h"

// python object.h
#include "object.h"

#include <iostream>
#include <iomanip>
#include <Matrix.h>
#include <filesystem>
#include <fstream>

using namespace std;

typedef vector<string> str_vector;

namespace rr {

    //    https://pythonextensionpatterns.readthedocs.io/en/latest/pickle.html#implementing-getstate
    static const char *PICKLE_VERSION_KEY = "_pickle_version";
    static int PICKLE_VERSION = 5;

#define STRINGIFY(x) #x
#define EXPAND_AND_STRINGIFY(x) STRINGIFY(x)

    char *rr_strclone(const char *src) {
        if (!src) {
            assert(0 && "rr_strclone passed null arg");
            return NULL;
        } else {
            size_t size = strlen(src) + 1;
            char *dst = (char *) malloc(size * sizeof(char));

            memcpy(dst, src, size); // copies null char

            return dst;
        }
    }

    void rr_strfree(const char *str) {
        free((void *) str);
    }

/**
 * @brief Get the UTF-8 encoded buffer for a Python string
 * @details Imported from graphfab
 * @note Caller must free the buffer using @ref rr_strfree
 */
    char *rrPyString_getString(PyObject *uni) {
        char *str = NULL;
//     #pragma message "PY_MAJOR_VERSION = " EXPAND_AND_STRINGIFY(PY_MAJOR_VERSION)
#if PY_MAJOR_VERSION == 3
        PyObject *bytes = PyUnicode_AsUTF8String(uni);
        str = rr_strclone(PyBytes_AsString(bytes));
        Py_XDECREF(bytes);
#else
        str = rr_strclone(PyString_AsString(uni));
#endif
        return str;
    }

    std::string rrPyString_getCPPString(PyObject *uni) {
        char *cstr = rrPyString_getString(uni);
        std::string str(cstr);
        rr_strfree(cstr);
        return str;
    }

    int rrPyCompareString(PyObject *uni, const char *str) {
#if SAGITTARIUS_DEBUG_LEVEL >= 2
        {
            printf("PyCompareString started\n");
        }
#endif
        {
            char *s = rrPyString_getString(uni);
            int cmp = !strcmp(s, str);
            rr_strfree(s);

            if (cmp)
                return 1;
            else
                return 0;
        }
    }

    PyObject *rrPyString_FromString(const char *s) {
#if PY_MAJOR_VERSION == 3
        return PyUnicode_FromString(s);
#else
        return PyString_FromString(s);
#endif
    }

    PyObject *rrPyString_FromStringAndSize(const char *s, Py_ssize_t size) {
#if PY_MAJOR_VERSION == 3
        return PyUnicode_FromStringAndSize(s, size);
#else
        return PyString_FromStringAndSize(s, size);
#endif
    }

    char *rrPyString_AsString(PyObject *s) {
#if PY_MAJOR_VERSION == 3
        char *str;
        PyObject *bytes = PyUnicode_AsUTF8String(s);
        str = rr_strclone(PyBytes_AsString(bytes));
        Py_XDECREF(bytes);
        return str;
#else
        return rr_strclone(PyString_AsString(s));
#endif
    }

    char *rrGetPyErrMessage() {
        // It is a bit convoluted to get the Python error message as a string
        // from https://stackoverflow.com/a/1418703
        // Don't need to free these as they are only "references"
        PyObject *ptype, *pvalue, *ptraceback;
        PyErr_Fetch(&ptype, &pvalue, &ptraceback);

        if (ptype == NULL) {
            return NULL;
        }

        //Get error message
        char *pStrErrorMessage = rrPyString_AsString(pvalue);
        return pStrErrorMessage;
    }

    PyObject *Variant_to_py(const Setting &var) {
        PyObject *result = nullptr;

        const std::type_info &type = var.typeInfo();

        if (type == typeid(std::monostate)) {
            Py_RETURN_NONE;
        }

        if (type == typeid(std::string)) {
            return rrPyString_FromString(var.get<string>().c_str());
        }

        if (type == typeid(bool)) {
            return PyBool_FromLong(var.get<bool>());
        }

        if (type == typeid(std::uint64_t)) {
            return PyLong_FromUnsignedLong(var.get<std::uint64_t>());
        }

        if (type == typeid(std::int64_t)) {
            return PyLong_FromLong(var.get<std::int64_t>());
        }

        if (type == typeid(std::int32_t)) {
# if PY_MAJOR_VERSION == 3
            return PyLong_FromLong(var.get<std::int32_t>());
# else
            return PyInt_FromLong(var.get<std::int32_t>());
# endif
        }

        if (type == typeid(std::uint32_t)) {
            return PyLong_FromUnsignedLong(var.get<std::uint32_t>());
        }

        if (type == typeid(char)) {
            char c = var.get<char>();
            return rrPyString_FromStringAndSize(&c, 1);
        }

        if (type == typeid(unsigned char)) {
# if PY_MAJOR_VERSION == 3
            return PyLong_FromLong((long) var.get<std::int64_t>());
# else
            return PyInt_FromLong(var.get<std::int64_t>());
# endif
        }

        if (type == typeid(float)) {
            return PyFloat_FromDouble(var.get<float>());
        }

        if (type == typeid(double)) {
            return PyFloat_FromDouble(var.get<double>());
        }

        if (type == typeid(vector<double>)) {
            PyObject *list = PyList_New((std::int64_t) var.get<vector<double> >().size());
            if (!list) throw logic_error("Unable to allocate memory for Python list");
            for (unsigned int i = 0; i < var.get<vector<double> >().size(); i++) {
                PyObject *num = PyFloat_FromDouble((double) var.get<vector<double> >()[i]);
                if (!num) {
                    Py_DECREF(list);
                    throw logic_error("Unable to allocate memory for Python list");
                }
                PyList_SET_ITEM(list, i, num);
            }

            return list;
        }


        throw invalid_argument("could not convert " + var.get<std::string>() + "to Python object");
    }

    PyObject *Variant_to_py(Setting *var) {
        return Variant_to_py(*var);
    }

    Setting Variant_from_py(PyObject *py) {
        Setting var;

        if (py == Py_None) {
            return var;
        }

# if PY_MAJOR_VERSION == 3
        if (PyUnicode_Check(py))
# else
            if (PyString_Check(py))
# endif
        {
            var = rrPyString_getCPPString(py);
            return var;
        } else if (PyBool_Check(py)) {
            var = (bool) (py == Py_True);
            return var;
        } else if (PyLong_Check(py)) {
            // need to check for overflow.
            var = std::int64_t(PyLong_AsLong(py));

            // Borrowed reference.
            PyObject *err = PyErr_Occurred();
            if (err) {
                char *message = rrGetPyErrMessage();
                std::stringstream ss;
                ss << "Could not convert Python long to C ";
                ss << sizeof(long) * 8 << " bit long: ";
                ss << std::string(message);
                // clear error, raise our own
                PyErr_Clear();
                rr_strfree(message);

                throw invalid_argument(ss.str());
            }

            return var;
        }

# if PY_MAJOR_VERSION == 3
        else if (PyLong_Check(py))
# else
            else if (PyInt_Check(py))
# endif
        {
# if PY_MAJOR_VERSION == 3
            var = std::int64_t(PyLong_AsLong(py));
# else
            var = (std::int32_t)PyInt_AsLong(py);
# endif
            return var;
        } else if (PyFloat_Check(py)) {
            var = PyFloat_AsDouble(py);
            return var;
        } else if (PyList_Check(py)) {
            std::vector<double> data;
            for (Py_ssize_t i = 0; i < PyList_Size(py); i++) {
                PyObject *value = PyList_GetItem(py, i);
                data.push_back(PyFloat_AsDouble(value));
            }
            return data;
        }


        std::string msg = "could not convert Python type to built in type";
        throw invalid_argument(msg);
    }

    PyObject *dictionary_keys(const Dictionary *dict) {
        std::vector<std::string> keys = dict->getKeys();

        size_t size = keys.size();

        PyObject *pyList = PyList_New(size);

        unsigned j = 0;

        for (std::vector<std::string>::const_iterator i = keys.begin(); i != keys.end(); ++i) {
            const std::string &key = *i;
            PyObject *pyStr = rrPyString_FromString(key.c_str());
            PyList_SET_ITEM(pyList, j++, pyStr);
        }

        return pyList;
    }

    PyObject *dictionary_values(const Dictionary *dict) {
        std::vector<std::string> keys = dict->getKeys();

        size_t size = keys.size();

        PyObject *pyList = PyList_New(size);

        unsigned j = 0;

        for (std::vector<std::string>::const_iterator i = keys.begin(); i != keys.end(); ++i) {
            const std::string &key = *i;
            PyObject *pyVal = Variant_to_py(dict->getItem(key));
            PyList_SET_ITEM(pyList, j++, pyVal);
        }

        return pyList;
    }

    PyObject *dictionary_items(const Dictionary *dict) {
        std::vector<std::string> keys = dict->getKeys();

        size_t size = keys.size();

        PyObject *pyList = PyList_New(size);

        unsigned j = 0;

        for (std::vector<std::string>::const_iterator i = keys.begin(); i != keys.end(); ++i) {
            const std::string &key = *i;
            PyObject *pyStr = Variant_to_py(dict->getItem(key));

            PyObject *pyKey = rrPyString_FromString(key.c_str());
            PyObject *pyVal = Variant_to_py(dict->getItem(key));
            PyObject *tup = PyTuple_Pack(2, pyKey, pyVal);

            Py_DECREF(pyKey);
            Py_DECREF(pyVal);

            // list takes ownershipt of tuple
            PyList_SET_ITEM(pyList, j++, tup);
        }

        return pyList;
    }

    PyObject *dictionary_getitem(const Dictionary *dict, const char *key) {
        return Variant_to_py(dict->getItem(key));
    }

    PyObject *dictionary_setitem(Dictionary *dict, const char *key, PyObject *value) {
        dict->setItem(key, Variant_from_py(value));
        Py_RETURN_NONE;
    }

    void dictionary_delitem(Dictionary *dict, const char *key) {
        dict->deleteItem(key);
    }

    PyObject *dictionary_contains(const Dictionary *dict, const char *key) {
        bool contains = dict->hasKey(key);
        return PyBool_FromLong(contains);
    }
/*
 *
 */
#define VERIFY_PYARRAY(p) { \
    assert(p && "PyArray is NULL"); \
    assert((PyArray_NBYTES(p) > 0 ? PyArray_ISCARRAY(p) : true) &&  "PyArray must be C format"); \
}

    PyObject *doublematrix_to_py(const ls::DoubleMatrix *m, bool structured_result, bool copy_result) {
        ls::DoubleMatrix *mat = const_cast<ls::DoubleMatrix *>(m);

        // a valid array descriptor:
        // In [87]: b = array(array([0,1,2,3]),
        //      dtype=[('r', 'f8'), ('g', 'f8'), ('b', 'f8'), ('a', 'f8')])


        // are we returning a structured array?
        if (structured_result) {

            // get the column names
            //const std::vector<SelectionRecord>& sel = ($self)->getSelections();
            //std::vector<string> names(sel.size());

            //for(int i = 0; i < sel.size(); ++i) {
            //    names[i] = sel[i].to_string();
            //}

            std::vector<string> names = mat->getColNames();

            unsigned int rows = mat->numRows();
            unsigned int cols = mat->numCols();

            if (cols == 0) {
                Py_RETURN_NONE;
            }

            if (cols != names.size()) {
                throw std::logic_error("column names size does not match matrix columns size");
            }

            double *mData = mat->getArray();

            PyObject *list = PyList_New(names.size());

            for (int i = 0; i < names.size(); ++i) {
                PyObject *col = rrPyString_FromString(names[i].c_str());
                PyObject *type = rrPyString_FromString("f8");
                PyObject *tup = PyTuple_Pack(2, col, type);

                Py_DECREF(col);
                Py_DECREF(type);

                // list takes ownershipt of tuple
                void PyList_SET_ITEM(list, i, tup);
            }

            PyArray_Descr *descr = nullptr;
            PyArray_DescrConverter(list, &descr);

            // done with list
            Py_CLEAR(list);
            npy_intp dims[] = {rows};

            // steals a reference to descr
            PyObject *pyres = PyArray_SimpleNewFromDescr(1, dims, descr);
//            VERIFY_PYARRAY(pyres);

            if (pyres) {

                assert(PyArray_NBYTES(pyres) == rows * cols * sizeof(double) && "invalid array size");

                double *data = (double *) PyArray_BYTES(pyres);

                memcpy(data, mData, sizeof(double) * rows * cols);
            }

            return pyres;
        }
            // standard array result.
            // this version just wraps the roadrunner owned data.
        else {
            int rows = mat->numRows();
            int cols = mat->numCols();
            PyObject *pArray = NULL;

            if (copy_result) {

                rrLog(rr::Logger::LOG_DEBUG) << "copying result data";

                // passing a NULL for data tells numpy to allocate its own data

                // make a 1D array in this case
                if (cols == 1 && mat->getColNames().size() == 0) {
                    int nd = 1;
                    npy_intp dims[1] = {rows};
                    pArray = PyArray_New(&PyArray_Type, nd, dims, NPY_DOUBLE,
                                         NULL, NULL, 0, 0, NULL);
                } else {
                    int nd = 2;
                    npy_intp dims[2] = {rows, cols};
                    pArray = NamedArray_New(nd, dims, NULL,
                                            0, mat);
                }

//                VERIFY_PYARRAY(pArray);

                // copy our data into the numpy array
                double *data = static_cast<double *>(PyArray_DATA(pArray));
                memcpy(data, mat->getArray(), sizeof(double) * rows * cols);

            } else {

                rrLog(rr::Logger::LOG_DEBUG) << "wraping existing data";

                double *data = mat->getArray();

                if (cols == 1 && mat->getColNames().size() == 0) {
                    int nd = 1;
                    npy_intp dims[1] = {rows};
                    pArray = PyArray_New(&PyArray_Type, nd, dims, NPY_DOUBLE,
                                         NULL, data, 0, NPY_ARRAY_CARRAY, NULL);
                } else {
                    int nd = 2;
                    npy_intp dims[2] = {rows, cols};
                    pArray = NamedArray_New(nd, dims, data,
                                            NPY_ARRAY_CARRAY, mat);
                }

//                                VERIFY_PYARRAY(pArray);

            }
            return pArray;
        }
    }

/**
 * Casts a rr::Matrix<double> to its superclass ls::DoubleMatrix
 * and reuses doublematrix_to_py
 */
    PyObject *rrDoubleMatrix_to_py(const rr::Matrix<double> *m, bool copy_result) {
        rr::Matrix<double> *mat = const_cast<rr::Matrix<double> *>(m);
        // this code doesn't work due to some bug in NamedArray stuff. No time to figure this out now
//        bool structured_result = true;
//        if (mat->rowNames.empty() && mat->colNames.empty()) {
//            structured_result = false;
//        }
//        std::cout << __FILE__ << ":" << __LINE__ << "is structured? :" << structured_result << std::endl;
        auto superMat = ls::DoubleMatrix(mat->getValues());
        return doublematrix_to_py(&superMat, false, copy_result);
    }


    /**
     * @brief Use the Python C API to convert a string vector to a Python list of
     * strings.
     */
    PyObject *convertStringVectorToPython(const std::vector<std::string> &vec);


    static void NamedArrayObject_dealloc(NamedArrayObject *self) {
        rrLog(Logger::LOG_INFORMATION) << __FUNC__;
        Py_XDECREF(self->rowNames);
        Py_XDECREF(self->colNames);

        PyObject *pself = (PyObject *) self;

        assert(pself->ob_type->tp_base == &PyArray_Type);
//        std::cout << "pself->ob_type->tp_name: " << pself->ob_type->tp_name << std::endl;
//        std::cout << "pself->ob_refcnt: " << pself->ob_refcnt << std::endl;
//        std::cout << pself-> << std::endl;
        PyArray_Type.tp_dealloc(pself);

        rrLog(Logger::LOG_INFORMATION) << __FUNC__ << ", Done";
    }


    static PyObject *NamedArrayObject_alloc(PyTypeObject *type, Py_ssize_t nitems) {
        rrLog(Logger::LOG_INFORMATION) << __FUNC__;
        PyObject *obj;

        assert(type->tp_basicsize == sizeof(NamedArrayObject));

        obj = (PyObject *) PyArray_malloc(type->tp_basicsize);
        PyObject_Init(obj, type);
        ((NamedArrayObject *) obj)->rowNames = NULL;
        ((NamedArrayObject *) obj)->colNames = NULL;

        rrLog(Logger::LOG_INFORMATION) << "created obj: " << obj;
        return obj;
    }


#define rrPyNullCheck(object, errMsg)  if (object == nullptr){ \
            std::cerr << (errMsg) << std::endl;                \
            PyErr_Print();                                     \
        }\


#define CSTR(str) const_cast<char*>(str)


    /**
     * @brief Define the data members of the NamedArray type.
     * @details Each entry of this array is a PyMemberDef
     * with the following signature:
     *    typedef struct PyMemberDef {
     *         const char *name;
     *         int         type;
     *         int         offset;
     *         int         flags;
     *         const char *doc;
     *     } PyMemberDef;
     * Each entry of the NamedArray_members array links an attribute of the Python side NamedArray
     * to the NamedArray C struct.
     *
     * The `(Py_ssize_t )offsetof(NamedArrayObject, x)` part returns the starting byte of data member x
     *
     * @see https://docs.python.org/3/extending/newtypes.html#generic-attribute-management
     * @see https://en.cppreference.com/w/cpp/types/offsetof
     * @see Python/include/structmember.h
     */
    static PyMemberDef NamedArray_members[] = {
            {CSTR("rownames"), T_OBJECT_EX, (Py_ssize_t) offsetof(NamedArrayObject, rowNames), 0, CSTR("row names")},
            {CSTR("colnames"), T_OBJECT_EX, (Py_ssize_t) offsetof(NamedArrayObject, colNames), 0, CSTR("column names")},
            {NULL}  /* Sentinel */
    };

    /**
     * @brief Define methods of the NamedArray object.
     * @details The struct PyMethodDef* tp_methods field
     * in the PyTypeObject points to this array of PyMethodDef
     * objects. Each entry in this array is an instance of PyMethodDef.
     * and has the following type:
     * typedef struct PyMethodDef {
     *    const char  *ml_name;        method name
     *    PyCFunction  ml_meth;        implementation function
     *    int          ml_flags;       flags
     *    const char  *ml_doc;         docstring
     * } PyMethodDef;
     * Note the deliberate sentinal entry at the end of the array.
     * @see https://docs.python.org/3/extending/newtypes.html#generic-attribute-management
     */
    static PyMethodDef NamedArray_methods[] = {
            {CSTR("__array_finalize__"), (PyCFunction) NamedArrayObject_Finalize, METH_VARARGS, CSTR("")},
            {CSTR("__reduce__"),         (PyCFunction) NamedArray___reduce__,     METH_NOARGS,  CSTR("")},
            {CSTR("__getstate__"),       (PyCFunction) NamedArray___getstate__,   METH_NOARGS,  CSTR("")},
            {CSTR("__setstate__"),       (PyCFunction) NamedArray___setstate__,   METH_O,       CSTR("")},
            {NULL}/* Sentinel */
    };


/**
 * binaryfunc PyMappingMethods.mp_subscript
 * This function is used by PyObject_GetItem() and has the same signature.
 * This slot must be filled for the PyMapping_Check() function to return 1,
 * it can be NULL otherwise.
 */
    static PyObject *NammedArray_subscript(NamedArrayObject *self, PyObject *op) {
        binaryfunc PyArray_subscript = PyArray_Type.tp_as_mapping->mp_subscript;
        int dim = PyArray_NDIM(self);

# if PY_MAJOR_VERSION == 3
        if (dim == 2 && PyUnicode_Check(op)) {
# else
            if (dim == 2 && PyString_Check(op)) {
# endif
            char *keyName = rrPyString_AsString(op);

            PyObject *colSeq = PySequence_Fast(self->colNames, "expected a sequence");
            Py_ssize_t len = PySequence_Size(colSeq);
            for (int col = 0; col < len; col++) {
                PyObject *item = PySequence_Fast_GET_ITEM(colSeq, col);
                char *itemStr = rrPyString_AsString(item);

                if (strcmp(keyName, itemStr) == 0) {

                    npy_intp rows = PyArray_DIM(self, 0);
                    npy_intp cols = PyArray_DIM(self, 1);

                    npy_intp dims[1] = {rows};
                    PyObject *result = PyArray_New(&PyArray_Type, 1, dims, NPY_DOUBLE, NULL, NULL, 0,
                                                   NPY_ARRAY_CARRAY, NULL);

                    // copy data to result array
                    double *data = (double *) PyArray_DATA(self);
                    double *newData = (double *) PyArray_DATA(result);

                    for (int i = 0; i < rows; ++i) {
                        newData[i] = data[(i * cols) + col];
                    }

                    // free the seq and return
                    Py_DECREF(colSeq);
                    return result;
                }

                rr_strfree(itemStr);
            }

            // did not find a col name, free the seq
            Py_DECREF(colSeq);


            // look for row names
            PyObject *rowSeq = PySequence_Fast(self->rowNames, "expected a sequence");
            len = PySequence_Size(rowSeq);
            for (int row = 0; row < len; ++row) {
                PyObject *item = PySequence_Fast_GET_ITEM(rowSeq, row);
                char *itemStr = rrPyString_AsString(item);

                if (strcmp(keyName, itemStr) == 0) {

                    npy_intp rows = PyArray_DIM(self, 0);
                    npy_intp cols = PyArray_DIM(self, 1);

                    npy_intp dims[1] = {cols};
                    PyObject *result = PyArray_New(&PyArray_Type, 1, dims, NPY_DOUBLE, NULL, NULL, 0,
                                                   NPY_ARRAY_CARRAY, NULL);

                    // copy data to result array
                    double *data = (double *) PyArray_DATA(self);
                    double *newData = (double *) PyArray_DATA(result);

                    for (int i = 0; i < cols; ++i) {
                        newData[i] = data[(row * cols) + i];
                    }

                    // free the seq and return
                    Py_DECREF(rowSeq);
                    return result;
                }

                rr_strfree(itemStr);
            }

            // did not find a col name, free the seq
            Py_DECREF(rowSeq);

            rr_strfree(keyName);

        }

        // not found in row or col names, return default numpy func.
        return PyArray_subscript((PyObject *) self, op);
    }


    static PyMappingMethods NamedArray_MappingMethods = {
#if PY_VERSION_HEX >= 0x02050000
            (lenfunc) 0,             /*mp_length*/
#else
            (inquiry)0,             /*mp_length*/
#endif
            (binaryfunc) 0,          /*mp_subscript*/
            (objobjargproc) 0,       /*mp_ass_subscript*/
    };

    static PyTypeObject NamedArray_Type = {
            PyVarObject_HEAD_INIT(NULL, 0)
            "NamedArray",                              /* tp_name */
            sizeof(NamedArrayObject),                              /* tp_basicsize */
            0,                              /* tp_itemsize */
            (destructor) NamedArrayObject_dealloc,                              /* tp_dealloc */
            0,                              /* tp_vectorcall_offset */
            0,                              /* tp_getattr */
            0,                              /* tp_setattr */
            0,                              /* tp_as_async */
            (reprfunc) NamedArray_repr,                              /* tp_repr */
            0,                              /* tp_as_number */
            0,                              /* tp_as_sequence */
            &NamedArray_MappingMethods,                              /* tp_as_mapping */
            0,                              /* tp_hash */
            0,                              /* tp_call */
            (reprfunc) NamedArray_str,                              /* tp_str */
            0,                              /* tp_getattro */
            0,                              /* tp_setattro */
            0,                              /* tp_as_buffer */
            Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,                              /* tp_flags */
            "NamedArray",                              /* tp_doc */
            0,                              /* tp_traverse */
            0,                              /* tp_clear */
            0,                              /* tp_richcompare */
            0,                              /* tp_weaklistoffset */
            0,                              /* tp_iter */
            0,                              /* tp_iternext */
            NamedArray_methods,                              /* tp_methods */
            NamedArray_members,                              /* tp_members */
            0,                              /* tp_getset */
            0,                              /* tp_base */
            0,                              /* tp_dict */
            0,                              /* tp_descr_get */
            0,                              /* tp_descr_set */
            0,                              /* tp_dictoffset */
            0,                              /* tp_init : note we use numpy's ct'r*/
            NamedArrayObject_alloc,                              /* tp_alloc */
            0,                              /* tp_new */
            0,                              /* tp_free */
            0,                              /* tp_is_gc */
            0,                              /* tp_bases */
            0,                              /* tp_mro */
            0,                              /* tp_cache */
            0,                              /* tp_subclasses */
            0,                              /* tp_weaklist */
            0,                              /* tp_del */
            0,                              /* tp_version_tag */
            0,                              /* tp_finalize */
    };


    /* Pickle the object */
    static PyObject *
    NamedArray___getstate__(NamedArrayObject *self, PyObject *Py_UNUSED(ignored)) {
        std::cout << "using NamedArray___getstate__" << std::endl;
        // see https://numpy.org/doc/stable/reference/c-api/array.html#c.PyArray_Dumps
        PyObject *arrayBytes = self->arrayToBytes();
        if (!arrayBytes) {
            PyErr_SetString(PyExc_ValueError, "Could not convert array to bytes");
            return nullptr;
        }
        // number of dimensions
        int nDims = PyArray_NDIM(self);
        std::int64_t dim1 = 0;
        std::int64_t dim2 = 0;

        npy_intp *shape = PyArray_SHAPE((PyArrayObject *) self);
        if (!shape) {
            PyErr_SetString(PyExc_ValueError, "Could not extract shape from array");
            return nullptr;
        }
        if (nDims > 0)
            dim1 = shape[0];
        if (nDims > 1)
            dim2 = shape[1];

        // string, bytes, string, object, string object string int
        PyObject *ret = Py_BuildValue("{sSsisLsLsOsOsi}",
                                      "array", arrayBytes,
                                      "nDims", nDims,
                                      "dim1", dim1,
                                      "dim2", dim2,
                                      "rownames", self->rowNames,
                                      "colnames", self->colNames,
                                      PICKLE_VERSION_KEY, PICKLE_VERSION);

        if (!ret) {
            PyErr_SetString(PyExc_ValueError, "Could not create dict using Py_BuildValue "
                                              "in NamedArray.__getstate__");
            return nullptr;
        }
        return ret;
    }

    static PyObject *
    NamedArray___reduce__(NamedArrayObject *self) {
        std::cout << "using NamedArray___reduce__" << std::endl;
        PyObject *state = NamedArray___getstate__(self, nullptr);
        if (!state) {
            // do not set an error here as it will override the
            // useful error from NamedArray___getstate__
            return nullptr;
        }

        // verify we have a dict
        PyDict_CheckExact(state);

        // borrowed reference
        PyObject *nDimsObj = PyDict_GetItemString(state, "nDims");
        if (!nDimsObj) {
            PyErr_SetString(PyExc_ValueError, "Could not extract nDims "
                                              "from state dict");
        }
        auto nDims = PyLong_AsLong(nDimsObj);
        PyObject *callableArgs = nullptr;
        if (nDims == 2) {
            callableArgs = PyTuple_Pack(
                    2,
                    PyDict_GetItemString(state, "dim1"),
                    PyDict_GetItemString(state, "dim2"));
        } else if (nDims == 1) {
            callableArgs = PyTuple_Pack(1, PyDict_GetItemString(state, "dim1"));
        } else {
            PyErr_Format(PyExc_ValueError, "Unexpected number of dimensions %i", nDims);
        }

        // import _roadrunner and grab the NamedArray object
        // from the Python end
        PyObject *roadrunner = PyImport_ImportModule("roadrunner._roadrunner");
        if (!roadrunner) {
            PyErr_SetString(PyExc_ImportError, "Could not import roadrunner._roadrunner");
            return nullptr;
        }
        PyObject *namedArrayObj = PyObject_GetAttrString(roadrunner, "NamedArray");
        if (!namedArrayObj) {
            PyErr_SetString(PyExc_AttributeError, "Could not find NamedArray in the roadrunner._roadrunner module");
            return nullptr;
        }

        PyObject *tup = nullptr;
        tup = Py_BuildValue(
                "(OOO)",
                /**
                 * A callable object that will be called to
                 * create the initial version of the object.
                 * Aka
                 */
                namedArrayObj,

                /**
                 * A tuple of arguments for the callable object.
                 * An empty tuple must be given if the callable
                 * does not accept any argument.
                 *
                 * Our __setstate__ method takes a
                 * NamedArrayObject* and a dict containing state
                 */
                callableArgs,

                /**
                 * Optionally, the object’s state, which will be passed to
                 * the object’s __setstate__() method as previously
                 * described. If the object has no such method then,
                 * the value must be a dictionary and it will be added
                 * to the object’s __dict__ attribute.
                 */
                state
        );

        Py_DecRef(roadrunner);
        Py_DecRef(namedArrayObj);

        return tup;
    }


    static PyObject *
    NamedArray___setstate__(NamedArrayObject *self, PyObject *state) {
        std::cout << "using NamedArray___setstate__" << std::endl;
        std::cout << state->ob_type->tp_name << std::endl;
        std::cout << PyTuple_Size(state) << std::endl;

        // ensure we have a dict object, ruling out dict subclasses
        if (!PyDict_CheckExact(state)) {
            PyErr_SetString(PyExc_ValueError, "Pickled object is not a dict");
        }

        // check pickle version. At the time of writing there is only one
        // pickle version, but this helps to future proof this code
        PyObject *pklVersionPyObj = PyDict_GetItemString(state, PICKLE_VERSION_KEY);
        if (pklVersionPyObj == nullptr) {
            /* PyDict_GetItemString does not set error, so we do it*/
            PyErr_Format(PyExc_KeyError, "No '%s' in pickled data", PICKLE_VERSION_KEY);
            return nullptr;
        }
        int pklVersion = (int) PyLong_AsLong(pklVersionPyObj);
        if (pklVersion != PICKLE_VERSION) {
            PyErr_Format(
                    PyExc_ValueError,
                    "Pickle version mismatch. Got version %d but expected version %d.",
                    pklVersion, PICKLE_VERSION);
            return nullptr;
        }

        // decrement the existing before assigning the new
        Py_DECREF(&self->array);

        // Grab the bytes object.
        PyObject *bytesObj = PyDict_GetItemString(state, "array"); // borrowed reference
        if (!bytesObj) {
            PyErr_SetString(PyExc_KeyError, "No 'array' key in pickled dict");
            return nullptr;
        }

        /**
         * We can load our bytes object back into a numpy array
         * with:
         *      PyObject *PyArray_FromBuffer(PyObject *buf, PyArray_Descr *dtype, npy_intp count, npy_intp offset)
         *
         */
        PyArray_Descr *descr = PyArray_DescrFromType(NPY_DOUBLE);
        PyObject *arr = PyArray_FromBuffer(bytesObj, descr, -1, 0);

        // but we need to reshape the result which is 1D
        auto nDims = PyLong_AsLong(PyDict_GetItemString(state, "nDims"));

        PyObject *shape = nullptr;
        if (nDims == 1) {
            shape = Py_BuildValue(
                    "i",
                    PyLong_AsLong(PyDict_GetItemString(state, "dim1"))
            );
        } else if (nDims == 2) {
            shape = Py_BuildValue(
                    "ii",
                    PyLong_AsLong(PyDict_GetItemString(state, "dim1")),
                    PyLong_AsLong(PyDict_GetItemString(state, "dim2"))
            );
        }

        PyObject* np = PyImport_ImportModule("numpy");
        if (!np){
            PyErr_SetString(PyExc_ImportError, "Cannot import numpy");
            return nullptr;
        }
        PyObject* resize = PyObject_GetAttrString(np,"resize");
        if (!resize){
            PyErr_SetString(PyExc_ImportError, "Cannot get resize method from numpy module");
            return nullptr;
        }
        // call resize.
        PyObject * newArr = PyObject_CallFunctionObjArgs(resize,arr, shape,NULL);
        if (!newArr){
            PyErr_SetString(PyExc_ValueError, "Failed to reshape NamedArray");
        }
        // newArr is a new reference so we decrement the old
        Py_DecRef(arr);
        Py_DecRef(np);
        Py_DecRef(resize);

        // We've been using a PyArrayObject until here.
        // assign its type back to NamedArray
        newArr->ob_type = &NamedArray_Type;

        // and finally we can assign the array
        self->array = *(PyArrayObject *) newArr;

        // increment the ref count of array for our instance
        Py_INCREF(&self->array);

        Py_DecRef(shape);

        // now for rownames,
        self->rowNames = PyDict_GetItemString(state, "rownames");
        if (!self->rowNames) {
            PyErr_SetString(PyExc_KeyError, "No key 'rownames' in state dict");
        }
        Py_INCREF(self->rowNames);

        // now for colnames,
        self->colNames = PyDict_GetItemString(state, "colnames");
        if (!self->colNames) {
            PyErr_SetString(PyExc_KeyError, "No key 'colnames' in state dict");
        }
        Py_INCREF(self->colNames);

        return Py_None;
    }

    PyObject *NamedArrayObject_Finalize(NamedArrayObject *self, PyObject *parent) {
        rrLog(Logger::LOG_INFORMATION) << __FUNC__;

        if (parent != NULL && parent->ob_type == &NamedArray_Type) {
            NamedArrayObject *p = (NamedArrayObject *) parent;
            if (p->rowNames != NULL) {
                Py_INCREF(p->rowNames);
                self->rowNames = p->rowNames;
            }

            if (p->colNames != NULL) {
                Py_INCREF(p->colNames);
                self->colNames = p->colNames;
            }
        }
        Py_RETURN_NONE;
    }


    void pyutil_init(PyObject *module) {
        std::cout << "Inside pyutil_init" << std::endl;
        // set up the base class to be the numpy ndarray PyArray_Type
        NamedArray_Type.tp_base = &PyArray_Type;


        // set up the pointers of the NamedArray_MappingMethods to point
        // to the numpy ones
        PyMappingMethods *numpyMappMethods = PyArray_Type.tp_as_mapping;

        assert(numpyMappMethods && "Numpy PyMappingMethods is NULL");

        NamedArray_MappingMethods = *numpyMappMethods;

        // set our getitem pointer
        NamedArray_MappingMethods.mp_subscript = (binaryfunc) NammedArray_subscript;

        // set our constructor
//        NamedArray_Type.tp_init = NamedArray___init__;

        int result;

        if ((result = PyType_Ready(&NamedArray_Type)) < 0) {
            PyObject *moduleName = PyModule_GetNameObject(module);
            if (moduleName == nullptr) {
                std::cerr << "Could not get ModuleName object" << std::endl;
                return;
            }
            Py_ssize_t size;
            const char *moduleNameCStr = PyUnicode_AsUTF8(moduleName);
            if (!moduleNameCStr) {
                std::cerr << "Could not convert PyUnicode to const char*" << std::endl;
                return;
            }
            std::cerr << "PyType_Ready(&NamedArray_Type)) Error. Failed to import module: " << moduleNameCStr << result;
            return;
        }

        Py_INCREF(&NamedArray_Type);

        if (PyModule_AddObject(module, "NamedArray", (PyObject *) (&NamedArray_Type)) < 0) {

            PyErr_SetString(PyExc_ValueError, "Could not add NamedArray object to _roadrunner module");
        };
    }

/*
/* PyArray_New(&PyArray_Type, nd, dims, NPY_DOUBLE, NULL, data, 0,
                                     NPY_CARRAY | NPY_OWNDATA, NULL);
 */
    PyObject *NamedArray_New(
            int nd, npy_intp *dims,
            double *data, int pyFlags,
            const ls::DoubleMatrix *mat) {
        bool named = Config::getValue(Config::PYTHON_ENABLE_NAMED_MATRIX);

        if (named) {
            rrLog(Logger::LOG_INFORMATION) << "creating NEW style array";

            //         (*(PyObject * (*)(PyTypeObject *, int, npy_intp const *, int, npy_intp const *, void *, int, int, PyObject *))
            NamedArrayObject *array = (NamedArrayObject *) PyArray_New(
                    &NamedArray_Type, nd, dims, NPY_DOUBLE, NULL, data, 0,
                    pyFlags, NULL);

            if (array == NULL) {
                const char *error = rrGetPyErrMessage();
                rrLog(Logger::LOG_CRITICAL) << error;
                rr_strfree(error);
                return NULL;
            }

            array->rowNames = stringvector_to_py(mat->getRowNames());
            array->colNames = stringvector_to_py(mat->getColNames());
            return (PyObject *) array;

        } else {
            rrLog(Logger::LOG_INFORMATION) << "creating old style array";
            return PyArray_New(&PyArray_Type, nd, dims, NPY_DOUBLE, NULL, data, 0,
                               pyFlags, NULL);
        }
    }


    PyObject *stringvector_to_py(const std::vector<std::string> &vec) {
        size_t size = vec.size();

        PyObject *pyList = PyList_New(size);

        unsigned j = 0;

        for (auto i = vec.begin(); i != vec.end(); ++i) {
            const std::string &str = *i;
            PyObject *pyStr = rrPyString_FromString(str.c_str());
            PyList_SET_ITEM(pyList, j++, pyStr);
        }
        return pyList;
    }

    PyObject *PyList_toPickle(PyObject *list) {
        static PyObject *module = NULL;
        PyObject *pickle;

        if (module == NULL && (module = PyImport_ImportModule("pickle")) == NULL)
            return NULL;

        pickle = PyObject_CallMethodObjArgs(module,
                                            PyUnicode_FromString("dumps"),
                                            list,
                                            NULL);
        if (!pickle) {
            std::cerr << "PyList_toPickle returned None";
            return Py_None;
        }
        return pickle;
    }

    std::vector<std::string> py_to_stringvector(PyObject *obj) {
        str_vector result;
        if (obj) {
            PyObject *seq = PySequence_Fast(obj, "expected a sequence");
            if (obj) {
                Py_ssize_t len = PySequence_Size(obj);
                if (PyList_Check(seq))
                    for (unsigned i = 0; i < len; i++) {
                        PyObject *item = PyList_GET_ITEM(seq, i);
                        // Return value: New reference.
                        PyObject *pystr = PyObject_Str(item);
                        result.push_back(rrPyString_AsString(pystr));
                        Py_XDECREF(pystr);
                    }
                else
                    for (unsigned i = 0; i < len; i++) {
                        PyObject *item = PyTuple_GET_ITEM(seq, i);
                        // Return value: New reference.
                        PyObject *pystr = PyObject_Str(item);
                        result.push_back(rrPyString_AsString(pystr));
                        Py_XDECREF(pystr);
                    }
                Py_XDECREF(seq);
            }
        }

        return result;
    }


    Dictionary *Dictionary_from_py(PyObject *py) {
        if (PyDict_Check(py) == 0) {
            throw invalid_argument("object is not a dictionary");
        }

        BasicDictionary *dict = new BasicDictionary();

        PyObject *pkey, *pvalue;
        Py_ssize_t pos = 0;

        while (PyDict_Next(py, &pos, &pkey, &pvalue)) {
# if PY_MAJOR_VERSION == 3
            if (PyUnicode_Check(pkey)) {
# else
                if (PyString_Check(pkey)) {
# endif
                std::string key(rrPyString_AsString(pkey));
                Setting value = Variant_from_py(pvalue);

                dict->setItem(key, value);

            } else {
                throw invalid_argument("keys must be strings");
            }
        }

        return dict;
    }


/*****************************************************************************************
 * Array Printing Stuff
 ****************************************************************************************/



    static int longestStrLen(const str_vector &s) {
        string::size_type longest = 0;
        for (str_vector::const_iterator i = s.begin(); i != s.end(); ++i) {
            longest = std::max(longest, i->length());
        }
        return static_cast<int>(longest);
    }

    static std::string array_format(PyArrayObject *arr,
                                    const str_vector &rowNames, const str_vector &colNames) {

        unsigned ndim = PyArray_NDIM(arr);
        npy_intp rows = ndim > 0 ? PyArray_DIM(arr, 0) : 0;
        npy_intp cols = ndim > 1 ? PyArray_DIM(arr, 1) : 0;

        assert(rows > 0 && cols > 0);

#define GET_VAL(_row, _col) *((double *) PyArray_GETPTR2(arr, _row, _col))

        // longest row name + " [["
        int rowNameWidth = longestStrLen(rowNames) + 1;

        // find widest element in each column
        vector<unsigned> columnWidths(cols);
        for (unsigned i = 0; i < rows; ++i) {
            for (unsigned j = 0; j < cols; ++j) {
                unsigned colw = columnWidths[j];
                colw = std::max(colw, j < colNames.size() ? (unsigned) colNames[j].length() + 1 : 0);
                stringstream ss;
                ss << GET_VAL(i, j);
                colw = std::max(colw, (unsigned) ss.str().length() + 1);
                columnWidths[j] = colw;
            }
        }

        stringstream ss;

        // first row, column names
        if (colNames.size()) {
            for (unsigned j = 0; j < cols; ++j) {
                ss << setw((j == 0 ? rowNameWidth + 2 : 0) + columnWidths[j]);
                ss << right;
                ss << (j < colNames.size() ? colNames[j] : "");
                if (j < cols - 1) {
                    ss << ",";
                }
            }
            ss << endl;
        }

        // main matrix bit
        for (unsigned i = 0; i < rows; ++i) {
            // row names
            ss << setw(rowNameWidth);
            ss << left;
            ss << (i < rowNames.size() ? rowNames[i] : "");
            ss << ((i > 0 && rows > 1) ? " [" : "[[");
            // columns
            for (unsigned j = 0; j < cols; ++j) {
                ss << setw(columnWidths[j]);
                ss << right;
                ss << GET_VAL(i, j);
                if (j < cols - 1) {
                    ss << ",";
                }
            }
            ss << ((i < rows - 1) ? "]," : "]]");
            ss << endl;
        }
        return ss.str();

#undef GET_VAL
    }

    PyObject *NamedArray_repr(NamedArrayObject *self) {
        PyArrayObject *array = (PyArrayObject *) self;
        str_vector rowNames = py_to_stringvector(self->rowNames);
        str_vector colNames = py_to_stringvector(self->colNames);

        unsigned ndim = PyArray_NDIM(array);
        npy_intp rows = ndim > 0 ? PyArray_DIM(array, 0) : 0;
        npy_intp cols = ndim > 1 ? PyArray_DIM(array, 1) : 0;

        if (rows == 0 || cols == 0) {
            return PyArray_Type.tp_str((PyObject *) self);
        }

        string str = array_format(array, rowNames, colNames);

        return rrPyString_FromString(str.c_str());
    }

    PyObject *NamedArray_str(NamedArrayObject *self) {
        return NamedArray_repr(self);
    }


    PyObject *convertStringVectorToPython(const std::vector<std::string> &vec) {
        long long size = (long long) vec.size();

        PyObject *pyList = PyList_New(size);

        unsigned j = 0;

        for (const auto &item : vec) {
            PyObject *pyStr = rrPyString_FromString(item.c_str());
            PyList_SET_ITEM(pyList, j, pyStr);
            j++;
        }

        return pyList;
    }

    std::vector<std::string> convertPythonListToStringVector(PyObject *pyList) {
        int size = (int) PyList_Size(pyList);
        std::vector<std::string> vec(size);
        for (Py_ssize_t i = 0; i < size; i++) {
            PyObject *item = PyList_GetItem(pyList, i);
            const char *s = PyUnicode_AsUTF8(item);
            vec[(int) i] = std::string(s);
        }

        return vec;
    }


    Matrix3DToNumpy::Matrix3DToNumpy(Matrix3DToNumpy::DoubleMatrix3D &matrix)
            : matrix_(matrix) {}

    PyObject *Matrix3DToNumpy::convertData() {
        // collect dimensions. x = width, y=height, z = depth.
        const npy_intp xMax = matrix_.numCols();
        const npy_intp yMax = matrix_.numRows();
        const npy_intp zMax = matrix_.numZ();

        // allocate 1D array with enough space to store linearized 3D matrix
        double *data = new double[yMax * xMax * zMax];

        // populate the 1D array with values from the Matrix3D<double, double> data values
        for (int z = 0; z < zMax; z++) {
            for (int y = 0; y < yMax; y++) {
                for (int x = 0; x < xMax; x++) {
                    // compute the linear index (so loop does idx \in 1, 2, ..., yMax*xMax*zMax)
                    unsigned int linearIdx = x + y * xMax + z * xMax * yMax;
                    data[linearIdx] = matrix_.slice(z, y, x);
                }
            }
        }
        // dimensions for resulting np array
        npy_intp dims[3] = {zMax, yMax, xMax};
        // create the numpy array
        PyObject *result = PyArray_SimpleNewFromData(3, dims, NPY_DOUBLE, (void *) data);

        // we need to cast to a PyArrayObject so that we can enable the NPY_ARRAY_OWNDATA flag
        //  which gives responsibility for handling the memory of `data` to numpy.
        PyArrayObject *arr = reinterpret_cast<PyArrayObject *>(result);
        PyArray_ENABLEFLAGS(arr, NPY_ARRAY_OWNDATA);
        int owned = PyArray_CHKFLAGS(arr, NPY_ARRAY_OWNDATA);
        if (!owned) {
            std::cerr << "PyArrayObject does not own its memory" << std::endl;
        }
        return result;
    }

    PyObject *Matrix3DToNumpy::convertIndex() {

        // matrix_.index dimensions == depth of Matrix3D
        const npy_intp zMax = matrix_.numZ();

        // allocate 1D array with enough space to store linearized 3D matrix
        double *data = new double[zMax];

        // populate the 1D array with values from the Matrix3D<double, double> data values
        auto &index = matrix_.getIndex();
        for (int z = 0; z < zMax; z++) {
            data[z] = index[z];
        }
        // dimensions for resulting np array
        npy_intp dims = zMax;

        // create the numpy array
        PyObject *result = PyArray_SimpleNewFromData(1, &dims, NPY_DOUBLE, (void *) data);

        // we need to cast to a PyArrayObject so that we can enable the NPY_ARRAY_OWNDATA flag
        //  which gives responsibility for handling the memory of `data` to numpy.
        PyArrayObject *arr = reinterpret_cast<PyArrayObject *>(result);
        PyArray_ENABLEFLAGS(reinterpret_cast< PyArrayObject *>(result), NPY_ARRAY_OWNDATA);
        int owned = PyArray_CHKFLAGS(reinterpret_cast< PyArrayObject *>(result), NPY_ARRAY_OWNDATA);
        if (!owned) {
            std::cerr << "PyArrayObject does not own its memory" << std::endl;
        }
        return result;
    }

    PyObject *Matrix3DToNumpy::convertRowNames() {
        return convertStringVectorToPython(matrix_.getRowNames());
    }

    PyObject *Matrix3DToNumpy::convertColNames() {
        return convertStringVectorToPython(matrix_.getColNames());
    }


} /* namespace rr */

