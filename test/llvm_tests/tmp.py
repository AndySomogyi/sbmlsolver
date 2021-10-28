from roadrunner import RoadRunner, Config

# Switch to using a different back end.
Config.setValue(Config.LLVM_COMPILER, Config.MCJIT)
Config.setValue(Config.LLVM_COMPILER, Config.LLJIT)
Config.setValue(Config.LLVM_COMPILER, Config.LAZY_JIT)
Config.setValue(Config.LLVM_COMPILER, Config.MAY_MODELS)
Config.setValue(Config.LLVM_COMPILER, Config.NVIDIA_GPU_JIT)

# recompile an existing roadrunner instance
# using a different JIT compiler.
r = RoadRunner(sbml)
r.setCompiler("MCJit")
r.setCompiler("LLJit")
r.setCompiler("LazyJit")

