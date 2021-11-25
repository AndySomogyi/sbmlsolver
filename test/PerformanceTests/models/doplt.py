import roadrunner
rr = roadrunner.RoadRunner(r"D:\roadrunner\roadrunner\test\PerformanceTests\models\LargeHanekomNetworks\2.sbml")
rr.simulate(0, 3000, 100)

rr.plot()