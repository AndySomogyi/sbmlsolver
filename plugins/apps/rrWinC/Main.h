#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "rrc_api.h"
#include <Vcl.ExtCtrls.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.AppEvnts.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ComCtrls.hpp>

#include <Vcl.ToolWin.hpp>
#include "rrSimulationFrame.h"
#include <VCLTee.Chart.hpp>

#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <VCLTee.Series.hpp>
#include "mtkFloatLabeledEdit.h"
#include <string>
#include <vector>
#include <sstream>
#include "rrc_types.h"
#include "memoLogger.h"
#include "rrSimulationData.h"
using std::string;
using std::vector;
//---------------------------------------------------------------------------
using namespace rrc;;

typedef void __fastcall (__closure *TOnSimulationFinished)();

class TMainF : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *GroupBox1;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *apiVersionLBL;
	TLabel *buildDateLbl;
	TTimer *startupTimer;
	TLabel *Label3;
	TLabel *buildTimeLbl;
	TMemo *infoMemo;
	TPanel *Panel1;
	TActionList *ActionList1;
	TAction *loadPluginsA;
	TAction *unloadPlugins;
	TButton *Button1;
	TListBox *pluginList;
	TPanel *Panel2;
	TApplicationEvents *ApplicationEvents1;
	TPanel *Panel3;
	TPanel *Panel4;
	TGroupBox *GroupBox2;
	TGroupBox *GroupBox3;
	TSplitter *Splitter1;
	TPopupMenu *PopupMenu1;
	TAction *clearMemo;
	TMenuItem *Clear1;
	TButton *Button2;
	TAction *getPluginInfoA;
	TGroupBox *GroupBox4;
	TComboBox *pluginParasCB;
	TEdit *paraEdit;
	TButton *SetParaBtn;
	TButton *Button3;
	TAction *executePluginA;
	TButton *Button4;
	TAction *getLastErrorA;
	TGroupBox *GroupBox5;
	TComboBox *pluginCapsCB;
	TGroupBox *Model;
	TToolBar *ToolBar1;
	TButton *loadBtn;
	TAction *loadModelA;
	TTimer *loadModelJobTimer;
	TAction *unLoadModelA;
	TrrSettingFrame *simFrame;
	TChart *Chart1;
	TPageControl *PageControl1;
	TTabSheet *TabSheet1;
	TToolBar *ToolBar2;
	TAction *PlotA;
	TButton *Button5;
	TPageControl *PageControl2;
	TTabSheet *TabSheet3;
	TTabSheet *TabSheet4;
	TTabSheet *TabSheet5;
	mtkFloatLabeledEdit *noiseSigmaE;
	TButton *Button6;
	TLineSeries *Series1;
	TAction *executeNoisePluginA;
	TPageControl *PageControl3;
	TTabSheet *TabSheet2;
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall startupTimerTimer(TObject *Sender);
	void __fastcall loadPluginsAExecute(TObject *Sender);
	void __fastcall ApplicationEvents1Exception(TObject *Sender, Exception *E);
	void __fastcall pluginListClick(TObject *Sender);
	void __fastcall unloadPluginsExecute(TObject *Sender);
	void __fastcall clearMemoExecute(TObject *Sender);
	void __fastcall getPluginInfoAExecute(TObject *Sender);
	void __fastcall pluginCBChange(TObject *Sender);
	void __fastcall SetParaBtnClick(TObject *Sender);
	void __fastcall executePluginAExecute(TObject *Sender);
	void __fastcall getLastErrorAExecute(TObject *Sender);
	void __fastcall loadModelAExecute(TObject *Sender);
	void __fastcall loadModelJobTimerTimer(TObject *Sender);
	void __fastcall loadModelAUpdate(TObject *Sender);
	void __fastcall PlotAExecute(TObject *Sender);
	void __fastcall noiseSigmaEKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Button6Click(TObject *Sender);
	void __fastcall simFramesimBtnClick(TObject *Sender);


private:	// User declarations
   	RRHandle	                        mInstanceH;
    RRResult* 							mData;
    RRJobHandle	                        mLoadModelJob;
    RRJobHandle	                        mSimulateModelJob;
	TOnSimulationFinished				OnSimulationFinsihed;
    string 		                        mModel;
    bool								mUIIsStartingUp;
	string 		                        getCurrentPluginName();
	string 		                        getCurrentSelectedParameter();
	void 								Plot(const rr::SimulationData& result);
	void 								Plot1D();
	void								UpdateNoisePanel();
    void								configurePlugin(const string& pluginName);


public:		// User declarations
			__fastcall 					TMainF(TComponent* Owner);
	void 	__fastcall 					onSimulationStarted();
	void 	__fastcall 					onSimulationFinished();

};

extern PACKAGE TMainF *MainF;
#endif
