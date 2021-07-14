#pragma hdrstop
#include "telLogger.h"
#include "add_noise.h"
#include "telTelluriumData.h"
#include "noise.h"
#include "add_noise_doc.h"


//---------------------------------------------------------------------------
namespace addNoise
{
    rrc::THostInterface* gHostInterface;

    AddNoise::AddNoise(rrc::THostInterface* mhost, PluginEvent fn1, PluginEvent fn2, PluginEvent fn3) :
        CPPPlugin("AddNoise", "Signal Processing"),//Construct Base
        mNoiseType(ntGaussian, "NoiseType", "Type of noise (Gaussian = 0)."),
        mSigma(1, "Sigma", "Size of applied noise"),
        mData(TelluriumData(), "InputData", "Data on which noise will be applied to"),
        mSeed(0, "Seed", "Seed to use for each use of 'execute' (0 to ignore)"),
        mProgress(0, "Progress", "Indicate progress in (0-100%)"),
        mAssumeTime(false, "AssumeTime", "Assume the first column in the input data is time, and don't add noise to it."),
        mAddNoiseWorker(*this)
    {
        mVersion = "1.0.1";

        //Setup the plugins capabilities
        mProperties.add(&mNoiseType);
        mProperties.add(&mSigma);
        mProperties.add(&mData);
        mProperties.add(&mAssumeTime);
        mProperties.add(&mProgress);
        mProperties.add(&mSeed);

        mHint = "Add Gaussian Noise to RoadRunner Data";
        mDescription = "The AddNoise plugin adds Gaussian noise to synthetic data. The amount of noise is controlled \
by the plugins Sigma property. Specifically, noise is generated for each single data value, with a probability corresponding to a Gaussian distribution centered around the value, and with a variance equal to (sigma^2). \
The Plugin accepts Tellurium data as input, in the \"InputData\" property. \
Currently only Gaussian noise is supported. \
The progress of the application of noise can be read in the Progress property. \
Noise will not be generated onto the first column of data, if its column label is equal to \"Time\", (not case sensitive). \
The AddNoise plugin was developed at the University of Washington by Totte Karlsson, 2012-2014.";

        //The function below assigns property descriptions
        assignPropertyDescriptions();
    }

    AddNoise::~AddNoise()
    {
    }

    unsigned char* AddNoise::getManualAsPDF() const
    {
        return pdf_doc;
    }

    size_t AddNoise::getPDFManualByteSize()
    {
        return sizeofPDF;
    }

    bool AddNoise::isWorking() const
    {
        return mAddNoiseWorker.isRunning();
    }

    bool AddNoise::execute(bool inThread)
    {
        RRPLOG(lDebug) << "Executing the AddNoise plugin by Totte Karlsson";
        //go away and carry out the work in a thread
        if (mSeed.getValue() != 0) {
            mAddNoiseWorker.setSeed(mSeed.getValue());
        }
        return mAddNoiseWorker.start(inThread);
    }

    void AddNoise::assignPropertyDescriptions()
    {
        stringstream s;

        s << "Type of noise applied on data. Only Gaussian noise is currently supported.";
        mNoiseType.setDescription(s.str());
        s.str("");

        s << "Size of applied noise. Noise is generated for each single data value, with a probability corresponding to a Gaussian distribution, centered around the value, and with a variance equal to sigma^2";
        mSigma.setDescription(s.str());
        s.str("");

        s << "Data on which noise will be applied to";
        mData.setDescription(s.str());
        s.str("");

        s << "The progress property communicates the progress (in percent) of Noise application.";
        mProgress.setDescription(s.str());
        s.str("");

    }
#ifdef EXPORT_ADD_NOISE

    // Plugin factory function
    Plugin* plugins_cc createPlugin()
    {
        //allocate a new object and return it
        return new AddNoise;
    }

    const char* plugins_cc getImplementationLanguage()
    {
        return "CPP";
    }

    void plugins_cc setHostInterface(rrc::THostInterface* _hostInterface) {
        gHostInterface = _hostInterface;
    }

#endif

}



