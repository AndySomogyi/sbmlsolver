#ifndef rrTestUtilsH
#define rrTestUtilsH
#include <string>
#include "rr-libstruct/lsMatrix.h"

std::string getModelDir(std::string base, std::string suite, std::string name);
bool testValidateSBML(std::string sbml);
ls::DoubleMatrix ParseMatrixFromText(const std::string& textMatrix);
void expectArrayClose(const ls::DoubleMatrix& jRef, const ls::DoubleMatrix& fullJacobian, double epsilon);


#endif

