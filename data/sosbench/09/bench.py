def run():
    from roadrunner import *

    o=SimulateOptions()

    #o.stiff = True

    o.duration=150

    o.steps = 300

    o.absolute = 1e-15
    o.relative = 1e-9

    o.resetModel = True

    r=RoadRunner("00001-sbml-l2v4.xml")

    Logger.setLevel(Logger.LOG_INFORMATION)
    
    r.simulate(o);

    plot(r.getSimulationResult())


if __name__ == "__main__":
    run()
