#ifndef PyIntegratorListener_H_
#define PyIntegratorListener_H_

#include "Integrator.h"
#include "rrLogger.h"

namespace rr
{
class PyIntegratorListener : public IntegratorListener
{
public:

    PyIntegratorListener() : pyOnTimeStep(0), pyOnEvent(0)
    {
        Log(Logger::LOG_NOTICE) << __FUNC__;
    }

    virtual ~PyIntegratorListener()
    {
        Log(Logger::LOG_NOTICE) << __FUNC__;

        PyGILState_STATE gstate = PyGILState_Ensure();

        Py_XDECREF(pyOnTimeStep);
        Py_XDECREF(pyOnEvent);

        PyGILState_Release(gstate);
    }

    void setOnTimeStep(PyObject *py)
    {
        if (!PyCallable_Check(py)) {
            throw std::invalid_argument("argument must be callable");
        }

        Py_XINCREF(py);           /* Add a reference to new callback */
        Py_XDECREF(pyOnTimeStep);  /* Dispose of previous callback */
        pyOnTimeStep = py;         /* Remember new callback */
    }

    PyObject* getOnTimeStep()
    {
        return pyOnTimeStep;
    }

    void setOnEvent(PyObject* py)
    {
        if (!PyCallable_Check(py)) {
            throw std::invalid_argument("argument must be callable");
        }

        Py_XINCREF(py);              /* Add a reference to new callback */
        Py_XDECREF(pyOnEvent);  /* Dispose of previous callback */
        pyOnEvent = py;           /* Remember new callback */

    }

    PyObject *getOnEvent()
    {
        return pyOnEvent;
    }


private:
    PyObject *pyOnTimeStep;
    PyObject *pyOnEvent;

    /**
     * is called after the internal integrator completes each internal time step.
     */
    virtual uint onTimeStep(Integrator* integrator, ExecutableModel* model, double time)
    {
        uint result = 0;
        std::string err;

        Log(Logger::LOG_INFORMATION) << __FUNC__ << "integrator: " << (void*)integrator << ", model: " << (void*)model << ", time: " << time;

        if (pyOnTimeStep) {

            PyGILState_STATE gstate = PyGILState_Ensure();

            PyObject *pyIntegrator = Integrator_NewPythonObj(integrator);
            PyObject *pyModel = ExecutableModel_NewPythonObj(model);

            /* format strings:
               N (object) [PyObject *] - Same as O, except it doesn’t increment the reference count
               on the object. Useful when the object is created by a call to an object constructor
               in the argument list.
            */

            PyObject *args = Py_BuildValue("(N, N, d)", pyIntegrator, pyModel, time);
            PyObject *pyres = PyEval_CallObject(pyOnTimeStep, args);

            if (PyErr_Occurred()) {

                PyObject* pystr = PyObject_Str(PyErr_Occurred());
                err = std::string("Python error occured in onTrigger: ") + PyString_AsString(pystr);

                Log(Logger::LOG_ERROR) << err;

                Py_XDECREF(pystr);
                PyErr_Clear();
            }
            else if (PyInt_Check(pyres)) {
                result = PyInt_AsLong(pyres);
            }
            else if (pyres != Py_None) {
                PyObject* pystr = PyObject_Str(pyres);
                const char* cstr = PyString_AsString(pystr);

                Log(Logger::LOG_WARNING) << "The Python onTrigger handler returned " << cstr;
                Log(Logger::LOG_WARNING) << "No result is required";

                Py_XDECREF(pystr);
            }

            Py_XDECREF(pyres);
            // frees both new objects
            Py_XDECREF(args);

            // Release the thread. No Python API allowed beyond this point.
            PyGILState_Release(gstate);

            if (!err.empty())
            {
                throw std::runtime_error(err);
            }
        }

        return result;
    }



    /**
     * whenever model event occurs and after it is procesed.
     */
    virtual uint onEvent(Integrator* integrator, ExecutableModel* model, double time)
    {
        uint result = 0;
        std::string err;

        Log(Logger::LOG_INFORMATION) << __FUNC__ << "integrator: " << (void*)integrator << ", model: " << (void*)model << ", time: " << time;

        if (pyOnEvent) {
            PyGILState_STATE gstate = PyGILState_Ensure();

            PyObject *pyIntegrator = Integrator_NewPythonObj(integrator);
            PyObject *pyModel = ExecutableModel_NewPythonObj(model);
            PyObject *args = Py_BuildValue("(N, N, s)", pyIntegrator, pyModel, time);
            PyObject *pyres = PyEval_CallObject(pyOnEvent, args);

            if (PyErr_Occurred()) {

                PyObject* pystr = PyObject_Str(PyErr_Occurred());
                err = std::string("Python error occured in onAssignment: ") + PyString_AsString(pystr);

                Log(Logger::LOG_ERROR) << err;

                Py_XDECREF(pystr);
                PyErr_Clear();
            }
            else if (pyres != Py_None) {
                PyObject* pystr = PyObject_Str(pyres);
                const char* cstr = PyString_AsString(pystr);

                Log(Logger::LOG_WARNING) << "The Python onAssignment handler returned " << cstr;
                Log(Logger::LOG_WARNING) << "No result is required";

                Py_XDECREF(pystr);
            }


            Py_XDECREF(pyres);
            Py_XDECREF(args);

            // Release the thread. No Python API allowed beyond this point.
            PyGILState_Release(gstate);

            if (!err.empty())
            {
                throw std::runtime_error(err);
            }
        }

        return result;
    }
};

/**
 * listeners are shared objects, so use std smart pointers
 * to manage them.
 */
typedef std::tr1::shared_ptr<rr::PyIntegratorListener> PyIntegratorListenerPtr;


}

#endif // PyEventHandler_H_
