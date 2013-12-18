.. RoadRunner documentation master file, created by
   sphinx-quickstart on Sun Sep 29 08:52:48 2013.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

########################
libRoadRunner User Guide
########################


libRoadRunner Overview
======================

This guide is intended as an introduction and reference of the RoadRunner
SBML Simulation Engine. This guide will show the most important features 
and provides a complete API reference. 


To get you started here is a very simple script that will load an example SBML model and run a time course
simulation and plot the results::

   import roadrunner
   import roadrunner.testing
   rr = roadrunner.RoadRunner()
   rr.load(roadrunner.testing.get_data('feedback.xml'))
   result = rr.simulate()
   roadrunner.plot(result)
 
The result is a standard numpy array which may be plotted using `matplotlib
<http://matplotlib.org/>`_. Now read the tutorials to learn more about the capabilities of RoadRunner. 

Contents:

.. toctree::

   introduction
   read_write_functions
   steady_state
   stoichiometric
   metabolic
   stability
   accessing_model
   utility_functions
   api_reference

Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`


.. highlight:: python