#include "gtest/gtest.h"
#include "../test_util.h"
#include "telPluginManager.h"
#include "telPlugin.h"
#include "telProperties.h"
#include "telTelluriumData.h"
#include "telProperty.h"
//#include "tel"


using namespace testing;
using namespace std;
using namespace tlp;

extern string gRRTestDir;
extern string gRROutputDir;
extern string gRRPluginDir;
//extern APIHandleManager gHM;


TEST(RRPLUGIN_TEST_MODEL, OPTIMIZE_TEST_MODEL)
{
    if (gRRPluginDir.empty()) {
        std::cerr << "Please set the 'plugindir' environment variable before running the plugin tests.  This should be the directory where the plugin dlls are created." << std::endl;
        EXPECT_TRUE(false);
    }
    PluginManager* PM = new PluginManager(gRRPluginDir);

    Plugin* tmplugin = PM->getPlugin("tel_test_model");
    ASSERT_TRUE(tmplugin != NULL);
    tmplugin->execute();

    Plugin* mcplugin = PM->getPlugin("tel_monte_carlo_bs");
    ASSERT_TRUE(mcplugin != NULL);

    PropertyBase* sbml = tmplugin->getProperty("Model");
    mcplugin->setPropertyByString("SBML", sbml->getValueAsString().c_str());

    PropertyBase* testdata = tmplugin->getProperty("TestDataWithNoise");
    TelluriumData* exdata = static_cast<TelluriumData*>(testdata->getValueHandle());
    mcplugin->setPropertyValue("ExperimentalData", exdata);

    Property<double> k1val(0.3, "k1", "", "", "", true);
    Properties ipl;
    ipl.add(&k1val);
    //tlp::Property tpcre();
    mcplugin->setPropertyValue("InputParameterList", &ipl);
    mcplugin->setPropertyByString("NrOfMCRuns", "100");
    mcplugin->setPropertyByString("FittedDataSelectionList", "[S1] [S2]");
    mcplugin->setPropertyByString("ExperimentalDataSelectionList", "[S1] [S2]");
    
    mcplugin->execute();
    //EXPECT_EQ(mcplugin->getPropertyValueAsString("StatusMessage").find("converged"), 0);

    TelluriumData* params = static_cast<TelluriumData*>(mcplugin->getPropertyValueHandle("MonteCarloParameters"));
    ASSERT_TRUE(params != NULL);
    EXPECT_EQ(params->rSize(), 100);
    EXPECT_EQ(params->cSize(), 1);
    for (int r = 0; r < params->rSize(); r++)
    {
        EXPECT_NEAR(params->getDataElement(r, 0), 1.0, 0.5);
    }

    Properties* conf_limits = static_cast<Properties*>(mcplugin->getPropertyValueHandle("ConfidenceLimits"));
    ASSERT_TRUE(conf_limits != NULL);
    Property<double>* cl = static_cast<Property<double>* >(conf_limits->getFirst());
    EXPECT_EQ(cl->getName(), "k1");
    EXPECT_NEAR(cl->getValue(), 0.01, 0.008);

    EXPECT_TRUE(conf_limits->getNext() == NULL);


}

TEST(RRPLUGIN_TEST_MODEL, OPTIMIZE_NELDER_MEAD)
{
    if (gRRPluginDir.empty()) {
        std::cerr << "Please set the 'plugindir' environment variable before running the plugin tests.  This should be the directory where the plugin dlls are created." << std::endl;
        EXPECT_TRUE(false);
    }
    PluginManager* PM = new PluginManager(gRRPluginDir);

    Plugin* tmplugin = PM->getPlugin("tel_test_model");
    ASSERT_TRUE(tmplugin != NULL);
    tmplugin->execute();

    Plugin* mcplugin = PM->getPlugin("tel_monte_carlo_bs");
    ASSERT_TRUE(mcplugin != NULL);

    mcplugin->setPropertyByString("MinimizerPlugin", "tel_nelder_mead");

    PropertyBase* sbml = tmplugin->getProperty("Model");
    mcplugin->setPropertyByString("SBML", sbml->getValueAsString().c_str());

    PropertyBase* testdata = tmplugin->getProperty("TestDataWithNoise");
    TelluriumData* exdata = static_cast<TelluriumData*>(testdata->getValueHandle());
    mcplugin->setPropertyValue("ExperimentalData", exdata);

    Property<double> k1val(0.3, "k1", "", "", "", true);
    Properties ipl;
    ipl.add(&k1val);
    //tlp::Property tpcre();
    mcplugin->setPropertyValue("InputParameterList", &ipl);
    mcplugin->setPropertyByString("NrOfMCRuns", "100");
    mcplugin->setPropertyByString("FittedDataSelectionList", "[S1] [S2]");
    mcplugin->setPropertyByString("ExperimentalDataSelectionList", "[S1] [S2]");

    mcplugin->execute();
    //EXPECT_EQ(mcplugin->getPropertyValueAsString("StatusMessage").find("converged"), 0);

    TelluriumData* params = static_cast<TelluriumData*>(mcplugin->getPropertyValueHandle("MonteCarloParameters"));
    ASSERT_TRUE(params != NULL);
    EXPECT_EQ(params->rSize(), 100);
    EXPECT_EQ(params->cSize(), 1);
    for (int r = 0; r < params->rSize(); r++)
    {
        EXPECT_NEAR(params->getDataElement(r, 0), 1.0, 0.5);
    }

    Properties* conf_limits = static_cast<Properties*>(mcplugin->getPropertyValueHandle("ConfidenceLimits"));
    ASSERT_TRUE(conf_limits != NULL);
    Property<double>* cl = static_cast<Property<double>*>(conf_limits->getFirst());
    EXPECT_EQ(cl->getName(), "k1");
    EXPECT_NEAR(cl->getValue(), 0.01, 0.008);

    EXPECT_TRUE(conf_limits->getNext() == NULL);


}
