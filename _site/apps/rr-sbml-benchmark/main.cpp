// Roadrunner sbml test program which takes the same argument format as
// COPASIs and does the same thing.
//
// basically all this program does is read the config from the sbml test
// suite and dump out a csv of the results, nothing more.
// the actuall RoadRunner sbml test suite does much more as it
// also generates a formated report and compares results to
// the sbml test suite.

// Copyright (C) 2013 Andy Somogyi
// Indiana University, University of Washington

#include "rrRoadRunner.h"
#include "rrRoadRunnerOptions.h"

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <sstream>
#include <set>
#include <stdlib.h>

using namespace rr;

// copasi test suite called like
// ./sbml-testsuite ~/src/sbml_test/cases/semantic 00221 ~/tmp/copasi 2 4

int main(int argc, char** argv)
{
    std::string in_dir;
    std::string out_dir;
    std::string test_name;
    unsigned int level = 0;
    unsigned int version = 0;
    double start_time = 0.0;
    double duration = 0.0;
    unsigned int steps = 0;
    std::list<std::string> variables;
    std::set<std::string> amounts;
    std::set<std::string> concentrations;
    double absolute_error = 0.0;
    double relative_error = 0.0;


    if (argc < 6)
    {
        std::cerr << "Usage: rr-sbml-benchmrk INPUT_DIRECTORY TESTNAME OUTPUTDIRECTORY SBMLLEVEL SBMLVERSION [steps override] [durration] [compiler]" << std::endl;
        exit(1);
    }

    in_dir = argv[1];
    test_name = argv[2];
    out_dir = argv[3];
    level = strtol(argv[4], NULL, 10);
    version = strtol(argv[5], NULL, 10);

    if (level < 1 || level > 3)
    {
        std::cerr << "SBML Level " << level << " Version " << version << " not supported." << std::endl;
        exit(1);
    }

    if (level == 1 && (version < 1 || version > 2))
    {
        std::cerr << "SBML Level " << level << " Version " << version << " not supported." << std::endl;
        exit(1);
    }

    std::ostringstream os;
    os << in_dir << "/" << test_name << "/" << test_name << "-settings.txt";
    // open the settings file and parse it
    std::cout << "parsing file: " << os.str() << std::endl;
    std::ifstream f;

    std::string settingsFile = in_dir + "/" + test_name + "/" + test_name + "-settings.txt";
    std::string sbmlFile;

    {
        stringstream os;
        os << in_dir << "/" << test_name << "/" << test_name << "-sbml-l" << level << "v" << version << ".xml";
        sbmlFile = os.str();
    }

    std::string output_filename;
    if (!out_dir.empty() || out_dir.compare("/dev/null") != 0)
    {
        stringstream os;
        os.str("");
        os << out_dir << "/" << test_name << ".csv";
        output_filename = os.str();
    }

    RoadRunner roadRunner;

    LoadSBMLOptions opt;

    // don't generate cache for models
    opt.modelGeneratorOpt = opt.modelGeneratorOpt & ~LoadSBMLOptions::RECOMPILE;

    roadRunner.load(sbmlFile, &opt);

    SimulateOptions settings(settingsFile);


    // override steps
    if (argc > 6)
    {
        steps = strtol(argv[6], NULL, 0);
        settings.steps = steps;
    }

    if (argc > 7)
    {
        settings.duration = atof(argv[7]);
    }

    roadRunner.setSimulateOptions(settings);

    const RoadRunnerData& data = *roadRunner.simulate(0);
    const DoubleMatrix& result = data.getData();

    if (!output_filename.empty())
    {
        ofstream output;
        output.open(output_filename.c_str());

        int rows = result.numRows();
        int cols = result.numCols();

        for (int i = 0; i < rows; ++i)
        {
            for (int j = 0; j < cols; ++j)
            {
                output << result(i, j);

                if (j + 1 < cols)
                {
                    output << ", ";
                }
                else
                {
                    output << "\n";
                }
            }
        }
    }

    return 0;
}
