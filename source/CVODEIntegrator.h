#ifndef rrCvodeInterfaceH
#define rrCvodeInterfaceH

#include "Integrator.h"
#include "rrRoadRunnerOptions.h"

#include <string>
#include <vector>

/**
* CVode vector struct
*/
typedef struct _generic_N_Vector *N_Vector;

namespace rr
{
    using std::string;

    class ExecutableModel;
    class RoadRunner;

    /**
     * @author WBC, ETS, MTK
     * @brief A RoadRunner integrator based on CVODE; serves as RoadRunner's main integrator for ODEs
     * @details Provides a wrapper around CVODE from the SUNDIALS suite.
     * CVODE has two main solvers: an Adams-Moulton solver for non-stiff problems,
     * and a backward differentiation formula (BDF) solver for stiff problems.
     * See: https://computation.llnl.gov/casc/sundials/documentation/toms_sundials.pdf
     */
    class CVODEIntegrator : public Integrator
    {

    public:
        CVODEIntegrator(ExecutableModel* oModel);
        virtual ~CVODEIntegrator();

        void loadConfigSettings();

        /**
         * @author WBC
         * @brief Load an SBML settings file and apply the configuration options
         * @details
         */
        void loadSBMLSettings(const std::string& filename);

        /**
         * @author WBC
         * @brief Get the name for this integrator
         */
        std::string getIntegratorName() const;

        /**
         * @author WBC
         * @brief Get the description for this integrator
         */
        std::string getIntegratorDescription() const;

        /**
         * @author WBC
         * @brief Get the hint for this integrator
         */
        std::string getIntegratorHint() const;

        /**
         * @author WBC, ETS, MTK
         * @brief Always deterministic for CVODE
         */
        IntegrationMethod getIntegrationMethod() const;

        /**
         * @author WBC, ETS, MTK
         * @brief Sets the value of an integrator setting (e.g. absolute_tolerance)
         */
        void setValue(std::string setting, const Variant& value);

        /**
         * @author WBC, ETS, MTK
         * @brief Main integration routine
         */
        double integrate(double t0, double tf);

        /**
         * @author WBC, ETS, MTK
         * @brief Reset time to zero and reinitialize model
         * @details Applies events which occur before time zero.
         * Reinitializes CVODE and the executable model.
         */
        void restart(double timeStart);

        /**
         * @author WBC, ETS, MTK
         * @brief Sets tolerances based on config values
         * @details Sets absolute and relative tolerances to the minimum
         * of Config::CVODE_MIN_ABSOLUTE, Config::CVODE_MIN_RELATIVE and
         * their respective current values
         */
        void tweakTolerances();


        /**
         * @author WBC
         * @brief Gets the integrator listener
         */
        IntegratorListenerPtr getListener();

        /**
         * @author WBC
         * @brief Sets the integrator listener
         */
        void setListener(IntegratorListenerPtr);

    private:
        static const int mDefaultMaxNumSteps;
        static const int mDefaultMaxAdamsOrder;
        static const int mDefaultMaxBDFOrder;

        void* mCVODE_Memory;
        N_Vector mStateVector;
        ExecutableModel* mModel;

        IntegratorListenerPtr listener;
        double lastEventTime;
        bool variableStepPendingEvent;
        bool variableStepTimeEndEvent;
        double *variableStepPostEventState;
        std::vector<unsigned char> eventStatus;

        void testRootsAtInitialTime();
        bool haveVariables();
        void assignResultsToModel();
        void setCVODETolerances();
        void reInit(double t0);

        void updateCVODE();
        void applyPendingEvents(double timeEnd);
        void applyEvents(double timeEnd, std::vector<unsigned char> &previousEventStatus);
        double applyVariableStepPendingEvents();

        void createCVode();
        void freeCVode();
        bool stateVectorVariables;


        friend int cvodeDyDtFcn(double t, N_Vector cv_y, N_Vector cv_ydot, void *f_data);
        friend int cvodeRootFcn(double t, N_Vector y, double *gout, void *g_data);
    };
}

#endif
