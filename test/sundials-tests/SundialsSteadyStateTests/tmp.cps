<?xml version="1.0" encoding="UTF-8"?>
<sbml xmlns="http://www.sbml.org/sbml/level3/version1/core" level="3" version="1">
  <model metaid="__main" id="__main">
    <listOfCompartments>
      <compartment sboTerm="SBO:0000410" id="default_compartment" spatialDimensions="3" size="1" constant="true"/>
    </listOfCompartments>
    <listOfSpecies>
      <species id="scUPA" compartment="default_compartment" initialConcentration="10" hasOnlySubstanceUnits="false" boundaryCondition="false" constant="false"/>
      <species id="PLG" compartment="default_compartment" initialConcentration="10" hasOnlySubstanceUnits="false" boundaryCondition="false" constant="false"/>
      <species id="PLS" compartment="default_compartment" initialConcentration="10" hasOnlySubstanceUnits="false" boundaryCondition="false" constant="false"/>
      <species id="tcUPA" compartment="default_compartment" initialConcentration="10" hasOnlySubstanceUnits="false" boundaryCondition="false" constant="false"/>
    </listOfSpecies>
    <listOfParameters>
      <parameter id="keff1" value="0.0017" constant="true"/>
      <parameter id="keff2" value="1" constant="true"/>
      <parameter id="keff3" value="0.03" constant="true"/>
      <parameter id="n" value="3" constant="true"/>
      <parameter id="u1" value="0.0001" constant="true"/>
      <parameter id="u3" value="0.0001" constant="true"/>
      <parameter id="u2" value="0.001" constant="true"/>
      <parameter id="u4" value="0.001" constant="true"/>
      <parameter id="alpha1" value="9e-05" constant="true"/>
      <parameter id="alpha2" value="0.001" constant="true"/>
    </listOfParameters>
    <listOfReactions>
      <reaction id="r1" reversible="true" fast="false">
        <listOfProducts>
          <speciesReference species="scUPA" stoichiometry="1" constant="true"/>
        </listOfProducts>
        <kineticLaw>
          <math xmlns="http://www.w3.org/1998/Math/MathML">
            <ci> alpha1 </ci>
          </math>
        </kineticLaw>
      </reaction>
      <reaction id="r2" reversible="true" fast="false">
        <listOfReactants>
          <speciesReference species="scUPA" stoichiometry="1" constant="true"/>
        </listOfReactants>
        <kineticLaw>
          <math xmlns="http://www.w3.org/1998/Math/MathML">
            <apply>
              <times/>
              <ci> scUPA </ci>
              <ci> u1 </ci>
            </apply>
          </math>
        </kineticLaw>
      </reaction>
      <reaction id="r3" reversible="true" fast="false">
        <listOfProducts>
          <speciesReference species="PLG" stoichiometry="1" constant="true"/>
        </listOfProducts>
        <kineticLaw>
          <math xmlns="http://www.w3.org/1998/Math/MathML">
            <ci> alpha2 </ci>
          </math>
        </kineticLaw>
      </reaction>
      <reaction id="r4" reversible="true" fast="false">
        <listOfReactants>
          <speciesReference species="PLG" stoichiometry="1" constant="true"/>
        </listOfReactants>
        <kineticLaw>
          <math xmlns="http://www.w3.org/1998/Math/MathML">
            <apply>
              <times/>
              <ci> PLG </ci>
              <ci> u2 </ci>
            </apply>
          </math>
        </kineticLaw>
      </reaction>
      <reaction id="r5" reversible="true" fast="false">
        <listOfReactants>
          <speciesReference species="PLG" stoichiometry="1" constant="true"/>
        </listOfReactants>
        <listOfProducts>
          <speciesReference species="PLS" stoichiometry="1" constant="true"/>
        </listOfProducts>
        <listOfModifiers>
          <modifierSpeciesReference species="scUPA"/>
        </listOfModifiers>
        <kineticLaw>
          <math xmlns="http://www.w3.org/1998/Math/MathML">
            <apply>
              <times/>
              <ci> keff1 </ci>
              <ci> scUPA </ci>
              <ci> PLG </ci>
            </apply>
          </math>
        </kineticLaw>
      </reaction>
      <reaction id="r6" reversible="true" fast="false">
        <listOfReactants>
          <speciesReference species="scUPA" stoichiometry="1" constant="true"/>
        </listOfReactants>
        <listOfProducts>
          <speciesReference species="tcUPA" stoichiometry="1" constant="true"/>
        </listOfProducts>
        <listOfModifiers>
          <modifierSpeciesReference species="PLS"/>
        </listOfModifiers>
        <kineticLaw>
          <math xmlns="http://www.w3.org/1998/Math/MathML">
            <apply>
              <times/>
              <ci> keff2 </ci>
              <ci> scUPA </ci>
              <apply>
                <power/>
                <ci> PLS </ci>
                <ci> n </ci>
              </apply>
            </apply>
          </math>
        </kineticLaw>
      </reaction>
      <reaction id="r7" reversible="true" fast="false">
        <listOfReactants>
          <speciesReference species="PLG" stoichiometry="1" constant="true"/>
        </listOfReactants>
        <listOfProducts>
          <speciesReference species="PLS" stoichiometry="1" constant="true"/>
        </listOfProducts>
        <listOfModifiers>
          <modifierSpeciesReference species="tcUPA"/>
        </listOfModifiers>
        <kineticLaw>
          <math xmlns="http://www.w3.org/1998/Math/MathML">
            <apply>
              <times/>
              <ci> keff3 </ci>
              <ci> tcUPA </ci>
              <ci> PLG </ci>
            </apply>
          </math>
        </kineticLaw>
      </reaction>
      <reaction id="r8" reversible="true" fast="false">
        <listOfReactants>
          <speciesReference species="tcUPA" stoichiometry="1" constant="true"/>
        </listOfReactants>
        <kineticLaw>
          <math xmlns="http://www.w3.org/1998/Math/MathML">
            <apply>
              <times/>
              <ci> tcUPA </ci>
              <ci> u3 </ci>
            </apply>
          </math>
        </kineticLaw>
      </reaction>
      <reaction id="r9" reversible="true" fast="false">
        <listOfReactants>
          <speciesReference species="PLS" stoichiometry="1" constant="true"/>
        </listOfReactants>
        <kineticLaw>
          <math xmlns="http://www.w3.org/1998/Math/MathML">
            <apply>
              <times/>
              <ci> PLS </ci>
              <ci> u4 </ci>
            </apply>
          </math>
        </kineticLaw>
      </reaction>
    </listOfReactions>
  </model>
</sbml>
