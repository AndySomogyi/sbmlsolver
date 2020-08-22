#include <iostream>

#include "rr/rrRoadRunner.h"
#include "rr/rrUtils.h"
#include "rr/C/rrc_api.h"
#include "telLogger.h"
#include "telException.h"
#include "telplugins_c_api.h"

using namespace rr;
using namespace rrc;
using namespace tlp;
using namespace std;

int main(int argc, char** argv)
{
    const char* rootPath = "..";

    try
    {
        Logger::setLevel(lDebug);
        Logger::enableFileLogging("Test.log");
        string model = "bistable.xml";
        const string modelFile = joinPath(rootPath, "models", model);

        RoadRunner rr1;
        LoadSBMLOptions opt;

        //opt.modelGeneratorOpt |= LoadSBMLOptions::RECOMPILE;

        if(!rr1.load(modelFile, &opt))
        {
            RRPLOG(lError)<<"There was a problem loading model in file: "<<modelFile;
            throw(Exception("Bad things in loadSBMLFromFile function"));
        }

        SimulateOptions  simOpt;
        simOpt.start = 0;
        simOpt.start = 10;
        simOpt.steps= 511;

        rr1.setGlobalParameterByIndex(2, 0.2);
        double ss = rr1.steadyState();

        RRPLOG(lInfo) << "Steady State: " <<ss;
        const RoadRunnerData* rrData = rr1.simulate(&simOpt);

        TELHandle dataHandle = tpCreateTelluriumDataFromRoadRunnerData( (TELHandle) rrData);

        int nrRows = tpGetTelluriumDataNumRows(dataHandle);
        int nrCols = tpGetTelluriumDataNumCols(dataHandle);
        char* colHeader = tpGetTelluriumDataColumnHeader(dataHandle);

        cout<<"NrRows: "<<nrRows<<endl;
        cout<<"NrCols "<<nrCols<<endl;
        cout<<"Header: "<<colHeader<<endl;
        for(int row = 0; row < nrRows; row++)
        {
            cout<<"row: "<<row<<"\t";
            for(int col = 0; col < nrCols; col++)
            {
                double value;
                tpGetTelluriumDataElement(dataHandle, row, col, &value);
                cout<<value<<"\t";
            }
            cout<<endl;
        }

    }
    catch(const exception& ex)
    {

        RRPLOG(Logger::LOG_ERROR)<<"There was a  problem: "<<ex.what();
    }

    return 0;
}




