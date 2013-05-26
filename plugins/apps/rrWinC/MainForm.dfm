object MainF: TMainF
  Left = 0
  Top = 0
  Caption = 'RoadRunner UI'
  ClientHeight = 674
  ClientWidth = 1119
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  Position = poDefault
  OnClose = FormClose
  OnCloseQuery = FormCloseQuery
  OnKeyDown = FormKeyDown
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter1: TSplitter
    Left = 0
    Top = 418
    Width = 1119
    Height = 3
    Cursor = crVSplit
    Align = alBottom
    ExplicitLeft = 402
    ExplicitTop = 0
    ExplicitWidth = 256
  end
  object Panel2: TPanel
    Left = 0
    Top = 0
    Width = 209
    Height = 418
    Align = alLeft
    TabOrder = 0
    object Panel3: TPanel
      Left = 1
      Top = 1
      Width = 208
      Height = 416
      Align = alLeft
      TabOrder = 0
      object GroupBox1: TGroupBox
        Left = 1
        Top = 1
        Width = 206
        Height = 81
        Align = alTop
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
          Width = 51
          Height = 13
          Caption = 'Build Time:'
        end
        object buildTimeLbl: TLabel
          Left = 112
          Top = 62
          Width = 57
          Height = 13
          Caption = 'buildTimeLbl'
        end
      end
      object Model: TGroupBox
        Left = 1
        Top = 82
        Width = 206
        Height = 47
        Align = alTop
        Caption = 'Model'
        TabOrder = 1
        object loadBtn: TButton
          Left = 130
          Top = 16
          Width = 75
          Height = 25
          Action = loadModelA
          TabOrder = 0
        end
        object modelDD: TComboBox
          Left = 3
          Top = 16
          Width = 117
          Height = 21
          TabOrder = 1
          Text = 'modelDD'
          OnChange = modelDDChange
        end
      end
      inline simFrame: TSimulateFrame
        Left = 1
        Top = 129
        Width = 206
        Height = 286
        Align = alClient
        TabOrder = 2
        ExplicitLeft = 1
        ExplicitTop = 129
        ExplicitWidth = 206
        ExplicitHeight = 286
        inherited GroupBox1: TGroupBox
          Width = 206
          ExplicitWidth = 206
          inherited mNrOfSimulationPointsE: mtkIntLabeledEdit
            Text = '50'
          end
          inherited mEndTimeE: mtkFloatLabeledEdit
            Text = '5'
          end
        end
        inherited Panel1: TPanel
          Width = 109
          Height = 219
          ExplicitWidth = 109
          ExplicitHeight = 219
          inherited Panel2: TPanel
            Width = 107
            Height = 217
            ExplicitWidth = 107
            ExplicitHeight = 217
          end
        end
        inherited Panel3: TPanel
          Height = 219
          ExplicitHeight = 219
          inherited Splitter1: TSplitter
            Top = 104
            ExplicitLeft = 1
            ExplicitTop = 19
            ExplicitWidth = 95
          end
          inherited GroupBox2: TGroupBox
            Height = 103
            ExplicitHeight = 103
            inherited SelList: TCheckListBox
              Height = 86
              ExplicitHeight = 86
            end
          end
          inherited GroupBox3: TGroupBox
            Top = 107
            ExplicitTop = 107
          end
        end
        inherited ActionList1: TActionList
          Left = 104
          Top = 128
        end
      end
    end
  end
  object PC1: TPageControl
    Left = 209
    Top = 0
    Width = 910
    Height = 418
    ActivePage = TabSheet6
    Align = alClient
    DoubleBuffered = False
    ParentDoubleBuffered = False
    TabOrder = 1
    object TabSheet1: TTabSheet
      Caption = 'RR Data'
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object Chart1: TChart
        Left = 0
        Top = 29
        Width = 902
        Height = 361
        Legend.Visible = False
        Title.Text.Strings = (
          'TChart')
        Title.Visible = False
        LeftAxis.AxisValuesFormat = '#.0 "x10" E+0'
        LeftAxis.LabelsAlign = alOpposite
        LeftAxis.LabelStyle = talValue
        LeftAxis.RoundFirstLabel = False
        View3D = False
        Align = alClient
        Color = 14145495
        TabOrder = 0
        ColorPaletteIndex = 13
        object Series1: TLineSeries
          Marks.Arrow.Visible = True
          Marks.Callout.Brush.Color = clBlack
          Marks.Callout.Arrow.Visible = True
          Marks.Visible = False
          Brush.BackColor = clDefault
          Pointer.Brush.Gradient.EndColor = 10708548
          Pointer.Gradient.EndColor = 10708548
          Pointer.InflateMargins = True
          Pointer.Style = psCircle
          Pointer.Visible = True
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
      end
      object ToolBar2: TToolBar
        Left = 0
        Top = 0
        Width = 902
        Height = 29
        Caption = 'ToolBar2'
        TabOrder = 1
        object Button5: TButton
          Left = 0
          Top = 0
          Width = 75
          Height = 22
          Action = PlotA
          TabOrder = 0
        end
      end
    end
    object TabSheet6: TTabSheet
      Caption = 'Fit Results'
      ImageIndex = 1
      object Chart2: TChart
        Left = 0
        Top = 0
        Width = 902
        Height = 390
        Legend.Visible = False
        Title.Text.Strings = (
          'TChart')
        Title.Visible = False
        LeftAxis.AxisValuesFormat = '#.0 "x10" E+0'
        LeftAxis.LabelsAlign = alOpposite
        LeftAxis.LabelStyle = talValue
        LeftAxis.RoundFirstLabel = False
        View3D = False
        Align = alClient
        Color = 14145495
        TabOrder = 0
        ColorPaletteIndex = 13
        object LineSeries1: TLineSeries
          Marks.Arrow.Visible = True
          Marks.Callout.Brush.Color = clBlack
          Marks.Callout.Arrow.Visible = True
          Marks.Visible = False
          Brush.BackColor = clDefault
          Pointer.Brush.Gradient.EndColor = 10708548
          Pointer.Gradient.EndColor = 10708548
          Pointer.InflateMargins = True
          Pointer.Style = psCircle
          Pointer.Visible = True
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
      end
    end
  end
  object lowerPanel: TPanel
    Left = 0
    Top = 421
    Width = 1119
    Height = 253
    Align = alBottom
    TabOrder = 2
    object Splitter2: TSplitter
      Left = 469
      Top = 1
      Height = 251
      ExplicitLeft = 554
      ExplicitTop = 17
    end
    object Panel5: TPanel
      Left = 472
      Top = 1
      Width = 646
      Height = 251
      Align = alClient
      Caption = 'Panel5'
      TabOrder = 0
      object infoMemo: TMemo
        Left = 1
        Top = 30
        Width = 644
        Height = 220
        Align = alClient
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Courier New'
        Font.Style = []
        ParentFont = False
        PopupMenu = PopupMenu1
        ScrollBars = ssBoth
        TabOrder = 0
        WordWrap = False
      end
      object ToolBar3: TToolBar
        Left = 1
        Top = 1
        Width = 644
        Height = 29
        Caption = 'ToolBar3'
        TabOrder = 1
        object Button2: TButton
          Left = 0
          Top = 0
          Width = 75
          Height = 22
          Action = clearMemo
          TabOrder = 0
        end
      end
    end
    object bottomLeftPanelWidth: TPanel
      Left = 1
      Top = 1
      Width = 468
      Height = 251
      Align = alLeft
      Caption = 'bottomLeftPanelWidth'
      Ctl3D = False
      ParentCtl3D = False
      TabOrder = 1
      object ToolBar1: TToolBar
        Left = 1
        Top = 1
        Width = 466
        Height = 29
        ButtonHeight = 23
        Caption = 'ToolBar1'
        TabOrder = 0
        object Button4: TButton
          Left = 0
          Top = 0
          Width = 69
          Height = 23
          Action = getLastErrorA
          TabOrder = 0
        end
        object Button6: TButton
          Left = 69
          Top = 0
          Width = 101
          Height = 23
          Action = getCapabilitiesA
          TabOrder = 1
        end
        object Button7: TButton
          Left = 170
          Top = 0
          Width = 75
          Height = 23
          Action = getAllSymbols
          TabOrder = 2
        end
        object Button3: TButton
          Left = 245
          Top = 0
          Width = 75
          Height = 23
          Action = getCapabilitiesAsXMLA
          TabOrder = 3
        end
      end
      object Panel4: TPanel
        Left = 1
        Top = 30
        Width = 96
        Height = 220
        Align = alLeft
        TabOrder = 1
        object GroupBox2: TGroupBox
          Left = 1
          Top = 1
          Width = 94
          Height = 56
          Align = alTop
          Caption = 'Plugins'
          TabOrder = 0
          object Button1: TButton
            Left = 3
            Top = 25
            Width = 75
            Height = 25
            Action = loadPluginsA
            TabOrder = 0
          end
        end
        object pluginList: TListBox
          Left = 1
          Top = 57
          Width = 94
          Height = 162
          Align = alClient
          ItemHeight = 13
          TabOrder = 1
          OnClick = pluginListClick
        end
      end
      object MainPC: TPageControl
        Left = 97
        Top = 30
        Width = 370
        Height = 220
        ActivePage = TabSheet3
        Align = alClient
        TabOrder = 2
        object TabSheet3: TTabSheet
          Caption = 'Capabilities'
          inline TcapFrame1: TcapFrame
            Left = 0
            Top = 0
            Width = 362
            Height = 192
            Align = alClient
            TabOrder = 0
            ExplicitWidth = 362
            ExplicitHeight = 192
            inherited Panel1: TPanel
              Width = 362
              Height = 192
              ExplicitWidth = 362
              ExplicitHeight = 192
              inherited GroupBox3: TGroupBox
                Width = 360
                ExplicitWidth = 360
              end
              inherited GroupBox4: TGroupBox
                Width = 360
                ExplicitWidth = 360
                inherited paraEdit: TEdit
                  Height = 19
                  ExplicitHeight = 19
                end
              end
            end
          end
        end
        object TabSheet5: TTabSheet
          Caption = 'Make Noise'
          ImageIndex = 2
          ExplicitLeft = 0
          ExplicitTop = 0
          ExplicitWidth = 0
          ExplicitHeight = 0
          object noiseSigmaE: mtkFloatLabeledEdit
            Left = 13
            Top = 32
            Width = 84
            Height = 19
            EditLabel.Width = 28
            EditLabel.Height = 13
            EditLabel.Caption = 'Sigma'
            TabOrder = 0
            Text = '1'
            OnKeyDown = noiseSigmaEKeyDown
          end
          object addNoiseBtn: TButton
            Left = 13
            Top = 72
            Width = 75
            Height = 25
            Caption = 'Do It'
            TabOrder = 1
            OnClick = addNoiseBtnClick
          end
        end
      end
    end
  end
  object startupTimer: TTimer
    Enabled = False
    Interval = 100
    OnTimer = startupTimerTimer
    Left = 320
    Top = 24
  end
  object ActionList1: TActionList
    Left = 536
    Top = 376
    object loadPluginsA: TAction
      Caption = 'Load'
      OnExecute = loadPluginsAExecute
    end
    object unloadPlugins: TAction
      Caption = 'Unload'
      OnExecute = unloadPluginsExecute
    end
    object clearMemo: TAction
      Caption = 'Clear'
      OnExecute = clearMemoExecute
    end
    object getPluginInfoA: TAction
      Category = 'PluginActions'
      Caption = 'Get Info'
      OnExecute = getPluginInfoAExecute
    end
    object executePluginA: TAction
      Category = 'PluginActions'
      Caption = 'Execute'
      OnExecute = executePluginAExecute
    end
    object getLastErrorA: TAction
      Category = 'C_API'
      Caption = 'Last Error'
      OnExecute = getLastErrorAExecute
    end
    object loadModelA: TAction
      Category = 'C_API'
      Caption = 'Load'
      OnExecute = loadModelAExecute
      OnUpdate = loadModelAUpdate
    end
    object unLoadModelA: TAction
      Category = 'C_API'
      Caption = 'Unload'
      OnExecute = loadModelAExecute
      OnUpdate = loadModelAUpdate
    end
    object PlotA: TAction
      Caption = 'Plot'
      OnExecute = PlotAExecute
    end
    object executeNoisePluginA: TAction
      Category = 'PluginActions'
      Caption = 'Execute'
    end
    object getCapabilitiesA: TAction
      Category = 'C_API'
      Caption = 'RR Capabilities'
      OnExecute = getCapabilitiesAExecute
    end
    object getAllSymbols: TAction
      Category = 'C_API'
      Caption = 'getAllSymbols'
      OnExecute = getAllSymbolsExecute
    end
    object getCapabilitiesAsXMLA: TAction
      Category = 'C_API'
      Caption = 'getCapabilitiesAsXMLA'
      OnExecute = getCapabilitiesAsXMLAExecute
    end
  end
  object ApplicationEvents1: TApplicationEvents
    OnException = ApplicationEvents1Exception
    Left = 520
    Top = 24
  end
  object PopupMenu1: TPopupMenu
    Left = 168
    Top = 112
    object Clear1: TMenuItem
      Action = clearMemo
    end
  end
  object loadModelJobTimer: TTimer
    Interval = 40
    OnTimer = loadModelJobTimerTimer
    Left = 416
    Top = 24
  end
  object ShutDownTimer: TTimer
    Enabled = False
    Interval = 100
    OnTimer = ShutDownTimerTimer
    Left = 712
    Top = 72
  end
  object mIniFile: mtkIniFileC
    IniFileName = 'RRWinC.ini'
    RootFolder = '.'
    Left = 408
    Top = 344
  end
end
