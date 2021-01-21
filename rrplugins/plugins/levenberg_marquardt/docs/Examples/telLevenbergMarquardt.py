from teplugins import *

# Load Plugins
chiPlugin       = Plugin("tel_chisquare")
lm              = Plugin("tel_levenberg_marquardt")
modelPlugin     = Plugin("tel_test_model")
addNoisePlugin  = Plugin("tel_add_noise")

try:
    #========== EVENT FUNCTION SETUP ===========================
    def myEvent(dummy): #We are not capturing any data from the plugin, so just pass a dummy
        print 'Iteration, Norm = ' + `lm.getProperty("NrOfIter")`  + ', ' + `lm.getProperty("Norm")`

    #Setup progress event function
    progressEvent =  NotifyEventEx(myEvent)     
    assignOnProgressEvent(lm.plugin, progressEvent)
    #============================================================
    
    #Create model data, with and without noise using the test_model plugin
    modelPlugin.execute()     
    
    #Setup lmfit properties.
    lm.SBML             = modelPlugin.Model 
    lm.ExperimentalData = modelPlugin.TestDataWithNoise
    
    # Add the parameters that we're going to fit and an initial 'start' value
    lm.setProperty("InputParameterList", ["k1", .3])
    lm.setProperty("FittedDataSelectionList", "[S1] [S2]")
    lm.setProperty("ExperimentalDataSelectionList", "[S1] [S2]")
    
    # Start minimization
    lm.execute()
    
    print 'Minimization finished. \n==== Result ===='    
    print 'Fit engine status: ' + `lm.getProperty('StatusMessage')`

    print 'Hessian Matrix'
    print lm.getProperty("Hessian")
    
    print 'Covariance  Matrix'
    print lm.getProperty("CovarianceMatrix")
             
    print 'ChiSquare = '            + `lm.getProperty("ChiSquare")`
    print 'Reduced ChiSquare = '    + `lm.getProperty("ReducedChiSquare")`
        
    #This is a list of parameters
    parameters = tpc.getPluginProperty (lm.plugin, "OutputParameterList")
    confLimits = tpc.getPluginProperty (lm.plugin, "ConfidenceLimits")    
    
    #Iterate trough list of parameters and confidence limits
    para  = getFirstProperty(parameters)
    limit = getFirstProperty(confLimits)     
    while para and limit:           
        print getPropertyName(para) + ' = ' + `getPropertyValue(para)` + ' +/- ' + `getPropertyValue(limit)`
        para  = getNextProperty(parameters)
        limit = getNextProperty(confLimits)                        
                                 
    
    # Get the fitted and residual data
    fittedData = lm.getProperty ("FittedData").toNumpy
    residuals  = lm.getProperty ("Residuals").toNumpy

    # Get the experimental data as a numpy array
    experimentalData = modelPlugin.TestDataWithNoise.toNumpy
    
    telplugins.plot(fittedData         [:,[0,1]], "blue", "-",    "",    "S1 Fitted")
    telplugins.plot(fittedData         [:,[0,2]], "blue", "-",    "",    "S2 Fitted")
    telplugins.plot(residuals          [:,[0,1]], "blue", "None", "x",   "S1 Residual")
    telplugins.plot(residuals          [:,[0,2]], "red",  "None", "x",   "S2 Residual")
    telplugins.plot(experimentalData   [:,[0,1]], "red",  "",     "*",   "S1 Data")
    telplugins.plot(experimentalData   [:,[0,2]], "blue", "",     "*",   "S2 Data")
    telplugins.plt.show()
    
except Exception as e:
    print 'Problem.. ' + `e`         