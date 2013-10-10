**************************
Basic Tutorial
**************************

.. currentmodule:: roadrunner

Load RoadRunner
---------------

To load and startup roadRunner in python use the commands::

  import roadrunner
  rr = roadrunner.RoadRunner()
  
The variable rr is your reference to roadrunner. Anything yu want to do with roadrunner must be 
specified by the variable rr.

Loading Models
--------------

RoadRunner reads m odels using the SBML format. If you have a SBML model stored on your hard drive, it is 
possible to load that model using the method, loadSBMLFromFile. Let's assume there is a model called
mymodel.xml. To laod this model in windows we would use the command::

  rr.loadSBMLFromFile("C:\MyModels\mymodel.xml")

or on the Mac or Linux we would use the command::

  rr.loadSBMLFromFile("/home/foo/myfile.xml")

If the model was loaded successful the load method will return True otherwise it will return False.
*TO DO: How to get the error message*

Running Simulations
-------------------

Once a model is successfuly loaded we can next run a time course simualation. To do this we can jsut use the
simulate method::

  result = rr.simulate()

The variable result will be a Pythn numpy array. The first column will contain time and the reminaing columns will include
all the floating species. In the simulate method we didn't specify how long to do the simulation for or how many 
points to generate.

To finout more about the options used by simulate we can involve the simulateOptions command. For example to find
out how long the simulation was run for we can use the command::

  rr.simulateOptions.end
  
Or to find out how many points were used we can use the command:
  
  r.rsimulateOptions.steps
  
By default the time start is set to zero, time end to 40 time units and the number of points to 500. To quickly
overrride this defaults it is possible to include these argumetns in the simulate command, for example:

  result = rrsimulate (0, 10, 100)
  
This means set the time start to zero, the time end to 10 and generate 100 points. This means that the simualation points
will be output in intervals of 0.1.

One important point to note about simulate(). When simualte() is run, the concentration of the floating species 
will naturally change. If simulate() is called a second time, the simulation will use as initial conditions the last 
values from the previous simulation. To reset the initial conditions back to the values there were in the orginal model
run the command reset::

  rr.reset()






