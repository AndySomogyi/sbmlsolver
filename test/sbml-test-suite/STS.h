//
// Created by Ciaran on 13/12/2021.
//

#ifndef ROADRUNNER_STS_H
#define ROADRUNNER_STS_H

#include <iostream>
#include <vector>
#include "rrLogger.h"

using namespace rr;

class STS {
public:

    STS();

    std::string constructSTSXmlFile(int caseId, int level, int version) ;

    std::vector<std::string> getFirstNModelsFromSTS(int N, int start = 1) ;
};


#endif //ROADRUNNER_STS_H
