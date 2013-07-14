/*
 * tests.h
 *
 *  Created on: Jul 1, 2013
 *      Author: andy
 */

#ifndef TESTS_H_
#define TESTS_H_

#include <string>

using namespace std;

string getModelFileName(const string& version, int caseNumber);

bool runModelDataAccessorTest(const string& version, int caseNumber);

bool runInitialValueAssigmentTest(const string& version, int caseNumber) ;






#endif /* TESTS_H_ */
