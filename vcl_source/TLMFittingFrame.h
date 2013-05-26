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
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>

using namespace rr;
using namespace rrc;
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
	TChart *Chart1;
	TCheckListBox *speciesList;
	TPanel *Panel1;
	TSplitter *Splitter1;
	TGroupBox *GroupBox2;
	TGroupBox *GroupBox3;
	TGroupBox *GroupBox4;
	TSplitter *Splitter3;
	TGroupBox *GroupBox1;
	TLabel *nameLbl;
	TLineSeries *Series1;
	void __fastcall paraListClick(TObject *Sender);
	void __fastcall executeBtnClick(TObject *Sender);
	void __fastcall logResultAExecute(TObject *Sender);

    private:	// User declarations
        RRHandle			                   	mRRI;
        RRPluginHandle							mPlugin;
        RRJobHandle			                   	mSimJobH;
		Parameters								mParameters;	//Parameters for the current model

        static void 		                    ThreadEnterCB(void *UserData);
        static void 		                    ThreadExitCB(void *UserData);

		//If we want to interact with the C threads
        static void 			__stdcall  		fittingStartedCB(void *UserData);
        static void 			__stdcall  		fittingFinishedCB(void *UserData);

        void 					__fastcall      fittingStarted();
        void 				   	__fastcall      fittingFinished();

	public:		// User declarations
                        		__fastcall      TLMFittingFrame(TComponent* Owner);
        TMemo			               		   *infoMemo;	//For the logger

        void								    loadParameterList();
        void								    loadSpeciesList();
		void 								    assignRRHandle(RRHandle aHandle);
		void 								    assignPluginHandle(RRPluginHandle aHandle);
	    string									getResult();
		void 								    populate();
		//Assign in parent
        TOnEvent			   				    onFittingStarted;
        TOnEvent	   						    onFittingFinished;

};
//---------------------------------------------------------------------------
extern PACKAGE TLMFittingFrame *LMFittingFrame;
//---------------------------------------------------------------------------
#endif
