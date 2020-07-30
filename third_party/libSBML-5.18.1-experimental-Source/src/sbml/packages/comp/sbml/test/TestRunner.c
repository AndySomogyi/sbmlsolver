/**
 * \file    TestRunner.c
 * \brief   Runs all unit tests in the comp sbml module
 * \author  Frank T. Bergmann
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

#include <check.h>
#include <stdlib.h>
#include <string.h>

#include <sbml/util/memory.h>
#include <sbml/common/extern.h>

#ifdef LIBSBML_USE_VLD
  #include <vld.h>
#endif


/**
 * Test suite creation function prototypes.
 *
 * These functions are needed only for calls in main() below.  Therefore a
 * separate header file is not necessary and only adds a maintenance burden
 * to keep the two files synchronized.
 */
#if defined(__cplusplus)
LIBSBML_CPP_NAMESPACE_USE
CK_CPPSTART
#endif

Suite *create_suite_TestCompSBaseObjects      (void);
Suite *create_suite_TestReadWriteSBaseObjects (void);
Suite *create_suite_TestComp_Deletion (void);
Suite *create_suite_TestComp_ExternalModelDefinition (void);
Suite *create_suite_TestComp_ModelDefinition (void);
Suite *create_suite_TestComp_Port (void);
Suite *create_suite_TestComp_ReplacedBy (void);
Suite *create_suite_TestComp_ReplacedElement (void);
Suite *create_suite_TestComp_SBaseRef (void);
Suite *create_suite_TestComp_Submodel (void);
Suite *create_suite_TestComp_AttributeNamespaces (void);


/**
 * Global.
 *
 * Declared extern in TestReadFromFileN suites.
 */
char *TestDataDirectory;


/**
 * Sets TestDataDirectory for the the TestReadFromFileN suites.
 *
 * For Automake's distcheck target to work properly, TestDataDirectory must
 * begin with the value of the environment variable SRCDIR.
 */
void
setTestDataDirectory (void)
{
  char *srcdir = getenv("srcdir");
  int  length  = (srcdir == NULL) ? 0 : (int)strlen(srcdir);


  /**
   * strlen("/test-data/") = 11 + 1 (for NULL) = 12
   */
  TestDataDirectory = (char *) safe_calloc( length + 12, sizeof(char) );

  if (srcdir != NULL)
  {
    strcpy(TestDataDirectory, srcdir);
    strcat(TestDataDirectory, "/");
  }

  strcat(TestDataDirectory, "test-data/");
}


int
main (void) 
{ 
  int num_failed;

  setTestDataDirectory();

  SRunner *runner = srunner_create( create_suite_TestCompSBaseObjects() );
  srunner_add_suite( runner, create_suite_TestComp_AttributeNamespaces() );
  srunner_add_suite( runner, create_suite_TestReadWriteSBaseObjects() );
  srunner_add_suite( runner, create_suite_TestComp_Deletion() );
  srunner_add_suite( runner, create_suite_TestComp_ExternalModelDefinition() );
  srunner_add_suite( runner, create_suite_TestComp_ModelDefinition() );
  srunner_add_suite( runner, create_suite_TestComp_Port() );
  srunner_add_suite( runner, create_suite_TestComp_ReplacedBy() );
  srunner_add_suite( runner, create_suite_TestComp_ReplacedElement() );
  srunner_add_suite( runner, create_suite_TestComp_SBaseRef() );
  srunner_add_suite( runner, create_suite_TestComp_Submodel() );

  /* srunner_set_fork_status(runner, CK_NOFORK); */

  srunner_run_all(runner, CK_NORMAL);
  num_failed = srunner_ntests_failed(runner);

  srunner_free(runner);

  safe_free(TestDataDirectory);
  return num_failed;
}

#if defined(__cplusplus)
CK_CPPEND
#endif


