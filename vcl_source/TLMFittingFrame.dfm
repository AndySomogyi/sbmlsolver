object LMFittingFrame: TLMFittingFrame
  Left = 0
  Top = 0
  Width = 730
  Height = 325
  Align = alClient
  TabOrder = 0
  ExplicitWidth = 451
  ExplicitHeight = 304
  object fittingFrame: TGroupBox
    Left = 0
    Top = 0
    Width = 201
    Height = 325
    Align = alLeft
    Caption = 'Parameters'
    TabOrder = 0
    ExplicitHeight = 304
    object executeBtn: TButton
      Left = 129
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
      Height = 308
      Align = alLeft
      ItemHeight = 13
      TabOrder = 1
      OnClick = paraListClick
      ExplicitHeight = 287
    end
    object paraEdit: mtkFloatLabeledEdit
      Left = 129
      Top = 72
      Width = 64
      Height = 21
      EditLabel.Width = 40
      EditLabel.Height = 13
      EditLabel.Caption = 'paraEdit'
      TabOrder = 2
      Text = '0.00'
    end
  end
  object Chart1: TChart
    Left = 201
    Top = 0
    Width = 529
    Height = 325
    Legend.ResizeChart = False
    Legend.TopPos = 6
    Legend.Transparent = True
    Title.Text.Strings = (
      'TChart')
    Title.Visible = False
    View3D = False
    Align = alClient
    TabOrder = 1
    ExplicitWidth = 250
    ExplicitHeight = 304
    ColorPaletteIndex = 13
    object Series1: TLineSeries
      LegendTitle = 'Model'
      Marks.Arrow.Visible = True
      Marks.Callout.Brush.Color = clBlack
      Marks.Callout.Arrow.Visible = True
      Marks.Visible = False
      SeriesColor = clRed
      Brush.BackColor = clDefault
      LinePen.Width = 3
      Pointer.InflateMargins = True
      Pointer.Style = psRectangle
      Pointer.Visible = False
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
    end
    object Series2: TPointSeries
      LegendTitle = 'Input Data'
      Marks.Arrow.Visible = True
      Marks.Callout.Brush.Color = clBlack
      Marks.Callout.Arrow.Visible = True
      Marks.Visible = False
      ClickableLine = False
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
    object Series3: TLineSeries
      LegendTitle = 'Residual'
      Marks.Arrow.Visible = True
      Marks.Callout.Brush.Color = clBlack
      Marks.Callout.Arrow.Visible = True
      Marks.Visible = False
      SeriesColor = clBlue
      Brush.BackColor = clDefault
      LinePen.Color = clBlue
      LinePen.Width = 3
      Pointer.InflateMargins = True
      Pointer.Style = psRectangle
      Pointer.Visible = False
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
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
