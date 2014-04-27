/*
 * PyUtils.cpp
 *
 *  Created on: Apr 27, 2014
 *      Author: andy
 */

#include <PyUtils.h>
#include <stdexcept>
#include <string>

using namespace std;

namespace rr
{


PyObject* Variant_to_py(const Variant& var)
{
    PyObject *result = 0;


    if (var.isString())
    {
        return PyString_FromString(var.convert<string>().c_str());
    }

    else if (var.isInteger())
    {
        return PyInt_FromLong(var.convert<long>());
    }

    else if (var.isNumeric())
    {
        return PyFloat_FromDouble(var.convert<double>());
    }

    else if (var.type() == typeid(bool))
    {
        return PyBool_FromLong(var.convert<bool>());
    }

    throw invalid_argument("could not convert " + var.toString() + "to Python object");
}

Variant Variant_from_py(PyObject* py)
{
    Variant var;

    if (PyString_Check(py))
    {
        var = std::string(PyString_AsString(py));
        return var;
    }

    else if (PyInt_Check(py))
    {
        var = PyInt_AsLong(py);
        return var;
    }

    else if (PyBool_Check(py))
    {
        var = (bool)(py == Py_True);
        return var;
    }

    else if (PyLong_Check(py))
    {
        var = PyLong_AsLong(py);
        return var;
    }

    else if (PyFloat_Check(py))
    {
        var = PyFloat_AsDouble(py);
        return var;
    }

    string msg = "could not convert Python type to built in type";
    throw invalid_argument(msg);
}

} /* namespace rr */

