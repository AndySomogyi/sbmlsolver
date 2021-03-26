import tellurium as te

sbml = """<?xml version="1.0" encoding="UTF-8"?>
<sbml xmlns="http://www.sbml.org/sbml/level3/version1/core" level="3" version="1">
  <model metaid="Brown2004_NGF_EGF_signaling" id="Brown2004_NGF_EGF_signaling">
    <listOfCompartments>
      <compartment id="cell" spatialDimensions="3" size="1" constant="true"/>
    </listOfCompartments>
    <listOfSpecies>
      <species id="EGF" compartment="cell" initialConcentration="10002000" hasOnlySubstanceUnits="false" boundaryCondition="false" constant="false"/>
      <species id="NGF" compartment="cell" initialConcentration="456000" hasOnlySubstanceUnits="false" boundaryCondition="false" constant="false"/>
      <species id="freeEGFReceptor" compartment="cell" initialConcentration="80000" hasOnlySubstanceUnits="false" boundaryCondition="false" constant="false"/>
      <species id="boundEGFReceptor" compartment="cell" initialConcentration="0" hasOnlySubstanceUnits="false" boundaryCondition="false" constant="false"/>
      <species id="freeNGFReceptor" compartment="cell" initialConcentration="10000" hasOnlySubstanceUnits="false" boundaryCondition="false" constant="false"/>
      <species id="boundNGFReceptor" compartment="cell" initialConcentration="0" hasOnlySubstanceUnits="false" boundaryCondition="false" constant="false"/>
      <species id="SosInactive" compartment="cell" initialConcentration="120000" hasOnlySubstanceUnits="false" boundaryCondition="false" constant="false"/>
      <species id="SosActive" compartment="cell" initialConcentration="0" hasOnlySubstanceUnits="false" boundaryCondition="false" constant="false"/>
      <species id="P90RskInactive" compartment="cell" initialConcentration="120000" hasOnlySubstanceUnits="false" boundaryCondition="false" constant="false"/>
      <species id="P90RskActive" compartment="cell" initialConcentration="0" hasOnlySubstanceUnits="false" boundaryCondition="false" constant="false"/>
      <species id="RasInactive" compartment="cell" initialConcentration="120000" hasOnlySubstanceUnits="false" boundaryCondition="false" constant="false"/>
      <species id="RasActive" compartment="cell" initialConcentration="0" hasOnlySubstanceUnits="false" boundaryCondition="false" constant="false"/>
      <species id="RasGapActive" compartment="cell" initialConcentration="120000" hasOnlySubstanceUnits="false" boundaryCondition="true" constant="false"/>
      <species id="Raf1Inactive" compartment="cell" initialConcentration="120000" hasOnlySubstanceUnits="false" boundaryCondition="false" constant="false"/>
      <species id="Raf1Active" compartment="cell" initialConcentration="0" hasOnlySubstanceUnits="false" boundaryCondition="false" constant="false"/>
      <species id="BRafInactive" compartment="cell" initialConcentration="120000" hasOnlySubstanceUnits="false" boundaryCondition="false" constant="false"/>
      <species id="BRafActive" compartment="cell" initialConcentration="0" hasOnlySubstanceUnits="false" boundaryCondition="false" constant="false"/>
      <species id="MekInactive" compartment="cell" initialConcentration="600000" hasOnlySubstanceUnits="false" boundaryCondition="false" constant="false"/>
      <species id="MekActive" compartment="cell" initialConcentration="0" hasOnlySubstanceUnits="false" boundaryCondition="false" constant="false"/>
      <species id="ErkInactive" compartment="cell" initialConcentration="600000" hasOnlySubstanceUnits="false" boundaryCondition="false" constant="false"/>
      <species id="ErkActive" compartment="cell" initialConcentration="0" hasOnlySubstanceUnits="false" boundaryCondition="false" constant="false"/>
      <species id="PI3KInactive" compartment="cell" initialConcentration="120000" hasOnlySubstanceUnits="false" boundaryCondition="false" constant="false"/>
      <species id="PI3KActive" compartment="cell" initialConcentration="0" hasOnlySubstanceUnits="false" boundaryCondition="false" constant="false"/>
      <species id="AktInactive" compartment="cell" initialConcentration="120000" hasOnlySubstanceUnits="false" boundaryCondition="false" constant="false"/>
      <species id="AktActive" compartment="cell" initialConcentration="0" hasOnlySubstanceUnits="false" boundaryCondition="false" constant="false"/>
      <species id="C3GInactive" compartment="cell" initialConcentration="120000" hasOnlySubstanceUnits="false" boundaryCondition="false" constant="false"/>
      <species id="C3GActive" compartment="cell" initialConcentration="0" hasOnlySubstanceUnits="false" boundaryCondition="false" constant="false"/>
      <species id="Rap1Inactive" compartment="cell" initialConcentration="120000" hasOnlySubstanceUnits="false" boundaryCondition="false" constant="false"/>
      <species id="Rap1Active" compartment="cell" initialConcentration="0" hasOnlySubstanceUnits="false" boundaryCondition="false" constant="false"/>
      <species id="RapGapActive" compartment="cell" initialConcentration="120000" hasOnlySubstanceUnits="false" boundaryCondition="true" constant="false"/>
      <species id="PP2AActive" compartment="cell" initialConcentration="120000" hasOnlySubstanceUnits="false" boundaryCondition="true" constant="false"/>
      <species id="Raf1PPtase" compartment="cell" initialConcentration="120000" hasOnlySubstanceUnits="false" boundaryCondition="true" constant="false"/>
    </listOfSpecies>
    <listOfParameters>
      <parameter id="krbEGF" value="2.18503e-05" constant="true"/>
      <parameter id="kruEGF" value="0.0121008" constant="true"/>
      <parameter id="krbNGF" value="1.38209e-07" constant="true"/>
      <parameter id="kruNGF" value="0.00723811" constant="true"/>
      <parameter id="kEGF" value="694.731" constant="true"/>
      <parameter id="KmEGF" value="6086070" constant="true"/>
      <parameter id="kNGF" value="389.428" constant="true"/>
      <parameter id="KmNGF" value="2112.66" constant="true"/>
      <parameter id="kdSos" value="1611.97" constant="true"/>
      <parameter id="KmdSos" value="896896" constant="true"/>
      <parameter id="kSos" value="32.344" constant="true"/>
      <parameter id="KmSos" value="35954.3" constant="true"/>
      <parameter id="kRasGap" value="1509.36" constant="true"/>
      <parameter id="KmRasGap" value="1432410" constant="true"/>
      <parameter id="kRasToRaf1" value="0.884096" constant="true"/>
      <parameter id="KmRasToRaf1" value="62464.6" constant="true"/>
      <parameter id="kpRaf1" value="185.759" constant="true"/>
      <parameter id="KmpRaf1" value="4768350" constant="true"/>
      <parameter id="kpBRaf" value="125.089" constant="true"/>
      <parameter id="KmpBRaf" value="157948" constant="true"/>
      <parameter id="kpMekCytoplasmic" value="9.85367" constant="true"/>
      <parameter id="KmpMekCytoplasmic" value="1007340" constant="true"/>
      <parameter id="kdMek" value="2.83243" constant="true"/>
      <parameter id="KmdMek" value="518753" constant="true"/>
      <parameter id="kdErk" value="8.8912" constant="true"/>
      <parameter id="KmdErk" value="3496490" constant="true"/>
      <parameter id="kdRaf1" value="0.126329" constant="true"/>
      <parameter id="KmdRaf1" value="1061.71" constant="true"/>
      <parameter id="kpP90Rsk" value="0.0213697" constant="true"/>
      <parameter id="KmpP90Rsk" value="763523" constant="true"/>
      <parameter id="kPI3K" value="10.6737" constant="true"/>
      <parameter id="KmPI3K" value="184912" constant="true"/>
      <parameter id="kPI3KRas" value="0.0771067" constant="true"/>
      <parameter id="KmPI3KRas" value="272056" constant="true"/>
      <parameter id="kAkt" value="0.0566279" constant="true"/>
      <parameter id="KmAkt" value="653951" constant="true"/>
      <parameter id="kdRaf1ByAkt" value="15.1212" constant="true"/>
      <parameter id="KmRaf1ByAkt" value="119355" constant="true"/>
      <parameter id="kC3GNGF" value="146.912" constant="true"/>
      <parameter id="KmC3GNGF" value="12876.2" constant="true"/>
      <parameter id="kC3G" value="1.40145" constant="true"/>
      <parameter id="KmC3G" value="10965.6" constant="true"/>
      <parameter id="kRapGap" value="27.265" constant="true"/>
      <parameter id="KmRapGap" value="295990" constant="true"/>
      <parameter id="kRap1ToBRaf" value="2.20995" constant="true"/>
      <parameter id="KmRap1ToBRaf" value="1025460" constant="true"/>
      <parameter id="kdBRaf" value="441.287" constant="true"/>
      <parameter id="KmdBRaf" value="10879500" constant="true"/>
    </listOfParameters>
    <listOfReactions>
      <reaction id="EGFBindingReaction" reversible="false" fast="false">
        <listOfReactants>
          <speciesReference species="EGF" stoichiometry="1" constant="true"/>
          <speciesReference species="freeEGFReceptor" stoichiometry="1" constant="true"/>
        </listOfReactants>
        <listOfProducts>
          <speciesReference species="boundEGFReceptor" stoichiometry="1" constant="true"/>
        </listOfProducts>
        <kineticLaw>
          <math xmlns="http://www.w3.org/1998/Math/MathML">
            <apply>
              <times/>
              <ci> cell </ci>
              <ci> krbEGF </ci>
              <ci> EGF </ci>
              <ci> freeEGFReceptor </ci>
            </apply>
          </math>
        </kineticLaw>
      </reaction>
      <reaction id="EGFUnbindingReaction" reversible="false" fast="false">
        <listOfReactants>
          <speciesReference species="boundEGFReceptor" stoichiometry="1" constant="true"/>
        </listOfReactants>
        <listOfProducts>
          <speciesReference species="EGF" stoichiometry="1" constant="true"/>
          <speciesReference species="freeEGFReceptor" stoichiometry="1" constant="true"/>
        </listOfProducts>
        <kineticLaw>
          <math xmlns="http://www.w3.org/1998/Math/MathML">
            <apply>
              <times/>
              <ci> cell </ci>
              <ci> kruEGF </ci>
              <ci> boundEGFReceptor </ci>
            </apply>
          </math>
        </kineticLaw>
      </reaction>
      <reaction id="NGFBindingReaction" reversible="false" fast="false">
        <listOfReactants>
          <speciesReference species="NGF" stoichiometry="1" constant="true"/>
          <speciesReference species="freeNGFReceptor" stoichiometry="1" constant="true"/>
        </listOfReactants>
        <listOfProducts>
          <speciesReference species="boundNGFReceptor" stoichiometry="1" constant="true"/>
        </listOfProducts>
        <kineticLaw>
          <math xmlns="http://www.w3.org/1998/Math/MathML">
            <apply>
              <times/>
              <ci> krbNGF </ci>
              <ci> NGF </ci>
              <ci> freeNGFReceptor </ci>
              <ci> cell </ci>
            </apply>
          </math>
        </kineticLaw>
      </reaction>
      <reaction id="NGFUnbindingReaction" reversible="false" fast="false">
        <listOfReactants>
          <speciesReference species="boundNGFReceptor" stoichiometry="1" constant="true"/>
        </listOfReactants>
        <listOfProducts>
          <speciesReference species="NGF" stoichiometry="1" constant="true"/>
          <speciesReference species="freeNGFReceptor" stoichiometry="1" constant="true"/>
        </listOfProducts>
        <kineticLaw>
          <math xmlns="http://www.w3.org/1998/Math/MathML">
            <apply>
              <times/>
              <ci> kruNGF </ci>
              <ci> boundNGFReceptor </ci>
              <ci> cell </ci>
            </apply>
          </math>
        </kineticLaw>
      </reaction>
      <reaction id="SosActivationByEGFReaction" reversible="true" fast="false">
        <listOfReactants>
          <speciesReference species="SosInactive" stoichiometry="1" constant="true"/>
        </listOfReactants>
        <listOfProducts>
          <speciesReference species="SosActive" stoichiometry="1" constant="true"/>
        </listOfProducts>
        <listOfModifiers>
          <modifierSpeciesReference species="boundEGFReceptor"/>
        </listOfModifiers>
        <kineticLaw>
          <math xmlns="http://www.w3.org/1998/Math/MathML">
            <apply>
              <times/>
              <ci> cell </ci>
              <apply>
                <divide/>
                <apply>
                  <times/>
                  <ci> kEGF </ci>
                  <ci> boundEGFReceptor </ci>
                  <ci> SosInactive </ci>
                </apply>
                <apply>
                  <plus/>
                  <ci> SosInactive </ci>
                  <ci> KmEGF </ci>
                </apply>
              </apply>
            </apply>
          </math>
        </kineticLaw>
      </reaction>
      <reaction id="SosActivationByNGFReaction" reversible="true" fast="false">
        <listOfReactants>
          <speciesReference species="SosInactive" stoichiometry="1" constant="true"/>
        </listOfReactants>
        <listOfProducts>
          <speciesReference species="SosActive" stoichiometry="1" constant="true"/>
        </listOfProducts>
        <listOfModifiers>
          <modifierSpeciesReference species="boundNGFReceptor"/>
        </listOfModifiers>
        <kineticLaw>
          <math xmlns="http://www.w3.org/1998/Math/MathML">
            <apply>
              <times/>
              <ci> cell </ci>
              <apply>
                <divide/>
                <apply>
                  <times/>
                  <ci> kNGF </ci>
                  <ci> boundNGFReceptor </ci>
                  <ci> SosInactive </ci>
                </apply>
                <apply>
                  <plus/>
                  <ci> SosInactive </ci>
                  <ci> KmNGF </ci>
                </apply>
              </apply>
            </apply>
          </math>
        </kineticLaw>
      </reaction>
      <reaction id="SosDeactivationReaction" reversible="true" fast="false">
        <listOfReactants>
          <speciesReference species="SosActive" stoichiometry="1" constant="true"/>
        </listOfReactants>
        <listOfProducts>
          <speciesReference species="SosInactive" stoichiometry="1" constant="true"/>
        </listOfProducts>
        <listOfModifiers>
          <modifierSpeciesReference species="P90RskActive"/>
        </listOfModifiers>
        <kineticLaw>
          <math xmlns="http://www.w3.org/1998/Math/MathML">
            <apply>
              <times/>
              <ci> cell </ci>
              <apply>
                <divide/>
                <apply>
                  <times/>
                  <ci> kdSos </ci>
                  <ci> P90RskActive </ci>
                  <ci> SosActive </ci>
                </apply>
                <apply>
                  <plus/>
                  <ci> SosActive </ci>
                  <ci> KmdSos </ci>
                </apply>
              </apply>
            </apply>
          </math>
        </kineticLaw>
      </reaction>
      <reaction id="RasActivationReaction" reversible="true" fast="false">
        <listOfReactants>
          <speciesReference species="RasInactive" stoichiometry="1" constant="true"/>
        </listOfReactants>
        <listOfProducts>
          <speciesReference species="RasActive" stoichiometry="1" constant="true"/>
        </listOfProducts>
        <listOfModifiers>
          <modifierSpeciesReference species="SosActive"/>
        </listOfModifiers>
        <kineticLaw>
          <math xmlns="http://www.w3.org/1998/Math/MathML">
            <apply>
              <times/>
              <ci> cell </ci>
              <apply>
                <divide/>
                <apply>
                  <times/>
                  <ci> kSos </ci>
                  <ci> SosActive </ci>
                  <ci> RasInactive </ci>
                </apply>
                <apply>
                  <plus/>
                  <ci> RasInactive </ci>
                  <ci> KmSos </ci>
                </apply>
              </apply>
            </apply>
          </math>
        </kineticLaw>
      </reaction>
      <reaction id="RasDeactivationReaction" reversible="true" fast="false">
        <listOfReactants>
          <speciesReference species="RasActive" stoichiometry="1" constant="true"/>
        </listOfReactants>
        <listOfProducts>
          <speciesReference species="RasInactive" stoichiometry="1" constant="true"/>
        </listOfProducts>
        <listOfModifiers>
          <modifierSpeciesReference species="RasGapActive"/>
        </listOfModifiers>
        <kineticLaw>
          <math xmlns="http://www.w3.org/1998/Math/MathML">
            <apply>
              <times/>
              <ci> cell </ci>
              <apply>
                <divide/>
                <apply>
                  <times/>
                  <ci> kRasGap </ci>
                  <ci> RasGapActive </ci>
                  <ci> RasActive </ci>
                </apply>
                <apply>
                  <plus/>
                  <ci> RasActive </ci>
                  <ci> KmRasGap </ci>
                </apply>
              </apply>
            </apply>
          </math>
        </kineticLaw>
      </reaction>
      <reaction id="Raf1ByRasActivationReaction" reversible="true" fast="false">
        <listOfReactants>
          <speciesReference species="Raf1Inactive" stoichiometry="1" constant="true"/>
        </listOfReactants>
        <listOfProducts>
          <speciesReference species="Raf1Active" stoichiometry="1" constant="true"/>
        </listOfProducts>
        <listOfModifiers>
          <modifierSpeciesReference species="RasActive"/>
        </listOfModifiers>
        <kineticLaw>
          <math xmlns="http://www.w3.org/1998/Math/MathML">
            <apply>
              <times/>
              <ci> cell </ci>
              <apply>
                <divide/>
                <apply>
                  <times/>
                  <ci> kRasToRaf1 </ci>
                  <ci> RasActive </ci>
                  <ci> Raf1Inactive </ci>
                </apply>
                <apply>
                  <plus/>
                  <ci> Raf1Inactive </ci>
                  <ci> KmRasToRaf1 </ci>
                </apply>
              </apply>
            </apply>
          </math>
        </kineticLaw>
      </reaction>
      <reaction id="MekbyRaf1ActivationReaction" reversible="true" fast="false">
        <listOfReactants>
          <speciesReference species="MekInactive" stoichiometry="1" constant="true"/>
        </listOfReactants>
        <listOfProducts>
          <speciesReference species="MekActive" stoichiometry="1" constant="true"/>
        </listOfProducts>
        <listOfModifiers>
          <modifierSpeciesReference species="Raf1Active"/>
        </listOfModifiers>
        <kineticLaw>
          <math xmlns="http://www.w3.org/1998/Math/MathML">
            <apply>
              <times/>
              <ci> cell </ci>
              <apply>
                <divide/>
                <apply>
                  <times/>
                  <ci> kpRaf1 </ci>
                  <ci> Raf1Active </ci>
                  <ci> MekInactive </ci>
                </apply>
                <apply>
                  <plus/>
                  <ci> MekInactive </ci>
                  <ci> KmpRaf1 </ci>
                </apply>
              </apply>
            </apply>
          </math>
        </kineticLaw>
      </reaction>
      <reaction id="MekbyBRafActivationReaction" reversible="true" fast="false">
        <listOfReactants>
          <speciesReference species="MekInactive" stoichiometry="1" constant="true"/>
        </listOfReactants>
        <listOfProducts>
          <speciesReference species="MekActive" stoichiometry="1" constant="true"/>
        </listOfProducts>
        <listOfModifiers>
          <modifierSpeciesReference species="BRafActive"/>
        </listOfModifiers>
        <kineticLaw>
          <math xmlns="http://www.w3.org/1998/Math/MathML">
            <apply>
              <times/>
              <ci> cell </ci>
              <apply>
                <divide/>
                <apply>
                  <times/>
                  <ci> kpBRaf </ci>
                  <ci> BRafActive </ci>
                  <ci> MekInactive </ci>
                </apply>
                <apply>
                  <plus/>
                  <ci> MekInactive </ci>
                  <ci> KmpBRaf </ci>
                </apply>
              </apply>
            </apply>
          </math>
        </kineticLaw>
      </reaction>
      <reaction id="ErkActivationReaction" reversible="true" fast="false">
        <listOfReactants>
          <speciesReference species="ErkInactive" stoichiometry="1" constant="true"/>
        </listOfReactants>
        <listOfProducts>
          <speciesReference species="ErkActive" stoichiometry="1" constant="true"/>
        </listOfProducts>
        <listOfModifiers>
          <modifierSpeciesReference species="MekActive"/>
        </listOfModifiers>
        <kineticLaw>
          <math xmlns="http://www.w3.org/1998/Math/MathML">
            <apply>
              <times/>
              <ci> cell </ci>
              <apply>
                <divide/>
                <apply>
                  <times/>
                  <ci> kpMekCytoplasmic </ci>
                  <ci> MekActive </ci>
                  <ci> ErkInactive </ci>
                </apply>
                <apply>
                  <plus/>
                  <ci> ErkInactive </ci>
                  <ci> KmpMekCytoplasmic </ci>
                </apply>
              </apply>
            </apply>
          </math>
        </kineticLaw>
      </reaction>
      <reaction id="MekDeactivationReaction" reversible="true" fast="false">
        <listOfReactants>
          <speciesReference species="MekActive" stoichiometry="1" constant="true"/>
        </listOfReactants>
        <listOfProducts>
          <speciesReference species="MekInactive" stoichiometry="1" constant="true"/>
        </listOfProducts>
        <listOfModifiers>
          <modifierSpeciesReference species="PP2AActive"/>
        </listOfModifiers>
        <kineticLaw>
          <math xmlns="http://www.w3.org/1998/Math/MathML">
            <apply>
              <times/>
              <ci> cell </ci>
              <apply>
                <divide/>
                <apply>
                  <times/>
                  <ci> kdMek </ci>
                  <ci> PP2AActive </ci>
                  <ci> MekActive </ci>
                </apply>
                <apply>
                  <plus/>
                  <ci> MekActive </ci>
                  <ci> KmdMek </ci>
                </apply>
              </apply>
            </apply>
          </math>
        </kineticLaw>
      </reaction>
      <reaction id="ErkDeactivationReaction" reversible="true" fast="false">
        <listOfReactants>
          <speciesReference species="ErkActive" stoichiometry="1" constant="true"/>
        </listOfReactants>
        <listOfProducts>
          <speciesReference species="ErkInactive" stoichiometry="1" constant="true"/>
        </listOfProducts>
        <listOfModifiers>
          <modifierSpeciesReference species="PP2AActive"/>
        </listOfModifiers>
        <kineticLaw>
          <math xmlns="http://www.w3.org/1998/Math/MathML">
            <apply>
              <times/>
              <ci> cell </ci>
              <apply>
                <divide/>
                <apply>
                  <times/>
                  <ci> kdErk </ci>
                  <ci> PP2AActive </ci>
                  <ci> ErkActive </ci>
                </apply>
                <apply>
                  <plus/>
                  <ci> ErkActive </ci>
                  <ci> KmdErk </ci>
                </apply>
              </apply>
            </apply>
          </math>
        </kineticLaw>
      </reaction>
      <reaction id="Raf1byPPtaseDeactivationReaction" reversible="true" fast="false">
        <listOfReactants>
          <speciesReference species="Raf1Active" stoichiometry="1" constant="true"/>
        </listOfReactants>
        <listOfProducts>
          <speciesReference species="Raf1Inactive" stoichiometry="1" constant="true"/>
        </listOfProducts>
        <listOfModifiers>
          <modifierSpeciesReference species="Raf1PPtase"/>
        </listOfModifiers>
        <kineticLaw>
          <math xmlns="http://www.w3.org/1998/Math/MathML">
            <apply>
              <times/>
              <ci> cell </ci>
              <apply>
                <divide/>
                <apply>
                  <times/>
                  <ci> kdRaf1 </ci>
                  <ci> Raf1PPtase </ci>
                  <ci> Raf1Active </ci>
                </apply>
                <apply>
                  <plus/>
                  <ci> Raf1Active </ci>
                  <ci> KmdRaf1 </ci>
                </apply>
              </apply>
            </apply>
          </math>
        </kineticLaw>
      </reaction>
      <reaction id="P90RskActivationReaction" reversible="true" fast="false">
        <listOfReactants>
          <speciesReference species="P90RskInactive" stoichiometry="1" constant="true"/>
        </listOfReactants>
        <listOfProducts>
          <speciesReference species="P90RskActive" stoichiometry="1" constant="true"/>
        </listOfProducts>
        <listOfModifiers>
          <modifierSpeciesReference species="ErkActive"/>
        </listOfModifiers>
        <kineticLaw>
          <math xmlns="http://www.w3.org/1998/Math/MathML">
            <apply>
              <times/>
              <ci> cell </ci>
              <apply>
                <divide/>
                <apply>
                  <times/>
                  <ci> kpP90Rsk </ci>
                  <ci> ErkActive </ci>
                  <ci> P90RskInactive </ci>
                </apply>
                <apply>
                  <plus/>
                  <ci> P90RskInactive </ci>
                  <ci> KmpP90Rsk </ci>
                </apply>
              </apply>
            </apply>
          </math>
        </kineticLaw>
      </reaction>
      <reaction id="PI3KbyEGFRActivationReaction" reversible="true" fast="false">
        <listOfReactants>
          <speciesReference species="PI3KInactive" stoichiometry="1" constant="true"/>
        </listOfReactants>
        <listOfProducts>
          <speciesReference species="PI3KActive" stoichiometry="1" constant="true"/>
        </listOfProducts>
        <listOfModifiers>
          <modifierSpeciesReference species="boundEGFReceptor"/>
        </listOfModifiers>
        <kineticLaw>
          <math xmlns="http://www.w3.org/1998/Math/MathML">
            <apply>
              <times/>
              <ci> cell </ci>
              <apply>
                <divide/>
                <apply>
                  <times/>
                  <ci> kPI3K </ci>
                  <ci> boundEGFReceptor </ci>
                  <ci> PI3KInactive </ci>
                </apply>
                <apply>
                  <plus/>
                  <ci> PI3KInactive </ci>
                  <ci> KmPI3K </ci>
                </apply>
              </apply>
            </apply>
          </math>
        </kineticLaw>
      </reaction>
      <reaction id="PI3KbyRasActivationReaction" reversible="true" fast="false">
        <listOfReactants>
          <speciesReference species="PI3KInactive" stoichiometry="1" constant="true"/>
        </listOfReactants>
        <listOfProducts>
          <speciesReference species="PI3KActive" stoichiometry="1" constant="true"/>
        </listOfProducts>
        <listOfModifiers>
          <modifierSpeciesReference species="RasActive"/>
        </listOfModifiers>
        <kineticLaw>
          <math xmlns="http://www.w3.org/1998/Math/MathML">
            <apply>
              <times/>
              <ci> cell </ci>
              <apply>
                <divide/>
                <apply>
                  <times/>
                  <ci> kPI3KRas </ci>
                  <ci> RasActive </ci>
                  <ci> PI3KInactive </ci>
                </apply>
                <apply>
                  <plus/>
                  <ci> PI3KInactive </ci>
                  <ci> KmPI3KRas </ci>
                </apply>
              </apply>
            </apply>
          </math>
        </kineticLaw>
      </reaction>
      <reaction id="AktActivationReaction" reversible="true" fast="false">
        <listOfReactants>
          <speciesReference species="AktInactive" stoichiometry="1" constant="true"/>
        </listOfReactants>
        <listOfProducts>
          <speciesReference species="AktActive" stoichiometry="1" constant="true"/>
        </listOfProducts>
        <listOfModifiers>
          <modifierSpeciesReference species="PI3KActive"/>
        </listOfModifiers>
        <kineticLaw>
          <math xmlns="http://www.w3.org/1998/Math/MathML">
            <apply>
              <times/>
              <ci> cell </ci>
              <apply>
                <divide/>
                <apply>
                  <times/>
                  <ci> kAkt </ci>
                  <ci> PI3KActive </ci>
                  <ci> AktInactive </ci>
                </apply>
                <apply>
                  <plus/>
                  <ci> AktInactive </ci>
                  <ci> KmAkt </ci>
                </apply>
              </apply>
            </apply>
          </math>
        </kineticLaw>
      </reaction>
      <reaction id="Raf1ByAktDeactivationReaction" reversible="true" fast="false">
        <listOfReactants>
          <speciesReference species="Raf1Active" stoichiometry="1" constant="true"/>
        </listOfReactants>
        <listOfProducts>
          <speciesReference species="Raf1Inactive" stoichiometry="1" constant="true"/>
        </listOfProducts>
        <listOfModifiers>
          <modifierSpeciesReference species="AktActive"/>
        </listOfModifiers>
        <kineticLaw>
          <math xmlns="http://www.w3.org/1998/Math/MathML">
            <apply>
              <times/>
              <ci> cell </ci>
              <apply>
                <divide/>
                <apply>
                  <times/>
                  <ci> kdRaf1ByAkt </ci>
                  <ci> AktActive </ci>
                  <ci> Raf1Active </ci>
                </apply>
                <apply>
                  <plus/>
                  <ci> Raf1Active </ci>
                  <ci> KmRaf1ByAkt </ci>
                </apply>
              </apply>
            </apply>
          </math>
        </kineticLaw>
      </reaction>
      <reaction id="C3GActivationReaction" reversible="true" fast="false">
        <listOfReactants>
          <speciesReference species="C3GInactive" stoichiometry="1" constant="true"/>
        </listOfReactants>
        <listOfProducts>
          <speciesReference species="C3GActive" stoichiometry="1" constant="true"/>
        </listOfProducts>
        <listOfModifiers>
          <modifierSpeciesReference species="boundNGFReceptor"/>
        </listOfModifiers>
        <kineticLaw>
          <math xmlns="http://www.w3.org/1998/Math/MathML">
            <apply>
              <times/>
              <ci> cell </ci>
              <apply>
                <divide/>
                <apply>
                  <times/>
                  <ci> kC3GNGF </ci>
                  <ci> boundNGFReceptor </ci>
                  <ci> C3GInactive </ci>
                </apply>
                <apply>
                  <plus/>
                  <ci> C3GInactive </ci>
                  <ci> KmC3GNGF </ci>
                </apply>
              </apply>
            </apply>
          </math>
        </kineticLaw>
      </reaction>
      <reaction id="Rap1ActivationReaction" reversible="true" fast="false">
        <listOfReactants>
          <speciesReference species="Rap1Inactive" stoichiometry="1" constant="true"/>
        </listOfReactants>
        <listOfProducts>
          <speciesReference species="Rap1Active" stoichiometry="1" constant="true"/>
        </listOfProducts>
        <listOfModifiers>
          <modifierSpeciesReference species="C3GActive"/>
        </listOfModifiers>
        <kineticLaw>
          <math xmlns="http://www.w3.org/1998/Math/MathML">
            <apply>
              <times/>
              <ci> cell </ci>
              <apply>
                <divide/>
                <apply>
                  <times/>
                  <ci> kC3G </ci>
                  <ci> C3GActive </ci>
                  <ci> Rap1Inactive </ci>
                </apply>
                <apply>
                  <plus/>
                  <ci> Rap1Inactive </ci>
                  <ci> KmC3G </ci>
                </apply>
              </apply>
            </apply>
          </math>
        </kineticLaw>
      </reaction>
      <reaction id="Rap1DeactivationReaction" reversible="true" fast="false">
        <listOfReactants>
          <speciesReference species="Rap1Active" stoichiometry="1" constant="true"/>
        </listOfReactants>
        <listOfProducts>
          <speciesReference species="Rap1Inactive" stoichiometry="1" constant="true"/>
        </listOfProducts>
        <listOfModifiers>
          <modifierSpeciesReference species="RapGapActive"/>
        </listOfModifiers>
        <kineticLaw>
          <math xmlns="http://www.w3.org/1998/Math/MathML">
            <apply>
              <times/>
              <ci> cell </ci>
              <apply>
                <divide/>
                <apply>
                  <times/>
                  <ci> kRapGap </ci>
                  <ci> RapGapActive </ci>
                  <ci> Rap1Active </ci>
                </apply>
                <apply>
                  <plus/>
                  <ci> Rap1Active </ci>
                  <ci> KmRapGap </ci>
                </apply>
              </apply>
            </apply>
          </math>
        </kineticLaw>
      </reaction>
      <reaction id="BRafByRap1ActivationReaction" reversible="true" fast="false">
        <listOfReactants>
          <speciesReference species="BRafInactive" stoichiometry="1" constant="true"/>
        </listOfReactants>
        <listOfProducts>
          <speciesReference species="BRafActive" stoichiometry="1" constant="true"/>
        </listOfProducts>
        <listOfModifiers>
          <modifierSpeciesReference species="Rap1Active"/>
        </listOfModifiers>
        <kineticLaw>
          <math xmlns="http://www.w3.org/1998/Math/MathML">
            <apply>
              <times/>
              <ci> cell </ci>
              <apply>
                <divide/>
                <apply>
                  <times/>
                  <ci> kRap1ToBRaf </ci>
                  <ci> Rap1Active </ci>
                  <ci> BRafInactive </ci>
                </apply>
                <apply>
                  <plus/>
                  <ci> BRafInactive </ci>
                  <ci> KmRap1ToBRaf </ci>
                </apply>
              </apply>
            </apply>
          </math>
        </kineticLaw>
      </reaction>
      <reaction id="BRafbyPPtaseDeactivationReaction" reversible="true" fast="false">
        <listOfReactants>
          <speciesReference species="BRafActive" stoichiometry="1" constant="true"/>
        </listOfReactants>
        <listOfProducts>
          <speciesReference species="BRafInactive" stoichiometry="1" constant="true"/>
        </listOfProducts>
        <listOfModifiers>
          <modifierSpeciesReference species="Raf1PPtase"/>
        </listOfModifiers>
        <kineticLaw>
          <math xmlns="http://www.w3.org/1998/Math/MathML">
            <apply>
              <times/>
              <ci> cell </ci>
              <apply>
                <divide/>
                <apply>
                  <times/>
                  <ci> kdBRaf </ci>
                  <ci> Raf1PPtase </ci>
                  <ci> BRafActive </ci>
                </apply>
                <apply>
                  <plus/>
                  <ci> BRafActive </ci>
                  <ci> KmdBRaf </ci>
                </apply>
              </apply>
            </apply>
          </math>
        </kineticLaw>
      </reaction>
    </listOfReactions>
  </model>
</sbml>"""


ant = """// Created by libAntimony v2.12.0.3
model *Brown2004_NGF_EGF_signaling()
  // Compartments and Species:
  compartment cell;
  species EGF in cell, NGF in cell, freeEGFReceptor in cell, boundEGFReceptor in cell;
  species freeNGFReceptor in cell, boundNGFReceptor in cell, SosInactive in cell;
  species SosActive in cell, P90RskInactive in cell, P90RskActive in cell;
  species RasInactive in cell, RasActive in cell, $RasGapActive in cell, Raf1Inactive in cell;
  species Raf1Active in cell, BRafInactive in cell, BRafActive in cell, MekInactive in cell;
  species MekActive in cell, ErkInactive in cell, ErkActive in cell, PI3KInactive in cell;
  species PI3KActive in cell, AktInactive in cell, AktActive in cell, C3GInactive in cell;
  species C3GActive in cell, Rap1Inactive in cell, Rap1Active in cell, $RapGapActive in cell;
  species $PP2AActive in cell, $Raf1PPtase in cell;
  // Reactions:
  EGFBindingReaction: EGF + freeEGFReceptor => boundEGFReceptor; cell*krbEGF*EGF*freeEGFReceptor;
  EGFUnbindingReaction: boundEGFReceptor => EGF + freeEGFReceptor; cell*kruEGF*boundEGFReceptor;
  NGFBindingReaction: NGF + freeNGFReceptor => boundNGFReceptor; krbNGF*NGF*freeNGFReceptor*cell;
  NGFUnbindingReaction: boundNGFReceptor => NGF + freeNGFReceptor; kruNGF*boundNGFReceptor*cell;
  SosActivationByEGFReaction: SosInactive -> SosActive; cell*(kEGF*boundEGFReceptor*SosInactive/(SosInactive + KmEGF));
  SosActivationByNGFReaction: SosInactive -> SosActive; cell*(kNGF*boundNGFReceptor*SosInactive/(SosInactive + KmNGF));
  SosDeactivationReaction: SosActive -> SosInactive; cell*(kdSos*P90RskActive*SosActive/(SosActive + KmdSos));
  RasActivationReaction: RasInactive -> RasActive; cell*(kSos*SosActive*RasInactive/(RasInactive + KmSos));
  RasDeactivationReaction: RasActive -> RasInactive; cell*(kRasGap*RasGapActive*RasActive/(RasActive + KmRasGap));
  Raf1ByRasActivationReaction: Raf1Inactive -> Raf1Active; cell*(kRasToRaf1*RasActive*Raf1Inactive/(Raf1Inactive + KmRasToRaf1));
  MekbyRaf1ActivationReaction: MekInactive -> MekActive; cell*(kpRaf1*Raf1Active*MekInactive/(MekInactive + KmpRaf1));
  MekbyBRafActivationReaction: MekInactive -> MekActive; cell*(kpBRaf*BRafActive*MekInactive/(MekInactive + KmpBRaf));
  ErkActivationReaction: ErkInactive -> ErkActive; cell*(kpMekCytoplasmic*MekActive*ErkInactive/(ErkInactive + KmpMekCytoplasmic));
  MekDeactivationReaction: MekActive -> MekInactive; cell*(kdMek*PP2AActive*MekActive/(MekActive + KmdMek));
  ErkDeactivationReaction: ErkActive -> ErkInactive; cell*(kdErk*PP2AActive*ErkActive/(ErkActive + KmdErk));
  Raf1byPPtaseDeactivationReaction: Raf1Active -> Raf1Inactive; cell*(kdRaf1*Raf1PPtase*Raf1Active/(Raf1Active + KmdRaf1));
  P90RskActivationReaction: P90RskInactive -> P90RskActive; cell*(kpP90Rsk*ErkActive*P90RskInactive/(P90RskInactive + KmpP90Rsk));
  PI3KbyEGFRActivationReaction: PI3KInactive -> PI3KActive; cell*(kPI3K*boundEGFReceptor*PI3KInactive/(PI3KInactive + KmPI3K));
  PI3KbyRasActivationReaction: PI3KInactive -> PI3KActive; cell*(kPI3KRas*RasActive*PI3KInactive/(PI3KInactive + KmPI3KRas));
  AktActivationReaction: AktInactive -> AktActive; cell*(kAkt*PI3KActive*AktInactive/(AktInactive + KmAkt));
  Raf1ByAktDeactivationReaction: Raf1Active -> Raf1Inactive; cell*(kdRaf1ByAkt*AktActive*Raf1Active/(Raf1Active + KmRaf1ByAkt));
  C3GActivationReaction: C3GInactive -> C3GActive; cell*(kC3GNGF*boundNGFReceptor*C3GInactive/(C3GInactive + KmC3GNGF));
  Rap1ActivationReaction: Rap1Inactive -> Rap1Active; cell*(kC3G*C3GActive*Rap1Inactive/(Rap1Inactive + KmC3G));
  Rap1DeactivationReaction: Rap1Active -> Rap1Inactive; cell*(kRapGap*RapGapActive*Rap1Active/(Rap1Active + KmRapGap));
  BRafByRap1ActivationReaction: BRafInactive -> BRafActive; cell*(kRap1ToBRaf*Rap1Active*BRafInactive/(BRafInactive + KmRap1ToBRaf));
  BRafbyPPtaseDeactivationReaction: BRafActive -> BRafInactive; cell*(kdBRaf*Raf1PPtase*BRafActive/(BRafActive + KmdBRaf));
  // Species initializations:
  EGF = 10002000;
  NGF = 456000;
  freeEGFReceptor = 80000;
  boundEGFReceptor = 0;
  freeNGFReceptor = 10000;
  boundNGFReceptor = 0;
  SosInactive = 120000;
  SosActive = 0;
  P90RskInactive = 120000;
  P90RskActive = 0;
  RasInactive = 120000;
  RasActive = 0;
  RasGapActive = 120000;
  Raf1Inactive = 120000;
  Raf1Active = 0;
  BRafInactive = 120000;
  BRafActive = 0;
  MekInactive = 600000;
  MekActive = 0;
  ErkInactive = 600000;
  ErkActive = 0;
  PI3KInactive = 120000;
  PI3KActive = 0;
  AktInactive = 120000;
  AktActive = 0;
  C3GInactive = 120000;
  C3GActive = 0;
  Rap1Inactive = 120000;
  Rap1Active = 0;
  RapGapActive = 120000;
  PP2AActive = 120000;
  Raf1PPtase = 120000;
  // Compartment initializations:
  cell = 1;
  // Variable initializations:
  krbEGF = 2.18503e-05;
  kruEGF = 0.0121008;
  krbNGF = 1.38209e-07;
  kruNGF = 0.00723811;
  kEGF = 694.731;
  KmEGF = 6086070;
  kNGF = 389.428;
  KmNGF = 2112.66;
  kdSos = 1611.97;
  KmdSos = 896896;
  kSos = 32.344;
  KmSos = 35954.3;
  kRasGap = 1509.36;
  KmRasGap = 1432410;
  kRasToRaf1 = 0.884096;
  KmRasToRaf1 = 62464.6;
  kpRaf1 = 185.759;
  KmpRaf1 = 4768350;
  kpBRaf = 125.089;
  KmpBRaf = 157948;
  kpMekCytoplasmic = 9.85367;
  KmpMekCytoplasmic = 1007340;
  kdMek = 2.83243;
  KmdMek = 518753;
  kdErk = 8.8912;
  KmdErk = 3496490;
  kdRaf1 = 0.126329;
  KmdRaf1 = 1061.71;
  kpP90Rsk = 0.0213697;
  KmpP90Rsk = 763523;
  kPI3K = 10.6737;
  KmPI3K = 184912;
  kPI3KRas = 0.0771067;
  KmPI3KRas = 272056;
  kAkt = 0.0566279;
  KmAkt = 653951;
  kdRaf1ByAkt = 15.1212;
  KmRaf1ByAkt = 119355;
  kC3GNGF = 146.912;
  KmC3GNGF = 12876.2;
  kC3G = 1.40145;
  KmC3G = 10965.6;
  kRapGap = 27.265;
  KmRapGap = 295990;
  kRap1ToBRaf = 2.20995;
  KmRap1ToBRaf = 1025460;
  kdBRaf = 441.287;
  KmdBRaf = 10879500;
  // Other declarations:
  const cell, krbEGF, kruEGF, krbNGF, kruNGF, kEGF, KmEGF, kNGF, KmNGF, kdSos;
  const KmdSos, kSos, KmSos, kRasGap, KmRasGap, kRasToRaf1, KmRasToRaf1, kpRaf1;
  const KmpRaf1, kpBRaf, KmpBRaf, kpMekCytoplasmic, KmpMekCytoplasmic, kdMek;
  const KmdMek, kdErk, KmdErk, kdRaf1, KmdRaf1, kpP90Rsk, KmpP90Rsk, kPI3K;
  const KmPI3K, kPI3KRas, KmPI3KRas, kAkt, KmAkt, kdRaf1ByAkt, KmRaf1ByAkt;
  const kC3GNGF, KmC3GNGF, kC3G, KmC3G, kRapGap, KmRapGap, kRap1ToBRaf, KmRap1ToBRaf;
  const kdBRaf, KmdBRaf;
end
"""

# reference values, computed by copasi
# key: speciesName
# value: (starting value, steady state value)
brown2004_ss = {
    "EGF": (10002000, 9922004.46501588),
    "NGF": (456000, 447048.6328396933),
    "freeEGFReceptor": (80000, 4.465015881563886),
    "boundEGFReceptor": (0, 79995.53498411844),
    "freeNGFReceptor": (10000, 1048.632839693278),
    "boundNGFReceptor": (0, 8951.367160306723),
    "SosInactive": (120000, 99569.9548813399),
    "SosActive": (0, 20430.0451186601),
    "P90RskInactive": (120000, 0),
    "P90RskActive": (0, 120000),
    "RasInactive": (120000, 115999.51997363668),
    "RasActive": (0, 4000.4800263633224),
    "Raf1Inactive": (0, 119918.26132477315),
    "Raf1Active": (0, 81.73867522685032),
    "BRafInactive": (120000, 119315.8938154474),
    "BRafActive": (0, 684.1061845525983),
    "MekInactive": (60000, 475871.3129791245),
    "MekActive": (0, 124128.68702087551),
    "ErkInactive": (60000, 118834.97021061042),
    "ErkActive": (0, 481165.02978938946),
    "PI3KInactive": (120000, 0),
    "PI3KActive": (0, 119999.99999999997),
    "AktInactive": (0, 0),
    "AktActive": (0, 120000),
    "C3GInactive": (120000, 0),
    "C3GActive": (0, 120000),
    "Rap1Inactive": (120000, 105544.62439152892),
    "Rap1Active": (0, 14455.375608471077)
}



r = te.loads(sbml)
r.conservedMoietyAnalysis = True
solver = r.getSteadyStateSolver()

solver.setValue("allow_presimulation", True)
solver.setValue("presimulation_time", 10000)


print([i for i in dir(r) if "ant" in i.lower()])



ans = r.getFloatingSpeciesConcentrationsNamedArray()
print(ans)

#
# """
#     SosInactive, Raf1Inactive, MekInactive, boundEGFReceptor, freeNGFReceptor, RasInactive, BRafInactive, ErkInactive, PI3KInactive, Rap1Inactive, P90RskInactive,  AktInactive, C3GInactive, BRafActive, ErkActive, PI3KActive, Rap1Active, MekActive, SosActive, boundNGFReceptor, RasActive, Raf1Active,    NGF, AktActive,     EGF, C3GActive, P90RskActive, freeEGFReceptor
#     [[       99570,       119918,      475871,          79995.5,         1048.63,    116000,       119316,      118835,  7.70372e-34,       105545,   -4.91112e-33, -8.99301e-29, 6.66267e-40,    684.106,    481165,     120000,    14455.4,124129,     20430,          8951.37,   4000.48,    81.7387, 447049,    120000, 9.922e+06,    120000,       120000,         4.46502]]
#
# """
#
# for name in ans.colnames:
#     expected = brown2004_ss[name][1]
#     actual = ans[name][0]
#     print(f"comparing {name}, expected = {expected}, actual: {actual}")
#






























