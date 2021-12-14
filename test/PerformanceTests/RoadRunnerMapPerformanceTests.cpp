//
// Created by Ciaran on 13/12/2021.
//

#include "STS.h"
#include "llvm/Support/CommandLine.h"
#include "rrRoadRunner.h"
#include "rrRoadRunnerMap.h"
#include "rrConfig.h"

using namespace rr;

/**
 * Add a "category" to suppress llvm giving us loads of unrelated options
 * Give this as argument to other options.
 */
llvm::cl::OptionCategory GeneralOptions("General Options");

enum TestNames {
    BuildSerial,
    BuildParallel
};

static llvm::cl::opt<TestNames> TestName(
        "testName",
        llvm::cl::init(TestNames::BuildParallel),
        llvm::cl::desc("Which test to run"),
        llvm::cl::values(
                llvm::cl::OptionEnumValue{"BuildParallel", (int) TestNames::BuildParallel,
                                          "(default) Build the parallel test"},
                llvm::cl::OptionEnumValue{"BuildSerial", (int) TestNames::BuildSerial, "Build the serial test"}
        ),
        llvm::cl::cat(GeneralOptions)
);

static llvm::cl::opt<Config::LLVM_BACKEND_VALUES> WhichJit(
        "jit",
        llvm::cl::init(Config::MCJIT),
        llvm::cl::desc("Which Jit compiler to use"),
        llvm::cl::values(
                llvm::cl::OptionEnumValue{"MCJit", (int) Config::MCJIT, "(default) Use the MCJit compiler"},
                llvm::cl::OptionEnumValue{"LLJit", (int) Config::LLJIT, "Use the LLJit compiler"}
        ),
        llvm::cl::cat(GeneralOptions)
);

static llvm::cl::opt<int> NumThreads(
        "nThreads",
        llvm::cl::init(1),
        llvm::cl::desc("Default=1. Number of threads to use in parallel"),
        llvm::cl::cat(GeneralOptions)
);

static llvm::cl::opt<int> NModels(
        "NModels",
        llvm::cl::init(25),
        llvm::cl::desc("Default=25. How many models from the sbml test suite to test. Must be between "
                       "1 and 1809"),
        llvm::cl::cat(GeneralOptions));

static llvm::cl::opt<int> Start(
        "start",
        llvm::cl::init(1),
        llvm::cl::desc("Default=1. Which sbml test suite case to start counting from. "
                       "start + N must not be greater than 1809"),
        llvm::cl::cat(GeneralOptions)
);

static llvm::cl::opt<bool> Verbose(
        "verbose",
        llvm::cl::init(false),
        llvm::cl::desc("Default=false. Output logging messages"),
        llvm::cl::cat(GeneralOptions)
);


/**
 * Build N RoadRunner models in serial. Add them to a map.
 */
void buildSerial() {
    STS sts;
    Config::setValue(Config::LLVM_BACKEND, WhichJit.getValue());
    std::vector<std::string> sbmlFiles = sts.getFirstNModelsFromSTS(NModels.getValue(), Start.getValue());
    std::unordered_map<std::string, std::unique_ptr<RoadRunner>> rrMap;
    for (auto &f: sbmlFiles) {
        if (Verbose.getValue()) {
            std::cout << "loading \"" << f << "\"";
        }
        std::unique_ptr<RoadRunner> rr = std::make_unique<RoadRunner>(f);
        rrMap[rr->getModelName()] = std::move(rr);
    }
}

/**
 * Build N RoadRunner models in parallel
 */
void buildParallel() {
    STS sts;
    Config::setValue(Config::LLVM_BACKEND, WhichJit.getValue());
    std::vector<std::string> sbmlFiles = sts.getFirstNModelsFromSTS(NModels.getValue(), Start.getValue());
    if (Verbose.getValue()) {
        std::cout << "Loading the following: \n";
        for (auto f : sbmlFiles){
            std::cout << "\t" << f << std::endl;
        }
        std::cout << std::endl;
    }
    RoadRunnerMap rrm(sbmlFiles, NumThreads.getValue());
}


int main(int argc, const char *argv[]) {
    Config::setValue(Config::LLJIT_NUM_THREADS, 1);

    /**
     * Parse arguments
     */
    llvm::cl::HideUnrelatedOptions(GeneralOptions);
    llvm::cl::ParseCommandLineOptions(argc, argv, " RoadRunner Map Performance Tests\n");

    TestNames testName = TestName.getValue();

    if (Verbose.getValue()){
        std::ostringstream os;
        os << "Option Values" << std::endl;
        os << "-------------" << std::endl;
        os << "TestName: " << TestName.getValue() << std::endl;
        os << "WhichJit: " << WhichJit.getValue() << std::endl;
        os << "NumThreads: " << NumThreads.getValue() << std::endl;
        os << "NModels: " << NModels.getValue() << std::endl;
        os << "Start: " << Start.getValue() << std::endl;
        os << "Verbose: " << Verbose.getValue() << std::endl;
        std::cout << os.str() << std::endl;
    }

    switch (testName) {
        case TestNames::BuildSerial: {
            buildSerial();
            break;
        }
        case TestNames::BuildParallel: {
            buildParallel();
            break;
        }
        default:
            std::ostringstream err;
            err << "Invalid argument to TestName: \"" << testName << "\"";
            throw std::invalid_argument(err.str());
    }

}

















































