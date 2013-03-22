#include "Test_1.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rrSupport.h"	     //Supportfunctions for event handling..
void loadSymbolTables(ModelData* md)
{
	md->variableTable[0] = "S1";
	md->variableTable[1] = "S2";
	md->variableTable[2] = "S3";
	md->boundaryTable[0] = "src";
	md->boundaryTable[1] = "waste";
	md->boundaryTable[2] = "Xo";
	md->boundaryTable[3] = "X1";
	md->globalParameterTable[0] = "kk1";
	md->globalParameterTable[1] = "k1";
	md->globalParameterTable[2] = "k_1";
	md->globalParameterTable[3] = "kk2";
	md->globalParameterTable[4] = "k2";
	md->globalParameterTable[5] = "k_2";
	md->globalParameterTable[6] = "kk3";
	md->globalParameterTable[7] = "k3";
	md->globalParameterTable[8] = "k_3";
	md->globalParameterTable[9] = "kk4";
	md->globalParameterTable[10] = "k4";
	md->globalParameterTable[11] = "k_4";
}

void resetEvents(ModelData* md)
{}

void setConcentration(ModelData* md, int index, double value)
{
	double volume = 0.0;
	md->y[index] = value;
	switch (index)
	{
		case 0:
			volume = md->c[0];
		break;
		case 1:
			volume = md->c[0];
		break;
		case 2:
			volume = md->c[0];
		break;
	}
	md->amounts[index] = md->y[index]*volume;
}

double getConcentration(ModelData* md, int index)
{
	return md->y[index];
}

void convertToAmounts(ModelData* md)
{
	md->amounts[0] = md->y[0]*md->c[0];
	md->amounts[1] = md->y[1]*md->c[0];
	md->amounts[2] = md->y[2]*md->c[0];
}

void convertToConcentrations(ModelData* md)
{
	md->y[0] = md->amounts[0] / md->c[0];
	md->y[1] = md->amounts[1] / md->c[0];
	md->y[2] = md->amounts[2] / md->c[0];
}

int getNumLocalParameters(ModelData* md, int reactionId)
{
	return md->localParameterDimensions[reactionId];
}

void initializeInitialConditions(ModelData* md)
{
	md->init_y[0] = (double)0.1;
	md->init_y[1] = (double)0.25;
	md->init_y[2] = (double)0.1;
}

void setInitialConditions(ModelData* md)
{
	md->y[0] =  md->init_y[0];
	md->amounts[0] = md->y[0]*md->c[0];

	md->y[1] =  md->init_y[1];
	md->amounts[1] = md->y[1]*md->c[0];

	md->y[2] =  md->init_y[2];
	md->amounts[2] = md->y[2]*md->c[0];
}

void setBoundaryConditions(ModelData* md)
{
	md->bc[0] = (double)0;
	md->bc[1] = (double)0;
	md->bc[2] = (double)1;
	md->bc[3] = (double)0;
}

void setCompartmentVolumes(ModelData* md)
{
	md->c[0] = (double)1;
	md->c[0] = 
	(double)1;
}

void setParameterValues(ModelData* md)
{
	md->gp[0] = (double)1;

	md->gp[1] = (double)0.1;

	md->gp[2] = (double)0.05;

	md->gp[3] = (double)1;

	md->gp[4] = (double)0.15;

	md->gp[5] = (double)0.067;

	md->gp[6] = (double)1;

	md->gp[7] = (double)0.4;

	md->gp[8] = (double)0.09;

	md->gp[9] = (double)1;

	md->gp[10] = (double)0.55;

	md->gp[11] = (double)0.01;
}

// Uses the equation: C = Sd - L0*Si
void computeConservedTotals(ModelData* md)
{}

// Compute values of dependent species 
// Uses the equation: Sd = C + L0*Si
void updateDependentSpeciesValues(ModelData* md, double* y)
{}

void computeRules(ModelData* md, double* y)
{

}

void InitializeRates(ModelData* md)
{}

void AssignRatesA(ModelData* md)
{
}

void InitializeRateRuleSymbols(ModelData* md) 
{}

void AssignRatesB(ModelData* md, double oRates[])
{}

double* GetCurrentValues(ModelData* md)
{
	double* dResult = (double*) malloc(sizeof(double)*0);
	return dResult;
}

//Uses the equation: dSd/dt = L0 dSi/dt
void computeAllRatesOfChange(ModelData* md)
{
	int i;

	double* dTemp = (double*) malloc( sizeof(double)* (md->amountsSize + md->rateRulesSize) );
	for(i = 0; i < md->amountsSize; i++)
	{
		dTemp[i + md->rateRulesSize] = md->amounts[i];
	}
	//amounts.CopyTo(dTemp, rateRules.Length); 
	__evalModel(md, md->time, dTemp);
}

// Compute the reaction rates
void computeReactionRates(ModelData* md, double time, double *y)
{
	md->rates[0]=md->gp[0]*(md->gp[1]*md->bc[2]-md->gp[2]*md->y[0]);

	md->rates[1]=md->gp[3]*(md->gp[4]*md->y[0]-md->gp[5]*md->y[1]);

	md->rates[2]=md->gp[6]*(md->gp[7]*md->y[1]-md->gp[8]*md->y[2]);

	md->rates[3]=md->gp[9]*(md->gp[10]*md->y[2]-md->gp[11]*md->bc[3]);
}

//Model Function
void __evalModel(ModelData* md, double timein, double* oAmounts)
{
	md->y[0] = oAmounts[0]/md->c[0];

	md->y[1] = oAmounts[1]/md->c[0];

	md->y[2] = oAmounts[2]/md->c[0];

	convertToAmounts(md);
	md->time = timein;  // Don't remove
	updateDependentSpeciesValues(md, md->y);
	computeReactionRates(md, md->time, md->y);
	md->dydt[0] = + md->rates[0] - md->rates[1];
	md->dydt[1] = + md->rates[1] - md->rates[2];
	md->dydt[2] = + md->rates[2] - md->rates[3];
	convertToAmounts(md);
}

//Event handling function
void evalEvents(ModelData* md, double timeIn, double *oAmounts)
{
	md->time = timeIn;
	updateDependentSpeciesValues(md, md->y);
	computeRules(md, md->y);
}

void InitializeDelays(ModelData* md)
{
}

void computeEventPriorities(ModelData* md)
{}

void evalInitialAssignments(ModelData* md)
{
}

void testConstraints(ModelData* md)
{}


//Function to initialize the model data structure. Returns an integer indicating result
int InitModelData(ModelData* md)
{
	return 0;
}



//Function to initialize the model data structure. Returns an integer indicating result
int InitModel(ModelData* md)
{
	setCompartmentVolumes(md);
	InitializeDelays(md);
	return 0;
}


