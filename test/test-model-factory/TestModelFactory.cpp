//
// Created by Ciaran on 17/05/2021.
//

#include "TestModelFactory.h"
#include "rrLogger.h"

std::vector<std::string> getAvailableTestModels() {
    return std::vector<std::string>(
            {
                    "SimpleFlux",
                    "SimpleFluxManuallyReduced",
                    "OpenLinearFlux",
                    "Model269",
                    "Model28",
                    "CeilInRateLaw",
                    "FactorialInRateLaw",
                    "Venkatraman2010",
                    "Brown2004",
                    "LayoutOnly",
                    "PythonAPITestsModel"
            });
}


TestModel *TestModelFactory(const std::string &modelName) {
    if (modelName == "SimpleFlux") {
        return new SimpleFlux();
    } else if (modelName == "Model269") {
        return new Model269();
    } else if (modelName == "Model28") {
        return new Model28();
    } else if (modelName == "CeilInRateLaw") {
        return new CeilInRateLaw();
    } else if (modelName == "FactorialInRateLaw") {
        return new FactorialInRateLaw();
    } else if (modelName == "Venkatraman2010") {
        return new Venkatraman2010();
    } else if (modelName == "OpenLinearFlux") {
        return new OpenLinearFlux();
    } else if (modelName == "SimpleFluxManuallyReduced") {
        return new SimpleFluxManuallyReduced();
    } else if (modelName == "Brown2004") {
        return new Brown2004();
    } else if (modelName == "PythonAPITestsModel") {
        return new PythonAPITestsModel();
    } else {
        std::ostringstream err;
        err << "TestModelFactory::TestModelFactory(): no model called \"" << modelName << "\" found. ";
        err << "Available test models include: ";
        for (auto name: getAvailableTestModels()){
            err << "\""<< name << "\", ";
        }
        // When windows throws error through Python, we do not get an error message
        // so we need to log it as well.
        rrLog(rr::Logger::LOG_ERROR) << err.str().c_str() << std::endl;
        throw std::runtime_error(err.str());
    }
}

