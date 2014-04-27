/*
 * TestVariant.cpp
 *
 *  Created on: Apr 26, 2014
 *      Author: andy
 */

#include <TestVariant.h>
#include <Variant.h>

#ifdef RR_BUILD_PYTHON
#include <Python.h>
#endif

#include <iostream>

using namespace std;

typedef struct {
    int a;
    int b;
} Foo;

namespace rr
{

int testPythonVariant(int argc, char* argv[])
{
#if 0
    PyObject *pName, *pModule, *pDict, *pFunc;
    PyObject *pArgs, *pValue;
    int i;


    Py_Initialize();
    pName = PyString_FromString("Test String");
    /* Error checking of pName left out */

    Variant v(pName);

    PyObject *obj = v.convert<PyObject*>();

    if (PyString_Check(obj)) {
        cout << "is string" << std::endl;

        cout << "value: " <<  PyString_AsString(obj) << endl;

        Py_XDECREF(obj);
    }


    Py_DECREF(pName);

    Foo f;

    Variant v2(f);

    PyClassObject *cls;

    Variant v3(cls);

    PyClassObject c;

    Variant v4(c);


    Py_Finalize();

#endif

    return 0;

}

} /* namespace rr */
