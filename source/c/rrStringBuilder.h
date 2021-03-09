#ifndef rrStringBuilderH
#define rrStringBuilderH
#include <sstream>
#include <string>
#include "rrExporter.h"

using std::stringstream;
using std::string;

namespace rr
{

/**
 * @internal
 * @deprecated
 */
class RR_DECLSPEC StringBuilder
{
    protected:
        std::stringstream                 mStringing;

    public:
                                    StringBuilder(const std::string& aStr = "");
        std::stringstream&                  operator<<(const std::string& str);
        std::stringstream&                  operator<<(const char& ch);
        std::string                      ToString();

        void                        NewLine(const std::string& line = "");
        void                        Line(const std::string& line);
        void                        TLine(const std::string& line, const int& tabs = 1);
        void                        Clear();
};

}

#endif
