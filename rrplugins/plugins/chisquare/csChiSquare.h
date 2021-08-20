#ifndef csChiSquareH
#define csChiSquareH
#include <vector>
#include "telProperty.h"
#include "rrplugins/pluginBaseClass/telCPPPlugin.h"
#include "csChiWorker.h"
//---------------------------------------------------------------------------

namespace cs_ChiSquare {

    using namespace tlp;
    using std::string;

    class ChiSquare : public CPPPlugin
    {
        friend class ChiWorker;

    public:
        //Input
        Property<int>                           mNrOfModelParameters;
        Property<TelluriumData>				    mExperimentalData;
        Property<TelluriumData>			        mModelData;

        //Output
        Property<double>		                mChiSquare;                     //Chi square
        Property<double>		                mReducedChiSquare;              //Reduced Chi Square

    protected:
        ChiWorker                               mWorker;

    public:
                                                ChiSquare();
                                                ~ChiSquare();
        bool                                    execute(bool inThread = false);
        bool                                    resetPlugin();
        string                                  getImplementationLanguage();
        bool                                    isWorking() const;
        unsigned char*                          getManualAsPDF() const;
        size_t                                  getPDFManualByteSize();
        void                                    assignPropertyDescriptions();
    };

    #ifdef EXPORT_CHI_SQUARE
    extern "C"
    {
        ChiSquare* plugins_cc        createPlugin();
        const char* plugins_cc       getImplementationLanguage();
        void plugins_cc setHostInterface(rrc::THostInterface* _hostInterface);
    }
    #endif
}
#endif
