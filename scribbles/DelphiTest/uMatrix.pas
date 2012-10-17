Unit uMatrix;

// Basic Matrix Unit for Delphi, May 1996. Implemented using original
// iMAP C matrix library.


// Version number: 1.1
// Date:           15 November 1999

// Version number 2.2
// Date:          10 Feb 2002

// Specially made for Jarnac

interface

Uses SysUtils, Classes, uJVector, Dialogs, Clipbrd, Math, uComplex;


const
     SMALLNUMBER = 1E-12;


type EMatrixError     = class (Exception);
     EMatrixSizeError = class (EMatrixError);
     ESingularMatrix  = class (EMatrixError);
     ENonSquareMatrix = class (EMatrixError);
     TUnaryDoubleFunc = function (const x : Double) : Double;
     TBinaryDoubleFunc = function (x, y : Double) : Double;
     TReOrderSortOrder = procedure (i, j : integer) of object;

     TMatError = (Singular, NonSingular, NonSquare);

     TDMatElement = double;
     TCMatElement = TComplex;

     // A Matrix is made up of a set of rows of type TRow, pTRow is
     // a pointer to a single row and a matrix is a row of pTRows, this
     // allows arrays larger then 65K to be built, the max size of
     // a matrix is roughly 4096 MBytes
     TDRow = array[0..20000000-1] of TDMatElement; pTDRow = ^TDRow;
     // The TRows are collected together into a RowList
     TDRowList = array[0..200000000-1] of pTDRow;
     // Finally, pTRowList points to a RowList
     pTDRowList = ^TDRowList;


     TCRow = array[0..20000000-1] of TCMatElement; pTCRow = ^TCRow;
     // The TRows are collected together into a RowList
     TCRowList = array[0..200000000-1] of pTCRow;
     // Finally, pTRowList points to a RowList
     pTCRowList = ^TCRowList;



     // Used by svdfit, supplies basis functions at x
     BasisProc = procedure (x : TDMatElement; var BasisFunc : TDVector);
     TMatrix = class;

     TBaseMatrix = class (TObject)
          private
             InternalTolerance : Double;
             nr, nc : integer;  // Number of rows and columns
             FName : string;
             FRowNames, FColumnNames : TStringList; // Row and column labels
             class procedure copyColumnToRowNames (s, d : TBaseMatrix);
             class procedure copyRowToColumnNames (s, d : TBaseMatrix);
          public
             constructor  Create; overload;

             procedure CreateNameLists (ri, ci : integer);
             procedure DestroyNameLists;
             procedure setRowName (ri : integer; name : string);
             function  getRowName (ri : integer) : string;
             procedure setColumnName (ci : integer; name : string);
             function  getColumnName (ci : integer) : string;

             procedure CheckLimits (ri, ci : integer);
             function  IsSquare : boolean;
             function  InRange (ri, ci : integer) : Boolean;
             function  SameDimensions (m1, m2 : TBaseMatrix) : boolean;
             property  Tolerance : double read InternalTolerance write InternalTolerance;
             property  r : integer read nr;
             property  c : integer read nc;
             property  Name : string read FName write FName;
             property  rName[ri : integer] : string read getRowName write setRowname;
             property  cName[ci : integer] : string read getColumnName write setColumnname;

     end;

     TCMatrix = class (TBaseMatrix)
           private
              mx   : pTCRowList;  // pointer to a list of rows
              procedure FreeSpace;
              procedure SetSize (ri, ci : integer);
              function  ComputeRank (m : TCMatrix) : integer;
           public
              constructor  Create (r, c : integer); overload; virtual;
              constructor  Create (n : integer); overload; virtual;
              constructor  Create (c : integer; d : array of TCMatElement); overload; virtual;
              constructor  Create (n : integer; c : TComplex); overload; virtual;
              constructor  Create (n, m : integer; c : TComplex); overload; virtual;
              constructor  Clone (m : TCMatrix); overload;
              constructor  Clone (m : TMatrix); overload;
              constructor  Diag (n : integer; c : TComplex);
              constructor  LoadFromStream (Stream : TStream);
              constructor  Augment (m1, m2 : TCMatrix); overload;
              constructor  Augment (m : TCMatrix; v : TCVector); overload;
              constructor  AugmentRows (m1, m2 : TCMatrix); overload;
              procedure    AugmentRows (m1 : TCMatrix); overload;

              destructor   destroy; override;

              function  getSizeOf : integer;
              procedure AddRow; overload;
              procedure AddRow (v : TCVector); overload;
              procedure AddRow (v : TCMatrix); overload;
              procedure AddFirstRow (ColSize : integer);
              function  copy  (m : TCMatrix) : TCMatrix; overload;
              function  copyPart (m : TCMatrix; lower, upper : integer) : TCMatrix;
              function  copy  (m : TMatrix) : TCMatrix; overload;
              function  Zero : TCMatrix;
              function  IsNotZero (r, c : integer) : Boolean;
              function  IsZero (r, c : integer) : boolean; overload;
              function  IsZero : Boolean; overload;
              function  IsOne (r, c : integer) : boolean;

              class     function Equal (m1, m2 : TCMatrix) : Boolean;
              procedure Scale  (k : Double);
              function  add    (m1, m2 : TCMatrix) : TCMatrix; overload;
              function  add    (m1 : TCMatrix; m2 : TMatrix) : TCMatrix; overload;
              function  add    (m : TCMatrix) : TCMatrix; overload;
              function  add    (k : Double; m : TCMatrix) : TCMatrix; overload;
              function  sub    (m1, m2 : TCMatrix) : TCMatrix; overload;
              function  sub    (m : TCMatrix) : TCMatrix; overload;
              function  sub    (m1 : TCMatrix; m2 : TMatrix) : TCMatrix; overload;
              function  sub    (m1 : TMatrix; m2 : TCMatrix) : TCMatrix; overload;
              function  sub    (k : Double; m : TCMatrix) : TCMatrix; overload;
              function  sub    (m : TCMatrix; k : Double) : TCMatrix; overload;
              function  mult   (m : TCMatrix; k : TCMatElement) : TCMatrix; overload;
              function  mult   (k : TCMatElement) : TCMatrix; overload;
              function  mult   (m1, m2 : TCMatrix) : TCMatrix; overload;
              function  mult   (m1 : TMatrix; m2 : TCMatrix) : TCMatrix; overload;
              function  mult   (m1 : TCMatrix; m2 : TMatrix) : TCMatrix; overload;
              function  mult   (v : TCVector; m : TCMatrix) : TCMatrix; overload;
              function  ApplyUnaryFunction (const f : TUnaryComplexFunc) : TCMatrix;
              function  ApplyBinaryFunction (f : TBinaryComplexFunc; x, y : TComplex) : TCMatrix;

              function  transpose : TCMatrix; overload;
              function  transpose (m : TCMatrix) : TCMatrix; overload;
              function  Prod : TComplex;
              function  Sum : TComplex;
              function  Mean : TComplex;
              function  StdDev : TComplex;
              function  SumofSquares : TComplex;
              function  Rank (echelon : TCMatrix; tol : double) : integer;
              function  ReOrder (var nRank : integer; Tol : Double; SortOrder : TReOrderSortOrder) : TCMatrix;
              function  Det : TComplex;
              function  abs (m : TCMatrix) : TCMatrix;

              procedure ReSize (ri, ci : integer);
              procedure setval (ri, ci : integer; v : TCMatElement);
              function  getval (ri, ci : integer) : TCMatElement;
              property  M[x, y : Integer] : TCMatElement read getVal write setVal; default;
              function  ExtractRow (rr : integer) : TCVector; overload;
              function  ExtractRowAsMatrix (rr : integer) : TCMatrix;
              procedure ExtractColumn (var v : TCVector; cc : integer);
              procedure ExtractRow (var v : TCVector; rr : integer); overload;
              function  InsertRow (v : TCVector; rr : integer) : TCMatrix;
              function  DeleteRow (rr : integer) : TCMatrix;
              function  ExchangeRows (r1, r2 : integer) : TCMatrix;
              function  ExchangeCols (c1, c2 : integer) : TCMatrix;

              function  SaveToString : string;
              procedure SaveAsCSV (fileName : string; delimiter : char);
              procedure Save (FileName : string);
              procedure Load (FileName : string);
              procedure SaveToStream (Stream : TStream);
     end;


     { Define a dynamic matrix type for holding doubles }
     TMatrix = class (TBaseMatrix)
                private
                   mx   : pTDRowList;  // pointer to a list of rows }
                   procedure SetSize (ri, ci : integer);
                   procedure FreeSpace;
                   function  ComputeRank (m : TMatrix) : integer;
                   function  getPtr : pTDRowList;
                public
                   constructor  Create; overload;
                   constructor  Create (r, c : integer); overload; virtual;
                   constructor  Create (n : integer); overload; virtual;
                   constructor  Create (c : integer; d : array of TDMatElement); overload; virtual;
                   constructor  Create (n : integer; d : double); overload; virtual;
                   constructor  Create (n, m : integer; d : double); overload; virtual;
                   constructor  Clone (m : TMatrix);
                   constructor  Diag (n : integer; d : double);
                   constructor  LoadFromStream (Stream : TStream);
                   constructor  Augment (m1, m2 : TMatrix); overload;
                   constructor  Augment (m : TMatrix; v : TDVector); overload;
                   constructor  AugmentRows (m1, m2 : TMatrix);
                   function     ExtractRow (rr : integer) : TDVector; overload;
                   destructor   destroy; override;
                   procedure    ReSize (ri, ci : integer);
                   procedure    Setval (ri, ci : integer; v : TDMatElement);
                   function     Getval (ri, ci : integer) : TDMatElement;
                   property     M[x, y : Integer] : TDMatElement read GetVal write SetVal; default;
                   property     ptr : pTDRowList read getPtr;

                   function  IsNotZero (r, c : integer) : Boolean;
                   function  IsZero (r, c : integer) : boolean; overload;
                   function  IsZero : Boolean; overload;
                   function  IsOne (r, c : integer) : boolean;
                   class     function Equal (m1, m2 : TMatrix) : Boolean; overload;
                   class     function Equal  (m1 : TCMatrix; m2 : TMatrix) : Boolean; overload;
                   function  getSizeOf : integer;
                   procedure AddRow;
                   procedure AddFirstRow (ColSize : integer);
                   function  Identity : TMatrix;
                   function  RandomMat : TMatrix; overload;
                   function  RandomMat (Lower, Upper : integer) : TMatrix; overload;
                   function  Diagonal (k : TDMatElement) : TMatrix; overload;
                   function  Diagonal (v : TDVector) : TMatrix; overload;
                   function  Diagonal (v : array of double) : TMatrix; overload;
                   function  Zero : TMatrix;
                   function  L (ci :integer; d : array of TDMatElement) : TMatrix;
                   function  transpose : TMatrix; overload;
                   function  transpose (m : TMatrix) : TMatrix; overload;
                   function  Norm : Double;
                   function  Prod : Double;
                   function  Sum : Double;
                   function  Mean : Extended;
                   function  SumofSquares : Extended;
                   function  add    (m1, m2 : TMatrix) : TMatrix; overload;
                   function  add    (m : TMatrix) : TMatrix; overload;
                   function  add    (k : Double; m : TMatrix) : TMatrix; overload;
                   function  sub    (m1, m2 : TMatrix) : TMatrix; overload;
                   function  sub    (m : TMatrix) : TMatrix; overload;
                   function  sub    (k : Double; m : TMatrix) : TMatrix; overload;
                   function  sub    (m : TMatrix; k : Double) : TMatrix; overload;
                   function  mult   (m : TMatrix; k : TDMatElement) : TMatrix; overload;
                   function  mult   (k : TDMatElement) : TMatrix; overload;
                   function  mult   (m1, m2 : TMatrix) : TMatrix; overload;
                   function  mult   (v : TDVector; m : TMatrix) : TMatrix; overload;
                   procedure Scale  (k : Double);
                   function  ApplyUnaryFunction (const f : TUnaryDoubleFunc) : TMatrix;
                   function  ApplyBinaryFunction (f : TBinaryDoubleFunc; x, y : double) : TMatrix;
                   function  copy   (m : TMatrix) : TMatrix;
                   //procedure CopyToClipBoard;
                   //function  CopyFromClipBoard : TMatrix;
                   function  SaveToString : string;
                   procedure Save (FileName : string);
                   procedure Load (FileName : string);
                   procedure SaveToStream (Stream : TStream);
                   procedure ExtractColumn (var v : TDVector; cc : integer);
                   procedure ExtractRow (var v : TDVector; rr : integer); overload;
                   procedure ExtractRow (var v : TCVector; rr : integer); overload;
                   function  InsertRow (v : TDVector; rr : integer) : TMatrix;
                   function  DeleteRow (rr : integer) : TMatrix;
                   function  ExchangeRows (r1, r2 : integer) : TMatrix;
                   function  ExchangeCols (c1, c2 : integer) : TMatrix;
                   function  Rank (echelon : TMatrix; tol : double) : integer;
                   procedure Invert (inv : TMatrix); overload;
                   procedure Invert; overload;
                   procedure SolveLinear (v, b : TDVector; SelfToInv : boolean);
                   procedure LUSolve (index : TIVector; b : TDVector);
                   procedure LUDecomp2 (m : TMatrix; piv : TIVector);
                   procedure LUDecomp3 (m : TMatrix; index : TIVector);
                   procedure LUDecomp (m : TMatrix; index : TIVector);
                   function  Det : double;
                   procedure NullSpace_2 (var NullVectors : TMatrix; var BasisSize : integer;
                                 var Echelon : TMatrix; var TheRank : integer; tol : double);
                   procedure NullSpace (var NullVectors : TMatrix; var BasisSize : Integer; Tol : double);
                   procedure GetNr(var Nr : TMatrix; tol : Double);
                   procedure GetL0 (var L0 : TMatrix; tol : Double);
                   procedure GetL (var L : TMatrix; tol : Double);
                   procedure GetK (var K : TMatrix; tol : Double);
                   function  GetConserve (Cn : TMatrix; tol : Double) : TMatrix;
                   function  GetConserve2(Cn : TMatrix; tol : Double) : TMatrix;
                   function  ReOrder (var nRank : integer; Tol : Double; SortOrder : TReOrderSortOrder) : TMatrix;

                   procedure svd (var u : TMatrix; var w : TDVector; var v : TMatrix);
                   procedure svd2 (var u : TMatrix; var w : TDVector; var v : TMatrix);
                   procedure svdSolve (var u : TMatrix; var w : TDVector; var v : TMatrix;
                                b : TDVector; var x : TDVector);
                   function  svdfit (x, y, yerr : TDVector; var fit : TDVector;
                             var u, v : TMatrix; var w : TDVector; funcs : BasisProc): TDMatElement;
                   procedure svdCovar (v : TMatrix; w : TDVector; alpha : TMatrix);
                   procedure EigenValues (var wr, wi : TDVector);

                   procedure eliminate_cms (S, Tk1 : TMatrix; var cr, N : integer);
                   procedure ElementaryModes (D : TIVector; var mf, mb, C, k : integer; Tk : TMatrix);
                   class procedure Tableau (N, R : integer; var mf, mb, C, k : integer; Tk, Tk1 : TMatrix);
                   class function  grecodiv_of_vector (N, R : integer; vec : TDVector) : integer;
                   class function  grecodiv(P, Rest: integer) : integer;
                end;


function doubleToComplex(m : TMatrix) : TCMatrix; overload;
function complexToDouble (m : TCMatrix) : TMatrix; overload;

{ ------------------------------------------------------------------------- }

implementation

uses uEigenvalues, uIMSLLib;

const MATERROR = 'Matrix Operation Error:';



function doubleToComplex(m : TMatrix) : TCMatrix;
var i, j : integer;
begin
  result := TCMatrix.Create (m.r, m.c);
  for i := 1 to m.r do
      for j := 1 to m.c do
          result.setVal(i, j, complex (m[i,j], 0.0));

  // Copy over column and row names, clear destination first then copy
  Result.FRowNames.clear; Result.FColumnNames.clear;
  if m.r > 0 then Result.FRowNames.Assign (m.FRowNames);
  if m.c > 0 then Result.FColumnNames.Assign (m.FColumnNames);
  Result.Name := m.Name;
end;


function complexToDouble (m : TCMatrix) : TMatrix;
var i, j : integer;
begin
  result := TMatrix.Create (m.r, m.c);
  for i := 1 to m.r do
      for j := 1 to m.c do
          result[i,j] := m[i,j].r;

  // Copy over column and row names, clear destination first then copy
  Result.FRowNames.clear; Result.FColumnNames.clear;
  if m.r > 0 then Result.FRowNames.Assign (m.FRowNames);
  if m.c > 0 then Result.FColumnNames.Assign (m.FColumnNames);
  Result.Name := m.Name;
end;


{ ------------------------------------------------------------------------- }
{                         START OF MATRIX IMPLEMETATION                     }
{ ------------------------------------------------------------------------- }


// Base Constructor - the mother of all matrix constructors
// Create an empty matrix
constructor TBaseMatrix.Create;
begin
  inherited Create;
  nr := 0; nc := 0;
  InternalTolerance := SMALLNUMBER;
end;


{ ******************************************************************** }
{ Returns true if matrices m1 and m2 have the same dimensions          }
{                                                                      }
{ Usage: if SameDimensions (A, B) then                                 }
{                                                                      }
{ ******************************************************************** }
function TBaseMatrix.SameDimensions (m1, m2 : TBaseMatrix) : boolean;
begin
  result := (m1.nr = m2.nr) and (m1.nc = m2.nc);  { use nr, nc for direct access }
end;


{ ******************************************************************** }
{ Returns true if matrix m is square                                   }
{                                                                      }
{ Usage: if IsSquare then                                              }
{                                                                      }
{ ******************************************************************** }
function TBaseMatrix.IsSquare : boolean;
begin
  result := Self.nr = Self.nc;
end;


{ ******************************************************************** }
{ ******************************************************************** }
procedure TBaseMatrix.CheckLimits (ri, ci : integer);
begin
 if ri > r then
    raise EMatrixSizeError.Create ('Error: Row index out of range (Max range=' + IntToStr (r) + '), value = ' + inttostr (ri) + ' in matrix ' + Name);

 if ci > c then
    raise EMatrixSizeError.Create ('Error: Column index out of range (Max range=' + IntToStr (c) + '), value = ' + inttostr (ci) + ' in matrix ' + Name);
end;


{ ******************************************************************** }
{ Returns true if ri, ci is in range for Self matrix                   }
{                                                                      }
{ Usage: if m.InRange (2, 3) then...                                   }
{ ******************************************************************** }
function TBaseMatrix.InRange (ri, ci : integer) : Boolean;
begin
  Result := ((ri > 0) and (ri <= r)) and
            ((ci > 0) and (ci <= c));
end;


procedure TBaseMatrix.CreateNameLists (ri, ci : integer);
var i : integer; str : string;
begin
  FRowNames := TStringList.Create; FColumnNames := TStringList.Create;
  // Build some dummy names, Zero Index entries are dummy entries
  for i := 0 to ri do
      begin
      str := 'R' + inttostr (i);
      FRowNames.add (str);
      end;
  for i := 0 to ci do FColumnNames.add ('C' + inttostr (i));
end;


procedure TBaseMatrix.DestroyNameLists;
begin
  FRowNames.free;    FRowNames := Nil;
  FColumnNames.free; FColumnNames := Nil;
end;


// ---------------

// Helper routine, use m.rName[i] := 'Name'
procedure TBaseMatrix.setRowName (ri : integer; name : string);
begin
  FRowNames[ri] := name;
end;


function TBaseMatrix.getRowName (ri : integer) : string;
begin
  result := FRowNames[ri];
end;

// Helper routine, use m.cName[i] := 'Name'
procedure TBaseMatrix.setColumnName (ci : integer; name : string);
begin
  FColumnNames[ci] := name;
end;


function TBaseMatrix.getColumnName (ci : integer) : string;
begin
  result := FColumnNames[ci];
end;


// ------------------------------------------------------------------- */
//               Copy row names to column names                        */
// ------------------------------------------------------------------- */
class procedure TBaseMatrix.copyRowToColumnNames (s, d : TBaseMatrix);
var i : integer;
begin
  // s = source; d = destination
  for i := 1 to s.r do
      d.SetColumnName (i, s.GetRowName (i));
end;


// ------------------------------------------------------------------- */
//               Copy column names to row names                        */
// ------------------------------------------------------------------- */
class procedure TBaseMatrix.copyColumnToRowNames (s, d : TBaseMatrix);
var i : integer;
begin
  // s = source; d = destination
  for i := 1 to s.c  do
      d.SetRowName (i, s.GetColumnName (i));
end;




// ------------------------------------------------------------------------
//  Complex Matrix Type
// ------------------------------------------------------------------------


{ ******************************************************************** }
{ Create a matrix of specific size, all bytes are zeroed               }
{                                                                      }
{ Usage:  A := TMatrix.create (3, 2);                                  }
{ ******************************************************************** }
constructor TCMatrix.Create (r, c : integer);
begin
  Create;
  Self.SetSize (r, c);
  Self.Name := 'untitled';
end;


{ ******************************************************************** }
{ Create an identity matrix                                            }
{                                                                      }
{ Usage:   A := TMatrix.create (3);                                    }
{ ******************************************************************** }
constructor TCMatrix.Create (n : integer);
var i : integer;
begin
  Create;
  Self.SetSize (n, n);
  for i := 1 to n do Self[i,i] := complex (1.0, 0);
end;


{ ******************************************************************** }
{ Create a square matrix of size n filled with value d                 }
{                                                                      }
{ Usage:   A := TCMatrix.create (3, complex (3.1415, 2));              }
{ ******************************************************************** }
constructor TCMatrix.Create (n : integer; c : TComplex);
var i, j : integer;
begin
  Create;
  Self.SetSize (n, n);
  for i := 1 to n do
      for j := 1 to n do
          Self[i,j] := c;
end;


{ ******************************************************************** }
{ Create a matrix of size n by m filled with value d                   }
{                                                                      }
{ Usage:   A := TCMatrix.create (3, 4, complex (3.1415, 2));           }
{ ******************************************************************** }
constructor TCMatrix.Create (n, m : integer; c : TComplex);
var i, j : integer;
begin
  Create;
  Self.SetSize (n, m);
  for i := 1 to n do
      for j := 1 to m do
          Self[i,j] := c;
end;


{ ******************************************************************** }
{ Create a matrix filled with values from array d given that the       }
{ number of columns equals c.                                          }
{                                                                      }
{ Usage:  A := TMatrix.create (2, [1, 2, 3, 4]);                       }
{         Creates a 2 by 2 matrix                                      }
{ ******************************************************************** }
constructor  TCMatrix.Create (c : integer; d : array of TCMatElement);
var i, j, ri, count : integer;
begin
  Create;
  ri := (High(d)+1) div c;
  Self.SetSize (ri, c);
  count := 0;
  for i := 1 to ri do
      for j := 1 to c do
          begin
          Self[i,j] := d[count];
          inc (count);
          end;
end;


{ ******************************************************************** }
{ Create identical copy of Self                                        }
{                                                                      }
{ Usage: n := TCMatrix.Clone (m)                                       }
{ ******************************************************************** }
constructor TCMatrix.Clone (m : TCMatrix);
begin
  Create;
  Self.SetSize (m.nr, m.nc);
  Self.Copy (m);
end;



{ ******************************************************************** }
{ Create identical copy of a TMatrix Type                              }
{                                                                      }
{ Usage: n := TCMatrix.Clone (m)                                       }
{ ******************************************************************** }
constructor TCMatrix.Clone (m : TMatrix);
begin
  Create;
  Self.SetSize (m.nr, m.nc);
  Self.Copy (m);
end;


{ ******************************************************************** }
{ Create a diagonal matrix with compelx value d                        }
{                                                                      }
{ Usage: n := TMatrix.Diag (m, d)                                      }
{ ******************************************************************** }
constructor TCMatrix.Diag (n : integer; c : TComplex);
var i : integer;
begin
  Create;
  Self.SetSize (n, n);
  Self.Zero;
  for i := 1 to n do Self[i,i] := c;
end;


{ ******************************************************************** }
{ Augment m1 with m2 and return result. m1.r must equal m2.r !         }
{                                                                      }
{ Usage: m := TMatrix.Augment (m1, m2);                                }
{                                                                      }
{ ******************************************************************** }
constructor TCMatrix.Augment (m1, m2 : TCMatrix);
var i, j : Integer;
begin
  Create;
  if m1.r <> m2.r then
     raise EMatrixError.Create ('Matrix arguments must have the same row size in the aug function');
  Self.SetSize (m1.r, m1.c + m2.c);
  for i := 1 to m1.r do
      for j := 1 to m1.c do
          Self[i,j] := m1[i,j];
  for i := 1 to m1.r do
      for j := 1 to m2.c do
          Self[i,m1.c + j] := m2[i,j];
  for i := 1 to m1.r do
      Self.rName[i] := m1.rName [i];
  for i := 1 to m1.c do
      Self.cName[i] := m1.cName [i];
  for i := 1 to m2.c do
      Self.cName[m1.c + i] := m2.cName [i];
end;


{ ******************************************************************** }
{ Augment m1 with m2 and return result. m1.r must equal m2.r !         }
{                                                                      }
{ Usage: m := TMatrix.Augment (m1, m2);                                }
{                                                                      }
{ ******************************************************************** }
constructor TCMatrix.Augment (m : TCMatrix; v : TCVector);
var i, j : Integer;
begin
  Create;
  if m.r <> v.size then
     raise EMatrixError.Create ('Vector and matrix row size incompatible in aug function');
  Self.SetSize (m.r, m.c + 1);
  for i := 1 to m.r do
      for j := 1 to m.c do
          Self[i,j] := m[i,j];
  for j := 1 to v.size do
      Self[j, m.c + 1] := v[j];
  for i := 1 to m.r do
      Self.rName[i] := m.rName [i];
  for i := 1 to m.c do
      Self.cName[i] := m.cName [i];
  Self.cName[m.c + 1] := 'C' + IntToStr (m.c+1);
end;


{ ******************************************************************** }
{ Augment m1 with m2 and return result. m1.c must equal m2.c !         }
{ Adds rows of m2 to rows of m1                                        }
{                                                                      }
{ Usage: m := TMatrix.Augment (m1, m2);                                }
{                                                                      }
{ ******************************************************************** }
constructor TCMatrix.AugmentRows (m1, m2 : TCMatrix);
var i, j : Integer;
begin
  Create;
  if m1.c <> m2.c then
     raise EMatrixError.Create ('Matrix arguments must have the same column size in the row augment function (augr)');

  Self.SetSize (m1.r + m2.r, m1.c);
  for i := 1 to m1.r do
      for j := 1 to m1.c do
          Self[i,j] := m1[i,j];

  for i := 1 to m2.r do
      for j := 1 to m2.c do
          Self[m1.r + i, j] := m2[i,j];

  for i := 1 to m1.r do
      Self.rName[i] := m1.rName [i];
  for i := 1 to m2.r do
      Self.rName[m1.r + i] := m2.rName [i];
  for i := 1 to m1.c do
      Self.cName[i] := m1.cName [i];
end;


{ ******************************************************************** }
{ Augment m1 with self and return result. m1.c must equal self.c !     }
{ Adds rows of m2 to rows of m1                                        }
{                                                                      }
{ Usage: m.Augment (m1);                                               }
{                                                                      }
{ ******************************************************************** }
procedure TCMatrix.AugmentRows (m1 : TCMatrix);
var i, j : Integer;
    tmp : TCMatrix;
begin
  if m1.c <> self.c then
     raise EMatrixError.Create ('Matrix arguments must have the same column size in the row augment function (augr)');

  tmp := TCMatrix.Create (m1.r + self.r, m1.c);
  try
    tmp.zero;

    // Copy a whole row at a time using move: Source, Dest, Count }
    for i := 1 to r do move (self.mx^[i]^, tmp.mx^[i]^, sizeof(TCMatElement) * (c+1));
    for i := 1 to r do tmp.rName[i] := self.rName[i];
    for i := 1 to c do tmp.cname[i] := self.cName[i];
    tmp.rName[r+1] := 'R' + IntToStr (r+1);
    self.FreeSpace; self.SetSize (tmp.nr, tmp.nc);
    self.Copy (tmp);
  finally
    tmp.free;
  end;


  for j := 1 to m1.c do
      Self[self.r, j] := m1[1, j];

  //for i := 1 to m1.r do
  //    for j := 1 to m1.c do
  //        Self[m1.r + i, j] := m1[i,j];

  //for i := 1 to m1.r do
  //    Self.rName[i] := m1.rName [i];
  //for i := 1 to self.r do
  //    Self.rName[m1.r + i] := m1.rName [i];
end;


procedure TCMatrix.ReSize (ri, ci : integer);
var oldnr : integer; i : integer;
begin
  oldnr := nr;
  nr := ri; nc := ci;
  reallocMem (mx, sizeof (pTCRowList) * (nr+1));  { r+1 so that we can index from 1 }
  for i := oldnr to nr do  mx^[i] := AllocMem (sizeof (TCMatElement) * (nc+1));
  CreateNameLists (ri, ci);
end;


{ ******************************************************************** }
{ Usage:    A.destroy, use A.free in a program                         }
{ ******************************************************************** }
destructor TCMatrix.destroy;
begin
  FreeSpace;
  Inherited Destroy;
end;


{ Free the data space but not the object }
procedure TCMatrix.FreeSpace;
var i : integer;
begin
  if mx <> Nil then
     begin
     for i := 1 to nr do
         if mx^[i] <> Nil then
            begin FreeMem (mx^[i], sizeof (TCMatElement) * (nc+1)); mx^[i] := Nil; end;
     FreeMem (mx, sizeof (PTCRowList) * (nr+1)); mx := Nil;
     end;
  DestroyNameLists;
end;


// Internal routine used to set the size of the matrix and allocate space
// DESTROYS ANY DATA !
procedure TCMatrix.setSize (ri, ci : integer);
var i : integer;
begin
  FreeSpace;
  nr := ri; nc := ci;
  mx := AllocMem (sizeof (pTCRowList) * (nr+1));  { r+1 so that we can index from 1 }
  for i := 1 to nr do  mx^[i] := AllocMem (sizeof (TCMatElement) * (nc+1));
  CreateNameLists (ri, ci);
end;


{ ******************************************************************** }
{ Returns the number of bytes occupied by matix                        }
{                                                                      }
{ Usage:  A.SizeOf                                                     }
{                                                                      }
{ ******************************************************************** }
function TCMatrix.GetSizeOf : integer;
var i : integer;
begin
  Result := nr*nc*SizeOf (TCMatElement) + Length (FName) + SizeOf (InternalTolerance);
  for i := 0 to FRowNames.Count - 1 do
      Result := Result + Length (FRowNames[i]);
  for i := 0 to FColumnNames.Count - 1 do
      Result := Result + Length (FColumnNames[i]);
end;


{ ******************************************************************** }
{ Normal Usage:  A[2, 3] := 1.2;                                       }
{ ******************************************************************** }
procedure TCMatrix.Setval (ri, ci : integer; v : TCMatElement);
begin
{$ifdef DEBUG}
  CheckLimits (ri, ci);
{$endif}
  mx^[ri]^[ci] := v;
end;


{ ******************************************************************** }
{ Normal Usage:  d := A[2, 3];                                         }
{ ******************************************************************** }
function TCMatrix.Getval (ri, ci : integer) : TCMatElement;
begin
{$ifdef DEBUG}
  CheckLimits (ri, ci);
{$endif}
  result := mx^[ri]^[ci];
end;


{ ******************************************************************** }
{ Add an empty row to the bottom of matrix without destroying data     }
{ in other rows                                                        }
{                                                                      }
{ Usage:  m.AddRow                                                     }
{ ******************************************************************** }
procedure TCMatrix.AddRow;
var tmp : TCMatrix; i : integer;
begin
  tmp := TCMatrix.Create (r+1, c);
  tmp.zero;
  try
    // Copy a whole row at a time using move: Source, Dest, Count }
    for i := 1 to r do move (Self.mx^[i]^, tmp.mx^[i]^, sizeof(TCMatElement) * (c+1));
    for i := 1 to r do tmp.rName[i] := Self.rName[i];
    for i := 1 to c do tmp.cname[i] := Self.cName[i];
    tmp.rName[r+1] := 'R' + IntToStr (r+1);
    //tmp.FRowNames.Assign (Self.FRowNames); tmp.FColumnNames.Assign (Self.FColumnNames);
    Self.FreeSpace; Self.SetSize (tmp.nr, tmp.nc);
    Self.Copy (tmp);
  finally
    tmp.free;
  end;
end;


{ ******************************************************************** }
{ Add a  row to the bottom of matrix without destroying data           }
{ in other rows                                                        }
{                                                                      }
{ Usage:  m.AddRow (myVector)                                          }
{ ******************************************************************** }
procedure TCMatrix.AddRow (v : TCVector);
var i : integer;
begin
  AddRow;
  for i := 1 to Self.c do
      Self.setVal (Self.r, i, v[i]);
end;

{ ******************************************************************** }
{ Add a  row to the bottom of matrix without destroying data           }
{ in other rows                                                        }
{                                                                      }
{ Usage:  m.AddRow (myMatrix)                                         }
{ ******************************************************************** }
procedure TCMatrix.AddRow (v : TCMatrix);
var i : integer;
begin
  AddRow;
  for i := 1 to Self.c do
      Self.setVal (Self.r, i, v[1,i]);
end;


{ ******************************************************************** }
{ Destroy Sef and recreate it with a single row of ColSize columns     }
{                                                                      }
{ Usage: m.AddFirstRow (4)                                             }
{ ******************************************************************** }
procedure TCMatrix.AddFirstRow (ColSize : integer);
begin
  if mx <> nil then Self.FreeSpace;
  Self.setSize (1, ColSize);
end;



{ ******************************************************************** }
{ Copy matrix 'm' to Self, Self must exist and is overwritten          }
{ in the process. This procedure does a fast deep copy of the matrix.  }
{                                                                      }
{ Usage:  B.Copy (A);   performs the operation:  B = A  with deep copy }
{                                                                      }
{ ******************************************************************** }
function TCMatrix.Copy (m : TCMatrix) : TCMatrix;
var i : integer;
begin
  if (r <> m.r) or (c <> m.c) then
     begin
     if r <> m.r then
        raise EMatrixSizeError.Create (MATERROR + #13#10'Cannot copy matrices with different sized rows: dest<'
            + inttostr (r) + '> src<' + inttostr (m.r) + '>')
     else
        raise EMatrixSizeError.Create (MATERROR + #13#10'Cannot copy matrices with different sized columns: dest<'
            + inttostr (c) + '> src<' + inttostr (m.c) + '>');
     end;
  { Copy a whole row at a time using move }
  for i := 1 to r do move (m.mx^[i]^, Self.mx^[i]^, sizeof(TCMatElement) * (c+1));
  // Copy over column and row names, clear destination first then copy
  Self.FRowNames.clear;  Self.FColumnNames.clear;
  if m.r > 0 then Self.FRowNames.Assign (m.FRowNames);
  if m.c > 0 then Self.FColumnNames.Assign (m.FColumnNames);
  Self.Name := m.Name;
  result := Self;
end;


{ ******************************************************************** }
{ Copy part of matrix 'm' to Self, Self must exist and is overwritten  }
{ in the process. This procedure does a fast deep copy of the matrix.  }
{                                                                      }
{ Usage:  B.CopyPart (A, lower, upper);                                }
{ performs the operation:  B = A  with deep copy                       }
{                                                                      }
{ ******************************************************************** }
function TCMatrix.CopyPart (m : TCMatrix; lower, upper : integer) : TCMatrix;
var i, j : integer;
begin
  if (c <> m.c) then
     raise EMatrixSizeError.Create (MATERROR + #13#10'Cannot copy matrices with different sized columns: dest<'
            + inttostr (c) + '> src<' + inttostr (m.c) + '>');
  Self.SetSize ((upper-lower)+1, m.c);
  { Copy a whole row at a time using move }
  for i := lower+1 to upper+1 do
      for j := 1 to c do
          Self[i-lower,j] := m[i-1,j];
          
  //for i := lower+1 to upper+1 do
  //    move (m.mx^[i]^, Self.mx^[i-lower+1]^, sizeof(TCMatElement) * (c+1));
  // Copy over column and row names, clear destination first then copy
  Self.FRowNames.clear;  Self.FColumnNames.clear;
  for i := 0 to self.r do Self.FRowNames.add ('R' + inttostr (i));
  if m.c > 0 then Self.FColumnNames.Assign (m.FColumnNames);
  Self.Name := m.Name;
  result := Self;
end;


{ ******************************************************************** }
{ Copy matrix 'm' to Self, Self must exist and is overwritten          }
{ in the process. This procedure does a fast deep copy of the matrix.  }
{                                                                      }
{ Usage:  B.Copy (A);   performs the operation:  B = A  with deep copy }
{                                                                      }
{ ******************************************************************** }
function TCMatrix.Copy (m : TMatrix) : TCMatrix;
var i : integer;
begin
  if (r <> m.r) or (c <> m.c) then
     begin
     if r <> m.r then
        raise EMatrixSizeError.Create (MATERROR + #13#10'Cannot copy matrices with different sized rows: dest<'
            + inttostr (r) + '> src<' + inttostr (m.r) + '>')
     else
        raise EMatrixSizeError.Create (MATERROR + #13#10'Cannot copy matrices with different sized columns: dest<'
            + inttostr (c) + '> src<' + inttostr (m.c) + '>');
     end;
  { Copy a whole row at a time using move }
  for i := 1 to r do move (m.mx^[i]^, Self.mx^[i]^, sizeof(TCMatElement) * (c+1));
  // Copy over column and row names, clear destination first then copy
  Self.FRowNames.clear;  Self.FColumnNames.clear;
  if m.r > 0 then Self.FRowNames.Assign (m.FRowNames);
  if m.c > 0 then Self.FColumnNames.Assign (m.FColumnNames);
  Self.Name := m.Name;
  result := Self;
end;


{ ******************************************************************** }
{ Zero the Self matrix                                                 }
{                                                                      }
{ Usage: A.Zero;                                                       }
{                                                                      }
{ ******************************************************************** }
function TCMatrix.Zero : TCMatrix;
var i, j : integer;
begin
  for i := 1 to r do
      for j := 1 to c do
          Self[i,j] := complex (0.0, 0.0);
  result := Self;
end;


{ ******************************************************************** }
{ Returns true if element r, c is not zero                             }
{                                                                      }
{ Usage: if m.IsNotZero (1, 2) then                                    }
{ ******************************************************************** }
function TCMatrix.IsNotZero (r, c : integer) : Boolean;
begin
  result := (System.abs (m[r, c].r) > InternalTolerance) or (System.abs (m[r, c].i) > InternalTolerance);
end;


{ ******************************************************************** }
{ Returns true if element r, c is zero                                 }
{                                                                      }
{ Usage: if m.IsZero (1, 2) then                                       }
{ ******************************************************************** }
function TCMatrix.IsZero (r, c : integer) : boolean;
begin
  result := (System.abs (m[r, c].r) < InternalTolerance) and
            (System.abs (m[r, c].i) < InternalTolerance);
end;


{ ******************************************************************** }
{ Returns true if the matrix is zero                                   }
{                                                                      }
{ Usage: if m.IsZero then                                              }
{ ******************************************************************** }
function TCMatrix.IsZero : Boolean;
var i, j : Integer;
begin
  Result := True;
  for i := 1 to nr do
      for j := 1 to nc do
          if not IsZero (i, j) then
             begin
             Result := False;
             Exit;
             end;
end;

{ ******************************************************************** }
{ Returns true if element r, c is Unity (+1 or -1)                     }
{                                                                      }
{ Usage: if m.IsOne (1, 2) then                                        }
{                                                                      }
{ ******************************************************************** }
function TCMatrix.IsOne (r, c : integer) : boolean;
begin
   result := (System.abs (m[r, c].r) < 1+SMALLNUMBER) and (System.abs (m[r, c].r) > 1-SMALLNUMBER)
             and (System.abs (m[r,c].i) = 0.0);
end;


{ ******************************************************************** }
{ Equality test                                                        }
{                                                                      }
{ Usage:  TMatrix.Equal (A, B);   returns true if  A = B               }
{                                                                      }
{ ******************************************************************** }
class function TCMatrix.Equal  (m1, m2 : TCMatrix) : Boolean;
var i, j : integer;
begin
  if (m1.r <> m2.r) or (m1.c <> m2.c) then
     Result := False
  else
     begin
     for i := 1 to m1.r do
         for j := 1 to m1.c do
             if not complexEquals (m1[i,j], m2[i,j]) then
                begin
                Result := False;
                Exit;
                end;
     Result := True;
     end;
end;


{ ******************************************************************** }
{ Multiply matrix Self by factor k to give result in Self              }
{                                                                      }
{ Usage:  A.Scale (k); multiply A by k giving A                        }
{                                                                      }
{ ******************************************************************** }
procedure TCMatrix.Scale  (k : Double);
var i, j : integer;
begin
  for i := 1 to Self.r do
      for j := 1 to Self.c do
          setVal (i, j, complex (Self[i,j].r*k, Self[i,j].i*k));
end;


{ ******************************************************************** }
{ Extract row rr from the Self matrix and return it as a TDVector       }
{                                                                      }
{ Usage: v = m.ExtractRow (1) extract row 1 from m and place in v      }
{                                                                      }
{ ******************************************************************** }
function TCMatrix.ExtractRow (rr : integer) : TCVector;
var i : integer;
begin
  if Self.InRange (rr, 1) then
     begin
     Result := TCVector.Create (Self.c);
     for i := 1 to Self.c do result[i] := Self[rr, i];
     end
  else
     raise EMatrixError.Create ('Row extract index out of range');
end;


{ ******************************************************************** }
{ Extract row rr from the Self matrix and return it as a TCMatrix      }
{                                                                      }
{ Usage: v = m.ExtractRow (1) extract row 1 from m and place in v      }
{                                                                      }
{ ******************************************************************** }
function TCMatrix.ExtractRowAsMatrix (rr : integer) : TCMatrix;
var i : integer;
begin
  if Self.InRange (rr, 1) then
     begin
     Result := TCMatrix.Create (1, Self.c);
     for i := 1 to Self.c do result[1,i] := Self[rr, i];
     end
  else
     raise EMatrixError.Create ('Row extract index out of range');
end;



{ ******************************************************************** }
{ Extract column cc from the Self matrix and return it as a TDVector    }
{                                                                      }
{ Usage: m.ExtractColumn (v, 1)  extract column 1 from m and place in v}
{                                                                      }
{ ******************************************************************** }
procedure TCMatrix.ExtractColumn (var v : TCVector; cc : integer);
var i : integer;
begin
  v.freeSpace; v.SetSize (Self.r); { Create result vector of appropriate size }
  for i := 1 to Self.r do v[i] := Self[i, cc];
end;


{ ******************************************************************** }
{ Extract rwo rr from the Self matrix and return it as a TDVector       }
{                                                                      }
{ Usage: m.ExtractRow (v, 1)  extract row 1 from m and place in v      }
{                                                                      }
{ ******************************************************************** }
procedure TCMatrix.ExtractRow (var v : TCVector; rr : integer);
var i : integer;
begin
  v.freespace; v.SetSize (Self.c);
  for i := 1 to Self.c do v[i] := Self[rr, i];
end;


function TCMatrix.InsertRow (v : TCVector; rr : integer) : TCMatrix;
var tmp : TCMatrix; i, j : integer;
begin
  tmp := TCMatrix.Create (Self.r + 1, Self.c);
  if rr = 1 then
     begin
     for i := 1 to Self.c do
         tmp[1,i] := v[i];
     for i := 2 to tmp.r do
         for j := 1 to Self.c do
             begin
             tmp.rName[i] := Self.rName[i-1];
             tmp[i,j] := Self[i-1,j];
             end;
     end
  else
     begin
     for i := 1 to rr - 1 do
         for j := 1 to Self.c do
             tmp[i,j] := Self[i,j];
     for i := 1 to Self.c do
         tmp[rr, i] := v[i];

     for i := rr to Self.r do
         for j := 1 to Self.c do
             tmp[i+1,j] := Self[i,j];
     end;
  Self.setSize (tmp.r, tmp.c);
  Self.Copy (tmp);
  tmp.Free;
  Result := Self;
end;


function TCMatrix.DeleteRow (rr : integer) : TCMatrix;
var tmp : TCMatrix; i, j : integer;
begin
  if Self.r = 1 then
     begin
     Self.setSize (0, 0);
     Result := Self;
     Exit;
     end;

  tmp := TCMatrix.Create (Self.r - 1, Self.c);
  if rr = 1 then
     begin
     for i := 2 to Self.r do
         for j := 1 to Self.c do
             tmp[i-1,j] := Self[i,j];
     end
  else
     begin
     for i := 1 to rr - 1 do
         for j := 1 to Self.c do
             tmp[i,j] := Self[i,j];
     if rr < Self.r then
        for i := rr+1 to Self.r do
            for j := 1 to Self.c do
                tmp[i-1,j] := Self[i,j];
     end;
  Self.setSize (tmp.r, tmp.c);
  Self.Copy (tmp);
  tmp.Free;
  Result := Self;
end;


{ Internal routine that sets any values less than tol to 0.0 }
procedure CleanUpCMatrix (m : TCMatrix);
var i, j, ri, ci : integer;
begin
  { Removes all numbers close to zero, i.e between -tol and +tol }
  ri := m.r; ci := m.c;
  for i := 1 to ri do
      for j := 1 to ci do
          if m.IsZero (i, j) then m [i, j] := complex (0.0, 0.0);
end;


{ Internal routine to work out the rank of a matrix given the reduced row-echelon }
function TCMatrix.ComputeRank (m : TCMatrix) : integer;
var i, j, ri, ci, rank : integer;
begin
  ri := m.r; ci := m.c;
  { find the rank - brute force algorithm }
  rank := 0;
  { search row by row  for zero rows }
  for i := 1 to ri do
      begin
      { search along the row looking for nonzero entry }
      for j := 1 to ci do
          if m.IsNotZero (i, j) then
             begin
             inc (rank);
             break;
             end;

      end;
 result := rank;
end;


{ ******************************************************************** }
{ Routine to exchange two rows, r1 and r2 in matrix Self               }
{                                                                      }
{ Usage:  A.exchangeRows (1, 2);                                       }
{                                                                      }
{ ******************************************************************** }
function TCMatrix.ExchangeRows (r1, r2 : integer) : TCMatrix;
var ci, i : integer; t : TComplex; name : string;
begin
  if (r1 > 0) and (r1 <= Self.r) and (r2 > 0) and (r2 <= Self.r) then
     begin
     ci := Self.c;
     for i := 1 to ci do
         begin
         t := Self[r1, i];
         Self[r1, i] := Self[r2, i];
         Self[r2, i] := t;
         end;
     name := Self.rName[r1];
     Self.rName[r1] := Self.rName[r2];
     Self.rName[r2] := name;
     result := Self;
     end
  else
     raise EMatrixSizeError.Create ('Rows not in range for exchange');
end;



{ ******************************************************************** }
{ Routine to exchange two columns, c1 and c2 in matrix Self            }
{                                                                      }
{ Usage:  A.exchangeCols (1, 2);                                       }
{                                                                      }
{ ******************************************************************** }
function TCMatrix.ExchangeCols (c1, c2 : integer) : TCMatrix;
var ri, i : integer; t : TComplex; name : string;
begin
  if (c1 > 0) and (c1 <= Self.c) and (c2 > 0) and (c2 <= Self.c) then
     begin
     ri := Self.r;
     for i := 1 to ri do
         begin
         t := Self[i, c1];
         Self[i, c1] := Self[i, c2];
         Self[i, c2] := t;
         end;
     name := Self.cName[c1];
     Self.cName[c1] := Self.cName[c2];
     Self.cName[c2] := name;
     result := Self;
     end
  else
     raise EMatrixSizeError.Create ('Columns not in range for exchange [c1 = '
             + inttostr (c1) + '; c2 = ' + inttostr (c2) + ']');
end;



// work out if the cth column of m is made up of zeros except for a one at
// the rth row position
function Cei_found (m : TCMatrix; c, r : integer) : boolean;
var i : integer;
begin
  result := TRUE;
  // scan down the rows of the cth column
  for i := 1 to m.r do
      begin
      if i = r then  // If we're at the chosen row then check if we've got a ONE
         begin
         // I expect a value of ONE here (+1 or -1)
         if not m.IsOne (i, c) then
            begin result := FALSE; exit; end;
         end
      else // if not then I expect a value of ZERO here */
         if not m.IsZero (i, c) then
            begin result := FALSE; exit; end;
    end;
end;


// Search for a column, in the matrix m, containing zeros throughout
// except for the cth row which must be a ONE
function Cfind_ei (m : TCMatrix; c : integer) : integer;
var i : integer;
begin
   Result := 0;
   // start by scanning all columns
   for i := 1 to m.c do
       begin
       // Now scan the particular column we're on
       if Cei_found (m, i, c) then
          begin
          result := i;
          exit;
          end;
       end;
end;

// ------------------------------------------------------------------- */
//     Reorder matrix rows into independent rows first follwed by      */
//                            dependent rows                           */
//     Returns p, the permutation vector, if the jth value of p        */
//     has the value i, the p A, will swap rows i and j                */
// ------------------------------------------------------------------- */
function TCMatrix.ReOrder (var nRank : integer; Tol : Double; SortOrder : TReOrderSortOrder) : TCMatrix;
var tm, echelon : TCMatrix;
    c, i : integer;
begin
  InternalTolerance := Tol;
  if (Self.r = 0) or (Self.c = 0) then
     begin
     nRank := 0;
     Result := Self;
     Exit;
     end;

  if tol = 0 then
     tol := 1.0e-8;

   //if p = nil then
   //   p := TDVectori.Create (Self.r)
   //else p.SetSize (Self.r);
   //for i := 1 to p.Size do p[i] := 1;

   // remember to work on the transpose of the stoichiometry matrix
   tm := TCMatrix.Create (Self.c, Self.r);
   tm.transpose (Self);

   echelon := TCMatrix.Create (tm.r, tm.c);
   nRank := tm.Rank (echelon, tol);

   // look for the unit vectors (r of them) in echelon to make up an
   // identity matrix at the front of the echelon
   for i := 1 to nRank do
       begin
       if (not Cei_found (echelon, i, i)) then
          begin
          c := Cfind_ei(echelon, i);
          echelon.ExchangeCols (i, c);
          // swap rows in the stoichiometry matrix to reorder
          Self.ExchangeRows (i, c);
          if @SortOrder <> nil then
             SortOrder (i, c);
          end;
       end;

   echelon.Free; tm.Free;
   Result := Self;
end;


{ ******************************************************************** }
{ Find determinant of matrix                                           }
{                                                                      }
{ Usage: d := m.det                                                    }
{                                                                      }
{ ******************************************************************** }
function TCMatrix.Det : TComplex;
var m : TCMatrix; index : TIVector; i : integer; rcond : double;
begin
  result := complex (1, 0.0);
  if r = c then
     begin
     index := TIVector.Create (r);
     m := TCMatrix.Create (r,r);
     try
       m.copy (Self);
       computeLUDecomp (Self, m, index, rcond);
       for i := 1 to r do result := complexMult (result, m[i,i]);
     finally
       m.free; index.free;
     end;
     end
  else
     raise ENonSquareMatrix.Create ('Determinant: Matrix must be square');
end;


{ ******************************************************************** }
{ Find absolute of matrix                                              }
{                                                                      }
{ Usage: d := m.abs                                                    }
{                                                                      }
{ ******************************************************************** }
function TCMatrix.abs (m : TCMatrix) : TCMatrix;
var i, j : integer;
begin
  for i := 1 to m.r do
      for j := 1 to m.c do
          Self.setVal (i, j, complex (complexAbs (m[i,j]), 0.0));
  result := Self;
end;


{ ******************************************************************** }
{ Add matrix 'm' to scalar k, giving a new Self                        }
{                                                                      }
{ Usage:  A.add (k, B);   add scalar k to all elements in B giving A   }
{                                                                      }
{ ******************************************************************** }
function TCMatrix.Add (k : Double; m : TCMatrix) : TCMatrix;
var i, j : Integer;
begin
  for i := 1 to r do
      for j := 1 to c do
          Self.setVal (i, j, complexAdd (m[i,j], complex (k, 0)));
  result := Self;
end;


{ ******************************************************************** }
{ Add matrix 'm' to Self, giving a new Self                            }
{                                                                      }
{ Usage:  A.add (B);   add B to A, giving A                            }
{                                                                      }
{ ******************************************************************** }
function TCMatrix.add (m : TCMatrix) : TCMatrix;
var i, j : integer;
begin
  if Not SameDimensions (m, Self) then
     raise EMatrixSizeError.Create ('Incorrectly sized result matrix for matrix addition');

  for i := 1 to r do
      for j := 1 to c do
          Self.setVal (i, j, complexAdd (Self[i,j], m[i,j]));
  result := Self;
end;


{ ******************************************************************** }
{ Add matrix 'm1' and 'm2' and assign to Self                          }
{                                                                      }
{ Usage: A.add (A1, A2);  add A1 to A2 giving A                        }
{                                                                      }
{ ******************************************************************** }
function TCMatrix.add (m1, m2 : TCMatrix) : TCMatrix;
var i, j : integer;
begin
  if Not SameDimensions (m1, m2) then
     raise EMatrixSizeError.Create ('Incompatible matrix operands to add');

  if Not SameDimensions (m1, Self) then
     raise EMatrixSizeError.Create ('Incorrectly sized result matrix for matrix addition');

  for i := 1 to r do
      for j := 1 to c do
          Self.setVal (i, j, complexAdd (m1[i,j], m2[i,j]));
  result := Self;
end;



{ ******************************************************************** }
{ Add matrix 'm1' and 'm2' and assign to Self                          }
{                                                                      }
{ Usage: A.add (A1, A2);  add A1 to A2 giving A                        }
{                                                                      }
{ ******************************************************************** }
function TCMatrix.add (m1 : TCMatrix; m2 : TMatrix) : TCMatrix;
var i, j : integer;
begin
  if Not SameDimensions (m1, m2) then
     raise EMatrixSizeError.Create ('Incompatible matrix operands to add');

  if Not SameDimensions (m1, Self) then
     raise EMatrixSizeError.Create ('Incorrectly sized result matrix for matrix addition');

  for i := 1 to r do
      for j := 1 to c do
          Self.setVal (i, j, complexAdd (m1[i,j], complex (m2[i,j], 0.0)));
  result := Self;
end;




{ **********************************************************************  }
{ Subtract matrix 'm' to scalar k, giving a new Self                      }
{                                                                         }
{ Usage:  A.sub (k, B); subtract scalar k from all elements in B giving A }
{                                                                         }
{ *********************************************************************** }
function TCMatrix.sub (k : Double; m : TCMatrix) : TCMatrix;
var i, j : Integer;
begin
  for i := 1 to r do
      for j := 1 to c do
          Self.setVal (i, j, complexSub (complex (k, 0), m[i,j]));
  result := Self;
end;


{ **********************************************************************  }
{ Subtract matrix 'm' to scalar k, giving a new Self                      }
{                                                                         }
{ Usage:  A.sub (k, B); subtract scalar k from all elements in B giving A }
{                                                                         }
{ *********************************************************************** }
function TCMatrix.sub (m : TCMatrix; k : Double) : TCMatrix;
var i, j : Integer;
begin
  for i := 1 to r do
      for j := 1 to c do
          Self.setVal (i, j, complexSub (m[i,j], complex (k, 0)));
  result := Self;
end;


{ ******************************************************************** }
{ Subtract matrix m from Self giving a new Self                        }
{                                                                      }
{ Usage:  A.subU (B);  subtract B from A giving A                      }
{                                                                      }
{ ******************************************************************** }
function TCMatrix.sub (m : TCMatrix) : TCMatrix;
var i, j : integer;
begin
  if Not SameDimensions (m, Self) then
     raise EMatrixSizeError.Create ('Incorrectly sized result matrix for matrix subtraction');

  for i := 1 to r do
      for j := 1 to c do
          Self.setVal (i, j, complexSub (Self[i,j], m[i,j]));
  result := Self;
end;



{ ******************************************************************** }
{ Subtract m2 from m1 giving Self                                      }
{                                                                      }
{ Usage:  A.sub (A1, A2);  subtract A2 from A1 giving A (A = A2 - A1)  }
{                                                                      }
{ ******************************************************************** }
function TCMatrix.sub (m1, m2 : TCMatrix) : TCMatrix;
var i, j : integer;
begin
  if Not SameDimensions (m1, m2) then
     raise EMatrixSizeError.Create ('Incompatible matrix operands to subtract');

  if Not SameDimensions (m1, Self) then
     raise EMatrixSizeError.Create ('Incorrectly sized result matrix for matrix subtraction');

  for i := 1 to r do
      for j := 1 to c do
          Self.setVal (i, j, complexSub (m1[i,j], m2[i,j]));
  result := Self;
end;


{ ******************************************************************** }
{ Subtract m2 from m1 giving Self                                      }
{                                                                      }
{ Usage:  A.sub (A1, A2);  subtract A2 from A1 giving A (A = A2 - A1)  }
{                                                                      }
{ ******************************************************************** }
function TCMatrix.sub (m1 : TCMatrix; m2 : TMatrix) : TCMatrix;
var i, j : integer;
begin
  if Not SameDimensions (m1, m2) then
     raise EMatrixSizeError.Create ('Incompatible matrix operands to subtract');

  if Not SameDimensions (m1, Self) then
     raise EMatrixSizeError.Create ('Incorrectly sized result matrix for matrix subtraction');

  for i := 1 to r do
      for j := 1 to c do
          Self.setVal (i, j, complexSub (m1[i,j], complex (m2[i,j], 0.0)));
  result := Self;
end;


{ ******************************************************************** }
{ Subtract m2 from m1 giving Self                                      }
{                                                                      }
{ Usage:  A.sub (A1, A2);  subtract A2 from A1 giving A (A = A2 - A1)  }
{                                                                      }
{ ******************************************************************** }
function TCMatrix.sub (m1 : TMatrix; m2 : TCMatrix) : TCMatrix;
var i, j : integer;
begin
  if Not SameDimensions (m1, m2) then
     raise EMatrixSizeError.Create ('Incompatible matrix operands to subtract');

  if Not SameDimensions (m1, Self) then
     raise EMatrixSizeError.Create ('Incorrectly sized result matrix for matrix subtraction');

  for i := 1 to r do
      for j := 1 to c do
          Self.setVal (i, j, complexSub (complex (m1[i,j], 0.0), m2[i,j]));
  result := Self;
end;


{ ******************************************************************** }
{ Multiply a matrix 'm' by scalar constant k and assign result to Self }
{                                                                      }
{ Usage: A.multk (B, 0.5);  multiply scalar, 0.5 by B giving A         }
{                                                                      }
{ ******************************************************************** }
function TCMatrix.mult (m : TCMatrix; k : TCMatElement) : TCMatrix;
var i, j : integer;
begin
  for i := 1 to m.r do
      for j := 1 to m.c do
          Self.setVal (i, j, complexMult (m[i,j], k));
  result := Self;
end;


{ ******************************************************************** }
{ Multiply the Self matrix by the scalar constant k                    }
{                                                                      }
{ Usage:  A.multKU (0.5);  multiply scalar 0.5 by A giving A           }
{                                                                      }
{ ******************************************************************** }
function TCMatrix.mult (k : TCMatElement) : TCMatrix;
var i, j : integer;
begin
  for i := 1 to r do
      for j := 1 to c do
          Self.setVal (i, j, complexMult (Self[i,j], k));
  result := Self;
end;



{ ******************************************************************** }
{ Multiply matrix 'm1' by 'm2' to give result in Self                  }
{                                                                      }
{ Usage:  A.mult (A1, A2); multiply A1 by A2 giving A                  }
{                                                                      }
{ ******************************************************************** }
function TCMatrix.mult (m1, m2 : TCMatrix) : TCMatrix;
var i, j, k, m1_Col : integer; sum : TCMatElement;
begin
  if m1.c = m2.r then
     begin
      Self.FreeSpace; Self.SetSize (m1.r, m2.c);
      m1_col := m1.c;
      for i := 1 to Self.r do
          for j := 1 to m2.c do
              begin
              sum := complex (0.0, 0.0);
              for k := 1 to m1_Col do
                  sum := complexAdd (sum, complexMult (m1[i, k], m2[k, j]));
              Self[i,j] := sum;
              end;
      result := Self;
    end
  else
     raise EMatrixSizeError.Create ('Incompatible matrix operands to multiply');
end;


function TCMatrix.mult (m1 : TMatrix; m2 : TCMatrix) : TCMatrix;
var i, j, k, m1_Col : integer; sum : TCMatElement;
begin
  if m1.c = m2.r then
     begin
      Self.FreeSpace; Self.SetSize (m1.r, m2.c);
      m1_col := m1.c;
      for i := 1 to Self.r do
          for j := 1 to m2.c do
              begin
              sum := complex (0.0, 0.0);
              for k := 1 to m1_Col do
                  sum := complexAdd (sum, complexMult (complex (m1[i, k], 0.0), m2[k, j]));
              Self[i,j] := sum;
              end;
      result := Self;
    end
  else
     raise EMatrixSizeError.Create ('Incompatible matrix operands to multiply');
end;




function TCMatrix.mult (m1 : TCMatrix; m2 : TMatrix) : TCMatrix;
var i, j, k, m1_Col : integer; sum : TCMatElement;
begin
  if m1.c = m2.r then
     begin
      Self.FreeSpace; Self.SetSize (m1.r, m2.c);
      m1_col := m1.c;
      for i := 1 to Self.r do
          for j := 1 to m2.c do
              begin
              sum := complex (0.0, 0.0);
              for k := 1 to m1_Col do
                  sum := complexAdd (sum, complexMult (m1[i, k], complex (m2[k, j], 0.0)));
              Self[i,j] := sum;
              end;
      result := Self;
    end
  else
     raise EMatrixSizeError.Create ('Incompatible matrix operands to multiply');
end;


{ ******************************************************************** }
{ Multiply vector 'v' by 'm' to give result in Self                    }
{                                                                      }
{ Usage:  A.mult (v1, A1); multiply v1 by A1 giving A                  }
{                                                                      }
{ ******************************************************************** }
function TCMatrix.mult (v : TCVector; m : TCMatrix) : TCMatrix;
var i, j : integer; sum : TComplex;
begin
  if v.Size = m.r then
     begin
     Self.FreeSpace; Self.SetSize (1, m.c);
     for i := 1 to m.c do
         begin
         sum := complex (0.0, 0.0);
         for j := 1 to v.Size do
             sum := complexAdd (sum, complexMult (v[j], m[i,j]));
         Self[1,i] := sum;
         end;
     result := Self;
     end
  else
     raise EMatrixSizeError.Create ('Incompatible vector and matrix operands to multiply');
end;



// -------------------------------------------------------------------------
// Applies the function f to all element of matrix Self
// Usage: m.ApplyFunction (MyFunction)
// -------------------------------------------------------------------------
function TCMatrix.ApplyUnaryFunction (const f : TUnaryComplexFunc) : TCMatrix;
var i, j : integer;
begin
  for i := 1 to Self.r do
      for j := 1 to Self.c do
      Self[i,j] := f (Self[i, j]);
  Result := Self;
end;


// -------------------------------------------------------------------------
// Applies the function f to all element of matrix Self
// Usage: m.ApplyFunction (MyFunction)
// -------------------------------------------------------------------------
function TCMatrix.ApplyBinaryFunction (f : TBinaryComplexFunc; x, y : TComplex) : TCMatrix;
var i, j : integer;
begin
  for i := 1 to Self.r do
      for j := 1 to Self.c do
      Self[i,j] := f (x, y);
  Result := Self;
end;


{ ******************************************************************** }
{ Transpose matrix 'Self', Self is thus destroyed and replaced         }
{                                                                      }
{ Usage:  A.transpose                                                  }
{                                                                      }
{ ******************************************************************** }
function TCMatrix.Transpose : TCMatrix;
var tmp : TCMatrix;
begin
  tmp := TCMatrix.create (c, r);
  try
    // Not a very efficient method I know....
    tmp.Transpose (Self);
    // move data from transpose to Self
    Self.FreeSpace;
    Self.setSize (tmp.r, tmp.c);
    Self.Copy (tmp);
  finally
    tmp.Destroy;
  end;
  result := Self;
end;


{ ******************************************************************* }
{ Transpose the matrix 'm' into Self                                  }
{                                                                     }
{ Usage:  T.transpose (A);   Transposes A and puts result into T      }
{ Will also accept T.transpose (T)                                    }
{ ******************************************************************* }
function TCMatrix.Transpose (m : TCMatrix) : TCMatrix;
var i, j : integer; t : TCMatrix;
begin
  if (m.r <> Self.c) and (m.c <> Self.r) then
     raise EMatrixSizeError.Create ('Destination matrix has incorrect dimensions for transpose');
  { If the user is trying to transpose itself.... }
  if Self = m then
     begin
     t := TCMatrix.Create (r, c);
     try
       t.Copy (m);
       for i := 1 to m.r do
           for j := 1 to m.c do
               Self[j,i] := t[i,j];
     finally
       t.free;
     end;
     end
  else
     begin
     for i := 1 to m.r do
         for j := 1 to m.c do
             Self[j,i] := m[i,j];
     end;

  Self.Name := m.Name;
  Self.FRowNames.clear;
  Self.FColumnNames.clear;
  if m.c > 0 then Self.FRowNames.Assign (m.FColumnNames);
  if m.r > 0 then Self.FColumnNames.Assign (m.FRowNames);
  result := Self;
end;


{ ******************************************************************** }
{ Returns the sum of elements in the matrix                            }
{                                                                      }
{ Usage s := m.Sum                                                     }
{ ******************************************************************** }
function TCMatrix.Sum : TComplex;
var i, j : integer;
begin
  Result := complex (0.0, 0.0);
  for i := 1 to nr do
      for j := 1 to nc do
          Result := complexAdd (Result, Self[i,j]);
end;


{ ******************************************************************** }
{ Returns the product of elements in the matrix                        }
{                                                                      }
{ Usage s := m.Sum                                                     }
{ ******************************************************************** }
function TCMatrix.Prod : TComplex;
var i, j : integer;
begin
  Result := complex (1.0, 0.0);
  for i := 1 to nr do
      for j := 1 to nc do
          Result := complexMult (Result, Self[i,j]);
end;


{ ******************************************************************** }
{ Returns the mean of elements in the matrix                           }
{                                                                      }
{ Usage m.Mean                                                         }
{ ******************************************************************** }
function TCMatrix.Mean : TComplex;
var i, j : integer;
begin
  Result := complex (0.0, 0.0);
  for i := 1 to nr do
      for j := 1 to nc do
          Result := complexAdd (Result, Self[i,j]);
  Result := complexDiv (Result, complex (nr*nc, 0.0));
end;


{ ******************************************************************** }
{ Returns the standard deviation of the elements in the matrix         }
{                                                                      }
{ Usage m.StdDev                                                         }
{ ******************************************************************** }
function TCMatrix.StdDev : TComplex;
var i, j : integer;
    sq : TComplex;
    total : TComplex;
begin
  Result := complex (0.0, 0.0);
  sq := complex (0, 0); total := complex (0, 0);
   for i := 1 to nr do
      for j := 1 to nc do
          begin
          sq := complexAdd (sq, complexSqr(Self[i,j])); total := complexAdd (total, Self[i,j]);
          end;
     result := complexSub (sq, complexDiv (complexSqr(total), complex (nr*nc, 0)));
     result := complexSqrt (complexDiv (result, complex ((nr*nc)-1, 0)));
end;



{ ******************************************************************** }
{ Returns the sum of squares of elements in the matrix                 }
{                                                                      }
{ Usage m.SumOfSquares                                                 }
{ ******************************************************************** }
function TCMatrix.SumOfSquares : TComplex;
var i, j : integer;
begin
  Result := complex (0.0, 0.0);
  for i := 1 to nr do
      for j := 1 to nc do
          Result := complexAdd (Result, complexMult (Self[i,j], Self[i,j]));
end;



{ ******************************************************************** }
{ Find the rank r, of the matrix Self, The reduced Row                 }
{ echelon is returned in mat. tol is the magnitude of                  }
{ the largest number before it is assumed to be zero.                  }
{                                                                      }
{ Usage:  r := A.Rank (echelon, 1e-8)                                  }
{         Find the rank of A, place echelon in echelon                 }
{                                                                      }
{ ******************************************************************** }
function TCMatrix.Rank (echelon : TCMatrix; tol : double) : integer;
var Arow, Acol, i, j, n, m, RowScan : integer;
    factor : TComplex;
begin
  n := Self.r; m := Self.c;
  if (n = 0) or (m = 0) then
     begin
     Result := 0;
     Exit;
     end;

  echelon.copy (Self);    // Work on Echelon, not Self, so make a copy of Self
  if Tol <> 0.0 then echelon.Tolerance := Tol;

  if (Tol <> 0.0) then
     InternalTolerance := Tol;

  Arow := 1; Acol := 1;
  repeat
    // locate a nonzero column
    if echelon.IsZero (Arow, Acol) then
       begin
       { If the entry is zero work our way down the matrix
       looking for a nonzero entry, when found, swap it for Arow }
       RowScan := Arow;
       repeat
         // next row
         inc (RowScan);
         // Have we reached the end of the rows but we've still got columns left to scan?
         if (RowScan > n) and (Acol < m) then
           begin
           { reset row counter back to where it was and try next column }
           RowScan := Arow; inc (Acol);
           end;

         { If we've scanned the whole matrix, then lets get out... }
         if (RowScan > n) then
            begin
            CleanUpCMatrix (echelon);
            result := ComputeRank (echelon);
            exit;
            end;
       until echelon.IsNotZero (RowScan, Acol);   { keep searching until non-zero entry found }

       { We've found a nonzero row entry so swap it with
       'Arow' which did have a zero as its entry }
       echelon.exchangeRows (Arow, RowScan);
       end;

    { Arow now holds the row of interest }
    factor := complexDiv (complex (1.0, 0.0), echelon [Arow, Acol]);
    { reduce all the entries along the column by the factor }
    for i := Acol to m do echelon[Arow,i] := complexMult (echelon[Arow, i], factor);

    { now eliminate all entries above and below Arow, this generates the reduced form }
    for i := 1 to n do
        { miss out Arow itself }
        if (i <> Arow) and echelon.IsNotZero (i, Acol) then
           begin
           factor := echelon [i, Acol];
           { work your way along the column doing the same operation }
           for j := Acol to m do
               begin
               echelon[i,j] := complexSub (echelon [i, j], complexMult (factor, echelon [Arow, j]));
               end;
           end;

    inc (Arow); inc (Acol);
 until (Arow > n) or (Acol > m);
 CleanUpCMatrix (echelon);
 result := ComputeRank (echelon);   { This is just a patch for the moment }
end;



{ ******************************************************************** }
{ Save the matrix to a string                                          }
{                                                                      }
{ Usage: m.SaveToString;                                               }
{                                                                      }
{ ******************************************************************** }
function TCMatrix.SaveToString : string;
var i, j : integer;
begin
  Result := '';
  for i := 1 to Self.r do
      begin
      for j := 1 to Self.c do
          Result := Result + Format ('%f ', [Self[i, j].r, Self[i, j].i]);
      Result := Result + #13#10;
      end;
end;


{ ******************************************************************** }
{ Save the matrix to a file as CSV                                     }
{                                                                      }
{ Usage: m.SaveToString;                                               }
{                                                                      }
{ ******************************************************************** }
procedure TCMatrix.SaveAsCSV (fileName : string; delimiter : char);
var i, j : integer;
    f : TextFile;
begin
  AssignFile (f, FileName);
  try
  rewrite (f);
  write (f, Self.getColumnName (1));
  for i := 2 to Self.c do
      write (f, delimiter + ' ' + Self.getColumnName (i));
  writeln (f);

  for i := 1 to Self.r do
      begin
      if Self[i,j].i = 0 then
         write (f, Format ('%e', [Self[i, 1].r]))
      else
         begin
         write (f, Format ('%e', [Self[i, 1].r]));
         if Self[i,j].i < 0 then
            write (f, Format (delimiter + ' -%e', [Self[i, 1].i]))
         else
            write (f, Format (delimiter + ' +%e', [Self[i, 1].i]));
         end;

      for j := 2 to Self.c do
          begin
          if Self[i,j].i = 0 then
             write (f, Format (delimiter + ' %e', [Self[i, j].r]))
          else
             begin
             write (f, Format (delimiter + ' %e', [Self[i, j].r]));
             if Self[i,j].i < 0 then
                write (f, Format (delimiter + ' -%e', [Self[i, j].i]))
             else
                write (f, Format (delimiter + ' +%e', [Self[i, j].i]));
             end;
          end;
      writeln (f);
      end;
  except
    on e: Exception do
         raise Exception.Create ('Error while trying to save csv data: ' + e.Message);
  end;
  closeFile (f);
end;

{ ******************************************************************** }
{ Save the matrix to a file                                            }
{                                                                      }
{ Usage: m.Save ('file.m');                                            }
{                                                                      }
{ ******************************************************************** }
procedure TCMatrix.Save (FileName : string);
var f : TextFile; i, j : integer;
begin
  AssignFile (f, FileName);
  rewrite (f);
  writeln (f, Self.r, ' ', Self.c);
  for i := 1 to Self.r do
      begin
      for j := 1 to Self.c do
          write (f, Format ('%f %f', [Self[i, j].r, Self[i, j].i]));
      writeln (f);
      end;
  CloseFile (f);
end;


{ ******************************************************************** }
{ Load the matrix from a file                                          }
{                                                                      }
{ Usage: m.Load ('file.m');                                            }
{                                                                      }
{ ******************************************************************** }
procedure TCMatrix.Load (FileName : string);
var f : TextFile; i, j, nRows, nColumns : integer; r, imag : double;
begin
  AssignFile (f, FileName);
  reset (f);
  readln (f, nRows, nColumns);
  Self.SetSize (nRows, nColumns);
  for i := 1 to nRows do
      begin
      for j := 1 to nColumns do
          begin
          read (f, r);
          read (f, imag);
          Self[i, j] := complex (r, imag);
          end;
      readln (f);
      end;
  CloseFile (f);
end;


{ ******************************************************************** }
{ Save a matrix to the stream                                          }
{                                                                      }
{ Usage: m.SaveToStream (Stream);                                      }
{                                                                      }
{ ******************************************************************** }
procedure TCMatrix.SaveToStream (Stream : TStream);
var i : integer;
begin
  Stream.Write (nr, Sizeof (nr));
  Stream.Write (nc, Sizeof (nc));
  // Save entire rows at a time
  for i := 1 to nr do
      Stream.Write (mx^[i]^, SizeOf (TCMatElement) * (nc+1));
end;


{ ******************************************************************** }
{ Load a matrix form the stream                                        }
{                                                                      }
{ Usage: m := TCMatrix.LoadFromStream (Stream);                        }
{                                                                      }
{ ******************************************************************** }
constructor TCMatrix.LoadFromStream (Stream : TStream);
var nRows, nColumns, i : integer;
begin
  Create;
  with Stream do
       begin
       Read (nRows, Sizeof (nr));
       Read (nColumns, Sizeof (nc));
       Self.SetSize (nRows, nColumns);
       for i := 1 to nRows do
           Read (mx^[i]^, SizeOf (TCMatElement) * (nColumns+1));
       end;
end;


{ -------------------------  Constructors first  ---------------------------- }


// Base Constructor - the mother of all matrix constructors
// Create an empty matrix
constructor TMatrix.Create;
begin
  inherited Create;
  mx := nil;
end;


// And now some specialised constructors........

{ ******************************************************************** }
{ Create a matrix of specific size, all bytes are zeroed               }
{                                                                      }
{ Usage:  A := TMatrix.create (3, 2);                                  }
{ ******************************************************************** }
constructor TMatrix.Create (r, c : integer);
begin
  Create;
  Self.SetSize (r, c);
  Self.Name := 'untitled';
end;


{ ******************************************************************** }
{ Create an identity matrix                                            }
{                                                                      }
{ Usage:   A := TMatrix.create (3);                                    }
{ ******************************************************************** }
constructor TMatrix.Create (n : integer);
var i : integer;
begin
  Create;
  Self.SetSize (n, n);
  for i := 1 to n do Self[i,i] := 1.0;
end;


{ ******************************************************************** }
{ Create a square matrix of size n filled with value d                 }
{                                                                      }
{ Usage:   A := TMatrix.create (3, 3.1415);                            }
{ ******************************************************************** }
constructor TMatrix.Create (n : integer; d : double);
var i, j : integer;
begin
  Create;
  Self.SetSize (n, n);
  for i := 1 to n do
      for j := 1 to n do
          Self[i,j] := d;
end;


{ ******************************************************************** }
{ Create a matrix of size n by m filled with value d                   }
{                                                                      }
{ Usage:   A := TMatrix.create (3, 4, 3.1415);                         }
{ ******************************************************************** }
constructor TMatrix.Create (n, m : integer; d : double);
var i, j : integer;
begin
  Create;
  Self.SetSize (n, m);
  for i := 1 to n do
      for j := 1 to m do
          Self[i,j] := d;
end;


{ ******************************************************************** }
{ Create a matrix filled with values from array d given that the       }
{ number of columns equals c.                                          }
{                                                                      }
{ Usage:  A := TMatrix.create (2, [1, 2, 3, 4]);                       }
{         Creates a 2 by 2 matrix                                      }
{ ******************************************************************** }
constructor  TMatrix.Create (c : integer; d : array of TDMatElement);
var i, j, ri, count : integer;
begin
  Create;
  ri := (High(d)+1) div c;
  Self.SetSize (ri, c);
  count := 0;
  for i := 1 to ri do
      for j := 1 to c do
          begin
          Self[i,j] := d[count];
          inc (count);
          end;
end;


{ ******************************************************************** }
{ Create identical copy of Self                                        }
{                                                                      }
{ Usage: n := TMatrix.Clone (m)                                        }
{ ******************************************************************** }
constructor TMatrix.Clone (m : TMatrix);
begin
  Create;
  Self.SetSize (m.nr, m.nc);
  Self.Copy (m);
end;



{ ******************************************************************** }
{ Create a diagonal matrix with value d                                }
{                                                                      }
{ Usage: n := TMatrix.Diag (m, d)                                      }
{ ******************************************************************** }
constructor TMatrix.Diag (n : integer; d : double);
var i : integer;
begin
  Create;
  Self.SetSize (n, n);
  Self.Zero;
  for i := 1 to n do Self[i,i] := d;
end;


{ ******************************************************************** }
{ Usage:    A.destroy, use A.free in a program                         }
{ ******************************************************************** }
destructor TMatrix.destroy;
begin
  FreeSpace;
  Inherited Destroy;
end;


procedure TMatrix.ReSize (ri, ci : integer);
begin
  Self.SetSize (ri, ci);
end;


{ Free the data space but not the object }
procedure TMatrix.FreeSpace;
var i : integer;
begin
  if mx <> Nil then
     begin
     for i := 1 to nr do
         if mx^[i] <> Nil then
            begin FreeMem (mx^[i], sizeof (TDMatElement) * (nc+1)); mx^[i] := Nil; end;
     FreeMem (mx, sizeof (PTDRowList) * (nr+1)); mx := Nil;
     end;
  DestroyNameLists;
end;


// Internal routine used to set the size of the matrix and allocate space
// DESTROYS ANY DATA !
procedure TMatrix.SetSize (ri, ci : integer);
var i : integer;
begin
  FreeSpace;
  nr := ri; nc := ci;
  mx := AllocMem (sizeof (pTDRowList) * (nr+1));  { r+1 so that we can index from 1 }
  for i := 1 to nr do  mx^[i] := AllocMem (sizeof (TDMatElement) * (nc+1));
  CreateNameLists (ri, ci);
end;


function TMatrix.getPtr : pTDRowList;
begin
  Result := mx;
end;


{ ******************************************************************** }
{ Add an empty row to the bottom of matrix without destroying data     }
{ in other rows                                                        }
{                                                                      }
{ Usage:  m.AddRow                                                     }
{ ******************************************************************** }
procedure TMatrix.AddRow;
var tmp : TMatrix; i : integer;
begin
  tmp := TMatrix.Create (r+1, c); tmp.zero;
  try
    // Copy a whole row at a time using move: Source, Dest, Count }
    for i := 1 to r do move (Self.mx^[i]^, tmp.mx^[i]^, sizeof(TDMatElement) * (c+1));
    for i := 1 to r do tmp.rName[i] := Self.rName[i];
    for i := 1 to c do tmp.cname[i] := Self.cName[i];
    tmp.rName[r+1] := 'R' + IntToStr (r+1);
    //tmp.FRowNames.Assign (Self.FRowNames); tmp.FColumnNames.Assign (Self.FColumnNames);
    Self.FreeSpace; Self.SetSize (tmp.nr, tmp.nc);
    Self.Copy (tmp);
  finally
    tmp.free;
  end;
end;


{ ******************************************************************** }
{ Destroy Sef and recreate it with a single row of ColSize columns     }
{                                                                      }
{ Usage: m.AddFirstRow (4)                                             }
{ ******************************************************************** }
procedure TMatrix.AddFirstRow (ColSize : integer);
begin
  if mx <> nil then Self.FreeSpace;
  Self.setSize (1, ColSize);
end;


{ ---------------------------------------------------------------------------- }
{                               BASIC ROUTINES                                 }
{ ---------------------------------------------------------------------------- }


{ ******************************************************************** }
{ Normal Usage:  A[2, 3] := 1.2;                                       }
{ ******************************************************************** }
procedure TMatrix.Setval (ri, ci : integer; v : TDMatElement);
begin
{$ifdef DEBUG}
  CheckLimits (ri, ci);
{$endif}
  mx^[ri]^[ci] := v;
end;


{ ******************************************************************** }
{ Normal Usage:  d := A[2, 3];                                         }
{ ******************************************************************** }
function TMatrix.Getval (ri, ci : integer) : TDMatElement;
begin
{$ifdef DEBUG}
  CheckLimits (ri, ci);
{$endif}
  result := mx^[ri]^[ci];
end;



{ ******************************************************************** }
{ Fill an existing matrix with the array d of numbers. ci equals       }
{ the number of columns.                                               }
{                                                                      }
{ Usage:   A.L(3, [1, 2, 3, 4, 5, 6, 7, 8, 9]);                        }
{                                                                      }
{ ******************************************************************** }
function TMatrix.L (ci :integer; d : array of TDMatElement) : TMatrix;
var i, j, ri, count : integer;
begin
  ri := (High(d)+1) div ci;
  FreeMem (mx, sizeof (TDMatElement) * nr * nc);
  Self.SetSize (ri, ci);
  count := 0;
  for i := 1 to ri do
      for j := 1 to ci do
          begin
          Self[i,j] := d[count];
          inc (count);
          end;
  result := Self;
end;


{ ******************************************************************** }
{ Zero the Self matrix                                                 }
{                                                                      }
{ Usage: A.Zero;                                                       }
{                                                                      }
{ ******************************************************************** }
function TMatrix.Zero : TMatrix;
var i, j : integer;
begin
  for i := 1 to r do
      for j := 1 to c do
          Self[i,j] := 0.0;
  result := Self;
end;


{ ******************************************************************** }
{ Create a matrix filled with random numbers (0 <= el < 1.0            }
{ Uses built-in random number generator                                }
{                                                                      }
{ Usage:  m.RandomMat                                                  }
{ ******************************************************************** }
function TMatrix.RandomMat : TMatrix;
var i, j : integer;
begin
  for i := 1 to Self.r do
      for j := 1 to Self.c do
          Self[i,j] := Random;
  Result := Self;
end;


{ ******************************************************************** }
{ Create a matrix filled with random numbers (Lower <= el <= Upper)    }
{                                                                      }
{ Usage:   A := TMatrix.RandomMat (-10, 10)                            }
{ ******************************************************************** }
function TMatrix.RandomMat (Lower, Upper : integer) : TMatrix;
var i, j : integer;
begin
  for i := 1 to Self.r do
      for j := 1 to Self.c do
          Self[i,j] := (Random * (Upper - Lower)) - System.abs (Lower);
  Result := Self;
end;


{ ******************************************************************** }
{ Returns true if element r, c is not zero                             }
{                                                                      }
{ Usage: if m.IsNotZero (1, 2) then                                    }
{ ******************************************************************** }
function TMatrix.IsNotZero (r, c : integer) : Boolean;
begin
  result := (System.abs (m[r, c]) > InternalTolerance)
end;


{ ******************************************************************** }
{ Returns true if element r, c is zero                                 }
{                                                                      }
{ Usage: if m.IsZero (1, 2) then                                       }
{ ******************************************************************** }
function TMatrix.IsZero (r, c : integer) : boolean;
begin
  result := System.abs (m[r, c]) < InternalTolerance;
end;


{ ******************************************************************** }
{ Returns true if the matrix is zero                                   }
{                                                                      }
{ Usage: if m.IsZero then                                              }
{ ******************************************************************** }
function TMatrix.IsZero : Boolean;
var i, j : Integer;
begin
  Result := True;
  for i := 1 to nr do
      for j := 1 to nc do
          if not IsZero (i, j) then
             begin
             Result := False;
             Exit;
             end;
end;

{ ******************************************************************** }
{ Returns true if element r, c is Unity (+1 or -1)                     }
{                                                                      }
{ Usage: if m.IsOne (1, 2) then                                        }
{                                                                      }
{ ******************************************************************** }
function TMatrix.IsOne (r, c : integer) : boolean;
begin
   result := (System.abs (m[r, c]) < 1+SMALLNUMBER) and (System.abs (m[r, c]) > 1-SMALLNUMBER);
end;


{ ******************************************************************** }
{ Returns the number of bytes occupied by matix                        }
{                                                                      }
{ Usage:  A.SizeOf                                                     }
{                                                                      }
{ ******************************************************************** }
function TMatrix.GetSizeOf : integer;
var i : integer;
begin
  Result := nr*nc*SizeOf (TDMatElement) + Length (FName) + SizeOf (InternalTolerance);
  for i := 0 to FRowNames.Count - 1 do
      Result := Result + Length (FRowNames[i]);
  for i := 0 to FColumnNames.Count - 1 do
      Result := Result + Length (FColumnNames[i]);
end;


{ ******************************************************************** }
{ Turn the matrix Self into an identify matrix                         }
{                                                                      }
{ Usage:  A.Identity                                                   }
{                                                                      }
{ ******************************************************************** }
function TMatrix.Identity : TMatrix;
var i : integer;
begin
  if Self.IsSquare then
     begin
     Self.Zero;
     for i := 1 to r do Self[i,i] := 1.0;
     result := Self;
     end
  else
     raise EMatrixSizeError.Create ('An identity matrix can only be formed from a square matrix');
end;


{ ******************************************************************** }
{ Make the matrix object a diagonal matrix with the value, k           }
{                                                                      }
{ Usage: A.Diagonal (3.1415);                                          }
{                                                                      }
{ ******************************************************************** }
function TMatrix.Diagonal (k : TDMatElement) : TMatrix;
var i : integer;
begin
  if Self.IsSquare then
     begin
     Self.Zero;
     for i := 1 to r do Self[i,i] := k;
     result := Self;
     end
  else
     raise EMatrixSizeError.Create ('Can only form a diagonal matrix from a square matrix');
end;


{ ******************************************************************** }
{ This forms a diagonal matrix from the elements of vector v.          }
{                                                                      }
{ Usage: A.Diagonal (v)                                                }
{                                                                      }
{ ******************************************************************** }
function TMatrix.Diagonal (v : TDVector) : TMatrix;
var i : integer;
begin
  if Self.IsSquare then
     begin
     if v.size = Self.nr then
        begin
        Self.zero;
        for i := 1 to r do Self[i,i] := v[i];
        result := Self;
        end
     else
        raise EMatrixSizeError.Create ('Vector must be same size as matrix in Diagonal');
     end
  else
     raise EMatrixSizeError.Create ('Can only form a diagonal matrix from a square matrix');
end;


{ ******************************************************************** }
{ This forms a diagonal matrix from the elements of v.                }
{                                                                      }
{ Usage: A.Diagonal (v)                                                }
{                                                                      }
{ ******************************************************************** }
function TMatrix.Diagonal (v : array of double) : TMatrix;
var i : integer;
begin
  if Self.IsSquare then
     begin
     // Note: High returns the highest range value, not the number of elements which is nr
     if High (v) + 1 = Self.nr then
        begin
        Self.zero;
        for i := 1 to r do Self[i,i] := v[i-1];  // v starts from v[0]
        result := Self;
        end
     else
        raise EMatrixSizeError.Create ('Array must have the same dimension as the matrix in Diagonal');
     end
  else
     raise EMatrixSizeError.Create ('Can only form a diagonal matrix from a square matrix');
end;


{ ******************************************************************** }
{ Transpose matrix 'Self', Self is thus destroyed and replaced         }
{                                                                      }
{ Usage:  A.transpose                                                  }
{                                                                      }
{ ******************************************************************** }
function TMatrix.Transpose : TMatrix;
var tmp : TMatrix;
begin
  tmp := TMatrix.create (c, r);
  try
    // Not a very efficient method I know....
    tmp.Transpose (Self);
    // move data from transpose to Self
    Self.FreeSpace;
    Self.setSize (tmp.r, tmp.c);
    Self.Copy (tmp);
  finally
    tmp.Destroy;
  end;
  result := Self;
end;


{ ******************************************************************* }
{ Transpose the matrix 'm' into Self                                  }
{                                                                     }
{ Usage:  T.transpose (A);   Transposes A and puts result into T      }
{ Will also accept T.transpose (T)                                    }
{ ******************************************************************* }
function TMatrix.Transpose (m : TMatrix) : TMatrix;
var i, j : integer; t : TMatrix;
begin
  if (m.r <> Self.c) and (m.c <> Self.r) then
     raise EMatrixSizeError.Create ('Destination matrix has incorrect dimensions for transpose');
  { If the user is trying to transpose itself.... }
  if Self = m then
     begin
     t := TMatrix.Create (r, c);
     try
       t.Copy (m);
       for i := 1 to m.r do
           for j := 1 to m.c do
               Self[j,i] := t[i,j];
     finally
       t.free;
     end;
     end
  else
     begin
     for i := 1 to m.r do
         for j := 1 to m.c do
             Self[j,i] := m[i,j];
     end;

  Self.Name := m.Name;
  Self.FRowNames.clear;
  Self.FColumnNames.clear;
  if m.c > 0 then Self.FRowNames.Assign (m.FColumnNames);
  if m.r > 0 then Self.FColumnNames.Assign (m.FRowNames);
  result := Self;
end;



{ ******************************************************************** }
{ Returns the Frobenuis Norm (similar to Euclidian norm)               }
{                                                                      }
{ Usage m.Norm                                                         }
{ ******************************************************************** }
function TMatrix.Norm : Double;
var i, j : integer;
begin
  Result := 0.0;
  for i := 1 to nr do
      for j := 1 to nc do
          Result := Result + sqr (Self[i,j]);
  Result := sqrt (Result);
end;


{ ******************************************************************** }
{ Returns the sum of elements in the matrix                            }
{                                                                      }
{ Usage s := m.Sum                                                     }
{ ******************************************************************** }
function TMatrix.Sum : Double;
var i, j : integer;
begin
  Result := 0.0;
  for i := 1 to nr do
      for j := 1 to nc do
          Result := Result + Self[i,j];
end;


{ ******************************************************************** }
{ Returns the product of elements in the matrix                        }
{                                                                      }
{ Usage s := m.Sum                                                     }
{ ******************************************************************** }
function TMatrix.Prod : Double;
var i, j : integer;
begin
  Result := 1.0;
  for i := 1 to nr do
      for j := 1 to nc do
          Result := Result * Self[i,j];
end;


{ ******************************************************************** }
{ Returns the mean of elements in the matrix                           }
{                                                                      }
{ Usage m.Mean                                                         }
{ ******************************************************************** }
function TMatrix.Mean : Extended;
var i, j : integer;
begin
  Result := 0.0;
  for i := 1 to nr do
      for j := 1 to nc do
          Result := Result + Self[i,j];
  Result := Result / nr*nc;
end;


{ ******************************************************************** }
{ Returns the sum of squares of elements in the matrix                 }
{                                                                      }
{ Usage m.SumOfSquares                                                 }
{ ******************************************************************** }
function TMatrix.SumOfSquares : Extended;
var i, j : integer;
begin
  Result := 0.0;
  for i := 1 to nr do
      for j := 1 to nc do
          Result := Result + sqr (Self[i,j]);
end;


{ ******************************************************************** }
{ Copy matrix 'm' to Self, Self must exist and is overwritten          }
{ in the process. This procedure does a fast deep copy of the matrix.  }
{                                                                      }
{ Usage:  B.Copy (A);   performs the operation:  B = A  with deep copy }
{                                                                      }
{ ******************************************************************** }
function TMatrix.Copy (m : TMatrix) : TMatrix;
var i : integer;
begin
  if (r <> m.r) or (c <> m.c) then
     begin
     if r <> m.r then
        raise EMatrixSizeError.Create (MATERROR + #13#10'Cannot copy matrices with different sized rows: dest<'
            + inttostr (r) + '> src<' + inttostr (m.r) + '>')
     else
        raise EMatrixSizeError.Create (MATERROR + #13#10'Cannot copy matrices with different sized columns: dest<'
            + inttostr (c) + '> src<' + inttostr (m.c) + '>');
     end;
  { Copy a whole row at a time using move }
  for i := 1 to r do move (m.mx^[i]^, Self.mx^[i]^, sizeof(TDMatElement) * (c+1));
  // Copy over column and row names, clear destination first then copy
  Self.FRowNames.clear;  Self.FColumnNames.clear;
  if m.r > 0 then Self.FRowNames.Assign (m.FRowNames);
  if m.c > 0 then Self.FColumnNames.Assign (m.FColumnNames);
  Self.Name := m.Name;
  result := Self;
end;


{ ******************************************************************** }
{ Equality test                                                        }
{                                                                      }
{ Usage:  TMatrix.Equal (A, B);   returns true if  A = B               }
{                                                                      }
{ ******************************************************************** }
class function TMatrix.Equal  (m1, m2 : TMatrix) : Boolean;
var i, j : integer;
begin
  if (m1.r <> m2.r) or (m1.c <> m2.c) then
     Result := False
  else
     begin
     for i := 1 to m1.r do
         for j := 1 to m1.c do
             if m1[i,j] <> m2[i,j] then
                begin
                Result := False;
                Exit;
                end;
     Result := True;
     end;
end;



{ ******************************************************************** }
{ Equality test                                                        }
{                                                                      }
{ Usage:  TMatrix.Equal (A, B);   returns true if  A = B               }
{                                                                      }
{ ******************************************************************** }
class function TMatrix.Equal  (m1 : TCMatrix; m2 : TMatrix) : Boolean;
var i, j : integer;
begin
  if (m1.r <> m2.r) or (m1.c <> m2.c) then
     Result := False
  else
     begin
     for i := 1 to m1.r do
         for j := 1 to m1.c do
              if not (complexEquals (m1[i,j], complex (m2[i,j], 0.0))) then
                begin
                Result := False;
                Exit;
                end;
     Result := True;
     end;
end;



{procedure TMatrix.CopyToClipBoard;
begin
  Clipboard.Assign(Self);
end;


function TMatrix.CopyFromClipBoard : TMatrix;
begin
  result.Assign(Clipboard);
end;}


{ ******************************************************************** }
{ Save the matrix to a string                                          }
{                                                                      }
{ Usage: m.SaveToString;                                               }
{                                                                      }
{ ******************************************************************** }
function TMatrix.SaveToString : string;
var i, j : integer;
begin
  Result := '';
  for i := 1 to Self.r do
      begin
      for j := 1 to Self.c do
          Result := Result + Format ('%f ', [Self[i, j]]);
      Result := Result + #13#10;
      end;
end;


{ ******************************************************************** }
{ Save the matrix to a file                                            }
{                                                                      }
{ Usage: m.Save ('file.m');                                            }
{                                                                      }
{ ******************************************************************** }
procedure TMatrix.Save (FileName : string);
var f : TextFile; i, j : integer;
begin
  AssignFile (f, FileName);
  rewrite (f);
  writeln (f, Self.r, ' ', Self.c);
  for i := 1 to Self.r do
      begin
      for j := 1 to Self.c do
          write (f, Format ('%f ', [Self[i, j]]));
      writeln (f);
      end;

  CloseFile (f);
end;


{ ******************************************************************** }
{ Load the matrix from a file                                          }
{                                                                      }
{ Usage: m.Load ('file.m');                                            }
{                                                                      }
{ ******************************************************************** }
procedure TMatrix.Load (FileName : string);
var f : TextFile; i, j, nRows, nColumns : integer; d : double;
begin
  AssignFile (f, FileName);
  reset (f);
  readln (f, nRows, nColumns);
  Self.SetSize (nRows, nColumns);
  for i := 1 to nRows do
      begin
      for j := 1 to nColumns do
          begin
          read (f, d);
          Self[i, j] := d;
          end;
      readln (f);
      end;
  CloseFile (f);
end;


{ ******************************************************************** }
{ Save a matrix to the stream                                          }
{                                                                      }
{ Usage: m.SaveToStream (Stream);                                      }
{                                                                      }
{ ******************************************************************** }
procedure TMatrix.SaveToStream (Stream : TStream);
var i : integer;
begin
  Stream.Write (nr, Sizeof (nr));
  Stream.Write (nc, Sizeof (nc));
  // Save entire rows at a time
  for i := 1 to nr do
      Stream.Write (mx^[i]^, SizeOf (TDMatElement) * (nc+1));
end;


{ ******************************************************************** }
{ Load a matrix form the stream                                        }
{                                                                      }
{ Usage: m := TMatrix.LoadFromStream (Stream);                         }
{                                                                      }
{ ******************************************************************** }
constructor TMatrix.LoadFromStream (Stream : TStream);
var nRows, nColumns, i : integer;
begin
  Create;
  with Stream do
       begin
       Read (nRows, Sizeof (nr));
       Read (nColumns, Sizeof (nc));
       Self.SetSize (nRows, nColumns);
       for i := 1 to nRows do
           Read (mx^[i]^, SizeOf (TDMatElement) * (nColumns+1));
       end;
end;


{ ******************************************************************** }
{ Augment m1 with m2 and return result. m1.r must equal m2.r !         }
{                                                                      }
{ Usage: m := TMatrix.Augment (m1, m2);                                }
{                                                                      }
{ ******************************************************************** }
constructor TMatrix.Augment (m1, m2 : TMatrix);
var i, j : Integer;
begin
  Create;
  if m1.r <> m2.r then
     raise EMatrixError.Create ('Matrix arguments must have the same row size in the aug function');
  Self.SetSize (m1.r, m1.c + m2.c);
  for i := 1 to m1.r do
      for j := 1 to m1.c do
          Self[i,j] := m1[i,j];
  for i := 1 to m1.r do
      for j := 1 to m2.c do
          Self[i,m1.c + j] := m2[i,j];
  for i := 1 to m1.r do
      Self.rName[i] := m1.rName [i];
  for i := 1 to m1.c do
      Self.cName[i] := m1.cName [i];
  for i := 1 to m2.c do
      Self.cName[m1.c + i] := m2.cName [i];
end;


{ ******************************************************************** }
{ Augment m1 with m2 and return result. m1.r must equal m2.r !         }
{                                                                      }
{ Usage: m := TMatrix.Augment (m1, m2);                                }
{                                                                      }
{ ******************************************************************** }
constructor TMatrix.Augment (m : TMatrix; v : TDVector);
var i, j : Integer;
begin
  Create;
  if m.r <> v.size then
     raise EMatrixError.Create ('Vector and matrix row size incompatible in aug function');
  Self.SetSize (m.r, m.c + 1);
  for i := 1 to m.r do
      for j := 1 to m.c do
          Self[i,j] := m[i,j];
  for j := 1 to v.size do
      Self[j, m.c + 1] := v[j];
  for i := 1 to m.r do
      Self.rName[i] := m.rName [i];
  for i := 1 to m.c do
      Self.cName[i] := m.cName [i];
  Self.cName[m.c + 1] := 'C' + IntToStr (m.c+1);
end;


{ ******************************************************************** }
{ Augment m1 with m2 and return result. m1.c must equal m2.c !         }
{ Adds rows of m2 to rows of m1                                        }
{                                                                      }
{ Usage: m := TMatrix.Augment (m1, m2);                                }
{                                                                      }
{ ******************************************************************** }
constructor TMatrix.AugmentRows (m1, m2 : TMatrix);
var i, j : Integer;
begin
  Create;
  if m1.c <> m2.c then
     raise EMatrixError.Create ('Matrix arguments must have the same column size in the row augment function (augr)');

  Self.SetSize (m1.r + m2.r, m1.c);
  for i := 1 to m1.r do
      for j := 1 to m1.c do
          Self[i,j] := m1[i,j];

  for i := 1 to m2.r do
      for j := 1 to m2.c do
          Self[m1.r + i, j] := m2[i,j];

  for i := 1 to m1.r do
      Self.rName[i] := m1.rName [i];
  for i := 1 to m2.r do
      Self.rName[m1.r + i] := m2.rName [i];
  for i := 1 to m1.c do
      Self.cName[i] := m1.cName [i];
end;


{ ******************************************************************** }
{ Extract row rr from the Self matrix and return it as a TDVector       }
{                                                                      }
{ Usage: v = m.ExtractRow (1) extract row 1 from m and place in v      }
{                                                                      }
{ ******************************************************************** }
function TMatrix.ExtractRow (rr : integer) : TDVector;
var i : integer;
begin
  if Self.InRange (rr, 1) then
     begin
     Result := TDVector.Create (Self.c);
     for i := 1 to Self.c do result[i] := Self[rr, i];
     end
  else
     raise EMatrixError.Create ('Row extract index out of range');
end;



{ ******************************************************************** }
{ Extract column cc from the Self matrix and return it as a TDVector    }
{                                                                      }
{ Usage: m.ExtractColumn (v, 1)  extract column 1 from m and place in v}
{                                                                      }
{ ******************************************************************** }
procedure TMatrix.ExtractColumn (var v : TDVector; cc : integer);
var i : integer;
begin
  v.freeSpace; v.SetSize (Self.r); { Create result vector of appropriate size }
  for i := 1 to Self.r do v[i] := Self[i, cc];
end;


{ ******************************************************************** }
{ Extract rwo rr from the Self matrix and return it as a TDVector       }
{                                                                      }
{ Usage: m.ExtractRow (v, 1)  extract row 1 from m and place in v      }
{                                                                      }
{ ******************************************************************** }
procedure TMatrix.ExtractRow (var v : TDVector; rr : integer);
var i : integer;
begin
  v.freespace; v.SetSize (Self.c);
  for i := 1 to Self.c do v[i] := Self[rr, i];
end;


procedure TMatrix.ExtractRow (var v : TCVector; rr : integer);
var i : integer;
begin
  v.freespace; v.SetSize (Self.c);
  for i := 1 to Self.c do v.setVal (i, complex (Self[rr, i], 0.0));
end;



function TMatrix.InsertRow (v : TDVector; rr : integer) : TMatrix;
var tmp : TMatrix; i, j : integer;
begin
  tmp := TMatrix.Create (Self.r + 1, Self.c);
  if rr = 1 then
     begin
     for i := 1 to Self.c do
         tmp[1,i] := v[i];
     for i := 2 to tmp.r do
         for j := 1 to Self.c do
             begin
             tmp.rName[i] := Self.rName[i-1];
             tmp[i,j] := Self[i-1,j];
             end;
     end
  else
     begin
     for i := 1 to rr - 1 do
         for j := 1 to Self.c do
             tmp[i,j] := Self[i,j];
     for i := 1 to Self.c do
         tmp[rr, i] := v[i];

     for i := rr to Self.r do
         for j := 1 to Self.c do
             tmp[i+1,j] := Self[i,j];
     end;
  Self.setSize (tmp.r, tmp.c);
  Self.Copy (tmp);
  tmp.Free;
  Result := Self;
end;


function TMatrix.DeleteRow (rr : integer) : TMatrix;
var tmp : TMatrix; i, j : integer;
begin
  if Self.r = 1 then
     begin
     Self.setSize (0, 0);
     Result := Self;
     Exit;
     end;

  tmp := TMatrix.Create (Self.r - 1, Self.c);
  if rr = 1 then
     begin
     for i := 2 to Self.r do
         for j := 1 to Self.c do
             tmp[i-1,j] := Self[i,j];
     end
  else
     begin
     for i := 1 to rr - 1 do
         for j := 1 to Self.c do
             tmp[i,j] := Self[i,j];
     if rr < Self.r then
        for i := rr+1 to Self.r do
            for j := 1 to Self.c do
                tmp[i-1,j] := Self[i,j];
     end;
  Self.setSize (tmp.r, tmp.c);
  Self.Copy (tmp);
  tmp.Free;
  Result := Self;
end;


{ ******************************************************************** }
{ Add matrix 'm' to scalar k, giving a new Self                        }
{                                                                      }
{ Usage:  A.add (k, B);   add scalar k to all elements in B giving A   }
{                                                                      }
{ ******************************************************************** }
function TMatrix.Add (k : Double; m : TMatrix) : TMatrix;
var i, j : Integer;
begin
  for i := 1 to r do
      for j := 1 to c do
          Self[i,j] := m[i,j] + k;
  result := Self;
end;


{ ******************************************************************** }
{ Add matrix 'm' to Self, giving a new Self                            }
{                                                                      }
{ Usage:  A.add (B);   add B to A, giving A                            }
{                                                                      }
{ ******************************************************************** }
function TMatrix.add (m : TMatrix) : TMatrix;
var i, j : integer;
begin
  if Not SameDimensions (m, Self) then
     raise EMatrixSizeError.Create ('Incorrectly sized result matrix for matrix addition');

  for i := 1 to r do
      for j := 1 to c do
          Self[i,j] := Self[i,j] + m[i,j];
  result := Self;
end;


{ ******************************************************************** }
{ Add matrix 'm1' and 'm2' and assign to Self                          }
{                                                                      }
{ Usage: A.add (A1, A2);  add A1 to A2 giving A                        }
{                                                                      }
{ ******************************************************************** }
function TMatrix.add (m1, m2 : TMatrix) : TMatrix;
var i, j : integer;
begin
  if Not SameDimensions (m1, m2) then
     raise EMatrixSizeError.Create ('Incompatible matrix operands to add');

  if Not SameDimensions (m1, Self) then
     raise EMatrixSizeError.Create ('Incorrectly sized result matrix for matrix addition');

  for i := 1 to r do
      for j := 1 to c do
          Self[i,j] := m1[i,j] + m2[i,j];
  result := Self;
end;


{ **********************************************************************  }
{ Subtract matrix 'm' to scalar k, giving a new Self                      }
{                                                                         }
{ Usage:  A.sub (k, B); subtract scalar k from all elements in B giving A }
{                                                                         }
{ *********************************************************************** }
function TMatrix.sub (k : Double; m : TMatrix) : TMatrix;
var i, j : Integer;
begin
  for i := 1 to r do
      for j := 1 to c do
          Self[i,j] := k - m[i,j];
  result := Self;
end;



{ **********************************************************************  }
{ Subtract matrix 'm' to scalar k, giving a new Self                      }
{                                                                         }
{ Usage:  A.sub (k, B); subtract scalar k from all elements in B giving A }
{                                                                         }
{ *********************************************************************** }
function TMatrix.sub (m : TMatrix; k : Double) : TMatrix;
var i, j : Integer;
begin
  for i := 1 to r do
      for j := 1 to c do
          Self[i,j] := m[i,j] - k;
  result := Self;
end;


{ ******************************************************************** }
{ Subtract matrix m from Self giving a new Self                        }
{                                                                      }
{ Usage:  A.subU (B);  subtract B from A giving A                      }
{                                                                      }
{ ******************************************************************** }
function TMatrix.sub (m : TMatrix) : TMatrix;
var i, j : integer;
begin
  if Not SameDimensions (m, Self) then
     raise EMatrixSizeError.Create ('Incorrecly sized result matrix for matrix subtraction');

  for i := 1 to r do
      for j := 1 to c do
          Self[i,j] := Self[i,j] - m[i,j];
  result := Self;
end;



{ ******************************************************************** }
{ Subtract m2 from m1 giving Self                                      }
{                                                                      }
{ Usage:  A.sub (A1, A2);  subtract A2 from A1 giving A (A = A2 - A1)  }
{                                                                      }
{ ******************************************************************** }
function TMatrix.sub (m1, m2 : TMatrix) : TMatrix;
var i, j : integer;
begin
  if Not SameDimensions (m1, m2) then
     raise EMatrixSizeError.Create ('Incompatible matrix operands to subtract');

  if Not SameDimensions (m1, Self) then
     raise EMatrixSizeError.Create ('Incorrectly sized result matrix for matrix subtraction');

  for i := 1 to r do
      for j := 1 to c do
          Self[i,j] := m1[i,j] - m2[i,j];
  result := Self;
end;


{ ******************************************************************** }
{ Multiply a matrix 'm' by scalar constant k and assign result to Self }
{                                                                      }
{ Usage: A.multk (B, 0.5);  multiply scalar, 0.5 by B giving A         }
{                                                                      }
{ ******************************************************************** }
function TMatrix.mult (m : TMatrix; k : TDMatElement) : TMatrix;
var i, j : integer;
begin
  for i := 1 to m.r do
      for j := 1 to m.c do
          Self[i, j] := m[i,j] * k;
  result := Self;
end;


{ ******************************************************************** }
{ Multiply the Self matrix by the scalar constant k                    }
{                                                                      }
{ Usage:  A.multKU (0.5);  multiply scalar 0.5 by A giving A           }
{                                                                      }
{ ******************************************************************** }
function TMatrix.mult (k : TDMatElement) : TMatrix;
var i, j : integer;
begin
  for i := 1 to r do
      for j := 1 to c do
          Self[i, j] := Self[i,j] * k;
  result := Self;
end;



{ ******************************************************************** }
{ Multiply matrix 'm1' by 'm2' to give result in Self                  }
{                                                                      }
{ Usage:  A.mult (A1, A2); multiply A1 by A2 giving A                  }
{                                                                      }
{ ******************************************************************** }
function TMatrix.mult (m1, m2 : TMatrix) : TMatrix;
var i, j, k, m1_Col : integer; sum : TDMatElement;
begin
  if m1.c = m2.r then
     begin
      Self.FreeSpace; Self.SetSize (m1.r, m2.c);
      m1_col := m1.c;
      for i := 1 to Self.r do
          for j := 1 to m2.c do
              begin
              sum := 0.0;
              for k := 1 to m1_Col do
                  sum := sum + m1[i, k]* m2[k, j];
              Self[i,j] := sum;
              end;
      result := Self;
    end
  else
     raise EMatrixSizeError.Create ('Incompatible matrix operands to multiply');
end;


{ ******************************************************************** }
{ Multiply vector 'v' by 'm' to give result in Self                    }
{                                                                      }
{ Usage:  A.mult (v1, A1); multiply v1 by A1 giving A                  }
{                                                                      }
{ ******************************************************************** }
function TMatrix.mult (v : TDVector; m : TMatrix) : TMatrix;
var i, j : integer; sum : Double;
begin
  if v.Size = m.r then
     begin
     Self.FreeSpace; Self.SetSize (1, m.c);
     for i := 1 to m.c do
         begin
         sum := 0.0;
         for j := 1 to v.Size do
             sum := sum + v[j]* m[i,j];
         Self[1,i] := sum;
         end;
     result := Self;
     end
  else
     raise EMatrixSizeError.Create ('Incompatible vector and matrix operands to multiply');
end;


{ ******************************************************************** }
{ Multiply matrix Self by factor k to give result in Self              }
{                                                                      }
{ Usage:  A.Scale (k); multiply A by k giving A                        }
{                                                                      }
{ ******************************************************************** }
procedure TMatrix.Scale  (k : Double);
var i, j : integer;
begin
  for i := 1 to Self.r do
      for j := 1 to Self.c do
          Self[i,j] := Self[i,j]*k;
end;


// -------------------------------------------------------------------------
// Applies the function f to all element of matrix Self
// Usage: m.ApplyFunction (MyFunction)
// -------------------------------------------------------------------------
function TMatrix.ApplyUnaryFunction (const f : TUnaryDoubleFunc) : TMatrix;
var i, j : integer;
begin
  for i := 1 to Self.r do
      for j := 1 to Self.c do
      Self[i,j] := f (Self[i, j]);
  Result := Self;
end;


// -------------------------------------------------------------------------
// Applies the function f to all element of matrix Self
// Usage: m.ApplyFunction (MyFunction)
// -------------------------------------------------------------------------
function TMatrix.ApplyBinaryFunction (f : TBinaryDoubleFunc; x, y : double) : TMatrix;
var i, j : integer;
begin
  for i := 1 to Self.r do
      for j := 1 to Self.c do
      Self[i,j] := f (x, y);
  Result := Self;
end;


{ ******************************************************************** }
{ LU Solve. Solve the linear system represented by m and right-hand    }
{ side b m is assumed have have been decomposed by LUDecomp            }
{                                                                      }
{ Usage: m.LUSolve (index, b)                                          }
{                                                                      }
{ ******************************************************************** }
procedure TMatrix.LUSolve (index : TIVector; b : TDVector);
var i, j, ii, ip, nRows : integer; sum : TDMatElement;
begin
   ii := 0;
   nRows := r;
   for i := 1 to nRows do
       begin
       ip := index[i];
       sum := b[ip];
       b[ip] := b[i];
       if ii <> 0 then
          for j := ii TO i-1 do sum := sum - Self[i,j]*b[j]
       else if sum <> 0.0 then ii := i;
       b[i] := sum;
       end;
    for i := nRows downto 1 do
        begin
        sum := b[i];
        if i < nRows then
           for j := i+1 to nRows do sum := sum - Self[i,j]*b[j];
       b[i] := sum/Self[i,i];
    end
end;


// Translated from the java lib jama. NOT TESTED
procedure TMatrix.LUDecomp2 (m : TMatrix; piv : TIVector);
var LU : TMatrix; LUrowi, LUcolj : TDVector;
    i, j, k, p, kmax, pivsign : integer; s, t : Double;
begin
  LU := TMatrix.Clone (m);
  LUrowi := TDVector.Create (m.r);
  LUcolj := TDVector.Create (m.r);
  for i := 1 to piv.Size do
      piv[i] := i;
  pivsign := 1;
  for j := 1 to m.c do
      begin
      for i := 1 to m.r do
          LUcolj[i] := LU[i,j];

          for i := 1 to m.r do
              begin
              LU.ExtractRow (LUrowi, i);
              kmax := min (i, j);
              s := 0;
              for k := 1 to kmax do
                  s := s + LUrowi[k]*LUcolj[k];
              LUcolj[i] := LUcolj[i] - s;
              LUrowi[j] := LUcolj[i];
              end;
          p := j;
          for i := j+1 to m.r do
              if System.abs (LUcolj[i]) > System.abs (LUcolj[p]) then
                  p := i;

          if p <> j then
             begin
             for k := 1 to m.c do
                 begin
                 t := LU[p,k];
                 LU[p,k] := LU[j,k];
                 LU[j,k] := t;
                 end;
             k := piv[p];
             piv[p] := piv[j];
             piv[j] := k;
             pivsign := - pivsign;
             end;

          if (j < m.r) and (LU[j,j] <> 0.0) then
             for i := j+1 to m.r do
                 LU[i,j] := LU[i,j] / LU[j,j];
      end;
end;


{ ******************************************************************** }
{ Form LU decomposition of Self matrix. Result goes into m             }
{                                                                      }
{ Usage: mat.LUDecomp(m, index);                                       }
{                                                                      }
{ ******************************************************************** }
procedure TMatrix.LUDecomp3 (m : TMatrix; index : TIVector);
var v : TDVector; i, k, j, imax, n, tmpi, kprime : integer; sum, big, tmp, p : TDMatElement;
tmpName : string;
begin
  if Self.r = m.c then
     begin
     m.Copy (Self);
     n := m.r;
     for i := 1 to n do Index[i] := i;
     for k := 1 to n - 1 do
         begin
         p := 0;
         for i := k to n do
             if System.abs (m[i,k]) > p then
                begin
                p := System.abs (m[i,k]);
                kprime := i;
                end;
         if p = 0 then
            raise ESingularMatrix.Create ('Singular matrix in LU Decomposition, found row of zeros');
         tmpi := Index [k];
         Index[k] := Index[kprime];
         Index[kprime] := tmpi;
         m.ExchangeRows (k, kprime);
         for i := k + 1 to n do
             begin
             m[i,k] := m[i,k]/m[k,k];
             for j := k + 1 to n do
                 m[i,j] := m[i,j] - m[i,k]*m[k,j];
             end;
         end;
     end
  else
     raise ENonSquareMatrix.Create ('LUDecomp: Matrix must be square');
end;

{ ******************************************************************** }
{ Form LU decomposition of Self matrix. Result goes into m             }
{                                                                      }
{ Usage: mat.LUDecomp(m, index);                                       }
{                                                                      }
{ ******************************************************************** }
procedure TMatrix.LUDecomp (m : TMatrix; index : TIVector);
var v : TDVector; i, k, j, imax, nRows : integer; sum, big, tmp : TDMatElement;
tmpName : string;
begin
  if Self.r = m.c then
     begin
     m.Copy (Self);
     v := TDVector.Create (m.r);
     try
     { Find the largest element in every row, and store its reciprocal in v[i] }
     nRows := m.r;
     for i := 1 to nRows do
         begin
         big := 0.0; { needed to test for singularity }
         { Although we're working across columns we can use nRows since m1 is square }
         for j := 1 to nRows do if (System.abs(m[i,j]) > big) then big := System.abs(m[i,j]);
         if big = 0.0 then raise ESingularMatrix.Create ('Singular matrix in LU Decomposition, found row of zeros');
         v[i] := 1.0/big
         end;

     for j := 1 TO nRows do
         begin
         // Form beta = aij - sum_{k=1}^{i-1} a[ik] * a[kj]
         for i := 1 To j-1 do
             begin
             sum := m[i,j];
             for k := 1 to i-1 do sum := sum - m[i,k]*m[k,j];
             m[i,j] := sum
             end;
         big := 0.0;
         for i := j to nRows do
             begin
             sum := m[i,j];
             for k := 1 to j-1 do sum := sum - m[i,k]*m[k,j];
             m[i,j] := sum;
             if v[i]*System.abs(sum) >= big then
                begin
                big := v[i]*System.abs(sum);
                imax := i
                end
             end;

         { Interchange rows if necessary }
         if j <> imax then
            begin
            { Swap row names aswell }
            tmpName := m.rName[imax]; m.rName[imax] := m.rName[j];
            m.rName[j] := tmpName;
            for k := 1 to nRows do
                begin
                tmp := m[imax,k];
                m[imax,k] := m[j,k];
                m[j,k] := tmp
                end;
             v[imax] := v[j]
             end;
          index[j] := imax;
          { Get ready to divide by pivot element }
          if System.abs (m[j,j]) < 1e-15 then
             raise ESingularMatrix.Create ('Singular Matrix found during LU decomposition, pivot value is zero');
          if j <> nRows then
             begin
             tmp := 1.0/m[j,j];
             for i := j+1 to nRows do m[i,j] := m[i,j]*tmp
             end
         end;
     finally
       v.destroy;
     end;
     end
  else
     raise ENonSquareMatrix.Create ('LUDecomp: Matrix must be square');
end;


{ ******************************************************************** }
{ Find determinant of matrix                                           }
{                                                                      }
{ Usage: d := m.det                                                    }
{                                                                      }
{ ******************************************************************** }
function TMatrix.Det : double;
var m : TMatrix; index : TIVector; i : integer;
begin
  result := 1;
  if r = c then
     begin
     index := TIVector.Create (r);
     m := TMatrix.Create (r,r);
     try
       m.copy (Self);
       Self.LUDecomp (m, index);
       for i := 1 to r do result := result * m[i,i];
     finally
       m.free; index.free;
     end;
     end
  else
     raise ENonSquareMatrix.Create ('Determinant: Matrix must be square');
end;


{ ******************************************************************** }
{ Solve a linear system of equations: Self.v = b, i.e solve for v      }
{                                                                      }
{ Usage: A.SolveLinear (v, b, t);                                      }
{        Solution in v                                                 }
{ If the boolean t is true then self is replaced by the inverse        }
{ ******************************************************************** }
procedure TMatrix.SolveLinear (v, b : TDVector; SelfToInv : boolean);
var n, i, j : integer;
    indx : TIVector; col : TDVector;
    dest, src : TMatrix;
begin
  if Self.r = Self.c then
     begin
     n := Self.r;
     { Make a copy and work on the copy }
     dest := TMatrix.Create (n, n);
     src  := TMatrix.Create (n, n);
     indx := TIVector.Create (n);
     try
       src.Copy (Self);
       for i := 1 to n do v[i] := b[i];
       src.LUDecomp (dest, indx);
       dest.LUSolve (indx, v);
       if SelfToInv then
          begin
          col := TDVector.Create (n);
          try
            for j := 1 to n do
                begin
                for i := 1 to n do col[i] := 0.0;
                col[j] := 1.0;
                dest.LUSolve (indx, col);
                for i := 1 to n do Self[i,j] := col[i];
                end;
          finally
            col.free;
          end;
          end;
     finally
     indx.destroy; dest.destroy; src.destroy;
     end;
     end
  else
     raise ENonSquareMatrix.Create ('SolveLinear: Matrix must be square');
end;



{ ******************************************************************** }
{ Fast method for inverting a matrix (Self)                            }
{ Result in inv                                                        }
{                                                                      }
{ Usage:  A.Invert (inv);                                              }
{ ******************************************************************** }
procedure TMatrix.Invert (inv : TMatrix);
var col : TDVector; n, i, j : integer;
    dest, src : TMatrix; indx : TIVector;
begin
  n := Self.r;
  col := TDVector.Create (n);
  dest := TMatrix.Create (n, n);
  src  := TMatrix.Create (n, n);
  indx := TIVector.Create (n);
  try
    src.Copy (Self);
    try
      src.LUDecomp (dest, indx);
    except
      on ESingularMatrix do
         raise ESingularMatrix.Create ('Invert: Singular Matrix');
    end;
    for j := 1 to n do
        begin
        for i := 1 to n do col[i] := 0.0;
        col[j] := 1.0;
        dest.LUSolve (indx, col);
        for i := 1 to n do inv[i,j] := col[i];
        end;
  finally
    col.destroy; dest.destroy; src.destroy; indx.destroy;
  end;
end;


{ ******************************************************************** }
{ Fast method for inverting a matrix (Self)                            }
{ Result in Self                                                       }
{                                                                      }
{ Usage:  A.Invert                                                     }
{ ******************************************************************** }
procedure TMatrix.Invert;
var col : TDVector; n, i, j : integer;
    dest, src : TMatrix; index : TIVector;
begin
  n := Self.r;
  col   := TDVector.Create (n);
  dest  := TMatrix.Create (n, n);
  src   := TMatrix.Create (n, n);
  index := TIVector.Create (n);
  try
    src.Copy (Self);
    try
      src.LUDecomp (dest, index);
    except
      on ESingularMatrix do
         raise ESingularMatrix.Create ('Invert: Singular Matrix');
    end;
    for j := 1 to n do
        begin
        for i := 1 to n do col[i] := 0.0;
        col[j] := 1.0;
        dest.LUSolve (index, col);
        for i := 1 to n do Self[i,j] := col[i];
        end;
  finally
    col.destroy; dest.destroy; src.destroy; index.destroy;
  end;
end;



{ Internal routine that sets any values less than tol to 0.0 }
procedure CleanUpMatrix (m : TMatrix);
var i, j, ri, ci : integer;
begin
  { Removes all numbers close to zero, i.e between -tol and +tol }
  ri := m.r; ci := m.c;
  for i := 1 to ri do
      for j := 1 to ci do
          if m.IsZero (i, j) then m [i, j] := 0.0;
end;


{ Internal routine to work out the rank of a matrix given the reduced row-echelon }
function TMatrix.ComputeRank (m : TMatrix) : integer;
var i, j, ri, ci, rank : integer;
begin
  ri := m.r; ci := m.c;
  { find the rank - brute force algorithm }
  rank := 0;
  { search row by row  for zero rows }
  for i := 1 to ri do
      begin
      { search along the row looking for nonzero entry }
      for j := 1 to ci do
          if System.abs (m [i, j]) > InternalTolerance then
             begin
             inc (rank);
             break;
             end;

      end;
 result := rank;
end;


{ ******************************************************************** }
{ Routine to exchange two rows, r1 and r2 in matrix Self               }
{                                                                      }
{ Usage:  A.exchangeRows (1, 2);                                       }
{                                                                      }
{ ******************************************************************** }
function TMatrix.ExchangeRows (r1, r2 : integer) : TMatrix;
var ci, i : integer; t : double; name : string;
begin
  if (r1 > 0) and (r1 <= Self.r) and (r2 > 0) and (r2 <= Self.r) then
     begin
     ci := Self.c;
     for i := 1 to ci do
         begin
         t := Self[r1, i];
         Self[r1, i] := Self[r2, i];
         Self[r2, i] := t;
         end;
     name := Self.rName[r1];
     Self.rName[r1] := Self.rName[r2];
     Self.rName[r2] := name;
     result := Self;
     end
  else
     raise EMatrixSizeError.Create ('Rows not in range for exchange');
end;



{ ******************************************************************** }
{ Routine to exchange two columns, c1 and c2 in matrix Self            }
{                                                                      }
{ Usage:  A.exchangeCols (1, 2);                                       }
{                                                                      }
{ ******************************************************************** }
function TMatrix.ExchangeCols (c1, c2 : integer) : TMatrix;
var ri, i : integer; t : double; name : string;
begin
  if (c1 > 0) and (c1 <= Self.c) and (c2 > 0) and (c2 <= Self.c) then
     begin
     ri := Self.r;
     for i := 1 to ri do
         begin
         t := Self[i, c1];
         Self[i, c1] := Self[i, c2];
         Self[i, c2] := t;
         end;
     name := Self.cName[c1];
     Self.cName[c1] := Self.cName[c2];
     Self.cName[c2] := name;
     result := Self;
     end
  else
     raise EMatrixSizeError.Create ('Columns not in range for exchange [c1 = '
             + inttostr (c1) + '; c2 = ' + inttostr (c2) + ']');
end;



{ ******************************************************************** }
{ Find the rank r, of the matrix Self, The reduced Row                 }
{ echelon is returned in mat. tol is the magnitude of                  }
{ the largest number before it is assumed to be zero.                  }
{                                                                      }
{ Usage:  r := A.Rank (echelon, 1e-8)                                  }
{         Find the rank of A, place echelon in echelon                 }
{                                                                      }
{ ******************************************************************** }
function TMatrix.Rank (echelon : TMatrix; tol : double) : integer;
var Arow, Acol, i, j, n, m, RowScan : integer;
    factor : double;
begin
  n := Self.r; m := Self.c;
  if (n = 0) or (m = 0) then
     begin
     Result := 0;
     Exit;
     end;

  echelon.copy (Self);    // Work on Echelon, not Self, so make a copy of Self
  if Tol <> 0.0 then echelon.Tolerance := Tol;

  if (Tol <> 0.0) then
     InternalTolerance := Tol;

  Arow := 1; Acol := 1;
  repeat
    // locate a nonzero column
    if echelon.IsZero (Arow, Acol) then
       begin
       { If the entry is zero work our way down the matrix
       looking for a nonzero entry, when found, swap it for Arow }
       RowScan := Arow;
       repeat
         // next row
         inc (RowScan);
         // Have we reached the end of the rows but we've still got columns left to scan?
         if (RowScan > n) and (Acol < m) then
           begin
           { reset row counter back to where it was and try next column }
           RowScan := Arow; inc (Acol);
           end;

         { If we've scanned the whole matrix, then lets get out... }
         if (RowScan > n) then
            begin
            CleanUpMatrix (echelon);
            result := ComputeRank (echelon);
            exit;
            end;
       until echelon.IsNotZero (RowScan, Acol);   { keep searching until non-zero entry found }

       { We've found a nonzero row entry so swap it with
       'Arow' which did have a zero as its entry }
       echelon.exchangeRows (Arow, RowScan);
       end;

    { Arow now holds the row of interest }
    factor := 1.0/echelon [Arow, Acol];
    { reduce all the entries along the column by the factor }
    for i := Acol to m do echelon[Arow,i] := echelon[Arow, i] * factor;

    { now eliminate all entries above and below Arow, this generates the reduced form }
    for i := 1 to n do
        { miss out Arow itself }
        if (i <> Arow) and echelon.IsNotZero (i, Acol) then
           begin
           factor := echelon [i, Acol];
           { work your way along the column doing the same operation }
           for j := Acol to m do
               begin
               echelon[i,j] := echelon [i, j] - factor * echelon [Arow, j];
               end;
           end;

    inc (Arow); inc (Acol);
 until (Arow > n) or (Acol > m);
 CleanUpMatrix (echelon);
 result := ComputeRank (echelon);   { This is just a patch for the moment }
end;




{ ******************************************************************** }
{               Find the null space of a matrix                        }
{ ******************************************************************** }
{                           Algorithm

    1. Reduce matrix to reduced echelon form
    2. There will be as many null space vectors as there are
       non-leading columns. Select one of these non-leading columns.
    3. Select the ith non-leading column and place a 1 at the ith
       position in the growing null space vector
    4. Consider the remaining non-leading columns, say j,k,l...
       and place zero's at positions j,k,l... in the growing null
       vector.
    5. Consider now the column positions of the leading columns, say
       l,m,n... The equivalent entries in the growing null space
       are what remains to be filled in. Select each of these leading
       columns in turn, say the lth first. Record which row the
       leading one is in, say r. Then place at position l in the
       growing null space vector, the element -1 * element (r, i)
       where i is the original ith non-leading column selected in
       step 3. Continue for leading columns m,n... until the growing
       null space vector is complete.
    6. Go back to step 2 and pick another non-leading column to
       compute the next null space vector.

Does not disturb the matrix Self. Null space to be found in NullVectors, size of
the basis in BasisSize, the reduced row-echelon in Echelon and the rank in TheRank }

{ Usage:   A.NullSpace (N, b, Echelon, r);
{                                                                      }
{ ******************************************************************** }
procedure TMatrix.NullSpace_2 (var NullVectors : TMatrix; var BasisSize : integer;
                     var Echelon : TMatrix; var TheRank : integer; tol : double);
const
    tLeadingColumn    = -999;
    tNonLeadingColumn = -888;
    tFreeEntryMarker  = -123456789;

var x : double;
    i, j : integer;
    mask    : TIVector;
    VectorCounter, maskcount : integer;
    EchelonCols : integer;
begin
  try
  if NullVectors <> Nil then NullVectors.Free;
  if Echelon <> Nil then Echelon.free;

  Echelon     := TMatrix.Create (Self.r, Self.c);
  Echelon.Name := 'Echelon';
  EchelonCols := Echelon.c;
  mask := TIVector.create (EchelonCols);
  mask.Name := 'mask';

  // STEP 1: Subtract the rank of Self from the number of columns to get the dimension of the basis
  TheRank := Self.Rank (Echelon, tol);
  BasisSize := Self.c - TheRank;
  NullVectors := TMatrix.Create (Self.c, BasisSize);
  NullVectors.Name := 'Null Vectors';

  if BasisSize > 0 then
     begin
     for i := 1 to EchelonCols do mask [i] := tNonLeadingColumn;

     for i := 1 to Echelon.r do
         begin
         // For row i, scan along the row looking for a leading one
         j := 1;  // Column counter
         repeat
           x := Echelon[i, j];
           if (x > 1.0-tol) and (x < 1.0+tol) then  { x is then = 1.0 }
              begin
              mask [j] := tLeadingColumn;  // Mark the existence of a leading column
              break;      // Get out of repeat loop
              end
           else
              j := j + 1;   // Next column

         until j > EchelonCols;  // Until we've done all columns along row i

         end;  // next row if one available

     { Find non-leading columns }
     VectorCounter := 1;
     i := 1;  { i = column counter, check all columns }
     repeat
       for j := 1 to EchelonCols do NullVectors[j, VectorCounter] := tFreeEntryMarker;

       { STEP 5 }
       { remember, all tNonLeadingColumn's in mask = non-leading columns }
       if mask [i] = tNonLeadingColumn then  { found a non-leading column }
          begin
          j := 1;
          { move down mask }
          for maskcount := 1 to EchelonCols do
               if (mask [maskcount] = tLeadingColumn) then
                  begin
                  NullVectors[maskcount, VectorCounter] := -Echelon[j, i];
                  inc (j);
                  end;

          { STEP 4 }
          { zero all free entries }
          for j := 1 to EchelonCols do
              if NullVectors[j, VectorCounter] = tFreeEntrymarker then
                 NullVectors[j, VectorCounter] := 0.0;

          { STEP 2 AND 3 }
          { mark free variable }
          NullVectors[i, VectorCounter] := 1.0;
          VectorCounter := VectorCounter + 1;
          if VectorCounter > BasisSize then
             begin
             ShowMessage ('Error in VectorCouter');
             break;
             end;
          end;
       inc (i);
     until i > EchelonCols;
     end
  else
     begin
     BasisSize := 0;
     NullVectors := Nil;
     end;
  finally
     mask.free;
  end;
end;


{ ******************************************************************** }
// Alternative algorthim to computing the null space
// Take Transpose of A and form the adjoint matrix: [Tr[A] | Id]
// where Id is the identity matrix. Row reduce this matrix to a matrix
// of the form [M | D]. Partition D into D=Tr[D11 D12] where D12 corresponds
// to the zero rows of M. The columns of Tr[D12] form the vectors
// of the null space of A.
// See: http://www.ma.iup.edu/projects/CalcDEMma/linalg2
{ ******************************************************************** }

procedure TMatrix.NullSpace (var NullVectors : TMatrix; var BasisSize : integer; Tol : double);
var m, mt, Id, Echelon : TMatrix; nRank, Count, i, j, k : integer;
begin
  Echelon := TMatrix.Create (Self.r, Self.c);
  try
    nRank := Self.Rank (Echelon, Tol);
    BasisSize := Self.c - nRank;

    if NullVectors <> Nil then NullVectors.Free;
    NullVectors := TMatrix.Create (Self.c, BasisSize);
    NullVectors.Name := 'Null Vectors';
    for i := 1 to NullVectors.r do NullVectors.rName[i] := Echelon.cName[i];
  finally
    Echelon.Free;
  end;

  // Form an adjoining identity matrix to the transpose of Self
  mt := TMatrix.Create (Self.c, Self.r);
  mt.Transpose (Self);

  // Prepare the space for the Echelon matrix we generate from mt
  Echelon := TMatrix.Create (mt.r, mt.c + mt.r);
  Echelon.Name := 'Echelon';

  try
    // Construct: m = [mt | Id (mt.r)]
    m := TMatrix.Create (mt.r, mt.c + mt.r);
    for i := 1 to mt.c do m.cName[i] := mt.cName[i];
    for i := 1 to mt.r do
        begin
        m.rName[i] := mt.rName[i];
        for j := 1 to mt.c do
            m[i,j] := mt[i,j];
        end;

    Id := TMatrix.Create (mt.r);
    for i := 1 to m.r do
        for j := mt.c + 1 to mt.c + mt.r do
            m[i,j] := Id[i, j-mt.c];

    // Row reduce m to form [M | D]
    nRank := m.Rank (Echelon, Tol);
    //Echelon.Save ('res_ech');

    // Look for zero rows in the M partition, the corresponding rows in D
    // will equal vectors of the null space
    Count := 1;  // Counts the number of null space vectors
    for i := m.r downto 1 do       // Start at the bottom row and work up to the top row
        begin
        for j := 1 to mt.c do  // Work across the M partition
            if not Echelon.IsZero (i,j) then
               Exit; // There won't be any more vectors to find
        // A zero row was found so copy over the adjacent row to NullVector
        for k := mt.c + 1 to mt.c + mt.r do
            begin
            NullVectors[k-mt.c, Count] := Echelon[i, k];
            end;
        Inc (Count); // Next vector
        end;
  finally
    Echelon.Free;
    mt.Free;
    Id.Free;
    m.Free;
  end;
end;


function sign (a, b : TDMatElement) : TDMatElement;
begin

  if b >= 0.0 then
     result := System.abs (a)
  else
     result := -System.abs(a);
end;


function max (a, b : TDMatElement) : TDMatElement;
begin
  if a > b then
     result := a
  else
     result := b;
end;


{ Compute sqrt (a^2 + b^2) using numerically more stable method. If x = sqrt(a^2 + b^2),
then, x/a^2 = 1/a^2 sqrt (a^2 + b^2), mult both sides by sqrt(..), so
x/a^2 * sqrt (a^2 + b^2) = 1/a^2 (a^2 + b^2) or
x/a^2 * sqrt (a^2 + b^2) = 1 + (b/a)^2 but on left side 1/a^2 sqrt(a^2 + b^2) equals
x/a^2, therefore x * x/a^2 = 1 + (b/a) ^2, take square roots on both side yields:
x/a := sqrt (1+(b/a)^2), or FINALLY: x := a sqrt (1 + (b/a)^2) }

function pythag (a, b : TDMatElement) : TDMatElement;
var at, bt, ct : TDMatElement;
begin
  result := sqrt (a*a + b*b);
  exit;
  at := System.abs (a); bt := System.abs (b);
  if at > bt then
     begin
     ct := bt/at;
     result := at*sqrt (1 + ct*ct);
     end
  else
     begin
     if bt > 0 then
        begin
        ct := at/bt;
        result := bt*sqrt (1 + ct*ct);
        end
     else
        result := 0.0;
     end;
end;

   function MyAbs (x : TDMatElement) : TDMatElement;
   begin
     if x < 0.0 then x := -x;
     result := x;
   end;


{procedure TMatrix.svd2 (var u : TMatrix; var w : TDVector; var v : TMatrix);}
procedure TMatrix.svd2 (var u : TMatrix; var w : TDVector; var v : TMatrix);
LABEL 1,2,3;
CONST
   nmax=100;
VAR
   n, m, nm, l, k, j, jj, its, i : integer;
   z, y, x, scale, s, h, g, f, cc, anorm : real;
   rv1 : TDVector; Aug : TMatrix;
   AugMatrix : boolean;

   function sign(a,b: TDMatElement): TDMatElement;
   begin
      if (b >= 0.0) then sign := System.abs(a) else sign := -System.abs(a)
   end;

   function max(a,b: TDMatElement): TDMatElement;
   begin
      if (a > b) then max := a else max := b
   end;

begin
  m := r; n := c; AugMatrix := false;
  {if m < n then
     begin
     { More parameters than data ! Change structure of Self by augmenting
     Self with additional rows (entries set to zero) so that m = n, don't change m or n though }
     {Aug := TMatrix.Create (n, n); Aug.zero;
     try
       for i := 1 to m do
           for j := 1 to n do
               Aug[i,j] := Self[i,j];
       u.FreeSpace; u.SetSize (n, n); u.Copy (Aug);
       AugMatrix := true;
     finally
       Aug.free;
     end;
     end
  else}
     u.Copy(Self); { Work on U, don't destroy Self }


  if AugMatrix then
     rv1 := TDVector.Create (n)  { Make enough room }
  else
     rv1 := TDVector.Create (m); { Save some space }
   g := 0.0;
   scale := 0.0;
   anorm := 0.0;
   FOR i := 1 TO n DO BEGIN
      l := i+1;
      rv1[i] := scale*g;
      g := 0.0;
      s := 0.0;
      scale := 0.0;
      IF (i <= m) THEN BEGIN
         FOR k := i TO m DO scale := scale + Myabs(u[k,i]);
         IF (Myabs(scale) > 1e-12) THEN BEGIN
         {IF (scale <> 0.0) THEN BEGIN}
            for k := i to m do
                begin
                u[k,i] := u[k,i]/scale;
                s := s + u[k,i]*u[k,i]
                end;
            f := u[i,i];
            g := -sign(sqrt(s),f);
            h := f*g-s;
            u[i,i] := f-g;
            if (i <> n) then
               begin
               for j := l to n do
                   begin
                   s := 0.0;
                   for k := i to m do s := s + u[k,i]*u[k,j];
                   f := s/h;
                   for k := i to m do u[k,j] := u[k,j] + f*u[k,i];
                   end
               end;
            for k := i to m do u[k,i] := scale*u[k,i]
         END
      END;
      w[i] := scale*g;
      g := 0.0;
      s := 0.0;
      scale := 0.0;
      IF ((i <= m) AND (i <> n)) THEN BEGIN
         for k := l to n do scale := scale + Myabs(u[i,k]);
         if (Myabs(scale) > 1e-12) then begin
         {if (scale <> 0.0) then begin}
            for k := l to n do
                begin
                u[i,k] := u[i,k]/scale;
                s := s + u[i,k]*u[i,k]
                end;
            f := u[i,l];
            g := -sign(sqrt(s),f);
            h := f*g-s;
            u[i,l] := f-g;
            for k := l to n do rv1[k] := u[i,k]/h;
            if (i <> m) then
               begin
               for j := l to m do
                   begin
                   s := 0.0;
                   for k := l to n do s := s + u[j,k]*u[i,k];
                   for k := l to n do u[j,k] := u[j,k] + s*rv1[k];
                   end
               end;
            for k := l to n do u[i,k] := scale*u[i,k];
         END
      END;
      anorm := max(anorm,(Myabs(w[i]) + Myabs(rv1[i])))
   END;

   FOR i := n DOWNTO 1 DO BEGIN
      IF (i < n) THEN BEGIN
         if (Myabs(g) > 1e-12) then
         {IF (g <> 0.0) THEN}
            begin
            for j := l to n do v[j,i] := (u[i,j]/u[i,l])/g;
            for j := l to n do
                begin
                s := 0.0;
                for k := l to n do s := s + u[i,k]*v[k,j];
                for k := l to n do v[k,j] := v[k,j] + s*v[k,i]
                end
            end;
         for j := l to n do
             begin
             v[i,j] := 0.0;
             v[j,i] := 0.0;
             end
      END;
      v[i,i] := 1.0;
      g := rv1[i];
      l := i
   end;
   FOR i := n DOWNTO 1 DO BEGIN
      l := i+1;
      g := w[i];
      if (i < n) then for j := l to n do u[i,j] := 0.0;
      if (Myabs(g) > 1e-12) then
      {IF (g <> 0.0) THEN}
         begin
         g := 1.0/g;
         IF (i <> n) THEN
            begin
            for j := l to n do
                begin
                s := 0.0;
                for k := l to m do s := s + u[k,i]*u[k,j];
                f := (s/u[i,i])*g;
                for k := i to m do u[k,j] := u[k,j] + f*u[k,i];
                end
            end;
         for j := i to m do u[j,i] := u[j,i]*g;
      end else
         begin
         for j := i to m do u[j,i] := 0.0;
         end;
      u[i,i] := u[i,i]+1.0
   END;
   FOR k := n DOWNTO 1 DO BEGIN
      FOR its := 1 TO 30 DO BEGIN
         for l := k downto 1 do
             begin
             nm := l-1;
             if ((Myabs(rv1[l]) + anorm) - anorm < 1e-12) then goto 2;
             {if ((Myabs(rv1[l]) + anorm) = anorm) then goto 2;}
             if ((Myabs(w[nm]) + anorm) - anorm < 1e-12) then goto 1
             {if ((Myabs(w[nm]) + anorm) = anorm) then goto 1}
             end;
1:         cc := 0.0;
         s := 1.0;
         for i := l to k do
             begin
             f := s*rv1[i];
             if ((Myabs(f) + anorm) - anorm > 1e-12) then
             {if ((Myabs(f)+anorm) <> anorm) then}
                begin
                g := w[i];
                h := sqrt(f*f+g*g);
                w[i] := h;
                h := 1.0/h;
                cc := (g*h);
                s := -(f*h);
                for j := 1 to m do
                    begin
                    y := u[j,nm];
                    z := u[j,i];
                    u[j,nm] := (y*cc)+(z*s);
                    u[j,i] := -(y*s)+(z*cc)
                    end
                end
             end;
2:       z := w[k];
         if (l = k) then
             begin
             if (z < 0.0) then
               begin
               w[k] := -z;
               for j := 1 to n do v[j,k] := -v[j,k];
               end;
            GOTO 3
            end;
         if (its = 30) then writeln ('no convergence in 30 SVDCMP iterations');
         x := w[l];
         nm := k-1;
         y := w[nm];
         g := rv1[nm];
         h := rv1[k];
         f := ((y-z)*(y+z)+(g-h)*(g+h))/(2.0*h*y);
         g := sqrt(f*f+1.0);
         f := ((x-z)*(x+z)+h*((y/(f+sign(g,f)))-h))/x;
         cc := 1.0;
         s := 1.0;
         for j := l to nm do
             begin
             i := j+1;
             g := rv1[i];
             y := w[i];
             h := s*g;
             g := cc*g;
             z := sqrt(f*f+h*h);
             rv1[j] := z;
             cc := f/z;
             s := h/z;
             f := (x*cc)+(g*s);
             g := -(x*s)+(g*cc);
             h := y*s;
             y := y*cc;
             for jj := 1 to n do
                 begin
                 x := v[jj,j];
                 z := v[jj,i];
                 v[jj,j] := (x*cc)+(z*s);
                 v[jj,i] := -(x*s)+(z*cc)
                 end;
             z := sqrt(f*f+h*h);
             w[j] := z;
             if (Myabs(z) > 1e-12) then
             {if (z <> 0.0) then}
                begin
                z := 1.0/z;
                cc := f*z;
                s := h*z
                end;
             f := (cc*g)+(s*y);
             x := -(s*g)+(cc*y);
             for jj := 1 to m do
                 begin
                 y := u[jj,j];
                 z := u[jj,i];
                 u[jj,j] := (y*cc)+(z*s);
                 u[jj,i] := -(y*s)+(z*cc)
                 end
             end;
         rv1[l] := 0.0;
         rv1[k] := f;
         w[k] := x
      END;
3:   END


END;


{ Perform a Singular Value Decompostion on self, returning u, w, and v, modified
from Numerical Recipes and Forsythe et al 1977, Computer methods for Math Calc }
procedure TMatrix.svd (var u : TMatrix; var w : TDVector; var v : TMatrix);
label 3;
var i, j, k, l, n, m, its, flag, nm, jj : integer; rv1 : TDVector;
    scale, g, h, f, anorm, s, cc, x, y, z : TDMatElement; Aug : TMatrix;
    AugMatrix : boolean;
begin
  m := r; n := c; AugMatrix := false;
  if m < n then
     begin
     { More parameters than data ! Change structure of Self by augmenting
     Self with additional rows (entries set to zero) so that m = n, don't change m or n though }
     Aug := TMatrix.Create (n, n); Aug.zero;
     try
       for i := 1 to m do
           for j := 1 to n do
               Aug[i,j] := Self[i,j];
       u.FreeSpace; u.SetSize (n, n); u.Copy (Aug);
       AugMatrix := true;
     finally
       Aug.free;
     end;
     end
  else
     u.Copy(Self); { Work on U, don't destroy Self }

  scale := 0.0; g := 0.0; anorm := 0.0;
  if AugMatrix then
     rv1 := TDVector.Create (n)  { Make enough room }
  else
     rv1 := TDVector.Create (m); { Save some space }

  try
  for i := 1 to n do
      begin
      l := i + 1;
      rv1[i] := scale * g;
      g := 0.0; s := 0.0; scale := 0.0;
      if i <= m then
         begin
         for k := i to m do scale := scale + abs (u[k,i]);
         if scale <> 0.0 then
            begin
            for k := i to m do
                begin
                u[k, i] := u[k, i] / scale;
                s := s + u[k,i]*u[k,i];
                end;
                f := u[i,i];
                g := -sign (sqrt (s), f);
                h := f*g - s;
                u[i,i] := f - g;
                if i <> n then
                   begin
                   for j := l to n do
                       begin
                       s := 0.0;
                       for k := i to m do s := s + u[k,i]*u[k,j];
                       f := s/h;
                       for k := i to m do u[k,j] := u[k,j] + f*u[k,i];
                       end;
                   end;
            for k := i to m do u[k,i] := u[k,i] * scale;
            end;
         end;
      w[i] := scale * g;
      g := 0.0; s := 0.0; scale := 0.0;
      if (i <= m) and (i <> n) then
         begin
         for k := l to n do scale := scale + System.abs (u[i,k]);
         if scale <> 0.0 then
            begin
            for k := l to n do
                begin
                u[i,k] := u[i,k] / scale;
                s := s + u[i,k]*u[i,k];
                end;
            f := u[i,l];
            g := -sign(sqrt (s), f);
            h := f*g - s;
            u[i,l] := f - g;
            for k := l to n do rv1[k] := u[i,k]/h;
            if i <> m then
               begin
               for j := l to m do
                   begin
                   s := 0.0;
                   for k := l to n do s := s + u[j,k]*u[i,k];
                   for k := l to n do u[j,k] := u[j,k] + s*rv1[k];
                   end;
               end;
            for k := l to n do u[i,k] := u[i,k] * scale;
            end;
         end;
      anorm := max (anorm, System.abs(w[i]) + System.abs(rv1[i]));
      end;

  { ------------------------------------------ }
  { Accumulation of right-hand transformations }
  for i := n downto 1 do
      begin
      if i < n then
         begin
         if g <> 0.0 then
            begin
            for j := l to n do v[j,i] := (u[i,j]/u[i,l])/g;
            for j := l to n do
                begin
                s := 0.0;
                for k := l to n do s := s + u[i,k]*v[k,j];
                for k := l to n do v[k,j] := v[k,j] + s*v[k,i];
                end;
            end;
         for j := l to n do begin v[i,j] := 0.0; v[j,i] := 0.0; end;
         end;
      v[i,i] := 1.0;
      g := rv1[i];
      l := i;
      end;

  { ------------------------------------------ }
  { Accumulation of left-hand transformations  }
  for i := n downto 1 do
      begin
      l := i + 1;
      g := w[i];
      if i < n then  for j := l to n do u[i,j] := 0.0;
         if g <> 0.0 then
            begin
            g := 1.0/g;
            if i <> n then
               begin
               for j := l to n do
                   begin
                   s := 0.0;
                   for k := l to m do s := s + u[k,i]*u[k,j];
                   f := (s/u[i,i])*g;
                   for k := i to m do u[k,j] := u[k,j] + f*u[k,i];
                   end;
               end;
            for j := i to m do u[j,i] := u[j,i] * g;
            end
         else
            begin
            for j := i to m do u[j,i] := 0.0;
            end;
      u[i,i] := u[i,i] + 1.0;
      end;

  { --------------------------------------------- }
  { Diagonalization of the bidiagonal form        }
  for k := n downto 1 do
      begin
      for its := 1 to 30 do
          begin
          flag := 1;
          for l := k downto 1 do
              begin
              nm := l - 1;
              if System.abs (rv1[l] + anorm) = anorm then
                 begin
                 flag := 0;
                 break;
                 end;
              if System.abs (w[nm] + anorm) = anorm then break;
              end;
          if flag <> 0 then
             begin
             cc := 0.0; s := 1.0;
             for i := l to k do
                 begin
                 f := s * rv1[i];
                 if (System.abs (f) + anorm) <> anorm then
                    begin
                    g := w[i];
                    h := pythag (f, g);
                    w[i] := h;
                    h := 1.0/h;
                    cc := g*h;
                    s := -f*h;
                    for j := 1 to m do
                        begin
                        y := u[j,nm];
                        z := u[j, i];
                        u[j,nm] := y*cc + z*s;
                        u[j,i]  := z*cc - y*s;
                        end;
                    end;
                 end;
             end;
          z := w[k];
          if l = k then
             begin
             if z < 0.0 then
                begin
                w[k] := -z;
                for j := 1 to n do v[j,k] := -v[j,k];
                end;
             {break;} goto 3;
             end;
      if (its = 30) then raise Exception.Create ('Exceeded iterations in SVD routine');
      x := w[l];
      nm := k - 1;
      y := w[nm]; g := rv1[nm];
      h := rv1[k];
      f := ((y - z)*(y + z) + (g - h)*(g + h))/(2.0*h*y);
      g := pythag (f, 1.0);
      f := ((x - z) * (x + z) + h*((y/(f + sign(g, f))) - h))/x;

      cc := 1.0; s := 1.0;
      for j := l to nm do
          begin
          i := j + 1;
          g := rv1[i];
          y := w[i]; h := s*g;
          g := cc*g;
          z := pythag (f, h);
          rv1[j] := z;
          cc := f/z; s := h/z;
          f := x*cc + g*s; g := g*cc - x*s;
          h := y*s;
          y := y*cc;
          for jj := 1 to n do
              begin
              x := v[jj,j]; z := v[jj,i];
              v[jj,j] := x*cc + z*s;
              v[jj,i] := z*cc - x*s;
              end;
          z := pythag (f, h);
          w[j] := z;
          if z <> 0 then
             begin
             z := 1.0/z; cc := f*z; s := h*z;
             end;
          f := (cc*g) + (s*y);
          x := (cc*y) - (s*g);
          for jj := 1 to m do
              begin
              y := u[jj,j]; z := u[jj,i];
              u[jj,j] := y*cc + z*s;
              u[jj,i] := z*cc - y*s;
              end;
          end;
      rv1[l] := 0.0;
      rv1[k] := f;
      w[k] := x;
3:      end;
  end;
  finally
    rv1.free;
  end;

  if AugMatrix then
     begin
     { This means that originally m < n, therefore u has some junk rows, remove them here }
     Aug := TMatrix.Create (m, n);
     try
       for i := 1 to m do
           for j := 1 to n do
               Aug[i,j] := u[i,j];
       u.FreeSpace; u.SetSize (m, n); u.Copy (Aug);
     finally
       Aug.free;
     end;
     end;
end;



{ Call this after having called svd, computes x = V [diag (1/wj)]. U^t.b }
procedure TMatrix.svdSolve (var u : TMatrix; var w : TDVector; var v : TMatrix;
                            b : TDVector; var x : TDVector);
var j, i, n, m : integer; s: TDMatElement; tmp: TDVector;
begin
  m := u.r; n := u.c;
  tmp := TDVector.Create (u.c);
  try
    { Compute diag (1/wj) . U^t . b }
    for j := 1 to n do
        begin
        s := 0.0;
        if (w[j] <> 0.0) then
           begin
           for i := 1 to m do s := s + u[i,j]*b[i];
           s := s/w[j]
           end;
        tmp[j] := s
        end;
    { ...mult by V to get solution vector x }
    for i := 1 to n do
        begin
        s := 0.0;
        for j := 1 to w.size do s := s + v[i,j]*tmp[j];
        x[i] := s
        end;
  finally
    tmp.free;
  end;
end;


{ Solves the equation: (A.a - b)^2 = 0 for a. Where, A is the 'design matrix',
Aij = Xj(xi)/sigi, where Xj is the value of the jth basis function; b is the set
of weighted observed y values, b = yi/sigi; and a is the set of fitting coefficients
for the basis functions. Thus A.a - b expresses predicted - observed }

{ BasisProc is a procedure which must return in an array the values for the
basis functions at a particular value of xi, i.e it computes, Xj(xi) }

function TMatrix.svdfit (x, y, yerr : TDVector; var fit : TDVector;
        var u, v : TMatrix; var w : TDVector; funcs : BasisProc): TDMatElement;
const
   tol=1.0e-5;
var
   i, j : integer; wmax, weight, thresh, sum: TDMatElement;
   BasisVal, b : TDVector; A : TMatrix;
begin
  BasisVal := TDVector.Create (fit.size); b := TDVector.Create (x.size);
  A := TMatrix.Create (x.size, fit.size);
  try
    { Form the A matrix }
    for i := 1 to x.size do
        begin
        funcs(x[i], BasisVal);
        weight := 1.0/yerr[i];
        for j := 1 to fit.size do A[i,j] := BasisVal[j]*weight;
        b[i] := y[i]*weight
        end;
    A.svd (u, w, v);

    wmax := 0.0;
    for j := 1 to fit.size do if (w[j] > wmax) then wmax := w[j];
    thresh := tol*wmax;
    for j := 1 to fit.size do if (w[j] < thresh) then w[j] := 0.0;

    svdSolve (u, w, v, b, fit);

    result := 0.0;  { chisqr set to zero ready to accumulate }
    for i := 1 to x.size do
        begin
        funcs(x[i], BasisVal);
        sum := 0.0;
        for j := 1 to fit.size do sum := sum + fit[j]*BasisVal[j];
        result := result + sqr((y[i]-sum)/yerr[i]); { Accumulate chisqr }
        end;
  finally
    BasisVal.free; A.free; b.free;
  end;
end;


procedure TMatrix.svdCovar (v : TMatrix; w : TDVector; alpha : TMatrix);
var i, j, k : integer; wti : TDVector; sum : TDMatElement;
begin
  wti := TDVector.Create (w.size);
  try
    for i := 1 to w.size do
        begin
        wti[i] := 0.0;
        if w[i] > 0.0 then wti[i] := 1.0/(w[i]*w[i]);
        end;
    for i := 1 to w.size do
        begin
        for j := 1 to i do
            begin
            sum := 0.0;
            for k := 1 to w.size do sum := sum + v[i,k]*v[j,k]*wti[k];
            alpha[j,i] := sum; alpha[i,j] := alpha[j,i];
            end;
        end;
  finally
    wti.free;
  end;
end;


procedure TMatrix.eliminate_cms (S, Tk1 : TMatrix; var cr, N : integer);            (* eliminating conserved moieties *)
var
  i,j,x,y,crc,old_cr : byte;
begin
  x := 0; cr := 0;                    (* cr - conservation relations *)
  for i := 1 to N do
      begin
      old_cr := cr;
      for j := i+1 to N do
          begin
          crc := 0;                                 (* crc - cr counter *)
          // S.c = number of reactions
          for y := 1 to S.c do crc := crc + trunc (System.abs(S[i,y]+S[j,y]));
          if crc = 0 then cr := cr+1;
          end;
      if cr = old_cr then
         begin
         x := x+1;
         for y := 1 to S.c do
             Tk1[x,y] := S[i,y];
         end;
      end;
end;


procedure TMatrix.EigenValues (var wr, wi : TDVector);
begin
  EigenVals (Self, wr, wi);
end;



procedure TMatrix.ElementaryModes (D : TIVector; var mf, mb, C, k : integer; Tk : TMatrix);
var i, j, cr, N, k1 : integer; Tk1 : TMatrix; hlpRow : TDVector;
begin
  N := Self.r;
  Tk1 := TMatrix.Create (Self.r, Self.c);
  hlpRow := TDVector.Create (Self.c);
  try
    {eliminate_cms;                       (* also transscribing S into  Tk1 *)
     N := N-cr;

     for i := 1 to R do
         begin
         for j := 1 to N do Tk[i,j] := Tk1[j,i];       (* transposing matrix  *)
         for j:=N+1 to N+R do
             if i=j-N then
                Tk[i,j]:=1       (* appending..  *)
             else Tk[i,j]:=0;                                         (*..unity matrix*)
         end;
                                    (* (preliminary) fund. rows to the top *)
    i := 0;                         (*          splitting indices into F/B *)
    for j := 1 TO R DO
    begin
      if (D[j] <> 0) then
         begin
         i := i+1;
         hlprow := Tk[i];
         Tk[i] := Tk[j];
         Tk[j] := hlprow;
         end;
    end;
    mf := i;                                     (* no. of fundamental rows *)
    mb := R-mf;}



    eliminate_cms (Self, Tk1, cr, N);          (* also transscribing S into  Tk1 *)
    N := N-cr;

    for i := 1 to Self.c do
        begin
        for j := 1 to N do Tk[i,j] := Tk1[j,i];     (* transposing matrix  *)
        for j := N+1 to N+Self.c do
            if i=j-N then
               Tk[i,j] := 1       (* appending..  *)
            else Tk[i,j] := 0;    (*..unity matrix*)
        end;
                                  (* (preliminary) fund. rows to the top *)
    i := 0;                       (*          splitting indices into F/B *)
    for j := 1 TO Self.c DO
    begin
      if (D[j] <>  0) then
         begin
         i := i+1;
         for k1 := 1 to Self.c do hlprow[k1] := Tk[i,k1];
         for k1 := 1 to Self.c do Tk[i,k1] := Tk[j,k1];
         for k1 := 1 to Self.c do Tk[j,k1] := hlprow[k1];
         //hlprow := Tk[i];
         //Tk[i] := Tk[j];
         //Tk[j] := hlprow;
         end;
    end;
    mf := i;                                     (* no. of fundamental rows *)
    mb := Self.c-mf;                                  (* no. of basis rows       *)

    Tableau (N, Self.c, mf, mb, C, k, Tk, Tk1);
  finally
    hlpRow.Free;
    Tk1.Free;
  end;
end;


class function TMatrix.grecodiv(P, Rest: integer) : integer;
var
  old_Rest : integer;
begin
  grecodiv := 1;
  if (Rest*P <> 0) then
      begin
      if ABS(P) < ABS(Rest) then
         begin
         old_Rest := Rest;
         Rest := P;
         P := old_Rest;    (* swap P 'n' R *)
         end;

      repeat                                     (* Euclidean Algorithm: *)
        old_Rest := Rest;
 Rest := P mod old_Rest;
 P := old_Rest;
      until (Rest = 0);
      grecodiv := P;
      end
  else
      if (P = 0) then
         begin
         if (Rest = 0) then grecodiv := 1 else grecodiv := Rest;
         end
      else grecodiv := P;
end;


class function TMatrix.grecodiv_of_vector (N, R : integer; vec : TDVector) : integer;
var
  x      : byte;
  coeff  : integer;
begin
  coeff := trunc (vec[1]);
  for x := 2 to (N+R) do
      begin
      if (vec[x] <> 0) then coeff := grecodiv(trunc (vec[x]), coeff);
      end;
  grecodiv_of_vector := coeff;
end;





class procedure TMatrix.Tableau (N, R : integer; var mf, mb, C, k : integer; Tk, Tk1 : TMatrix);
var
  i,j,k1,x,xa,y,xend,m      : integer;
  cf,dir,ifrom,iend      : integer;
  index,bool,allow_comb  : boolean;
  l : integer;
  vec : TDVector;
begin
  C := R;                            (* C: number of rows of the tableau *)
  k := 0;                                            (* k: tableau index *)
  vec := TDVector.Create (Tk1.c);

  repeat

    //output;                                        (* HELPFUL MONITORING*)
    //write(' k =  ');writeln(k);writeln('cf=',cf);           (* OF TABLEAU STEPS   *)
    {write(' Press <ENTER> to continue.'); readln;}

    l := 1;                         (* l: row index in the tableau k+1 *)
    cf := 0;                        (* counter for f-rows in the tableau k+1*)
    for dir :=1 to 2 do
        BEGIN
        IF dir=1 THEN
           BEGIN
           ifrom:=1; iend:=mf;
           END
        ELSE
           BEGIN
           ifrom:=mf+1; iend:=c
           END;
           FOR i := ifrom TO iend DO
               BEGIN
               IF Tk[i,k+1] = 0 THEN                     (*  copying rows that  *)
                  BEGIN                                  (* have a zero element *)
                  for k1 := 1 to Tk1.c do
                      Tk1[l, k1] := Tk[i, k1];             (*      already        *)
                  //Tk1[l] := Tk[i];                     (*      already        *)
                  l := l+1;
                  IF i <= mf THEN cf := cf+1;
                  END
               END;
               FOR i:=ifrom TO iend DO
                   BEGIN
                   IF Tk[i,k+1]<>0 THEN
                      BEGIN
                      FOR j := i+1 TO C DO
                          BEGIN
                          IF Tk[j,k+1] <> 0 THEN
                             BEGIN
                             IF Tk[i,k+1]*Tk[j,k+1] > 0 THEN
                                BEGIN             (* not for f-rows with *)
                                IF j <= mf THEN
                                   allow_comb := false    (*      same signum    *)
                                ELSE
                                   BEGIN
                                   FOR y := 1 TO N+R DO Tk[j,y] := -1 * Tk[j,y]; (* invert b-row *)
                                   allow_comb := true;
                                   END;
                                END
                             ELSE allow_comb := true;
                             IF allow_comb THEN
                                BEGIN
                                index:=true;                   (* first simplicity (S) test: *)
                                IF (l>1) THEN
                                   BEGIN
                                   IF dir=1 THEN x:=0
                                   ELSE x:=cf;
                            WHILE (x<l-1) AND (INDEX) DO
                                         BEGIN
                                  x:=x+1;
                                  y:=n;
                                  bool:=true;
                                  REPEAT
                                    y:=y+1;
                                    IF ((Tk[i,y] = 0) and (Tk[j,y] = 0)) THEN
                                       IF Tk1[x,y] <> Tk[i,y] THEN bool:=false;
                                  UNTIL (y=n+r)or NOT bool;
                                  IF (y=n+r)and bool THEN index:=false;
                                         END;
                                   END;
                            IF index THEN
                                      BEGIN              (* combine rows *)
                               FOR y:=1 TO R+N DO
                                   Tk1[l,y]:=abs(Tk[i,k+1])*Tk[j,y]+abs(Tk[j,k+1])*Tk[i,y];

                                      for k1 := 1 to Tk1.c do
                                          vec[i] := Tk1[l,k1];

                                      m:= Grecodiv_of_vector(N, R, vec) ;
                                      //m:= Grecodiv_of_vector(Tk1[l]) ;
                                      IF (ABS(m)<>1) AND (m<>0) THEN FOR y:=1 to R+N DO
                                      Tk1[l,y]:= trunc (Tk1[l,y]) DIV ABS(m);
                                      l := l+1;
                                      IF i <= mf THEN cf := cf+1;
                                          (* second simplicity (S) test: *)
                                      IF dir=1 THEN x:=0
                                      ELSE x:=cf;
                                      bool:=true;
                               WHILE (X<L-2) AND (bool=true) DO
                                            BEGIN
                                     x:=x+1;
                                     y:=n;
                                     bool:=false;
                                     REPEAT
                                       y:=y+1;
                                       IF Tk1[x,y]=0 THEN
                                          IF (Tk1[x,y]<>Tk[i,y]) OR (Tk1[x,y]<>Tk[j,y])
                                                    THEN bool:=true;
                                     UNTIL (y=n+r)or bool;
                                     IF (y=n+r)and NOT(bool) THEN
                                               BEGIN
                                               {writeln('Jetzt hat folgende Zeile:');
                                               FOR Y:=n+1 to n+r DO
                                                  write(Tk1[x,y]:3);
                                               writeln; writeln('x=',x);
                                               writeln; writeln('l-1=',l-1);
                                               writeln('verloren gegen folgende Zeilen:');
                                               FOR Y:=n+1 to n+r DO
                                                   write(Tk[i,y]:3);
                                               writeln; writeln('i=',i);
                                               FOR Y:=n+1 to n+r DO
                                                   write(Tk[j,y]:3);
                                               writeln; writeln('j=',j); writeln;
                                               writeln(x,'+1te Zeile:');
                                               FOR Y:=n+1 to n+r DO
                                               write(Tk1[x+1,y]:3);
                                               writeln;}
                                               FOR xa:=x TO l-2 DO
                                                   BEGIN
                                                   FOR y:=1 TO n+r DO
                                                       Tk1[xa,y]:=Tk1[xa+1,y];
                                                   END;
                                               l:=l-1;
                                               IF x<=cf THEN cf:=cf-1;
                                               END;
                                            END;
                               END;
                                   END;
                                END;
                             END;
                          END;
                      END;
                   END;

        C := l-1;                                      (* new no. of rows   *)
        mf := cf;
        mb := C-mf;

       k := k+1;                                      (*  next tableau        *)
       for i := 1 to C do
           begin
           for k1 := 1 to Tk.c do
               Tk[i, k1] := Tk1[i, k1];            (*  restarting with Tk1 *)
           end;
       //for i := 1 to C do Tk[i] := Tk1[i];            (*  restarting with Tk1 *)

  until (k = N) or ((mb = 0) and (mf = 0));

  //if ((mb = 0) and (mf = 0)) then
  //    writeln(' There exist neither irreversible nor reversible flux modes.')
  //else
  //   output;
  vec.Free;
end;


// Evaluate conservation relations, uses the algorthim: tr(ns(tr(m)))
function TMatrix.GetConserve(Cn : TMatrix; tol : Double) : TMatrix;
var tmp, ns, echelon : TMatrix; b : integer;
begin
  tmp := TMatrix.Create (Self.c, Self.r);
  ns  := TMatrix.Create (1,1);
  echelon := TMatrix.Create (1,1);
  try
    tmp.Transpose (Self);
    tmp.NullSpace (ns, b, Tol);
    //tmp.NullSpace (ns, b, Echelon, r, tol);
    Cn.SetSize (ns.c, ns.r);
    Cn.Transpose (ns);
    Result := Cn;
  finally
    ns.free;
    echelon.free;
    tmp.free;
  end;
end;


// Evaluate conservation relations, uses the algorthim: -L0 I
function TMatrix.GetConserve2(Cn : TMatrix; tol : Double) : TMatrix;
var tmp, ns, Ident, L0 : TMatrix; i, j, L0Width : integer;
begin
  Self.GetL0 (L0, Tol);
  L0Width := L0.c;
  Cn.SetSize (L0.r, L0.c + L0.r);
  Cn.Zero;
  for i := 1 to L0.r do
      for j := 1 to L0.c do
          Cn[i,j] := -L0[i,j];
  Ident := TMatrix.Create (L0.r);
  for i := 1 to Cn.c do
      Cn.cName[i] := Self.rName[i];

  for i := 1 to L0.r do
      begin
      Cn.rname[i] := 'C' + IntToStr (i);
      for j := 1 to L0.r do
          Cn[i,j+L0Width] := Ident[i,j];
      end;
  Result := Cn;
  L0.Free;

  //tmp := TMatrix.Create (Self.c, Self.r);
  //ns  := TMatrix.Create (1,1);
  //echelon := TMatrix.Create (1,1);
  //try
  //  tmp.Transpose (Self);
  //  tmp.NullSpace (ns, b, Tol);
  //  //tmp.NullSpace (ns, b, Echelon, r, tol);
  //  Cn.SetSize (ns.c, ns.r);
  //  Cn.Transpose (ns);
  //  Result := Cn;
  //finally
  //  ns.free;
  //  echelon.free;
  //  tmp.free;
  //end;
end;


// work out if the cth column of m is made up of zeros except for a one at
// the rth row position
function ei_found (m : TMatrix; c, r : integer) : boolean;
var i : integer;
begin
  result := TRUE;
  // scan down the rows of the cth column
  for i := 1 to m.r do
      begin
      if i = r then  // If we're at the chosen row then check if we've got a ONE
         begin
         // I expect a value of ONE here (+1 or -1)
         if not m.IsOne (i, c) then
            begin result := FALSE; exit; end;
         end
      else // if not then I expect a value of ZERO here */
         if not m.IsZero (i, c) then
            begin result := FALSE; exit; end;
    end;
end;


// Search for a column, in the matrix m, containing zeros throughout
// except for the cth row which must be a ONE
function find_ei (m : TMatrix; c : integer) : integer;
var i : integer;
begin
   Result := 0;
   // start by scanning all columns
   for i := 1 to m.c do
       begin
       // Now scan the particular column we're on
       if ei_found (m, i, c) then
          begin
          result := i;
          exit;
          end;
       end;
end;


// ------------------------------------------------------------------- */
//     Reorder matrix rows into independent rows first follwed by      */
//                            dependent rows                           */
//     Returns p, the permutation vector, if the jth value of p        */
//     has the value i, the p A, will swap rows i and j                */
// ------------------------------------------------------------------- */
function TMatrix.ReOrder (var nRank : integer; Tol : Double; SortOrder : TReOrderSortOrder) : TMatrix;
var tm, echelon : TMatrix;
    c, i : integer;
begin
  InternalTolerance := Tol;
  if (Self.r = 0) or (Self.c = 0) then
     begin
     nRank := 0;
     Result := Self;
     Exit;
     end;

  if tol = 0 then
     tol := 1.0e-8;

   //if p = nil then
   //   p := TDVectori.Create (Self.r)
   //else p.SetSize (Self.r);
   //for i := 1 to p.Size do p[i] := 1;

   // remember to work on the transpose of the stoichiometry matrix
   tm := TMatrix.Create (Self.c, Self.r);
   tm.transpose (Self);

   echelon := TMatrix.Create (tm.r, tm.c);
   nRank := tm.Rank (echelon, tol);

   // look for the unit vectors (r of them) in echelon to make up an
   // identity matrix at the front of the echelon
   for i := 1 to nRank do
       begin
       if (not ei_found (echelon, i, i)) then
          begin
          c := find_ei(echelon, i);
          echelon.ExchangeCols (i, c);
          // swap rows in the stoichiometry matrix to reorder
          Self.ExchangeRows (i, c);
          if @SortOrder <> nil then
             SortOrder (i, c);
          //p[i] := c;
          //p[c] := i;
          end;
       end;

   echelon.Free; tm.Free;
   Result := Self;
end;



// ------------------------------------------------------------------- */
//                  Evaluate Reder's Nr Matrix                         */
//                                                                     */
// Usage: S.getNr (Nr, 1e-9)                                           */
// ------------------------------------------------------------------- */
procedure TMatrix.GetNr(var Nr : TMatrix; tol : Double);
var i, j, c, nRank : integer;
    echelon, tm : TMatrix;
begin
  // remember to work on the transpose
  tm := TMatrix.Create (Self.c, Self.r);
  tm.transpose (Self);

  echelon := TMatrix.Create (tm.r, tm.c);
  nRank := tm.Rank(echelon, tol);

  // look for the unit vectors (r of them) in echelon to make up an
  // identity matrix at the front of the echelon
  for i := 1 to nRank do
      begin
      if (not ei_found (echelon, i, i)) then
         begin
         c := find_ei(echelon, i);
         echelon.ExchangeCols (i, nc);
         // swap the equivalent rows in the stoichiometry matrix to reorder
         Self.ExchangeRows (i, c);
       end;
  end;

  Nr := TMatrix.Create (nRank, Self.c);

  // copy top r rows of m into Nr
  for i := 1 to nRank do
      for j := 1 to Self.c do
          Nr[i, j] := Self[i, j];

  // remember to copy over the row and column labels
  // Note: only copy over 'r' rows, not all of them
  for i := 1 to Self.c do
      Nr.cName[i] := Self.cName[i];
  for i := 1 to nRank do
      Nr.rName[i] := Self.rName[i];

  echelon.free; tm.free;
end;


// ------------------------------------------------------------------- */
//                  Evaluate L0 matrix of Reder                        */
// THEORY                                                              */
// Make the reduced row echelon of the TRANSPOSE of the stoichiometry  */
// matrix. Now reorder the columns (if necessary) so as to bring to    */
// the left half of the matrix all the unit vectors in order to make   */
// up a leading identity matrix. Simultaneously, perform the same      */
// reordering on the stoichiometry matrix. The L0 vectors will now be  */
// found as the bottom set of vectors in the transpose of the echelon   */
// Note: number of rows in L0 = rows in m - rank of m                   */
// ------------------------------------------------------------------- */
procedure TMatrix.GetL0 (var L0 : TMatrix; tol : Double);
var nRank, c, i, j : integer;
    tm, echelon, techelon : TMatrix;
begin
   if (Self.r = 0) or (Self.c = 0) then
      begin
      L0 := TMatrix.Create (0, 0);
      Exit;
      end;

   try
     // remember to work on the transpose
     tm := TMatrix.Create (Self.c, Self.r);
     tm.Transpose (Self);

     echelon := TMatrix.Create (tm.r, tm.c);
     nRank := tm.Rank(echelon, tol);

     // look for the unit vectors (r of them) in echelon to make up an
     // identity matrix at the front of the echelon */
     for i := 1 to nRank do
         begin
         if (not ei_found (echelon, i, i)) then
            begin
            c := find_ei(echelon, i);
            echelon.ExchangeCols (i, c);
            // swap rows in the stoichiometry matrix to reorder
            Self.ExchangeRows (i, c);
         end;
     end;

     techelon := TMatrix.Create (echelon.c, echelon.r);
     techelon.transpose (echelon);
     // only bother making l0 matrix if there is one to be made
     if techelon.r - nRank > 0 then
        begin
        L0 := TMatrix.Create (techelon.r - nRank, nRank);
        for i := nRank+1 to techelon.r do
            begin
            L0.setRowName (i-nRank, techelon.getRowName (i));
            for j := 1 to nRank do
                L0[i-nRank, j] := techelon[i, j];
            end;
     end
     else
       L0 := TMatrix.Create (0, 0);
   finally
     echelon.free;
     tm.free;
     techelon.free;
   end;
end;


// ------------------------------------------------------------------- */
//                  Evaluate L Matrix or Reder                         */
// Algorithm: L = tr (echelon (tr (m)))                                */
// ------------------------------------------------------------------- */
procedure TMatrix.GetL (var L : TMatrix; tol : Double);
var i, j, nRank, c : integer;
    echelon, techelon, tm : TMatrix;
begin
  // remember to work on the transpose
  tm := TMatrix.Create (Self.c, Self.r);
  tm.transpose (Self);

  echelon := TMatrix.Create (tm.r, tm.c);
  nRank := tm.Rank(echelon, tol);

  // look for the unit vectors (r of them) in echelon to make up an
  // identity matrix at the front of the echelon
  for i := 1 to nRank do
      begin
      if (not ei_found (echelon, i, i))  then
         begin
         c := find_ei (echelon, i);
         echelon.ExchangeCols (i, c);
         // swap rows in the stoichiometry matrix while we're at it
         Self.ExchangeRows (i, c);
      end;
  end;
  tm.free;
  techelon := TMatrix.Create (echelon.c, echelon.r);
  techelon.transpose (echelon);
   // Now copy over the L part to eliminate redudant zero columns
   L := TMatrix.Create (techelon.r, nRank);
   for i := 1 to techelon.r do
       begin
       L.setRowName (i, techelon.getRowName (i));
       for j := 1 to nRank do // only copy upto nRank columns
           L[i, j] := techelon[i, j];
       end;
   echelon.free;
   techelon.free;
end;

// ------------------------------------------------------------------- */
//                  Evaluate K Matrix or Reder                         */
// Algorithm: K = tr (echelon (m))                                */
// ------------------------------------------------------------------- */
procedure TMatrix.GetK (var K : TMatrix; tol : Double);
var i, j, nRank, c : integer;
    echelon, techelon, cpy: TMatrix;
    numColsK : integer;
    Ko : TMatrix;
    ii : TMatrix;
begin
  // Make a copy
  cpy := TMatrix.Clone(self);

  echelon := TMatrix.Create (cpy.r, cpy.c);
  nRank := cpy.Rank(echelon, tol);
  numColsK := cpy.c - nRank;

  // look for the unit vectors (r of them) in echelon to make up an
  // identity matrix at the front of the echelon
  for i := 1 to nRank do
      begin
      if (not ei_found (echelon, i, i))  then
         begin
         c := find_ei (echelon, i);
         echelon.ExchangeCols (i, r);
         // swap rows in the stoichiometry matrix while we're at it
         //Self.ExchangeRows (i, r);
      end;
  end;
  cpy.free;
  Ko := TMatrix.Create (nRank, numColsK);
  for i := 1 to nRank do
      begin
      for j := nRank to echelon.c do
          Ko[i, j-nRank] := -echelon[i,j];
      end;

  ii := TMatrix.Create (numColsK);
  K := TMatrix.AugmentRows (ii, Ko);
  for i := 1 to K.r do
      K.setRowName (i, self.getColumnName (i));


  //techelon := TMatrix.Create (echelon.c, echelon.r);
  //techelon.transpose (echelon);
   //// Now copy over the K part to eliminate redudant zero columns
   //K := TMatrix.Create (techelon.r, numColsK); K.RefCount := 1;
   //for i := 1 to techelon.r do
   //    begin
   //    K.setRowName (i, techelon.getRowName (i));
   //    for j := 1 to numColsK do // only copy upto nRank - numColsK columns
   //        K[i, j] := techelon[i, j];
   //    end;
   echelon.free;
   Ko.free;
   ii.free;
   //techelon.free;
end;

end.

