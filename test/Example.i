%module Example

%{
    #define SWIG_FILE_WITH_INIT

    #include <vector>

    #include "Example.h"


%}

%include "std_vector.i"

%include "Example.h"








