#-------------------------------------------------------------
# Tests for steady state and stoichiometric calculations in
# roadRunner. Herbert M Sauro November 2012
#-------------------------------------------------------------

#------------------------------------------
# Change this line for different test files
nameOfResultsFile = 'results_roadRunnerTest_1.txt'


import sys
#pathToTests = 'C:\\Users\\hsauro\\Documents\\Papers\\roadRunnerTestSuite\\'
pathToTests = '..\\tests'


def expectApproximately (a, b, tol):
     diff = a - b
     return abs(diff) < tol

def passMsg (errorFlag):
    if errorFlag:
        print "*****FALSE*****"
    else:
        print "TRUE"

# Empty lines are ignored
# Lines starting with # are also ignored

def readLine (f):
    line = testFile.readline()
    while line == '\n':
       line = testFile.readline()
    if line == '':
       return line

    while (line[0] == '#') or (line == '') or (line[0] == '\n'):
          if line == '':
             return line
          line = testFile.readline();
    return line.strip('\n')

def jumpToNextTest(testFile):
     line = testFile.readline().strip ('\n')
     while line[0] != '[':
        line = testFile.readline().strip ('\n')
     return line

def checkSpeciesConcentrations():
   words = []
   species = []
   m = rrPython.getNumberOfFloatingSpecies()
   for i in range (0,m):
       line = readLine (testFile)
       words = line.split()
       words.append (rrPython.getValue(words[0]))
       species.append (words)

   # Steady State Concentrations
   print "Check Steady State Concentrations:",
   errorFlag = False
   for i in range (0,m):
       expectedValue =  float (species[i][1])
       if expectApproximately (expectedValue, species[i][2], 1E-6) == False:
          errorFlag = True
          break
   passMsg (errorFlag)

def checkFluxes():
   words = []
   fluxes = []
   # Steady State Fluxes
   print "Check Steady State Fluxes",
   errorFlag = False
   m = rrPython.getNumberOfFloatingSpecies()
   n = rrPython.getNumberOfReactions();
   for i in range (0,n):
       line = readLine (testFile)
       words = line.split()
       words.append (rrPython.getValue(words[0]))
       fluxes.append (words)

   for i in range (0,n):
       expectedValue = float (fluxes[i][1])
       if expectApproximately (expectedValue, fluxes[i][2], 1E-6) == False:
          errorFlag = True
          break
   passMsg (errorFlag)

def checkFullJacobian():
  # Jacobian
  print "Check Full Jacobian",
  errorFlag = False
  m = rrPython.getNumberOfFloatingSpecies()
  Jacobian = rrPython.getFullJacobian()
  for i in range(0,m):
      line = readLine (testFile)
      words = line.split()
      for j in range(0,m):
          expectedValue = float(words[j])
          if expectApproximately (expectedValue, Jacobian[i,j], 1E-6) == False:
             errorFlag = True
             break
  passMsg (errorFlag)

def checkIndividualEigenvalues():
   # Eigenvalues
   print "Check Eigenvalues",
   errorFlag = False
   m = rrPython.getNumberOfFloatingSpecies()
   try:
     for i in range(0,m):
        line = readLine (testFile)
        words = line.split()
        eigenvalueName = words[0]
        realPart = rrPython.getValue ('eigen_' + eigenvalueName)
        realPart = float (words[1])
        if (expectApproximately (realPart, eigenvalues[i,0], 1E-6) == False) == False:
            errorFlag = True
            break
     passMsg (errorFlag)
   except:
      print 'Unexpected error in checkIndividualEigenvalues:', sys.exc_info()[0]

def checkEigenvalueMatrix():
   # Eigenvalues
   print "Check Eigenvalue Matrix",
   errorFlag = False
   m = rrPython.getNumberOfFloatingSpecies()
   eigenvalues = rrPython.getEigenvalues()
   for i in range(0,m):
      line = readLine (testFile)
      words = line.split()
      realPart = float (words[0])
      imagPart= float (words[1])
      if (expectApproximately (realPart, eigenvalues[i,0], 1E-6) == False) or (expectApproximately (imagPart, eigenvalues[i,1], 1E-6)) == False:
          errorFlag = True
          break
   passMsg (errorFlag)

def checkStoichiometryMatrix():
   # Stoichiometry matrix
   print "Check Stoichiometry Matrix",
   errorFlag = False
   m = rrPython.getNumberOfFloatingSpecies()
   n = rrPython.getNumberOfReactions();
   st = rrPython.getStoichiometryMatrix()
   for i in range(0,m):
       line = readLine (testFile)
       words = line.split()
       for j in range(0,n):
           if expectApproximately(float (words[j]), st[i,j], 1E-6) == False:
              errorFlag = True
              break
   passMsg (errorFlag)

def checkLinkMatrix():
   # Link matrix
   print "Check Link Matrix",
   errorFlag = False
   m = rrPython.getNumberOfFloatingSpecies()
   st = rrPython.getLinkMatrix()
   for i in range(0,m):
       words = readLine (testFile).split()
       for j in range(0,m):
           if expectApproximately(float (words[j]), st[i,j], 1E-6) == False:
              errorFlag = True
              break
   passMsg (errorFlag)

def checkUnscaledConcentrationControlMatrix():
   # Unscaled Concentration Control matrix
   print "Check Unscaled Concentration Control matrix",
   words = []
   errorFlag = False
   m = rrPython.getNumberOfFloatingSpecies()
   n = rrPython.getNumberOfReactions();
   st = rrPython.getUnscaledConcentrationControlCoefficientMatrix();
   for i in range(0,m):
        words = readLine (testFile).split()
        for j in range(0,n):
            if expectApproximately(float (words[j]), st[i,j], 1E-6) == False:
               errorFlag = True
               break
   passMsg (errorFlag)

def checkUnscaledFluxControlCoefficientMatrix():
   # Unscaled Flux Control matrix
   print "Check Unscaled Flux Control matrix",
   words = []
   errorFlag = False
   n = rrPython.getNumberOfReactions();
   st = rrPython.getUnscaledFluxControlCoefficientMatrix();
   for i in range(0,n):
        words = readLine (testFile).split()
        for j in range(0,n):
            if expectApproximately(float (words[j]), st[i,j], 1E-6) == False:
               errorFlag = True
               break
   passMsg (errorFlag)

def checkUnscaledElasticityMatrix():
  # Jacobian
  print "Check Unscaled Elasticity Matrix",
  errorFlag = False
  m = rrPython.getNumberOfFloatingSpecies()
  uee = rrPython.getUnscaledElasticityMatrix()
  for i in range(0,m):
      line = readLine (testFile)
      words = line.split()
      for j in range(0,m):
          expectedValue = float(words[j])
          if expectApproximately (expectedValue, uee[i,j], 1E-6) == False:
             errorFlag = True
             break
  passMsg (errorFlag)

def checkScaledElasticityMatrix():
  # Jacobian
  print "Check Scaled Elasticity Matrix",
  errorFlag = False
  m = rrPython.getNumberOfFloatingSpecies()
  ee = rrPython.getScaledElasticityMatrix()
  for i in range(0,m):
      line = readLine (testFile)
      words = line.split()
      for j in range(0,m):
          expectedValue = float(words[j])
          if expectApproximately (expectedValue, ee[i,j], 1E-6) == False:
             errorFlag = True
             break
  passMsg (errorFlag)

functions = {'[Species Concentrations]': checkSpeciesConcentrations,
             '[Fluxes]': checkFluxes,
             '[Full Jacobian]': checkFullJacobian,
             '[Eigenvalue Matrix]': checkEigenvalueMatrix,
             '[Individual Eigenvalues]': checkIndividualEigenvalues,
             '[Stoichiometry Matrix]': checkStoichiometryMatrix,
             '[Link Matrix]': checkLinkMatrix,
             '[Unscaled Elasticity Matrix]': checkUnscaledElasticityMatrix,
             '[Scaled Elasticity Matrix]': checkScaledElasticityMatrix
             #'[Unscaled Concentration Control Matrix]': checkUnscaledConcentrationControlMatrix,
             #'[Unscaled Flux Control Matrix]': checkUnscaledFluxControlCoefficientMatrix
             }

# --------------------------------------------------------------

#sys.path.append ('c:\\RoadRunner\\Python')
sys.path.append (pathToTests)

import rrPython
print 'Version: ', rrPython.getVersion()
print 'Build Date: ', rrPython.getBuildDate()
print 'Copyright Message: ', rrPython.getCopyright()


testFile = open (pathToTests + "\\" + nameOfResultsFile, 'r')
modelFileName = pathToTests + "\\" +readLine (testFile)

print "Model File Name:", modelFileName
rrPython.enableLogging()
print "Logging File Name: ", rrPython.getLogFileName()
print "Temp Folder:", rrPython.getTempFolder()

if testFile.readline().strip ('\n') == '[Conservation Laws]':
    line = readLine (testFile)
    if line == 'True':
        rrPython.setComputeAndAssignConservationLaws(1)
    else:
        rrPython.setComputeAndAssignConservationLaws(0)

if rrPython.loadSBMLFromFile(modelFileName) == False:
    print 'Failed to load model'
    print rrPython.getLastError()
    sys.exit(-1)

print
print "Compute Steady State, distance to SteadyState:", rrPython.steadyState()

testId = jumpToNextTest(testFile)
while testId != '[END]':
    if functions.has_key(testId):
       func = functions[testId]
       func()
    else:
        print 'No test found for ' + testId
    testId = jumpToNextTest(testFile)

rrPython.setSteadyStateSelectionList ('S1 S2')

print 'Floating Species IDs: ', rrPython.getFloatingSpeciesIds()
print 'Boundary Species Ids: ', rrPython.getBoundarySpeciesIds()
print 'Global Parameter Ids: ', rrPython.getGlobalParameterIds()
print 'Compartment Ids: ', rrPython.getCompartmentIds()
print 'Reaction Ids: ', rrPython.getReactionIds()
print 'Get Rates of Change Ids: ', rrPython.getRatesOfChangeIds()
#print 'Get Floating Species Initial Condition Ids', rrPython.getFloatingSpeciesInitialConditionIds()
print 'Get Eigenvalue Ids: ', rrPython.getEigenvalueIds()
print 'Get Elasticity Ids: ', rrPython.getElasticityCoefficientIds()
print 'Unscaled Flux Control Ids: ', rrPython.getUnscaledFluxControlCoefficientIds()
print 'Scaled Flux Control Ids: ', rrPython.getFluxControlCoefficientIds()
print 'Unscaled Concentration Control Ids: ', rrPython.getUnscaledConcentrationControlCoefficientIds()
print 'Scaled Concentration Control Ids', rrPython.getConcentrationControlCoefficientIds()

#print 'Steady State Symbols: ', rrPython.getAvailableSteadyStateSymbols()
#print 'Time Course Symbols: ', rrPython.getAvailableTimeCourseSymbols()

print 'Steady State Selection List: ', rrPython.getSteadyStateSelectionList()
print 'Time Course Selection List: ', rrPython.getTimeCourseSelectionList()
#print 'Steady State Values: ', rrPython.computeSteadyStateValues()
print 'Floating Species Concentrations: ', rrPython.getFloatingSpeciesConcentrations()
print 'Boundary Species Concentrations: ', rrPython.getBoundarySpeciesConcentrations()
print 'Get Global Parameter Values: ', rrPython.getGlobalParameterValues()
print 'Get Initial Floating Species Concs: ', rrPython.getFloatingSpeciesInitialConcentrations()
print 'Get Reaction Rates: ', rrPython.getReactionRates()
#print rrPython.getFloatingSpeciesInitialConditionIds()

