#ifndef MainFormH
#define MainFormH
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
#include <VCLTee.Chart.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <VCLTee.Series.hpp>
#include "mtkFloatLabeledEdit.h"
#include "TFullSpaceFittingFrame.h"
#include "rrSimulationData.h"
#include "TSimulationFrame.h"
#include "rrCapabilitiesFrame.h"
#include "mtkIniFileC.h"
#include <string>
#include <vector>
#include <sstream>
#include "TRegistryForm.h"
#include "rrc_types.h"
#include "rrSimulationData.h"
#include "rrLogFileReader.h"
#include "rrMemoLogger.h"
#include "mtkIniParameters.h"
#include "mtkApplicationInfo.h"
using std::string;
using std::vector;
//---------------------------------------------------------------------------
using namespace rr;
using namespace rrc;
using mtk::mtkIniParameters;
using mtk::mtkIniParameter;
typedef void __fastcall (__closure *TOnSimulationFinished)();

class TMainF : public TRegistryForm
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
	TSplitter *Splitter1;
	TPopupMenu *PopupMenu1;
	TAction *clearMemo;
	TMenuItem *Clear1;
	TAction *getPluginInfoA;
	TAction *executePluginA;
	TButton *Button4;
	TAction *getLastErrorA;
	TGroupBox *Model;
	TToolBar *ToolBar1;
	TButton *loadBtn;
	TAction *loadModelA;
	TTimer *loadModelJobTimer;
	TAction *unLoadModelA;
	TChart *Chart1;
	TPageControl *PC1;
	TTabSheet *TabSheet1;
	TToolBar *ToolBar2;
	TAction *PlotA;
	TButton *Button5;
	TPageControl *PageControl2;
	TTabSheet *TabSheet3;
	TTabSheet *TabSheet4;
	TTabSheet *TabSheet5;
	mtkFloatLabeledEdit *noiseSigmaE;
	TButton *addNoiseBtn;
	TLineSeries *Series1;
	TAction *executeNoisePluginA;
	TPageControl *PageControl3;
	TTabSheet *TabSheet2;
	TSimulateFrame *simFrame;
	TFullSpaceFittingFrame *fullSpaceFitFrame;
	TButton *Button6;
	TAction *getCapabilitiesA;
	TButton *Button7;
	TAction *getAllSymbols;
	TComboBox *modelDD;
	TTabSheet *TabSheet6;
	TChart *Chart2;
	TLineSeries *LineSeries1;
	TTimer *ShutDownTimer;
	TcapFrame *TcapFrame1;
	TPanel *Panel5;
	TPanel *lowerPanel;
	TToolBar *ToolBar3;
	TButton *Button2;
	TButton *Button3;
	TAction *getCapabilitiesAsXMLA;
	mtkIniFileC *mIniFile;
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall startupTimerTimer(TObject *Sender);
	void __fastcall loadPluginsAExecute(TObject *Sender);
	void __fastcall ApplicationEvents1Exception(TObject *Sender, Sysutils::Exception *E);
	void __fastcall pluginListClick(TObject *Sender);
	void __fastcall unloadPluginsExecute(TObject *Sender);
	void __fastcall clearMemoExecute(TObject *Sender);
	void __fastcall getPluginInfoAExecute(TObject *Sender);
	void __fastcall executePluginAExecute(TObject *Sender);
	void __fastcall getLastErrorAExecute(TObject *Sender);
	void __fastcall loadModelAExecute(TObject *Sender);
	void __fastcall loadModelJobTimerTimer(TObject *Sender);
	void __fastcall loadModelAUpdate(TObject *Sender);
	void __fastcall PlotAExecute(TObject *Sender);
	void __fastcall noiseSigmaEKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall addNoiseBtnClick(TObject *Sender);
	void __fastcall simFramesimBtnClick(TObject *Sender);
	void __fastcall getCapabilitiesAExecute(TObject *Sender);
	void __fastcall getAllSymbolsExecute(TObject *Sender);
	void __fastcall modelDDChange(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall ShutDownTimerTimer(TObject *Sender);
	void __fastcall fullSpaceFitFrameexecuteBtnClick(TObject *Sender);
	void __fastcall getCapabilitiesAsXMLAExecute(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);

private:
	mtkApplicationInfo					mAppInfo;
    LogFileReader       	    		mLogFileSniffer;
   	RRHandle	                        mRRI;
    RRPluginHandle						mCurrentlySelectedPlugin;
    RRPluginHandle						mAddNoisePlugin;
    RRPluginHandle						mMinimizePlugin;
    RRData* 							mData;
    RRJobHandle	                        mLoadModelJob;
    RRJobHandle	                        mSimulateModelJob;
	TOnSimulationFinished				OnSimulationFinsihed;
    string 		                        mModel;
    string								mModelsFolder;
    bool								mUIIsStartingUp;
	string 		                        getCurrentPluginName();
	string 		                        getCurrentSelectedParameter();
	void 								Plot(const rr::SimulationData& result);
	void								UpdateNoisePanel();
    void								configurePlugin(RRPluginHandle plugin);
    static void __stdcall	   			addNoiseStartedCB(void *UserData);
    static void __stdcall	   			addNoiseFinishedCB(void *UserData);
    void								populateModelsDropDown();
	void								SetupAndReadParameters();

public:
	mtkIniParameters					mIniParas;
	mtkIniParameter<int>				mLowerPanelHeight;

	SimulationData						mCurrentData;
			__fastcall 					TMainF(TComponent* Owner);
	void 	__fastcall 					onSimulationStarted();
	void 	__fastcall 					onSimulationFinished();

	void 	__fastcall 					onAddNoiseStarted();
	void 	__fastcall 					onAddNoiseFinished();

	void 	__fastcall 					onFittingFinished();

 	void    __fastcall					LogMessage();			//Called from logfile reader
    string                         	   *mLogString; 			//Data exchanged with the logFileReader THread

};

extern PACKAGE TMainF *MainF;
#endif
