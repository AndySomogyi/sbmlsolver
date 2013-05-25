#include <vcl.h>
#pragma hdrstop
#include "rrRoadRunner.h"
#include "rrSimulationData.h"
#include "rrUtils.h"
#include "MainForm.h"
#include "rrc_api.h"
#include "rrException.h"
#include "rrVCLUtils.h"
#include "../../../Wrappers/C/rrc_support.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "mtkFloatLabeledEdit"
#pragma link "TFullSpaceFittingFrame"
#pragma link "TSimulationFrame"
#pragma resource "*.dfm"
TMainF *MainF;

__fastcall TMainF::TMainF(TComponent* Owner)
:
TRegistryForm(Owner, "RoadRunnerPluginTester"),
mModel(""),
mModelsFolder("r:\\models"),
mUIIsStartingUp(true),
mLogFileSniffer("", this),
mCurrentlySelectedPlugin(NULL),
mAddNoisePlugin(NULL),
mMinimizePlugin(NULL)
{
    //Log to one memo
	simFrame->infoMemo = infoMemo;
	fullSpaceFitFrame->infoMemo = infoMemo;
	startupTimer->Enabled = true;
}

void __fastcall TMainF::clearMemoExecute(TObject *Sender)
{
	infoMemo->Clear();
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

void __fastcall TMainF::executePluginAExecute(TObject *Sender)
{
	executePlugin(mCurrentlySelectedPlugin);
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
    	mModel = stdstr(modelDD->Items->Strings[modelDD->ItemIndex]);
        mModel +=".xml";
		//load model
    	mLoadModelJob = loadSBMLFromFileJobEx(mRRI, rr::JoinPath(mModelsFolder, mModel).c_str(), true);
		loadModelJobTimer->Enabled = true;
    }

    if(anAction == unLoadModelA)
    {
		//unLoad model
    	if(!unLoadModel(mRRI))
        {
            Log() << "Problem unloading model.";
            throw rr::Exception(getLastError());
        }
        Log() << "Model was unloaded.";
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
            series[i]->AddXY(xVal, yData);
        }
    }
    Chart1->Update();
}

void __fastcall TMainF::PlotAExecute(TObject *Sender)
{
//    if(aRR)
    {

    	Plot(mCurrentData);
    }
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
    	double val = noiseSigmaE->Text.ToDouble();
		string msg = (setPluginParameter(mAddNoisePlugin, "Sigma", (char*) &val)) ? "Sigma was updated" : "Failed to update Sigma";
        Log() << msg;

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
	Log()<<"A simulation was started";
}

void __fastcall TMainF::onSimulationFinished()
{
	Log()<<"Simulation was finishe.. Plot result";
	RoadRunner *aRR = (RoadRunner*) mRRI;
    mCurrentData = aRR->getSimulationResult();
    PlotAExecute(NULL);
}

void __fastcall TMainF::onAddNoiseStarted()
{
	Log()<<"Add noise started. Plot result";
}

void __fastcall TMainF::onAddNoiseFinished()
{
	Log()<<"Add noise finished.. Plot result";
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

void __fastcall TSimulateFrame::simulationStarted()
{
	Log()<<"Simulation was started at: " << getTime();
}

void __fastcall TSimulateFrame::simulationFinished()
{
	Log()<<"Simulation was finished at: "<< getTime();
    if(onSimulationFinished)
    {
        onSimulationFinished();
    }
}

void __fastcall TMainF::getCapabilitiesAExecute(TObject *Sender)
{
	char* list = getCapabilities(mRRI);
    if(list)
    {
    	Log()<<"Capabilities";
        Log()<<list;
    }
}

//---------------------------------------------------------------------------
void __fastcall TMainF::getAllSymbolsExecute(TObject *Sender)
{
	RRList* list = getAvailableTimeCourseSymbols(mRRI);

    if(list)
    {
    	char* text = listToString(list);
    	Log()<<"Time Course Symbols";
        Log()<<text;
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


