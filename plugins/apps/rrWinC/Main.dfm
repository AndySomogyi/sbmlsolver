object MainF: TMainF
  Left = 0
  Top = 0
  Caption = 'RoadRunner PluginTester'
  ClientHeight = 718
  ClientWidth = 1065
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
  object Splitter1: TSplitter
    Left = 0
    Top = 397
    Width = 1065
    Height = 3
    Cursor = crVSplit
    Align = alBottom
    ExplicitLeft = 402
    ExplicitTop = 0
    ExplicitWidth = 256
  end
  object Panel1: TPanel
    Left = 0
    Top = 400
    Width = 1065
    Height = 318
    Align = alBottom
    Caption = 'Panel1'
    TabOrder = 0
    object infoMemo: TMemo
      Left = 386
      Top = 30
      Width = 678
      Height = 287
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
    object ToolBar1: TToolBar
      Left = 1
      Top = 1
      Width = 1063
      Height = 29
      ButtonHeight = 23
      Caption = 'ToolBar1'
      TabOrder = 1
      object Button4: TButton
        Left = 0
        Top = 0
        Width = 69
        Height = 23
        Action = getLastErrorA
        TabOrder = 0
      end
    end
    object Panel4: TPanel
      Left = 1
      Top = 30
      Width = 96
      Height = 287
      Align = alLeft
      TabOrder = 2
      object GroupBox2: TGroupBox
        Left = 1
        Top = 1
        Width = 94
        Height = 56
        Align = alTop
        Caption = 'Plugins'
        TabOrder = 0
        object Button1: TButton
          Left = 6
          Top = 23
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
        Height = 229
        Align = alClient
        ItemHeight = 13
        TabOrder = 1
        OnClick = pluginListClick
      end
    end
    object PageControl2: TPageControl
      Left = 97
      Top = 30
      Width = 289
      Height = 287
      ActivePage = TabSheet4
      Align = alLeft
      TabOrder = 3
      object TabSheet3: TTabSheet
        Caption = 'Test'
        object GroupBox3: TGroupBox
          Left = 0
          Top = 0
          Width = 281
          Height = 211
          Align = alTop
          Caption = 'Plugin Actions'
          TabOrder = 0
          object Button2: TButton
            Left = 3
            Top = 21
            Width = 75
            Height = 25
            Action = getPluginInfoA
            TabOrder = 0
          end
          object GroupBox4: TGroupBox
            Left = 2
            Top = 128
            Width = 277
            Height = 81
            Align = alBottom
            Caption = 'Parameters'
            TabOrder = 1
            object pluginParasCB: TComboBox
              Left = 3
              Top = 24
              Width = 174
              Height = 21
              AutoDropDown = True
              Style = csDropDownList
              TabOrder = 0
              OnChange = pluginCBChange
            end
            object paraEdit: TEdit
              Left = 3
              Top = 53
              Width = 121
              Height = 21
              TabOrder = 1
              Text = 'paraEdit'
            end
            object SetParaBtn: TButton
              Left = 130
              Top = 51
              Width = 47
              Height = 25
              Caption = 'set'
              TabOrder = 2
              OnClick = SetParaBtnClick
            end
          end
          object Button3: TButton
            Left = 84
            Top = 21
            Width = 75
            Height = 25
            Action = executePluginA
            TabOrder = 2
          end
          object GroupBox5: TGroupBox
            Left = 2
            Top = 64
            Width = 277
            Height = 64
            Align = alBottom
            Caption = 'Capability'
            TabOrder = 3
            object pluginCapsCB: TComboBox
              Left = 3
              Top = 25
              Width = 174
              Height = 21
              AutoDropDown = True
              Style = csDropDownList
              TabOrder = 0
              OnChange = pluginCBChange
            end
          end
        end
      end
      object TabSheet4: TTabSheet
        Caption = 'Fitting'
        ImageIndex = 1
        object PageControl3: TPageControl
          Left = 0
          Top = 0
          Width = 281
          Height = 259
          ActivePage = TabSheet2
          Align = alClient
          TabOrder = 0
          ExplicitLeft = 40
          ExplicitTop = 48
          ExplicitWidth = 289
          ExplicitHeight = 193
          object TabSheet2: TTabSheet
            Caption = 'Full Space'
            ExplicitWidth = 281
            ExplicitHeight = 165
          end
        end
      end
      object TabSheet5: TTabSheet
        Caption = 'Make Noise'
        ImageIndex = 2
        object noiseSigmaE: mtkFloatLabeledEdit
          Left = 13
          Top = 32
          Width = 84
          Height = 21
          EditLabel.Width = 28
          EditLabel.Height = 13
          EditLabel.Caption = 'Sigma'
          TabOrder = 0
          Text = '1e-6'
          OnKeyDown = noiseSigmaEKeyDown
        end
        object Button6: TButton
          Left = 13
          Top = 72
          Width = 75
          Height = 25
          Caption = 'Do It'
          TabOrder = 1
          OnClick = Button6Click
        end
      end
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 0
    Width = 209
    Height = 397
    Align = alLeft
    TabOrder = 1
    object Panel3: TPanel
      Left = 1
      Top = 1
      Width = 208
      Height = 395
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
        Height = 66
        Align = alTop
        Caption = 'Model'
        TabOrder = 1
        object loadBtn: TButton
          Left = 3
          Top = 24
          Width = 75
          Height = 25
          Action = loadModelA
          TabOrder = 0
        end
      end
      inline simFrame: TrrSettingFrame
        Left = 1
        Top = 148
        Width = 206
        Height = 246
        Align = alClient
        TabOrder = 2
        ExplicitLeft = 1
        ExplicitTop = 148
        ExplicitWidth = 206
        ExplicitHeight = 246
        inherited Splitter1: TSplitter
          Width = 206
          ExplicitWidth = 206
        end
        inherited GroupBox1: TGroupBox
          Width = 206
          ExplicitWidth = 206
          inherited mNrOfSimulationPointsE: mtkIntLabeledEdit
            Text = '500'
          end
          inherited mEndTimeE: mtkFloatLabeledEdit
            Text = '5'
          end
        end
        inherited Panel1: TPanel
          Width = 206
          Height = 170
          ExplicitWidth = 206
          ExplicitHeight = 170
          inherited Panel2: TPanel
            Left = 87
            Height = 168
            ExplicitLeft = 87
            ExplicitHeight = 168
            inherited simBtn: TButton
              OnClick = simFramesimBtnClick
            end
          end
          inherited SelList: TCheckListBox
            Width = 86
            Height = 168
            ExplicitWidth = 86
            ExplicitHeight = 168
          end
        end
      end
    end
  end
  object PageControl1: TPageControl
    Left = 209
    Top = 0
    Width = 856
    Height = 397
    ActivePage = TabSheet1
    Align = alClient
    TabOrder = 2
    object TabSheet1: TTabSheet
      Caption = 'RR Data'
      object Chart1: TChart
        Left = 0
        Top = 29
        Width = 848
        Height = 340
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
        Color = clGray
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
          Pointer.Style = psRectangle
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
        Width = 848
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
  end
  object startupTimer: TTimer
    Enabled = False
    Interval = 100
    OnTimer = startupTimerTimer
    Left = 152
    Top = 608
  end
  object ActionList1: TActionList
    Left = 408
    Top = 456
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
  end
  object ApplicationEvents1: TApplicationEvents
    OnException = ApplicationEvents1Exception
    Left = 576
    Top = 56
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
    Left = 552
    Top = 104
  end
end
