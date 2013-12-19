**************
Basic Tutorial
**************

.. currentmodule:: roadrunner

Import RoadRunner
-----------------

To startup roadRunner use the commands at the Python prompt::

   import roadrunner
   rr = roadrunner.RoadRunner()
  
The variable ``rr`` is your reference to roadrunner. Anything you want to do with roadrunner must be 
specified using the variable ``rr``.

To get the current version of libRoadRunner type the following::
   
   roadrunner.__version__

.. _loading-models:

Loading Models
--------------

RoadRunner reads models using the SBML format. If you have a SBML model stored on your hard drive, it is 
possible to load that model using the method, ``load()``. Let's assume you have a model called
mymodel.xml in ``C:\MyModels``. To load this model in **Windows** we would use the command::

   rr.load("C:/MyModels/mymodel.xml")

Note, Windows typically used the back slash, '\\' to indicate a directory separator, however in
most languages including python, this is the escape character, therefore one can also enter Windows
paths using the forward slash '/' which does not cause issues. If one want to use the backslash,
these must be typed twice, i.e. ``'C:\\MyModels\\mymodel.xml'``.

On the **Mac or Linux** we might use one of these two commands::

   rr.load("/home/MyModels/mymodel.xml")
   
   rr.load("http://www.ebi.ac.uk/biomodels-main/download?mid=BIOMD0000000010") #Note: Not available on Windows

If the model was loaded successfully the load method will return ``True`` otherwise an exception will 
be raised that contains extendend information detailing exactly what failed. If any warnings are
found in the SBML document, these will be displayed in the console error log. 

Additionally, there are a couple models **included with libRoadRunner**. The models  ``feedback.xml`` 
and ``Test_1.xml`` are available in the ``roadrunner.testing`` module. To access these use:

.. code-block:: python
   :emphasize-lines: 2,3

   import roadrunner
   import roadrunner.testing
   rr.load(roadrunner.testing.getData('feedback.xml'))
   
There are a few additional models in the ``models/`` directory of the distribution, where you installed libRoadRunner.

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
  
will printout the current value for steps. The follow table summarizes the various options.
 
================  =============
 Option           Description
================  =============
start             Start time for simulation
end               End time for simulation. Setting 'end' will automatically change 'duration'
duration          Duration of the simulation. Setting 'duration' will automatically change 'end'
steps             Number of steps to generate
absolute          Absolute tolerance for the CVODE integrator
relative          Relative tolerance for the CVODE integrator
stiff             Tells the integrator to use the fully implicit backward difference stiff solver
resetModel        Resets the sbml state to the original values specified in the sbml. 
structuredResult  If set (default is True), the result from simulate is a numpy structured array 
                  with the column names set to the selections. This is required for plotting and 
                  displaying a legend for each time series. 
================  =============

One important point to note about simulate(). When simulate() is run, the concentration of the floating species 
will naturally change. If simulate() is called a second time, the simulation will use as the initial conditions the last 
values from the previous simulation. This can be used to easily to follow on simulations. However there will be times
when we wish to run the same simulation again but perhaps with a slightly different parameters value. To do
this we must reset the initial conditions back to the original values. To do that we run the command reset::
   
   rr.reset()

Changing Parameters
-------------------

Often during a modeling experiment we will need to change parameter values, that is the values of the various
kinetic constants in the model. If a model has a kinetic constants k1, then we can change or inspect the value using
the following syntax::
   
   print rr.model.k1
   rr.model.k1 = 1.2
  

Selecting Simulation Output
---------------------------

RoadRunner supports a range of options for selecting what data a simulation should return. For more
detailed information on selections, see the :ref:`selecting-values` section.

The simulate method, by default returns an `structured array
<http://docs.scipy.org/doc/numpy/user/basics.rec.html>`_,
which are arrays that also contain column names. These can be ploted directly using the
``roadrunner.plot`` function. 

The output selections default to time and the set of floating species. 
It is possible to change the simulation result values by changing the selection list. 
For example assume that a model has three species, S1, S2, and S3 but we only want simulate() to
return time in the first column and S2 in the second column. To specify this we would type::
   rr.selections = ['time', 'S2']
   result = rr.simulate (0, 10, 100)
  
In another example let say we wanted to plot a phase plot where S1 is plotted against S2. To do this we 
type the following::

   rr.selections = ['S1', 'S2']
   result = rr.simulate(0, 10, 100)

.. seealso:: More details on :doc:`selecting_values`   

.. _plotting-data:

Plotting Data
-------------

The built in roadrunner.plot function displays the simulation result and a legend using
matplotlib. Simply pass it a simulation result::
   
   result = rr.simulate(0, 10, 100)
   roadrunner.plot(result)

If one wants more control over the data plots, or if one wished to pass the simulation result to
other ``numpy`` functions that expect a **unstructured (conventional) array**, disable the structured array 
result with::
   
   rr.simulateOptions.structuredResult = False

This will return a unstructured array. Assuming the simulate returns an array called result, and that the first column represents
the x axis and the remaining columns the y axis, we type::

   import pylab
   pylab.plot (result[:,0],result[:,1:])
   pylab.show()

This will bring up a new window showing the plot. To clear the plot for next time, type the command::
   
   pylab.clf()

Plot a graph using normal numpy arrays and will use the selections from roadrunner to add a legend.

Below is a convenient function to plot with a legend. The first argument must be a roadrunner variable. 
The second argument must be an array containing data to plot. The first column of the array will be the 
x-axis and remaining columns the y-axis. Returns a handle to the plotting object.

Copy the code below into your own python script and use it::

   def plotWithLegend (r, result):
       """
       Plot an array and include a legend. The first argument must be a roadrunner variable. 
       The second argument must be an array containing data to plot. The first column of the array will
       be the x-axis and remaining columns the y-axis. Returns
       a handle to the plotting object.
       
       plotWithLegend (r, result)
       """
       if not isinstance (r, roadrunner.RoadRunner):
           raise Exception ('First argument must be a roadrunner variable')
       columns = result.shape[1]
       legendItems = r.selections[1:]       
       if columns-1 != len (legendItems):
           raise Exception ('Legend list must match result array')
       for i in range(columns-1):
           plt.plot (result[:,0], result[:,i+1], linewidth=2.5, label=legendItems[i])
       plt.legend (loc='upper left')    
       plt.show()
       return plt


.. todo:: add the section

   Changing Initial Conditions
   ---------------------------

   To be completed


.. highlight:: python