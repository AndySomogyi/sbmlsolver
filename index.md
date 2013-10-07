---
layout: page
title: Documentation
tagline: speedy simulation engine
---

a high performance and portable simulation engine for systems and synthetic biology.

RoadRunner 1.0 supports the following features:

 - Time Dependent Simulation (with optional conservation law reduction)
 - Supports SBML Level 2 to 3 but currently excludes algebraic rules and delay differential equations.
 - Generation of model C code and linking at run-time. Future version will provide an optional interpreter for systems where run-time compilation is forbidden, eg iOS mobile platform.
 - Steady State
 - Metabolic Control Analysis
 - Frequency Domain Analysis
 - Access to

    * Eigenvalues and Eigenvectors
    * Jacobian
    * Structural Matrices relation to the Stoichiometry matrix

RoadRunner is provided in the form of:

 - Standalone command line driven application
 - As a library, static or dynamic to be linked in to an existing application
 - Simulations can be run either in batch or interactive modes
 - Supplied with an extensive C++ as well as a C API
 - Supplied with a Python interface

Future expansion:

 - Builtin Interpreter
 - Data fitting
 - Bifurcation Analysis
 - Loading and saving of SED-ML

This work is funded by NIGMS grant: GM081070

## Licence
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at

[http://www.apache.org/licenses/LICENSE-2.0](http://www.apache.org/licenses/LICENSE-2.0)

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.

In plain english this means:

You CAN freely download and use this software, in whole or in part, for personal, company internal, or commercial purposes;

You CAN use the software in packages or distributions that you create.

You SHOULD include a copy of the license in any redistribution you may make;

You are NOT required include the source of software, or of any modifications you may have made to it, in any redistribution you may assemble that includes it.

YOU CANNOT:
redistribute any piece of this software without proper attribution;
