To run test, just call run_test_suite from the built binaries without any parameter(so using the default ones).

Now the default setting depends on that the sbml_dep project install the test suite and the right install path is set during cmake configuration.
Therefore a "path.h" file is generated at the same directory with "main.cpp", which contains the default paths and settings.

You can also use your own parameters if needed by refering to usage in "/src/Args.cpp".
