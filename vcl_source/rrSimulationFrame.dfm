object rrSettingFrame: TrrSettingFrame
  Left = 0
  Top = 0
  Width = 257
  Height = 315
  TabOrder = 0
  object Splitter1: TSplitter
    Left = 0
    Top = 73
    Width = 257
    Height = 3
    Cursor = crVSplit
    Align = alTop
    ExplicitLeft = 48
    ExplicitTop = 62
    ExplicitWidth = 320
  end
  object GroupBox1: TGroupBox
    Left = 0
    Top = 0
    Width = 257
    Height = 73
    Align = alTop
    Caption = 'Settings'
    TabOrder = 0
    object mNrOfSimulationPointsE: mtkIntLabeledEdit
      Left = 139
      Top = 35
      Width = 57
      Height = 21
      EditLabel.Width = 56
      EditLabel.Height = 13
      EditLabel.Caption = 'Nr of Points'
      Enabled = False
      TabOrder = 0
      Text = '0'
    end
    object mEndTimeE: mtkFloatLabeledEdit
      Left = 71
      Top = 35
      Width = 50
      Height = 21
      EditLabel.Width = 43
      EditLabel.Height = 13
      EditLabel.Caption = 'End Time'
      Enabled = False
      TabOrder = 1
      Text = '0.00'
    end
    object mStartTimeE: mtkFloatLabeledEdit
      Left = 7
      Top = 35
      Width = 50
      Height = 21
      EditLabel.Width = 49
      EditLabel.Height = 13
      EditLabel.Caption = 'Start Time'
      Enabled = False
      TabOrder = 2
      Text = '0.00'
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 76
    Width = 257
    Height = 239
    Align = alClient
    TabOrder = 1
    object Panel2: TPanel
      Left = 138
      Top = 1
      Width = 118
      Height = 237
      Align = alRight
      TabOrder = 0
      object Button1: TButton
        Left = 16
        Top = 16
        Width = 75
        Height = 25
        Action = simulateA
        TabOrder = 0
      end
    end
    object SelList: TCheckListBox
      Left = 1
      Top = 1
      Width = 137
      Height = 237
      Align = alClient
      ItemHeight = 13
      TabOrder = 1
      OnClick = SelListClick
    end
  end
  object ActionList1: TActionList
    Left = 56
    Top = 136
    object simulateA: TAction
      Caption = 'Simulate'
      OnExecute = simulateAExecute
    end
  end
end
