#ifndef rrStringUtilsH
#define rrStringUtilsH
#include <string>
#include <list>
#include <vector>
#include "rrExporter.h"

using std::string;
using std::list;
using std::vector;

namespace rr
{

string              RR_DECLSPEC ReplaceWord(const string& str1, const string& str2, const string& theString);
bool                RR_DECLSPEC ConvertFunctionCallToUseVarArgsSyntax(const string& funcName, string& expression);
string              RR_DECLSPEC RemoveChars(const string& str, const string& chars);
bool                RR_DECLSPEC IsUnwantedChar(char ch); //Predicate for find_if algorithms..
size_t              RR_DECLSPEC FindMatchingRightParenthesis(const string& expression, const size_t startFrom);
int                 RR_DECLSPEC GetNumberOfFunctionArguments(const string& expression);
string              RR_DECLSPEC tabs(const int& nr);
string              RR_DECLSPEC NL();

string				RR_DECLSPEC ToUpperOrLowerCase(const string& inStr, int (*func)(int));
string 				RR_DECLSPEC ToUpper(const string& str);
string 				RR_DECLSPEC ToLower(const string& str);

string              RR_DECLSPEC ExtractFilePath(const string& fileN);
string              RR_DECLSPEC ExtractFileName(const string& fileN);
string              RR_DECLSPEC ExtractFileNameNoExtension(const string& fileN);

string              RR_DECLSPEC ChangeFileExtensionTo(const string& theFileName, const string& newExtension);

string              RR_DECLSPEC Trim(const string& str);
bool                RR_DECLSPEC StartsWith(const string& src, const string& sub);
bool                RR_DECLSPEC EndsWith(const string& src, const string& sub);

string              RR_DECLSPEC JoinPath(const string& aPath, const string& aFile, const string& pathSeparator = "\\");
//conversions
string              RR_DECLSPEC IntToStr(const int& nt);
int                 RR_DECLSPEC StrToInt(const string& nt);
string              RR_DECLSPEC DblToStr(const double& nt);
double              RR_DECLSPEC StrToDbl(const string& nt);
vector<string>      RR_DECLSPEC SplitString(const string &text, const string &separators);
vector<string>      RR_DECLSPEC SplitString(const string& input, const char& delimiters);
int                 RR_DECLSPEC ToInt(const string& str);
bool                RR_DECLSPEC ToBool(const string& str);
double              RR_DECLSPEC ToDouble(const string& str);

string              RR_DECLSPEC ToString(const bool& b);
string              RR_DECLSPEC ToString(const double& d, const string& format = "%f");
string              RR_DECLSPEC ToString(const unsigned int& n, const string& format = "%d", const int nBase=10);
string              RR_DECLSPEC ToString(const int& n, const string& format = "%d", const int nBase=10);
string              RR_DECLSPEC ToString(const long n, const int nBase=10);
string              RR_DECLSPEC ToString(const unsigned long n, const int nBase=10);
string              RR_DECLSPEC ToString(const unsigned short n, const int nBase=10);
string              RR_DECLSPEC ToString(const short n, const int nBase=10);
string              RR_DECLSPEC ToString(const char n);
string              RR_DECLSPEC ToString(const unsigned char n);
string              RR_DECLSPEC ToString(const string & s);
string              RR_DECLSPEC ToString(const char* str);
//bool              RR_DECLSPEC InStringList(const string& fldr, list<string>& theList);

string              RR_DECLSPEC Format(const string& src, const int& arg);
string              RR_DECLSPEC Format(const string& str, const int& arg1);
string              RR_DECLSPEC Format(const string& src, const string& arg);
string              RR_DECLSPEC Format(const string& src, const string& arg1, const string& arg2, const string& arg3);
string              RR_DECLSPEC Format(const string& src, const string& arg1, const string& arg2);
string              RR_DECLSPEC Format(const string& src, const string& arg1, const int& arg2);
string              RR_DECLSPEC Format(const string& src, const string& arg1, const int& arg2, const string& arg3);
string              RR_DECLSPEC Format(const string& str1, const string& str2);
string              RR_DECLSPEC Format(const string& str1, const string& arg1, const string& arg2);
string              RR_DECLSPEC Format(const string& str1, const string& arg1, const int& arg2);
string              RR_DECLSPEC Format(const string& str1, const string& arg1, const string& arg2, const string& arg3);
string              RR_DECLSPEC Format(const string& str1, const string& arg1, const string& arg2, const string& arg3, const string& arg4);
string              RR_DECLSPEC Format(const string& str1, const string& arg1, const string& arg2, const string& arg3, const string& arg4, const string& arg5);

string              RR_DECLSPEC Format(const string& str1, const unsigned int& arg1, const string& arg2);
string              RR_DECLSPEC Format(const string& str1, const unsigned int& arg1, const string& arg2, const string& arg3);
string              RR_DECLSPEC Format(const string& str1, const string& arg1, const int& arg2, const string& arg3);
string              RR_DECLSPEC Format(const string& str1, const unsigned int& arg1, const unsigned int& arg2, const string& arg3, const string& arg4);

string              RR_DECLSPEC Append(const string& str);
string              RR_DECLSPEC Append(const int& str);
string              RR_DECLSPEC Append(const unsigned int& str);
string              RR_DECLSPEC Append(const string& s1, const string& s2);
string              RR_DECLSPEC Append(const string& s1, const string& s2, const string& s3);
string              RR_DECLSPEC Append(const string& s1, const unsigned int& s2, const string& s3);
string              RR_DECLSPEC Append(const string& s1, const unsigned int& s2, const string& s3, const string& s4);

string              RR_DECLSPEC Substitute(const string& src, const string& thisOne, const string& withThisOne, const int& howMany = -1);
string              RR_DECLSPEC Substitute(const string& src, const string& thisOne, const int& withThisOne, const int& howMany = -1);
string              RR_DECLSPEC RemoveNewLines(const string& str, const int& howMany = -1);
}
#endif
