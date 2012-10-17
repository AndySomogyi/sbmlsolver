unit uComplex;

// Complex arithmetic routines

interface

uses Classes, SysUtils, Math;

type
   TComplex = record r, i : double; end;

function complex (r, i : double) : TComplex;

function getComplexRealPart (const z : TComplex) : double;
function getComplexImagPart (const z : TComplex) : double;

function complexAdd (a, b : TComplex) : TComplex;
function complexSub (a, b : TComplex) : TComplex;
function complexMult (a, b : TComplex) : TComplex;
function complexDiv (a, b : TComplex) : TComplex;
function complexNeg (a : TComplex) : TComplex;
function complexPow (a, b : TComplex) : TComplex;
function complexSqrt (const z : TComplex) : TComplex;
function complexSqr  (const z : TComplex) : TComplex;
function complexAbs  (const z : TComplex) : double;

function complexSin (const z : TComplex) : TComplex;
function complexCos (const z : TComplex) : TComplex;
function complexTan (const z : TComplex) : TComplex;
function complexExp (const z : TComplex) : TComplex;
function complexLn  (const z : TComplex) : TComplex;
function complexLog10 (const z : TComplex) : TComplex;
function complexLog2 (const z : TComplex) : TComplex;

function complexLessThan (a, b : TComplex) : boolean;
function complexGreaterThan (a, b : TComplex) : boolean;
function complexLessThanOrEqual (a, b : TComplex) : boolean;
function complexGreaterThanOrEqual (a, b : TComplex) : boolean;

function complexNotEquals (a, b : TComplex) : boolean;
function complexEquals (a, b : TComplex) : boolean;

implementation


function complex (r, i : double) : TComplex;
begin
  result.r := r; result.i := i;
end;


function getComplexRealPart (const z : TComplex) : double;
begin
  result := z.r;
end;


function getComplexImagPart (const z : TComplex) : double;
begin
  result := z.i;
end;


function complexAdd (a, b : TComplex) : TComplex;
begin
  result.r := a.r + b.r;
  result.i := a.i + b.i;
end;


function complexSub (a, b : TComplex) : TComplex;
begin
  result.r := a.r - b.r;
  result.i := a.i - b.i;
end;


function complexMult (a, b : TComplex) : TComplex;
begin
  result.r := (a.r * b.r) - (a.i * b.i);
  result.i := (a.r * b.i) + (a.i * b.r);
end;


function complexDiv (a, b : TComplex) : TComplex;
var abs_breal, abs_bimag : double; ratio, denom : double;
begin
  // Taken from Python code, 2.1
  // This algorithm is better, and is pretty obvious:  first divide the
  // numerators and denominator by whichever of {b.real, b.imag} has
  // larger magnitude.  The earliest reference I found was to CACM
  // Algorithm 116 (Complex Division, Robert L. Smith, Stanford
  // University).  As usual, though, we're still ignoring all IEEE
  // endcases.

  if b.r < 0 then abs_breal := -b.r else abs_breal := b.r;
  if b.i < 0 then abs_bimag := -b.i else abs_bimag := b.i;

  if (abs_breal >= abs_bimag) then
     begin
     // divide tops and bottom by b.real
     if (abs_breal = 0.0) then
        raise Exception.Create ('Floating point divide by zero in complex divide')  // Doesn't return if successful
     else
        begin
  	ratio := b.i / b.r;
  	denom := b.r + b.i * ratio;
  	result.r := (a.r + a.i * ratio) / denom;
  	result.i := (a.i - a.r * ratio) / denom;
  	end
     end
  else
     begin
     // divide tops and bottom by b.imag
     ratio := b.r / b.i;
     denom := b.r * ratio + b.i;
     //assert(b.imag != 0.0);
     result.r := (a.r * ratio + a.i) / denom;
     result.i := (a.i * ratio - a.r) / denom;
     end;
end;


function complexNeg (a : TComplex) : TComplex;
begin
  result.r := -a.r;
  result.i := -a.i;
end;


function complexPow(a, b : TComplex) : TComplex;
var vabs, len, at, phase : double;
begin
  if (b.r = 0.0) and (b.i = 0.0) then
     begin
     result.r := 1.0;
     result.i := 0.0;
     end
  else if (a.r = 0.0) and (a.i = 0.0) then
          begin
          if (b.i <> 0.0) or (b.r < 0.0) then
             raise Exception.Create ('Floating point range error in complex power');  // Doesn't return if successful
          end
  else  begin
        vabs := hypot(a.r, a.i);
        len := power(vabs, b.r);
        at := arctan2(a.i, a.r);
        phase := at*b.r;
	if (b.i <> 0.0) then
           begin
	   len := len / exp(at*b.i);
	   phase := phase + b.i*ln(vabs);
	   end;
	result.r := len*cos(phase);
	result.i := len*sin(phase);
	end;
end;


function complexSqr (const z : TComplex) : TComplex;
begin
  result.r := (z.r * z.r) - (z.i * z.i);
  result.i := (z.r * z.i) + (z.i * z.r);
end;


function complexSqrt (const z : TComplex) : TComplex;
var r : TComplex; mag : double;
begin
  //
  // Complex square root.
  //
  // This function was adapted from the f2c "z_sqrt" function.
  // Copyright 1990 by AT&T Bell Laboratories and Bellcore.
  //

  mag := complexAbs (z);
  if mag = 0.0 then
     begin
     r.r := 0.0;
     r.i := 0.0;
     end
  else if (z.r > 0.0) then
     begin
     r.r := sqrt (0.5 * (mag + z.r));
     r.i := z.i / r.r / 2.0;
     end
  else
    begin
      r.i := sqrt (0.5 * (mag - z.r));
      if (z.i < 0.0) then
	r.i := -r.i;
      r.r := z.i / r.i / 2.0;
    end;
  result := r;
end;


function complexAbs (const z : TComplex) : double;
var r, i, temp : double;
begin
  //
  // Complex magnitude.
  //
  // This function was adapted from the f2c "z_abs" function.
  // Copyright 1990 by AT&T Bell Laboratories and Bellcore.
  //

  r := z.r;
  i := z.i;

  if (r < 0) then r := -r;
  if (i < 0) then i := -i;
  if (i > r) then
     begin
     temp := r;
     r := i;
     i := temp;
     end;

  if ((r + i) = r) then
     begin
     result := r;
     exit;
     end;

  temp := i / r;
  temp := r * sqrt (1.0 + temp * temp);

  result := temp;
end;


function complexSin (const z : TComplex) : TComplex;
begin
  result.r := sin (z.r) * cosh (z.i);
  result.i := cos (z.r) * sinh (z.i);
end;

function complexCos (const z : TComplex) : TComplex;
begin
  result.r := cos (z.r) * cosh (z.i);
  result.i := -sin (z.r) * sinh (z.i);
end;

function complexTan (const z : TComplex) : TComplex;
begin
  result.r := 1.0 / (cos (2 * z.r) + cosh (2 * z.i));
  result.i := result.r;
  result.r := result.r * sin (2 * z.r);
  result.i := result.i * sinh (2 * z.i);
end;

function complexExp (const z : TComplex) : TComplex;
var r : double;
begin
  r := exp(z.r);
  result.r := cos(z.i)*r;
  result.i := sin(z.i)*r;
end;


function complexATan (z : TComplex) : TComplex;
var w : TComplex;
begin
  // atan(z) = i*log((i+z)/(i-z))/2
  //         = i*log(-((x^2+y^2-1)+i*(2*x))/(x^2+y^2-2*y+1))/2

  result.r := -1.0 / (z.r * z.r + z.i * z.i - 2 * z.i + 1.0);
  result.i := result.r;

  result.r := result.r * (z.r * z.r + z.i * z.i - 1.0);
  result.i := result.i * (2 * z.r);

  z := complexLn (result);

  result.r := -0.5 * z.i;
  result.i := 0.5 * z.r;
end;


function complexLn (const z : TComplex) : TComplex;
begin
  result.r := Ln (sqrt (z.r*z.r + z.i*z.i));
  result.i := ArcTan2 (z.i, z.r);
end;


// Compute Log to the base n
function complexLog_n (const z : TComplex; n : double) : TComplex;
begin
  result := complexDiv (complexLn(z), complex (Ln(n), 0.0));
end;

// Uses: log(x)/log(10)
function complexLog10 (const z : TComplex) : TComplex;
begin
  result := complexLog_n (z, 10.0);
end;

// Uses: log(x)/log(a);
function complexLog2 (const z : TComplex) : TComplex;
begin
  result := complexLog_n (z, 2.0);
end;


// -----------------------------------------------------------------


function complexLessThan (a, b : TComplex) : boolean;
begin
  result := ((a.r*a.r+a.i*a.i) < (b.r*b.r+b.i*b.i));
end;


function complexGreaterThan (a, b : TComplex) : boolean;
begin
  result := ((a.r*a.r+a.i*a.i) > (b.r*b.r+b.i*b.i));
end;


function complexLessThanOrEqual (a, b : TComplex) : boolean;
begin
  result := ((a.r*a.r+a.i*a.i) <= (b.r*b.r+b.i*b.i));
end;


function complexGreaterThanOrEqual (a, b : TComplex) : boolean;
begin
  result := ((a.r*a.r+a.i*a.i) >= (b.r*b.r+b.i*b.i));
end;


function complexNotEquals (a, b : TComplex) : boolean;
begin
  result := (a.r <> b.r) and (a.i <> b.i);
end;


function complexEquals (a, b : TComplex) : boolean;
begin
  result := (a.r = b.r) and (a.i = b.i);
end;


end.
