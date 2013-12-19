#pragma hdrstop
#include "rrLogger.h"
#include "rrException.h"
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
mLMFit(                     "LMFit",                                "Fit Model Parameters Using the Levenberg-Marquardt Algorithm"),    //The 'capability'
mSBML(                      "SBML",                                 "<none>",               "SBML, i.e. the model to be used in the fitting"),
mObservedData(              "ExperimentalData",                     NULL,                   "Data object holding Experimental data: Provided by client"),
mModelData(                 "FittedData",                           NULL,                   "Data object holding model data: Handed to client"),
mResidualsData(             "Residuals",                            NULL,                   "Data object holding residuals: Handed to client"),
mInputParameterList(        "InputParameterList",                   Parameters(),           "List of parameters to fit"),
mOutputParameterList(       "OutputParameterList",                  Parameters(),           "List of parameters that was fittedt"),
mObservedDataSelectionList( "ExperimentalDataSelectionList",            StringList(),       "Experimental data selection list"),
mModelDataSelectionList(    "FittedDataSelectionList",               StringList(),           "Fitted data selection list"),
mNorm(                      "Norm",                                 -1.0,                   "Norm of fitting. An estimate of goodness of fit"),
mLMWorker(*this)
{
    mVersion = "1.0";
    //Setup the plugins capabilities
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

    //Allocate model and Residuals data
    mResidualsData.setValue(new RoadRunnerData());
    mModelData.setValue(new RoadRunnerData());
}

LM::~LM()
{
    //DE allocate data        
    RoadRunnerData* data = mResidualsData.getValue();
    delete data;
    data =  mModelData.getValue();
    delete data;
}

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
    mObservedData.getValueReference()->clear();
    mModelData.getValueReference()->clear();
    mResidualsData.getValueReference()->clear();
    mInputParameterList.getValueReference().clear();
    mOutputParameterList.getValueReference().clear();
    mObservedDataSelectionList.getValueReference().clear();
    mModelDataSelectionList.getValueReference().clear();
    return true;
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

bool LM::execute(bool inThread)
{
    try
    {
        Log(lInfo)<<"Executing the LM plugin";
        mLMWorker.start(inThread);
        return true;
    }
    catch(const Exception& ex)
    {
        Log(lError) << "There was a problem in the execute of the LMFIT plugin: " << ex.getMessage();
        throw(ex);
    }
    catch(...)
    {
        Log(lError) << "There was an unknown problem in the execute of the LMFIT plugin.";
        throw("There was an unknown problem in the execute of the LMFIT plugin.");
    }
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

