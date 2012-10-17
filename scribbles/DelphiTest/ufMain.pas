unit ufMain;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, uRoadRunnerAPI, ExtCtrls, IOUtils, Grids, ComCtrls,
  Clipbrd;

type
  TfrmMain = class(TForm)
    pnlBottm: TPanel;
    btnGetCopyright: TButton;
    btnLoadSBML: TButton;
    grid: TStringGrid;
    btnGetAvailableSymbols: TButton;
    btnSteadyState: TButton;
    btnLoadTwoModels: TButton;
    Label1: TLabel;
    lblTempFolder: TEdit;
    chkConservationLaws: TCheckBox;
    btnSimulate: TButton;
    btnGetCode: TButton;
    PageControl1: TPageControl;
    TabSheet1: TTabSheet;
    lstSummary: TListBox;
    TabSheet2: TTabSheet;
    MemoSource: TMemo;
    TabSheet3: TTabSheet;
    memoHeader: TMemo;
    btnSetFloatingSpeciesByIndex: TButton;
    btnSetBoundarySpeciesByIndex: TButton;
    edtCommonFloat: TEdit;
    lblCommon: TLabel;
    edtCommonInteger: TEdit;
    Label2: TLabel;
    btnDisplayModelSumamryByGetValue: TButton;
    Button1: TButton;
    btnGetGlobalParameterIndex: TButton;
    btnGetFloatingSpeciesByIndex: TButton;
    btnGetBoundarySpeciesByIndex: TButton;
    edtProgress: TEdit;
    btnGetSBML: TButton;
    btnGetCompartmentVolumeByIndex: TButton;
    btnSetCompartmentVolumeByIndex: TButton;
    btnDisplayModelSumamryByGetIndex: TButton;
    TabSheetCapabilities: TTabSheet;
    btnGetCapabilities: TButton;
    memoCapabilities: TMemo;
    btnEvalModel: TButton;
    btnGetFullMatrix: TButton;
    btnGetReducedMatrix: TButton;
    lstModelName: TListBox;
    btnGetStoichMatrix: TButton;
    btnGetLinkMatrix: TButton;
    btnGetNrMatrix: TButton;
    btnGetConservationMatrix: TButton;
    Button2: TButton;
    edtTimeStart: TEdit;
    edtTimeEnd: TEdit;
    edtNumberOfPoints: TEdit;
    Label3: TLabel;
    Label4: TLabel;
    Label5: TLabel;
    btnGetL0Matrix: TButton;
    btnCopygrid: TButton;
    Label6: TLabel;
    Label7: TLabel;
    procedure btnGetCopyrightClick(Sender: TObject);
    procedure btnLoadSBMLClick(Sender: TObject);
    procedure btnGetAvailableSymbolsClick(Sender: TObject);
    procedure btnSteadyStateClick(Sender: TObject);
    procedure btnLoadTwoModelsClick(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure chkConservationLawsClick(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure btnSimulateClick(Sender: TObject);
    procedure btnGetCodeClick(Sender: TObject);
    procedure btnSetFloatingSpeciesByIndexClick(Sender: TObject);
    procedure btnDisplayModelSumamryByGetValueClick(Sender: TObject);
    procedure btnSetBoundarySpeciesByIndexClick(Sender: TObject);
    procedure btnGetGlobalParameterIndexClick(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure btnGetFloatingSpeciesByIndexClick(Sender: TObject);
    procedure btnGetBoundarySpeciesByIndexClick(Sender: TObject);
    procedure btnGetSBMLClick(Sender: TObject);
    procedure btnGetCompartmentVolumeByIndexClick(Sender: TObject);
    procedure btnSetCompartmentVolumeByIndexClick(Sender: TObject);
    procedure btnDisplayModelSumamryByGetIndexClick(Sender: TObject);
    procedure btnGetCapabilitiesClick(Sender: TObject);
    procedure btnEvalModelClick(Sender: TObject);
    procedure btnGetFullMatrixClick(Sender: TObject);
    procedure btnGetReducedMatrixClick(Sender: TObject);
    procedure btnGetStoichMatrixClick(Sender: TObject);
    procedure btnGetLnkMatrixClick(Sender: TObject);
    procedure btnGetNrMatrixClick(Sender: TObject);
    procedure btnGetConservationMatrixClick(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure btnGetL0MatrixClick(Sender: TObject);
    procedure btnCopygridClick(Sender: TObject);
    procedure lblTempFolderKeyDown(Sender: TObject; var Key: Word;
      Shift: TShiftState);
  private
    { Private declarations }
    procedure getSummaryOfModelByIndex;
    procedure getSummaryOfModelByGetValue;
  public
    { Public declarations }
    rrInstance: Pointer;
    loadedSBMLStr: AnsiString;
  end;

var
  frmMain: TfrmMain;

implementation

{$R *.dfm}

uses uMatrix, uRRList;

function MemoryUsed: integer;
var
    st: TMemoryManagerState;
    sb: TSmallBlockTypeState;
begin
    GetMemoryManagerState(st);
    result := st.TotalAllocatedMediumBlockSize + st.TotalAllocatedLargeBlockSize;
    for sb in st.SmallBlockTypeStates do begin
        result := result + sb.UseableBlockSize * sb.AllocatedBlockCount;
    end;
end;


procedure TfrmMain.btnGetCapabilitiesClick(Sender: TObject);
begin
  memoCapabilities.Lines.Text := getCapabilities;
end;

procedure TfrmMain.btnGetCodeClick(Sender: TObject);
var
  c: TRRCCode;
begin
  lstSummary.Clear;
  c := getCCode;
  MemoSource.text := StringReplace(c.Source, #10, #13#10, [rfReplaceAll]);
  MemoHeader.text := StringReplace(c.Header, #10, #13#10, [rfReplaceAll]);
end;

procedure TfrmMain.btnGetCompartmentVolumeByIndexClick(Sender: TObject);
begin
  edtCommonFloat.Text :=
    floattostr(getCompartmentByIndex((strtoint(edtCommonInteger.Text))));
end;

procedure TfrmMain.btnGetConservationMatrixClick(Sender: TObject);
var
  m: TMatrix;
  i, j: integer;
begin
  m := getConservationMatrix;
  for i := 0 to grid.RowCount - 1 do
    for j := 0 to grid.ColCount - 1 do
      grid.Cells[j, i] := '';

  grid.ColCount := m.c + 1;
  grid.RowCount := m.r + 1;
  for i := 1 to m.r do
    for j := 1 to m.c do
      grid.Cells[j - 1, i] := Format('%8.5g', [m[i, j]]);
end;

procedure TfrmMain.btnGetCopyrightClick(Sender: TObject);
begin
  edtProgress.text := string(getCopyright);
end;

procedure TfrmMain.btnGetFloatingSpeciesByIndexClick(Sender: TObject);
begin
  edtCommonFloat.Text :=
    floattostr(getFloatingSpeciesByIndex((strtoint(edtCommonInteger.Text))));
end;

procedure TfrmMain.btnGetFullMatrixClick(Sender: TObject);
var
  m: TMatrix;
  i, j: integer;
begin
  m := getFullJacobian;
  for i := 0 to grid.RowCount - 1 do
    for j := 0 to grid.ColCount - 1 do
      grid.Cells[j, i] := '';

  grid.ColCount := m.c + 1;
  grid.RowCount := m.r + 1;
  for i := 1 to m.r do
    for j := 1 to m.c do
      grid.Cells[j - 1, i] := Format('%8.5g', [m[i, j]]);
end;

procedure TfrmMain.btnGetGlobalParameterIndexClick(Sender: TObject);
begin
  edtCommonFloat.Text :=
    floattostr(getGlobalParameterByIndex((strtoint(edtCommonInteger.Text))));
end;

procedure TfrmMain.btnGetReducedMatrixClick(Sender: TObject);
var
  m: TMatrix;
  i, j: integer;
begin
  m := getReducedJacobian;
  for i := 0 to grid.RowCount - 1 do
    for j := 0 to grid.ColCount - 1 do
      grid.Cells[j, i] := '';

  grid.ColCount := m.c + 1;
  grid.RowCount := m.r + 1;
  for i := 1 to m.r do
    for j := 1 to m.c do
      grid.Cells[j - 1, i] := Format('%8.5g', [m[i, j]]);
end;

procedure TfrmMain.btnGetSBMLClick(Sender: TObject);
var
  str: AnsiString;
begin
  str := getSBML;
  if str = loadedSBMLStr then
    showmessage('SBML Identical')
  else
    showmessage('SBML not the same');
end;

procedure TfrmMain.btnGetStoichMatrixClick(Sender: TObject);
var
  m: TMatrix;
  i, j: integer;
begin
  m := getStoichiometryMatrix;
  for i := 0 to grid.RowCount - 1 do
    for j := 0 to grid.ColCount - 1 do
      grid.Cells[j, i] := '';

  grid.ColCount := m.c + 1;
  grid.RowCount := m.r + 1;
  for i := 1 to m.r do
    for j := 1 to m.c do
      grid.Cells[j - 1, i] := Format('%8.5g', [m[i, j]]);
end;

procedure TfrmMain.btnLoadSBMLClick(Sender: TObject);
var
  m: TMatrix;
  i, j: integer;
  list: TStringList;
begin
  rrInstance := getRRInstance;
  setComputeAndAssignConservationLaws(chkConservationLaws.checked);
  lstSummary.Clear;

  loadedSBMLStr := AnsiString(TFile.ReadAllText(lstModelName.Items[lstModelName.ItemIndex]));
  if not loadSBML(loadedSBMLStr) then
  begin
    edtProgress.text := 'Failed to load SBML model';
    exit;
  end;
  label6.caption := inttostr (MemoryUsed);
end;

procedure TfrmMain.btnSetBoundarySpeciesByIndexClick(Sender: TObject);
var
  index: integer;
  value: double;
begin
  index := strtoint(edtCommonInteger.Text);
  value := strtofloat(edtCommonFloat.Text);
  if not setBoundarySpeciesByIndex(index, value) then
    showmessage('No such boundary species');
end;

procedure TfrmMain.btnSetCompartmentVolumeByIndexClick(Sender: TObject);
var
  value: double;
  index: integer;
begin
  index := strtoint(edtCommonInteger.Text);
  value := strtofloat(edtCommonFloat.Text);
  setCompartmentByIndex(index, value);
end;

procedure TfrmMain.btnSetFloatingSpeciesByIndexClick(Sender: TObject);
var
  value: double;
  index: integer;
begin
  index := strtoint(edtCommonInteger.Text);
  value := strtofloat(edtCommonFloat.Text);
  setFloatingSpeciesByIndex(index, value);
end;

procedure TfrmMain.getSummaryOfModelByIndex;
var
  list: TStringList;
  i: integer;
  d: TDoubleArray;
begin
  lstSummary.Clear;
  lstSummary.Items.Add('Number of Compartments: ' +
    inttostr(getNumberOfCompartments));
  lstSummary.Items.Add('Number of Reactions: ' +
    inttostr(getNumberOfReactions));
  lstSummary.Items.Add('Number of Boundary Species: ' +
    inttostr(getNumberOfBoundarySpecies));
  lstSummary.Items.Add('Number of Floating Species: ' +
    inttostr(getNumberOfFloatingSpecies));
  lstSummary.Items.Add('Number of Global Parameters: ' +
    inttostr(getNumberOfGlobalParameters));
  lstSummary.Items.Add('');

  lstSummary.Items.Add('GetNumber of Dependent Species: ' +
    inttostr(getNumberOfDependentSpecies));
  lstSummary.Items.Add('GetNumber of Independent Species: ' +
    inttostr(getNumberOfInDependentSpecies));
  lstSummary.Items.Add('');

  //evalModel;  // <- should we need to do this?

  list := getCompartmentIds;
  for i := 0 to list.Count - 1 do
    lstSummary.Items.Add('Compartment Name: ' + list[i] + ' (' +
      floattostr(getCompartmentByIndex(i)) + ')');
  list.Free;
  lstSummary.Items.Add('');

  list := getReactionIds;
  d := getReactionRates;
  for i := 0 to list.Count - 1 do
    lstSummary.Items.Add('Reaction Name: ' + list[i] + '   Reaction Rate=' +
      floattostr(d[i]));
  list.Free;
  setLength(d, 0);
  lstSummary.Items.Add('');

  list := getBoundarySpeciesIds;
  for i := 0 to list.Count - 1 do
    lstSummary.Items.Add('Boundary Species Name: ' + list[i] + ' (' +
      floattostr(getBoundarySpeciesByIndex(i)) + ')');
  if list.Count > 0 then
    lstSummary.Items.Add('');
  list.Free;

  list := getFloatingSpeciesIds;
  for i := 0 to list.Count - 1 do
    lstSummary.Items.Add('Floating Species Name: ' + list[i] + ' (' +
      floattostr(getFloatingSpeciesByIndex(i)) + ')');
  list.Free;
  lstSummary.Items.Add('');

  list := getGlobalParameterIds;
  for i := 0 to list.Count - 1 do
    lstSummary.Items.Add('Global Parameter Name: ' + list[i] + ' (' +
      floattostr(getGlobalParameterByIndex(i)) + ')');
  lstSummary.Items.Add('');
  list.Free;

  list := getRatesOfChangeIds;
  d := getRatesOfChange;
  for i := 0 to list.Count - 1 do
    lstSummary.Items.Add('Rates of Change Name: ' + list[i] + ' (' +
      floattostr(d[i]) + ')');
  list.Free;
  setLength(d, 0);
  lstSummary.Items.Add('');

  list := getEigenValueIds;
  for i := 0 to list.Count - 1 do
    lstSummary.Items.Add('Eigenvalue Names: ' + list[i] + ' (' +
      floattostr(getValue(list[i])) + ')');
  lstSummary.Items.Add('');
  list.Free;

  list := getElasticityIds;
  for i := 0 to list.Count - 1 do
    lstSummary.Items.Add('Elasticity Names: ' + list[i] + ' (' +
      floattostr(getValue(list[i])) + ')');
  lstSummary.Items.Add('');
  list.Free;
end;

procedure TfrmMain.lblTempFolderKeyDown(Sender: TObject; var Key: Word;
  Shift: TShiftState);
begin
    if(Key = VK_RETURN) then
    begin
        //set temp folder from edit text...
    end;

end;

procedure TfrmMain.btnGetL0MatrixClick(Sender: TObject);
var
  m: TMatrix;
  i, j: integer;
begin
  m := getL0Matrix;
  for i := 0 to grid.RowCount - 1 do
    for j := 0 to grid.ColCount - 1 do
      grid.Cells[j, i] := '';

  grid.ColCount := m.c + 1;
  grid.RowCount := m.r + 1;
  for i := 1 to m.r do
    for j := 1 to m.c do
      grid.Cells[j - 1, i] := Format('%8.5g', [m[i, j]]);
end;


procedure TfrmMain.btnGetLnkMatrixClick(Sender: TObject);
var
  m: TMatrix;
  i, j: integer;
begin
  m := getLinkMatrix;
  for i := 0 to grid.RowCount - 1 do
    for j := 0 to grid.ColCount - 1 do
      grid.Cells[j, i] := '';

  grid.ColCount := m.c + 1;
  grid.RowCount := m.r + 1;
  for i := 1 to m.r do
    for j := 1 to m.c do
      grid.Cells[j - 1, i] := Format('%8.5g', [m[i, j]]);
end;

procedure TfrmMain.btnGetNrMatrixClick(Sender: TObject);
var
  m: TMatrix;
  i, j: integer;
begin
  m := getNrMatrix;
  for i := 0 to grid.RowCount - 1 do
    for j := 0 to grid.ColCount - 1 do
      grid.Cells[j, i] := '';

  grid.ColCount := m.c + 1;
  grid.RowCount := m.r + 1;
  for i := 1 to m.r do
    for j := 1 to m.c do
      grid.Cells[j - 1, i] := Format('%8.5g', [m[i, j]]);
end;

procedure TfrmMain.getSummaryOfModelByGetValue;
var
  list: TStringList;
  i: integer;
  d: TDoubleArray;
begin
  lstSummary.Clear;
  lstSummary.Items.Add('Number of Compartments: ' +
    inttostr(getNumberOfCompartments));
  lstSummary.Items.Add('Number of Reactions: ' +
    inttostr(getNumberOfReactions));
  lstSummary.Items.Add('Number of Boundary Species: ' +
    inttostr(getNumberOfBoundarySpecies));
  lstSummary.Items.Add('Number of Floating Species: ' +
    inttostr(getNumberOfFloatingSpecies));
  lstSummary.Items.Add('Number of Global Parameters: ' +
    inttostr(getNumberOfGlobalParameters));
  lstSummary.Items.Add('');

  lstSummary.Items.Add('GetNumber of Dependent Species: ' +
    inttostr(getNumberOfDependentSpecies));
  lstSummary.Items.Add('GetNumber of Independent Species: ' +
    inttostr(getNumberOfInDependentSpecies));
  lstSummary.Items.Add('');

  list := getCompartmentIds;
  for i := 0 to list.Count - 1 do
    lstSummary.Items.Add('Compartment Name: ' + list[i] + ' (' +
      floattostr(getValue(list[i])) + ')');
  list.Free;
  lstSummary.Items.Add('');

  list := getReactionIds;
  d := getReactionRates;
  for i := 0 to list.Count - 1 do
    lstSummary.Items.Add('Reaction Name: ' + list[i] + '   Reaction Rate=' +
      floattostr(d[i]));
  list.Free;
  setLength(d, 0);
  lstSummary.Items.Add('');

  list := getBoundarySpeciesIds;
  for i := 0 to list.Count - 1 do
    lstSummary.Items.Add('Boundary Species Name: ' + list[i] + ' (' +
      floattostr(getValue(list[i])) + ')');
  if list.Count > 0 then
    lstSummary.Items.Add('');
  list.Free;

  list := getFloatingSpeciesIds;
  for i := 0 to list.Count - 1 do
    lstSummary.Items.Add('Floating Species Name: ' + list[i] + ' (' +
      floattostr(getValue(list[i])) + ')');
  list.Free;
  lstSummary.Items.Add('');

  list := getGlobalParameterIds;
  for i := 0 to list.Count - 1 do
    lstSummary.Items.Add('Global Parameter Name: ' + list[i] + ' (' +
      floattostr(getValue(list[i])) + ')');
  lstSummary.Items.Add('');
  list.Free;

  list := getRatesOfChangeIds;
  for i := 0 to list.Count - 1 do
    lstSummary.Items.Add('Rates of Change Name: ' + list[i] + ' (' +
      floattostr(getValue(list[i])) + ')');
  lstSummary.Items.Add('');
  list.Free;

  list := getEigenValueIds;
  for i := 0 to list.Count - 1 do
    lstSummary.Items.Add('Eigenvalue Names: ' + list[i] + ' (' +
      floattostr(getValue(list[i])) + ')');
  lstSummary.Items.Add('');
  list.Free;

  list := getElasticityIds;
  for i := 0 to list.Count - 1 do
    lstSummary.Items.Add('Elasticity Names: ' + list[i] + ' (' +
      floattostr(getValue(list[i])) + ')');
  lstSummary.Items.Add('');
  list.Free;
end;

procedure TfrmMain.btnSimulateClick(Sender: TObject);
var
  i, j: integer;
  m: TMatrix;
begin
  m := simulate();
  label6.caption := inttostr (MemoryUsed);
  try
    grid.ColCount := m.c + 1;
    grid.RowCount := m.r + 1;
    for i := 1 to m.r do
      for j := 1 to m.c do
        grid.Cells[j - 1, i] := Format('%8.5g', [m[i, j]]);
  finally
    m.free;
  end;
end;

procedure TfrmMain.btnSteadyStateClick(Sender: TObject);
var
  d: double;
  fn: TStringList;
  i: integer;
begin
  d := steadyState;
  lstSummary.Clear;
  lstSummary.Items.Add('Steady State Report');
  lstSummary.Items.Add('-------------------');
  lstSummary.Items.Add(Format('Return value from steadyState() = %g', [d]));
  if d < 0 then
    lstSummary.Items.Add(getLastError);
  lstSummary.Items.Add('');
  lstSummary.Items.Add('Species Concentrations:');
  fn := getFloatingSpeciesIds;
  for i := 0 to fn.Count - 1 do
    lstSummary.Items.Add(fn[i] + ' = ' + floattostr(getValue(fn[i])));
  fn.free;
end;

procedure TfrmMain.Button1Click(Sender: TObject);
var
  index: integer;
  value: double;
begin
  index := strtoint(edtCommonInteger.Text);
  value := strtofloat(edtCommonFloat.Text);
  if not setGlobalParameterByIndex(index, value) then
    showmessage('No such global parameter');
end;

procedure TfrmMain.Button2Click(Sender: TObject);
var
  i, j: integer;
  m: TMatrix;
  s : TStringList;
begin
  try
    s := TStringList.Create;
    s.Add ('S1');
    s.Add ('S2');
    setSelectionList (s);
    m := simulateEx(strtofloat(edtTimeStart.Text),
      strtofloat(edtTimeEnd.Text), strtoint(edtNumberOfPoints.Text));
    try
      grid.ColCount := m.c + 1;
      grid.RowCount := m.r + 1;
      for i := 1 to m.r do
        for j := 1 to m.c do
          grid.Cells[j - 1, i] := Format('%8.5g', [m[i, j]]);
    finally
      m.Free;
    end;
  except
    on E: exception do
      showmessage(E.Message);
  end;
end;

procedure TfrmMain.chkConservationLawsClick(Sender: TObject);
var
  b: boolean;
begin
  if chkConservationLaws.checked then
    b := true
  else
    b := false;
  setComputeAndAssignConservationLaws(b);
end;

procedure TfrmMain.FormClose(Sender: TObject; var Action: TCloseAction);
begin
  freeRRInstance(rrInstance);
end;

procedure TfrmMain.FormCreate(Sender: TObject);
var
  errMsg: AnsiString;
  methodList: TStringList;
  i: integer;
begin
  methodList := TStringList.Create;
  if loadRoadRunner(errMsg, methodList) then
  begin
    rrInstance := getRRInstance;
    edtProgress.text := 'RoadRunner Loaded: ' + 'Build Date: ' + getBuildDate +
      ', Revision: ' + inttostr(getRevision);
    lblTempFolder.Text := getTempFolder;
  end
  else
  begin
    edtProgress.text := string(errMsg);
  end;
  for i := 0 to methodList.Count - 1 do
    lstSummary.Items.Add(methodList[i]);
  methodList.Free;
  lstModelName.ItemIndex := 0;

  setTempFolder('C:\\rrTemp');
  setLogLevel('info');
  enableLogging();
end;

procedure TfrmMain.btnLoadTwoModelsClick(Sender: TObject);
var
  instance: Pointer;
  str1, str2: string;
  b: boolean;
begin
  instance := getRRInstance;
  str1 := AnsiString(TFile.ReadAllText('feedback.xml'));
  b := loadSBML(str1);
  if b then
    showmessage('loadSBML successful (true)')
  else
    showmessage('Failed to load SBML (false)');
  if fileExists('simple.xml') then
  begin
    str2 := AnsiString(TFile.ReadAllText('simple.xml'));
    b := loadSBML(str2);
    if b then
      showmessage('loadSBML successful (true)')
  end
  else
    showmessage('Failed to find file');
end;

procedure TfrmMain.btnCopygridClick(Sender: TObject);
var
   str : string;
   C, R : Integer;
begin
  str := '';
  for R := 0 to Grid.RowCount - 1 do begin
    for C := 0 to Grid.ColCount - 1 do begin
      str := str + Grid.Cells[C, R];
      if C < Grid.ColCount - 1 then str := str + #9;
    end;
    if R < Grid.RowCount - 1 then str := str + #13#10;
  end;
  Clipboard.AsText := str;
end;

procedure TfrmMain.btnDisplayModelSumamryByGetIndexClick(Sender: TObject);
begin
  getSummaryOfModelByIndex;
end;

procedure TfrmMain.btnDisplayModelSumamryByGetValueClick(Sender: TObject);
begin
  getSummaryOfModelByGetValue;
end;

procedure TfrmMain.btnEvalModelClick(Sender: TObject);
begin
  evalModel();
end;

procedure TfrmMain.btnGetAvailableSymbolsClick(Sender: TObject);
var
  x, list: TRRList;
  i, j: integer;
begin
  lstSummary.Clear;
  x := getAvailableSymbols;
  for i := 0 to x.count - 1 do
  begin
    lstSummary.Items.Add(x[i].getList[0].getString); // Label
    if x[i].getList.count > 1 then
    begin
      list := x[i].getList;
      for j := 1 to list.Count - 1 do
        lstSummary.Items.Add(list[j].getString);
    end;
  end;
end;

procedure TfrmMain.btnGetBoundarySpeciesByIndexClick(Sender: TObject);
begin
  edtCommonFloat.Text :=
    floattostr(getBoundarySpeciesByIndex((strtoint(edtCommonInteger.Text))));
end;

end.

