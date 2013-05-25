//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "rrRoadRunner.h"
#include "rrSimulationData.h"
#include "rrUtils.h"
#include "Main.h"
#include "rrc_api.h"
#include "rrException.h"
#include "vcl_utils.h"
#include "../../../Wrappers/C/rrc_support.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "rrSimulationFrame"
#pragma link "mtkFloatLabeledEdit"
#pragma resource "*.dfm"
TMainF *MainF;

//---------------------------------------------------------------------------
__fastcall TMainF::TMainF(TComponent* Owner)
:
TForm(Owner),
mModel("r:\\models\\test_1.xml"),
mUIIsStartingUp(true),
mData(NULL)
{
	simFrame->infoMemo = infoMemo;
	startupTimer->Enabled = true;
}

//---------------------------------------------------------------------------
void __fastcall TMainF::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if(Key == VK_ESCAPE)
	{
		Close();
	}
}

void __fastcall TMainF::startupTimerTimer(TObject *Sender)
{
	startupTimer->Enabled = false;
	mInstanceH = createRRInstanceEx("r:\\cTemp");
	if(!mInstanceH)
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

        simFrame->assignRRHandle(mInstanceH);
        enableLoggingToFile(mInstanceH);
        setLogLevel("Debug2");
	}
	loadPluginsAExecute(NULL);
	mUIIsStartingUp = false;
}

//---------------------------------------------------------------------------
void __fastcall TMainF::loadPluginsAExecute(TObject *Sender)
{
	if(!loadPlugins(mInstanceH))
	{
		Log() << "There was some problems loading plugins, check the log file.";
	}

	//Populate list box with plugins
	RRStringArray* pluginNames = getPluginNames(mInstanceH);

	for(int i = 0; i < pluginNames->Count; i++)
	{
		pluginList->AddItem(pluginNames->String[i], NULL);
	}
	Log() << "Loaded plugins..";
    Button1->Action = unloadPlugins;
}

void __fastcall TMainF::unloadPluginsExecute(TObject *Sender)
{
	if(!unLoadPlugins(mInstanceH))
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
	Log() << std_str(E->ToString()) <<endl;
}

void __fastcall TMainF::pluginListClick(TObject *Sender)
{
	//Retrieve the name of the plugin that was clicked
    if(pluginList->ItemIndex == -1)
    {
    	return;
    }

    string pluginName = std_str(pluginList->Items->Strings[pluginList->ItemIndex]);
    Log()<<pluginName;

    string test = getPluginInfo(mInstanceH, pluginName.c_str());

    infoMemo->Clear();
    Log()<<test;

    //Populate plugin frame
	pluginCapsCB->Clear();
    pluginParasCB->Clear();

    RRStringArray* caps = getPluginCapabilities(mInstanceH, pluginName.c_str());

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

void __fastcall TMainF::clearMemoExecute(TObject *Sender)
{
	infoMemo->Clear();
}

void TMainF::UpdateNoisePanel()
{
	//This function is called if the Plugin list box is clicked
    string pluginName = std_str(pluginList->Items->Strings[pluginList->ItemIndex]);
	if(pluginName != "AddNoise")
    {
    	return;
    }

	//At this point we know the capabilities and parameters of this plugin
	RRParameter* sigma = getPluginParameter(mInstanceH, "AddNoise", "Sigma");

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
    Log()<<getPluginInfo(mInstanceH, pName.c_str());
}

//---------------------------------------------------------------------------
string TMainF::getCurrentPluginName()
{
    if(pluginList->ItemIndex == -1)
    {
    	return "";
    }

    string pluginName = std_str(pluginList->Items->Strings[pluginList->ItemIndex]);
	return pluginName;
}

string TMainF::getCurrentSelectedParameter()
{
    if(pluginParasCB->ItemIndex == -1)
    {
    	return "";
    }

    string pluginParaName = std_str(pluginParasCB->Items->Strings[pluginParasCB->ItemIndex]);
	return pluginParaName;
}

void __fastcall TMainF::pluginCBChange(TObject *Sender)
{

	if(pluginCapsCB == (TComboBox*)(Sender))
    {
        string pluginName = std_str(pluginList->Items->Strings[pluginList->ItemIndex]);

		//Change available parameters in the parsCB...
		pluginParasCB->Clear();

        string capa = std_str(pluginCapsCB->Items->Strings[pluginCapsCB->ItemIndex]);
        RRStringArray* paras = getPluginParameters(mInstanceH, pluginName.c_str(), capa.c_str());
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
        RRParameterHandle para = getPluginParameter(mInstanceH, getCurrentPluginName().c_str(), getCurrentSelectedParameter().c_str());
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
	setPluginParameter(mInstanceH, getCurrentPluginName().c_str(), getCurrentSelectedParameter().c_str(), std_str(paraEdit->Text).c_str());
}

void __fastcall TMainF::executePluginAExecute(TObject *Sender)
{
	executePlugin(mInstanceH, getCurrentPluginName().c_str());
}

void __fastcall TMainF::getLastErrorAExecute(TObject *Sender)
{
	Log()<<getLastError();
}

void __fastcall TMainF::loadModelAExecute(TObject *Sender)
{
	TAction* anAction = (TAction*)(Sender);

    if(anAction == loadModelA)
    {
		//load model
    	mLoadModelJob = loadSBMLFromFileJobEx(mInstanceH, mModel.c_str(), true);
		loadModelJobTimer->Enabled = true;
    }

    if(anAction == unLoadModelA)
    {
		//unLoad model
    	if(!unLoadModel(mInstanceH))
        {
            Log() << "Problem unloading model.";
            throw rr::Exception(getLastError());
        }
        Log() << "Model was unloaded.";
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
        }
        else
        {
    		Log () << "Busy...";
        }
    }
}

void __fastcall TMainF::loadModelAUpdate(TObject *Sender)
{
	if(!mUIIsStartingUp)
    {
		loadBtn->Action = isModelLoaded(mInstanceH) ? unLoadModelA : loadModelA;
    }
}

void TMainF::Plot(const rr::SimulationData& data)
{
    Chart1->RemoveAllSeries();

    //Fill out data for all series
    int nrOfSeries = data.cSize() -1; //First one is time
    StringList colNames = data.getColumnNames();
    vector<TLineSeries*> series;
    for(int i = 0; i < nrOfSeries; i++)
    {
        TLineSeries* aSeries = new TLineSeries(Chart1);
        aSeries->Title = colNames[i+1].c_str();
        aSeries->LinePen->Width = 3;
        series.push_back(aSeries);
        Chart1->AddSeries(aSeries);
    }

    for(int j = 0; j < data.rSize(); j++)
    {
        double xVal = data(j,0);

        for(int i = 0; i < nrOfSeries; i++)
        {
            double yData = data(j, i+1);
            series[i]->AddXY(xVal, yData);
        }
    }
    Chart1->Update();
}

void TMainF::Plot1D()
{
	if(!mData)
    {
    	return;
    }
    Chart1->RemoveAllSeries();

    //Fill out data for all series
    Log()<<"Plotting internal data";
    int nrOfSeries = mData->CSize -1; //First one is time

    vector<TLineSeries*> series;
    for(int i = 0; i < nrOfSeries; i++)
    {
        TLineSeries* aSeries = new TLineSeries(Chart1);
        if(mData->ColumnHeaders)
        {
        	aSeries->Title = mData->ColumnHeaders[i+1];
        }
        else
        {
        	aSeries->Title = "No title";
        }
        aSeries->LinePen->Width = 3;
        series.push_back(aSeries);
        Chart1->AddSeries(aSeries);
    }

    for(int row = 0; row < mData->RSize; row++)
    {
        double xVal = mData->Data[row*mData->CSize];	//Time
        for(int col = 0; col < nrOfSeries; col++)
        {
            double yData = mData->Data[row*mData->CSize + col + 1];
            series[col]->AddXY(xVal, yData);
        }
    }
    Chart1->Update();
}

void __fastcall TMainF::PlotAExecute(TObject *Sender)
{
	RoadRunner *aRR = (RoadRunner*) mInstanceH;

    if(aRR)
    {
        SimulationData data = aRR->getSimulationResult();
    	Plot(data);
    }
}

//---------------------------------------------------------------------------
void __fastcall TMainF::noiseSigmaEKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if(Key == VK_RETURN)
    {
    	double val = noiseSigmaE->Text.ToDouble();
		string msg = (setPluginParameter(mInstanceH, "AddNoise", "Sigma", (char*) &val)) ? "Sigma was updated" : "Failed to update Sigma";
        Log() << msg;
    }
}

void TMainF::configurePlugin(const string& pluginName)
{
	if(pluginName == "AddNoise")
    {
    	double val = noiseSigmaE->Text.ToDouble();
		string msg = (setPluginParameter(mInstanceH, "AddNoise", "Sigma", (char*) &val)) ? "Sigma was updated" : "Failed to update Sigma";
        Log() << msg;

		RoadRunner *aRR = (RoadRunner*) mInstanceH;
    	if(!aRR)
	    {
    		return;
    	}

    	SimulationData data = aRR->getSimulationResult();

    	//Excute the noise plugin
    	//Populate its data with data from roadrunner
        if(mData)
        {
        	freeResult(mData);
        }
    	mData = createRRResult(data);

        //Assign the dataPtr to the plugin
		msg = setPluginParameter(mInstanceH, "AddNoise", "Data", (char*) mData) ? "Sigma was updated" : "Failed to update Sigma";
        Log() << msg;

     }
}

void __fastcall TMainF::Button6Click(TObject *Sender)
{
	configurePlugin("AddNoise");

    //Fill out the data
    executePlugin(mInstanceH, "AddNoise");
    Plot1D();
}

void __fastcall TMainF::onSimulationStarted()
{
	Log()<<"A simulation was started";
}

void __fastcall TMainF::onSimulationFinished()
{
	Log()<<"Simulation was finishe.. Plot result";
    PlotAExecute(NULL);
}

void __fastcall TMainF::simFramesimBtnClick(TObject *Sender)
{
	simFrame->onSimulationFinished = onSimulationFinished;
	mSimulateModelJob = simFrame->simulate();
}


