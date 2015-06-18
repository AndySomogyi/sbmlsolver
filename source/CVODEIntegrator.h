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
	* @internal
	* The integrator implemented by CVODE.
	*/
	class CVODEIntegrator : public Integrator
	{

	public:
		CVODEIntegrator(ExecutableModel* oModel);
		virtual ~CVODEIntegrator();
		
		void loadConfigSettings();
		void loadSBMLSettings(const std::string& filename);
		std::string getIntegratorName() const;
		std::string getIntegratorDescription() const;
		std::string getIntegratorHint() const;
		IntegrationMethod getIntegrationMethod() const;
		void setValue(std::string setting, const Variant& value);
		
		double integrate(double t0, double tf);
		void restart(double timeStart);
		void tweakTolerances();
		IntegratorListenerPtr getListener();
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
