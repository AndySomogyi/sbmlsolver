#ifndef modelH
#define modelH
#include <stdio.h>
#include <stdbool.h>
#include "rrModelData.h"	             //Contains the structure defining model data passed to the shared library.
#include "rrCExporter.h"	             //Export Stuff.
//************************************************************************** 
//Number of floating species: 3
	// y[0] = S1
	// y[1] = S2
	// y[2] = S3
//************************************************************************** 



//NON - EXPORTS ========================================
    void                                         InitializeDelays(ModelData* md);                       


//EXPORTS ========================================
D_S int                                          InitModelData(ModelData* md);                          
D_S int                                          InitModel(ModelData* md);                              
D_S char*                                        getModelName(ModelData* md);                           
D_S void                                         resetEvents(ModelData* md);                            
D_S void                                         setConcentration(ModelData* md, int index, double value);
D_S double                                       getConcentration(ModelData* md,int index);             
D_S void                                         convertToAmounts(ModelData* md);                       
D_S void                                         convertToConcentrations(ModelData* md);                
D_S int                                          getNumLocalParameters(ModelData* md, int reactionId);  
D_S void                                         initializeInitialConditions(ModelData* md);            
D_S void                                         setInitialConditions(ModelData* md);                   
D_S void                                         setBoundaryConditions(ModelData* md);                  
D_S void                                         setCompartmentVolumes(ModelData* md);                  
D_S void                                         setParameterValues(ModelData* md);                     
D_S void                                         computeConservedTotals(ModelData* md);                 
D_S void                                         updateDependentSpeciesValues(ModelData* md, double* y);
D_S void                                         computeRules(ModelData* md, double* y);                
D_S void                                         InitializeRates(ModelData* md);                        
D_S void                                         AssignRatesA(ModelData* md);                           
D_S void                                         InitializeRateRuleSymbols(ModelData* md);              
D_S void                                         AssignRatesB(ModelData* md, double oRates[]);          
D_S double*                                      GetCurrentValues(ModelData* md);                       
D_S void                                         computeAllRatesOfChange(ModelData* md);                
D_S void                                         computeReactionRates(ModelData* md, double time, double *y);
D_S void                                         __evalModel(ModelData* md, double, double*);           
D_S void                                         evalEvents(ModelData* md, double timeIn, double *oAmounts);
D_S void                                         computeEventPriorities(ModelData* md);                 
D_S void                                         evalInitialAssignments(ModelData* md);                 
D_S void                                         testConstraints(ModelData* md);                        


#endif //modelH
