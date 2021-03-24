import os
import sys
import unittest

from roadrunner.roadrunner import RoadRunner

# S1 <=> S2
# Network was reduced manually using conservation laws
# This model solves out of the box - presimulation not approx is necessary
simple_flux_manually_reduced = """<?xml version="1.0" encoding="UTF-8"?>
<sbml xmlns="http://www.sbml.org/sbml/level3/version1/core" level="3" version="1">
  <model metaid="x" id="x">
    <listOfCompartments>
      <compartment sboTerm="SBO:0000410" id="default_compartment" spatialDimensions="3" size="1" constant="true"/>
    </listOfCompartments>
    <listOfSpecies>
      <species id="S1" compartment="default_compartment" initialConcentration="10" hasOnlySubstanceUnits="false" boundaryCondition="false" constant="false"/>
    </listOfSpecies>
    <listOfParameters>
      <parameter id="S20" value="1" constant="true"/>
      <parameter id="Total" constant="false"/>
      <parameter id="kf" value="0.1" constant="true"/>
      <parameter id="kb" value="0.01" constant="true"/>
      <parameter id="S2" constant="false"/>
    </listOfParameters>
    <listOfInitialAssignments>
      <initialAssignment symbol="Total">
        <math xmlns="http://www.w3.org/1998/Math/MathML">
          <apply>
            <plus/>
            <ci> S1 </ci>
            <ci> S20 </ci>
          </apply>
        </math>
      </initialAssignment>
    </listOfInitialAssignments>
    <listOfRules>
      <assignmentRule variable="S2">
        <math xmlns="http://www.w3.org/1998/Math/MathML">
          <apply>
            <minus/>
            <ci> Total </ci>
            <ci> S1 </ci>
          </apply>
        </math>
      </assignmentRule>
    </listOfRules>
    <listOfReactions>
      <reaction id="_J0" reversible="false" fast="false">
        <listOfReactants>
          <speciesReference species="S1" stoichiometry="1" constant="true"/>
        </listOfReactants>
        <kineticLaw>
          <math xmlns="http://www.w3.org/1998/Math/MathML">
            <apply>
              <plus/>
              <apply>
                <times/>
                <apply>
                  <minus/>
                  <ci> kf </ci>
                </apply>
                <ci> S1 </ci>
              </apply>
              <apply>
                <times/>
                <ci> kb </ci>
                <apply>
                  <minus/>
                  <ci> Total </ci>
                  <ci> S1 </ci>
                </apply>
              </apply>
            </apply>
          </math>
        </kineticLaw>
      </reaction>
    </listOfReactions>
  </model>
</sbml>"""

# S1 <=> S2
# requires moiety conservation analysis before it'll solve
simple_flux_sbml = """<?xml version="1.0" encoding="UTF-8"?>
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
</sbml>"""

# -> S1 -> S2 ->
# Model does not solve from starting conditions.
# This model requires a little presimulation before it'll solve
open_linear_flux_sbml = """<?xml version="1.0" encoding="UTF-8"?>
<sbml xmlns="http://www.sbml.org/sbml/level3/version1/core" level="3" version="1">
  <model metaid="x" id="x">
    <listOfCompartments>
      <compartment sboTerm="SBO:0000410" id="default_compartment" spatialDimensions="3" size="1" constant="true"/>
    </listOfCompartments>
    <listOfSpecies>
      <species id="S1" compartment="default_compartment" initialConcentration="0" hasOnlySubstanceUnits="false" boundaryCondition="false" constant="false"/>
      <species id="S2" compartment="default_compartment" initialConcentration="0" hasOnlySubstanceUnits="false" boundaryCondition="false" constant="false"/>
    </listOfSpecies>
    <listOfParameters>
      <parameter id="kin" value="1" constant="true"/>
      <parameter id="kf" value="0.1" constant="true"/>
      <parameter id="kout" value="0.2" constant="true"/>
      <parameter id="kb" value="0.01" constant="true"/>
    </listOfParameters>
    <listOfReactions>
      <reaction id="_J0" reversible="false" fast="false">
        <listOfProducts>
          <speciesReference species="S1" stoichiometry="1" constant="true"/>
        </listOfProducts>
        <kineticLaw>
          <math xmlns="http://www.w3.org/1998/Math/MathML">
            <ci> kin </ci>
          </math>
        </kineticLaw>
      </reaction>
      <reaction id="_J1" reversible="false" fast="false">
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
      <reaction id="_J2" reversible="false" fast="false">
        <listOfReactants>
          <speciesReference species="S2" stoichiometry="1" constant="true"/>
        </listOfReactants>
        <kineticLaw>
          <math xmlns="http://www.w3.org/1998/Math/MathML">
            <apply>
              <times/>
              <ci> S2 </ci>
              <ci> kout </ci>
            </apply>
          </math>
        </kineticLaw>
      </reaction>
    </listOfReactions>
  </model>
</sbml>"""


def solve_simple_flux_manually_reduced():
    """Solving this model requires neither approximation nor presimulation"""
    rr = RoadRunner(simple_flux_manually_reduced)
    rr.setSteadyStateSolver("NewtonIteration")
    ss_solver = rr.getSteadyStateSolver()

    # The default behaviour of these two options has been turned to True
    # (easily changed back if you want). The behaviour now is this:
    #   if allow_presimulation is on:
    #       try first without it and fall back on allow_presimulation if needed.
    # Similarly for allow_approx.
    # When both are True (the default), roadrunner tries without any modifiers,
    # then it tries the presimulation and then it tries to approximate.
    #
    # Side note: I'd like to extend the presimulation to a presimulation program
    # i.e. try 0.1, 1, 10, 100 etc. up to prehaps 1e6.
    #
    # Another side note, now presimulation only uses max_steps argument for calculating
    # stepsize. See PresimulationDecorator and ApproxSteadyStateDecorator classes for details)
    ss_solver.setValue("allow_presimulation", False)
    ss_solver.setValue("allow_approx", False)

    rr.steadyState()
    return rr.getFloatingSpeciesConcentrationsNamedArray()

def solve_simple_flux_conservation_manual():
    """Solve simple flux: use moiety conservation option

    In previous versions of roadrunner, users were expected to
    manually turn on moiety conservation. This feature still
    works as expected
    """
    rr = RoadRunner(simple_flux_sbml)
    rr.setSteadyStateSolver("NewtonIteration")
    rr.conservedMoietyAnalysis = True
    ss_solver = rr.getSteadyStateSolver()

    ss_solver.setValue("allow_presimulation", False)
    ss_solver.setValue("allow_approx", False)

    rr.steadyState()
    return rr.getFloatingSpeciesConcentrationsNamedArray()

def solve_simple_flux_auto_conservation():
    """Solve simple flux: demonstrate automatic moiety conservation

    By computing the rank of the jacobian matrix (see SVD class),
    we can now reliably automate the when moiety conservation
    needs to be turned on (see RoadRunner::steadyState).

    Therefore, this function is equivalent solve_simple_flux_conservation_manual

    """
    rr = RoadRunner(simple_flux_sbml)
    rr.setSteadyStateSolver("NewtonIteration")
    ss_solver = rr.getSteadyStateSolver()
    ss_solver.setValue("allow_presimulation", False)
    ss_solver.setValue("allow_approx", False)

    rr.steadyState()
    return rr.getFloatingSpeciesConcentrationsNamedArray()
    # print([i for i in dir(rr) if "float" in i.lower()])

def solve_open_linear_flux_sbml_error():
    """demonstrate presimulation: failure

    This model does not require moiety conservation analysis, but
    the from the starting conditions (S1=0, S2=0), NewtonIteration
    cannot solve the problem. This function demonstrates what happens
    when this happens. The error messages are propaged directly from
    kinsol

    """
    rr = RoadRunner(open_linear_flux_sbml)
    rr.setSteadyStateSolver("NewtonIteration")
    ss_solver = rr.getSteadyStateSolver()
    ss_solver.setValue("allow_presimulation", False)
    ss_solver.setValue("allow_approx", False)

    rr.steadyState()
    return rr.getFloatingSpeciesConcentrationsNamedArray()

def solve_open_linear_flux_sbml_use_presimulation():
    """demonstrate presimulation:

    Turning on presimulation does a single step integration
    to `presimulation_time` from where it'll try to solve
    for steady state. See PresimulationDecorator.

    """
    rr = RoadRunner(open_linear_flux_sbml)
    rr.setSteadyStateSolver("NewtonIteration")
    ss_solver = rr.getSteadyStateSolver()
    ss_solver.setValue("allow_presimulation", True)
    ss_solver.setValue("presimulation_time", 3.5)
    ss_solver.setValue("allow_approx", False)

    rr.steadyState()
    return rr.getFloatingSpeciesConcentrationsNamedArray()

def solve_open_linear_flux_sbml_use_approximation_but_fail():
    """demonstrate approximation: failure

    We could use approximation instead of presimulation
    to solve the open_linear_flux model. But we need to
    be aware of the `approx_time` and the `approx_maximum_steps`
    arguments. Approx_time dictates when the approximation is
    to occur while approx_maximum_steps determines the step size used.

    Note that the name `approx_maximum_steps` is now misleading because a full
    timeseries simulation is *not* conducted, not necessary. Instead this
    argument is used to compute a step size. While it would be better to have a
    step_size argument, it did not fit into the existing options for steady state,
    so I thought it better to open a discussion for how we (you) want this.
    See ApproxSteadyStateDecorator::solve for details.

    It should be noted that kinsol uses two different tolerances, `fnormtol`
    and `scsteptol`. I'm not clear on the mathematics so it'd be worth taking a
    look at the `mathematical considerations` section from the kinsol docs to
    help decide which tolerance we use in the approximation (both?).

    This function will throw a RunTimeError because the computed `fnormtol`
    is larger than the approx_tolerance.
    """
    rr = RoadRunner(open_linear_flux_sbml)
    rr.setSteadyStateSolver("NewtonIteration")
    ss_solver = rr.getSteadyStateSolver()
    ss_solver.setValue("allow_presimulation", False)
    ss_solver.setValue("allow_approx", True)
    ss_solver.setValue("approx_time", 3.5)
    ss_solver.setValue("approx_maximum_steps", 10)
    ss_solver.setValue("approx_tolerance", 1e-6)

    rr.steadyState()
    return rr.getFloatingSpeciesConcentrationsNamedArray()

def solve_open_linear_flux_sbml_use_approximation_but_pass():
    """demonstrate approximation: pass

    By changing the approx_time argument, we find a flatter part of
    the time series whereby the computed tolerance is small enough to
    be considered an approximate steady state.

    """
    rr = RoadRunner(open_linear_flux_sbml)
    rr.setSteadyStateSolver("NewtonIteration")
    ss_solver = rr.getSteadyStateSolver()
    ss_solver.setValue("allow_presimulation", False)
    ss_solver.setValue("allow_approx", True)
    ss_solver.setValue("approx_time", 150)
    ss_solver.setValue("approx_maximum_steps", 1e6)
    ss_solver.setValue("approx_tolerance", 1e-6)

    rr.steadyState()
    return rr.getFloatingSpeciesConcentrationsNamedArray()


def use_linesearch():
    """Alternative strategy: linesearch (see kinsol docs for details).

    Kinsol supports "basic newton iteration" which is the currently the default option
    but also has an alternative strategy called `linesearch`. This linesearch algorithm
    is a variant on newton iteration. It might be worth setting this as default (?).

    Currently we have a "strategy" option and the user can pick between "basic" or "linesearch".
    However, an alternative design would be to have two forms of NewtonIteration solvers. The implementation
    would be exactly the same, its just how they are presented to the users. Currently we have

        rr = RoadRunner(open_linear_flux_sbml)
        rr.setSteadyStateSolver("NewtonIteration")
        ss_solver = rr.getSteadyStateSolver()
        ss_solver.setValue("strategy", "linesearch")

    But we could equally have

        rr = RoadRunner(open_linear_flux_sbml)
        rr.setSteadyStateSolver("BasicNewtonIteration")

    and
        rr = RoadRunner(open_linear_flux_sbml)
        rr.setSteadyStateSolver("LinesearchNewtonIteration")

    I'm leaning towards the latter option. Thoughts?
    """
    rr = RoadRunner(open_linear_flux_sbml)
    rr.setSteadyStateSolver("NewtonIteration")
    ss_solver = rr.getSteadyStateSolver()
    ss_solver.setValue("strategy", "linesearch")

    # remember this mode fails without Presimulation, but Presimulation is hidden from the user in this case
    rr.steadyState()
    return rr.getFloatingSpeciesConcentrationsNamedArray()


if __name__ == "__main__":


    print(solve_simple_flux_manually_reduced())
    # print(solve_simple_flux_conservation_manual())
    # print(solve_simple_flux_auto_conservation())
    # print(solve_open_linear_flux_sbml_error()) # will error
    # print(solve_open_linear_flux_sbml_use_presimulation())
    # print(solve_open_linear_flux_sbml_use_approximation_but_fail())
    # print(solve_open_linear_flux_sbml_use_approximation_but_pass())
    # print(use_linesearch())






