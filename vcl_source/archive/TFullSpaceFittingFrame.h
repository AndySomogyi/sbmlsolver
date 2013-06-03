#ifndef TFullSpaceFittingFrameH
#define TFullSpaceFittingFrameH
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "mtkFloatLabeledEdit.h"
#include "mtkIntLabeledEdit.h"
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include "rrc_types.h"
#include <Vcl.CheckLst.hpp>
#include "rrStringList.h"

using namespace rr;
using namespace rrc;
typedef void __fastcall (__closure *TOnEvent)();

//---------------------------------------------------------------------------
class TFullSpaceFittingFrame : public TFrame
{
__published:	// IDE-managed Components
	TGroupBox *sweepE;
	TActionList *ActionList1;
	TButton *executeBtn;
	TCheckListBox *paraList;
	mtkIntLabeledEdit *stepsE;
	mtkFloatLabeledEdit *sweepRangeE;
	TButton *Button1;
	TAction *logResultA;
	void __fastcall paraListClick(TObject *Sender);
	void __fastcall executeBtnClick(TObject *Sender);
	void __fastcall logResultAExecute(TObject *Sender);
	void __fastcall paraListClickCheck(TObject *Sender);

    private:	// User declarations
        RRHandle			                   	mRRI;
        RRPluginHandle							mPlugin;
        RRJobHandle			                   	mSimJobH;

        static void 		                    ThreadEnterCB(void *UserData);
        static void 		                    ThreadExitCB(void *UserData);

		//If we want to interact with the C threads
        static void 			__stdcall  		fsfStartedCB(void *UserData);
        static void 			__stdcall  		fsfFinishedCB(void *UserData);

        void 					__fastcall      fittingStarted();
        void 				   	__fastcall      fittingFinished();

	public:		// User declarations
                        		__fastcall      TFullSpaceFittingFrame(TComponent* Owner);
        TMemo			               		   *infoMemo;	//For the logger
        RRJobHandle	  			__fastcall	    simulate();

        void								    loadParameterList();
		void 								    assignRRHandle(RRHandle aHandle);
		void 								    assignPluginHandle(RRPluginHandle aHandle);
	    string									getResult();
		//Assign in parent
        TOnEvent			   				    onFittingStarted;
        TOnEvent	   						    onFittingFinished;

};
//---------------------------------------------------------------------------
extern PACKAGE TFullSpaceFittingFrame *FullSpaceFittingFrame;
//---------------------------------------------------------------------------
#endif
