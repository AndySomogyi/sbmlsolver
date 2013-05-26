#include <vcl.h>
#pragma hdrstop
#include "Poco/SharedLibrary.h"
#include "rrUtils.h"
#include "MainForm.h"
#include "rrException.h"
#include "rrVCLUtils.h"
#include "rrMemoLogger.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

using namespace std;
using Poco::SharedLibrary;

void __fastcall TMainF::startupTimerTimer(TObject *Sender)
{
	startupTimer->Enabled = false;
	mRRI = createRRInstanceEx("..\\Temp");
	if(!mRRI)
	{
		ML() << "Problem creating rr instance.";
		throw rr::Exception(getLastError());
	}
	else
	{
        simFrame->assignRRHandle(mRRI);
        enableLoggingToFile(mRRI);

        string lvl = mLogLevel;
	    setLogLevel(lvl.c_str());
	}

    mLogFileSniffer.SetFileName(getLogFileName());
    mLogFileSniffer.start();
	loadPluginsAExecute(NULL);

	populateFileSet(mModelsFolder, mModelFiles);
	populateDropDown(mModelFiles, modelDD);

    string modelNameNoExtension = getFileNameNoExtension(mModel);
    int index = modelDD->Items->IndexOf(vclstr(modelNameNoExtension));

    modelDD->ItemIndex = (index > -1) ? index : 2;
	mUIIsStartingUp = false;
    loadModelA->Execute();
}

void __fastcall TMainF::unloadPluginsExecute(TObject *Sender)
{
	if(!unLoadPlugins(mRRI))
	{
		ML() << "failed un-loading plugins..";
		throw rr::Exception(getLastError());
	}

    pluginList->Clear();
	ML() << "Un-Loaded plugins..";
    Button1->Action = loadPluginsA;
}

void __fastcall TMainF::ApplicationEvents1Exception(TObject *Sender, Sysutils::Exception *E)
{
	ML() << stdstr(E->ToString()) <<endl;
}

void __fastcall TMainF::pluginListClick(TObject *Sender)
{
	//Retrieve the name of the plugin that was clicked
    if(pluginList->ItemIndex == -1)
    {
    	return;
    }

    string pluginName = stdstr(pluginList->Items->Strings[pluginList->ItemIndex]);
    ML()<<pluginName;

	mCurrentlySelectedPlugin = getPlugin(mRRI, pluginName.c_str());
	if(!mCurrentlySelectedPlugin)
    {
    	return;
    }
    string test = getPluginInfo(mCurrentlySelectedPlugin);

    infoMemo->Clear();
    ML()<<test;
}

void TMainF::UpdateNoisePanel()
{
	//This function is called if the Plugin list box is clicked
    string pluginName = stdstr(pluginList->Items->Strings[pluginList->ItemIndex]);
	if(pluginName != "AddNoise")
    {
    	return;
    }

	//At this point we know the capabilities and parameters of this plugin
	RRParameterHandle sigma = getPluginParameter(mAddNoisePlugin, "Sigma", NULL);

	if(!sigma)
    {
    	ML()<<"Failed to get parameter: Sigma";
    }
    else
    {
		ML()<<"Name: "<<getParameterName(sigma);
    }
}

//---------------------------------------------------------------------------
void __fastcall TMainF::getPluginInfoAExecute(TObject *Sender)
{
	string pName = getCurrentPluginName();
    mCurrentlySelectedPlugin = getPlugin(mRRI, pName.c_str());
    ML()<<getPluginInfo(mCurrentlySelectedPlugin);
}

//---------------------------------------------------------------------------
string TMainF::getCurrentPluginName()
{
    if(pluginList->ItemIndex == -1)
    {
    	return "";
    }

    string pluginName = stdstr(pluginList->Items->Strings[pluginList->ItemIndex]);
	return pluginName;
}

void __fastcall TMainF::loadModelJobTimerTimer(TObject *Sender)
{
	if(mLoadModelJob)
    {
    	if(isJobFinished(mLoadModelJob))
        {
    		ML() << "Job did finish. Cleaning up.";
            if(freeJob(mLoadModelJob))
            {
            	mLoadModelJob = NULL;
				loadModelJobTimer->Enabled = false;
            }
            else
            {
				ML() << "Problem deleting a job..";
				throw rr::Exception(getLastError());
            }

			simFrame->loadSelectionList();
            simFramesimBtnClick(NULL);
        }
        else
        {
    		ML() << "Busy...";
        }
    }
}

//---------------------------------------------------------------------------
void __fastcall TMainF::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if(Key == VK_ESCAPE)
	{
		Close();
	}
}

void __fastcall TMainF::loadModelAUpdate(TObject *Sender)
{
	if(!mUIIsStartingUp)
    {
		loadBtn->Action = isModelLoaded(mRRI) ? unLoadModelA : loadModelA;
    }
}

//---------------------------------------------------------------------------
void __fastcall TMainF::noiseSigmaEKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if(Key == VK_RETURN)
    {
    	string val = stdstr(noiseSigmaE->Text);
		string msg = (setPluginParameter(mAddNoisePlugin, "Sigma", val.c_str())) ? "Sigma was updated" : "Failed to update Sigma";
        ML() << msg;
    }
}

