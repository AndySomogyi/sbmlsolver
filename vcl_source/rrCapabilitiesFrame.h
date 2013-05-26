#ifndef rrCapabilitiesFrameH
#define rrCapabilitiesFrameH
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "rrCapabilities.h"
#include "rrStringList.h"
#include "rrc_api.h"
//---------------------------------------------------------------------------

class TcapFrame : public TFrame
{
__published:	// IDE-managed Components
	TGroupBox *GroupBox3;
	TGroupBox *GroupBox4;
	TComboBox *parameterDropDown;
	TEdit *paraEdit;
	TButton *SetParaBtn;
	TComboBox *capDropDown;

	TPanel *Panel1;
	void __fastcall capDropDownClick(TObject *Sender);
	void __fastcall capDropDownChange(TObject *Sender);
	void __fastcall parameterDropDownChange(TObject *Sender);
	void __fastcall SetParaBtnClick(TObject *Sender);

private:	// User declarations
	rrc::RRHandle							mRR;
	string									getCurrentCapabilityName();
	string									getCurrentParameterName();
public:		// User declarations
				__fastcall 	                TcapFrame(TComponent* Owner);
	void					                Populate(rrc::RRHandle rr, const rr::StringList& caps);

    TMemo			               		   *infoMemo;	//For the logger
};
//---------------------------------------------------------------------------
extern PACKAGE TcapFrame *capFrame;
//---------------------------------------------------------------------------
#endif
