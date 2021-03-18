%module TestModelFactory

%{
#define SWIG_FILE_WITH_INIT

#include <map>
//#include <unordered_map>
#include <utility>
#include "TestModelFactory.h"

typedef std::pair<double, double> DoublePair;
//typedef std::unordered_map<std::string, DoublePair> ResultMap;
//typedef std::vector<ResultMap> MultiResultsMap;

%}



%include "std_string.i"
%include "std_vector.i"
%include "std_pair.i"
%include "std_unordered_map.i"


%template(DoublePair) std::pair<double, double>;
%include "TestModelFactory.h"


//%template(ResultMap) std::unordered_map<std::string, DoublePair>;
//%template(MultiResultMap) std::vector<ResultMap>;

%typemap(out) SBMLTestModel *TestModelFactory{
        const std::string lookup_typename = *arg1 + " *";
        swig_type_info * const outtype = SWIG_TypeQuery(lookup_typename.c_str());
        $result = SWIG_NewPointerObj(SWIG_as_voidptr($1), outtype, $owner);
}


