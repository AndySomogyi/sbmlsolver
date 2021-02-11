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
    //tpCreatePluginManager();
    //gHM.registerHandle(PM, typeid(PM).name());

    Plugin* tmplugin = PM->getPlugin("tel_test_model");
    ASSERT_TRUE(tmplugin != NULL);
    tmplugin->execute();

    Plugin* lmplugin = PM->getPlugin("tel_levenberg_marquardt");
    ASSERT_TRUE(lmplugin != NULL);

    Plugin* chiplugin = PM->getPlugin("tel_chisquare");
    ASSERT_TRUE(chiplugin != NULL);

    PropertyBase* sbml = tmplugin->getProperty("Model");
    lmplugin->setPropertyByString("SBML", sbml->getValueAsString().c_str());

    PropertyBase* testdata = tmplugin->getProperty("TestDataWithNoise");
    TelluriumData* exdata = static_cast<TelluriumData*>(testdata->getValueHandle());
    lmplugin->setPropertyValue("ExperimentalData", exdata);

    Property<double> k1val(0.3, "k1", "", "", "", true);
    Properties ipl;
    ipl.add(&k1val);
    //tlp::Property tpcre();
    lmplugin->setPropertyValue("InputParameterList", &ipl);
    lmplugin->setPropertyByString("FittedDataSelectionList", "[S1] [S2]");
    lmplugin->setPropertyByString("ExperimentalDataSelectionList", "[S1] [S2]");
    
    lmplugin->execute();
    EXPECT_EQ(lmplugin->getPropertyValueAsString("StatusMessage").find("converged"), 0);

    PropertyBase* hessian_property = lmplugin->getProperty("Hessian");
    ASSERT_TRUE(hessian_property != NULL);
    ls::DoubleMatrix* hessian = static_cast<ls::DoubleMatrix*>(hessian_property->getValueHandle());
    EXPECT_EQ(hessian->numRows(), 1);
    EXPECT_EQ(hessian->numCols(), 1);
    EXPECT_NEAR(*hessian[0][0], 3300, 1500); //Determined empirically.

    PropertyBase* cov_property = lmplugin->getProperty("CovarianceMatrix");
    ASSERT_TRUE(cov_property != NULL);
    ls::DoubleMatrix* covariance = static_cast<ls::DoubleMatrix*>(cov_property->getValueHandle());
    EXPECT_EQ(covariance->numRows(), 1);
    EXPECT_EQ(covariance->numCols(), 1);
    EXPECT_NEAR(*covariance[0][0], 0.0003, 0.0001); //Determined empirically.

    PropertyBase* chi_property = lmplugin->getProperty("ChiSquare");
    ASSERT_TRUE(chi_property != NULL);
    double* chisquare = static_cast<double*>(chi_property->getValueHandle());
    EXPECT_LT(*chisquare, 100); //Determined empirically.

    PropertyBase* red_chi_property = lmplugin->getProperty("ChiSquare");
    ASSERT_TRUE(red_chi_property != NULL);
    double* reduced_chi = static_cast<double*>(red_chi_property->getValueHandle());
    EXPECT_LT(*reduced_chi, 100); //Determined empirically.
    EXPECT_EQ(*chisquare, *reduced_chi); //True when there's only one estimated parameter, at least.

    PropertyBase* outparam_property = lmplugin->getProperty("OutputParameterList");
    ASSERT_TRUE(outparam_property != NULL);
    Properties* outparams = static_cast<Properties*>(outparam_property->getValueHandle());
    PropertyBase* outparam = outparams->getFirst();
    ASSERT_TRUE(outparam != NULL);
    EXPECT_EQ(outparam->getName(), "k1");
    double* outparam_val = static_cast<double*>(outparam->getValueHandle());
    EXPECT_NEAR(*outparam_val, 1.0, 0.5);
    EXPECT_TRUE(outparams->getNext() == NULL);

    PropertyBase* conflimit_property = lmplugin->getProperty("ConfidenceLimits");
    ASSERT_TRUE(conflimit_property != NULL);
    Properties* conflimits = static_cast<Properties*>(conflimit_property->getValueHandle());
    PropertyBase* conflimit = conflimits->getFirst();
    ASSERT_TRUE(conflimit != NULL);
    EXPECT_EQ(conflimit->getName(), "k1_confidence");
    double* conflimit_val = static_cast<double*>(conflimit->getValueHandle());
    EXPECT_NEAR(*conflimit_val, 0.03, 0.015);
    EXPECT_TRUE(conflimits->getNext() == NULL);

    PropertyBase* fit_property = lmplugin->getProperty("FittedData");
    ASSERT_TRUE(fit_property != NULL);
    TelluriumData* fit = static_cast<TelluriumData*>(fit_property->getValueHandle());
    EXPECT_EQ(fit->cSize(), 3);
    EXPECT_EQ(fit->rSize(), 14);

    PropertyBase* residuals_property = lmplugin->getProperty("Residuals");
    ASSERT_TRUE(residuals_property != NULL);
    TelluriumData* residuals = static_cast<TelluriumData*>(residuals_property->getValueHandle());
    EXPECT_EQ(residuals->cSize(), 3);
    EXPECT_EQ(residuals->rSize(), 14);

    for (int c = 0; c < fit->cSize(); c++)
    {
        for (int r = 0; r < fit->rSize(); r++)
        {
            double fitval = fit->getDataElement(r, c);
            double origval = exdata->getDataElement(r, c);
            double tol = max(abs(origval / 10), 0.0001);
            EXPECT_NEAR(fitval, origval, tol);

            if (c > 0) {
                double residual = residuals->getDataElement(r, c);
                EXPECT_NEAR(abs(origval - fitval), residual, 0.0001);
                EXPECT_LT(residual, 0.0001);
                //cout << origval << ", " << fitval << ", " << residual << ", " << abs(origval - fitval) << endl;
            }
        }
    }



}
