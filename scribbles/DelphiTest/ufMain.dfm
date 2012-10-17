object frmMain: TfrmMain
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'Test uRoadRunnerAPI.pas'
  ClientHeight = 708
  ClientWidth = 1068
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
  OnCreate = FormCreate
  DesignSize = (
    1068
    708)
  PixelsPerInch = 96
  TextHeight = 13
  object lblCommon: TLabel
    Left = 8
    Top = 204
    Width = 76
    Height = 13
    Caption = 'Common double'
  end
  object Label2: TLabel
    Left = 8
    Top = 160
    Width = 80
    Height = 13
    Caption = 'Common Integer'
  end
  object Label3: TLabel
    Left = 504
    Top = 447
    Width = 53
    Height = 13
    Caption = 'Time Start:'
  end
  object Label4: TLabel
    Left = 510
    Top = 474
    Width = 47
    Height = 13
    Caption = 'Time End:'
  end
  object Label5: TLabel
    Left = 482
    Top = 501
    Width = 86
    Height = 13
    Caption = 'Number of Points:'
  end
  object Label6: TLabel
    Left = 256
    Top = 40
    Width = 16
    Height = 13
    Caption = '0.0'
  end
  object Label7: TLabel
    Left = 183
    Top = 40
    Width = 69
    Height = 13
    Caption = 'Memory Used:'
  end
  object pnlBottm: TPanel
    Left = 0
    Top = 667
    Width = 1068
    Height = 41
    Align = alBottom
    TabOrder = 0
    DesignSize = (
      1068
      41)
    object Label1: TLabel
      Left = 487
      Top = 16
      Width = 66
      Height = 13
      Caption = 'Temp Folder: '
    end
    object lblTempFolder: TEdit
      Left = 555
      Top = 13
      Width = 317
      Height = 21
      TabOrder = 0
      Text = 'C:\'
      OnKeyDown = lblTempFolderKeyDown
    end
    object edtProgress: TEdit
      Left = 8
      Top = 12
      Width = 409
      Height = 21
      TabOrder = 1
      Text = 'C:\'
    end
    object btnCopygrid: TButton
      Left = 912
      Top = 9
      Width = 139
      Height = 25
      Anchors = [akTop, akRight]
      Caption = 'Copy Grid to Clipboard'
      TabOrder = 2
      OnClick = btnCopygridClick
    end
  end
  object btnGetCopyright: TButton
    Left = 328
    Top = 8
    Width = 151
    Height = 25
    Caption = 'Get Copyright'
    TabOrder = 1
    OnClick = btnGetCopyrightClick
  end
  object btnLoadSBML: TButton
    Left = 8
    Top = 77
    Width = 121
    Height = 25
    Caption = 'Load SBML'
    TabOrder = 2
    OnClick = btnLoadSBMLClick
  end
  object grid: TStringGrid
    Left = 646
    Top = 0
    Width = 422
    Height = 667
    Align = alRight
    Anchors = [akLeft, akTop, akBottom]
    DefaultColWidth = 84
    FixedCols = 0
    TabOrder = 3
  end
  object btnGetAvailableSymbols: TButton
    Left = 328
    Top = 62
    Width = 151
    Height = 25
    Caption = 'Get Available Symbols'
    TabOrder = 4
    OnClick = btnGetAvailableSymbolsClick
  end
  object btnSteadyState: TButton
    Left = 8
    Top = 129
    Width = 121
    Height = 25
    Caption = 'Steady State'
    TabOrder = 5
    OnClick = btnSteadyStateClick
  end
  object btnLoadTwoModels: TButton
    Left = 328
    Top = 196
    Width = 151
    Height = 25
    Caption = 'Load Two Models Test'
    TabOrder = 6
    OnClick = btnLoadTwoModelsClick
  end
  object chkConservationLaws: TCheckBox
    Left = 183
    Top = 9
    Width = 139
    Height = 17
    Caption = 'Use Conservation Laws'
    Checked = True
    State = cbChecked
    TabOrder = 7
    OnClick = chkConservationLawsClick
  end
  object btnSimulate: TButton
    Left = 8
    Top = 103
    Width = 121
    Height = 25
    Caption = 'Simulate'
    TabOrder = 8
    OnClick = btnSimulateClick
  end
  object btnGetCode: TButton
    Left = 328
    Top = 35
    Width = 151
    Height = 25
    Caption = 'Get Generated Code'
    TabOrder = 9
    OnClick = btnGetCodeClick
  end
  object PageControl1: TPageControl
    Left = 8
    Top = 256
    Width = 471
    Height = 405
    ActivePage = TabSheet1
    Anchors = [akLeft, akTop, akBottom]
    TabOrder = 10
    object TabSheet1: TTabSheet
      Caption = 'List Box'
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object lstSummary: TListBox
        Left = 0
        Top = 0
        Width = 463
        Height = 377
        Align = alClient
        ItemHeight = 13
        TabOrder = 0
      end
    end
    object TabSheet2: TTabSheet
      Caption = 'Source Code'
      ImageIndex = 1
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object MemoSource: TMemo
        Left = 0
        Top = 0
        Width = 463
        Height = 377
        Align = alClient
        ScrollBars = ssBoth
        TabOrder = 0
      end
    end
    object TabSheet3: TTabSheet
      Caption = 'Header File'
      ImageIndex = 2
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object memoHeader: TMemo
        Left = 0
        Top = 0
        Width = 463
        Height = 377
        Align = alClient
        ScrollBars = ssBoth
        TabOrder = 0
      end
    end
    object TabSheetCapabilities: TTabSheet
      Caption = 'Capabilities'
      ImageIndex = 3
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object memoCapabilities: TMemo
        Left = 0
        Top = 0
        Width = 463
        Height = 377
        Align = alClient
        TabOrder = 0
      end
    end
  end
  object btnSetFloatingSpeciesByIndex: TButton
    Left = 328
    Top = 116
    Width = 151
    Height = 25
    Caption = 'Set Float Species Index'
    TabOrder = 11
    OnClick = btnSetFloatingSpeciesByIndexClick
  end
  object btnSetBoundarySpeciesByIndex: TButton
    Left = 328
    Top = 142
    Width = 151
    Height = 25
    Caption = 'Set Boundary Species Index'
    TabOrder = 12
    OnClick = btnSetBoundarySpeciesByIndexClick
  end
  object edtCommonFloat: TEdit
    Left = 8
    Top = 220
    Width = 121
    Height = 21
    TabOrder = 13
    Text = '0.0'
  end
  object edtCommonInteger: TEdit
    Left = 8
    Top = 176
    Width = 121
    Height = 21
    TabOrder = 14
    Text = '0'
  end
  object btnDisplayModelSumamryByGetValue: TButton
    Left = 135
    Top = 77
    Width = 130
    Height = 36
    Caption = 'Display Model Summary by getValue'
    TabOrder = 15
    WordWrap = True
    OnClick = btnDisplayModelSumamryByGetValueClick
  end
  object Button1: TButton
    Left = 328
    Top = 169
    Width = 151
    Height = 25
    Caption = 'Set Global Parameter Index'
    TabOrder = 16
    OnClick = Button1Click
  end
  object btnGetGlobalParameterIndex: TButton
    Left = 485
    Top = 88
    Width = 151
    Height = 25
    Caption = 'Get Global Parameter Index'
    TabOrder = 17
    OnClick = btnGetGlobalParameterIndexClick
  end
  object btnGetFloatingSpeciesByIndex: TButton
    Left = 485
    Top = 35
    Width = 151
    Height = 25
    Caption = 'Get Float Species Index'
    TabOrder = 18
    OnClick = btnGetFloatingSpeciesByIndexClick
  end
  object btnGetBoundarySpeciesByIndex: TButton
    Left = 485
    Top = 62
    Width = 151
    Height = 25
    Caption = 'Get Boundary Species Index'
    TabOrder = 19
    OnClick = btnGetBoundarySpeciesByIndexClick
  end
  object btnGetSBML: TButton
    Left = 135
    Top = 153
    Width = 130
    Height = 25
    Caption = 'Get SBML and Compare'
    TabOrder = 20
    OnClick = btnGetSBMLClick
  end
  object btnGetCompartmentVolumeByIndex: TButton
    Left = 485
    Top = 8
    Width = 151
    Height = 25
    Caption = 'Get Compartment Vol Index'
    TabOrder = 21
    OnClick = btnGetCompartmentVolumeByIndexClick
  end
  object btnSetCompartmentVolumeByIndex: TButton
    Left = 328
    Top = 89
    Width = 151
    Height = 25
    Caption = 'Set Compartment by Index'
    TabOrder = 22
    OnClick = btnSetCompartmentVolumeByIndexClick
  end
  object btnDisplayModelSumamryByGetIndex: TButton
    Left = 135
    Top = 115
    Width = 130
    Height = 36
    Caption = 'Display Model Summary by getIndex'
    TabOrder = 23
    WordWrap = True
    OnClick = btnDisplayModelSumamryByGetIndexClick
  end
  object btnGetCapabilities: TButton
    Left = 135
    Top = 184
    Width = 130
    Height = 25
    Caption = 'Get Capabilities'
    TabOrder = 24
    OnClick = btnGetCapabilitiesClick
  end
  object btnEvalModel: TButton
    Left = 135
    Top = 218
    Width = 130
    Height = 25
    Caption = 'Eval Model'
    TabOrder = 25
    OnClick = btnEvalModelClick
  end
  object btnGetFullMatrix: TButton
    Left = 485
    Top = 142
    Width = 151
    Height = 25
    Caption = 'Get Full Jacobian'
    TabOrder = 26
    OnClick = btnGetFullMatrixClick
  end
  object btnGetReducedMatrix: TButton
    Left = 485
    Top = 169
    Width = 151
    Height = 25
    Caption = 'Get Reduced Jacobian'
    TabOrder = 27
    OnClick = btnGetReducedMatrixClick
  end
  object lstModelName: TListBox
    Left = 8
    Top = 7
    Width = 169
    Height = 64
    ItemHeight = 13
    Items.Strings = (
      '_test.xml'
      'ss_SimpleConservedCycle.xml'
      'ss_threeSpecies.xml'
      'feedback.xml'
      'squareWaveModel.xml'
      'oneNodeSimple.xml')
    TabOrder = 28
  end
  object btnGetStoichMatrix: TButton
    Left = 485
    Top = 256
    Width = 151
    Height = 25
    Caption = 'Get Stoichiometry Matrix'
    TabOrder = 29
    OnClick = btnGetStoichMatrixClick
  end
  object btnGetLinkMatrix: TButton
    Left = 485
    Top = 282
    Width = 151
    Height = 25
    Caption = 'Get Link Matrix'
    TabOrder = 30
    OnClick = btnGetLnkMatrixClick
  end
  object btnGetNrMatrix: TButton
    Left = 485
    Top = 308
    Width = 151
    Height = 25
    Caption = 'Get Nr Matrix'
    TabOrder = 31
    OnClick = btnGetNrMatrixClick
  end
  object btnGetConservationMatrix: TButton
    Left = 485
    Top = 361
    Width = 151
    Height = 25
    Caption = 'Get Conservation Matrix'
    TabOrder = 32
    OnClick = btnGetConservationMatrixClick
  end
  object Button2: TButton
    Left = 485
    Top = 413
    Width = 151
    Height = 25
    Caption = 'SimulateEx'
    TabOrder = 33
    OnClick = Button2Click
  end
  object edtTimeStart: TEdit
    Left = 568
    Top = 444
    Width = 68
    Height = 21
    TabOrder = 34
    Text = '0.0'
  end
  object edtTimeEnd: TEdit
    Left = 568
    Top = 471
    Width = 68
    Height = 21
    TabOrder = 35
    Text = '10.0'
  end
  object edtNumberOfPoints: TEdit
    Left = 568
    Top = 498
    Width = 68
    Height = 21
    TabOrder = 36
    Text = '10'
  end
  object btnGetL0Matrix: TButton
    Left = 485
    Top = 334
    Width = 151
    Height = 25
    Caption = 'Get L0 Matrix'
    TabOrder = 37
    OnClick = btnGetL0MatrixClick
  end
end
