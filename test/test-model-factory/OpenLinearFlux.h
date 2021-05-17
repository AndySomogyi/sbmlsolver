//
// Created by Ciaran on 17/05/2021.
//

#ifndef ROADRUNNER_OPENLINEARFLUX_H
#define ROADRUNNER_OPENLINEARFLUX_H

#include "TestModelInterface.h"


/**
 * In [33]:  def ss(s):
    ...:      m = te.loada(s)
    ...:      m.conservedMoietyAnalysis = False
    ...:      m.steadyState()
    ...:      print(m.getFloatingSpeciesConcentrations())
    ...:      print(m.getFloatingSpeciesConcentrationIds())
    ...:      print(m.getFullJacobian())
    ...:      return m
    ...:
    ...:  r = ss("""
    ...:  model x
    ...:      S1 = 0;
    ...:      S2 = 0;
    ...:      => S1; kin
    ...:      S1 => S2; kf*S1;
    ...:      S2 => ; S2*kout;
    ...:      kf = 0.1;
    ...:      kb = 0.01;
    ...:      kin = 1;
    ...:      kout = 0.1
    ...:  end
    ...:  """)
 */
class OpenLinearFlux :
        public TestModel,
        public SteadyStateResult,
        public TimeSeriesResult,
        public JacobianResult {
public:
    std::string str() override;

    StringDoubleMap steadyState() override;

    ls::DoubleMatrix timeSeriesResult() override;

    std::string modelName() override;

    std::unordered_map<std::string, rr::Setting> steadyStateSettings() override;

    std::unordered_map<std::string, rr::Setting> timeSeriesSettings() override;

    ls::DoubleMatrix fullJacobianAmt() override;

    ls::DoubleMatrix fullJacobianConc() override;

    ls::DoubleMatrix reducedJacobianAmt() override;

    ls::DoubleMatrix reducedJacobianConc() override;

    std::unordered_map<std::string, rr::Setting> jacobianSettings() override;

};



#endif //ROADRUNNER_OPENLINEARFLUX_H
