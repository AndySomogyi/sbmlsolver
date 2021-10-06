Using RoadRunner with Python's parallel libraries
======================================================

The model
---------
For these simulations we use a simple model that we can access easily from one of RoadRunners
test suites. Its called 'OpenLinearFlux' and can be generated with the following
`antimony <https://tellurium.readthedocs.io/en/latest/antimony.html#antimony-reference>`_ code.

.. code-block:: python

    antimony_string = """
    model OpenLinearFlux
        S1 = 0;
        S2 = 0;
        => S1; kin
        S1 => S2; kf*S1;
        S2 => ; S2*kout;
        kf = 0.1;
        kb = 0.01;
        kin = 1;
        kout = 0.1
    end
    """

In each of the following sections we use RoadRunner to generate 100,000 stochastic simulations
from this model

Serial code
------------


Multiprocessing library
------------------------

The simplest way in Python to use multiple cores simultaneously for running RoadRunner simulations
is to use the `multiprocessing <https://docs.python.org/3/library/multiprocessing.html>`_ library.
Further, the simplest multiprocessing tool that one can use is a `multiprocessing.Pool`.



Threading library
-----------------





Ray library
------------




MPI library
------------









































