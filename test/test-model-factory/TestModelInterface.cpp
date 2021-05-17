#include "TestModelInterface.h"


std::unordered_map<std::string, rr::Setting> SteadyStateResult::steadyStateSettings() {
    return std::unordered_map<std::string, rr::Setting>();
}

std::unordered_map<std::string, rr::Setting> TimeSeriesResult::timeSeriesSettings() {
    return std::unordered_map<std::string, rr::Setting>();
}


namespace privateSwigTests_ {
    // this section exists only to test the swig bindings
    // and make sure the typemaps are doing what they are supposed
    // to be. Users should completely ignore this

    DoublePair *_testDoublePair(double first, double second) {
        DoublePair *pair = new DoublePair(first, second);
        return pair;
    }

    std::unordered_map<double, double> *_testDoubleMap(double first, double second) {
        std::unordered_map<double, double> *map = new std::unordered_map<double, double>{
                {first, second}
        };
        return map;
    }

    std::unordered_map<std::string, rr::Setting> *_testVariantMap() {
        std::unordered_map<std::string, rr::Setting> *map = new std::unordered_map<std::string, rr::Setting>{
                {"mapsy", rr::Setting(5)}
        };
        return map;
    }

    rr::Setting *_testVariant() {
        rr::Setting *x = new rr::Setting(5.4);
        return x;
    }

    StringDoublePairMap _testResultMap() {
        return StringDoublePairMap{
                {"First", DoublePair(0.5, 1.6)},
        };
    }
}

