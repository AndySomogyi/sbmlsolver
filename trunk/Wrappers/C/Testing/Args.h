#ifndef CommandLineParametersH
#define CommandLineParametersH
#include <string>
using std::string;

string Usage(const string& prg);
class Args
{
    public:
                                        Args();
        virtual                        ~Args(){}
        string                          SBMLModelsFilePath;                         //option m:
        string                          ResultOutputFolder;                         //option r:
        string                          TempDataFolder;                             //option t:
};

#endif
