/* linalg_nleq2.f -- translated by f2c (version 20100827).
   You must link the resulting object file with libf2c:
	on Microsoft Windows system, link with libf2c.lib;
	on Linux or Unix systems, link with .../path/to/libf2c.a -lm
	or, if you install libf2c.a in a standard place, with -lf2c -lm
	-- in that order, at the end of the command line, as in
		cc *.o -lf2c -lm
	Source for libf2c is in /netlib/f2c/libf2c.zip, e.g.,

		http://www.netlib.org/f2c/libf2c.zip
*/

#include "f2c_nleq.h"


/* *    Group  Linear Solver subroutines (Code DECCON/SOLCON) */

/* Subroutine */ int deccon_(doublereal *a, integer *nrow, integer *ncol, 
	integer *mcon, integer *m, integer *n, integer *irankc, integer *
	irank, doublereal *cond, doublereal *d__, integer *pivot, integer *
	kred, doublereal *ah, doublereal *v, integer *ierr)
{
    /* System generated locals */
    integer a_dim1, a_offset, ah_dim1, ah_offset, i__1, i__2, i__3;
    doublereal d__1, d__2;

    /* Builtin functions */
    double sqrt(doublereal);
    /* Subroutine */ int s_stop(char *, ftnlen);
    double d_sign(doublereal *, doublereal *);

    /* Local variables */
    extern /* Subroutine */ int zibconst_(doublereal *, doublereal *);
    static doublereal h__;
    static integer i__, j, k;
    static doublereal s, t;
    static integer i1, k1, l1;
    static doublereal s1, dd;
    static integer jd, ii, jj, mh;
    static doublereal sh;
    static integer irk1;
    static doublereal hmax;
    static integer idata, level;
    static doublereal small;
    static integer iranc1;
    static doublereal epmach;
    static integer irankh;

/* *    Begin Prologue DECCON */
/*     ------------------------------------------------------------ */

/* *  Title */

/* *    Deccon - Constrained Least Squares QR-Decomposition */

/* *  Written by        P. Deuflhard, U. Nowak, L. Weimann */
/* *  Purpose           Solution of least squares problems, optionally */
/*                     with equality constraints. */
/* *  Method            Constrained Least Squares QR-Decomposition */
/*                     (see references below) */
/* *  Category          D9b1. -  Singular, overdetermined or */
/*                              underdetermined systems of linear */
/*                              equations, generalized inverses. */
/*                              Constrained Least Squares solution */
/* *  Keywords          Linear Least Square Problems, constrained, */
/*                     QR-decomposition, pseudo inverse. */
/* *  Version           1.3 */
/* *  Revision          December 1993 */
/* *  Latest Change     August 2006 */
/* *  Library           CodeLib */
/* *  Code              Fortran 77, Double Precision */
/* *  Environment       Standard Fortran 77 environment on PC's, */
/*                     workstations and hosts. */
/* *  Copyright     (c) Konrad-Zuse-Zentrum fuer */
/*                     Informationstechnik Berlin (ZIB) */
/*                     Takustrasse 7, D-14195 Berlin-Dahlem */
/*                     phone : + 49/30/84185-0 */
/*                     fax   : + 49/30/84185-125 */
/* *  Contact           Bodo Erdmann */
/*                     ZIB, Division Scientific Computing, */
/*                          Department Numerical Analysis and Modelling */
/*                     phone : + 49/30/84185-185 */
/*                     fax   : + 49/30/84185-107 */
/*                     e-mail: erdmann@zib.de */

/* *    References: */
/*     =========== */

/*       /1/ P.Deuflhard, V.Apostolescu: */
/*           An underrelaxed Gauss-Newton method for equality */
/*           constrained nonlinear least squares problems. */
/*           Lecture Notes Control Inform. Sci. vol. 7, p. */
/*           22-32 (1978) */
/*       /2/ P.Deuflhard, W.Sautter: */
/*           On rank-deficient pseudoinverses. */
/*           J. Lin. Alg. Appl. vol. 29, p. 91-111 (1980) */

/* *    Related Programs:     SOLCON */

/*  --------------------------------------------------------------- */

/* * Licence */
/*    You may use or modify this code for your own non commercial */
/*    purposes for an unlimited time. */
/*    In any case you should not deliver this code without a special */
/*    permission of ZIB. */
/*    In case you intend to use the code commercially, we oblige you */
/*    to sign an according licence agreement with ZIB. */

/* * Warranty */
/*    This code has been tested up to a certain level. Defects and */
/*    weaknesses, which may be included in the code, do not establish */
/*    any warranties by ZIB. ZIB does not take over any liabilities */
/*    which may follow from acquisition or application of this code. */

/* * Software status */
/*    This code is under care of ZIB and belongs to ZIB software class 1. */

/*     ------------------------------------------------------------ */

/* *    Summary: */
/*     ======== */
/*     Constrained QR-decomposition of (M,N)-system  with */
/*     computation of pseudoinverse in case of rank-defeciency . */
/*     First MCON rows belong to equality constraints. */

/*     ------------------------------------------------------------ */

/* *    Parameters list description (* marks inout parameters) */
/*     ====================================================== */

/* *    Input parameters */
/*     ================ */

/*       A(NROW,NCOL) Dble   Array holding the (M,N)-Matrix to be */
/*                           decomposed */
/*       NROW         Int    Declared number of rows of array A */
/*       NCOL         Int    Declared number of columns of array A and */
/*                           rows and columns of array AH */
/*       MCON         Int    Number of equality constraints (MCON.LE.N) */
/*                           Internally reduced if equality constraints */
/*                           are linearly dependent */
/*       M            Int    Current number of rows of matrix A */
/*       N            Int    Current number of columns of matrix A */
/*     * IRANKC       Int    Prescribed maximum pseudo-rank of */
/*                           constrained part of matrix A (IRANKC.LE.MCON) */
/*     * IRANK        Int    Prescribed maximum pseudo-rank of matrix A */
/*                           (IRANK.LE.N) */
/*     * COND         Dble   Permitted upper bound for the subcondition */
/*                           of the least squares part of A, .i.e. */
/*                           DABS(D(IRANKC+1)/D(IRANK)) */
/*       KRED         Int    Type of operation */
/*                           >=0  Householder triangularization */
/*                                (build up pseudo-inverse,if IRANK.LT.N) */
/*                           < 0  Reduction of pseudo-rank of matrix A, */
/*                                skipping Householder triangularization, */
/*                                 build-up new pseudo-inverse */

/* *    Output parameters */
/*     ================= */

/*       A(NROW,NCOL)  Dble   Array holding the (M,N)-output consisting */
/*                            of the transformed matrix in the upper */
/*                            right triangle and the performed House- */
/*                            holder transf. in the lower left triangle. */
/*     * IRANKC        Int    New pseudo-rank of constrained part of */
/*                            matrix A, determined so that */
/*                            DABS(D(1)/D(IRANKC))<1/EPMACH */
/*     * IRANK         Int    New pseudo-rank of matrix A, determined */
/*                            so that DABS(D(IRANKC+1)/D(IRANK)) < COND */
/*       D(IRANK)      Dble   Diagonal elements of upper triangular matr. */
/*       PIVOT(N)      Int    Index vector storing permutation of columns */
/*                            due to pivoting */
/*     * COND          Dble   The sub-condition number belonging to the */
/*                            least squares part of A. */
/*                            (in case of rank reduction: */
/*                             sub-condition number which led to */
/*                             rank reduction) */
/*                            COND=0 indicates COND=infinity */
/*       AH(NCOL,NCOL) Dble   In case of rank-defect used to compute the */
/*                            pseudo-inverse (currently used will be an */
/*                            (N,N)-part of this array) */
/*       V(N)          Dble   V(1) holds on output the sub-condition */
/*                            number belonging to the constrained part */
/*                            of A. */
/*       IERR          Int    Error indicator: */
/*                            = 0 : DECCON computations are successfull. */
/*                            =-2 : Numerically negative diagonal element */
/*                                  encountered during computation of */
/*                                  pseudo inverse - due to extremely bad */
/*                                  conditioned Matrix A. DECCON is */
/*                                  unable to continue rank-reduction. */

/* *    Workspace parameters */
/*     ==================== */

/*       V(N)         Dble   Workspace array */

/* *    Subroutines called: ZIBCONST */

/* *    Machine constants used */
/*     ====================== */

/*     EPMACH = relative machine precision */

/*     ------------------------------------------------------------ */
/* *    End Prologue */
/* *    Begin */
/*     -------------------------------------------------------------- */
/*     1 Initialization */
    /* Parameter adjustments */
    --v;
    ah_dim1 = *ncol;
    ah_offset = 1 + ah_dim1;
    ah -= ah_offset;
    --pivot;
    --d__;
    a_dim1 = *nrow;
    a_offset = 1 + a_dim1;
    a -= a_offset;

    /* Function Body */
    zibconst_(&epmach, &small);
    if (*irank > *n) {
	*irank = *n;
    }
    if (*irank > *m) {
	*irank = *m;
    }
/*     -------------------------------------------------------------- */
/*     1.1 Special case M=1 and N=1 */
    if (*m == 1 && *n == 1) {
	pivot[1] = 1;
	d__[1] = a[a_dim1 + 1];
	*cond = 1.;
	return 0;
    }
    if (*kred >= 0) {
/*       ------------------------------------------------------------ */
/*       1.1 Initialize pivot-array */
	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	    pivot[j] = j;
/* L11: */
	}
/*       ------------------------------------------------------------ */
/*       2. Constrained Householder triangularization */
	jd = 1;
	iranc1 = *irankc + 1;
	mh = *mcon;
	irankh = *irankc;
	idata = 0;
	if (mh == 0) {
	    irankh = *irank;
	    mh = *m;
	    idata = 1;
	}
	irk1 = *irank;
	i__1 = irk1;
	for (k = 1; k <= i__1; ++k) {
L2000:
	    level = 1;
	    if (k != *n) {
		k1 = k + 1;
/*            DO (Until) */
L20:
		if (jd != 0) {
		    i__2 = *n;
		    for (j = k; j <= i__2; ++j) {
			s = 0.;
			i__3 = mh;
			for (l1 = k; l1 <= i__3; ++l1) {
/* Computing 2nd power */
			    d__1 = a[l1 + j * a_dim1];
			    s += d__1 * d__1;
/* L2011: */
			}
			d__[j] = s;
/* L201: */
		    }
		}
/*            ------------------------------------------------------ */
/*            2.1 Column pivoting */
		s1 = d__[k];
		jj = k;
		i__2 = *n;
		for (l1 = k; l1 <= i__2; ++l1) {
		    if (d__[l1] > s1) {
			s1 = d__[l1];
			jj = l1;
		    }
/* L21: */
		}
		h__ = d__[jj];
		if (jd == 1) {
/* Computing MAX */
		    d__1 = 10., d__2 = *cond * .05;
		    hmax = h__ / max(d__1,d__2);
		}
		jd = 0;
		if (h__ < hmax) {
		    jd = 1;
		}
		if (! (h__ >= hmax)) {
		    goto L20;
		}
/*            UNTIL ( expression - negated above) */

		if (jj != k) {
/*              ------------------------------------------------------ */
/*               2.2 Column interchange */
		    i__ = pivot[k];
		    pivot[k] = pivot[jj];
		    pivot[jj] = i__;
		    d__[jj] = d__[k];
		    i__2 = *m;
		    for (l1 = 1; l1 <= i__2; ++l1) {
			s1 = a[l1 + jj * a_dim1];
			a[l1 + jj * a_dim1] = a[l1 + k * a_dim1];
			a[l1 + k * a_dim1] = s1;
/* L221: */
		    }
		}
/*          endif for k.ne.n case */
	    }
	    h__ = 0.;
	    i__2 = mh;
	    for (l1 = k; l1 <= i__2; ++l1) {
/* Computing 2nd power */
		d__1 = a[l1 + k * a_dim1];
		h__ += d__1 * d__1;
/* L222: */
	    }
	    t = sqrt(h__);
/*          ---------------------------------------------------------- */
/*          2.3.0 A-priori test on pseudo-rank */
	    if (k == 1 || k == iranc1) {
		dd = t / *cond;
	    }
	    if (t <= dd || k > irankh) {
/*            ------------------------------------------------------ */
/*             2.3.1 Rank reduction */
		irankh = k - 1;
		if (mh != *mcon || idata == 1) {
		    *irank = irankh;
		    if (*irankc == *irank) {
			level = 4;
		    } else {
			level = 3;
		    }
		} else {
		    *irankc = irankh;
		    if (*irankc != *mcon) {
			mh = *m;
			irankh = *irank;
			jd = 1;
			idata = 1;
			goto L2000;
		    } else {
			s_stop("INTERNAL ERROR OF DECCON", (ftnlen)24);
		    }
		}
	    }

	    if (level == 1) {
/*             ------------------------------------------------------ */
/*             2.4 Householder step */
		s = a[k + k * a_dim1];
		t = -d_sign(&t, &s);
		d__[k] = t;
/*             By updating a(k,k) at this stage the 241 and 242 loop */
/*             must not be modified for l1=k. */
		a[k + k * a_dim1] = s - t;
		if (k != *n) {
		    t = 1. / (h__ - s * t);
		    i__2 = *n;
		    for (j = k1; j <= i__2; ++j) {
			s = 0.;
			i__3 = mh;
			for (l1 = k; l1 <= i__3; ++l1) {
			    s += a[l1 + k * a_dim1] * a[l1 + j * a_dim1];
/* L241: */
			}
			s *= t;
			s1 = -s;
			if (s != 0.) {
/*                      Update the sub columns */
			    i__3 = *m;
			    for (l1 = k; l1 <= i__3; ++l1) {
				a[l1 + j * a_dim1] += a[l1 + k * a_dim1] * s1;
/* L242: */
			    }
			}
/*                   Update sub column norms */
/* Computing 2nd power */
			d__1 = a[k + j * a_dim1];
			d__[j] -= d__1 * d__1;
/* L24: */
		    }
		    if (k == *irankc) {
			mh = *m;
			jd = 1;
			irankh = *irank;
		    }
		    if (k == irk1) {
			level = 3;
		    }
		} else {
		    level = 4;
		}
/* endif Householder step */
	    }
/*        Exit Do 2 If ... */
	    if (level > 1) {
		goto L2999;
	    }
/* L2: */
	}
/*        ENDDO */
L2999:
	;
    } else {
	k = -1;
	level = 3;
    }
/*     -------------------------------------------------------------- */
/*     3 Rank-deficient pseudo-inverse */
    if (level == 3) {
	irk1 = *irank + 1;
	i__1 = *n;
	for (j = irk1; j <= i__1; ++j) {
	    i__2 = *irank;
	    for (ii = 1; ii <= i__2; ++ii) {
		i__ = irk1 - ii;
		s = a[i__ + j * a_dim1];
		if (ii != 1) {
		    sh = 0.;
		    i__3 = *irank;
		    for (l1 = i1; l1 <= i__3; ++l1) {
			sh += a[i__ + l1 * a_dim1] * v[l1];
/* L311: */
		    }
		    s -= sh;
		}
		i1 = i__;
		v[i__] = s / d__[i__];
		ah[i__ + j * ah_dim1] = v[i__];
/* L31: */
	    }
	    i__2 = j;
	    for (i__ = irk1; i__ <= i__2; ++i__) {
		s = 0.;
		i__3 = i__ - 1;
		for (l1 = 1; l1 <= i__3; ++l1) {
		    s += ah[l1 + i__ * ah_dim1] * v[l1];
/* L321: */
		}
		if (i__ != j) {
		    v[i__] = -s / d__[i__];
		    ah[i__ + j * ah_dim1] = -v[i__];
		}
/* L32: */
	    }
	    if (s > -1.) {
		d__[j] = sqrt(s + 1.);
	    } else {
		*ierr = -2;
		goto L999;
	    }
/* L3: */
	}
    }
/*    -------------------------------------------------------------- */
/*     9 Exit */
    if (*irankc != 0) {
	sh = d__[*irankc];
	if (sh != 0.) {
	    sh = (d__1 = d__[1] / sh, abs(d__1));
	}
    } else {
	sh = 0.;
    }
    v[1] = sh;
    if (k == *irank) {
	t = d__[*irank];
    }
    if (*irankc + 1 <= *irank && t != 0.) {
	s = (d__1 = d__[*irankc + 1] / t, abs(d__1));
    } else {
	s = 0.;
    }
    *cond = s;
    *ierr = 0;
L999:
    return 0;
} /* deccon_ */

/* Subroutine */ int solcon_(doublereal *a, integer *nrow, integer *ncol, 
	integer *mcon, integer *m, integer *n, doublereal *x, doublereal *b, 
	integer *irankc, integer *irank, doublereal *d__, integer *pivot, 
	integer *kred, doublereal *ah, doublereal *v)
{
    /* System generated locals */
    integer a_dim1, a_offset, ah_dim1, ah_offset, i__1, i__2;

    /* Local variables */
    static integer i__, j;
    static doublereal s;
    static integer i1, j1, l1, l2, ii, jj, mh;
    static doublereal sh;
    static integer irk1, iranc1;

/* *    Begin Prologue SOLCON */
/*     ------------------------------------------------------------ */

/* *    Summary */
/*     ======= */

/*     Best constrained linear least squares solution of (M,N)- */
/*     system . First MCON rows comprise MCON equality constraints. */
/*     To be used in connection with subroutine DECCON */
/*     References:       See DECCON */
/*     Related Programs: DECCON */

/* *    Parameters: */
/*     =========== */

/* *    Input parameters (* marks inout parameters) */
/*     =========================================== */

/*       A(M,N), NROW, NCOL, M, N, MCON, IRANKC, IRANK, */
/*       D(N), PIVOT(N), AH(N,N), KRED */
/*                           See input- respective output-parameters */
/*                           description of subroutine DECCON */
/*     * B(M)         Dble   Right-hand side of linear system, if */
/*                           KRED.GE.0 */
/*                           Right-hand side of upper linear system, */
/*                           if KRED.LT.0 */

/* *    Output parameters */
/*     ================= */

/*       X(N)         Dble   Best LSQ-solution of linear system */
/*       B(M)         Dble   Right-hand of upper trigular system */
/*                           (transformed right-hand side of linear */
/*                            system) */

/* *    Workspace parameters */
/*     ==================== */

/*       V(N)         Dble   Workspace array */

/*     ------------------------------------------------------------ */
/* *    End Prologue */
/* *    Begin */
/*     ------------------------------------------------------------ */
/*     1 Solution for pseudo-rank zero */
    /* Parameter adjustments */
    --b;
    --v;
    ah_dim1 = *ncol;
    ah_offset = 1 + ah_dim1;
    ah -= ah_offset;
    --pivot;
    --d__;
    --x;
    a_dim1 = *nrow;
    a_offset = 1 + a_dim1;
    a -= a_offset;

    /* Function Body */
    if (*irank == 0) {
	i__1 = *n;
	for (l1 = 1; l1 <= i__1; ++l1) {
	    x[l1] = 0.;
/* L11: */
	}
	return 0;
    }
    if (*irank <= *irankc && *irank != *n) {
	iranc1 = *irankc + 1;
	i__1 = *n;
	for (l1 = iranc1; l1 <= i__1; ++l1) {
	    v[l1] = 0.;
/* L12: */
	}
    }
    if (*kred >= 0 && (*m != 1 || *n != 1)) {
/*       ---------------------------------------------------------- */
/*       2 Constrained householder transformations of right-hand side */
	mh = *mcon;
	if (*irankc == 0) {
	    mh = *m;
	}
	i__1 = *irank;
	for (j = 1; j <= i__1; ++j) {
	    s = 0.;
	    i__2 = mh;
	    for (l1 = j; l1 <= i__2; ++l1) {
		s += a[l1 + j * a_dim1] * b[l1];
/* L211: */
	    }
	    s /= d__[j] * a[j + j * a_dim1];
	    i__2 = *m;
	    for (l1 = j; l1 <= i__2; ++l1) {
		b[l1] += a[l1 + j * a_dim1] * s;
/* L212: */
	    }
	    if (j == *irankc) {
		mh = *m;
	    }
/* L21: */
	}
    }
/*     ------------------------------------------------------------ */
/*     3 Solution of upper triangular system */
    irk1 = *irank + 1;
    i__1 = *irank;
    for (ii = 1; ii <= i__1; ++ii) {
	i__ = irk1 - ii;
	i1 = i__ + 1;
	s = b[i__];
	if (ii != 1) {
	    sh = 0.;
	    i__2 = *irank;
	    for (l1 = i1; l1 <= i__2; ++l1) {
		sh += a[i__ + l1 * a_dim1] * v[l1];
/* L311: */
	    }
	    s -= sh;
	}
	v[i__] = s / d__[i__];
/* L31: */
    }
    if (*irank != *n && *irank != *irankc) {
/*       ---------------------------------------------------------- */
/*       3.2 Computation of the best constrained least squares- */
/*           solution */
	i__1 = *n;
	for (j = irk1; j <= i__1; ++j) {
	    s = 0.;
	    i__2 = j - 1;
	    for (l1 = 1; l1 <= i__2; ++l1) {
		s += ah[l1 + j * ah_dim1] * v[l1];
/* L3211: */
	    }
	    v[j] = -s / d__[j];
/* L321: */
	}
	i__1 = *n;
	for (jj = 1; jj <= i__1; ++jj) {
	    j = *n - jj + 1;
	    s = 0.;
	    if (jj != 1) {
		i__2 = *n;
		for (l1 = j1; l1 <= i__2; ++l1) {
		    s += ah[j + l1 * ah_dim1] * v[l1];
/* L3221: */
		}
	    }
	    if (jj != 1 && j <= *irank) {
		v[j] -= s;
	    } else {
		j1 = j;
		v[j] = -(s + v[j]) / d__[j];
	    }
/* L322: */
	}
    }
/*     ------------------------------------------------------------ */
/*     4 Back-permutation of solution components */
    i__1 = *n;
    for (l1 = 1; l1 <= i__1; ++l1) {
	l2 = pivot[l1];
	x[l2] = v[l1];
/* L4: */
    }
    return 0;
} /* solcon_ */

