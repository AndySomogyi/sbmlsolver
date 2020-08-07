//
#ifndef RR_SBMLFACTORY_H
#define RR_SBMLFACTORY_H

#include <utility>
#include <string>
#include <stdexcept>

namespace rrtest {


    enum ModelType {
        BISTABLE
    };

    /*
     * Abstract class implemented by all SBML models
     */
    class SBMLModel {
    public:
        SBMLModel() = default;

        virtual std::string str() = 0;

    };

    class Bistable : SBMLModel {
    public:
        Bistable() = default;

        std::string str() override;
    };

    class BooleanTriggerL3V2 : SBMLModel {
    public:
        BooleanTriggerL3V2() = default;

        std::string str() override;
    };




    std::string SBMLFactory();


}

#endif //RR_SBMLFACTORY_H
