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
#include <string>
#include <vector>
#include <sstream>
using std::string;
using std::vector;
//---------------------------------------------------------------------------

class Logger
{
    protected:
        std::ostringstream          mStream;
		TMemo					   *mMemo;
    public:
                                    Logger(TMemo* aMemo): mMemo(aMemo){}
        virtual                    ~Logger()
        							{
                                    	mMemo->Lines->Add(mStream.str().c_str());
                                    }
        std::ostringstream&         Get(){return mStream;}

};

#define Log() \
    Logger(infoMemo).Get()

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
	TAction *loadPlugins;
	TAction *unloadPlugins;
	TButton *Button1;
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall startupTimerTimer(TObject *Sender);
	void __fastcall loadPluginsExecute(TObject *Sender);

private:	// User declarations
	RRHandle	mTheAPI;

public:		// User declarations
	__fastcall TMainF(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainF *MainF;
//---------------------------------------------------------------------------
#endif
