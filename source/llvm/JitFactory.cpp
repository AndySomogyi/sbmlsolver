//
// Created by Ciaran on 15/11/2021.
//

#include "JitFactory.h"

namespace rrllvm {

    std::unique_ptr<Jit> JitFactory::makeJitEngine(std::uint32_t opt) {
        std::unique_ptr<Jit> jit;
        if (opt & LoadSBMLOptions::MCJIT) {
            jit = std::move(std::make_unique<MCJit>(opt));
        }

        else if (opt & LoadSBMLOptions::LLJIT) {
            jit = std::move(std::make_unique<LLJit>(opt));
        }

        return std::move(jit);
    }

    std::unique_ptr<Jit> JitFactory::makeJitEngine() {
        LoadSBMLOptions opt;
        std::unique_ptr<Jit> j = JitFactory::makeJitEngine(opt.modelGeneratorOpt);
        return std::move(j);
    }
}
