/*
 * rrModelGeneratorFactory.h
 *
 *  Created on: May 20, 2013
 *      Author: andy
 */

#ifndef RRMODELGENERATORFACTORY_H_
#define RRMODELGENERATORFACTORY_H_

#include <string>

namespace rr {

class ModelGenerator;

/**
 * A factory class to create a ModelGenerator.
 *
 * Currently we have two concrete Model implmentations, an LLVM JIT compiled
 * model and the older C file generating shared library model.
 */
class ModelGeneratorFactory {
public:
    /**
     * Create a model generator.
     * Eventauly we will have another static method that returns the names of the available
     * model generators and their descriptions. Currently we only support CModelGenerator, although
     * this is ignored and this will always return a CModelGenerator.
     *
     * The created object is owned by the caller and must be deleted accordingly.
     *
     * @param compiler: the textual name of the compiler. If this is "llvm", then then
     * the LLVM ModelGenerator is created, otherwise, A C based model generator is
     * created.
     *
     * @param tmpDir The C version stores all the generated temporary
     * C files and objects here, not used for LLVM.
     *
     * @param supportCodeDir Location where roadrunner include files are,
     * not used in LLVM.
     */
    static ModelGenerator* createModelGenerator(const std::string& compiler,
            const std::string& tmpDir="", const std::string& supportCodeDir="");
};

} /* namespace rr */
#endif /* RRMODELGENERATORFACTORY_H_ */
