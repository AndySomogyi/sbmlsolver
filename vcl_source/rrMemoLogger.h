#ifndef rrMemoLoggerH
#define rrMemoLoggerH
#include <Vcl.StdCtrls.hpp>
#include <sstream>

//---------------------------------------------------------------------------
//Minimalistic logger to a memo...
class TMemoLogger
{
    protected:
        std::ostringstream          mStream;
		TMemo					   *mMemo;
    public:
                                    TMemoLogger(TMemo* aMemo);
        virtual                    ~TMemoLogger();
        std::ostringstream&         Get();
};

#define Log() \
    TMemoLogger(infoMemo).Get()

#endif
