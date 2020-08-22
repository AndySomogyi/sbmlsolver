import roadrunner
import teplugins as tel

i = 0
#for i in range(100):
try:
    noisePlugin = tel.Plugin ("tel_add_noise")
    print noisePlugin.listOfProperties()

    # Create a roadrunner instance
    rr = roadrunner.RoadRunner()
    rr.load("sbml_test_0001.xml")

    # Generate data
    data = rr.simulate(0, 10, 511) # Want 512 points
        
    # Get the dataseries from roadrunner
    d = tel.getDataSeries (data)        

    # Assign the dataseries to the plugin inputdata
    noisePlugin.InputData = d

    # Set parameter for the 'size' of the noise
    noisePlugin.Sigma = 3.e-6

    # Add the noise
    noisePlugin.execute()

    # Get the data to plot
    noisePlugin.InputData.plot()

#    tel.show()

    d.writeDataSeries ("testData2.dat")

    d.readDataSeries ("testData2.dat")
    print "done"
    

    
    print i

except Exception as e:
    print 'Problem: ' + `e`
        