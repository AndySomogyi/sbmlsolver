object FullSpaceFittingFrame: TFullSpaceFittingFrame
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
    Caption = 'Settings'
    TabOrder = 0
    object executeBtn: TButton
      Left = 168
      Top = 120
      Width = 75
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
    object stepsE: mtkIntLabeledEdit
      Left = 129
      Top = 72
      Width = 92
      Height = 21
      EditLabel.Width = 96
      EditLabel.Height = 13
      EditLabel.Caption = 'Steps per dimension'
      TabOrder = 2
      Text = '50'
    end
    object sweepRangeE: mtkFloatLabeledEdit
      Left = 129
      Top = 24
      Width = 92
      Height = 21
      EditLabel.Width = 85
      EditLabel.Height = 13
      EditLabel.Caption = 'Sweep range (%)'
      TabOrder = 3
      Text = '10'
    end
  end
  object ActionList1: TActionList
    Left = 56
    Top = 128
  end
end
