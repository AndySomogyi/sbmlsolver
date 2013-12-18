*********************
What is libRoadRunner
*********************

RoadRunner is package for loading, JIT compilation, simulation and
analysis of SBML systems biology models. 


RoadRunner 1.0
Up to date documentation can be found on http://libroadrunner.org/
Also the static `documentation home page <../index.html>`_ provides an introduction.


Licence and Copyright
---------------------
libRoadRunner is free and open source. Licensing and Distribution 
Terms can be found in the LICENCE.txt file in the root directory 
of the distribution.

Copyright (C) 2012-2013 University of Washington, Seattle, WA, USA

Licensed under the `Apache License, Version 2.0`_

.. _Apache License, Version 2.0: http://www.apache.org/licenses/LICENSE-2.0.html
 
http://libroadrunner.org/

Technical Details
-----------------

Here is a transcript of an interactive ipython session:

.. import roadrunner as rr
..  rr?
..  Type:       module
..  String Form:<module 'RoadRunner' from 'RoadRunner.pyc'>
..  File:       /Users/andy/.local/lib/Python/RoadRunner.py
..  Docstring:
..  The RoadRunner SBML Simulation Engine,
..  (c) 2009-2013 Herbert Sauro, Andy Somogyi and Totte Karlsson
..      
..  rr.RoadRunner?
..  Type:       type
..  String Form:<class 'RoadRunner.RoadRunner'>
..  File:       /Users/andy/.local/lib/Python/RoadRunner.py
..  Docstring:
..  The main RoadRunner class.
..
..  All three of the RoadRunner options default to the empty string, in this
..  case, the default values are used.


import libroardunner::

   import roadrunner
   import roadrunner.testing
   import numpy as n
   import numpy.linalg as lin

make a RoadRunner and load an SBML model::

   rr = roadrunner.RoadRunner()
   rr.load(roadrunner.testing.get_data('Test_1.xml'))
   True

get the model, the model obj has all the accessors sbml elements names / values::
   
   m = rr.getModel()

Use the built in RR function to get the Jacobian, notice this is returned as a native
numpy matrix::
   
   jac = rr.getFullJacobian()

display it::

   jac
   array([[-0.2  ,  0.067,  0.   ],
          [ 0.15 , -0.467,  0.09 ],
          [ 0.   ,  0.4  , -0.64 ]])

get a vector of floating species amounts::

   amt = m.getFloatingSpeciesAmounts()

dsplay it::
   
   amt
   array([ 0.1 ,  0.25,  0.1 ])

look at the floating species ids::
   
   m.getFloatingSpeciesIds()
   ['S1', 'S2', 'S3']

numpy has a huge amount of numeric capability, here we calculate
the eigensystem from the Jacobian.::
   lin.eig(jac)
   (array([-0.15726345, -0.38237134, -0.76736521]),
    array([[ 0.77009381, -0.19510707,  0.03580588],
           [ 0.49121122,  0.53107368, -0.30320915],
           [ 0.40702219,  0.82455683,  0.95225109]]))

suppose we wanted to calculate the matrix vector product of the jacobian with the 
floating species amounts, its a single statement now that we use native types.::
   n.dot(jac, amt)
   array([-0.00325, -0.09275,  0.036  ])


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
