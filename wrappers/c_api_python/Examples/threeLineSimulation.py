import roadrunner
rr = roadrunner.RoadRunner()

rr.load('../models/feedback.xml')
print rr.simulate(0,2,20)
