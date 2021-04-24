#ifndef CommandLineParametersH
#define CommandLineParametersH
#include <string>
using std::string;

std::string Usage(const std::string& prg);
class Args
{
    public:
                                        Args();
        virtual                        ~Args(){}
        std::string                          SBMLModelsFilePath;                       	//option m:
//		std::string                          Compiler;       		                  	//option l:
        std::string                          ResultOutputFile;                         	//option r:
        std::string                          TempDataFolder;                           	//option t:
//        std::string                          DataOutputFolder;                          	//option d:
//		std::string                          SupportCodeFolder;                        	//option s:
		bool                            EnableLogging;                    	     	//option v:
};

#endif
