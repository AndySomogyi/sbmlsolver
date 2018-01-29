SBML Solver
===========


Copyright 2018

SBML Solver is high performance and portable simulation engine for systems and
synthetic biology, which is a continuation of the original libRoadRunner
project. 

SBML Solver 1.0 supports the following features
-----------------------------------------------

- Time Dependent Simulation (with optional conservation law reduction) using CVODE
- Supports SBML Level 2 to 3 but currently excludes algebraic rules and delay differential equations
- Uses latest libSBML distribution
- Defaults to LLVM code generation on the backend, resulting is very fast simulation times
- Optional generation of model C code and linking at run-time
- Add plugins, distribution comes with Levenberg-Marquardt optimizer plugin
- Compute steady state
- Metabolic Control Analysis
- Frequency Domain Analysis
- Access to:
  - Eigenvalues and Eigenvectors
  - Jacobian, full and reduced
  - Structural Matrices of the stoichiometry matrix


Availability
------------

SBML Solver is licensed for free as an open source programmatic library for use in other 
applications and as a standalone command line driven application. Its C++ API, C API, and 
Python APIs have comprehensive documentation. On Windows, OS X, and Linux binary files can be 

downloaded from <https://github.com/AndySomogyi/sbmlsolver> ready for use.


Acknowledgements
----------------

This work is funded by NIGMS grant: GM081070

Licence

Licensed under the GNU Library General Public License (LGPL), Version 2.0 (the License); you may not use this 
file except in compliance with the License. You may obtain a copy of the License at


YOU CANNOT: redistribute any piece of this software without proper attribution;

