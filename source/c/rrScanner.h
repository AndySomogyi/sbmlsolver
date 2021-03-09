#ifndef rrScannerH
#define rrScannerH
#include "rrExporter.h"
#include <vector>
#include <queue>
#include <fstream>
#include <sstream>
#include "rrHashTable.h"
#include "rrCodeTypes.h"
#include "rrToken.h"
using std::vector;
using std::queue;
using std::fstream;
using std::stringstream;

namespace rr
{

namespace TCharCode
{
enum TCharCode
{
    cLETTER = 0,
    cDIGIT,
    cPOINT,
    cDOUBLEQUOTE,
    cUNDERSCORE,
    cSPECIAL,
    cWHITESPACE,
    cETX
};
}


class RR_DECLSPEC Scanner
{
    protected:
        const char                                EOFCHAR; // Deemed end of std::string marker, used internally
        const char                                CR;
        const char                                LF;
        std::vector<TCharCode::TCharCode>              FCharTable;
        std::vector<char>                              buffer;
        queue<Token>                              tokenQueue;
        std::map<std::string, CodeTypes::CodeTypes>         wordTable;
        std::stringstream                              *pStream;
        std::streamsize                           bufferLength;
        int                                       bufferPtr; // Index of position in buffer containing current char
        CodeTypes::CodeTypes                      ftoken;
        int                                       yylineno; // Current line number
        void                                      initScanner();
        char                                      getCharFromBuffer();
        bool                                      IsDoubleQuote(char ch);
        void                                      getNumber();
        void                                      getSpecial();
        void                                      getString();
        void                                      getTokenFromQueue();
        void                                      getWord();
        void                                      nextTokenInternal();

    public:
        std::string                                    timeWord1;
        std::string                                    timeWord2;
        std::string                                    timeWord3;
        bool                                      FromQueue;
        bool                                      IgnoreNewLines;
        Token                                     currentToken;
        char                                      fch; // Current character read
        Token                                     previousToken;
        double                                    tokenDouble;
        int                                       tokenInteger;
        double                                    tokenScalar; // Used to retrieve int or double
        std::string                                    tokenString;
        int                                       lineNumber();

        CodeTypes::CodeTypes                      token();


                                                  Scanner();
        void                                      startScanner();
        bool                                      IsQueueEmpty();
        char                                      nextChar();
        std::string                                    tokenToString(const CodeTypes::CodeTypes& code);
        void                                      AddTokenToQueue();
        void                                      nextToken();
        void                                      skipBlanks();
        void                                      UnGetToken();
        void                                      AssignStream(std::stringstream& str);
}; //class scanner

} //rr
#endif
