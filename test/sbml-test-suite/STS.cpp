//
// Created by Ciaran on 13/12/2021.
//

#include "STS.h"
#include <filesystem>

std::string errorMsg(const std::string& varName){
    std::ostringstream os;
    os << "The \""<<varName<<"\" variable is not defined. Make sure you have added it to your library or "
                             "executable using cmake's target_compile_options command";
    return os.str();
}

STS::STS() {
#ifndef SBMLTestSuiteRoot
    throw std::logic_error(errorMsg("SBMLTestSuiteRoot"));
#endif
#ifndef SBMLTestSuiteSemantic
    throw std::logic_error(errorMsg("SBMLTestSuiteSemantic"));
#endif
#ifndef SBMLTestSuiteStochastic
    throw std::logic_error(errorMsg("SBMLTestSuiteStochastic"));
#endif
    if (!std::filesystem::exists(SBMLTestSuiteRoot)) {
        throw std::invalid_argument("Path to SBMLTestSuiteRoot does not exist");
    }
    if (!std::filesystem::exists(SBMLTestSuiteSemantic)) {
        throw std::invalid_argument("Path to SBMLTestSuiteSemantic does not exist");
    }
    if (!std::filesystem::exists(SBMLTestSuiteStochastic)) {
        throw std::invalid_argument("Path to SBMLTestSuiteStochastic does not exist");
    }

    Logger::setLevel(Logger::LOG_WARNING);

}

std::string STS::constructSTSXmlFile(int caseId, int level, int version) {
    std::ostringstream caseOs;
    std::ostringstream fname;

    caseOs << std::setfill('0') << std::setw(5) << caseId;
    fname << caseOs.str() << "-sbml-l" << level << "v" << version << ".xml";

    std::filesystem::path root = SBMLTestSuiteSemantic;
    std::filesystem::path caseDir = root / caseOs.str();
    std::filesystem::path xmlFile = caseDir / fname.str();
    if (!std::filesystem::exists(xmlFile)) {
        std::ostringstream os;
        os << "file not found : " << xmlFile;
        throw std::logic_error(os.str());
    }
    return xmlFile.string();
}

std::vector<std::string> STS::getFirstNModelsFromSTS(int N, int start) {
        if (start < 1){
            throw std::invalid_argument("start must be 1 or more");
        }
        int end = start + N;
        int maxSize = 1809;
        if (end > maxSize){
            throw std::invalid_argument("start + N must be less than or equal to 1809: " + std::to_string(end));
        }
        std::vector<std::string> vec(N);
        for (int i = start; i < end; i++) {
            int idx = i - start;
            try {
                vec[idx] = constructSTSXmlFile(i, 3, 2);
            } catch (std::exception &e) {
                rrLogInfo << "Failed to find case " << i << ", l" << 3 << "v" << 2 << ". Looking for another version...";
                try {
                    vec[idx] = constructSTSXmlFile(i, 3, 1);
                } catch (std::exception &e) {
                    rrLogInfo << "Failed to find case " << i << ", l" << 3 << "v" << 1 << ". Looking for another version...";
                    try {
                        vec[idx] = constructSTSXmlFile(i, 2, 4);
                    } catch (std::exception &e) {
                        rrLogInfo << "Failed to find case " << i << ", l" << 2 << "v" << 4 << ". Looking for another version...";
                        try {
                            vec[idx] = constructSTSXmlFile(i, 2, 3);
                        } catch (std::exception &e) {
                            rrLogInfo << "Failed to find case " << i << ", l" << 2 << "v" << 2 << ". Looking for another version...";
                            try {
                                vec[idx] = constructSTSXmlFile(i, 2, 2);
                            } catch (std::exception &e) {
                                rrLogInfo << "Failed to find case " << i << ", l" << 2 << "v" << 2 << ". Looking for another version...";
                                try {
                                    vec[idx] = constructSTSXmlFile(i, 2, 1);
                                } catch (std::exception &e) {
                                    rrLogErr << "Failed to find case " << i << ", l" << 2 << "v" << 1;
                                    throw std::logic_error("can't locate sbml file");
                                }
                            }
                        }
                    }
                }
            }
        }
        return vec;
    }


