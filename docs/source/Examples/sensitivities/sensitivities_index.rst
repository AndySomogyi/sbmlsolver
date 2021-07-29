Time Series Sensitivities
=========================


simple sensitivities. Explain the return type
getting different order of sensitivities
talk about the options available in sensitivities

change the nonlinear solver used.
change the sensitivity method from simultaneous (default) to staggered
change DQ_method (centered is default) but can also be "forward"

All other options are passed on to CVODEIntegrator and are the same,
so go and look at docs for CVODEIntegrator

How to be selective about which prameters you want sens for - performance advantage of computing less