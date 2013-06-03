#ifndef TSimulationFrameH
#define TSimulationFrameH
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
#include "rrParameters.h"
using namespace rr;
using namespace rrc;

typedef void __fastcall (__closure *TOnSimulationFinished)();

//---------------------------------------------------------------------------
class TSimulateFrame : public TFrame
{
__published:	// IDE-managed Components
	TGroupBox *GroupBox1;
	mtkIntLabeledEdit *mNrOfSimulationPointsE;
	mtkFloatLabeledEdit *mEndTimeE;
	mtkFloatLabeledEdit *mStartTimeE;
	TPanel *Panel1;
	TActionList *ActionList1;
	TPanel *Panel2;
	TButton *simBtn;
	TAction *simulateA;
	TCheckListBox *SelList;
	TGroupBox *GroupBox2;
	TPanel *Panel3;
	TGroupBox *GroupBox3;
	TSplitter *Splitter1;
	mtkFloatLabeledEdit *paraEdit;
	TListBox *paraList;
	void __fastcall SelListClick(TObject *Sender);
	void __fastcall simulateAExecute(TObject *Sender);
	void __fastcall paraListClick(TObject *Sender);
	void __fastcall paraEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);

    private:
        RRHandle					mRRI;
        RRJobHandle					mSimJobH;
		Parameters					mParameters;	//Parameters for the current model

        static void __stdcall	   	ThreadEnterCB(void *UserData);					//These need to be static in order to be passed to the C API thread function
        static void __stdcall	   	ThreadExitCB(void *UserData);

        void 		__fastcall     	simulationStarted();
        void 		__fastcall     	simulationFinished();

	public:
                    __fastcall     	TSimulateFrame(TComponent* Owner);
        TMemo			           *infoMemo;	//For the logger
        void			           	assignRRHandle(RRHandle aHandle);
        bool			           	loadSelectionList();
        void 					    EnableDisableSimulation();

        RRJobHandle	__fastcall		simulate();
        TOnSimulationFinished	   	onSimulationFinished;	//Function pointer, called in main form
};

//---------------------------------------------------------------------------
extern PACKAGE TSimulateFrame *SimulateFrame;
//---------------------------------------------------------------------------
#endif
