import roadrunner

import rrplugins as tel

rr = roadrunner.RoadRunner() 

rr.load("path to the model") 

data = rr.simulate(0, 10, 511)

noisePlugin = tel.Plugin ("tel_add_noise") 

d = tel.getDataSeries (data) 

noisePlugin.InputData = d 

noisePlugin.Sigma = 3.e-6 

noisePlugin.execute()

noisePlugin.InputData.plot()

