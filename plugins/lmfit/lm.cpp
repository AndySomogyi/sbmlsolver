#pragma hdrstop
#include "rrLogger.h"
#include "lm.h"
#include "../../wrappers/C/rrc_api.h"
#include "../../wrappers/C/rrc_utilities.h"
#include "rrRoadRunner.h"
#include "rrRoadRunnerData.h"
#include "rrUtils.h"
//---------------------------------------------------------------------------

namespace lm
{
using namespace rr;
using namespace rrc;

LM::LM(rr::RoadRunner* aRR)
:
Plugin(                 "Levenberg-Marquardt", "Fitting",       aRR),
mLMFit(                 "LMFit",                                "",                   "Fit Parameters using the Levenberg-Marquardt algorithm"),    //The 'capability'
mTempFolder(            "TempFolder",                           "",                   "Tempfolder used in the fitting"),
mSBML(                  "SBML",                                 "<none>",             "SBML, i.e. the model to be used in the fitting"),
mMinimizationData(      "MinData",                              MinimizationData(),   "Data structure holding minimization data"),
mLMFitThread(*this)
{
    //Setup the plugins capabilities
    mLMFit.addParameter(&mTempFolder);
    mLMFit.addParameter(&mSBML);
    mLMFit.addParameter(&mMinimizationData);
    mCapabilities.add(mLMFit);
}

LM::~LM()
{}

bool LM::isWorking()
{
    return mLMFitThread.isRunning();
}

string LM::getStatus()
{
    stringstream msg;
    msg<<Plugin::getStatus();
    msg<<"TempFolder: "<<mTempFolder<<"\n";
    msg<<"SBML: "<<mSBML<<"\n";
    MinimizationData* minData = (MinimizationData*) (mMinimizationData.getValueAsPointer());
    msg<<"MinData"<<(*minData)<<"\n";
    return msg.str();
}

string LM::getImplementationLanguage()
{
    return lm::getImplementationLanguage();
}

bool LM::resetPlugin()
{
    if(mLMFitThread.isRunning())
    {
        return false;
    }

    MinimizationData& data = getMinimizationData();
    data.reset();
    return true;
}

MinimizationData& LM::getMinimizationData()
{
    return *(mMinimizationData.getValuePointer());
}

string LM::getTempFolder()
{
    return mTempFolder.getValue();
}

string LM::getSBML()
{
    return mSBML.getValue();
}

string LM::getResult()
{
    stringstream msg;
    MinimizationData& data = getMinimizationData();
    Parameters pars = data.getParametersOut();
    for(int i = 0; i < pars.count(); i++)
    {
        msg<<pars[i]->asString();
    }
    msg<<"Norm: "<<data.getNorm();
    return msg.str();
}

bool LM::setInputData(void* inputData)
{
    //Cast data to RRData structire
    RRCDataPtr data = (RRCData*) inputData;

    if(!data)
    {
        return false;
    }

    RoadRunnerData rrData(data->RSize, data->CSize);

    //Column Names
    StringList colNames;
    for(int c = 0; c < data->CSize; c++)
    {
        colNames.add(data->ColumnHeaders[c]);
    }
    rrData.setColumnNames(colNames);

    //The data
    for(int r = 0; r < data->RSize; r++)
    {
        for(int c = 0; c < data->CSize; c++)
        {
            rrData(r,c) = data->Data[r*data->CSize + c];
        }
    }

    //Weights ?
    if(data->Weights != NULL)
    {
        rrData.allocateWeights();
        for(int r = 0; r < data->RSize; r++)
        {
            for(int c = 0; c < data->CSize; c++)
            {
                rrData.setWeight(r,c) = data->Weights[r*data->CSize + c];
            }
        }
    }

    MinimizationData& minData = getMinimizationData();
    minData.setInputData(rrData);
    return true;
}

bool LM::execute(void* inputData)
{
    Log(lInfo)<<"Executing the LM plugin";

    //go away and carry out the work in a thread
    //Assign callback functions to communicate the progress of the thread
    mLMFitThread.assignCallBacks(mWorkStartedCB, mWorkFinishedCB, mUserData);

    mLMFitThread.start();
    return true;
}

// Plugin factory function
LM* plugins_cc createPlugin(rr::RoadRunner* aRR)
{
    //allocate a new object and return it
    return new LM(aRR);
}

const char* plugins_cc getImplementationLanguage()
{
    return "CPP";
}

_xmlNode* LM::createConfigNode()
{
    //mLMFit(                 "LMFit",                                "",                   "Run a one species fit"),    //The 'capability'
    //mTempFolder(            "TempFolder",                           "",                   "Tempfolder used in the fitting"),
    //mSBML(                  "SBML",                                 "<none>",             "SBML, i.e. the model to be used in the fitting"),
    //mMinimizationData(      "MinData",                                 MinimizationData(),   "Data structure holding minimization data"),

    _xmlNode *caps = Configurable::createCapabilityNode("LMFit", "", "Run a one species fit");
    Configurable::addChild(caps, Configurable::createParameterNode("TempFolder",  "Tempfolder used in the fitting", "<none>"));
    Configurable::addChild(caps, Configurable::createParameterNode("SBML", "SBML, i.e. the model to be used in the fitting", ""));
    return caps;
}

void LM::loadConfig(const _xmlDoc* doc)
{
}

}

