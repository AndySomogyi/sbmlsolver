object LMFittingFrame: TLMFittingFrame
  Left = 0
  Top = 0
  Width = 257
  Height = 256
  TabOrder = 0
  object sweepE: TGroupBox
    Left = 0
    Top = 0
    Width = 257
    Height = 161
    Align = alTop
    Caption = 'Parameters'
    TabOrder = 0
    object executeBtn: TButton
      Left = 179
      Top = 24
      Width = 62
      Height = 25
      Caption = 'Run'
      TabOrder = 0
      OnClick = executeBtnClick
    end
    object paraList: TCheckListBox
      Left = 2
      Top = 15
      Width = 121
      Height = 144
      Align = alLeft
      ItemHeight = 13
      TabOrder = 1
      OnClick = paraListClick
    end
    object Button1: TButton
      Left = 179
      Top = 55
      Width = 62
      Height = 25
      Action = logResultA
      TabOrder = 2
    end
  end
  object ActionList1: TActionList
    Left = 56
    Top = 128
    object logResultA: TAction
      Caption = 'Log Result'
      OnExecute = logResultAExecute
    end
  end
end
