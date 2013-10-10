******************
What is RoadRunner
******************

RoadRunner is package for loading, JIT compilation, simulation and
analysis of SBML systems biology models. 


So, here is a simple example:



The new API is really cool, this is all you have to do::

  import RoadRunner as rr
  import pylab as p
  r = rr.RoadRunner()

this takes an optional LoadSBMLOptions class as a second parameter::

  In Windows
  
  r loadSBMLFromFile("C:\MyModels\mymodel.xml")

  or on the Mac or Linux
  
  r.loadSBMLFromFile("/home/foo/myfile.xml")
  

this takes an optional SimulateOptions class as a second parameter.::

  s = r.simulate()


Hi,

Here is a transcript of an interactive ipython session::

  import RoadRunner as rr
  rr?
  Type:       module
  String Form:<module 'RoadRunner' from 'RoadRunner.pyc'>
  File:       /Users/andy/.local/lib/Python/RoadRunner.py
  Docstring:
  The RoadRunner SBML Simulation Engine,
  (c) 2009-2013 Herbert Sauro, Andy Somogyi and Totte Karlsson
      
  rr.RoadRunner?
  Type:       type
  String Form:<class 'RoadRunner.RoadRunner'>
  File:       /Users/andy/.local/lib/Python/RoadRunner.py
  Docstring:
  The main RoadRunner class.

All three of the RoadRunner options default to the empty string, in this
case, the default values are used.

usual import stuff::

  import roadrunner as rr
  import numpy as n
  import numpy.linalg as lin

make a RoadRunner and load a doc::

  r = rr.RoadRunner()
  r.loadSBMLFromFile("/Users/andy/src/sbml_test/cases/semantic/00110/00110-sbml-l3v1.xml")
  True

get the model, the model obj has all the accessors sbml elements names / values::

  m = r.getModel()

use the build in RR function to get the Jacobian, notice this is returned as a native
numpy matrix::

  jac = r.getFullJacobian()

display it::

  jac
  array([[ 0.75,  0.5 ,  0.25],
  [-0.75, -0.75,  0.  ],
  [ 0.  ,  0.25, -0.25]])

get a vector of floating species amounts::

  amt = m.getFloatingSpeciesAmounts()

dsplay it::

  amt
  array([ 1.,  1.,  0.])

look at the floating species ids::

  m.getFloatingSpeciesIds()
  ['S1', 'S2', 'S3']

numpy has a huge amount of numeric capability, here we calculate
the eigensystem from the Jacobian.::

  lin.eig(jac)
  (array([ -5.75693909e-01,   3.25693909e-01,  -8.55019192e-17]),
  array([[-0.18130286, -0.79611302, -0.57735027],
       [ 0.78010553,  0.55506939,  0.57735027],
       [-0.59880267,  0.24104363,  0.57735027]]))

suppose we wanted to calculate the matrix vector product of the jacobian with the 
floating species amounts, its a single statement now that we use native types.::

  n.dot(jac, amt)
  Out[13]: array([ 1.25, -1.5 ,  0.25])


first column in result is time, rest are whatever is selected.::

  p.plot(s[:,0], s[:,1:])

stored in two Python lists, ``a`` and ``b``, we could iterate over
each element::

  c = []
  for i in range(len(a)):
      c.append(a[i]*b[i])

This produces the correct answer, but if ``a`` and ``b`` each contain

and initializations, memory allocation, etc.)

::

  for (i = 0; i < rows; i++): {
    c[i] = a[i]*b[i];
  }

This saves all the overhead involved in interpreting the Python code
and manipulating Python objects, but at the expense of the benefits
gained from coding in Python.  Furthermore, the coding work required
increases with the dimensionality of our data. In the case of a 2-D
array, for example, the C code (abridged as before) expands to

::

  for (i = 0; i < rows; i++): {
    for (j = 0; j < columns; j++): {
      c[i][j] = a[i][j]*b[i][j];
    }
  }

NumPy gives us the best of both worlds: element-by-element operations
are the "default mode" when an `ndarray` is involved, but the
element-by-element operation is speedily executed by pre-compiled C
code.  In NumPy

::

  c = a * b

does what the earlier examples do, at near-C speeds, but with the code
simplicity we expect from something based on Python (indeed, the NumPy


Technical Aspects
-----------------

The roadrunner package contains two fundametal objects, `RoadRunner`
and `ExecutableModel`. `RoadRunner` is the typically the top level object,
it is responsible for orchestrating all of the internal components 
such as model loading, JIT compilation, integration and output. 

** roadrunner overview

** ExecutableModel
 - represents a compiled sbml model
 - properties to get and set any state variables.


I've got much of the new Python API working, its very clean simple interface that uses all native Python objects. All the returned types are Numpy arrays. Much of the time, the Numpy array just holds a pointer to a block of data owned by RoadRunner, for example, the
array returned by simulate just has a pointer to the results matrix which is owned by RoadRunner, so there is NO COPYING involved. If you have no need for the result, just ignore it, it costs virtually nothing to return it. 
