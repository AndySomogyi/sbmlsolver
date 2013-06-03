#ifndef aboutFormH
#define aboutFormH

#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Imaging.GIFImg.hpp>
#include "rrc_api.h"

//---------------------------------------------------------------------------
using namespace rrc;


class TrrAboutForm : public TForm
{
__published:	// IDE-managed Components
	TImage *Image1;
	TPanel *Panel1;
	TPanel *Panel2;
	TMemo *Memo1;
	TPanel *Panel3;
	TButton *Button1;
	TEdit *Edit1;
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);

private:	// User declarations
	RRHandle		mRRI;	//To get info from...

public:		// User declarations
	__fastcall 		TrrAboutForm(RRHandle handle, TComponent* Owner);

};
//---------------------------------------------------------------------------
extern PACKAGE TrrAboutForm *rrAboutForm;
//---------------------------------------------------------------------------
#endif
