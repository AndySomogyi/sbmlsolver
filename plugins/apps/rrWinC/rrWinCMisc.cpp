#include <vcl.h>
#pragma hdrstop
#include "Poco/Glob.h"
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
using Poco::Glob;

void __fastcall TMainF::startupTimerTimer(TObject *Sender)
{
	startupTimer->Enabled = false;
	mRRI = createRRInstanceEx("r:\\cTemp");
	if(!mRRI)
	{
		Log() << "Problem creating rr instance.";
		throw rr::Exception(getLastError());
	}
	else
	{
		apiVersionLBL->Caption = getAPIVersion();
		buildDateLbl->Caption  = getBuildDate();
		buildTimeLbl->Caption  = getBuildTime();
		string info 		   = getExtendedAPIInfo();

		vector<string> lines = rr::splitString(info, "\n");
		for(int i =0; i < lines.size(); i++)
		{
			Log()<<lines[i];
		}

        simFrame->assignRRHandle(mRRI);
//        fullSpaceFitFrame->assignRRHandle(mRRI);
        enableLoggingToFile(mRRI);
        setLogLevel("Info");
	}

    mLogFileSniffer.SetFileName(getLogFileName());
    mLogFileSniffer.start();
	loadPluginsAExecute(NULL);

	populateModelsDropDown();

    string modelNameNoExtension = extractFileNameNoExtension(mModel);
    int index = modelDD->Items->IndexOf(vclstr(modelNameNoExtension));

    modelDD->ItemIndex = (index > -1) ? index : 2;
	mUIIsStartingUp = false;
    loadModelA->Execute();
}

void TMainF::populateModelsDropDown()
{
	//Populate the drop down.
 	//Get all models in models  folder
    std::set<std::string> files;
    string globPath =  rr::joinPath(mModelsFolder, "*.xml");
    Glob::glob(globPath, files);
    std::set<std::string>::iterator it = files.begin();
	modelDD->Clear();
    for (; it != files.end(); ++it)
    {
    	string model  = extractFileNameNoExtension(*it);
        Log()<<"Adding model: "<<model;
		modelDD->Items->Add(model.c_str());
    }
}

void __fastcall TMainF::unloadPluginsExecute(TObject *Sender)
{
	if(!unLoadPlugins(mRRI))
	{
		Log() << "failed un-loading plugins..";
		throw rr::Exception(getLastError());
	}

    pluginList->Clear();
	Log() << "Un-Loaded plugins..";
    Button1->Action = loadPluginsA;
}

void __fastcall TMainF::ApplicationEvents1Exception(TObject *Sender, Sysutils::Exception *E)
{
	Log() << stdstr(E->ToString()) <<endl;
}

void __fastcall TMainF::pluginListClick(TObject *Sender)
{
	//Retrieve the name of the plugin that was clicked
    if(pluginList->ItemIndex == -1)
    {
    	return;
    }

    string pluginName = stdstr(pluginList->Items->Strings[pluginList->ItemIndex]);
    Log()<<pluginName;

	mCurrentlySelectedPlugin = getPlugin(mRRI, pluginName.c_str());
	if(!mCurrentlySelectedPlugin)
    {
    	return;
    }
    string test = getPluginInfo(mCurrentlySelectedPlugin);

    infoMemo->Clear();
    Log()<<test;


	UpdateNoisePanel();
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
    	Log()<<"Failed to get parameter: Sigma";
    }
    else
    {
		Log()<<"Name: "<<getParameterName(sigma);
    }
}

//---------------------------------------------------------------------------
void __fastcall TMainF::getPluginInfoAExecute(TObject *Sender)
{
	string pName = getCurrentPluginName();
    mCurrentlySelectedPlugin = getPlugin(mRRI, pName.c_str());
    Log()<<getPluginInfo(mCurrentlySelectedPlugin);
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
    		Log () << "Job did finish. Cleaning up.";
            if(freeJob(mLoadModelJob))
            {
            	mLoadModelJob = NULL;
				loadModelJobTimer->Enabled = false;
            }
            else
            {
				Log() << "Problem deleting a job..";
				throw rr::Exception(getLastError());
            }
			simFrame->loadSelectionList();
//            fullSpaceFitFrame->loadParameterList();

			if(mLMFrame)
            {
            	mLMFrame->loadParameterList();
            }

			if(mLMAFrame)
            {
            	mLMAFrame->loadParameterList();
            }
            simFramesimBtnClick(NULL);
        }
        else
        {
    		Log () << "Busy...";
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
        Log() << msg;
    }
}

