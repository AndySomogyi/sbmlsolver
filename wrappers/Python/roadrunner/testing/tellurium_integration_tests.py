"""Test the integration between roadrunner and tellurium.

Tellurium and roadrunner need to work seamlessly together, these tests
are designed to exercise this interface, and ensure roadrunner can't be
updated in a way that breaks tellurium.

These tests require tellurium, with the roadrunner version it comes with
removed and replaced with the latest experimental roadrunner that we're
testing. For that reason, these tests should ideally be run in their
own conda environment, otherwise the script may mess with your
tellurium / roadrunner setup. These tests are therefore not added to the
roadrunner test suite, but are run separately on the roadrunner CI
system. To run these manually, its best to create a new Python environment:

    conda create --name rr-te-integration python=3.9
    conda activate rr-te-integration

"""

import unittest
import os, sys, glob

thisDir = os.path.dirname(os.path.realpath(__file__))
rr_site_packages = os.path.dirname(os.path.dirname(thisDir))

sys.path += [
    r"D:\tellurium",
    # r"D:\roadrunner\roadrunner\cmake-build-release-visual-studio---with-python\lib\site-packages",
    # rr_site_packages,
]


# try:
#     import tellurium as te
# except ImportError:
#     if we do not have tellurium, install it
    # install_tellurium = f"{sys.executable} -m pip install --no-input tellurium"
    # uninstall_libroadrunner = f"{sys.executable} -m pip uninstall --yes libroadrunner"
    # print(f"running {install_tellurium}")
    # os.system(install_tellurium)
    # the tellurium install will install the version of roadrunner on pip, so we remove
    # os.system(uninstall_libroadrunner)

import tellurium as te
# import roadrunner
from roadrunner.testing import TestModelFactory as tmf

# sbml = tmf.SimpleFlux().str()
# print(sbml)

sbml = """<?xml version="1.0" encoding="UTF-8"?>
<sbml xmlns="http://www.sbml.org/sbml/level3/version1/core" level="3" version="1">
  <model metaid="x" id="x">
    <listOfCompartments>
      <compartment sboTerm="SBO:0000410" id="default_compartment" spatialDimensions="3" size="1" constant="true"/>
    </listOfCompartments>
    <listOfSpecies>
      <species id="S1" compartment="default_compartment" initialConcentration="10" hasOnlySubstanceUnits="false" boundaryCondition="false" constant="false"/>
      <species id="S2" compartment="default_compartment" initialConcentration="1" hasOnlySubstanceUnits="false" boundaryCondition="false" constant="false"/>
    </listOfSpecies>
    <listOfParameters>
      <parameter id="kf" value="0.1" constant="true"/>
      <parameter id="kb" value="0.01" constant="true"/>
    </listOfParameters>
    <listOfReactions>
      <reaction id="_J0" reversible="false" fast="false">
        <listOfReactants>
          <speciesReference species="S1" stoichiometry="1" constant="true"/>
        </listOfReactants>
        <listOfProducts>
          <speciesReference species="S2" stoichiometry="1" constant="true"/>
        </listOfProducts>
        <kineticLaw>
          <math xmlns="http://www.w3.org/1998/Math/MathML">
            <apply>
              <times/>
              <ci> kf </ci>
              <ci> S1 </ci>
            </apply>
          </math>
        </kineticLaw>
      </reaction>
      <reaction id="_J1" reversible="false" fast="false">
        <listOfReactants>
          <speciesReference species="S2" stoichiometry="1" constant="true"/>
        </listOfReactants>
        <listOfProducts>
          <speciesReference species="S1" stoichiometry="1" constant="true"/>
        </listOfProducts>
        <kineticLaw>
          <math xmlns="http://www.w3.org/1998/Math/MathML">
            <apply>
              <times/>
              <ci> kb </ci>
              <ci> S2 </ci>
            </apply>
          </math>
        </kineticLaw>
      </reaction>
    </listOfReactions>
  </model>
</sbml>
"""

# roadrunner.Logger.setLevel(roadrunner.Logger.LOG_CRITICAL)
# print(roadrunner.__version__)
te.loads(sbml)

# print(roadrunner.RoadRunner(sbml))

# r = te.roadrunner.ExtendedRoadRunner(sbml)
# #
# #
# class InheritFromRoadRunnerInPython(roadrunner.RoadRunner):
#
#     def __init__(self, *args, **kwargs):
#         super(InheritFromRoadRunnerInPython, self).__init__(*args, **kwargs)
#
#
# r = InheritFromRoadRunnerInPython(sbml)
#
# print(r.simulate(0, 10, 11))
#


















# # print(tmf.TestModel.__subclasses__())
