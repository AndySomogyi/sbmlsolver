.. RoadRunner documentation master file, created by
   sphinx-quickstart on Sun Sep 29 08:52:48 2013.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

#####################
RoadRunner User Guide
#####################


RoadRunner Overview
===================

This guide is intended as an introduction and reference of the RoadRunner
SBML Simulation Engine. This guide will show the most important features 
and provides a complete API reference. 


.. warning::

   This "User Guide" is still a work in progress; some of the material
   is not organized, and several aspects of RoadRunner are not yet covered
   sufficient detail.     <http://andysomogyi.github.io/roadrunner/pydoc/>`__ website.

To get you started here is a very simple script that will load a model and run a time course simulation::

  import roadRunner
  rr = roadrunner()
  rr.loadSBMLFromFile ("c:\mymodel.xml")
  result = rr.simulate()
 
You can plot your results using matplotlib. Now read the tutorials to learn more about the capabilities of roadRunner. 

Contents:

.. toctree::

   introduction
   rr_basics
   timecourse_simulation
   read_write_functions
   accessing_model
   stoichiometric
   stability
   metabolic
   steady_state
   api_reference

Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`


