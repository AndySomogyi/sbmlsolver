#pragma hdrstop
#include <iostream>
#include "telPluginManager.h"
#include "telException.h"
#include "telLogger.h"
#include "telPlugin.h"
#include "telUtils.h"
#include "telProperty.h"
#include "telAutoPlugin.h"
#include "telAutoTelluriumInterface.h"
#include "telAutoDataParser.h"

using namespace tlp;
using namespace std;
using namespace rr;

int main()
{
    string tempFolder("r:\\temp");
	gLog.setLevel(lInfo);
    gLog.enableConsoleLogging();
    gLog.enableFileLogging(joinPath(tempFolder, "auto.log"));

    try
    {
        string pluginName("tel_auto2000");

        //A Plugin manager, using a roadrunner instance
        PluginManager pm("../plugins");

        //Load auto plugin
        if(!pm.load(pluginName))
        {
            RRPLOG(lError)<<"Failed to load plugin: "<<pluginName;
            return 0;
        }

        //Get a plugin handle and get some plugin info
        AutoPlugin* autoPlugin = (AutoPlugin*) pm.getPlugin(pluginName);
        if(!autoPlugin)
        {
            RRPLOG(lError)<<"Problem..";
            throw("AutoPlugin don't exist");
        }

        //A serious client would check if these calls are succesfull or not
        string sbmlFile("../models/bistable.xml");
        string sbml = getFileContent(sbmlFile);

        //Various plugin constants

        autoPlugin->setPropertyByString("SBML", sbml.c_str());
        autoPlugin->setPropertyByString("TempFolder", tempFolder.c_str());
        autoPlugin->setPropertyByString("KeepTempFiles", "false");

        //Specific auto parameters

//        autoPlugin->setPropertyByString("ScanDirection", "Positive");
        autoPlugin->setPropertyByString("ScanDirection", "Negative");
        autoPlugin->setPropertyByString("PrincipalContinuationParameter", "k3");
//        autoPlugin->setPropertyByString("PrincipalContinuationParameter", "A");
        autoPlugin->setPropertyByString("RL0", ".1");
        autoPlugin->setPropertyByString("RL1", "20");
        autoPlugin->setPropertyByString("NMX", "1000");

        if(!autoPlugin->execute(false))
        {
            RRPLOG(lError)<<"Problem executing the Auto plugin";
            return 0;
        }

        RRPLOG(lInfo)<<"Auto plugin is done.";
        Property<string>* biD = (Property<string>*) autoPlugin->getProperty("BifurcationDiagram");
        Property<string>* biS = (Property<string>*) autoPlugin->getProperty("BifurcationSummary");
        RRPLOG(lInfo)<<"BIFURCATION SUMMARY\n"<< biS->getValue();

        //Check bifurcaation data
        Property< vector<int> >*    biPtsP      = (Property< vector<int> >*)       autoPlugin->getProperty("BifurcationPoints");
        Property< vector<string> >* biLblsP     = (Property< vector<string> >*)    autoPlugin->getProperty("BifurcationLabels");
        Property< TelluriumData >*  biDataP     = (Property< TelluriumData >*)     autoPlugin->getProperty("BifurcationData");

        vector<int>         biPts = biPtsP->getValue();
        vector<string>      biLbls = biLblsP->getValue();
        TelluriumData       biData = biDataP->getValue();

        RRPLOG(lInfo) << "Number of bifurc points: "  <<biPts.size();
        RRPLOG(lInfo) << "Number of bifurc labels: "  <<biLbls.size();

        RRPLOG(lInfo) <<"\n\n";

        if(biPts.size() != biLbls.size())
        {
            throw("Bad");
        }

        RRPLOG(lInfo) << "Solution points ===";
        for(int i = 0; i < biPts.size(); i++)
        {
            RRPLOG(lInfo) << "Point, Label " << biPts[i] << ": "<< biLbls[i];
        }

//###
        //A serious client would check if these calls are succesfull or not
        sbmlFile = "r:/models/BIOMD0000000203.xml";
        sbml = getFileContent(sbmlFile);

        //Various plugin constants

        autoPlugin->setPropertyByString("SBML", sbml.c_str());
        autoPlugin->setPropertyByString("TempFolder", tempFolder.c_str());
        autoPlugin->setPropertyByString("KeepTempFiles", "false");

        //Specific auto parameters

        autoPlugin->setPropertyByString("ScanDirection", "Positive");
        autoPlugin->setPropertyByString("PrincipalContinuationParameter", "A");
        autoPlugin->setPropertyByString("RL0", "10");
        autoPlugin->setPropertyByString("RL1", "100");
        autoPlugin->setPropertyByString("NMX", "5000");

        if(!autoPlugin->execute(false))
        {
            RRPLOG(lError)<<"Problem executing the Auto plugin";
            return 0;
        }

        RRPLOG(lInfo)<<"Auto plugin is done.";
        biD = (Property<string>*) autoPlugin->getProperty("BifurcationDiagram");
        biS = (Property<string>*) autoPlugin->getProperty("BifurcationSummary");
        RRPLOG(lInfo)<<"BIFURCATION SUMMARY\n"<< biS->getValue();

        //Check bifurcaation data
        biPtsP      = (Property< vector<int> >*)       autoPlugin->getProperty("BifurcationPoints");
        biLblsP     = (Property< vector<string> >*)    autoPlugin->getProperty("BifurcationLabels");
        biDataP     = (Property< TelluriumData >*)     autoPlugin->getProperty("BifurcationData");

        biPts = biPtsP->getValue();
        biLbls = biLblsP->getValue();
        biData = biDataP->getValue();

        RRPLOG(lInfo) << "Number of bifurc points: "  <<biPts.size();
        RRPLOG(lInfo) << "Number of bifurc labels: "  <<biLbls.size();

        RRPLOG(lInfo) <<"\n\n";

        if(biPts.size() != biLbls.size())
        {
            throw("Bad");
        }

        RRPLOG(lInfo) << "Solution points ===";
        for(int i = 0; i < biPts.size(); i++)
        {
            RRPLOG(lInfo) << "Point, Label " << biPts[i] << ": "<< biLbls[i];
        }


        RRPLOG(lInfo) << "Save TelluriumData ===";
        biData.write(joinPath(tempFolder, "auto_data.dat"));
        RRPLOG(lInfo)<<"\n\nAUTO 2000 is unloading...\n";

        //Check plugin data..
//        pm.unload(autoPlugin);
    }
    catch(Exception& ex)
    {
        RRPLOG(lError)<<ex.getMessage();
    }
    catch(Poco::Exception& ex)
    {
        RRPLOG(lError)<<"Problem..."<<ex.message();
    }
    catch(exception& ex)
    {
        RRPLOG(lError)<<"Bad problem...!"<<ex.what();
    }

    catch(...)
    {
        RRPLOG(lError)<<"Bad problem...!";
    }

    pause(true);
    return 0;
}
