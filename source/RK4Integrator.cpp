/*
 * RK4Integrator.cpp
 *
 *  Created on: Jul 22, 2014
 *      Author: andy
 */

#include <RK4Integrator.h>
#include <rrExecutableModel.h>

#include <cassert>

extern "C" {
#include <clapack/f2c.h>
#include <clapack/clapack.h>
}

namespace rr
{

    RK4Integrator::RK4Integrator(ExecutableModel *m)
    {
        Log(Logger::LOG_NOTICE) << "creating runge-kutta integrator";
        resetSettings();
        stateVectorSize = 0;
        k1 = k2 = k3 = k4 = y = ytmp = NULL;
        syncWithModel(m);
    }

    void RK4Integrator::syncWithModel(ExecutableModel* m)
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

    RK4Integrator::~RK4Integrator()
    {
        delete []k1;
        delete []k2;
        delete []k3;
        delete []k4;
        delete []y;
        delete []ytmp;
    }

    double RK4Integrator::integrate(double t, double h)
    {
        static const double epsilon = 1e-12;
        double tf = 0;
        bool singleStep;

        assert(h > 0 && "h must be > 0");
        tf = t + h;
        singleStep = false;

        if (!model) {
            throw std::runtime_error("RK4Integrator::integrate: No model");
        }

        Log(Logger::LOG_DEBUG) <<
                "RK4Integrator::integrate(" << t << ", " << h << ")";

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

    void RK4Integrator::testRootsAtInitialTime()
    {
        std::vector<unsigned char> initialEventStatus(model->getEventTriggers(0, 0, 0), false);
        model->getEventTriggers(initialEventStatus.size(), 0, initialEventStatus.size() == 0 ? NULL : &initialEventStatus[0]);
        applyEvents(0, initialEventStatus);
    }

    void RK4Integrator::applyEvents(double timeEnd, std::vector<unsigned char> &previousEventStatus)
    {
        model->applyEvents(timeEnd, previousEventStatus.size() == 0 ? NULL : &previousEventStatus[0], y, y);
    }

    void RK4Integrator::restart(double t0)
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

    void RK4Integrator::setListener(IntegratorListenerPtr)
    {
    }

    IntegratorListenerPtr RK4Integrator::getListener()
    {
        return IntegratorListenerPtr();
    }

    //std::string RK4Integrator::toString() const
    //{
    //    return toRepr();
    //}

    //std::string RK4Integrator::toRepr() const
    //{
    //    std::stringstream ss;
    //    ss << "< roadrunner.RK4Integrator() { 'this' : "
    //            << (void*)this << " }>";
    //    return ss.str();
    //}

    std::string RK4Integrator::getName() const {
        return RK4Integrator::getRK4Name();
    }

    std::string RK4Integrator::getRK4Name() {
        return "rk4";
    }

    std::string RK4Integrator::getDescription() const {
        return RK4Integrator::getRK4Description();
    }

    std::string RK4Integrator::getRK4Description() {
        return "Runge-Kutta methods are a family of algorithms for solving "
            "ODEs. They have considerably better accuracy than the Euler "
            "method. This integrator is a standard 4th order Runge-Kutta "
            "solver.";
    }

    std::string RK4Integrator::getHint() const {
        return RK4Integrator::getRK4Hint();
    }

    std::string RK4Integrator::getRK4Hint() {
        return "Internal RK4 ODE solver";
    }

    Variant RK4Integrator::getValue(std::string key)
    {
        if (key == "variable_step_size")
            return false;
        else
            return Integrator::getValue(key);
    }

    Integrator::IntegrationMethod RK4Integrator::getIntegrationMethod() const
    {
        return Integrator::Deterministic;
    }

    void RK4Integrator::resetSettings()
    {
        Solver::resetSettings();
    }

//     void RK4Integrator::setItem(const std::string& key,
//             const rr::Variant& value)
//     {
//         throw std::invalid_argument("invalid key");
//     }
//
//     Variant RK4Integrator::getItem(const std::string& key) const
//     {
//         throw std::invalid_argument("invalid key");
//     }
//
//     bool RK4Integrator::hasKey(const std::string& key) const
//     {
//         return false;
//     }
//
//     int RK4Integrator::deleteItem(const std::string& key)
//     {
//         return -1;
//     }
//
//     std::vector<std::string> RK4Integrator::getKeys() const
//     {
//         return std::vector<std::string>();
//     }

} /* namespace rr */
