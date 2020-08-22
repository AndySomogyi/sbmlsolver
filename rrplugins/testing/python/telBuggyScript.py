import roadrunner
import teplugins as tel

##================
## Running this script in a internal python engine session causes a CRASH after a few iterations
## Running in remote session seem to be ok..
##===============

for i in range(100):
# Create a roadrunner instance
    rr = roadrunner.RoadRunner()
    rr.load("sbml_test_0001.xml")
    
    # Generate data
    rr.simulate(0, 10, i + 1) # Want 512 points
    
    ### Comment this out and the crash do not happen    
    #d = tel.getRoadRunnerDataHandle (rr)
        
    print i
