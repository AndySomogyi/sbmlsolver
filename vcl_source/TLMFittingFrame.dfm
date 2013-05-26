object LMFittingFrame: TLMFittingFrame
  Left = 0
  Top = 0
  Width = 882
  Height = 289
  Align = alClient
  TabOrder = 0
  ExplicitWidth = 451
  ExplicitHeight = 304
  object Splitter3: TSplitter
    Left = 113
    Top = 0
    Height = 289
    ExplicitLeft = 16
    ExplicitTop = 136
    ExplicitHeight = 100
  end
  object Chart1: TChart
    Left = 204
    Top = 0
    Width = 678
    Height = 289
    Legend.Brush.Gradient.Direction = gdBottomTop
    Legend.CheckBoxesStyle = cbsRadio
    Legend.LeftPercent = 80
    Legend.PositionUnits = muPercent
    Legend.ResizeChart = False
    Legend.ShapeStyle = fosRoundRectangle
    Legend.Symbol.DefaultPen = False
    Legend.Symbol.Emboss.Visible = True
    Legend.Title.Transparent = False
    Legend.TopPercent = 15
    Legend.TopPos = 15
    Title.Text.Strings = (
      'TChart')
    Title.Visible = False
    BottomAxis.Labels = False
    View3D = False
    Align = alClient
    TabOrder = 0
    ExplicitLeft = 210
    ColorPaletteIndex = 2
    object Series1: TLineSeries
      Marks.Arrow.Visible = True
      Marks.Callout.Brush.Color = clBlack
      Marks.Callout.Arrow.Visible = True
      Marks.Visible = False
      Brush.BackColor = clDefault
      Pointer.InflateMargins = True
      Pointer.Style = psRectangle
      Pointer.Visible = False
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
    end
  end
  object GroupBox2: TGroupBox
    Left = 116
    Top = 0
    Width = 88
    Height = 289
    Align = alLeft
    Caption = 'Actions'
    TabOrder = 1
    ExplicitHeight = 304
    object paraEdit: mtkFloatLabeledEdit
      Left = 6
      Top = 32
      Width = 64
      Height = 21
      EditLabel.Width = 40
      EditLabel.Height = 13
      EditLabel.Caption = 'paraEdit'
      TabOrder = 0
      Text = '0.00'
    end
    object executeBtn: TButton
      Left = 6
      Top = 111
      Width = 62
      Height = 25
      Caption = 'Run'
      TabOrder = 1
      OnClick = executeBtnClick
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 113
    Height = 289
    Align = alLeft
    TabOrder = 2
    ExplicitHeight = 304
    object Splitter1: TSplitter
      Left = 1
      Top = 157
      Width = 111
      Height = 3
      Cursor = crVSplit
      Align = alTop
      ExplicitLeft = 25
      ExplicitTop = 217
      ExplicitWidth = 294
    end
    object GroupBox3: TGroupBox
      Left = 1
      Top = 160
      Width = 111
      Height = 128
      Align = alClient
      Caption = 'Parameters'
      TabOrder = 0
      ExplicitTop = 96
      ExplicitHeight = 207
      object paraList: TCheckListBox
        Left = 2
        Top = 15
        Width = 107
        Height = 111
        Align = alClient
        ItemHeight = 13
        TabOrder = 0
        OnClick = paraListClick
        ExplicitHeight = 190
      end
    end
    object GroupBox4: TGroupBox
      Left = 1
      Top = 65
      Width = 111
      Height = 92
      Align = alTop
      Caption = 'Species'
      TabOrder = 1
      ExplicitTop = 1
      object speciesList: TCheckListBox
        Left = 2
        Top = 15
        Width = 107
        Height = 75
        Align = alClient
        ItemHeight = 13
        TabOrder = 0
        OnClick = paraListClick
      end
    end
    object GroupBox1: TGroupBox
      Left = 1
      Top = 1
      Width = 111
      Height = 64
      Align = alTop
      Caption = 'Info'
      TabOrder = 2
      object nameLbl: TLabel
        Left = 16
        Top = 16
        Width = 39
        Height = 13
        Caption = 'nameLbl'
      end
    end
  end
  object ActionList1: TActionList
    Left = 48
    Top = 32
    object logResultA: TAction
      Caption = 'Log Result'
      OnExecute = logResultAExecute
    end
  end
end
