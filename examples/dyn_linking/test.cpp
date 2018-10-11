#include <iostream>
#include <MxReactionNetwork.h>


int main(int argv, const char** argc) {


    if(argv != 2) {
        std::cout << "usage: test /absolute/path/to/sbml/file" << std::endl;
        return -1;
    }


    // The basic way of creating a solver in C is:
    // ISBMLSolver *solver = NULL;
    ISBMLSolver *solver = NULL;

    std::cout << "creating solver, loading file \"" << argc[1] << "\"" << std::endl;
    HRESULT result = MxCreateSBMLSolver(argc[1], NULL, &solver);

    if(SUCCEEDED(result)) {
        std::cout << "we’re good, made a solver, ready to use it" << std::endl;
    }
    else {
        std::cout << "failed to make a solver, need to check the error code, figure out what happened" << std::endl;
        return -1;
    }

    // grab the scalar system (the compiled SBML ode model) from the solver
    IScalarSystem *system = 0;
    result = solver->getScalarSystem(&system);
    if(SUCCEEDED(result)) {
        std::cout << "ready to use the scalar system, connect it to our compartment" << std::endl;
    }
    else {
        std::cout << "this really should never happen" << std::endl;
    }
    return 0;
}
