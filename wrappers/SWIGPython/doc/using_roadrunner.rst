**************************
Basic Tutorial
**************************

.. currentmodule:: roadrunner

Load RoadRunner
---------------

To startup roadRunner use the commands at the Python prompt::

  import roadrunner
  rr = roadrunner.RoadRunner()
  
The variable rr is your reference to roadrunner. Anything you want to do with roadrunner must be 
specified using the variable rr. For exmaple, to get the current version of roadRunner type the
following::

  rr.getVersion()

Loading Models
--------------

RoadRunner reads models using the SBML format. If you have a SBML model stored on your hard drive, it is 
possible to load that model using the method, loadSBMLFromFile. Let's assume you have a model called
mymodel.xml in C:\MyModels. To load this model in Windows we would use the command::

  rr.loadSBMLFromFile("C:\MyModels\mymodel.xml")

On the Mac or Linux we might use the command::

  rr.loadSBMLFromFile("/home/MyModels/mymodel.xml")

If the model was loaded successfully the load method will return True otherwise it will return False.
*TO DO: How to get the error message*

Running Simulations
-------------------

Once a model is successfuly loaded we can next run a time course simulation. To do this we use the
simulate method::

  result = rr.simulate()

The variable result will be a Python numpy array. The first column will contain time and the reminaing columns will include
all the floating species. In the simulate method we didn't specify how long to do the simulation for or how many 
points to generate.  By default the time start is set to zero, time end to 40 time units and the number of points to 500.
There are two ways to set these values to different values. The easiest is to add them to the called in the following
way::

  result = rr.simulate (0, 10, 100)
  
This means set the time start to zero, the time end to 10 and generate 100 points. This means that the simualation points
will be output in intervals of 0.1.

The alternative is to set the options record in simulateOptions. The advantage here is that the simulateOptions record
holds many other options that might be of interest. To set the time start using the simulationOptions record we
would type::

  rr.simulateOptions.start = 0
  
Or to set the many points to use we would use::
  
  rr.simulateOptions.steps = 100
  
And to set the time end for the simulation we would type::

  rr.simulateOptions.end = 10

Typing somethng like::

  print rr.simulateOptions.steps
  
will printout the current value for steps.
 
=====    ======
Option   Description
=====    ======
start    Start time for simulation
end      End time for simulation
steps    Number of steps to generate
=====   ======

One important point to note about simulate(). When simulate() is run, the concentration of the floating species 
will naturally change. If simulate() is called a second time, the simulation will use as the initial conditions the last 
values from the previous simulation. This can be used to easily to follow on simulations. However there will be times
when we wish to run the same simulation again but perhaps with a slightly different parameters value. To do
this we must reset the initial conditions back to the original values. To do that we run the command reset::

  rr.reset()

Changing Parameters
-------------------

Often during a modeling experiment we will need to change parameter values, that is the values of the various
kinetic constants in the model.


Changing Initial Conditions
---------------------------




