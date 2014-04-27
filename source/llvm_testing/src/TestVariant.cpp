/*
 * TestVariant.cpp
 *
 *  Created on: Apr 26, 2014
 *      Author: andy
 */

#include <TestVariant.h>
#include <Variant.h>
#include <rrConfig.h>

#include "rrLogger.h"

#include "rrRoadRunner.h"

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


int TestVariant::testPythonVariant(int argc, char* argv[])
{
    return 0;
}

void TestVariant::test(int argc, char* argv[])
{
    Logger::setLevel(Logger::LOG_TRACE);
    rr::Config::getString(rr::Config::LOADSBMLOPTIONS_CONSERVED_MOIETIES);

    Config::writeConfigFile("/Users/andy/test.conf");

    Config::readConfigFile("/Users/andy/test.conf");

    RoadRunner r("/Users/andy/ex.xml");

    r.simulate();


}

void TestVariant::testConfig(int argc, char* argv[])
{
}

}
