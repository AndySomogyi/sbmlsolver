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
    double	                       	    mTime;
    int                            	    numIndependentVariables;
    int                            	    numDependentVariables;
    int                            	    numTotalVariables;
    int                            	    numBoundaryVariables;
    int                            	    numGlobalParameters;
    int                            	    numCompartments;
    int                            	    numReactions;
    int                            	    numRules;
    int                            	    numEvents;





	//These need allocation
	char*		                        modelName;
    bool*                          	    eventType; 				        //",                      mNumEvents                                , "Array holding the status whether events are useValuesFromTriggerTime or not");
    bool*                          	    eventPersistentType;	        //",            mNumEvents                                , "Array holding the status whether events are persitstent or not");
    int                                 ySize;
    double*                             y;

 	int                                 gpSize;
	double* 	                        gp; 				            //Vector containing all the global parameters in the System
	double* 	                        sr;					            //Vector containing all the modifiable species references  ");
    double*		                        lp;					            //Vector containing all the local parameters in the System  ");
	int									init_ySize;
    double*		                        init_y;				            //mFloatingSpeciesConcentrationList.Count(),   "Vector containing the initial concentrations of all floating species");

    int									amountsSize;
    double*	                            amounts; 			            //mFloatingSpeciesConcentrationList.size(),    "Vector containing the amounts of all floating species ");

    int									bcSize;
    double*	                            bc;					            //mNumBoundarySpecies,                        "Vector containing all the boundary species concentration values");

    double*	                            c;					            // mNumCompartments                         ,  "Vector containing all the compartment values   ");

	int                                 dydtSize;
    double*	                            dydt;				            //mFloatingSpeciesConcentrationList.size() ,   "Vector containing rates of changes of all species   ");
    int									ratesSize;
    double*	                            rates;				            //mNumReactions                             , "Vector containing the rate laws of all reactions    ");
    int									rateRulesSize;
    double*	                            rateRules;			            //mNumEvents                                 ,"Vector containing results of any event tests        ");
	int									ctSize;
    double*	                            ct;					            //mNumDependentSpecies                     ,  "Vector containing values of all conserved sums      ");
    double*	                            eventTests;		            //mNumEvents                                 ,"Vector containing results of any event tests        ");
	double*	                     	    eventPriorities;				//", mNumEvents);
    char**                              variableTable;                 	//",                mFloatingSpeciesConcentrationList.size());
    char**                              boundaryTable;                 	//",                mBoundarySpeciesList.size());
    char**                              globalParameterTable;			//",         mGlobalParameterList.size());
    int*                           	    localParameterDimensions;		//",     mNumReactions );
    bool*                               eventStatusArray;				//",             mNumEvents);
    bool*                               previousEventStatusArray;		//",     mNumEvents);
//	TEventDelayDelegate*			    mEventDelay;			        //	",                     mNumEvents                                 ,"Array of trigger function pointers");
    TEventDelayDelegate*                eventDelays;
	TEventAssignmentDelegate*       	eventAssignments;
    TComputeEventAssignmentDelegate* 	computeEventAssignments;
    TPerformEventAssignmentDelegate*    performEventAssignments;

} ModelData;

#endif
