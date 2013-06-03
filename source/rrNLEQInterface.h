#ifndef rrNLEQInterfaceH
#define rrNLEQInterfaceH
#include <vector>
#include "rrObject.h"
#include "rrExecutableModel.h"
#include "rrSteadyStateSolver.h"
#include "rrParameter.h"
using std::vector;

namespace rr
{

class RR_DECLSPEC NLEQInterface : public SteadyStateSolver
{
protected:
    int                             nOpts;
    long                           *IWK;
    long                            LIWK;
    long                            LWRK;
    double                         *RWK;
    double                         *XScal;
    long                            ierr;
    long                           *iopt;
    ExecutableModel         *model;     // Model generated from the SBML. Static so we can access it from standalone function
    long                     n;
    void							setup();

public:
    /// <summary>
    /// Creates a new Instance of NLEQ for the given Model
    /// </summary>
    /// <param name="model">the model to create NLEQ for</param>
    NLEQInterface(ExecutableModel *_model = NULL);
    ~NLEQInterface();
    Capability&						getCapability();
    bool                            isAvailable();
    //static ExecutableModel*         getModel();
    //static long						getN();
    int                             defaultMaxInterations;
    int                             maxIterations;
    double                          defaultTolerance;
    double                          relativeTolerance;


    /// <summary>
    /// Sets the Scaling Factors
    /// </summary>
    /// <param name="sx">Array of Scaling factors</param>
    void                            setScalingFactors(const vector<double>& sx);

    /// <summary>
    /// Returns the Number of Newton Iterations
    /// </summary>
    /// <returns>the Number of Newton Iterations</returns>
    int                             getNumberOfNewtonIterations();

    /// <summary>
    /// Returns the Number of Corrector steps
    /// </summary>
    /// <returns>Returns the Number of Corrector steps</returns>
    int                             getNumberOfCorrectorSteps();

    /// <summary>
    /// Returns the Number of Model Evaluations
    /// </summary>
    /// <returns>the Number of Model Evaluations</returns>
    int                             getNumberOfModelEvaluations();

    /// <summary>
    /// Returns the Number Of Jacobian Evaluations
    /// </summary>
    /// <returns>the Number Of Jacobian Evaluations</returns>
    int                             getNumberOfJacobianEvaluations();

    /// <summary>
    /// Returns the Number of Model Evaluations For Jacobian
    /// </summary>
    /// <returns>the Number of Model Evaluations For Jacobian</returns>
    int                             getNumberOfModelEvaluationsForJacobian();

    /// <summary>
    /// Thea actual solver routine making the call to NLEQ1
    /// </summary>
    /// <param name="yin">Array of Model variables</param>
    /// <returns>sums of squares </returns>
    double                          solve(const vector<double>& yin);
    double                          computeSumsOfSquares();

private:
    Parameter<int> maxIterationsParam;
    Parameter<double> relativeToleranceParam;
};
}

#endif
