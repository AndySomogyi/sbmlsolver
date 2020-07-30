/**
 * \file    TestSBaseRef.c
 * \brief   SBaseRef unit tests
 * \author  Sarah Keating
 * 
 * <!--------------------------------------------------------------------------
 * This file is part of libSBML.  Please visit http://sbml.org for more
 * information about SBML, and the latest version of libSBML.
 *
 * Copyright (C) 2019 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. University of Heidelberg, Heidelberg, Germany
 *
 * Copyright (C) 2013-2018 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 *     3. University of Heidelberg, Heidelberg, Germany
 * 
 * Copyright 2011-2012 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.  A copy of the license agreement is provided
 * in the file named "LICENSE.txt" included with this software distribution
 * and also available online as http://sbml.org/software/libsbml/license.html
 * ---------------------------------------------------------------------- -->*/

#include <sbml/common/common.h>

#include <sbml/packages/comp/sbml/SBaseRef.h>
#include <sbml/packages/comp/extension/CompExtension.h>

#include <sbml/SBMLDocument.h>

#include <check.h>



#include <sbml/common/extern.h>

LIBSBML_CPP_NAMESPACE_USE


BEGIN_C_DECLS

static SBaseRef_t *P;


void
SBaseRefTest_setup (void)
{
  P = SBaseRef_create(3, 1, 1);

  if (P == NULL)
  {
    fail("SBaseRef_create(3, 1, 1) returned a NULL pointer.");
  }
}


void
SBaseRefTest_teardown (void)
{
  SBaseRef_free(P);
}


START_TEST (test_comp_sBaseRef_create)
{
  fail_unless( SBase_getTypeCode  ((SBase_t *) P) == SBML_COMP_SBASEREF );
  fail_unless( SBase_getMetaId    ((SBase_t *) P) == NULL );
  fail_unless( SBase_getNotes     ((SBase_t *) P) == NULL );
  fail_unless( SBase_getAnnotation((SBase_t *) P) == NULL );

  fail_unless( SBaseRef_getIdRef     (P) == NULL );
  fail_unless( SBaseRef_getUnitRef   (P) == NULL );
  fail_unless( SBaseRef_getMetaIdRef (P) == NULL );
  fail_unless( SBaseRef_getPortRef   (P) == NULL );
  fail_unless( SBaseRef_getSBaseRef   (P) == NULL );

  fail_unless( !SBaseRef_isSetIdRef     (P) );
  fail_unless( !SBaseRef_isSetUnitRef   (P) );
  fail_unless( !SBaseRef_isSetMetaIdRef   (P) );
  fail_unless( !SBaseRef_isSetPortRef   (P) );
  fail_unless( !SBaseRef_isSetSBaseRef   (P) );
}
END_TEST


START_TEST (test_comp_sBaseRef_free_NULL)
{
  SBaseRef_free(NULL);
}
END_TEST


START_TEST (test_comp_sBaseRef_idRef)
{
  const char *idRef = "mitochondria";


  fail_unless( !SBaseRef_isSetIdRef(P) );
  
  SBaseRef_setIdRef(P, idRef);

  char* getchar = SBaseRef_getIdRef(P);
  fail_unless( !strcmp(getchar, idRef) );
  fail_unless( SBaseRef_isSetIdRef(P) );

  if (getchar == idRef)
  {
    fail("SBaseRef_setIdRef(...) did not make a copy of string.");
  }
  free(getchar);
 
  SBaseRef_unsetIdRef(P);
  
  fail_unless( !SBaseRef_isSetIdRef(P) );

  if (SBaseRef_getIdRef(P) != NULL)
  {
    fail("SBaseRef_unsetIdRef(P) did not clear string.");
  }
}
END_TEST


START_TEST (test_comp_sBaseRef_unitRef)
{
  const char *unitRef = "My_Favorite_Factory";


  fail_unless( !SBaseRef_isSetUnitRef(P) );

  SBaseRef_setUnitRef(P, unitRef);

  char* getchar = SBaseRef_getUnitRef(P);
  fail_unless( !strcmp(getchar, unitRef) );
  fail_unless( SBaseRef_isSetUnitRef(P) );

  if (getchar == unitRef)
  {
    fail("SBaseRef_setUnitRef(...) did not make a copy of string.");
  }
  free(getchar);

  SBaseRef_unsetUnitRef(P);
  
  fail_unless( !SBaseRef_isSetUnitRef(P) );

  if (SBaseRef_getUnitRef(P) != NULL)
  {
    fail("SBaseRef_unsetUnitRef(P) did not clear string.");
  }
}
END_TEST


START_TEST (test_comp_sBaseRef_metaIdRef)
{
  const char *metaIdRef = "My_Favorite_Factory";


  fail_unless( !SBaseRef_isSetMetaIdRef(P) );

  SBaseRef_setMetaIdRef(P, metaIdRef);

  char* getchar = SBaseRef_getMetaIdRef(P);
  fail_unless( !strcmp(getchar, metaIdRef) );
  fail_unless( SBaseRef_isSetMetaIdRef(P) );

  if (getchar == metaIdRef)
  {
    fail("SBaseRef_setMetaIdRef(...) did not make a copy of string.");
  }
  free(getchar);

  SBaseRef_unsetMetaIdRef(P);
  
  fail_unless( !SBaseRef_isSetMetaIdRef(P) );

  if (SBaseRef_getMetaIdRef(P) != NULL)
  {
    fail("SBaseRef_unsetMetaIdRef(P) did not clear string.");
  }
}
END_TEST


START_TEST (test_comp_sBaseRef_portRef)
{
  const char *portRef = "My_Favorite_Factory";


  fail_unless( !SBaseRef_isSetPortRef(P) );

  SBaseRef_setPortRef(P, portRef);

  char* getchar = SBaseRef_getPortRef(P);
  fail_unless( !strcmp(getchar, portRef) );
  fail_unless( SBaseRef_isSetPortRef(P) );

  if (getchar == portRef)
  {
    fail("SBaseRef_setPortRef(...) did not make a copy of string.");
  }
  free(getchar);

  SBaseRef_unsetPortRef(P);
  
  fail_unless( !SBaseRef_isSetPortRef(P) );

  if (SBaseRef_getPortRef(P) != NULL)
  {
    fail("SBaseRef_unsetPortRef(P) did not clear string.");
  }
}
END_TEST


START_TEST (test_comp_sBaseRef_sBaseRef)
{
  SBaseRef_t *sBaseRef =  SBaseRef_create(3, 1, 1);

  fail_unless( !SBaseRef_isSetSBaseRef(P) );

  SBaseRef_setSBaseRef(P, sBaseRef);

  SBaseRef_t* getsbr = SBaseRef_getSBaseRef(P);
  fail_unless( getsbr != NULL);
  fail_unless( SBaseRef_isSetSBaseRef(P) );

  if (getsbr == sBaseRef)
  {
    fail("SBaseRef_setSBaseRef(...) did not make a copy of the SBaseRef_t.");
  }

  SBaseRef_unsetSBaseRef(P);
  
  fail_unless( !SBaseRef_isSetSBaseRef(P) );

  if (SBaseRef_getSBaseRef(P) != NULL)
  {
    fail("SBaseRef_unsetSBaseRef(P) did not clear the SBaseRef_t.");
  }

  SBaseRef_free(sBaseRef);
}
END_TEST


START_TEST (test_comp_sBaseRef_hasRequiredAttributes )
{
  SBaseRef_t *P = SBaseRef_create (3, 1, 1);

  fail_unless ( !SBaseRef_hasRequiredAttributes(P));

  SBaseRef_setIdRef(P, "p");

  fail_unless ( SBaseRef_hasRequiredAttributes(P));

  SBaseRef_unsetIdRef(P);
  SBaseRef_setMetaIdRef(P, "p");

  fail_unless ( SBaseRef_hasRequiredAttributes(P));

  SBaseRef_unsetMetaIdRef(P);
  SBaseRef_setUnitRef(P, "p");

  fail_unless ( SBaseRef_hasRequiredAttributes(P));

  SBaseRef_unsetUnitRef(P);
  SBaseRef_setPortRef(P, "p");

  fail_unless ( SBaseRef_hasRequiredAttributes(P));

  SBaseRef_free(P);
}
END_TEST


Suite *
create_suite_TestComp_SBaseRef (void)
{
  Suite *suite = suite_create("comp_sBaseRef");
  TCase *tcase = tcase_create("comp_sBaseRef");


  tcase_add_checked_fixture( tcase,
                             SBaseRefTest_setup,
                             SBaseRefTest_teardown );

  tcase_add_test( tcase, test_comp_sBaseRef_create              );
  tcase_add_test( tcase, test_comp_sBaseRef_free_NULL           );
  tcase_add_test( tcase, test_comp_sBaseRef_idRef               );
  tcase_add_test( tcase, test_comp_sBaseRef_unitRef             );
  tcase_add_test( tcase, test_comp_sBaseRef_metaIdRef           );
  tcase_add_test( tcase, test_comp_sBaseRef_portRef             );
  tcase_add_test( tcase, test_comp_sBaseRef_sBaseRef            );
  tcase_add_test( tcase, test_comp_sBaseRef_hasRequiredAttributes        );

  suite_add_tcase(suite, tcase);

  return suite;
}

END_C_DECLS

