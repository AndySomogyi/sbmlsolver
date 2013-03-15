#ifndef rrModelDataH
#define rrModelDataH


typedef double 	(*TEventDelayDelegate)();
typedef double* (*TComputeEventAssignmentDelegate)();
typedef void 	(*TPerformEventAssignmentDelegate)(double* );

#if defined(WIN32)
    typedef void __cdecl (*TEventAssignmentDelegate)();
#else
    typedef void (*TEventAssignmentDelegate)();
#endif

//Data that is used in SBML models
typedef struct SModelData
{
    double	                       	    time;
    int                            	    numIndependentVariables;
    int                            	    numDependentVariables;
    int                            	    numTotalVariables;
    int                            	    numBoundaryVariables;
    int                            	    numGlobalParameters;
    int                            	    numCompartments;
    int                            	    numReactions;
    int                            	    numRules;
    int                            	    numEvents;

	//These need allocation...
	char*		                        modelName;

	int									eventTypeSize;
    bool*                          	    eventType;

	int									eventPersistentTypeSize;
    bool*                          	    eventPersistentType;

    int                                 ySize;
    double*                             y;

 	int                                 gpSize;
	double* 	                        gp;

	int		 	                        srSize;
	double* 	                        sr;

    int									lpSize;
    double*		                        lp;

	int									init_ySize;
    double*		                        init_y;

    int									amountsSize;
    double*	                            amounts;

    int									bcSize;
    double*	                            bc;

    int		                            cSize;
    double*	                            c;

	int                                 dydtSize;
    double*	                            dydt;

    int									ratesSize;
    double*	                            rates;

    int									rateRulesSize;
    double*	                            rateRules;

	int									ctSize;
    double*	                            ct;

	int									eventTestsSize;
    double*	                            eventTests;

	int									eventPrioritiesSize;
	double*	                     	    eventPriorities;

    char**                              variableTable;
    char**                              boundaryTable;
    char**                              globalParameterTable;

    int*                           	    localParameterDimensions;

    int		                            eventStatusArraySize;
    bool*                               eventStatusArray;

	int									previousEventStatusArraySize;
    bool*                               previousEventStatusArray;

//	TEventDelayDelegate*			    mEventDelay;
    TEventDelayDelegate*                eventDelays;
	TEventAssignmentDelegate*       	eventAssignments;

    TComputeEventAssignmentDelegate* 	computeEventAssignments;
    TPerformEventAssignmentDelegate*    performEventAssignments;

} ModelData;

#endif
