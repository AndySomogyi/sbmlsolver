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
   rr = roadrunner.RoadRunner()
   rr.load("mymodel.xml")
   result = rr.simulate()
   roadrunner.plot(result)

Looking for a model to start with? We included a couple with libRoadRunner, see :ref:`loading-models`.   

.. note:: The result is a structured numpy array which cannot be plotted using `matplotlib <http://matplotlib.org/>`_ directly. See :ref:`plotting-data` for help in plotting. 

Now read the tutorials to learn more about the capabilities of RoadRunner.  

Contents:

.. toctree::

   introduction
   read_write_functions
   selecting_values
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