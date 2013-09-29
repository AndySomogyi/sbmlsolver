#ifndef rrNLEQInterfaceH
#define rrNLEQInterfaceH
#include <vector>
#include "rrExporter.h"
#include "rrExecutableModel.h"
#include "rrSteadyStateSolver.h"
#include "Configurable.h"
using std::vector;

namespace rr
{

/**
 * @internal
 */
class RR_DECLSPEC NLEQInterface : public SteadyStateSolver,
    public Configurable
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
    void                            setup();

public:
    /// <summary>
    /// Creates a new Instance of NLEQ for the given Model
    /// </summary>
    /// <param name="model">the model to create NLEQ for</param>
    NLEQInterface(ExecutableModel *_model = NULL);
    ~NLEQInterface();

    bool                            isAvailable();

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

    /**
     * creates a new xml element that represent the current state of this
     * Configurable object and all if its child objects.
     */
    virtual _xmlNode *createConfigNode();

    /**
     * Given an xml element, the Configurable object should pick its needed
     * values that are stored in the element and use them to set its
     * internal configuration state.
     */
    virtual void loadConfig(const _xmlDoc* doc);

};
}

#endif
