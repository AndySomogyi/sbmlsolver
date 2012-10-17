object MainForm: TMainForm
  Left = 0
  Top = 0
  Caption = 'MainForm'
  ClientHeight = 560
  ClientWidth = 740
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  OnKeyDown = FormKeyDown
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 336
    Top = 208
    Width = 185
    Height = 41
    TabOrder = 0
  end
  object Panel2: TPanel
    Left = 0
    Top = 0
    Width = 145
    Height = 392
    Align = alLeft
    TabOrder = 1
    object Panel4: TPanel
      Left = 1
      Top = 1
      Width = 143
      Height = 104
      Align = alTop
      TabOrder = 0
      object Button1: TButton
        Left = 32
        Top = 40
        Width = 75
        Height = 25
        Action = LoadDLL
        TabOrder = 0
      end
    end
    object ListBox1: TListBox
      Left = 1
      Top = 105
      Width = 143
      Height = 286
      Align = alClient
      ItemHeight = 13
      TabOrder = 1
    end
  end
  object Panel3: TPanel
    Left = 544
    Top = 208
    Width = 185
    Height = 41
    TabOrder = 2
  end
  object Panel5: TPanel
    Left = 0
    Top = 392
    Width = 740
    Height = 168
    Align = alBottom
    TabOrder = 3
    object Memo1: TMemo
      Left = 1
      Top = 1
      Width = 738
      Height = 166
      Align = alClient
      Lines.Strings = (
        'Memo1')
      TabOrder = 0
      ExplicitLeft = 376
      ExplicitTop = 80
      ExplicitWidth = 185
      ExplicitHeight = 89
    end
  end
  object ActionList1: TActionList
    Left = 376
    Top = 80
    object LoadDLL: TAction
      Caption = 'LoadDLL'
      OnExecute = LoadDLLExecute
    end
  end
  object OpenDialog1: TOpenDialog
    FileName = 'rr_c_api.dll'
    Left = 192
    Top = 56
  end
end
