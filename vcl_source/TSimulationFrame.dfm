object SimulateFrame: TSimulateFrame
  Left = 0
  Top = 0
  Width = 257
  Height = 315
  TabOrder = 0
  object GroupBox1: TGroupBox
    Left = 0
    Top = 0
    Width = 257
    Height = 67
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
    Left = 97
    Top = 67
    Width = 160
    Height = 248
    Align = alClient
    TabOrder = 1
    object Panel2: TPanel
      Left = 1
      Top = 1
      Width = 158
      Height = 246
      Align = alClient
      TabOrder = 0
      object simBtn: TButton
        Left = 5
        Top = 5
        Width = 75
        Height = 25
        Action = simulateA
        TabOrder = 0
      end
      object paraEdit: mtkFloatLabeledEdit
        Left = 5
        Top = 52
        Width = 76
        Height = 21
        EditLabel.Width = 3
        EditLabel.Height = 13
        TabOrder = 1
        Text = '0.00'
        OnKeyDown = paraEditKeyDown
      end
    end
  end
  object Panel3: TPanel
    Left = 0
    Top = 67
    Width = 97
    Height = 248
    Align = alLeft
    TabOrder = 2
    object Splitter1: TSplitter
      Left = 1
      Top = 133
      Width = 95
      Height = 3
      Cursor = crVSplit
      Align = alBottom
      ExplicitLeft = 18
      ExplicitTop = 4
      ExplicitWidth = 91
    end
    object GroupBox2: TGroupBox
      Left = 1
      Top = 1
      Width = 95
      Height = 132
      Align = alClient
      Caption = 'Selection List'
      TabOrder = 0
      object SelList: TCheckListBox
        Left = 2
        Top = 15
        Width = 91
        Height = 115
        Align = alClient
        ItemHeight = 13
        TabOrder = 0
        OnClick = SelListClick
      end
    end
    object GroupBox3: TGroupBox
      Left = 1
      Top = 136
      Width = 95
      Height = 111
      Align = alBottom
      Caption = 'Parameters'
      TabOrder = 1
      object paraList: TListBox
        Left = 2
        Top = 15
        Width = 91
        Height = 94
        Align = alClient
        ItemHeight = 13
        TabOrder = 0
        OnClick = paraListClick
      end
    end
  end
  object ActionList1: TActionList
    Left = 184
    Top = 144
    object simulateA: TAction
      Caption = 'Simulate'
      OnExecute = simulateAExecute
    end
  end
end
