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

or on the Mac or Linux we would use the command

  rr.loadSBMLFromFile("/home/foo/myfile.xml")

If the model was loaded successful the load method will return True otherwise it will return False.
*TO DO: How to get the error message*

Running Simulations
-------------------

Once a model is successfuly loaded we can next run a time course simualation. To do this we can jsut use the
simulate method::

  result = rr.simulate()

The variable result will be a Pythn numpy array. The first column will contain time and the reminaing columns will include
all the floating species.


