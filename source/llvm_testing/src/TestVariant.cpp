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
        std::cout << "is std::string" << std::endl;

        std::cout << "value: " <<  PyString_AsString(obj) << std::endl;

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

template <typename T>
void tryConv(Variant& v, T t) {
    const type_info& info = typeid(T);

    try {
        T res = v.convert<T>();
        std::cout << "converted " << v.toString() << " to " << info.name() << " OK" << std::endl;
    } catch (std::exception& e) {
        std::cout << "could not convert " << v.toString() << " to " << info.name() << ", what: " << e.what() << std::endl;
    }

}

void TestVariant::test(int argc, char* argv[])
{
    if (argc < 2) {
        return;
    }

    Variant v = Variant::parse(argv[1]);

    std::cout << "converted \'" << argv[1] << "\' to \'" << v.toString() << "\', with type " << v.typeInfo().name() << std::endl;

    Config::writeConfigFile("/Users/andy/temp.conf");
}

void TestVariant::testConfig(int argc, char* argv[])
{
    Logger::setLevel(Logger::LOG_TRACE);
    rr::Config::getString(rr::Config::LOADSBMLOPTIONS_CONSERVED_MOIETIES);

    Config::writeConfigFile("/Users/andy/test.conf");

    Config::readConfigFile("/Users/andy/test.conf");

    RoadRunner r("/Users/andy/ex.xml");

    r.simulate();
}

}
