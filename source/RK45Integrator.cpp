/*
 * RK45Integrator.cpp
 *
 *  Created on: Jul 22, 2014
 *      Author: andy
 */

#include <RK45Integrator.h>
#include <rrExecutableModel.h>

#include <cassert>

extern "C" {
#include <clapack/f2c.h>
#include <clapack/clapack.h>
}

namespace rr
{

    RK45Integrator::RK45Integrator(ExecutableModel *m)
    {
        Log(Logger::LOG_NOTICE) << "Creating Runge-Kutta Fehlberg integrator";
        stateVectorSize = 0;
        k1 = k2 = k3 = k4 = y = ytmp = NULL;
        syncWithModel(m);
    }

    void RK45Integrator::syncWithModel(ExecutableModel* m)
    {
        // free existing memory
        delete []k1;
        delete []k2;
        delete []k3;
        delete []k4;
        delete []y;
        delete []ytmp;

        model = m;

        if (model) {
            stateVectorSize = model->getStateVector(NULL);
            k1 = new double[stateVectorSize];
            k2 = new double[stateVectorSize];
            k3 = new double[stateVectorSize];
            k4 = new double[stateVectorSize];
            y = new double[stateVectorSize];
            ytmp = new double[stateVectorSize];
        } else {
            stateVectorSize = 0;
            k1 = k2 = k3 = k4 = y = NULL;
        }

        resetSettings();
    }

    RK45Integrator::~RK45Integrator()
    {
        delete []k1;
        delete []k2;
        delete []k3;
        delete []k4;
        delete []y;
        delete []ytmp;
    }

    double RK45Integrator::integrate(double t, double h)
    {
        // NOTE: @kirichoi, please implement rk45 here
        static const double epsilon = 1e-12;
        double tf = 0;
        bool singleStep;

        assert(h > 0 && "h must be > 0");
        tf = t + h;
        singleStep = false;

        if (!model) {
            throw std::runtime_error("RK45Integrator::integrate: No model");
        }

        Log(Logger::LOG_DEBUG) <<
                "RK45Integrator::integrate(" << t << ", " << h << ")";

        // blas daxpy: y -> y + \alpha x
        integer n = stateVectorSize;
        integer inc = 1;
        double alpha = 0;

        model->setTime(t);

        model->getStateVector(y);

        // k1 = f(t_n, y_n)
        model->getStateVectorRate(t, y, k1);

        // k2 = f(t_n + h/2, y_n + (h/2) * k_1)
        alpha = h/2.;
        dcopy_(&n, y, &inc, ytmp, &inc);
        daxpy_(&n, &alpha, k1, &inc, ytmp, &inc);
        model->getStateVectorRate(t + alpha, ytmp, k2);

        // k3 = f(t_n + h/2, y_n + (h/2) * k_2)
        alpha = h/2.;
        dcopy_(&n, y, &inc, ytmp, &inc);
        daxpy_(&n, &alpha, k2, &inc, ytmp, &inc);
        model->getStateVectorRate(t + alpha, ytmp, k3);

        // k4 = f(t_n + h, y_n + (h) * k_3)
        alpha = h;
        dcopy_(&n, y, &inc, ytmp, &inc);
        daxpy_(&n, &alpha, k3, &inc, ytmp, &inc);
        model->getStateVectorRate(t + alpha, ytmp, k4);

        // k_1 = k_1 + 2 k_2
        alpha = 2.;
        daxpy_(&n, &alpha, k2, &inc, k1, &inc);

        // k_1 = (k_1 + 2 k_2) + 2 k_3
        alpha = 2.;
        daxpy_(&n, &alpha, k3, &inc, k1, &inc);

        // k_1 = (k_1 + 2 k_2 + 2 k_3) + k_4
        alpha = 1.;
        daxpy_(&n, &alpha, k4, &inc, k1, &inc);

        // y_{n+1} = (h/6)(k_1 + 2 k_2 + 2 k_3 + k_4);
        alpha = h/6.;

        daxpy_(&n, &alpha, k1, &inc, y, &inc);

        model->setTime(t + h);
        model->setStateVector(y);

        return t + h;
    }

    void RK45Integrator::testRootsAtInitialTime()
    {
        std::vector<unsigned char> initialEventStatus(model->getEventTriggers(0, 0, 0), false);
        model->getEventTriggers(initialEventStatus.size(), 0, initialEventStatus.size() == 0 ? NULL : &initialEventStatus[0]);
        applyEvents(0, initialEventStatus);
    }

    void RK45Integrator::applyEvents(double timeEnd, std::vector<unsigned char> &previousEventStatus)
    {
        model->applyEvents(timeEnd, previousEventStatus.size() == 0 ? NULL : &previousEventStatus[0], y, y);
    }

    void RK45Integrator::restart(double t0)
    {
        if (!model) {
            return;
        }

        if (t0 <= 0.0) {
            if (y)
            {
                model->getStateVector(y);
            }

            testRootsAtInitialTime();
        }

        model->setTime(t0);

        // copy state vector into memory
        if (y)
        {
            model->getStateVector(y);
        }
    }

    void RK45Integrator::setListener(IntegratorListenerPtr)
    {
    }

    IntegratorListenerPtr RK45Integrator::getListener()
    {
        return IntegratorListenerPtr();
    }

    std::string RK45Integrator::toString() const
    {
        return toRepr();
    }

    std::string RK45Integrator::toRepr() const
    {
        std::stringstream ss;
        ss << "< roadrunner.RK45Integrator() { 'this' : "
                << (void*)this << " }>";
        return ss.str();
    }

    std::string RK45Integrator::getName() const {
        return RK45Integrator::getRK45Name();
    }

    std::string RK45Integrator::getRK45Name() {
        return "rk45";
    }

    std::string RK45Integrator::getDescription() const {
        return RK45Integrator::getRK45Description();
    }

    std::string RK45Integrator::getRK45Description() {
        return "Kiri, please fill in";
    }

    std::string RK45Integrator::getHint() const {
        return RK45Integrator::getRK45Hint();
    }

    std::string RK45Integrator::getRK45Hint() {
        return "Internal RK45 ODE solver";
    }

    Variant RK45Integrator::getValue(std::string key)
    {
        if (key == "variable_step_size")
            return false;
        else
            return Integrator::getValue(key);
    }

    Integrator::IntegrationMethod RK45Integrator::getIntegrationMethod() const
    {
        return Integrator::Deterministic;
    }

    void RK45Integrator::resetSettings()
    {
        Solver::resetSettings();
    }

} /* namespace rr */
