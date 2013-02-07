#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "rr_c_api.h"
#include <Vcl.ExtCtrls.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.AppEvnts.hpp>
#include <Vcl.Menus.hpp>
#include <string>
#include <vector>
#include <sstream>
#include "memoLogger.h"
using std::string;
using std::vector;
//---------------------------------------------------------------------------


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
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall startupTimerTimer(TObject *Sender);
	void __fastcall loadPluginsAExecute(TObject *Sender);
	void __fastcall ApplicationEvents1Exception(TObject *Sender, Exception *E);
	void __fastcall pluginListClick(TObject *Sender);
	void __fastcall unloadPluginsExecute(TObject *Sender);
	void __fastcall clearMemoExecute(TObject *Sender);
	void __fastcall getPluginInfoAExecute(TObject *Sender);
	void __fastcall pluginParasCBChange(TObject *Sender);
	void __fastcall SetParaBtnClick(TObject *Sender);
	void __fastcall executePluginAExecute(TObject *Sender);
	void __fastcall getLastErrorAExecute(TObject *Sender);


private:	// User declarations
   	RRHandle	mTheAPI;
	string 		getCurrentPluginName();
	string 		getCurrentSelectedParameter();

public:		// User declarations
	__fastcall TMainF(TComponent* Owner);
};

extern PACKAGE TMainF *MainF;
#endif
