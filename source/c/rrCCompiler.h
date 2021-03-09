#ifndef rrCompilerH
#define rrCompilerH
#include <vector>
#include <string>
#include "rrCompiler.h"
#include "rrStringList.h"

using std::vector;
using std::string;

namespace rr
{

/**
 * compiler class for the C based model system.
 */
class RR_DECLSPEC CCompiler : public Compiler
{
public:
    CCompiler(const std::string& supportCodeFolder, const std::string& compiler);
    virtual ~CCompiler();

    virtual std::string              getCompiler() const;
    virtual bool                setCompiler(const std::string& compiler);

    bool                        setupCompiler(const std::string& supportCodeFolder);
    bool                        compile(const std::string& cmdLine);

    virtual bool                setCompilerLocation(const std::string& path);
    virtual std::string              getCompilerLocation() const;

    virtual bool                setSupportCodeFolder(const std::string& path);
    virtual std::string              getSupportCodeFolder() const;

    bool                        setIncludePath(const std::string& path);
    bool                        setLibraryPath(const std::string& path);
    void                        execute(StringList& oProxyCode);
    bool                        compileSource(const std::string& cSource);
    std::string                      getCompilerMessages();
    bool                        setOutputPath(const std::string& path);


    virtual std::string getDefaultTargetTriple();

    virtual std::string getProcessTriple();

    virtual std::string getHostCPUName();

    virtual std::string getVersion();

private:
    std::string                      mDLLFileName;
    std::string                      mSupportCodeFolder;
    std::string                      mCompilerName;
    std::string                      mCompilerLocation;    //Path to executable

    std::vector<std::string>              mCompilerOutput;
    std::vector<std::string>              mIncludePaths;
    std::vector<std::string>              mLibraryPaths;
    std::vector<std::string>              mCompilerFlags;
    std::string                      createCompilerCommand(const std::string& sourceFileName);
    bool                        setupCompilerEnvironment();
    std::string                      mOutputPath;

};

} //namespace rr


#endif
