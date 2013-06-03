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
#include "rrc_cpp_support.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "mtkFloatLabeledEdit"
#pragma link "mtkIntLabeledEdit"
#pragma resource "*.dfm"
TLMFittingFrame *LMFittingFrame;

using namespace rr;

__fastcall TLMFittingFrame::TLMFittingFrame(TComponent* Owner)
	:
TFrame(Owner),
infoMemo(NULL),
mChart(NULL)
{
    mColors.push_back(clRed);
    mColors.push_back(clBlue);
    mColors.push_back(clGreen);
    mRRI = createRRInstance();
}

void TLMFittingFrame::setModelsFolder(const string& folder)
{
	mModelsFolder = folder;
    populateFileSet(folder, mModelFiles);
    populateDropDown(mModelFiles, modelDD);
    modelDD->ItemIndex = 0;
}

void TLMFittingFrame::assignRRHandle(RRHandle aHandle)
{
//	mRRI = aHandle;
}

void TLMFittingFrame::assignChart(TChart* aChart)
{
	mChart = aChart;
}

void TLMFittingFrame::assignPluginHandle(RRPluginHandle aHandle)
{
	mPlugin = aHandle;
}

void TLMFittingFrame::populate()
{
	string modelName = getModelName(mRRI);
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
            ML()<<"There was a problem with parameter: "<<parName;
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
   	ML()<<"Parameter "<<para->getName()<<" = "<<para->getValue();
	paraEdit->EditLabel->Caption = vclstr(para->getName());
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
    	ML()<<"Error: Please select a parameter to fit";
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

    //Set input data to fit to
    RRData* rrData = rrc::createRRData(MainF->mCurrentData);
	setInputData(mPlugin, rrData);

    //Add species to minimization data structure.. The species are defined in the input data
    StringList modelSpecies = getCheckedItems(speciesList);
    setMinimizationModelDataSelectionList(minData, modelSpecies.AsString().c_str());

    StringList checkedSpecies(rrData->ColumnHeaders, rrData->CSize);
    if(checkedSpecies.indexOf("time") != -1)
    {
    	checkedSpecies.removeAt(checkedSpecies.indexOf("time"));
    }

    setMinimizationExperimentalDataSelectionList(minData, checkedSpecies.AsString().c_str());
    freeRRData(rrData);

    //Requirement!! the modDataSelection list must be equal or larger than the expSelectionlist!
    if(checkedSpecies.Count() > modelSpecies.Count())
    {
    	Log(lError)<<"The minimization engine requires modelspecies to be equal or larger than selected exp species!";
        Log(lError)<<"Exiting minimization.....";
        return;
    }

    string strVal = getTempFolder(mRRI);
	string msg	  = setPluginParameter(mPlugin, "TempFolder", strVal.c_str())
    		? "Updated tempfolder" : "Failed to set tempFolder";
    		ML() << msg;

    strVal = getSBML(mRRI);
    msg = setPluginParameter(mPlugin, "SBML", strVal.c_str())
    		? "Assigned SBML" : "Failed assigning SBML";
    		ML() << msg;

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
	ML()<<"Full space fitting was started";
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
	ML()<<"Levenberg-Marquard fitting was finished";
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

    MinimizationData& minData 			= *(MinimizationData*) getParameterValueAsPointer(minDataParaHandle);
    RoadRunnerData obsData 				= minData.getObservedData();
	RoadRunnerData modelData 			= minData.getModelData();
	RoadRunnerData resData	 			= minData.getResidualsData();

    if(mChart == NULL)
    {
    	Log(lInfo)<<"No chart is assigned to the minimization frame...";
    	return;
    }

    ClearChart();
	vector<TLineSeries*> obsDataSeries;
	vector<TLineSeries*> resDataSeries;
	vector<TLineSeries*> modDataSeries;

  	StringList expSelList 	= minData.getExperimentalDataSelectionList();
  	StringList modelSelList = minData.getModelDataSelectionList();

    //The user may have generated data for species that were not fitted. These are in modelSelList.
    for(int i = 0; i < modelSelList.Count(); i++)
    {
	    TColor color = (i < mColors.size() ) ? mColors[i] : clPurple;

		//ModelData
    	TLineSeries* aSerie;
        aSerie = new TLineSeries(mChart);
		modDataSeries.push_back(aSerie);
        aSerie->Color = color;
        aSerie->Pen->Width = 3;
		mChart->AddSeries(aSerie);
        aSerie->Title = vclstr(modelSelList[i] + "-Model");
    }

    for(int i = 0; i < expSelList.Count(); i++)
    {
	    TColor color = (i < mColors.size() ) ? mColors[i] : clPurple;

    	TLineSeries* aSerie;
        //Experimental data
    	aSerie = new TLineSeries(mChart);
        aSerie->Color = color;
		aSerie->Pointer->Visible = true;
        aSerie->Pointer->Style  = psDownTriangle;
        aSerie->Pointer->Size = 3;
        aSerie->LinePen->Visible = false;
		obsDataSeries.push_back(aSerie);
		mChart->AddSeries(aSerie);
        aSerie->Title = vclstr(expSelList[i] + "-Observed");

    }
    for(int i = 0; i < expSelList.Count(); i++)
    {
	    TColor color = (i < mColors.size() ) ? mColors[i] : clPurple;

    	TLineSeries* aSerie;

		//Residual data
    	aSerie = new TLineSeries(mChart);
		resDataSeries.push_back(aSerie);
        aSerie->Color = color;
        aSerie->Pointer->Style  = psDiamond;
        aSerie->Pointer->Size = 2;
		aSerie->Pointer->Visible = true;
        aSerie->LinePen->Visible = false;
		mChart->AddSeries(aSerie);
        aSerie->Title = vclstr(expSelList[i] + "-Residual");
	}

    //Plot the data..
    PlotData(obsData, obsDataSeries, expSelList);
    PlotData(resData, resDataSeries, expSelList);
    PlotData(modelData, modDataSeries, modelSelList);

	for(int i = 0; i < modDataSeries.size(); i++)
    {
		mChart->AddSeries(modDataSeries[i]);
    }

	for(int i = 0; i < obsDataSeries.size(); i++)
    {
		mChart->AddSeries(obsDataSeries[i]);
		mChart->AddSeries(resDataSeries[i]);
    }

	mChart->Legend->ResizeChart = false;
    mChart->Update();
}

//This selection list do not contain time... but the data does!
void __fastcall TLMFittingFrame::PlotData(RoadRunnerData& rrData, vector<TLineSeries*>& series, StringList& selList)
{
    for(int i = 0; i < rrData.rSize(); i++)
    {
        for(int sel = 0; sel < selList.Count(); sel++)
        {
            double xVal = rrData(i, 0);
            double yVal = rrData(i, sel + 1);
            if(!isNaN(yVal))
            {
            	series[sel]->AddXY(xVal, yVal);
            }
        }
    }
}

//---------------------------------------------------------------------------
void __fastcall TLMFittingFrame::logResultAExecute(TObject *Sender)
{
	char*  res = getPluginResult(mPlugin);
    if(res)
    {
		ML()<< res;
    }
    else
    {
    	ML()<<"No result available..";
    }
}

void TLMFittingFrame::ClearChart()
{
	//Clear the chart
    mChart->RemoveAllSeries();
    mChart->ClearChart();
	mChart->View3D = false;
}
void __fastcall TLMFittingFrame::modelDDChange(TObject *Sender)
{
	string model = stdstr(modelDD->Items->Strings[modelDD->ItemIndex]);

    model += ".xml";
    //load model
    mLoadModelJob = loadSBMLFromFileJobEx(mRRI, rr::joinPath(mModelsFolder, model).c_str(), true);
    loadModelJobTimer->Enabled = true;
}

//---------------------------------------------------------------------------
void __fastcall TLMFittingFrame::loadModelJobTimerTimer(TObject *Sender)
{
	if(mLoadModelJob)
    {
    	if(isJobFinished(mLoadModelJob))
        {
	        loadModelJobTimer->Enabled = false;
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

            populate();

        }
        else
        {
    		ML() << "Busy...";
        }
    }
}
//---------------------------------------------------------------------------

