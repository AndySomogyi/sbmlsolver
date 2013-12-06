#pragma hdrstop
#include "rrLogger.h"
#include "lm.h"
#include "../../wrappers/C/rrc_api.h"
#include "../../wrappers/C/rrc_utilities.h"
#include "rrRoadRunner.h"
#include "rrRoadRunnerData.h"
#include "rrUtils.h"
#include "lmfit_doc.h"
//---------------------------------------------------------------------------

namespace lmfit
{
using namespace rr;
using namespace rrc;


LM::LM(rr::RoadRunner* aRR)
:
CPPPlugin(                  "Levenberg-Marquardt", "Fitting",       aRR, NULL),
mLMFit(                     "LMFit",                                "",                     "Fit Parameters using the Levenberg-Marquardt algorithm"),    //The 'capability'
mTempFolder(                "TempFolder",                           "",                     "Tempfolder used in the fitting"),
mSBML(                      "SBML",                                 "<none>",               "SBML, i.e. the model to be used in the fitting"),
mObservedData(              "ObservedData",                         RoadRunnerData(),       "Data object holding observed (experimental) data"),
mModelData(                 "ModelData",                            RoadRunnerData(),       "Data object holding model data"),
mResidualsData(             "ResidualsData",                        RoadRunnerData(),       "Data object holding residuals"),
mInputParameterList(        "InputParameterList",                   Parameters(),           "List of parameters to fit"),
mOutputParameterList(       "OutputParameterList",                  Parameters(),           "List of parameters that was fittedt"),
mObservedDataSelectionList( "ObservedDataSelectionList",            StringList(),           "Observed data selection list"),
mModelDataSelectionList(    "ModelDataSelectionList",               StringList(),           "Modeled data selection list"),
mNorm(                      "Norm",                                 -1.0,                   "Norm of fitting. An estimate of goodness of fit"),
mLMWorker(*this)
{

    //Setup the plugins capabilities
    mLMFit.addParameter(&mTempFolder);
    mLMFit.addParameter(&mSBML);
    mLMFit.addParameter(&mObservedData);
    mLMFit.addParameter(&mModelData);
    mLMFit.addParameter(&mResidualsData);
    mLMFit.addParameter(&mInputParameterList);
    mLMFit.addParameter(&mOutputParameterList);
    mLMFit.addParameter(&mObservedDataSelectionList);
    mLMFit.addParameter(&mModelDataSelectionList);
    mLMFit.addParameter(&mNorm);
    mCapabilities.add(mLMFit);
}

LM::~LM()
{}

bool LM::isWorking()
{
    return mLMWorker.isRunning();
}

unsigned char* LM::getManualAsPDF() const
{
    return pdf_doc;
}

unsigned int LM::getPDFManualByteSize()
{
    return sizeofPDF;
}

StringList LM::getObservedDataSelectionList()
{
    return mObservedDataSelectionList.getValue();
}

string LM::getStatus()
{
    stringstream msg;
    msg<<Plugin::getStatus();
    msg<<"TempFolder: "<<mTempFolder<<"\n";
    msg <<getResult();
    return msg.str();
}

string LM::getImplementationLanguage()
{
    return lmfit::getImplementationLanguage();
}

bool LM::resetPlugin()
{
    if(mLMWorker.isRunning())
    {
        return false;
    }

    mTerminate = false;
    mSBML.getValueReference().clear();
    mObservedData.getValueReference().clear();
    mModelData.getValueReference().clear();
    mResidualsData.getValueReference().clear();
    mInputParameterList.getValueReference().clear();
    mOutputParameterList.getValueReference().clear();
    mObservedDataSelectionList.getValueReference().clear();
    mModelDataSelectionList.getValueReference().clear();
    return true;
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
    Parameters& pars = mOutputParameterList.getValueReference();

    for(int i = 0; i < pars.count(); i++)
    {
        msg<<pars[i]->asString();
    }
    msg<<"Norm: "<<mNorm.getValue();
    return msg.str();
}

bool LM::setInputData(void* inputData)
{
    //Cast data to RRData
    RoadRunnerData* data = (RoadRunnerData*) inputData;

    if(!data)
    {
        return false;
    }

    RoadRunnerData rrData((*data));
    mObservedData.setValue((*data));    //This copies the data
    return true;
}

bool LM::execute(void* data, bool inThread)
{
    Log(lInfo)<<"Executing the LM plugin";
    mLMWorker.start(inThread);
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

}

