%module hello_swig
%{
    // tell SWIG to make Python C extension
    #define SWIG_FILE_WITH_INIT

    #include "hello.h"
%}

#ifdef SWIGWIN
    // not clear on what this does
    // but it looks important
    %include <windows.i>
#endif

// tell SWIG how to free strings
%typemap(newfree) char* "free($1);";

// tell SWIG to free the strings returned
// from these functions immediately
// (after copying their values into Python)
%newobject get_phrase;

// tell SWIG that these functions delete
// their first argument, and it doesnt need
// to be garbage collected
%delobject delete_greeter;

// list functions you want exported
// (to export everything, you can #include headers here)
Greeter* create_greeter();
void delete_greeter(Greeter* g);
int get_count(Greeter* g);
void set_phrase(Greeter* g, char* phrase);
char* get_phrase(Greeter* g);
void print_greeting(Greeter* g, char* name);

