//
// Created by Ciaran on 09/12/2021.
//

#include "gtest/gtest.h"
#include "rrRoadRunnerMap.h"
#include "rrConfig.h"
#include "TestModelFactory.h"
#include <filesystem>

using namespace rr;

class RoadRunnerMapTests : public ::testing::Test {
public:
    std::vector<std::string> sbml;

    RoadRunnerMapTests() {
#ifndef SBMLTestSuiteRoot
        throw std::logic_error("The SBMLTestSuiteRoot variable is not defined even though it has been "
                               "set with target_compile_definitions")';
#endif
        if (!std::filesystem::exists(SBMLTestSuiteRoot)){
            throw std::invalid_argument("Path to sbml test suite does not exist");
        }
        Logger::setLevel(Logger::LOG_DEBUG);

        // little easy list of sbmls
        std::vector<std::string> input({
                                               OpenLinearFlux().str(),
                                               SimpleFlux().str(),
                                               Brown2004().str(),
                                               Venkatraman2010().str()
                                       });
        sbml.swap(input);
    }

    std::string constructSTSXmlFile(int caseId, int level, int version){
        std::ostringstream caseOs;
        std::ostringstream fname;

        caseOs << std::setfill('0') << std::setw(5) << caseId;
        fname <<  caseOs.str() << "-sbml-l" << level << "v" << version << ".xml";

        std::filesystem::path root = SBMLTestSuiteRoot;
        std::filesystem::path caseDir = root / caseOs.str();
        std::filesystem::path xmlFile = caseDir / fname.str() ;
        if (!std::filesystem::exists(xmlFile)){
            std::ostringstream os;
            os << "file not found : " << xmlFile;
            throw std::logic_error(os.str());
        }
        return xmlFile.string();
    }

    std::vector<std::string> getFirstNModelsFromSTS(int N){
        std::vector<std::string> vec(N);
        for (int i=1; i<=N; i++){
            int idx = i-1;
            try{
                vec[idx] = constructSTSXmlFile(i, 3, 2);
            } catch (std::exception& e){
                try {
                    vec[idx] = constructSTSXmlFile(i, 2, 4);
                } catch (std::exception &e){
                    try {
                        vec[idx] = constructSTSXmlFile(i, 2, 3);
                    } catch (std::exception &e){
                        try {
                            vec[idx] = constructSTSXmlFile(i, 2, 2);
                        } catch (std::exception &e){
                            try{
                                vec[idx] = constructSTSXmlFile(i, 2, 1);
                            } catch (std::exception &e){
                                throw std::logic_error("can't locate sbml file");
                            }
                        }
                    }
                }
            }
        }
        return vec;
    }
};


//TEST_F(RoadRunnerMapTests, serial) {
//    Config::setValue(Config::LLVM_BACKEND, Config::LLJIT);
//    int N = 100;
//    std::vector<std::string> sbmlFiles = getFirstNModelsFromSTS(N);
//    std::unordered_map<std::string, std::unique_ptr<RoadRunner>> rrMap;
//    for (auto &f : sbmlFiles){
//        std::unique_ptr<RoadRunner> rr = std::make_unique<RoadRunner>(f);
//        rrMap[rr->getModelName()] = std::move(rr);
//    }
//}

TEST_F(RoadRunnerMapTests, ParallelWith16Threads) {
    Config::setValue(Config::LLVM_BACKEND, Config::LLJIT);
    int N = 100;
    std::vector<std::string> sbmlFiles = getFirstNModelsFromSTS(N);
    RoadRunnerMap rrm(sbmlFiles, 1);
}

//TEST_F(RoadRunnerMapTests, d) {
//    RoadRunner rr(OpenLinearFlux().str());
//}
//
//#include "thread_pool.hpp"
//#include <thread>
//#include <iostream>
//
//static std::mutex mtx;
//static void printString(const std::string& s) {
//    std::lock_guard lock(mtx);
//    std::hash<std::thread::id> tid{};
//    auto id = tid(std::this_thread::get_id()) ;
//    std::cout << "thread: " << id << " " << s << std::endl;
//}
//
//TEST(test, t) {
//    /**
//     * Order of variable declaration is important.
//     * We have one vector and many threads. When reclaiming memory,
//     * the vector needs to be destructed after all jobs have been completed.
//     * So you can do this by either decl v before pool so pool gets destr before v
//     * or waiting for all threads to finish before the end of fn.
//     */
//    int N = 1000000;
//    std::vector<std::string> v(N);
//    thread_pool pool(12);
//    for (int i = 0; i < N; i++) {
//        v[i] = std::to_string(i);
//    }
//    for (auto &s: v) {
//        pool.push_task([&s]() {
//            printString(s);
//        });
//    }
//    pool.wait_for_tasks();
//
//}
//
//


















