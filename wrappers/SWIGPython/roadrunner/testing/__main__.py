import roadrunner
import tester
import testfiles



r = testfiles.get_roadrunner('feedback.xml')

m = r.model

roadrunner.sigtrap()



print(m.getFloatingSpeciesInitAmountIds())

tester.runTester()
