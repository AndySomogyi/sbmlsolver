**************************
Metabolic Control Analysis
**************************

.. currentmodule:: roadrunner

Preliminaries
-------------

Metabolic control analysis is the study of how sensitive the system is to pertubration and how those perturbations
progagate through the network. Two kinds of sensitivity are defined, systems and local. The local sensitivities are
described by the elasticities. These are defined as follows:

:math:`\varepsilon^v_S = \frac{\partial v}{\partial S} \frac{S}{v} = \frac{\partial \ln v}{\partial \ln S} </math>`

Given a reaction rate :math:`v_i`, the elasticity describes how a given effector of the reaction step affects
the reaction rate. Because the definition is in terms of partial deriviatives, any effector that is perturbed assumes
that all other potential effectors are unchanged.

The system sensitivities are described by the control and response coefficients. These come in two forms, flux
and concentration. The flux control coefficients measures how senstive a given flux is to a perturbation in the
local rate of a reaction step. Often the local rate is perturbed by changing the enzyme concdntration at the step. 
In this siutation the flux control coefficident is defined as follow:

:math:`C^J_{E_i} = \frac{\partial J}{\partial E_1} \frac{E_1}{J} = \frac{\partial \ln J}{\partial \ln E_i} </math>`

Likewise the concentration control coefficient is defined by:

:math:`C^S_{E_i} = \frac{\partial J}{\partial E_1} \frac{E_1}{S} = \frac{\partial \ln S}{\partial \ln E_i} </math>`

where :math:`S` is a given species. 

The following methods allow users to obtain various metabolic control coefficients.

.. autosummary::

   RoadRunner.getUnscaledFluxControlCoefficientMatrix
   RoadRunner.getUnscaledConcentrationControlCoefficientMatrix
   RoadRunner.getUnscaledElasticityMatrix
   RoadRunner.getUnscaledSpeciesElasticity
   RoadRunner.getScaledElasticityMatrix
   RoadRunner.getScaledFloatingSpeciesElasticity
   RoadRunner.getScaledFluxControlCoefficientMatrix
   RoadRunner.getScaledConcentrationControlCoefficientMatrix
   RoadRunner.getuCC
   RoadRunner.getCC
   RoadRunner.getuEE
   RoadRunner.getEE
