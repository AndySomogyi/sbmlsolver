object LMFittingFrame: TLMFittingFrame
  Left = 0
  Top = 0
  Width = 451
  Height = 304
  Align = alClient
  TabOrder = 0
  object Splitter3: TSplitter
    Left = 177
    Top = 0
    Height = 304
    ExplicitLeft = 16
    ExplicitTop = 136
    ExplicitHeight = 100
  end
  object GroupBox2: TGroupBox
    Left = 388
    Top = 0
    Width = 88
    Height = 304
    Align = alLeft
    Caption = 'Actions'
    TabOrder = 0
    object executeBtn: TButton
      Left = 5
      Top = 33
      Width = 62
      Height = 25
      Caption = 'Run'
      TabOrder = 0
      OnClick = executeBtnClick
    end
    object Button1: TButton
      Left = 6
      Top = 64
      Width = 58
      Height = 25
      Action = saveResult
      TabOrder = 1
    end
    object Button2: TButton
      Left = 6
      Top = 95
      Width = 58
      Height = 25
      Action = logResultA
      TabOrder = 2
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 177
    Height = 304
    Align = alLeft
    TabOrder = 1
    object GroupBox4: TGroupBox
      Left = 1
      Top = 48
      Width = 175
      Height = 255
      Align = alClient
      Caption = 'Species'
      TabOrder = 0
      object speciesList: TCheckListBox
        Left = 2
        Top = 15
        Width = 171
        Height = 238
        Align = alClient
        ItemHeight = 13
        TabOrder = 0
        OnClick = paraListClick
      end
    end
    object Model: TGroupBox
      Left = 1
      Top = 1
      Width = 175
      Height = 47
      Align = alTop
      Caption = 'Model'
      TabOrder = 1
      object modelDD: TComboBox
        Left = 2
        Top = 20
        Width = 158
        Height = 21
        TabOrder = 0
        Text = 'modelDD'
        OnChange = modelDDChange
      end
    end
  end
  object Panel2: TPanel
    Left = 180
    Top = 0
    Width = 208
    Height = 304
    Align = alLeft
    TabOrder = 2
    object GroupBox3: TGroupBox
      Left = 1
      Top = 1
      Width = 206
      Height = 238
      Align = alClient
      Caption = 'Parameters'
      TabOrder = 0
      object paraList: TCheckListBox
        Left = 2
        Top = 15
        Width = 202
        Height = 221
        Align = alClient
        ItemHeight = 13
        TabOrder = 0
        OnClick = paraListClick
      end
    end
    object GroupBox5: TGroupBox
      Left = 1
      Top = 239
      Width = 206
      Height = 64
      Align = alBottom
      Caption = 'Set parameter'
      TabOrder = 1
      object paraEdit: mtkFloatLabeledEdit
        Left = 14
        Top = 32
        Width = 83
        Height = 21
        EditLabel.Width = 40
        EditLabel.Height = 13
        EditLabel.Caption = 'paraEdit'
        TabOrder = 0
        Text = '0.00'
      end
    end
  end
  object ActionList1: TActionList
    Left = 200
    Top = 56
    object logResultA: TAction
      Caption = 'Log Result'
      OnExecute = logResultAExecute
    end
    object saveResult: TAction
      Caption = 'Save'
    end
  end
  object loadModelJobTimer: TTimer
    Enabled = False
    Interval = 10
    OnTimer = loadModelJobTimerTimer
    Left = 272
    Top = 160
  end
end
