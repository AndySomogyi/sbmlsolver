#pragma hdrstop

#include "TestEvalModel.h"
#include "TestRoadRunner.h"
#include "GetBoundarySpeciesAmountTest.h"
#include "TestEvalInitialConditions.h"
#include "TestGetSetValues.h"
#include "TestCapabilities.h"
#include "TestEvalReactionRates.h"
#include "rrConfig.h"
#include "rrUtils.h"

#include "SBMLValidator.h"

#include "ConfigurableTest.h"

#include "rrRoadRunner.h"

#include "Integrator.h"

#include "rrLogger.h"

#include "TestVariant.h"

#include <sbml/SBMLDocument.h>
#include <sbml/Model.h>
#include <sbml/SBMLReader.h>
#include <time.h>
#include <stdio.h>
#include <cmath>
#include <stdint.h>




using namespace rr;



/**
 * perform a stochastic ensemble calculation.
 *
 *   :param: r an existing roadrunner obj, loaded with a mode.
 *   :param: n how many ensembles to run.
 *   :param: seed the seed value, use None to continue with previous
 *           random number stream.
 *   :param: start start time
 *   :param: stop stop time
 *   :param: ntps how many points display in the result.
 */
DoubleMatrix ensemble(RoadRunner &r, int n, unsigned long seed, double start, double stop, int npts) {

    // set the seed value of the integrator
	r.setIntegrator("gillespie");
    Integrator *itg = r.getIntegrator();
	itg->setValue("seed", seed);

	// setup the simulation
	// create a sim opt obj, loads itself with
	// default values in ctor.
	SimulateOptions o;
	o.start = start;
	o.duration = stop;
	o.steps = npts;

    // we should reset the model each time we simulate,
    // set the RESET_MODEL bit.
	o.reset_model = true;
	r.setIntegrator("gillespie");

    // make a result var
    DoubleMatrix result(npts+1, n+1);

    for (int i = 0; i < n; ++i) {
        const DoubleMatrix &sim = *r.simulate(&o);

        // copy result data colum
        for (int row = 0; row < npts+1; ++row) {
            result(row, i+1) = sim(row, 1);
        }
    }

    // copy result time column
    const DoubleMatrix &sim = *r.getSimulationData();

    // copy result data colum
    for (int row = 0; row < npts+1; ++row) {
        result(row, 0) = sim(row, 0);
    }

    return result;
}


void run_ensemble(const char* fname, unsigned long seed)
{
    try {
        RoadRunner r(fname);

        DoubleMatrix result = ensemble(r, 5, seed, 0, 10, 150);

        std::cout.precision(6);

        for(int row = 0; row < result.numRows(); ++row) {
            for(int col = 0; col < result.numCols(); ++col) {
                std::cout << result(row, col) << ", ";
            }
            std::cout << std::endl;
        }
    } catch (std::exception& e) {
        std::cout << "Error running ensemble: " << e.what() << std::endl;
    }
}

int ensemble_test(int argc, char* argv[])
{
    if (argc < 4)
    {
        std::cout << "usage: llvm_testing ensemble fname num_ensembles" << std::endl;
        return -1;
    }

    run_ensemble(argv[2], atol(argv[3]));

    return 0;
}

int path_test(int argc, char* argv[])
{
    try
    {
        std::string cmpTest = joinPath("..", "compilers", "tcc", "tcc.exe");
        std::cout << "comp test: " << cmpTest << std::endl;

        std::cout << "default tmp dir: " << getTempDir() << std::endl;

        Logger::enableFileLogging();
        std::string defLogFile = Logger::getFileName();

        std::cout << "default log file path: " << defLogFile << std::endl;

        if (argc >= 3) {
            std::cout << "setting config temp dir to: " << argv[2] << std::endl;
            Config::setValue(Config::TEMP_DIR_PATH, std::string(argv[2]));
        }

        if (argc >= 4) {
            std::cout << "setting config log file path to " << argv[3] << std::endl;
            Config::setValue(Config::LOGGER_LOG_FILE_PATH, std::string(argv[3]));
        }

        std::cout << "getTempDir: " << getTempDir() << std::endl;

        Logger::enableFileLogging();

        std::cout << "log file: " << Logger::getFileName() << std::endl;

        rrLog(Logger::LOG_NOTICE) << "log messsage";
    }
    catch (std::exception& e)
    {
        std::cout << "Fatal error: " << e.what() << std::endl;
    }

    return 0;
}

int jacobian_test(int argc, char* argv[])
{
    try
    {
        if (argc < 3)
        {
            std::cout << "usage: llvm_testing jacobian fname";
        }

        RoadRunner r(argv[2]);

        std::cout << "amount mode" << std::endl;
        Config::setValue(Config::ROADRUNNER_JACOBIAN_MODE, (unsigned)Config::ROADRUNNER_JACOBIAN_MODE_AMOUNTS);

        ls::DoubleMatrix mat = r.getFullJacobian();

        std::cout << "amount mode full jacobian: " << std::endl;

        std::cout << mat << std::endl;

        mat = r.getReducedJacobian();

        std::cout << "amount mode reduced jacobian: " << std::endl;

        std::cout << mat << std::endl;

        std::cout << "concentration mode" << std::endl;

        Config::setValue(Config::ROADRUNNER_JACOBIAN_MODE, (unsigned)Config::ROADRUNNER_JACOBIAN_MODE_CONCENTRATIONS);

        mat = r.getFullJacobian();

        std::cout << "concentration mode full jacobian: " << std::endl;

        std::cout << mat << std::endl;

        mat = r.getReducedJacobian();

        std::cout << "concentration mode reduced jacobian: " << std::endl;

        std::cout << mat << std::endl;

    }
    catch(std::exception &e)
    {
        std::cout << e.what() << std::endl;
        return -1;
    }
    return 0;
}


int cm_1_test(int argc, char* argv[])
{
    try {
        if (argc < 3)
        {
            std::cout << "usage: llvm_testing jacobian fname";
        }

        RoadRunner r(argv[2]);

        r.setConservedMoietyAnalysis(true);

        ExecutableModel *m = r.getModel();

        int ncm = m->getNumConservedMoieties();

        std::cout << "num conserved moieties: " << ncm << std::endl;

        double val;

        for (int i = 0; i < ncm; ++i) {
            std::string name = m->getConservedMoietyId(i);
            m->getConservedMoietyValues(1, &i, &val);

            std::cout << "cm name: " << name << ", value: " << val << std::endl;
        }


    } catch (std::exception& e) {
        std::cout << "Error running ensemble: " << e.what() << std::endl;
    }

    return 0;

}

int reset_test(int argc, char* argv[])
{
    try {
        if (argc < 4)
        {
            std::cout << "usage: llvm_testing reset fname what";
            return -1;
        }

        RoadRunner r(argv[2]);

        r.setConservedMoietyAnalysis(true);

        std::string what(argv[3]);

        uint opt = 0;

        if(what.find("GLOBAL_PARAMETER") != std::string::npos)
        {
            opt |= SelectionRecord::GLOBAL_PARAMETER;
        }

        if(what.find("TIME") != std::string::npos)
        {
            opt |= SelectionRecord::TIME;
        }

        if(what.find("FLOATING") != std::string::npos)
        {
            opt |= SelectionRecord::FLOATING;
        }

        if(what.find("COMPARTMENT") != std::string::npos)
        {
            opt |= SelectionRecord::COMPARTMENT;
        }

        if(what.find("RATE") != std::string::npos)
        {
            opt |= SelectionRecord::RATE;
        }

        if(what.find("ALL") != std::string::npos)
        {
            opt |= SelectionRecord::ALL;
        }

        r.reset(opt);

        return opt;
    }
    catch(std::exception& e) {
        std::cout << e.what();
    }
    return -1;
}


int validate_test(int argc, char* argv[])
{
    try {
        if (argc < 3)
        {
            std::cout << "usage: llvm_testing validate fname";
            return -1;
        }


        std::string errors = rr::validateSBML(argv[2], (rr::ValidateSBML)0);

        std::cout << errors << std::endl;

    }
    catch(std::exception& e) {
        std::cout << e.what();
    }
    return -1;
}

int distrib_test(int argc, char* argv[])
{
    try {
        if (argc < 3)
        {
            std::cout << "usage: llvm_testing distrib fname";
            return -1;
        }

        std::cout << "r1 values:" << std::endl;

        RoadRunner r1(argv[2]);

        for (int i = 0; i < 5; ++i) {
            std::cout << "value: " << r1.getValue("rand") << std::endl;
        }

        std::cout << "r2 values: " << std::endl;

        RoadRunner r2(argv[2]);

        for (int i = 0; i < 5; ++i) {
            std::cout << "value: " << r2.getValue("rand") << std::endl;
        }


    }
    catch(std::exception& e) {
        std::cout << e.what();
    }
    return -1;
}

typedef std::vector<std::string> strvec;


std::string variantName(const Setting& var) {
    switch(var.type()) {
    case Setting::STRING: return "std::string";
    case Setting::INT32: return "int32";
    case Setting::UINT32: return "uint32";
    case Setting::INT64: return "int64";
    case Setting::UINT64: return "uint64";
    case Setting::FLOAT: return "float";
    case Setting::DOUBLE: return "double";
    case Setting::CHAR: return "char";
    case Setting::UCHAR: return "uchar";
    case Setting::BOOL: return "bool";
    default: return "empty";
    }
}

int variant_test(int argc, char* argv[])
{
    BasicDictionary d;

    d.setItem("int", 123);
    d.setItem("long", (long)123);
    d.setItem("bool", false);
    d.setItem("ulong", (unsigned long)123);
    d.setItem("std::string", "hello");
    d.setItem("double", 1.23);
    d.setItem("char", 'a');

    std::vector<std::string> keys = d.getKeys();

    for(int i = 0; i < keys.size(); ++i) {
        std::cout << "key: " << keys[i] << ", type: " << variantName(d.getItem(keys[i])) << std::endl;
    }

    return 0;
}



int intparam_test(int argc, char* argv[])
{
	// Function suppressed by Wilbert Copeland on June 18, 2015.

    //// get a list of all of the integrator options
    //std::vector<const Dictionary*> opts = IntegratorFactory::getIntegratorOptions();

    //// iterate through the integrator options
    //for(int i = 0; i < opts.size(); ++i) {
    //    // each dictionary will contain all the keys that
    //    // are valid for a particular integrator.
    //    const Dictionary &d = *opts[i];
    //    std::vector<std::string> keys = d.getKeys();
    //    for(int j = 0; j < keys.size(); ++j) {
    //        std::string key = keys[j];
    //        std::string item = (std::string)d.getItem(key);
    //        std::cout << "key: " << key << ", value: " << item << std::endl;
    //    }
    //}

    return 0;
}


int matnames_test(int argc, char* argv[]) {

    ls::DoubleMatrix mat;

    typedef std::vector<std::string> strvec;

    const char* data[] = {
            "1", "2", "3", "4", "5", "6", "7", "8", "9"
    };



    mat.setRowNames(&data[0], &data[sizeof(data)/sizeof(char*)]);

    std::vector<std::string> rowNames = mat.getRowNames();


    mat.setColNames(rowNames.begin(), rowNames.end());


    for(strvec::const_iterator i = mat.getRowNames().begin(); i != mat.getRowNames().end(); ++i) {
        std::cout << *i << ", ";
    }

    std::cout << std::endl;


    for(strvec::const_iterator i = mat.getColNames().begin(); i != mat.getColNames().end(); ++i) {
        std::cout << *i << ", ";
    }

    std::cout << std::endl;


    return 0;
}



int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cout << "usage: llvm_testing test" << std::endl;
        return -1;
    }

    if (strcmp("ensemble", argv[1]) == 0) {
        return ensemble_test(argc, argv);
    }

    if (strcmp("jacobian", argv[1]) == 0) {
        return jacobian_test(argc, argv);
    }

    if (strcmp("cm_1", argv[1]) == 0) {
        return cm_1_test(argc, argv);
    }

    if(strcmp("path", argv[1]) == 0) {
        return path_test(argc, argv);
    }

    if(strcmp("reset", argv[1]) == 0) {
        return reset_test(argc, argv);
    }

    if(strcmp("validate", argv[1]) == 0) {
        return validate_test(argc, argv);
    }

    if(strcmp("distrib", argv[1]) == 0) {
        return distrib_test(argc, argv);
    }

    if(strcmp("variant", argv[1]) == 0) {
        return variant_test(argc, argv);
    }

    if(strcmp("options", argv[1]) == 0) {
        return intparam_test(argc, argv);
    }


    if(strcmp("matnames", argv[1]) == 0) {
        return matnames_test(argc, argv);
    }

    std::cout << "error, invalid test name: " << argv[1] << std::endl;
    return -1;
}






