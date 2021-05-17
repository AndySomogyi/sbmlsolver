//
// Created by Ciaran on 17/05/2021.
//

#ifndef ROADRUNNER_LAYOUTONLY_H
#define ROADRUNNER_LAYOUTONLY_H

#include "TestModelInterface.h"

/**
 * This SBML model does not have any rate laws not parameters.
 * It contains layout information only, which is valid sbml.
 * This model comes from issue-756
 *  https://github.com/sys-bio/roadrunner/issues/756
 */
class LayoutOnly : public TestModel {
public:

    std::string str() override;

    std::string modelName() override;
};


#endif //ROADRUNNER_LAYOUTONLY_H
