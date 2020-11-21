########################
RoadRunner Documentation
########################

The Roadrunner Simulatin Engine is a C++ library for simulating and analysing
systems of differential equations. Roadrunner was designed
with performance as a priority and is an exceptionally fast simulator.

Features
========

* Time Dependent Simulation (with optional conservation law reduction) using CVODE from the sundials suite.
* Steady State evaluation using NLEQ2
* Supports SBML Level 2 to 3 but currently excludes algebraic rules and delay differential equations
* Uses latest libSBML distribution
* Defaults to LLVM code generation on the backend, resulting is very fast simulation times
* Optional generation of model C code and linking at run-time
* Add plugins, distribution comes with Levenberg-Marquardt optimizer plugin
* Supports Metabolic Control Analysis
* Supports Frequency Domain Analysis
* Additional Plugin API (C and C++)
* APIs support fast access to model quantities
* Access to other items such as:
* Eigenvalues and Eigenvectors
* Jacobian, full and reduced
* Structural Matrices of the stoichiometry matrix

.. todo::

    todo link to figure comparing speed of roadrunner with other simulators. We want to sell it to people afterall.

Installation
=============
For instructions on how to install roadrunner please see the :ref:`installation section <Installing RoadRunner>`.

Quickstart
==========

To get you started here is a very simple script that will load an example SBML model and run a time course
simulation and plot the results.

.. code-block:: python

   import roadrunner

   # load an SBML model
   rr = roadrunner.RoadRunner("mymodel.xml")

   # simulate from 0 to 10 time units with 100 output rows
   result = rr.simulate(0,10,100)

   rr.plot()


Looking for a model to start with? We included a couple with libRoadRunner, see :ref:`loading-models`.


.. toctree::
    :maxdepth: 1
    :hidden:

    self
    introduction
    Installation/installation
    read_write_functions
    selecting_values
    steady_state
    stochastic
    metabolic
    stability
    stoichiometric
    bifurcation
    integration
    utility_functions
    accessing_model
    api_reference
    C++APIReference/index
    CAPIReference/index






Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`


.. highlight:: python
