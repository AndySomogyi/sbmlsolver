#ifndef telAutoDataParserH
#define telAutoDataParserH
#include <vector>
#include "telTelluriumData.h"
#include "telStringList.h"
#include "telConstants.h"
#include "telAutoConstants.h"
//---------------------------------------------------------------------------

using tlp::StringList;
using std::vector;

class AutoDataParser
{
    public:
                                                AutoDataParser(const string& input = tlp::gEmptyString);
                                               ~AutoDataParser();
        //Data input
        bool                                    parse(const string& input = tlp::gEmptyString);
        int                                     getNumberOfDataPoints();
        int                                     getNumberOfBifurcationPoints();
        string                                  getBifurcationDiagram();
        StringList                              getDataFileHeader();
        StringList                              getRawSolutionData();
        vector<int>                             getBifurcationPoints();
        StringList                              getBifurcationLabels();
        tlp::TelluriumData                      getSolutionData();

        telauto::ScanDirection                  getScanDirection();

    protected:
        string                                  mBifurcationDiagram;
        tlp::StringList                         mDataHeader;
        tlp::StringList                         mRawSolutionData;
        tlp::TelluriumData                      mSolutionData;
        vector<int>                             mBifurcationPoints;
        vector<string>                          mBifurcationLabels;
        void                                    resetOutput();
        int                                     getNrOfSolutions();
        string                                  getDataHeaderLine();

};

#endif
