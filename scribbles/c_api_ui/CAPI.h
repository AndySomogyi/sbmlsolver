//---------------------------------------------------------------------------

#ifndef CAPIH
#define CAPIH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <string>
//---------------------------------------------------------------------------

using std::string;

class TMainForm : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TPanel *Panel2;
    TPanel *Panel3;
    TPanel *Panel4;
    TPanel *Panel5;
    TMemo *Memo1;
    TListBox *ListBox1;
    TActionList *ActionList1;
    TButton *Button1;
    TAction *LoadDLL;
    TOpenDialog *OpenDialog1;
    void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
    void __fastcall LoadDLLExecute(TObject *Sender);

private:	// User declarations
    string                  mDLLFileName;
    string                  mDLLFilePath;

public:		// User declarations
            __fastcall      TMainForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
