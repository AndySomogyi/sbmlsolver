#include <vcl.h>
#pragma hdrstop
#include "rrRoadRunner.h"
#include "rrRoadRunnerData.h"
#include "rrUtils.h"
#include "MainForm.h"
#include "rrc_api.h"
#include "rrException.h"
#include "rrUtils.h"
#include "rrVCLUtils.h"
#include "../../../Wrappers/C/rrc_utilities.h"
#include "aboutForm.h"
#pragma package(smart_init)
#pragma link "mtkFloatLabeledEdit"
#pragma link "TSimulationFrame"
#pragma link "rrCapabilitiesFrame"
#pragma link "mtkIniFileC"
#pragma link "TLMFittingFrame"
#pragma link "TRegistryForm"
#pragma resource "*.dfm"
TMainF *MainF;
//---------------------------------------------------------------------------
using namespace rr;

using mtk::mtkBaseIniParameter;

__fastcall TMainF::TMainF(TComponent* Owner)
:
TRegistryForm(Owner, "RoadRunner-UI"),
mModel(""),
mModelsFolder("..\\models"),
mUIIsStartingUp(true),
mLogFileSniffer("", this),
mCurrentlySelectedPlugin(NULL),
mAddNoisePlugin(NULL),
mAppInfo(Application)
{

    mIniFile->SetFilePath(mAppInfo.mExePath);
	mIniFile->Load();

	mIniParas.SetIniFile(mIniFile->GetIniFile());
  	mIniParas.SetIniSection("GENERAL");
	SetupAndReadParameters();

    //Log to one memo
	simFrame->infoMemo = infoMemo;
	startupTimer->Enabled = true;
}

void __fastcall TMainF::clearMemoExecute(TObject *Sender)
{
	infoMemo->Clear();
}

void TMainF::SetupAndReadParameters()
{
	mIniParas.Insert( (mtkBaseIniParameter*) &mLowerPanelHeight.Setup(            "LOWER_PANEL_HEIGHT", 	            250, 			true));
	mIniParas.Insert( (mtkBaseIniParameter*) &mBottomLeftPanelWidth.Setup(        "BOTTOM_LEFT_PANEL_WIDTH",            450, 			true));
	mIniParas.Insert( (mtkBaseIniParameter*) &mModel.Setup(       				  "MODEL_FILE", 			            "Test_1.xml", 	true));
	mIniParas.Insert( (mtkBaseIniParameter*) &mLogLevel.Setup(       	  		  "LOG_LEVEL", 				            "INFO", 		true));

	mIniParas.Read();

    lowerPanel->Height 			= mLowerPanelHeight;
	bottomLeftPanelWidth->Width = mBottomLeftPanelWidth;
}

__fastcall	TMainF::~TMainF()
{
    mCurrentData.~RoadRunnerData();
	freeRRInstance(mRRI);

}

void __fastcall	TMainF::LogMessage()
{
    if(mLogString)
    {
        infoMemo->Lines->Add(mLogString->c_str());
        delete mLogString;

        // Signal to the data sink thread that we can now accept another message...
        mLogString = NULL;
    }
}

//---------------------------------------------------------------------------
void __fastcall TMainF::loadPluginsAExecute(TObject *Sender)
{
	if(!loadPlugins(mRRI))
	{
		ML() << "There were some problems loading plugins, please check the log file.";
	}

	//Populate list box with plugins
	RRStringArray* pluginNames = getPluginNames(mRRI);

    if(!pluginNames)
    {
    	ML()<<"No plugins to load...";
        return;
    }

	for(int i = 0; i < pluginNames->Count; i++)
	{
		pluginList->AddItem(pluginNames->String[i], NULL);
	}
	ML() << "Loaded plugins..";

    mAddNoisePlugin = getPlugin(mRRI, "AddNoise");
   	mLMPlugin       = getPlugin(mRRI, "Levenberg-Marquardt Minimization");

    Button1->Action = unloadPlugins;

	if(mLMPlugin)
    {
    	//Create the plugin frame
        mLMFrame = new TLMFittingFrame(MainPC);
        mLMFrame->Name = "LM";
        //Create a new tab
        TTabSheet* page = new TTabSheet(MainPC);
		page->Caption = vclstr(string(getPluginName(mLMPlugin)));
        mLMFrame->Parent = page;
        mLMFrame->Align = alClient;
        page->PageControl =  MainPC;
        mLMFrame->assignPluginHandle(mLMPlugin);
        mLMFrame->assignRRHandle(mRRI);
        mLMFrame->onFittingStarted  = onLMFittingStarted;
        mLMFrame->onFittingFinished = onLMFittingFinished;
        mLMFrame->infoMemo = infoMemo;
        mLMFrame->assignChart(mFittingChart);
        MainPC->TabIndex = MainPC->PageCount -1;
        mLMFrame->setModelsFolder(mModelsFolder);
    }

    freeStringArray(pluginNames);
}

void __fastcall TMainF::executePluginAExecute(TObject *Sender)
{
	executePlugin(mCurrentlySelectedPlugin);
}

void __fastcall TMainF::getLastErrorAExecute(TObject *Sender)
{
	ML()<<getLastError();
}

void __fastcall TMainF::loadModelAExecute(TObject *Sender)
{
	TAction* anAction = (TAction*)(Sender);

    if(anAction == loadModelA)
    {
    	mModel  = rr::stdstr(modelDD->Items->Strings[modelDD->ItemIndex]);
        mModel += ".xml";
		//load model
    	mLoadModelJob = loadSBMLFromFileJobEx(mRRI, rr::joinPath(mModelsFolder, mModel).c_str(), true);
		loadModelJobTimer->Enabled = true;
    }

    if(anAction == unLoadModelA)
    {
		//unLoad model
    	if(!unLoadModel(mRRI))
        {
            ML() << "Problem unloading model.";
            throw rr::Exception(getLastError());
        }
        ML() << "Model was unloaded.";
    }
}

void __fastcall TMainF::PlotAExecute(TObject *Sender)
{
	Plot(mCurrentData);
}

void TMainF::Plot(const rr::RoadRunnerData& data)
{
    Chart1->RemoveAllSeries();

    //Fill out data for all series
    int nrOfSeries = data.cSize() -1; //First one has to be time
    StringList colNames = data.getColumnNames();
    vector<TLineSeries*> series;
    for(int i = 0; i < nrOfSeries; i++)
    {
        TLineSeries* aSeries = new TLineSeries(Chart1);
        aSeries->Title = colNames[i+1].c_str();
        aSeries->LinePen->Width = 3;
        aSeries->Pointer->Style = psCircle;
        aSeries->Pointer->Visible = true;
        series.push_back(aSeries);
        Chart1->AddSeries(aSeries);
    }

    for(int j = 0; j < data.rSize(); j++)
    {
        double xVal = data(j,0);

        for(int i = 0; i < nrOfSeries; i++)
        {
            double yData = data(j, i+1);
            if(!isNaN(yData))
            {
            	series[i]->AddXY(xVal, yData);
            }
        }
    }
    Chart1->Update();
}

void TMainF::configurePlugin(RRPluginHandle plugin)
{
    //For convenience
    RoadRunner *aRR = (RoadRunner*) mRRI;
    if(!aRR)
    {
        return;
    }

	if(plugin == mAddNoisePlugin)
    {
    	string val = rr::stdstr(noiseSigmaE->Text);
		string msg = (setPluginParameter(mAddNoisePlugin, "Sigma", val.c_str())) ? "Sigma was updated" : "Failed to update Sigma";
        ML() << msg;

        assignCallbacks(mAddNoisePlugin, addNoiseStartedCB, addNoiseFinishedCB, this);
     }
}

void __fastcall TMainF::addNoiseBtnClick(TObject *Sender)
{
	configurePlugin(mAddNoisePlugin);
    executePluginEx(mAddNoisePlugin, (void*) &mCurrentData);
}

void __fastcall TMainF::onSimulationStarted()
{
	ML()<<"A simulation was started";
}

void __fastcall TMainF::onSimulationFinished()
{
	ML()<<"Simulation was finishe.. Plot result";
	RoadRunner *aRR = (RoadRunner*) mRRI;
    mCurrentData = aRR->getSimulationResult();
    PlotAExecute(NULL);

    //Switch page control to show new data..
    PC1->TabIndex = 0;
}

void __fastcall TMainF::onAddNoiseStarted()
{
	ML()<<"Add noise started. Plot result";
}

void __fastcall TMainF::onAddNoiseFinished()
{
	ML()<<"Add noise finished.. Plot result";
    Plot(mCurrentData);
}

void __fastcall TMainF::simFramesimBtnClick(TObject *Sender)
{
	simFrame->onSimulationFinished = onSimulationFinished;
	mSimulateModelJob = simFrame->simulate();
}

void __stdcall TMainF::addNoiseStartedCB(void *UserData)
{
    TMainF *pThis = (TMainF*) UserData;
    TThread::Synchronize(NULL, pThis->onAddNoiseStarted);
}

void __stdcall TMainF::addNoiseFinishedCB(void *UserData)
{
    TMainF *pThis = (TMainF*) UserData;
    TThread::Synchronize(NULL, pThis->onAddNoiseFinished);
}

void __fastcall TMainF::getCapabilitiesAExecute(TObject *Sender)
{
	RRStringArray* list = getListOfCapabilities(mRRI);
    StringList caps(list);
   	ML()<<"Capabilities";
    for(int i = 0; i < caps.Count(); i++)
    {
	   	ML()<<caps[i];
    }
}

void __fastcall TMainF::getCapabilitiesAsXMLAExecute(TObject *Sender)
{
	char* caps = getCapabilities(mRRI);
	ML()<<caps;
}

//---------------------------------------------------------------------------
void __fastcall TMainF::getAllSymbolsExecute(TObject *Sender)
{
	RRList* list = getAvailableTimeCourseSymbols(mRRI);

    if(list)
    {
    	char* text = listToString(list);
    	ML()<<"Time Course Symbols";
        ML()<<text;
        freeText(text);
        freeRRList(list);
    }
}

void __fastcall TMainF::modelDDChange(TObject *Sender)
{
    unLoadModelA->Execute();
    loadModelA->Execute();
}

void __fastcall TMainF::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	if(mLogFileSniffer.isActive())
    {
    	CanClose = false;
        ShutDownTimer->Enabled = true;
    }
}

void __fastcall TMainF::ShutDownTimerTimer(TObject *Sender)
{
	ShutDownTimer->Enabled = false;
	mLogFileSniffer.shutDown();
    Close();
}

void __fastcall TMainF::onLMFittingStarted()
{
	ML()<<"LM Fitting was started..";
//    string result = fullSpaceFitFrame->getResult();
	ML()<<"==================================";
//    ML()<<result;
	ML()<<"==================================";
//    //Switch page control to show new data..
//    PC1->TabIndex = 1;
}

void __fastcall TMainF::onLMFittingFinished()
{
	ML()<<"LM Fitting was finished..";
//    string result = fullSpaceFitFrame->getResult();
	ML()<<"==================================";
//    ML()<<result;
	ML()<<"==================================";
//    //Switch page control to show new data..
//    PC1->TabIndex = 1;
}

//---------------------------------------------------------------------------
void __fastcall TMainF::FormClose(TObject *Sender, TCloseAction &Action)
{
	mLowerPanelHeight 		= lowerPanel->Height;
	mBottomLeftPanelWidth 	= bottomLeftPanelWidth->Width;
	mIniParas.Write();
    mIniFile->Save();
}

void __fastcall TMainF::saveCurrentDataAExecute(TObject *Sender)
{
	//Open a file save dialog
    if(!saveRRDataDialog->Execute())
    {
    	return;
    }
    	//Get the filename
    string fileName = stdstr(saveRRDataDialog->FileName);
    if(rr::fileExists(fileName))
    {
        if(MessageDlg("The file exists. OverWrite?", mtInformation, TMsgDlgButtons() << mbYes<<mbCancel, 0) == mbCancel)
        {
            return;
        }
    }

    //Start writing...
    //Create fileName
    if(getFileExtension(fileName) != "dat")
    {
        fileName = changeFileExtensionTo(fileName, "dat");
    }

    mCurrentData.allocateWeights();

    ofstream theFile(fileName.c_str());
    theFile << mCurrentData;
    theFile.close();

    ML()<<"The file "<<fileName<<" was written to the file system";
}

void __fastcall TMainF::openDataAExecute(TObject *Sender)
{
	//Open a file open dialog
    if(!OpenDialog1->Execute())
    {
    	return;
    }
    	//Get the filename
    string fileName = stdstr(OpenDialog1->FileName);

    //Start reading ...
    //Create fileName
    ifstream theFile(fileName.c_str());

    theFile >> mCurrentData;
    theFile.close();
    ML()<<"The file "<<fileName<<" was read";
    PlotA->Execute();

}

//---------------------------------------------------------------------------
void __fastcall TMainF::exitAExecute(TObject *Sender)
{
	Close();
}



void __fastcall TMainF::logLevelCBChange(TObject *Sender)
{
	//Change the loglevel
    if(logLevelCB->ItemIndex != -1)
    {
    	mLogLevel = stdstr(logLevelCB->Items->Strings[logLevelCB->ItemIndex]);
        setLogLevel(mLogLevel.GetValueAsString().c_str());
        ML()<<"The log level was changed to: "<<mLogLevel;
    }
}

void __fastcall TMainF::AboutExecute(TObject *Sender)
{
	TrrAboutForm* f = new TrrAboutForm(mRRI, this);
    f->ShowModal();

    delete f;
}


