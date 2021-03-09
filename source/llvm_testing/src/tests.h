/*
 * tests.h
 *
 *  Created on: Jul 1, 2013
 *      Author: andy
 */

#ifndef TESTS_H_
#define TESTS_H_

#include <string>



std::string getModelFileName(const std::string& version, int caseNumber);

bool runModelDataAccessorTest(const std::string& version, int caseNumber);

bool runInitialValueAssigmentTest(const std::string& version, int caseNumber) ;






#endif /* TESTS_H_ */
