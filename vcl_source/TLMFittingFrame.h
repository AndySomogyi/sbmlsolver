#ifndef TLMFittingFrameH
#define TLMFittingFrameH
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "mtkFloatLabeledEdit.h"
#include "mtkIntLabeledEdit.h"
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include "../../Wrappers/C/rrc_types.h"
#include <Vcl.CheckLst.hpp>
#include "rrStringList.h"
#include "rrParameters.h"
#include "rrRoadRunnerData.h"
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <set>
using namespace rr;
using namespace rrc;
using std::set;
typedef void __fastcall (__closure *TOnEvent)();

//---------------------------------------------------------------------------
class TLMFittingFrame : public TFrame
{
__published:	// IDE-managed Components
	TActionList *ActionList1;
	TButton *executeBtn;
	TCheckListBox *paraList;
	TAction *logResultA;
	mtkFloatLabeledEdit *paraEdit;
	TCheckListBox *speciesList;
	TPanel *Panel1;
	TGroupBox *GroupBox2;
	TGroupBox *GroupBox3;
	TGroupBox *GroupBox4;
	TSplitter *Splitter3;
	TButton *Button1;
	TAction *saveResult;
	TButton *Button2;
	TPanel *Panel2;
	TGroupBox *GroupBox5;
	TGroupBox *Model;
	TComboBox *modelDD;
	TTimer *loadModelJobTimer;
	void __fastcall paraListClick(TObject *Sender);
	void __fastcall executeBtnClick(TObject *Sender);
	void __fastcall logResultAExecute(TObject *Sender);
	void __fastcall modelDDChange(TObject *Sender);
	void __fastcall loadModelJobTimerTimer(TObject *Sender);

    private:	// User declarations
		string									mModelsFolder; 	//Where to look for models
	    set<string> 							mModelFiles;	//Holds model filenames in models folder

        RRHandle			                   	mRRI;
        RRPluginHandle							mPlugin;
	    RRJobHandle	                        	mLoadModelJob;
//        RRJobHandle			                   	mSimJobH;

		Parameters								mParameters;	//Parameters for the current model
		TChart*									mChart;			//Chart to show minimization results

        static void 		                    ThreadEnterCB(void *UserData);
        static void 		                    ThreadExitCB(void *UserData);

		//If we want to interact with the C threads
        static void 			__stdcall  		fittingStartedCB(void *UserData);
        static void 			__stdcall  		fittingFinishedCB(void *UserData);

        void 					__fastcall      fittingStarted();
        void 				   	__fastcall      fittingFinished();
        vector<TColor> 							mColors;
        void									ClearChart();
		void 					__fastcall 		PlotData(RoadRunnerData& obsData, vector<TLineSeries*>& series, StringList& expSelList);

	public:		// User declarations
                        		__fastcall      TLMFittingFrame(TComponent* Owner);
        TMemo			               		   *infoMemo;	//For the logger

        void								    loadParameterList();
        void								    loadSpeciesList();
		void 								    assignChart(TChart* aChart);
		void 								    assignRRHandle(RRHandle aHandle);
		void 								    assignPluginHandle(RRPluginHandle aHandle);
	    string									getResult();
		void 								    populate();
        void									setModelsFolder(const string& folder);

		//Assign in parent
        TOnEvent			   				    onFittingStarted;
        TOnEvent	   						    onFittingFinished;

};
//---------------------------------------------------------------------------
extern PACKAGE TLMFittingFrame *LMFittingFrame;
//---------------------------------------------------------------------------
#endif
