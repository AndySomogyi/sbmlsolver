/*
 * TestEvalReactionRates.h
 *
 *  Created on: Jul 20, 2013
 *      Author: andy
 */

#ifndef TESTEVALREACTIONRATES_H_
#define TESTEVALREACTIONRATES_H_

#include "TestBase.h"

namespace rr
{
    
    class TestEvalReactionRates : public TestBase
    {
    public:
        TestEvalReactionRates(const std::string& version, int caseNumber);
        virtual ~TestEvalReactionRates();
    };
    
} /* namespace rr */
#endif /* TESTEVALREACTIONRATES_H_ */
