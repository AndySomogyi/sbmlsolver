#ifndef rrSBMLTestSuiteSimulation_CAPIH
#define rrSBMLTestSuiteSimulation_CAPIH
//---------------------------------------------------------------------------
#include "rrTestSuiteModelSimulation.h"
#include "rr_c_api.h"

using namespace rr;
using namespace rr_c_api;

class SBMLTestSuiteSimulation_CAPI : public rr::TestSuiteModelSimulation
{
	private:
		RRHandle 			mRRHandle;
		RRResultHandle		mResultHandle;

    public:
    						SBMLTestSuiteSimulation_CAPI(const string& dataOutputFolder = "", const string& modelFilePath = "", const string& modelFileName = "");
                           ~SBMLTestSuiteSimulation_CAPI();
		void	  			UseHandle(RRHandle handle);
        bool            	LoadSBMLFromFile();                    //Use current file information to load sbml from file
        bool            	LoadSettings(const string& fName = "");
		bool 				Simulate();
        bool            	SaveResult();
        SimulationData  	GetResult();
};

#endif
