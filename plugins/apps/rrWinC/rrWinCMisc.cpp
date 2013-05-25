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

		vector<string> lines = rr::SplitString(info, "\n");
		for(int i =0; i < lines.size(); i++)
		{
			Log()<<lines[i];
		}

        simFrame->assignRRHandle(mRRI);
        fullSpaceFitFrame->assignRRHandle(mRRI);
        enableLoggingToFile(mRRI);
        setLogLevel("Info");
	}

    mLogFileSniffer.SetFileName(getLogFileName());
    mLogFileSniffer.start();
	loadPluginsAExecute(NULL);

	populateModelsDropDown();
	mUIIsStartingUp = false;
    loadModelA->Execute();
}

void TMainF::populateModelsDropDown()
{
	//Populate the drop down.
 	//Get all models in models  folder
    std::set<std::string> files;
    string globPath =  rr::JoinPath(mModelsFolder, "*.xml");
    Glob::glob(globPath, files);
    std::set<std::string>::iterator it = files.begin();
	modelDD->Clear();
    for (; it != files.end(); ++it)
    {
    	string model  = ExtractFileNameNoExtension(*it);
        Log()<<"Adding model: "<<model;
		modelDD->Items->Add(model.c_str());
    }
    modelDD->ItemIndex = 2;

}

//---------------------------------------------------------------------------
void __fastcall TMainF::loadPluginsAExecute(TObject *Sender)
{
	if(!loadPlugins(mRRI))
	{
		Log() << "There was some problems loading plugins, check the log file.";
	}

	//Populate list box with plugins
	RRStringArray* pluginNames = getPluginNames(mRRI);

	for(int i = 0; i < pluginNames->Count; i++)
	{
		pluginList->AddItem(pluginNames->String[i], NULL);
	}
	Log() << "Loaded plugins..";

    mAddNoisePlugin = getPlugin(mRRI, "AddNoise");
	mMinimizePlugin = getPlugin(mRRI, "FullSpaceMinimization");
    Button1->Action = unloadPlugins;
	fullSpaceFitFrame->assignPluginHandle(mMinimizePlugin);
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

    //Populate plugin frame
	pluginCapsCB->Clear();
    pluginParasCB->Clear();

    RRStringArray* caps = getPluginCapabilities(mCurrentlySelectedPlugin);

    if(!caps)
    {
	    GroupBox5->Enabled = false;
    	return;
    }

    GroupBox5->Enabled = true;
    for(int i =0; i < caps->Count; i++)
    {
        pluginCapsCB->AddItem(caps->String[i], NULL);
    }

    pluginCapsCB->ItemIndex = 0;
    pluginCBChange(pluginCapsCB);

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
	RRParameter* sigma = getPluginParameter(mAddNoisePlugin, "Sigma", NULL);

	if(!sigma)
    {
    	Log()<<"Failed to get parameter: Sigma";
    }
    else
    {
		Log()<<"Name: "<<sigma->mName;
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

string TMainF::getCurrentSelectedParameter()
{
    if(pluginParasCB->ItemIndex == -1)
    {
    	return "";
    }

    string pluginParaName = stdstr(pluginParasCB->Items->Strings[pluginParasCB->ItemIndex]);
	return pluginParaName;
}

void __fastcall TMainF::pluginCBChange(TObject *Sender)
{

	if(pluginCapsCB == (TComboBox*)(Sender))
    {
        string pluginName = stdstr(pluginList->Items->Strings[pluginList->ItemIndex]);
		mCurrentlySelectedPlugin = getPlugin(mRRI, pluginName.c_str());

		//Change available parameters in the parsCB...
		pluginParasCB->Clear();

        string capa = stdstr(pluginCapsCB->Items->Strings[pluginCapsCB->ItemIndex]);
        RRStringArray* paras = getPluginParameters(mCurrentlySelectedPlugin, capa.c_str());
        pluginParasCB->Clear();

        if(!paras)
        {
            Log()<<"No parameters!";
            pluginCBChange(NULL);
            return;
        }

        for(int i =0; i < paras->Count; i++)
        {
            pluginParasCB->AddItem(paras->String[i], NULL);
        }
        pluginParasCB->ItemIndex = 0;
    }

    if(pluginParasCB == (TComboBox*)(Sender))
    {
        //Query the current plugin for the current value of selected parameter
        RRParameterHandle para = getPluginParameter(mCurrentlySelectedPlugin, getCurrentSelectedParameter().c_str(), NULL);
        if(!para)
        {
            paraEdit->Enabled = false;
            return;
        }

        paraEdit->Enabled = true;
        pluginParasCB->Hint = para->mHint;

        if(para->ParaType == ptInteger)
        {
            paraEdit->Text = rr::ToString(para->data.iValue).c_str();
        }
        else
        {
            paraEdit->Text = "not implemented";
        }
    }
}

//---------------------------------------------------------------------------
void __fastcall TMainF::SetParaBtnClick(TObject *Sender)
{
	int iVal;
    double dVal;
	string paraText = stdstr(paraEdit->Text);

	switch(RadioGroup1->ItemIndex)
    {
    	case 0:
        break;
        case 1:
        break;
        case 3:
        	dVal =  rr::ToDouble(paraText);
			setPluginParameter(mCurrentlySelectedPlugin, getCurrentSelectedParameter().c_str(), (char*) &dVal);
        break;
    }
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
            fullSpaceFitFrame->loadParameterList();
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
    	double val = noiseSigmaE->Text.ToDouble();
		string msg = (setPluginParameter(mAddNoisePlugin, "Sigma", (char*) &val)) ? "Sigma was updated" : "Failed to update Sigma";
        Log() << msg;
    }
}

