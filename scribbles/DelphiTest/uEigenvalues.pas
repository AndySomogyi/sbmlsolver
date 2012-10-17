unit uEigenvalues;

interface

uses SysUtils, uJVector, uMatrix;

procedure EigenVals (a : TMatrix; wr, wi : TDVector);
//procedure Test (a : TMatrix);

Implementation

procedure balanc(var a : TMatrix; n: integer);
(* Programs using routine BALANC should define the type
TYPE
   glnpnp = ARRAY [1..np,1..np] OF real;
where 'np by np' is the physical dimension of the array to be analyzed. *)
CONST
   radix=2.0;
VAR
   last,j,i: integer;
   s,r,g,f,c,sqrdx, x: double;
BEGIN
   sqrdx := sqr(radix);
   REPEAT
      last := 1;
      FOR i := 1 TO n DO BEGIN
         c := 0.0;
         r := 0.0;
         FOR j := 1 TO n DO
            IF (j <> i) THEN BEGIN
               c := c+abs(a[j,i]);
               r := r+abs(a[i,j])
            END;
         IF ((c <> 0.0) AND (r <> 0.0)) THEN BEGIN
            g := r/radix;
            f := 1.0;
            s := c+r;
            WHILE (c < g) DO BEGIN
               f := f*radix;
               c := c*sqrdx
            END;
            g := r*radix;
            WHILE (c > g) DO BEGIN
               f := f/radix;
               c := c/sqrdx
            END;
            IF ((c+r)/f < 0.95*s) THEN BEGIN
               last := 0;
               g := 1.0/f;
               for j := 1 to n do a[i,j] := a[i,j]*g;
               for j := 1 to n do a[j,i] := a[j,i]*f;
            END
         END
      END;
   UNTIL (last <> 0)
END;


PROCEDURE elmhes(VAR a: TMatrix; n: integer);
(* Programs using routine ELMHES must define the type
TYPE
   glnpnp = ARRAY [1..np,1..np]
where 'np by np' is the physical dimension of the matrix to be reduced. *)
VAR
   m,j,i: integer;
   y,x: real;
BEGIN
   IF (n > 2) THEN BEGIN
      FOR m := 2 TO n-1 DO BEGIN
         x := 0.0;
         i := m;
         FOR j := m TO n DO BEGIN
            IF (abs(a[j,m-1]) > abs(x)) THEN BEGIN
               x := a[j,m-1];
               i := j
            END
         END;
         IF (i <> m) THEN BEGIN
            FOR j := m-1 TO n DO BEGIN
               y := a[i,j];
               a[i,j] := a[m,j];
               a[m,j] := y;
            END;
            FOR j := 1 TO n DO BEGIN
               y := a[j,i];
               a[j,i] := a[j,m];
               a[j,m] := y;
            end
         END;
         IF (x <> 0.0) THEN BEGIN
            FOR i := m+1 TO n DO BEGIN
               y := a[i,m-1];
               IF (y <> 0.0) THEN BEGIN
                  y := y/x;
                  a[i,m-1] := y;
                  FOR j := m TO n DO a[i,j] := a[i,j]-y*a[m,j];
                  FOR j := 1 TO n DO a[j,m] := a[j,m]+y*a[j,i];
               END
            END
         END
      END
   END
END;


PROCEDURE hqr(VAR a: TMatrix; n: integer; VAR wr, wi: TDVector);
(* Programs using routine HQR must define the type
TYPE
   glnpnp = ARRAY [1..np,1..np] OF real;
   glnp = ARRAY [1..np] OF real;
where 'np by np' is the physical dimension of the matrix whose
eigenvalues are to be found. *)
LABEL 2,3,4;
VAR
   nn,m,l,k,j,its,i,mmin: integer;
   z,y,x,w,v,u,t,s,r,q,p,anorm: real;
FUNCTION sign(a,b: real): real;
   BEGIN
      IF (b < 0.0) THEN sign := -abs(a) ELSE sign := abs(a)
   END;
FUNCTION min(a,b: integer): integer;
   BEGIN
      IF (a < b) THEN min := a ELSE min := b
   END;
BEGIN
   anorm := abs(a[1,1]);
   FOR i := 2 TO n DO BEGIN
      FOR j := i-1 TO n DO BEGIN
         anorm := anorm+abs(a[i,j])
      END
   END;
   nn := n;
   t := 0.0;
   WHILE (nn >= 1) DO BEGIN
      its := 0;
2:      FOR l := nn DOWNTO 2 DO BEGIN
         s := abs(a[l-1,l-1])+abs(a[l,l]);
         IF (s = 0.0) THEN s := anorm;
         IF ((abs(a[l,l-1])+s) = s) THEN GOTO 3
      END;
      l := 1;
3:    x := a[nn,nn];
      if (l = nn) then
         begin
         wr[nn] := x+t;
         wi[nn] := 0.0;
         nn := nn-1
         end
      ELSE BEGIN
         y := a[nn-1,nn-1];
         w := a[nn,nn-1]*a[nn-1,nn];
         IF (l = nn-1) THEN BEGIN
            p := 0.5*(y-x);
            q := sqr(p)+w;
            z := sqrt(abs(q));
            x := x+t;
            IF (q >= 0.0) THEN BEGIN
               z := p+sign(z,p);
               wr[nn] := x+z;
               wr[nn-1] := wr[nn];
               IF (z <> 0.0) THEN wr[nn] := x-w/z;
               wi[nn] := 0.0;
               wi[nn-1] := 0.0
            END ELSE BEGIN
               wr[nn] := x+p;
               wr[nn-1] := wr[nn];
               wi[nn] := z;
               wi[nn-1] := -z
            END;
            nn := nn-2
         END ELSE BEGIN
            IF (its = 30) THEN BEGIN
               writeln('pause in routine HQR');
               writeln('too many iterations'); readln
            END;
            IF ((its = 10) OR (its = 20)) THEN BEGIN
               t := t+x;
               FOR i := 1 TO nn DO BEGIN
                  a[i,i] := a[i,i]-x;
               END;
               s := abs(a[nn,nn-1])+abs(a[nn-1,nn-2]);
               x := 0.75*s;
               y := x;
               w := -0.4375*sqr(s)
            END;
            its := its+1;
            FOR m := nn-2 DOWNTO l DO BEGIN
               z := a[m,m];
               r := x-z;
               s := y-z;
               p := (r*s-w)/a[m+1,m]+a[m,m+1];
               q := a[m+1,m+1]-z-r-s;
               r := a[m+2,m+1];
               s := abs(p)+abs(q)+abs(r);
               p := p/s;
               q := q/s;
               r := r/s;
               IF (m = l) THEN GOTO 4;
               u := abs(a[m,m-1])*(abs(q)+abs(r));
               v := abs(p)*(abs(a[m-1,m-1])+abs(z)
                     +abs(a[m+1,m+1]));
               IF ((u+v) = v) THEN GOTO 4
            END;
4:            FOR i := m+2 TO nn DO BEGIN
               a[i,i-2] := 0.0;
               IF  (i <> (m+2)) THEN a[i,i-3] := 0.0;
            END;
            FOR k := m TO nn-1 DO BEGIN
               IF (k <> m) THEN BEGIN
                  p := a[k,k-1];
                  q := a[k+1,k-1];
                  r := 0.0;
                  IF (k <> (nn-1)) THEN
                     r := a[k+2,k-1];
                  x := abs(p)+abs(q)+abs(r);
                  IF (x <> 0.0) THEN BEGIN
                     p := p/x;
                     q := q/x;
                     r := r/x
                  END
               END;
               s := sign(sqrt(sqr(p)+sqr(q)+sqr(r)),p);
               IF (s <> 0.0) THEN BEGIN
                  IF (k = m) THEN BEGIN
                     IF (l <> m) THEN
                       a[k,k-1] := -a[k,k-1];
                  END ELSE BEGIN
                     a[k,k-1] := -s*x;
                  END;
                  p := p+s;
                  x := p/s;
                  y := q/s;
                  z := r/s;
                  q := q/p;
                  r := r/p;
                  FOR j := k TO nn DO BEGIN
                     p := a[k,j]+q*a[k+1,j];
                     IF (k <> (nn-1)) THEN BEGIN
                        p := p+r*a[k+2,j];
                        a[k+2,j] := a[k+2,j]-p*z;
                     END;
                     a[k+1,j] := a[k+1,j]-p*y;
                     a[k,j] := a[k,j]-p*x;
                  END;
                  mmin := min(nn,k+3);
                  FOR i := l TO mmin DO BEGIN
                     p := x*a[i,k]+y*a[i,k+1];
                     IF (k <> (nn-1)) THEN BEGIN
                        p := p+z*a[i,k+2];
                        a[i,k+2] := a[i,k+2]-p*r;
                     END;
                     a[i,k+1] := a[i,k+1]-p*q;
                     a[i,k] := a[i,k]-p;
                  END
               END
            END;
            GOTO 2
         END
      END
   END
END;



procedure EigenVals (a : TMatrix; wr, wi : TDVector);
begin
   balanc(a, a.r);
   elmhes(a, a.r);
   hqr(a, a.r, wr, wi);
end;

{procedure Test (a : TMatrix);
var i, j : integer; str : string; wr, wi : TDVector;
begin
   wr := TDVector.Create (a.r);
   wi := TDVector.Create (a.r);

   a[1,1] := 1.0; a[1,2] := 2.0; a[1,3] := 0.0;
   a[1,4] := 0.0; a[1,5] := 0.0;
   a[2,1] := -2.0; a[2,2] := 3.0; a[2,3] := 0.0;
   a[2,4] := 0.0; a[2,5] := 0.0;
   a[3,1] := 3.0; a[3,2] := 4.0; a[3,3] := 50.0;
   a[3,4] := 0.0; a[3,5] := 0.0;
   a[4,1] := -4.0; a[4,2] := 5.0; a[4,3] := -60.0;
   a[4,4] := 7.0; a[4,5] := 0.0;
   a[5,1] := -5.0; a[5,2] := 6.0; a[5,3] := -70.0;
   a[5,4] := 8.0; a[5,5] := -9.0;
   frmMain.memo1.Lines.Add ('matrix:');
   FOR i := 1 to a.r DO
      BEGIN
      str := '';
      FOR j := 1 to a.c do
          begin
          str := str + ' ' + floattostr (a[i,j]);
          end;
      frmMain.Memo1.Lines.Add (str);
      frmMain.memo1.Lines.Add ('')
   END;
   balanc(a, a.r);
   elmhes(a, a.r);
   hqr(a, a.r, wr, wi);
   frmMain.Memo1.Lines.Add ('eigenvalues:');
   frmMain.Memo1.Lines.Add ('real    ' + 'imag.    ');
   FOR i := 1 to a.r DO
      BEGIN
      frmmain.memo1.Lines.Add ('  ' + floattostr (wr[i]) + '  ' + floattostr (wi[i]))
   end;
   wr.Free;
   wi.Free;
end;}


end.


