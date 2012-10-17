program DelphiTestProject;

uses
  madExcept,
  madLinkDisAsm,
  madListHardware,
  madListProcesses,
  madListModules,
  Forms,
  ufMain in 'ufMain.pas' {frmMain},
  uRoadRunnerAPI in '..\..\Wrappers\Delphi\uRoadRunnerAPI.pas',
  uRRList in '..\..\Wrappers\Delphi\uRRList.pas';

{$R *.res}

begin
  Application.Initialize;
  Application.MainFormOnTaskbar := True;
  Application.CreateForm(TfrmMain, frmMain);
  Application.Run;
end.
