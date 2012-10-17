unit uIMSLLib;

// Delphi interface to the IMSL library DLL

interface

uses Windows, SysUtils, Dialogs, uJVector, uComplex, uMatrix;

type
  TdArray = array[0..10000] of double;
  pdArray = ^TdArray;

  TLevenbergFunction = procedure (var M, N : integer; X, F : pdArray); stdcall;

function  LoadDLL (var msg : string) : boolean;
procedure Release;

procedure computeEigen (A : TCMatrix; eigenvalues : TCVector; eigenvectors : TCMatrix);
procedure computeSVD (A : TCMatrix; W : TCVector; U, V : TCMatrix);
procedure computeLUDecomp (A, LU : TCMatrix; index : TIVector; var rcond : double);
procedure computeInverse (A, Inv : TCMatrix);

procedure LevenbergMarquard (fcn : TLevenbergFunction; nFunctions, nVariables : integer;
               XGuessVector, XFitted, XResiduals : TCVector; var ErrMsg : string);

implementation


type
  TcdArray = array[0..10000] of TComplex;
  pcdArray = ^TcdArray;

  TiArray = array[0..10000] of integer;
  piArray = ^TiArray;

  TComputeEigen = procedure (var n : integer; mat : pcdArray; eigenval : pcdArray; eigenvec : pcdArray; var ICODE, IERROR : integer); stdcall;
  TComputeSVD = procedure (var NRA, NCA : integer; mat : pcdArray; S : pcdArray; U, V : pcdArray; var ICODE, IERROR : integer); stdcall;
  TComputeLU = procedure (var NRA : integer; mat, LU : pcdArray; ipvt : piArray; var rcond : double; var ICODE, IERROR : integer); stdcall;
  TComputeInverse = procedure (var N : integer; mat, Inv : pcdArray; var ICODE, IERROR : integer); stdcall;

  TLevenbergInternalJacobian = procedure (FCN : TLevenbergFunction; var M, N : integer;
                    XGUESS, XSCALE, FSCALE : pdArray;
                    IPARAM : piArray; RPARAM : pdArray; X, FVEC : pdArray; FJAC : pdArray;
                    var LDFJAC, ICODE, IERROR : integer); stdcall;

var DLLHandle : THandle;
    IMSLComputeEigen   : TComputeEigen = nil;
    IMSLComputeSVD     : TComputeSVD = nil;
    IMSLComputeLU      : TComputeLU = nil;
    IMSLComputeInverse : TComputeInverse = nil;

    IMSLLevenbergInternalJacobian : TLevenbergInternalJacobian;

    DLLLoaded : boolean;

function LoadDLL (var msg : string) : Boolean;
begin
  DLLHandle := LoadLibrary ('computeIMSLDLL.DLL');
  if DLLHandle <> 0 then
     begin
     @IMSLcomputeEigen := getProcAddress(DLLHandle, 'COMPUTEEIGEN');
     if @IMSLcomputeEigen = nil then
        begin result := False; exit; end
     else Result := True;

     @IMSLcomputeSVD := getProcAddress (DLLHandle, 'COMPUTESVD');
     if @IMSLcomputeSVD = nil then
        begin
        result := False;
        msg := 'Unable to locate eigen function in computeIMSL DLL';
        exit;
        end
     else Result := True;

     @IMSLComputeLU := getProcAddress (DLLHandle, 'COMPUTELU');
     if @IMSLComputeLU = nil then
        begin
        result := False;
        msg := 'Unable to locate LU Decomposition function in computeIMSL DLL';
        exit;
        end
     else Result := True;

     @IMSLComputeInverse := getProcAddress (DLLHandle, 'COMPUTEINVERSE');
     if @IMSLComputeInverse = nil then
        begin
        result := False;
        msg := 'Unable to locate Inverse function in computeIMSL DLL';
        exit;
        end
     else Result := True;

     @IMSLLevenbergInternalJacobian := getProcAddress (DLLHandle, 'LEVENBERGMARQUARD');
     if @IMSLLevenbergInternalJacobian = nil then
        begin
        result := False;
        msg := 'Unable to locate Levenberg-Marquard function in computeIMSL DLL';
        exit;
        end
     else Result := True;

     end
  else
     begin
     msg := 'Unable to locate eigen DLL';
     Result := False;
     end;
end;


procedure Release;
begin
  FreeLibrary(DLLHandle);
end;


// -------------------------------------------------------------------------

function LoadIntoIMSLMatrix (A : TCMatrix) : pcdArray;
var i, j, k, nr, nc : integer;
begin
  nr := A.r; nc := A.c;
  Result := AllocMem (nr * nc * sizeof (TComplex));
  for j := 0 to nc - 1 do
      for i := 0 to nr - 1 do
          begin
          k := (j*nr) + i;
          if abs (A[i+1,j+1].r) < 1E-14 then
             A[i+1,j+1] := complex (0.0, A[i+1,j+1].i);
          Result[k] := A[i+1,j+1];
          end;
end;


procedure copyMatrixFromIMSL (dest : TCMatrix; src : pcdArray);
var i, j, k, nr, nc : integer;
begin
  nr := dest.r; nc := dest.c;
  for j := 0 to nc - 1 do
      begin
      for i := 0 to nr - 1 do
          begin
          k := (j*nc) + i;
          dest[i+1,j+1] := src [k];
          end;
      end;
end;


procedure copyVectorFromIMSL (dest : TCVector; src : pcdArray); overload;
var n : integer; ptr : Pointer;
begin
  n := dest.Size;
  ptr := dest.getPtr;
  move (src^, ptr^, sizeOf (TComplex) * n);
end;


procedure copyVectorFromIMSL (dest : TCVector; src : pdArray); overload;
var n : integer; ptr : Pointer;
begin
  n := dest.Size;
  ptr := dest.getPtr;
  move (src^, ptr^, sizeOf (double) * n);
end;


// -----------------------------------------------------------------------


procedure computeEigen (A : TCMatrix; eigenvalues : TCVector; eigenvectors : TCMatrix);
var n, i, j, k : integer; IMSL_eigenval, IMSL_eigenvec : pcdArray; mat : pcdArray;
    ICode, IType : integer;
begin
  if DLLHandle = 0 then
     raise Exception.Create ('IMSL Library not loaded, check that the dll is available');
  if A.r <> A.c then
     raise Exception.Create ('Matrix must be square for eigenvalue computation');

  n := A.r;
  try
    IMSL_eigenval := AllocMem (n * sizeof(TComplex));
    IMSL_eigenvec := AllocMem (n * n * sizeof(TComplex));

    mat := LoadIntoIMSLMatrix (A);

    IMSLcomputeEigen (n, mat, IMSL_eigenval, IMSL_eigenvec, ICode, IType);

    if IType = 4 then
       raise Exception.Create ('The iteration for the eigenvalues failed to converge.');
       
    copyVectorFromIMSL (eigenvalues, IMSL_eigenval);
    copyMatrixFromIMSL (eigenvectors, IMSL_eigenvec);
  finally
    FreeMem (mat);
    FreeMem (IMSL_eigenvec);
    Freemem (IMSL_eigenval);
  end;
end;


procedure computeSVD (A : TCMatrix; W : TCVector; U, V : TCMatrix);
var nr, nc, i, j, k : integer; iU, iV, iW : pcdArray; mat : pcdArray;
    ICode, IType : integer;
begin
  if DLLHandle = 0 then
     raise Exception.Create ('IMSL Library not loaded, check that the dll is available');

  try
    try
      nr := A.r;
      nc := A.c;
      iW := AllocMem (W.size * sizeof(TComplex));
      iU := AllocMem (U.r * U.c * sizeof(TComplex));
      iV := AllocMem (V.r * V.c * sizeof(TComplex));

      mat := LoadIntoIMSLMatrix (A);

      IMSLcomputeSVD (nr, nc, mat, iW, iU, iV, ICode, IType);

      if IType = 4 then
         raise Exception.Create ('Convergence cannot be achieved for all the singular values and their corresponding singular vectors.');

      copyVectorFromIMSL (W, iW);

      // U and V
      copyMatrixFromIMSL (U, iU);
      copyMatrixFromIMSL (V, iV);
    finally
      FreeMem (iU);
      FreeMem (iV);
      FreeMem (iW);
      FreeMem (mat);
    end;
  except
    on Exception do
       raise Exception.Create ('Memory error in SVD');
  end;
end;


procedure computeLUDecomp (A, LU : TCMatrix; index : TIVector; var rcond : double);
var mat, IMSL_LU : pcdArray; nr, nc, i : integer; ipvt: piArray; ICode, IType : integer;
begin
  if DLLHandle = 0 then
     raise Exception.Create ('IMSL Library not loaded, check that the dll is available');
  if A.r <> A.c then
     raise Exception.Create ('Matrix must be square for LU decomposition');

  try
    nr := A.r; nc := A.c;
    mat := LoadIntoIMSLMatrix (A);
    IMSL_LU := AllocMem (nr * nc * sizeof (TComplex));
    ipvt := AllocMem (nr * sizeof (Integer));

    IMSLComputeLU (nr, mat, IMSL_LU, ipvt, rcond, ICode, IType);

    if IType = 3 then
       raise Exception.Create ('Input matrix is algorithmically singular.');

    if IType = 4 then
       raise ESingularMatrix.Create ('The input matrix is singular.');

    rcond := 1/rcond;

    copyMatrixFromIMSL (LU, IMSL_LU);
    for i := 0 to nr - 1 do
        index[i+1] := ipvt[i];

  finally
    FreeMem (mat);
    FreeMem (IMSL_LU);
    FreeMem (ipvt);
  end;
end;


procedure computeInverse (A, Inv : TCMatrix);
var n : integer; inputMat, outputMat : pcdArray; ICode, IType : integer;
begin
  if DLLHandle = 0 then
     raise Exception.Create ('IMSL Library not loaded, check that the dll is available');
  if A.r <> A.c then
     raise Exception.Create ('Matrix must be square for inversion');

  try
    n := A.r;
    try
    inputMat := LoadIntoIMSLMatrix (A);
    outputMat := AllocMem (n * n * sizeof (TComplex));

    IMSLcomputeInverse (n, inputMat, outputMat, ICode, IType);
    except
      on E: exception do
         showmessage (e.message);
    end;
    case IType of
       3 : raise Exception.Create ('The input matrix is too ill-conditioned. The inverse might not be accurate');
       4 : raise ESingularMatrix.Create ('The input matrix is singular.');
    end;

    copyMatrixFromIMSL (Inv, outputMat);
  finally
    FreeMem (inputMat);
    FreeMem (outputMat);
  end;
end;


// Values for IPARAM

// IPARAM(1) = Initialization flag.
// IPARAM(2) = Number of good digits in the function.
// Default: Machine dependent.
// IPARAM(3) = Maximum number of iterations.
// Default: 100.
// IPARAM(4) = Maximum number of function evaluations.
// Default: 400.
// IPARAM(5) = Maximum number of Jacobian evaluations.
// Default: Not used in UNLSF.
// IPARAM(6) = Internal variable scaling flag.
// If IPARAM(6) = 1, then the values for XSCALE are set internally.
// Default: 1.

// Values for RPARAM

// RPARAM — Real vector of length 7.
// RPARAM(1) = Scaled gradient tolerance.
// RPARAM(2) = Scaled step tolerance. (STEPTL)
// RPARAM(3) = Relative function tolerance.
// RPARAM(4) = Absolute function tolerance.
// RPARAM(5) = False convergence tolerance.
// Default: 100*epsilon where epsilon is the machine precision.
// RPARAM(6) = Maximum allowable step size.
// Default: 1000 max(e1, e2) where
// RPARAM(7) = Size of initial trust region radius.
// Default: based on the initial scaled Cauchy step.


// XFitted and XResiduals are outputs
// XGuessVector is an input
// fcn is the objective function
procedure LevenbergMarquard (fcn : TLevenbergFunction; nFunctions, nVariables : integer;
              XGuessVector, XFitted, XResiduals : TCVector; var ErrMsg : string);
var IPARAM : piArray; RPARAM, XSCALE, XGUESS, FSCALE, X, FVEC, FJAC : pdArray;
    i, LDFJAC, ICODE, ITYPE : integer;
begin
  ErrMsg := '';
  IPARAM := AllocMem (6 * sizeof (integer));
  RPARAM := AllocMem (7 * sizeof (double));
  XSCALE := AllocMem (nVariables * sizeof (double));
  XGUESS := AllocMem (nVariables * sizeof (double));
  for i := 0 to nVariables - 1 do
      XGUESS[i] := XGuessVector[i+1].r;
  FSCALE := AllocMem (nFunctions * sizeof (double));
  for i := 0 to nFunctions - 1 do
      FSCALE[i] := 1.0;
  X := AllocMem (nVariables * Sizeof (double));
  FVEC := AllocMem (nFunctions * Sizeof (double));
  LDFJAC := nFunctions;
  FJAC := AllocMem (nFunctions * nVariables * sizeof (double));

  IPARAM[0] := 0; IPARAM[1] := 0; IPARAM[2] := 0; IPARAM[3] := 0; IPARAM[4] := 0; IPARAM[5] := 0;
  RPARAM[0] := 0.0; RPARAM[1] := 0.0; RPARAM[2] := 0.0;
  RPARAM[3] := 0.0; RPARAM[4] := 0.0; RPARAM[5] := 0.0; RPARAM[6] := 0.0;

  try
     IMSLLevenbergInternalJacobian (fcn, nFunctions, nVariables, XGUESS, XSCALE,
                 FSCALE, IPARAM, RPARAM, X, FVEC, FJAC, LDFJAC, ICODE, ITYPE);

     case IType of
       2 : begin
           case ICode of
           7 : ErrMsg := 'Scaled step tolerance satisfied; the current point may ' +
                              'be an approximate local solution, or the algorithm is ' +
                              'making very slow progress and is not near a solution, ' +
                              'or STEPTL is too big.';
           end;
           end;

       3 : begin
           case ICode of
              1 : ErrMsg := 'Both the actual and predicted relative reductions in the ' +
                          'function are less than or equal to the relative function ' +
                          'convergence tolerance.';
              2 : ErrMsg := 'The iterates appear to be converging to a noncritical point.';
              6 : ErrMsg := 'Five consecutive steps have been taken with the maximum step length.';
           end;
           end;

       4 : begin
           case ICode of
             3 : ErrMsg := 'Maximum number of iterations exceeded.';
             4 : ErrMsg := 'Maximum number of function evaluations exceeded';
           end;
           end;
       5 : begin
           ErrMsg := 'Fatal error calling Levenberg-Marquardt routine, contact author';
           end;
    end;

     copyVectorFromIMSL (XFitted, X);
     copyVectorFromIMSL (XResiduals, FVEC);
  finally
    FreeMem (IPARAM);
    FreeMem (RPARAM);
    FreeMem (XSCALE);
    FreeMem (XGUESS);
    FreeMem (FSCALE);
    FreeMem (X);
    FreeMem (FVEC);
    FreeMem (FJAC);
  end;
end;


initialization
  DLLHandle := 0;
  IMSLcomputeEigen := nil;
  IMSLcomputeSVD := nil;
  IMSLcomputeLU := nil;
  IMSLcomputeInverse := nil;
  IMSLLevenbergInternalJacobian := nil;
end.
