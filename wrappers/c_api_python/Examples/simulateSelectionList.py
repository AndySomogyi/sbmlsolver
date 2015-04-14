import roadrunner
rr = roadrunner.RoadRunner()

rr.load('../models/test_1.xml')

rr.selections = ['time', 'S2']
print rr.simulate(0,10,11)

