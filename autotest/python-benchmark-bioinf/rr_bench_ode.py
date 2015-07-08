#!/usr/bin/env python

# Roadrunner benchmarking script, Apr 2015
# Authors: Andy Somogyi, J Kyle Medley
# Language: Python 2.7.9
# Usage: cd <this_dir> && python bench.py >rr-ode-results.csv

from __future__ import print_function
from roadrunner import *
import time
import csv
from sys import stderr, stdout, exit

# Set tolerance parameters (must match across simulators)
absolute_tol_default = 1.000000e-007
rel_tol_default = 0.0001

Config.setValue(Config.SIMULATEOPTIONS_ABSOLUTE, absolute_tol_default)
Config.setValue(Config.SIMULATEOPTIONS_RELATIVE, rel_tol_default)

tests = [ \
  ('jean_marie', "./jean_marie/Jean_Marie_AMPA16_RobHow_v6.xml", 0.234, 0.0005),
  ('jana_wolf', "./jana_wolf/Jana_WolfGlycolysis.xml", 0.189, 0.3),
  ('biomod14', "./biomod14/BIOMD0000000014.xml", 0.886, 0.015),
  ('biomod33', "./biomod33/BIOMD0000000033.xml", 0.191, 0.06),
  ('00001',   "./00001/00001-sbml-l2v4.xml", 0.084, 0.002),
  ('00002',   "./00002/00002-sbml-l2v4.xml", 0.086, 0.002),
  ('00050',  "./00050/00050-sbml-l2v4.xml",  0.389, 0.007),
  ('00100', "./00100/00100-sbml-l2v4.xml",   0.829, 0.01),
  ('00150', "./00150/00150-sbml-l2v4.xml",   1.230, 0.015),
  ('00200', "./00200/00200-sbml-l2v4.xml",   1.806, 0.02),
  ('00250', "./00250/00250-sbml-l2v4.xml",   3.236, 0.02),
  ('00300', "./00300/00300-sbml-l2v4.xml",   3.188, 0.03),
  ('00350', "./00350/00350-sbml-l2v4.xml",   4.130, 0.03),
  ('00400', "./00400/00400-sbml-l2v4.xml",   4.735, 0.038),
  ('00450', "./00450/00450-sbml-l2v4.xml",   5.211, 0.04),
  ('00500', "./00500/00500-sbml-l2v4.xml",   6.209, 0.045)]

start = 0;
end = 50;
steps = 50;

csvwriter = csv.writer(stdout, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)

# set to true if performance regression detected
regDetected = False

def timeit(name, path, loadTimeCap, runTimeCap):
  print('Model: {}'.format(name), file=stderr)
  startTime = time.time()
  global regDetected
  passFail = 'pass'

  # Load the model
  r=RoadRunner(path)
  #if name == '00500':
    #time.sleep(1)

  loadTime = time.time()
  if loadTime-startTime > loadTimeCap:
    regDetected = True
    passFail = '***FAIL***'

  m=r.simulate(start, end, steps)
  endTime = time.time()
  if endTime-loadTime > runTimeCap:
    regDetected = True
    passFail = '***FAIL***'

  csvwriter.writerow([name, loadTime-startTime, endTime-loadTime, endTime-startTime, passFail])

# First row in output: Version info
print(getVersionStr(), file=stderr)
csvwriter.writerow([getVersionStr()])

for n in range(1):
  print('Trial {}'.format(n+1), file=stderr)
  csvwriter.writerow(['Trial {}'.format(n+1)])
  csvwriter.writerow(['Name', 'Load_time', 'Run_time', 'Total_time'])
  for t in tests:
      timeit(*t)

if regDetected == True:
  exit(1)