Performance Test Suites
=================================
This directory contains a python package designed to test 
roadrunner's performance in different situations. 

We want to know:
    * How long does it take to build a roadrunner model?
    * How long does it take to simulate a roadrunner model? (discounting build time).
    * How long does it take to build and simulate a roadrunner model? 

More importantly, we want to how the answer to these questions change 
with different sbml conditions, such as size and connectivity. 

Currently (as of 18-11-2021) there are three model series generators:
    * NLinearChain.generateNLinearModels
    * NReactionsUncoupled.generateModelsWithNUncoupledReactions
    * NSpeciesIncreasingConnectivity.generateModelsWithNSpeciesIncreasingConnectivity

These functions are Python generators that sequentially return 
bigger or more complicated models. These generators all have a single 
input parameter `n` which determines the scale of the current model series. 

.. note:: 
    NLinearChain and NReactionsUncoupled both scale linearly with `n`. That is, 
    The last model in the series will have N species or N reactions. 
    NSpeciesIncreasingConnectivity on the other hand scales quadratically.

Importantly, new model generators should adhere to the same interface 
so that the test code in performance_tests.py still work. 

NLinearChain
------------
N represents the number of species in a first order 
mass action linear chain. 

If N = 3: 

    S1 -> S2 -> S3

All kinetic parameters are set to 0.1. 

NReactionsUncoupled
--------------------
N represents the number of reactions. The reactions are 
all first order mass action and they are uncoupled. In 
other words, they are mutually exclusive mini-networks
inside one big network.

Aka for N = 3:

    model NReactions2
        // species
        S0 = 10;
        S1 = 10;
        S2 = 10;
        S3 = 10;
        S4 = 10;
        S5 = 10;
        // parameters
        k0 = 0.1;
        k1 = 0.1;
        k2 = 0.1;
        // reactions
        r0: S0 => S1; k0*S0;
        r1: S2 => S3; k1*S2;
        r2: S4 => S5; k2*S4;
    end


NSpeciesIncreasingConnectivity
---------------------------------
N represents the number of species in the network. This time, 
N is fixed -- each model that is generated has N species. What 
changes is the inter-connectedness between the species. Its easiest
to think of this as a square link matrix with increasing 1's 
across the rows. 

Aka, if N = 3.

    Link matrix for model 1: 
            S0  S1  S2
        S0  1   0   0
        S1  0   0   0
        S2  0   0   0

    Link matrix for model 2: 
            S0  S1  S2
        S0  1   1   0
        S1  0   0   0
        S2  0   0   0

    Link matrix for model 3: 
            S0  S1  S2
        S0  1   1   1
        S1  0   0   0
        S2  0   0   0
    
    ... and so on

    A "1" represents a first order mass action reaction from 
    substrate (columns) to products (rows) and a "0" is no such
    reaction. 


Conditions
-----------
All test suites and generators have been executed using different conditions.
* RoadRunner2.1.3 was run with the latest roadrunner package from pypi
* RoadRunner_MCJit was run with the latest commit from the orc-jit branch
  * Uses LLVM-13
  * Has a MCJit class
* LLJit is the prepackaged llvm Jit compiler that is available for use.
  It is multithreadded if you want and has 4 optimization levels, None, 
  Less, Default and Aggressive. The following conditions with LLJit have 
  been run:
  * Aggressive with 1 thread
  * Aggressive with 16 thread
  * None with 1 thread
  * None with 16 thread









