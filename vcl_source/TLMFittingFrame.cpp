#include <vcl.h>
#pragma hdrstop
#include <Classes.hpp>
#include "rrException.h"
#include "TLMFittingFrame.h"
#include "rrNewArrayList.h"
#include "rrUtils.h"
#include "rrMemoLogger.h"
#include "rrc_api.h"
#include "rrEndUserUtils.h"
#include "rrVCLUtils.h"
#include "MainForm.h"
#include "rrMinimizationData.h"
#include "rrc_api.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "mtkFloatLabeledEdit"
#pragma link "mtkIntLabeledEdit"
#pragma resource "*.dfm"
TLMFittingFrame *LMFittingFrame;

using namespace rr;
//---------------------------------------------------------------------------
__fastcall TLMFittingFrame::TLMFittingFrame(TComponent* Owner)
	:
TFrame(Owner),
infoMemo(NULL)
{}

void TLMFittingFrame::assignRRHandle(RRHandle aHandle)
{
	mRRI = aHandle;
}

void TLMFittingFrame::assignPluginHandle(RRPluginHandle aHandle)
{
	mPlugin = aHandle;
}

void TLMFittingFrame::populate()
{
	string modelName = getModelName(mRRI);
	nameLbl->Caption = vclstr(modelName);

	loadSpeciesList();
	loadParameterList();
}

void TLMFittingFrame::loadParameterList()
{
    if(!mRRI)
    {
        return;
    }
    paraList->Clear();
    mParameters.clear();
    RRStringArrayHandle cSymbolsIDs = getGlobalParameterIds(mRRI);
    StringList symbols = convertCStringArray(cSymbolsIDs);

    for(int i = 0; i < symbols.Count(); i++)
    {
        string parName = symbols[i];
        double value;
        if(!getValue(mRRI, parName.c_str(), &value))
        {
            Log()<<"There was a problem with parameter: "<<parName;
        }
        else
        {
            mParameters.add(new Parameter<double>(parName, value, ""));
        }
    }
    for(int i = 0; i < mParameters.count(); i++)
    {
       int index = paraList->Items->Add(mParameters[i]->getName().c_str());
       paraList->Items->Objects[i] = (TObject*) mParameters[i];
       paraList->Checked[index] = true;
    }
}

void TLMFittingFrame::loadSpeciesList()
{
    if(!mRRI)
    {
    	return;
    }

    speciesList->Clear();

    RRStringArrayHandle cSpecies = getFloatingSpeciesIds(mRRI);
    StringList species = convertCStringArray(cSpecies);

    for(int i = 0; i < species.Count(); i++)
    {
		speciesList->Items->Add(species[i].c_str());
        speciesList->Checked[i] = true;
    }
}

void __fastcall TLMFittingFrame::paraListClick(TObject *Sender)
{
	//Get the currently selected parameter
    int index = paraList->ItemIndex;
    if(index == -1)
    {
    	return;
    }

    Parameter<double>* para = (Parameter<double>*) paraList->Items->Objects[paraList->ItemIndex];
   	Log()<<"Parameter "<<para->getName()<<" = "<<para->getValue();
    paraEdit->FNumber = para->getValuePointer();
	paraEdit->Update();
}

void __fastcall TLMFittingFrame::executeBtnClick(TObject *Sender)
{
	int checkedItem = -1;
	for(int i = 0; i < paraList->Count; i++)
    {
        if(paraList->Checked[i])
        {
        	checkedItem = i;
            break;
        }
    }

    if(checkedItem == -1)
    {
    	Log()<<"Error: Please select a parameter to fit";
        return;
    }

    //Reset on each run
	resetPlugin(mPlugin);

    //Get the plugins minimizationData handle
    RRParameterHandle minDataParaHandle = getPluginParameter(mPlugin, "MinData", NULL);
    if(!minDataParaHandle)
    {
    	throw("bad....");
    }

    RRMinimizationDataHandle minData = getParameterValueAsPointer(minDataParaHandle);

	//Add parameters to minimization structure
    for(int i = 0; i < mParameters.count(); i++)
    {
    	Parameter<double>* para = (Parameter<double>*) mParameters[i];
    	addDoubleParameter(minData, para->getName().c_str(), para->getValue());
    }

    //Add selected species to minimization data structure
    StringList checkedSpecies = getCheckedItems(speciesList);
    setMinimizationSelectionList(minData, checkedSpecies.AsString().c_str());

    //Set input data to fit to
    RRData* rrData = rrc::createRRData(MainF->mCurrentData);
	setInputData(mPlugin, rrData);
    freeRRData(rrData);

    string strVal;
	string msg;
    strVal = getTempFolder(mRRI);

    msg = setPluginParameter(mPlugin, "TempFolder", strVal.c_str())
    		? "Updated tempfolder" : "Failed to set tempFolder";
    		Log() << msg;

    strVal = getSBML(mRRI);
    msg = setPluginParameter(mPlugin, "SBML", strVal.c_str())
    		? "Assigned SBML" : "Failed assigning SBML";
    		Log() << msg;

	assignCallbacks(mPlugin, fittingStartedCB, fittingFinishedCB, this);
    TMainF *mainForm = (TMainF*) this->Owner;

    //This call returns before the result has been created, non blocking.
    //The result is returned in the callback assigned in the configurePlugin function
    executePluginEx(mPlugin, (void*) &(mainForm->mCurrentData));
}

void __stdcall TLMFittingFrame::fittingStartedCB(void *UserData)
{
    TLMFittingFrame *pThis = (TLMFittingFrame*) UserData;
    TThread::Synchronize(NULL, pThis->fittingStarted);
}

void __stdcall TLMFittingFrame::fittingFinishedCB(void *UserData)
{
    TLMFittingFrame *pThis = (TLMFittingFrame*) UserData;
    TThread::Synchronize(NULL, pThis->fittingFinished);
}

void __fastcall TLMFittingFrame::fittingStarted()
{
	Log()<<"Full space fitting was started";
    if(onFittingStarted)
    {
        onFittingStarted();
    }
}

string TLMFittingFrame::getResult()
{
	return getPluginResult(mPlugin);
}

void __fastcall TLMFittingFrame::fittingFinished()
{
	Log()<<"Full space fitting was finished";
    if(onFittingFinished)
    {
        onFittingFinished();
    }

    //Get the plugins minimizationData handle
    RRParameterHandle minDataParaHandle = getPluginParameter(mPlugin, "MinData", NULL);
    if(!minDataParaHandle)
    {
    	throw("bad....");
    }

    RRMinimizationDataHandle _minData 	= getParameterValueAsPointer(minDataParaHandle);
    MinimizationData& minData 			= *(MinimizationData*) _minData;
    RoadRunnerData obsData 				= minData.getObservedData();
	RoadRunnerData modelData 			= minData.getModelData();
	RoadRunnerData resData	 			= minData.getResidualsData();

	//Clear the chart
    Chart1->RemoveAllSeries();
    Chart1->ClearChart();
	Chart1->View3D = false;
    //There will be 3 x nrOfSpecies series
	vector<TLineSeries*> modDataSeries;
	vector<TLineSeries*> obsDataSeries;
	vector<TLineSeries*> resDataSeries;

    vector<TColor> colors;
    colors.push_back(clRed);
    colors.push_back(clBlue);
    colors.push_back(clGreen);
  	StringList selList = minData.getSelectionList();
    int nrOfSpecies = selList.Count();
    for(int i = 0; i < selList.Count(); i++)
    {
	    TColor color;
    	if(i < colors.size())
        {
			color = colors[i];
        }
        else
        {
        	color = clPurple;
        }

		//ModelData
    	TLineSeries* aSerie;
        aSerie = new TLineSeries(Chart1);
		modDataSeries.push_back(aSerie);
        aSerie->Color = color;
        aSerie->Pen->Width = 5;
		Chart1->AddSeries(aSerie);
        aSerie->Title = vclstr(selList[i] + "-Model");

        //Experimental data
    	aSerie = new TLineSeries(Chart1);
        aSerie->Color = color;
		aSerie->Pointer->Visible = true;
        aSerie->Pointer->Style  = psDownTriangle;
        aSerie->Pointer->Size = 3;
        aSerie->LinePen->Visible = false;
		obsDataSeries.push_back(aSerie);
		Chart1->AddSeries(aSerie);
        aSerie->Title = vclstr(selList[i] + "-Observed");

		//Residual data
    	aSerie = new TLineSeries(Chart1);
		resDataSeries.push_back(aSerie);
        aSerie->Color = color;
        aSerie->Pointer->Style  = psDiamond;
        aSerie->Pointer->Size = 2;
		aSerie->Pointer->Visible = true;
        aSerie->LinePen->Visible = false;
		Chart1->AddSeries(aSerie);
        aSerie->Title = vclstr(selList[i] + "-Residual");
    }

    //Plot the input, model and residual data..
    for(int i = 0; i < modelData.rSize(); i++)
    {
        for(int sel = 0; sel < nrOfSpecies; sel++)
        {
            double xVal;
            double yVal;
            xVal = modelData(i,0);
            yVal = modelData(i, sel + 1);
            modDataSeries[sel]->AddXY(xVal, yVal);

            xVal = obsData(i,0);
            yVal = obsData(i, sel + 1);
            obsDataSeries[sel]->AddXY(xVal, yVal);

            xVal = resData(i,0);
            yVal = resData(i, sel + 1);
            resDataSeries[sel]->AddXY(xVal, yVal);
        }
    }

	for(int i = 0; i < modDataSeries.size(); i++)
    {
		Chart1->AddSeries(modDataSeries[i]);
		Chart1->AddSeries(obsDataSeries[i]);
		Chart1->AddSeries(resDataSeries[i]);
    }

	Chart1->Legend->ResizeChart = false;
    Chart1->Update();
}

//---------------------------------------------------------------------------
void __fastcall TLMFittingFrame::logResultAExecute(TObject *Sender)
{
	Log()<< getPluginResult(mPlugin);
}
