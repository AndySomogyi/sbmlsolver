#ifndef rrStreamWriterH
#define rrStreamWriterH
#include <fstream>
#include <string>
#include "rrExporter.h"

using std::ofstream;
using std::string;

namespace rr
{

/**
 * @internal
 * @deprecated
 */
class RR_DECLSPEC StreamWriter
{
    protected:
        string             mFilePath;
        ofstream         mFileStream;

    public:
                        StreamWriter(const string& filePath);
        bool             WriteLine(const string& line);
        bool             Write(const string& text);
        bool             Close();

};

} //ns rr
#endif
