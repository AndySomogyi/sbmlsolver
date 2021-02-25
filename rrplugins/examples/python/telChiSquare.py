#-------------------------------------------------------------------------------
# Purpose: Example demonstrating how to setup the ChiSquare plugin and
# calculate the ChiSquare for two data sets
#
# Author: Totte Karlsson (totte@dunescientific.com)
#-------------------------------------------------------------------------------
import rrplugins

try:   
    modelPlugin     = rrplugins.Plugin("tel_test_model")            
    noisePlugin     = rrplugins.Plugin("tel_add_noise")
    chiSquarePlugin = rrplugins.Plugin("tel_chisquare")       
    
    #Generate internal test data
    modelPlugin.execute()
    modelData = modelPlugin.TestData
    expData = modelPlugin.TestDataWithNoise
        
    chiSquarePlugin.ExperimentalData = expData         
    chiSquarePlugin.ModelData = modelData
    chiSquarePlugin.NrOfModelParameters = 1
        
    chiSquarePlugin.execute()
    
    chi = chiSquarePlugin.ChiSquare
    reduced_chi = chiSquarePlugin.ReducedChiSquare
    
    print('ChiSquare is: ', chi)
    print('Reduced ChiSquare is: ', reduced_chi)
    
    #chiSquarePlugin.viewManual()
    print('Plugin version: ', chiSquarePlugin.getVersion())        
except Exception as e:
    print('Problem: ', e)
