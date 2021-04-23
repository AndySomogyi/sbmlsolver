#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include "rrc_api.h"
#include "rrc_support.h"
//---------------------------------------------------------------------------


using namespace rrc;;

void printMatrix(char* msg1, RRMatrixHandle mat);
int main(int argc, char* argv[])
{
	enableLogging();
    setLogLevel("Debug3");

	std::string modelsPath(".\\..\\Models");
	if(argc > 1)
	{
		modelsPath = argv[1];
	}

	RRHandle rrHandle = NULL;
    rrHandle =  getRRInstance();

    if(!rrHandle)
    {
        std::cout<<"No handle...";
    }

	std::cout<<"wrappers Version: "<<getVersion()<<std::endl;
	std::cout<<"libSBML Version: "<<getlibSBMLVersion()<<std::endl;
    setTempFolder("c:\\rrTemp");
    enableLogging();

	text = getLogFileName();
    if(text)
	{
		std::cout<<"Log File Name: "<<text<<std::endl;
		freeText(text);
	}

	text = getBuildDate();

	if(text)
	{
		std::cout<<"Build date: "<<text<<std::endl;
		freeText(text);
	}

//	   std::string fileName = modelsPath + "\\ss_TurnOnConservationAnalysis.xml";
//	   std::string fileName = modelsPath + "\\ss_SimpleConservedCycle.xml";
	 std::string fileName = modelsPath + "\\ss_threeSpecies.xml";
//	 std::string fileName = modelsPath + "\\selectionListBug.xml";
//	 std::string fileName = modelsPath + "\\boundary.xml";

	std::ifstream ifs(fileName.c_str());
	if(!ifs)
	{
		cerr<<"Failed opening file: "<<fileName;
		return false;
	}
	std::cout << "\nRunning model: " << fileName << std::endl;
	setComputeAndAssignConservationLaws(false);
	std::string sbml((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

	if(!loadSBML(sbml.c_str()))
	{
		cerr<<"Failed loading SBML from file:"<<fileName<<std::endl;
		cerr<<"Last error was: "<<getLastError()<<std::endl;
		return -1;
	}

	RRListHandle sList = getAvailableTimeCourseSymbols();
    std::cout<<"Symbols: "<<listToString(sList);

    sList = getAvailableSteadyStateSymbols();
    std::cout<<"\n\n Steady state symbols\n";
    std::cout<<listToString(sList);
    freeRRList(sList);

    char* cFileName = getCSourceFileName();
    if(cFileName)
    {
    	std::cout<<"\n C File Name: "<<cFileName<<"\n";
    }

    freeText(cFileName);

    std::cout<<"Number of rules: "<<getNumberOfRules()<<"\n";
	int r = getNumberOfReactions();
	int m = getNumberOfFloatingSpecies();
	int b = getNumberOfBoundarySpecies();
	int p = getNumberOfGlobalParameters();
	int c = getNumberOfCompartments();

	printf ("Number of reactions = %d\n", r);
	printf ("Number of floating species = %d\n", m);
	printf ("Number of boundary species = %d\n\n", b);
	printf ("Number of compartments = %d\n\n", c);

	if (m > 0) {
	   printf ("Compartment names:\n");
	   printf ("------------------\n");
	   std::cout<<stringArrayToString(getCompartmentIds())<<std::endl<<std::endl;
	}

	if (m > 0) {
	   printf ("Floating species names:\n");
	   printf ("-----------------------\n");
	   std::cout<<stringArrayToString(getFloatingSpeciesIds())<<std::endl<<std::endl;
	}

	if (m > 0) {
	   printf ("Initial Floating species names:\n");
	   printf ("-------------------------------\n");
	   std::cout<<stringArrayToString(getFloatingSpeciesInitialConditionIds())<<std::endl;
	}

	if (b > 0) {
       printf ("\nBoundary species names:\n");
	   printf ("-----------------------\n");
	   std::cout<<stringArrayToString(getBoundarySpeciesIds())<<std::endl;
	}
	printf ("\n");

	if (p > 0) {
       printf ("\nGlobal Parameter names:\n");
	   printf ("-----------------------\n");
	   std::cout<<stringArrayToString(getGlobalParameterIds())<<std::endl;
	}
	printf ("\n");

	if (r > 0) {
       printf ("\nReaction names:\n");
	   printf ("---------------\n");
	   std::cout<<stringArrayToString(getReactionIds())<<std::endl;
	}
	printf ("\n");

	if (m> 0) {
       printf ("\nRates of change names:\n");
	   printf ("----------------------\n");
	   std::cout<<stringArrayToString(getRatesOfChangeIds())<<std::endl;
	}
	printf ("\n");


	if (r > 0) {
       printf ("\nUnscaled flux control coefficient names:\n");
	   printf ("----------------------------------------\n");
	   RRListHandle stringArray = getUnscaledFluxControlCoefficientIds();
	   std::cout<<listToString(stringArray)<<std::endl;
	}
	printf ("\n");

	if (m > 0) {
       printf ("\nUnscaled concentration control coefficient names:\n");
	   printf ("-------------------------------------------------\n");
	   std::cout<<listToString(getUnscaledConcentrationControlCoefficientIds())<<std::endl;
	}
	printf ("\n");

	double ssVal;
    bool success = steadyState(ssVal);
    if(!success)
    {
		cerr<<"Steady State call failed. Error was: "<<getLastError()<<std::endl;
    }
    else
    {
	    std::cout<<"Compute Steady State: sums of squares: "<<ssVal<<std::endl;
    }

	std::cout<<"Steady State selection List: "<<listToString(getSteadyStateSelectionList());
	setSteadyStateSelectionList("S2 S1");
	std::cout<<"\nSteady State selection List: "<<listToString(getSteadyStateSelectionList());

    printMatrix("Stoichiometry Matrix", getStoichiometryMatrix());

    std::cout<<"Number of independent species = "<<getNumberOfIndependentSpecies()<<std::endl;
    std::cout<<"Number of dependent Species = "<<getNumberOfDependentSpecies()<<std::endl<<std::endl;

    printMatrix("Link Matrix", getLinkMatrix());
	printMatrix("Nr Matrix", getNrMatrix());
	printMatrix("L0 Matrix", getL0Matrix());
	printMatrix("Full Jacobian Matrix", getFullJacobian());
	printMatrix("Reduced Jacobian Matrix:", getReducedJacobian());
    printMatrix("Eigenvalue Matrix (real/imag)", getEigenValues());
	printMatrix("Unscaled Elasticity Matrix:", getUnScaledElasticityMatrix());
    printMatrix("Scaled Elasticity Matrix:", getScaledElasticityMatrix());
	printMatrix("Unscaled Concentration Control Coefficients Matrix", getUnscaledConcentrationControlCoefficientMatrix());
	printMatrix("Scaled Concentration Control Coefficients Matrix:", getScaledConcentrationControlCoefficientMatrix());
	printMatrix("Unscaled Flux Control Coefficients Matrix", getUnscaledFluxControlCoefficientMatrix());
	printMatrix("Scaled Flux Control Coefficients Matrix", getScaledFluxControlCoefficientMatrix());

	double value;
	printf ("Flux Control Coefficient, CC^(_J1)_k1\n");
	getCC("_J1", "k1", value);
	printf ("Coefficient = %f\n", value);

	printf ("Flux Control Coefficient, CC^(_J1)_k2\n");
	getCC("_J1", "k2", value);
	printf ("Coefficient = %f\n", value);

	printf ("Flux Control Coefficient, CC^(_J1)_k3\n");
	getCC("_J1", "k3", value);
	printf ("Coefficient = %f\n", value);

	printf ("Flux Control Coefficient, CC^(_J1)_k4\n");
	getCC("_J1", "k4", value);
	printf ("Coefficient = %f\n", value);

	printf ("Elasticity Coefficient, EE^(_J1)_S1\n");
	getEE("_J1", "S1", value);
	printf ("Elasticity = %f\n", value);

	printf ("Elasticity Coefficient, EE^(_J2)_S1\n");
	getEE("_J2", "S1", value);
	printf ("Elasticity = %f\n", value);

	printf ("Elasticity Coefficient, EE^(_J2)_S2\n");
	getEE("_J2", "S2", value);
	printf ("Elasticity = %f\n", value);

	printf ("Elasticity Coefficient, EE^(_J3)_S2\n");
	getEE("_J3", "S2", value);
	printf ("Elasticity = %f\n", value);

	printf ("\n");
	//printf ("Flux Control Coefficient, C^(_J1)_k1\n");
	//double value;
	//getCC("_J1", "k1", value);
	//printf ("FCC = %f\n", value);

	/*getGlobalParameterByIndex (0, value);
	printf ("%f\n", value);
	getGlobalParameterByIndex (1, value);
	printf ("%f\n", value);
	getGlobalParameterByIndex (2, value);
	printf ("%f\n", value);
	getGlobalParameterByIndex (3, value);
	printf ("%f\n", value);*/

	RRVector veca;
	veca.Count = 3;
	veca.Data = new double[3];
   	veca.Data[0] = 1;
	veca.Data[1] = 2;
	veca.Data[2] = 3;

    double aValue = 231.23;
    bool bResult = setVectorElement(&veca, 0, aValue);
    if(!bResult)
    {
    	std::cout<<"Problem";
    }


    std::cout<<"List of floating species: \n"<<stringArrayToString(getFloatingSpeciesIds())<<std::endl;

	printf ("\nCall to getRatesOfChangeEx (S1=1, S2=2, S3=3):\n");
	std::cout<<vectorToString (getRatesOfChangeEx(&veca))<<std::endl;

//	printf ("\nCall to getReactionRatesEx (S1=1, S2=2, S3=3):\n");
//	std::cout<<printVector (getReactionRatesEx (&veca))<<std::endl;
//
//	printf ("\nCall to getRatesOfChange (with S1=1, S2=2, S3=3):\n");
//	std::cout<<printVector (getRatesOfChange())<<std::endl;

    setTimeCourseSelectionList("S1 S2");
//-------- The latest
    std::cout<<vectorToString(getFloatingSpeciesConcentrations());
    std::cout<<vectorToString(getGlobalParameterValues());
    std::cout<<"\n\n Symbols\n";
    RRList* symHandle = getAvailableTimeCourseSymbols();
    std::cout<<listToString(symHandle);
    freeRRList(symHandle);

    symHandle = getAvailableSteadyStateSymbols();
    std::cout<<"\n\n Steady state symbols\n";
    std::cout<<listToString(symHandle);
    freeRRList(symHandle);

    std::cout<<"\n\n ================================\n";
    RRVector* test = getReactionRates();
    std::cout<<vectorToString(test);

    setFloatingSpeciesByIndex(0,2);
    setFloatingSpeciesByIndex(1,4);
    setFloatingSpeciesByIndex(2,6);

    test = getReactionRates();
    std::cout<<vectorToString(test);

    //Get value problem..
    getValue("S1", value);
    std::cout<<value<<std::endl;
    getValue("S2", value);
    std::cout<<value<<std::endl;
    getValue("S3", value);
    std::cout<<value<<std::endl;

    getRatesOfChange();

    getValue("S1", value);
    std::cout<<value<<std::endl;
    getValue("S2", value);
    std::cout<<value<<std::endl;
    getValue("S3", value);
    std::cout<<value<<std::endl;

	//std::cout<<getBoundarySpeciesByIndex (0)<<std::endl;
    //getGlobalParameterByIndex(0, value);

    //std::cout<<value<<std::endl;
    //getGlobalParameterByIndex(2, value);
    //std::cout<<value<<std::endl;

    //std::cout<<getParamPromotedSBML(sbml.c_str());

    //std::cout<<getSBML()<<std::endl;

    //std::cout<<printMatrix(getScaledElasticityMatrix());     //How to free, when doing something like this??
    //std::cout<<printStringList(getEigenValueNames());

    std::cout<<"\n FluxControlCoeff ------\n"<<listToString(getFluxControlCoefficientIds())<<std::endl;

    std::cout<<"\n Unscaled FluxControlCoeff ------\n"<<listToString(getUnscaledFluxControlCoefficientIds())<<std::endl;
    RRList* list =getConcentrationControlCoefficientIds();
    std::cout<<listToString(list)<<std::endl;
    //freeList(list);


    //std::cout<<printStringList(getElasticityNames())<<std::endl;

//    setBoundarySpeciesByIndex(0,34);
    std::cout<<"Nr of Compartments: "<<getNumberOfCompartments()<<std::endl;
    setCompartmentByIndex(0,456);
    if(getCompartmentByIndex(0, value))
    {
        std::cout<<"Compartment Volume: "<<value<<std::endl;
    }
    else
    {
        std::cout<<getLastError()<<std::endl;
    }
    std::cout<<stringArrayToString(getCompartmentIds())<<std::endl;

    getRateOfChange(0, value);
    std::cout<<"Rate of change:"<<value<<std::endl;

	
    //std::cout<<stringArrayToString(getFloatingSpeciesInitialConditionIds())<<std::endl;


    std::cout<<" ---- getElasticityCoefficientNames ---\n"<<listToString(getElasticityCoefficientIds())<<std::endl;
//    std::cout<<stringArrayToString(getRateOfChangeIds())<<std::endl;
    setCapabilities (NULL);
    std::cout<<getCapabilities()<<std::endl;

//    C_DECL_SPEC bool                    rrCallConv   getScaledFloatingSpeciesElasticity(const char* reactionName, const char* speciesName, double& value);
    if(getScaledFloatingSpeciesElasticity("_J1", "S1", value))
    {
        std::cout<<"ScaledFloatingSpeciesElasticity "<<value<<std::endl;
    }
    else
    {
        std::cout<<getLastError()<<std::endl;
    }

    std::cout<<"getFloatingSpeciesInitialConditionNames: "<<stringArrayToString(getFloatingSpeciesInitialConditionIds())<<std::endl;


    std::cout<<getCurrentSBML();
	///////////////////
    text = getCopyright();
    if(hasError())
    {
        char* error = getLastError();
        std::cout<<error<<std::endl;
    }

    std::cout<<text<<std::endl;
    freeText(text);
    freeRRInstance(rrHandle);
    return 0;
}

void printMatrix(char* msg1, RRMatrixHandle mat)
{
	std::cout<<msg1<<"\n";
	std::cout<<("------------\n\n");
    char *text = matrixToString(mat);
    if(text)
    {
		std::cout<<text<<"\n\n";
        freeText(text);
    }
    else
    {
    	std::cout<<"NULL\n\n";
    }
}
