Steady State Solver Class
_________________________

.. class:: roadrunner.SteadyStateSolver

    A Steady State Solver gives the steady states of involved species. Currently, libRoadRunner supports NLEQ2 solver. For solver related methods, refer to http://sys-bio.github.io/roadrunner/python_docs/api_reference.html#solver-class.


NLEQ2
-----

   NLEQ2 is a non-linear equation solver which uses a global Newton method with adaptive damping strategies (see http://www.zib.de/weimann/NewtonLib/index.html).


.. attribute:: SteadyStateSolver.broyden_method

    Switches on Broyden method, which is a quasi-Newton approximation for rank-1 updates. Default value is 0.

    To enable, type:
   
   >>> r.getSteadyStateSolver().broyden_method = 1


.. attribute:: SteadyStateSolver.linearity

    Specifies linearity of the problem. 1 is for linear problem and 4 is for extremly nonlinear problem. Default value is 3.

    >>> r.getSteadyStateSolver().linearity = 1


.. attribute:: SteadyStateSolver.maximum_iterations

    The maximum number of iterations the solver is allowed to use. Iteration caps off at the maximum, regardless of whether a solution has been reached. Default value is 100.

    >>> r.getSteadyStateSolver().maximum_iterations = 50


.. attribute:: SteadyStateSolver.minimum_damping

    The minimum damping factor used by the algorithm. Default value is 1e-4.

    >>> r.getSteadyStateSolver().minimum_damping = 1e-20


.. attribute:: SteadyStateSolver.relative_tolerance

    Specifies the relative tolerance used by the solver. Default value is 1e-16.

    >>> r.getSteadyStateSolver().relative_tolerance = 1e-15


.. method:: SteadyStateSolver.solve()

    Main solver routine