#ifndef PyEventHandler_H_
#define PyEventHandler_H_

#include "rrExecutableModel.h"
#include "rrLogger.h"

namespace rr
{
class PyEventHandler : public EventHandler
{
public:
    
    PyEventHandler() : pyOnTrigger(0), pyOnAssignment(0)
    {
        Log(Logger::LOG_INFORMATION) << __FUNC__;
    }

    virtual ~PyEventHandler()
    {
        Log(Logger::LOG_NOTICE) << __FUNC__;

        PyGILState_STATE gstate = PyGILState_Ensure();

        Py_XDECREF(pyOnTrigger);
        Py_XDECREF(pyOnAssignment);

        PyGILState_Release(gstate);
    }

    void setOnTrigger(PyObject *py)
    {
        if (!PyCallable_Check(py)) {
            throw std::invalid_argument("argument must be callable");
        }

        Py_XINCREF(py);           /* Add a reference to new callback */
        Py_XDECREF(pyOnTrigger);  /* Dispose of previous callback */
        pyOnTrigger = py;         /* Remember new callback */
    }

    PyObject* getOnTrigger()
    {
        return pyOnTrigger;
    }

    void setOnAssignment(PyObject* py)
    {
        if (!PyCallable_Check(py)) {
            throw std::invalid_argument("argument must be callable");
        }
        
        Py_XINCREF(py);              /* Add a reference to new callback */
        Py_XDECREF(pyOnAssignment);  /* Dispose of previous callback */
        pyOnAssignment = py;           /* Remember new callback */

    }

    PyObject *getOnAssignment()
    {
        return pyOnAssignment;
    }

    void fireOnTrigger(ExecutableModel* model, int index, const std::string& eventId)
    {
        Log(Logger::LOG_INFORMATION) << __FUNC__;
        onTrigger(model, index, eventId);
    }

    void fireOnAssignment(ExecutableModel* model, int index, const std::string& eventId)
    {
        Log(Logger::LOG_INFORMATION) << __FUNC__;
        onAssignment(model, index, eventId);
    }

private:

    PyObject *pyOnTrigger;
    PyObject *pyOnAssignment;

    virtual void onTrigger(ExecutableModel* model, int index, const std::string& eventId)
    {
        Log(Logger::LOG_INFORMATION) << __FUNC__ << "model: " << (void*)model << ", event id: " << eventId << ", index: " << index;

        if (pyOnTrigger) {

            PyGILState_STATE gstate = PyGILState_Ensure();

            PyObject *pyModel = ExecutableModel_NewPythonObj(model);

            /* format strings:
               N (object) [PyObject *] - Same as O, except it doesn’t increment the reference count 
               on the object. Useful when the object is created by a call to an object constructor 
               in the argument list.
            */

            PyObject *args = Py_BuildValue("(N, i, s)", pyModel, index, eventId.c_str());
            PyObject *result = PyEval_CallObject(pyOnTrigger, args);

            
            Py_XDECREF(result);
            Py_XDECREF(args);

            // Release the thread. No Python API allowed beyond this point.
            PyGILState_Release(gstate);

        }
    }

    virtual void onAssignment(ExecutableModel* model, int index, const std::string& eventId)
    {
        Log(Logger::LOG_INFORMATION) << __FUNC__ << "model: " << (void*)model << ", event id: " << eventId << ", index: " << index;

        if (pyOnAssignment) {
            PyGILState_STATE gstate = PyGILState_Ensure();

            PyObject *pyModel = ExecutableModel_NewPythonObj(model);
            PyObject *args = Py_BuildValue("(N, i, s)", pyModel, index, eventId.c_str());
            PyObject *result = PyEval_CallObject(pyOnAssignment, args);

            Py_XDECREF(result);
            Py_XDECREF(args);

            // Release the thread. No Python API allowed beyond this point.
            PyGILState_Release(gstate);
        }
    }
};


}

#endif // PyEventHandler_H_
