object MainF: TMainF
  Left = 0
  Top = 0
  Caption = 'PluginTester'
  ClientHeight = 429
  ClientWidth = 832
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnKeyDown = FormKeyDown
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 0
    Top = 0
    Width = 201
    Height = 198
    Align = alLeft
    Caption = 'RoadRunner C-API'
    TabOrder = 0
    object Label1: TLabel
      Left = 16
      Top = 24
      Width = 39
      Height = 13
      Caption = 'Version:'
    end
    object Label2: TLabel
      Left = 16
      Top = 43
      Width = 52
      Height = 13
      Caption = 'Build Date:'
    end
    object apiVersionLBL: TLabel
      Left = 112
      Top = 24
      Width = 65
      Height = 13
      Caption = 'apiVersionLBL'
    end
    object buildDateLbl: TLabel
      Left = 112
      Top = 43
      Width = 58
      Height = 13
      Caption = 'buildDateLbl'
    end
    object Label3: TLabel
      Left = 16
      Top = 62
      Width = 52
      Height = 13
      Caption = 'Build Date:'
    end
    object buildTimeLbl: TLabel
      Left = 112
      Top = 62
      Width = 57
      Height = 13
      Caption = 'buildTimeLbl'
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 198
    Width = 832
    Height = 231
    Align = alBottom
    Caption = 'Panel1'
    TabOrder = 1
    ExplicitTop = 248
    ExplicitWidth = 785
    object infoMemo: TMemo
      Left = 1
      Top = 1
      Width = 830
      Height = 229
      Align = alClient
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Segoe UI Light'
      Font.Style = []
      ParentFont = False
      ScrollBars = ssBoth
      TabOrder = 0
      WordWrap = False
      ExplicitTop = 6
      ExplicitWidth = 783
      ExplicitHeight = 152
    end
  end
  object Button1: TButton
    Left = 488
    Top = 48
    Width = 75
    Height = 25
    Action = loadPlugins
    TabOrder = 2
  end
  object startupTimer: TTimer
    Enabled = False
    Interval = 100
    OnTimer = startupTimerTimer
    Left = 216
    Top = 32
  end
  object ActionList1: TActionList
    Left = 240
    Top = 112
    object loadPlugins: TAction
      Caption = 'loadPlugins'
      OnExecute = loadPluginsExecute
    end
    object unloadPlugins: TAction
      Caption = 'unloadPlugins'
    end
  end
end
