#ifndef CommandLineParametersH
#define CommandLineParametersH
#include <string>
using std::string;

string Usage(const string& prg);
enum ModelVersions {l2v4 = 0, l3v1};
class Args
{
    public:
                                        Args();
        virtual                        ~Args(){}
        string                          SBMLModelsFilePath;       // option m:
        string                          TempDataFolder;           // option t:
        bool                            EnableLogging;            // option v:
        int                             ModelNumber;              // option i:
        ModelVersions                   ModelVersion;             // option a:
        string                          compiler;                 // option c:
};

#endif
