unit uJVector;

// Implements dynamic vector type, integer and TDVectorElementType, Summer 1996 H M Sauro
// Copyright 1996/97 Future Skill Software

// There are no restrictions on the use of this software
// However if you make any modifications which might be useful to others
// then please notify HSauro@fssc.demon.co.uk so that the changes can
// be made available to all.

// Latest modification: March 1997. Added Norm
// Updated Feb 2002, added complex vector type

{ Available functions for TDVectorElementType vector:

        EnlargeBy       Enlarge vector by n elements
        ReduceBy        Reduce vector by n elements
        Enlarge         Enlarge by one element (top element)
        Reduce          Reduce by one element (top element)
        Zero            Set all elemenets to 0.0
        Clear           Same as Zero
        Size            Get size of vector
        Assign          Assigns one vector to another (Copy function)
        Add, Sub        Add or subract two vectors to give Self
        Dot             Form the dot product
        CrossU          Form the cross product (self overwritten)
        Cross           Form the cross product
        Sum             Form sum of elements
        Mean            Form mean of elements
        SumofSquares    Form sum of squares of elements
        Norm            Returns the Euclidean Norm
        StdDev          Form sample standard deviation
        Scale           Scale vector by factor }


{ Example of use:

  v : TDVector;

  v := TDVector.Create (5);
  for i := 1 to 5 do v[i] := i*2.3;
  m := v.mean;
  v.free; }


interface

uses Classes, SysUtils, uComplex;


type
     EVectorSizeError = class (Exception);
     TUnaryDoubleFunc = function (const x : Double) : Double;
     TUnaryComplexFunc = function (const x : TComplex) : TComplex;
     TBinaryDoubleFunc = function (x, y : double) : double;
     TBinaryComplexFunc = function (x, y : TComplex) : TComplex;

     //TComplex = record r, i : double; end;
     //TDVectorElementType = double;
     { The 1000 in the array types below does not impose a limit at runtime!
     If you compile with range checking on then the compiled code will impose
     an effective limit of 1000, but with range checking off the size of
     vector is limited to 64K under 16bit OS or *much* greater under 32bit OS }
     TArrayd = array[1..20000000-1] of double;   pTArrayd = ^TArrayd;
     TArrayc = array[1..20000000-1] of TComplex; pTArrayc = ^TArrayc;
     TArrayi = array[1..20000000-1] of integer;  pTArrayi = ^TArrayi;

     // Base vector
     TJVector = class (TObject)
           private
              s : integer;
              FName : string;
           public
              function    InRange (i : integer) : Boolean;
              property    Size : integer read s;
              property    Name : string read FName write FName;
         end;


     { Define a dynamic array type for holding integers }
     TIVector = class (TJVector)
                 private
                    vx : pTArrayi; { pointer to the data }
                 private
                 public
                   constructor create (i : integer); virtual;
                   destructor  destroy; override;
                   constructor Clone (v : TIVector); virtual;
                   procedure   EnlargeBy (n : integer);
                   procedure   ReduceBy (n : integer);
                   procedure   Enlarge;
                   procedure   Reduce;
                   procedure   Zero;
                   procedure   Clear;
                   procedure   Assign (v : TIVector);
                   procedure   Setval (i : integer; v : integer);
                   function    Getval (i : integer) : integer;
                   function    GetSize : integer;
                   procedure   Copy (v : TIVector);
                   procedure   SetSize (NewSize : integer);
                   property    Elem[x : Integer] : integer read GetVal write SetVal; default;
                   property    Size : integer read s;
                   property    Name : string read FName write FName;
                end;

     { Define a dynamic array type for holding TDVectorElementTypes }
     TDVector = class (TJVector)
                 private
                    vx : pTArrayd;  { pointer to the data }
                    Tmp : boolean;  { set to true if temporary }
                 public
                   { Declare as a class method, saves having a self variable }
                   class function Dot (u, v : TDVector) : double;
                   class function Equal (u, v : TDVector) : Boolean;

                   constructor Create; overload; virtual;
                   constructor create (n : integer); overload; virtual;
                   constructor Create (n : integer; d : Double); overload; virtual;
                   constructor createTmp (n : integer);
                   constructor Clone (v : TDVector); virtual;
                   constructor LoadFromStream (st : TStream); virtual;
                   destructor  destroy; override;
                   procedure   FreeSpace;
                   procedure   SetSize (i : integer);
                   procedure   EnlargeBy (n : integer);
                   procedure   ReduceBy (n : integer);
                   procedure   Enlarge;
                   procedure   Reduce;
                   procedure   Zero;
                   procedure   Clear;
                   function    GetSizeOf : integer;
                   procedure   Setval (i : integer; v : double);
                   function    Getval (i : integer) : double;
                   property    Elem[x : Integer] : double read GetVal write SetVal; default;
                   property    Size : integer read s;
                   procedure   Assign (v : TDVector);
                   procedure   Copy (v : TDVector);
                   function    Add (v, u : TDVector) : TDVector; overload;
                   function    Add (v : TDVector; k : Double) : TDVector; overload;
                   function    Sub (v, u : TDVector) : TDVector; overload;
                   function    Sub (v : TDVector; k : Double) : TDVector; overload;
                   class function xAdd (v, u : TDVector) : TDVector;
                   class function xSub (v, u : TDVector) : TDVector;
                   function    DotU (v : TDVector) : double;
                   function    CrossU (v : TDVector) : TDVector;
                   function    Cross (v1, v2 : TDVector) : TDVector;
                   function    Mult (v : TDVector; k : Double) : TDVector; overload;
                   function    Mult (v : TDVector; m : TObject) : TDVector; overload;
                   function    Mult (m : TObject; v : TDVector) : TDVector; overload;
                   function    Power (v : TDVector; x : double) : TDVector;
                   function    CreateRange (Start, Stop, Step : double) : TDVector;
                   //function    CreateLogRange (Start, Stop : double; numberOfPoints : integer) : TDVector;
                   function    Prod : double;
                   function    Sum  : double;
                   function    Min : double;
                   function    Max : double;
                   function    Mean : double;
                   function    SumofSquares : double;
                   function    Norm : double;
                   function    StdDev : double;
                   procedure   ApplyUnaryFunction (const f : TUnaryDoubleFunc);
                   procedure   ApplyBinaryFunction (f : TBinaryDoubleFunc; x, y : Double);
                   procedure   Scale (factor : double);
                   function    SaveToString : string;
                   procedure   SaveToStream (st : TStream);
                end;


     // Define a dynamic array type for holding TComplex types
     TCVector = class (TJVector)
                 private
                    vx : pTArrayc;  { pointer to the data }
                    Tmp : boolean;  { set to true if temporary }
                 public
                   { Declare as a class method, saves having a self variable }
                   class function Dot (u, v : TCVector) : TComplex;
                   class function Equal (u, v : TCVector) : Boolean;

                   constructor create; overload; virtual;
                   constructor create (n : integer); overload; virtual;
                   constructor Create (n : integer; d : TComplex); overload; virtual;
                   constructor createTmp (n : integer);
                   constructor Clone (v : TCVector); virtual;
                   constructor LoadFromStream (st : TStream); virtual;
                   destructor  destroy; override;
                   procedure   FreeSpace;
                   procedure   SetSize (i : integer);
                   procedure   EnlargeBy (n : integer);
                   procedure   ReduceBy (n : integer);
                   procedure   Enlarge;
                   procedure   Reduce;
                   procedure   Zero;
                   procedure   Clear;
                   function    getSizeOf : integer;
                   function    getPtr : pTArrayc;
                   procedure   Setval (i : integer; v : TComplex);
                   function    Getval (i : integer) : TComplex;
                   property    Elem[x : Integer] : TComplex read GetVal write SetVal; default;
                   property    Size : integer read s;
                   procedure   Assign (v : TCVector);
                   procedure   Copy (v : TCVector);
                   function    Add (v, u : TCVector) : TCVector; overload;
                   function    Add (v : TCVector; k : Double) : TCVector; overload;
                   function    Sub (v, u : TCVector) : TCVector; overload;
                   function    Sub (v : TCVector; k : Double) : TCVector; overload;
                   class function xAdd (v, u : TCVector) : TCVector;
                   class function xSub (v, u : TCVector) : TCVector;
                   {function    DotU (v : TDVectord) : double;
                   function    CrossU (v : TDVectord) : TDVectord;
                   function    Cross (v1, v2 : TDVectord) : TDVectord;}
                   function    Mult (v : TCVector; k : Double) : TCVector; overload;
                   function    Mult (v : TCVector; m : TObject) : TCVector; overload;
                   function    Mult (m : TObject; v : TCVector) : TCVector; overload;
                   function    Divide (v : TCVector; k : TComplex) : TCVector; overload;
                   function    Power (v : TCVector; x : TComplex) : TCVector;
                   function    CreateRange (Start, Stop, Step : double) : TCVector;
                   function    CreateLogRange (Start, Stop : double; numberOfPoints : integer) : TCVector;
                   function    Prod : TComplex;
                   function    Sum  : TComplex;
                   function    Min : TComplex;
                   function    Max : TComplex;
                   function    Mean : TComplex;
                   function    SumofSquares : TComplex;
                   function    Norm : TComplex;
                   function    Variance : TComplex;
                   function    populationVariance : TComplex;
                   function    StdDev : TComplex;
                   function    populationStandardDeviation : TComplex;
                   function    Abs (v : TCVector) : TCVector;
                   procedure   ApplyUnaryFunction (const f : TUnaryComplexFunc);
                   procedure   ApplyBinaryFunction (f : TBinaryComplexFunc; x, y : TComplex);
                   procedure   Scale (factor : double);
                   function    SaveToString : string;
                   procedure   SaveToStream (st : TStream);
                end;


function doubleToComplex(v : TDVector) : TCVector; overload;
function complexToDouble (v : TCVector) : TDVector; overload;

implementation

uses Math, uMatrix;


function doubleToComplex(v : TDVector) : TCVector;
var i : integer;
begin
  result := TCVector.Create (v.size);
  for i := 1 to v.size do
      result.setVal(i, complex (v[i], 0.0));
end;


function complexToDouble (v : TCVector) : TDVector;
var i : integer;
begin
  result := TDVector.Create (v.size);
  for i := 1 to v.size do
      result[i] := v[i].r;
end;


// ---------------------------------------------------------------------


function TJVector.InRange (i : integer) : Boolean;
begin
  Result := (i <= s) and (i > 0);
end;


// -------------------------------------------------------------------------
//                   START OF COMPLEX VECTOR TYPE IMPLEMETATION
// -------------------------------------------------------------------------


constructor TCVector.Create;
begin
  Inherited Create;
  s := 0; vx := Nil;  { vx set to Nil to indicate empty vector, used by SetSize }
end;


{ Create a vector of size n }
constructor TCVector.Create(n : integer);
begin
  Inherited Create;
  if n > 0 then Self.SetSize (n);
  FName := 'untitled';
end;


constructor TCVector.Create (n : integer; d : TComplex);
var i : integer;
begin
  Inherited Create;
  if n > 0 then Self.SetSize (n);
  for i := 1 to n do
      Self[i] := d;
end;


constructor TCVector.createTmp (n : integer);
begin
  Inherited Create;
  if n > 0 then Self.SetSize (n);
  Tmp := true;
end;


{ ******************************************************************** }
{ Usage: u := TCVector.Clone (v)                                        }
{ ******************************************************************** }
constructor TCVector.Clone (v : TCVector);
begin
  Inherited Create;
  Self.SetSize (v.s);
  Self.Copy (v);
end;


destructor TCVector.destroy;
begin
  FreeSpace;
  Inherited Destroy;
end;


// Private internal procedure
procedure TCVector.FreeSpace;
begin
  if vx <> Nil then FreeMem (vx, sizeof (TComplex) * s); vx := Nil; s := 0;
end;


// Internal routine to allocate space. If space already exists then it frees it first
procedure TCVector.SetSize (i : integer);
begin
  if vx <> Nil then FreeMem (vx, sizeof (TComplex) * s);
  s := i; vx := AllocMem (sizeof (TComplex) * s);
end;


{ used internally but is also accessible from the outside }
procedure TCVector.Setval (i : integer; v : TComplex);
begin
  vx^[i] := v;
end;


{ used internally but is also accessible from the outside }
function  TCVector.Getval (i : integer) : TComplex;
begin
  result := vx^[i];
end;


// Enlarge the vector by one element without destroying any existing data
procedure TCVector.Enlarge;
begin
  ReAllocMem (vx, sizeof (TComplex)*(s+1)); inc (s); { Modified for D2 }
end;


{ Increase the size of the vector without destroying and existing data }
procedure TCVector.EnLargeBy (n : integer);
begin
  if n < 0 then raise EVectorSizeError.Create ('Argument to EnLargeBy must be positive');
  ReAllocMem (vx, sizeof (TComplex)*(s+n)); inc (s,n); { Modified for D2 }
end;


{ Reduce the size of the vector }
procedure TCVector.ReduceBy (n : integer);
begin
  if n >= s then
     raise EVectorSizeError.Create ('Can''t reduce size of vector to below zero elements');
  ReAllocMem (vx, sizeof (TComplex)*(s-n)); dec (s,n); { modified for D2 }
end;


{ Reduce the vector by one element, the top most element is destroyed }
procedure TCVector.Reduce;
begin
  ReAllocMem (vx, sizeof (TComplex)*(s-1)); dec (s); { Modified for D2 }
end;


{ Clears the vector, sets all elements to zero }
procedure TCVector.Zero;
var i : integer;
begin
  for i := 1 to s do vx^[i] := complex (0.0, 0.0);
end;


{ Clears the vector, sets all elements to zero }
procedure TCVector.Clear;
begin
  Zero;
end;


// -------------------------------------------------------------------------
// Scale the vector by factor
// Usage: v.Scale (2)   Multiplies all elements by 2
// -------------------------------------------------------------------------
procedure TCVector.Scale (factor : double);
var i : integer;
begin
  for i := 1 to s do vx^[i].r := vx^[i].r*factor;
end;


// -------------------------------------------------------------------------
// Write vector to a string
// Usage: v.SaveToString
// -------------------------------------------------------------------------
function TCVector.SaveToString : string;
var i : integer;
begin
  Result := '';
  for i := 1 to s do
      if vx^[i].i = 0 then
         Result := Result + Format ('%g ', [vx^[i].r])
      else
         begin
         Result := Result + Format ('(%g ', [vx^[i].r]);
         if vx^[i].i > 0 then
            Result := Result + Format ('+ %g)', [vx^[i].i])
         else Result := Result + Format ('- %g)', [System.abs (vx^[i].i)]) 
         end;
end;


procedure TCVector.SaveToStream (st : TStream);
begin
  st.write (s, SizeOf (s));
  st.write (vx^, sizeof (TComplex) * s);
end;


// Usage: v := TDVectord.Read (st);
constructor TCVector.LoadFromStream (st : TStream);
var c : integer;
begin
  inherited Create;
  st.read (c, SizeOf (integer));
  SetSize (c);
  st.read (vx^, SizeOf (TComplex) * c);
end;


// -------------------------------------------------------------------------
// Copies vector v, including contects to self. If self is not the same
// size as v then self is resized

//  Copy v to u:
//  Usage: u.Assign (v)
// -------------------------------------------------------------------------
procedure TCVector.Assign (v : TCVector);
begin
  Self.Copy (v);
end;


// -------------------------------------------------------------------------
// Copies vector v, including contects to self. If self is not the same
// size as v then self is resized

//  Copy v to u:
//  Usage: u.Copy (v)
// -------------------------------------------------------------------------
procedure TCVector.Copy (v : TCVector);
begin
  v.Tmp := False;  { just in case its a temporary variable }
  if v.s <> Self.s then Self.SetSize (v.s);
  move (v.vx^, Self.vx^, sizeof(TComplex) * s)
end;


// -------------------------------------------------------------------------
// Returns number of bytes occupied by vector

//  Usage: u.SizeOf
// -------------------------------------------------------------------------
function TCVector.getSizeOf : integer;
begin
  Result := s*SizeOf (TComplex) + SizeOf (s) + Length (FName);
end;


function TCVector.getPtr : pTArrayc;
begin
  result := vx;
end;


// -------------------------------------------------------------------------
// Test whether vectors 'u' and 'v' are equal
// Usage: AreEqual := TCVector.Equal (u, v);
// -------------------------------------------------------------------------
class function TCVector.Equal (u, v : TCVector) : boolean;
var i : integer;
begin
  Result := False;
  if u.Size = v.Size then
     begin
     Result := True;
     for i := 1 to u.Size do
         if (u[i].r <> v[i].r) or (u[i].i <> v[i].i) then
            begin
            Result := False;
            Exit;
            end;
     end;
end;


// -------------------------------------------------------------------------
// Compute the dot product of vectors 'u' and 'v'
// Usage: d := TDVectord.dot (u, v);
// -------------------------------------------------------------------------
class function TCVector.Dot (u, v : TCVector) : TComplex;
var i : integer;
begin
  if u.Size <> v.Size then
     raise EVectorSizeError.Create ('Vectors must be of the same size to compute dot product');

  result := complex (0, 0);
  for i := 1 to u.Size do result := complexAdd (result, complexMult (u[i], v[i]));
end;


// -------------------------------------------------------------------------
// Add the vectors, 'v' and 'u' together to produce Self. Error if v and u are
// the the same size. If Self is not sized correctly, then Add will resize Self

// Usage: w.Add (u, v)
// Add u to v giving result w
// -------------------------------------------------------------------------

function TCVector.Add (v, u : TCVector) : TCVector;
var i : integer;
begin
  if v.s <> u.s then
     raise EVectorSizeError.Create ('Vectors must be the same size to sum them');
  if Self.s <> v.s then Self.SetSize (v.s);
  for i := 1 to v.s do Self[i] := complexAdd (v[i], u[i]);
  if v.tmp then v.free; if u.tmp then u.free;
  result := Self;
end;


// -------------------------------------------------------------------------
// Add a scalar value 'k' to a vector, 'v' to produce Self.

// Usage: w.Add (u, 6)
// Add 6 to every element of v giving result w
// -------------------------------------------------------------------------
function TCVector.Add (v : TCVector; k : Double) : TCVector;
var i : integer;
begin
  for i := 1 to v.s do Self.setVal (i, complex (v[i].r + k, v[i].i));
  Result := Self;
end;


// -------------------------------------------------------------------------
// Add the vectors, 'v' and 'u' together and RETURN the result. An Error
// occurs if v and u are the the same size. xAdd returns the result to the
// caller therefore it is the responsibility of the caller to dispose of the
// memory allocated by xSub. Note, the variable which is used to store the
// returned result must not have been previously allocated, otherwise you'll
// get memory leak!

// w must be unallocated
// Usage: w := Add (u, v)
// Add u to v giving result w
// -------------------------------------------------------------------------

class function TCVector.xAdd (v, u : TCVector) : TCVector;
var i : integer; t : TCVector;
begin
  if v.s <> u.s then
     raise EVectorSizeError.Create ('Vectors must be the same size to sum them');
  t := TCVector.CreateTmp (v.s);
  for i := 1 to v.s do t[i] := complexAdd (v[i], u[i]);
  result := t;
end;



// -------------------------------------------------------------------------
// Subtract the vectors, 'v' and 'u' together to produce Self. Error if v and u are
// the the same size. If Self is not sized correctly, then Add will resize Self

// Usage: w.Sub (u, v)
// Add u to v giving result w
// -------------------------------------------------------------------------

function TCVector.Sub (v, u : TCVector) : TCVector;
var i : integer;
begin
  if v.s <> u.s then
     raise EVectorSizeError.Create ('Vectors must be the same size to subtract them');
  if Self.s <> v.s then Self.SetSize (v.s);
  for i := 1 to v.s do Self[i] := complexSub (v[i], u[i]);
  if v.tmp then v.free; if u.tmp then u.free;
  result := Self;
end;


// -------------------------------------------------------------------------
// Add a scalar value 'k' to a vector, 'v' to produce Self.

// Usage: w.Add (u, 6)
// Add 6 to every element of v giving result w
// -------------------------------------------------------------------------
function TCVector.Sub (v : TCVector; k : Double) : TCVector;
var i : integer;
begin
  for i := 1 to v.s do Self.setVal (i, complex (v[i].r - k, v[i].i));
  Result := Self;
end;


// -------------------------------------------------------------------------
// Subtract the vectors, 'v' and 'u' together and RETURN the result. An Error
// occurs if v and u are the the same size. xSub returns the result to the
// caller therefore it is the responsibility of the caller to dispose of the
// memory allocated by xSub. Note, the variable which is used to store the
// returned result must not have been previously allocated, otherwise you'll
// get memory leak!

// w must be unallocated
// Usage: w := Sub (u, v)
// Add u to v giving result w
// -------------------------------------------------------------------------


class function TCVector.xSub (v, u : TCVector) : TCVector;
var i : integer; t : TCVector;
begin
  if v.s <> u.s then
     raise EVectorSizeError.Create ('Vectors must be the same size to subtract them');
  t := TCVector.CreateTmp (v.s);
  for i := 1 to v.s do t.setVal (i, complexSub (v[i], u[i]));
  result := t;
end;


// Multiply the vector v by scalar k, returning result in Self
function TCVector.Mult (v : TCVector; k : Double) : TCVector;
var i : integer;
begin
  for i := 1 to s do Self.setVal (i, complex (k * v[i].r, k * v[i].i));
  Result := Self;
end;


function TCVector.Mult (v : TCVector; m : TObject) : TCVector;
var i, j : integer; AMatrix : TCMatrix; Sum : TComplex;
begin
  Self.Zero;
  AMatrix := m as TCMatrix;
  if AMatrix.r <> v.Size then
     raise EVectorSizeError.Create ('Incompatible size in Vector/Matrix multiply');
  for j := 1 to AMatrix.c do
      begin
      Sum := complex (0.0, 0.0);
      for i := 1 to v.Size do
          Sum := complexAdd (Sum, complexMult (v[i], AMatrix [i, j]));
      Self[j] := sum;
      end;
  Result := Self;
end;


// Matrix multiplied by vector
function TCVector.Mult (m : TObject; v : TCVector) : TCVector;
var i, j : integer; AMatrix : TCMatrix;
begin
  Self.Zero;
  AMatrix := TCMatrix (m);
  if AMatrix.c <> v.Size then
     raise EVectorSizeError.Create ('Incompatible dimensions in Matrix/Vector multiply (m:'
           + inttostr (AMatrix.c) + ' and v:' + inttostr (v.size) + ')');
  for i := 1 to AMatrix.r do
      for j := 1 to v.Size do
          Self[i] := complexAdd (Self[i], complexMult (AMatrix [i, j], v[j]));
  Result := Self;
end;


// Vector divide by scalar
function TCVector.Divide (v : TCVector; k : TComplex) : TCVector;
var i : integer;
begin
  for i := 1 to s do Self.setVal (i, complexDiv (v[i], k));
  Result := Self;
end;


// -------------------------------------------------------------------------
// Returns the vector v2 raised to the power of x and returns it as v1
// Usage: v1.power (v2, x);
// -------------------------------------------------------------------------
function TCVector.Power (v : TCVector; x : TComplex) : TCVector;
var i : integer;
begin
  if Self.s <> v.s then Self.SetSize (v.s);
  for i := 1 to v.Size do
      Self[i] := ComplexPow (v[i], x);
  Result := Self;
end;



// -------------------------------------------------------------------------
// Generates a range vector
// Usage: v.CreateRange (0, 10, 1)
// -------------------------------------------------------------------------
function TCVector.CreateRange (Start, Stop, Step : double) : TCVector;
var Count : integer; x : TComplex;
begin
  Self.FreeSpace;

  x := complex (Start, 0.0);
  Count := 1;
  repeat
    Enlarge;
    Self[Count] := x;
    x := complexAdd (x, Complex (Step, 0.0));
    Inc (Count);
  until complexAbs (x) > complexAbs (complex (Stop, 0.0));
  Result := Self;
end;


function TCVector.CreateLogRange (Start, Stop : double; numberOfPoints : integer) : TCVector;
var d : double; i : integer; x : TComplex;
begin
  Self.FreeSpace;

  for i := 0 to numberOfPoints - 1 do
    begin
    x := complex (Math.Power (10, start + (i*(stop-start)/(trunc (numberOfPoints)-1))), 0);
    Enlarge;
    Self[i+1] := x;
    end;
  Result := self;
end;

// -------------------------------------------------------------------------
// Returns the product of values in the vector
// Usage: product := v.prod
// -------------------------------------------------------------------------
function TCVector.Prod : TComplex;
var i : integer;
begin
  if s > 0 then
     begin
     result := vx^[1];
     if s > 1 then
        for i := 2 to s do result := complexMult (result, vx^[i]);
     end
  else result := complex (0, 0);
end;


// -------------------------------------------------------------------------
// Returns the sum of values in the vector
// Usage: total := v.sum
// -------------------------------------------------------------------------
function TCVector.Sum : TComplex;
var i : integer;
begin
  result := complex (0.0, 0.0);
  for i := 1 to s do result := complexAdd (result, vx^[i]);
end;


// -------------------------------------------------------------------------
// Returns the minimum value in the vector
// Usage: m := v.min
// -------------------------------------------------------------------------
function TCVector.Min : TComplex;
var i : integer;
begin
  if s > 0 then
     Result := vx^[1]
  else Result := complex (0, 0);

  for i := 1 to s do
      if complexLessThan (vx^[i], Result) then
         Result := vx^[i];
end;


// -------------------------------------------------------------------------
// Returns the maximum value in the vector
// Usage: m := v.max
// -------------------------------------------------------------------------
function TCVector.Max : TComplex;
var i : integer;
begin
  if s > 0 then
     Result := vx^[1]
  else Result := complex (0, 0);

  for i := 1 to s do
      if complexGreaterThan (vx^[i], Result) then
         Result := vx^[i];
end;


// -------------------------------------------------------------------------
// Returns the mean of the elements of the vector
// Usage: average := v.mean;
// -------------------------------------------------------------------------
function TCVector.Mean : TComplex;
begin
  if s > 0 then
     result := complex (sum.r/s, sum.i/s)
  else raise Exception.Create ('Vector must have at least one element to compute mean');
end;


// -------------------------------------------------------------------------
// Returns the sum of the squares of values in Data
// Usage: s := v.SumOfSquares;
// -------------------------------------------------------------------------
function TCVector.SumOfSquares : TComplex;
var i : integer;
begin
  result := complex (0.0, 0.0);
  for i := 1 to s do result := complexAdd (result, complexMult (vx^[i], vx^[i]));
end;


// -------------------------------------------------------------------------
// Returns the Euclidean norm of the Self vector
// -------------------------------------------------------------------------
function TCVector.Norm : TComplex;
begin
  result := complexSqrt (Self.SumOfSquares);
end;


// -------------------------------------------------------------------------
// Returns the sample variance
// Usage: sd := v.var;
// -------------------------------------------------------------------------
function TCVector.Variance : TComplex;
var i : integer; sq, total : TComplex;
begin
  sq := complex (0, 0); total := complex (0, 0);
  if s > 1 then
     begin
     for i := 1 to s do
         begin
         sq := complexAdd (sq, complexSqr(vx^[i]));
         total := complexAdd (total, vx^[i]);
         end;
     result := complexSub (sq, complexDiv (complexSqr(total), complex (s, 0)));
     result := complexDiv (result, complex (s-1, 0));
     end
  else
     raise Exception.Create ('Can''t calculate the variance for a vector with one or no elements');
end;


// -------------------------------------------------------------------------
// Returns the population variance
// Usage: sd := v.PopVar;
// -------------------------------------------------------------------------
function TCVector.populationVariance : TComplex;
var i : integer; sq, total : TComplex;
begin
  sq := complex (0, 0); total := complex (0, 0);
  if s > 1 then
     begin
     for i := 1 to s do
         begin
         sq := complexAdd (sq, complexSqr(vx^[i]));
         total := complexAdd (total, vx^[i]);
         end;
     result := complexSub (sq, complexDiv (complexSqr(total), complex (s, 0)));
     result := complexDiv (result, complex (s, 0));
     end
  else
     raise Exception.Create ('Can''t calculate the variance for a vector with one or no elements');
end;


// -------------------------------------------------------------------------
// Returns the population standard deviation
// Usage: sd := v.popSd;
// -------------------------------------------------------------------------
function TCVector.populationStandardDeviation : TComplex;
begin
  result := complexSqrt (self.populationVariance);
end;


// -------------------------------------------------------------------------
// Returns the sample standard deviation
// Usage: sd := v.sd;
// -------------------------------------------------------------------------
function TCVector.StdDev : TComplex;
begin
   result := complexSqrt (self.Variance);
end;


// -------------------------------------------------------------------------
// Returns the absolute vector of v2 as v1
// Usage: v1.abs (v2);
// -------------------------------------------------------------------------
function TCVector.Abs (v : TCVector) : TCVector;
var i : integer;
begin
  for i := 1 to v.Size do
      Self.setVal (i, complex (complexAbs (v[i]), 0.0));
  Result := Self;
end;


// -------------------------------------------------------------------------
// Applies the function f to all element of vector Self
// Usage: v.ApplyUnaryFunction (MyFunction)
// -------------------------------------------------------------------------
procedure TCVector.ApplyUnaryFunction (const f : TUnaryComplexFunc);
var i : integer;
begin
  for i := 1 to s do
      vx^[i] := f (vx^[i]);
end;



// -------------------------------------------------------------------------
// Applies the function f, using args x and y to all element of vector Self
// Usage: v.ApplyBinaryFunction (MyFunction, arg1, arg2)
// -------------------------------------------------------------------------
procedure TCVector.ApplyBinaryFunction (f : TBinaryComplexFunc; x, y : TComplex);
var i : integer;
begin
  for i := 1 to s do
      vx^[i] := f (x, y);
end;


// -------------------------------------------------------------------------
//                      START OF DOUBLE VECTOR TYPE IMPLEMETATION
// -------------------------------------------------------------------------



{ The data space which holds the data for a vector is typed as [1..x] so that
indexing autmatically starts at one, therefore there is no need in the
following code to add 1 to the size of the vector when creating or destroying it }

constructor TDVector.Create;
begin
  Inherited Create;
  s := 0; vx := Nil;  { vx set to Nil to indicate empty vector, used by SetSize }
end;


{ Create a vector of size n }
constructor TDVector.Create(n : integer);
begin
  Inherited Create;
  if n > 0 then Self.SetSize (n);
  FName := 'untitled';
end;


constructor TDVector.Create (n : integer; d : Double);
var i : integer;
begin
  Inherited Create;
  if n > 0 then Self.SetSize (n);
  for i := 1 to n do
      Self[i] := d;
end;


constructor TDVector.createTmp (n : integer);
begin
  Inherited Create;
  if n > 0 then Self.SetSize (n);
  Tmp := true;
end;


{ ******************************************************************** }
{ Usage: u := TDVectord.Clone (v)                                        }
{ ******************************************************************** }
constructor TDVector.Clone (v : TDVector);
begin
  Inherited Create;
  Self.SetSize (v.s);
  Self.Copy (v);
end;


destructor TDVector.destroy;
begin
  FreeSpace;
  Inherited Destroy;
end;


{ Private internal procedure }
procedure TDVector.FreeSpace;
begin
  if vx <> Nil then FreeMem (vx, sizeof (double) * s); vx := Nil; s := 0;
end;


{ Internal routine to allocate space. If space already exists then it frees it first }
procedure TDVector.SetSize (i : integer);
begin
  if vx <> Nil then FreeMem (vx, sizeof (double) * s);
  s := i; vx := AllocMem (sizeof (double) * s);
end;


{ Increase the size of the vector without destroying and existing data }
procedure TDVector.EnLargeBy (n : integer);
begin
  if n < 0 then raise EVectorSizeError.Create ('Argument to EnLargeBy must be positive');
  ReAllocMem (vx, sizeof (double)*(s+n)); inc (s,n); { Modified for D2 }
end;


{ Reduce the size of the vector }
procedure TDVector.ReduceBy (n : integer);
begin
  if n >= s then
     raise EVectorSizeError.Create ('Can''t reduce size of vector to below zero elements');
  ReAllocMem (vx, sizeof (double)*(s-n)); dec (s,n); { modified for D2 }
end;


{ Enlarge the vector by one element without destroying any existing data }
procedure TDVector.Enlarge;
begin
  ReAllocMem (vx, sizeof (double)*(s+1)); inc (s); { Modified for D2 }
end;


{ Reduce the vector by one element, the top most element is destroyed }
procedure TDVector.Reduce;
begin
  ReAllocMem (vx, sizeof (double)*(s-1)); dec (s); { Modified for D2 }
end;


{ Clears the vector, sets all elements to zero }
procedure TDVector.Zero;
var i : integer;
begin
  for i := 1 to s do vx^[i] := 0.0;
end;


{ Clears the vector, sets all elements to zero }
procedure TDVector.Clear;
begin
  Zero;
end;


{ used internally but is also accessible from the outside }
procedure TDVector.Setval (i : integer; v : double);
begin
  vx^[i] := v;
end;


{ used internally but is also accessible from the outside }
function  TDVector.Getval (i : integer) : double;
begin
  result := vx^[i];
end;


// -------------------------------------------------------------------------
// Copies vector v, including contects to self. If self is not the same
// size as v then self is resized

//  Copy v to u:
//  Usage: u.Assign (v)
// -------------------------------------------------------------------------
procedure TDVector.Assign (v : TDVector);
begin
  Self.Copy (v);
end;


// -------------------------------------------------------------------------
// Returns number of bytes occupied by vector

//  Usage: u.SizeOf
// -------------------------------------------------------------------------
function TDVector.GetSizeOf : integer;
begin
  Result := s*SizeOf (double) + SizeOf (s) + Length (FName);
end;


// -------------------------------------------------------------------------
// Copies vector v, including contects to self. If self is not the same
// size as v then self is resized

//  Copy v to u:
//  Usage: u.Copy (v)
// -------------------------------------------------------------------------
procedure TDVector.Copy (v : TDVector);
begin
  v.Tmp := False;  { just in case its a temporary variable }
  if v.s <> Self.s then Self.SetSize (v.s);
  move (v.vx^, Self.vx^, sizeof(double) * s)
end;


// -------------------------------------------------------------------------
// Add the vectors, 'v' and 'u' together to produce Self. Error if v and u are
// the the same size. If Self is not sized correctly, then Add will resize Self

// Usage: w.Add (u, v)
// Add u to v giving result w
// -------------------------------------------------------------------------

function TDVector.Add (v, u : TDVector) : TDVector;
var i : integer;
begin
  if v.s <> u.s then
     raise EVectorSizeError.Create ('Vectors must be the same size to sum them');
  if Self.s <> v.s then Self.SetSize (v.s);
  for i := 1 to v.s do Self[i] := v[i] + u[i];
  if v.tmp then v.free; if u.tmp then u.free;
  result := Self;
end;


// -------------------------------------------------------------------------
// Add a scalar value 'k' to a vector, 'v' to produce Self.

// Usage: w.Add (u, 6)
// Add 6 to every element of v giving result w
// -------------------------------------------------------------------------
function TDVector.Add (v : TDVector; k : Double) : TDVector;
var i : integer;
begin
  for i := 1 to v.s do Self[i] := v[i] + k;
  Result := Self;
end;


// -------------------------------------------------------------------------
// Add the vectors, 'v' and 'u' together and RETURN the result. An Error
// occurs if v and u are the the same size. xAdd returns the result to the
// caller therefore it is the responsibility of the caller to dispose of the
// memory allocated by xSub. Note, the variable which is used to store the
// returned result must not have been previously allocated, otherwise you'll
// get memory leak!

// w must be unallocated
// Usage: w := Add (u, v)
// Add u to v giving result w
// -------------------------------------------------------------------------

class function TDVector.xAdd (v, u : TDVector) : TDVector;
var i : integer; t : TDVector;
begin
  if v.s <> u.s then
     raise EVectorSizeError.Create ('Vectors must be the same size to sum them');
  t := TDVector.CreateTmp (v.s);
  for i := 1 to v.s do t[i] := v[i] + u[i];
  result := t;
end;


// -------------------------------------------------------------------------
// Subtract the vectors, 'v' and 'u' together to produce Self. Error if v and u are
// the the same size. If Self is not sized correctly, then Add will resize Self

// Usage: w.Sub (u, v)
// Add u to v giving result w
// -------------------------------------------------------------------------

function TDVector.Sub (v, u : TDVector) : TDVector;
var i : integer;
begin
  if v.s <> u.s then
     raise EVectorSizeError.Create ('Vectors must be the same size to subtract them');
  if Self.s <> v.s then Self.SetSize (v.s);
  for i := 1 to v.s do Self[i] := v[i] - u[i];
  if v.tmp then v.free; if u.tmp then u.free;
  result := Self;
end;


// -------------------------------------------------------------------------
// Add a scalar value 'k' to a vector, 'v' to produce Self.

// Usage: w.Add (u, 6)
// Add 6 to every element of v giving result w
// -------------------------------------------------------------------------
function TDVector.Sub (v : TDVector; k : Double) : TDVector;
var i : integer;
begin
  for i := 1 to v.s do Self[i] := v[i] - k;
  Result := Self;
end;


// -------------------------------------------------------------------------
// Subtract the vectors, 'v' and 'u' together and RETURN the result. An Error
// occurs if v and u are the the same size. xSub returns the result to the
// caller therefore it is the responsibility of the caller to dispose of the
// memory allocated by xSub. Note, the variable which is used to store the
// returned result must not have been previously allocated, otherwise you'll
// get memory leak!

// w must be unallocated
// Usage: w := Sub (u, v)
// Add u to v giving result w
// -------------------------------------------------------------------------


class function TDVector.xSub (v, u : TDVector) : TDVector;
var i : integer; t : TDVector;
begin
  if v.s <> u.s then
     raise EVectorSizeError.Create ('Vectors must be the same size to subtract them');
  t := TDVector.CreateTmp (v.s);
  for i := 1 to v.s do t[i] := v[i] - u[i];
  result := t;
end;


// -------------------------------------------------------------------------
// Compute the dot product of vectors 'u' and 'v'
// Usage: d := TDVectord.dot (u, v);
// -------------------------------------------------------------------------
class function TDVector.Dot (u, v : TDVector) : double;
var i : integer;
begin
  if u.Size <> v.Size then
     raise EVectorSizeError.Create ('Vectors must be of the same size to compute dot product');

  result := 0.0;
  for i := 1 to u.Size do result := result + u[i]*v[i];
end;


// -------------------------------------------------------------------------
// Test whether vectors 'u' and 'v' are equal
// Usage: AreEqual := TDVectord.Equal (u, v);
// -------------------------------------------------------------------------
class function TDVector.Equal (u, v : TDVector) : boolean;
var i : integer;
begin
  Result := False;
  if u.Size = v.Size then
     begin
     Result := True;
     for i := 1 to u.Size do
         if u[i] <> v[i] then
            begin
            Result := False;
            Exit;
            end;
     end;
end;


// -------------------------------------------------------------------------
// Apply a dot product to Self and argument, 'v'
// Usage: d := u.dotU (v);
// -------------------------------------------------------------------------
function TDVector.DotU (v : TDVector) : double;
var i : integer;
begin
  if Self.Size <> v.Size then
     raise EVectorSizeError.Create ('Vectors must be of the same size to compute dot product');

  result := 0.0;
  for i := 1 to Self.Size do
      result := result + Self[i]*v[i];
end;


// -------------------------------------------------------------------------
// Compute the cross product of Self and vector 'v', replacing Self
// Usage: v.CrossU (u)
// -------------------------------------------------------------------------
function TDVector.CrossU (v : TDVector) : TDVector;
begin
  if (v.Size = 3) and (Self.Size = 3) then
     begin
     Self[1] := Self[2]*v[3] - Self[3]*v[2];
     Self[2] := Self[3]*v[1] - Self[1]*v[3];
     Self[3] := Self[1]*v[2] - Self[2]*v[1];
     result := Self;
     end
  else
     raise EVectorSizeError.Create ('Cross product can only be calculated for vectors in 3D');
end;


// -------------------------------------------------------------------------
// Compute the cross product of 'v1' and vector 'v2' giving Self
// Usage: v.Cross (v1, v2)
// -------------------------------------------------------------------------
function TDVector.Cross (v1, v2 : TDVector) : TDVector;
begin
  if (v1.Size = 3) and (v2.Size = 3) and (Self.Size = 3) then
     begin
     Self[1] := v1[2]*v2[3] - v1[3]*v2[2];
     Self[2] := v1[3]*v2[1] - v1[1]*v2[3];
     Self[3] := v1[1]*v2[2] - v1[2]*v2[1];
     result := Self;
     end
  else
     raise EVectorSizeError.Create ('Cross product can only be calculated for vectors in 3D');
end;


// Multiply the vector v by scalar k, returning result in Self
function TDVector.Mult (v : TDVector; k : Double) : TDVector;
var i : integer;
begin
  for i := 1 to s do Self[i] := k * v[i];
  Result := Self;
end;


function TDVector.Mult (v : TDVector; m : TObject) : TDVector;
var i, j : integer; AMatrix : TMatrix; Sum : Double;
begin
  Self.Zero;
  AMatrix := m as TMatrix;
  if AMatrix.r <> v.Size then
     raise EVectorSizeError.Create ('Incompatible size in Vector/Matrix multiply');
  for j := 1 to AMatrix.c do
      begin
      Sum := 0.0;
      for i := 1 to v.Size do
          Sum := Sum + v[i]*AMatrix [i, j];
      Self[j] := sum;
      end;
  Result := Self;
end;


// Matrix multiplied by vector
function TDVector.Mult (m : TObject; v : TDVector) : TDVector;
var i, j : integer; AMatrix : TMatrix;
begin
  Self.Zero;
  AMatrix := TMatrix (m);
  if AMatrix.c <> v.Size then
     raise EVectorSizeError.Create ('Incompatible sizez in Matrix/Vector multiply');
  for i := 1 to AMatrix.r do
      for j := 1 to v.Size do
          Self[i] := Self[i] + AMatrix [i, j]*v[j];
  Result := Self;
end;



// -------------------------------------------------------------------------
// Returns the vector v2 raised to the power of x and return v1
// Usage: v1.power (v2, x);
// -------------------------------------------------------------------------
function TDVector.Power (v : TDVector; x : Double) : TDVector;
var i : integer;
begin
  if Self.s <> v.s then Self.SetSize (v.s);
  for i := 1 to v.Size do
      Self[i] := Math.Power (v[i], x);
  Result := Self;
end;



// -------------------------------------------------------------------------
// Generates a range vector
// Usage: v.CreateRange (0, 10, 1)
// -------------------------------------------------------------------------
function TDVector.CreateRange (Start, Stop, Step : double) : TDVector;
var Count : integer; x : Double;
begin
  Self.FreeSpace;

  x := Start;
  Count := 1;
  repeat
    Enlarge;
    Self[Count] := x;
    x := x + Step;
    Inc (Count);
  until abs (x) > abs (Stop);
  Result := Self;
end;


// -------------------------------------------------------------------------
// Returns the sum of values in the vector
// Usage: total := v.sum
// -------------------------------------------------------------------------
function TDVector.Sum : double;
var i : integer;
begin
  result := 0.0;
  for i := 1 to s do result := result + vx^[i];
end;


// -------------------------------------------------------------------------
// Returns the minimum value in the vector
// Usage: m := v.min
// -------------------------------------------------------------------------
function TDVector.Min : double;
var i : integer;
begin
  if s > 0 then
     Result := vx^[1]
  else Result := 0;

  for i := 1 to s do
      if vx^[i] < Result then
         Result := vx^[i];
end;


// -------------------------------------------------------------------------
// Returns the maximum value in the vector
// Usage: m := v.max
// -------------------------------------------------------------------------
function TDVector.Max : double;
var i : integer;
begin
  if s > 0 then
     Result := vx^[1]
  else Result := 0;

  for i := 1 to s do
      if vx^[i] > Result then
         Result := vx^[i];
end;


// -------------------------------------------------------------------------
// Returns the product of values in the vector
// Usage: product := v.prod
// -------------------------------------------------------------------------
function TDVector.Prod : double;
var i : integer;
begin
  result := 1.0;
  for i := 1 to s do result := Result * vx^[i];
end;


// -------------------------------------------------------------------------
// Returns the mean of the elements of the vector
// Usage: average := v.mean;
// -------------------------------------------------------------------------
function TDVector.Mean : double;
begin
  if s > 0 then result := sum / s
  else raise Exception.Create ('Vector must have at least one element to compute mean');
end;


// -------------------------------------------------------------------------
// Returns the sum of the squares of values in Data
// Usage: s := v.SumOfSquares;
// -------------------------------------------------------------------------
function TDVector.SumOfSquares : double;
var i : integer;
begin
  result := 0.0;
  for i := 1 to s do result := result + sqr(vx^[i]);
end;


// -------------------------------------------------------------------------
// Returns the Euclidean norm of the Self vector
// -------------------------------------------------------------------------
function TDVector.Norm : double;
begin
  result := sqrt (Self.SumOfSquares);
end;


// -------------------------------------------------------------------------
// Returns the sample standard deviation
// Usage: sd := v.StdDev;
// -------------------------------------------------------------------------
function TDVector.StdDev : double;
var sq, total : double; i : integer;
begin
  sq := 0; total := 0;
  if s > 1 then
     begin
     for i := 1 to s do
         begin sq := sq + sqr(vx^[i]); total := total + vx^[i]; end;
     result := sqrt ((sq - sqr(total)/s)/(s-1));
     // The following code is easier to read but slightly slower in execution:
     // result := sqrt ((SumOfSquares - sqr (sum)/s)/(s-1));}
     end
  else
     raise Exception.Create ('Can''t calculate stddev for vector with one or no elements');
end;


// -------------------------------------------------------------------------
// Applies the function f to all element of vector Self
// Usage: v.ApplyUnaryFunction (MyFunction)
// -------------------------------------------------------------------------
procedure TDVector.ApplyUnaryFunction (const f : TUnaryDoubleFunc);
var i : integer;
begin
  for i := 1 to s do
      vx^[i] := f (vx^[i]);
end;


// -------------------------------------------------------------------------
// Applies the function f, using args x and y to all element of vector Self
// Usage: v.ApplyBinaryFunction (MyFunction, arg1, arg2)
// -------------------------------------------------------------------------
procedure TDVector.ApplyBinaryFunction (f : TBinaryDoubleFunc; x, y : Double);
var i : integer;
begin
  for i := 1 to s do
      vx^[i] := f (x, y);
end;


// -------------------------------------------------------------------------
// Scale the vector by factor
// Usage: v.Scale (2)   Multiplies all elements by 2
// -------------------------------------------------------------------------
procedure TDVector.Scale (factor : double);
var i : integer;
begin
  for i := 1 to s do vx^[i] := vx^[i]*factor;
end;


// -------------------------------------------------------------------------
// Write vector to a string
// Usage: v.SaveToString
// -------------------------------------------------------------------------
function TDVector.SaveToString : string;
var i : integer;
begin
  Result := '';
  for i := 1 to s do
      Result := Result + Format ('%g ', [vx^[i]]);
end;


procedure TDVector.SaveToStream (st : TStream);
begin
  st.write (s, SizeOf (s));
  st.write (vx^, sizeof (double) * s);
end;


// Usage: v := TDVectord.Read (st);
constructor TDVector.LoadFromStream (st : TStream);
var c : integer;
begin
  inherited Create;
  st.read (c, SizeOf (integer));
  SetSize (c);
  st.read (vx^, SizeOf (double) * c);
end;



{ ------------------------------------------------------------------------- }
{                         START OF INTEGER VECTOR IMPLEMETATION             }
{ ------------------------------------------------------------------------- }


{ Create a vector of size i }
constructor TIVector.create(i : integer);
begin
  Inherited Create; vx := Nil;
  Self.SetSize (i);
  FName := 'untitled';
end;


destructor TIVector.destroy;
begin
  if vx <> Nil then FreeMem (vx, sizeof (integer) * s);
  Inherited Destroy;
end;


{ ******************************************************************** }
{ Usage: u := TIVector.Clone (v)                                        }
{ ******************************************************************** }
constructor TIVector.Clone (v : TIVector);
begin
  Inherited Create;
  Self.SetSize (v.s);
  Self.Copy (v);
end;


{ Internal routine used by define }
procedure TIVector.SetSize (NewSize : integer);
begin
  if vx <> Nil then FreeMem (vx, sizeof (integer) * s);
  s := NewSize; vx := AllocMem (sizeof (integer) * NewSize);
end;


procedure TIVector.EnLargeBy (n : integer);
begin
  ReAllocMem (vx, sizeof (integer)*(s+n)); inc (s,n); { Modified for D2 }
end;


procedure TIVector.ReduceBy (n : integer);
begin
  if n >= s then
     raise EVectorSizeError.Create ('Can''t reduce size of vector to below zero elements');
  ReAllocMem (vx, sizeof (integer)*(s-n)); dec (s,n);  { Modified for D2 }
end;


{ Enlarge the vector by one element without destroying any existing data }
procedure TIVector.Enlarge;
begin
  ReAllocMem (vx, sizeof (integer)*(s+1)); inc (s); { Modified for D2 }
end;


{ Reduce the vector by one element, the top most element is destroyed }
procedure TIVector.Reduce;
begin
  ReAllocMem (vx, sizeof (integer)*(s-1)); dec (s); { Modified for D2 }
end;


{ Clear the vector, sets all elements to zero }
procedure TIVector.Zero;
var i : integer;
begin
  for i := 1 to s do vx^[i] := 0;
end;


{ Clear the vector, sets all elements to zero }
procedure TIVector.Clear;
begin
  Zero;
end;


// -------------------------------------------------------------------------
// Copies vector v, including contects to self. If self is not the same
// size as v then self is resized

//  Copy v to u:
//  Usage: u.Copy (v)
// -------------------------------------------------------------------------
procedure TIVector.Copy (v : TIVector);
begin
  if v.s <> Self.s then Self.SetSize (v.s);
  move (v.vx^, Self.vx^, SizeOf(Integer) * s)
end;


procedure TIVector.Assign (v : TIVector);
begin
  if v.s <> Self.s then Self.SetSize (v.s);
  move (v.vx^, Self.vx^, sizeof(integer) * s)
end;


{ used internally but is also accessible from the outside }
procedure TIVector.Setval (i : integer; v : integer);
begin
  vx^[i] := v;
end;


{ used internally but is also accessible from the outside }
function  TIVector.Getval (i : integer) : integer;
begin
  result := vx^[i];
end;


function TIVector.GetSize : integer;
begin
  result := s;
end;


end.
