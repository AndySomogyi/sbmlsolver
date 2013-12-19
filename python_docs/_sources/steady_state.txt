*********************
Steady State Analysis
*********************

.. currentmodule:: roadrunner

The dynamics of a biochemical network is described by the system equation

.. math::

   \frac{d}{dt}\mathbf{s}(t) = \mathbf{N} \mathbf{v}(\mathbf{s}(t),\mathbf{p},t), 

where :math:`\mathbf{s}` is the vector of species concentrations, :math:`\mathbf{p}` 
is a vector of time independent parameters, and :math:`t` is time. The steady state is the solution to
the network equation when all the rates of change zero. That is the concentrations of the floating
species, :math:`\mathbf{s}` that satisfy:

.. math::

   \mathbf{N} \mathbf{v}(\mathbf{s}(t),\mathbf{p},t) = 0 

For example you can use ``steadyStateSelections`` to select which value ``getSteadyStateValues`` should retrieve::

     >>> rr.steadyStateSelections = ['S1']
     >>> rr.getSteadyStateValues ()
     array([ 0.54314239])


The following methods deal wth steady state analysis:

.. autosummary::

   RoadRunner.steadyStateSelections

   RoadRunner.steadyState

   RoadRunner.getSteadyStateValues
   
