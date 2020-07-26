macro(BuildLLVMFromSource)
        set(LLVM_SOURCE_URL "https://github.com/CiaranWelsh/LLVM6.0.1/archive/v0.0.1.zip")
        set(LLVM_SOURCE_ZIP "${LLVM_ROOT}/llvm6.0.1-source.zip")
        set(LLVM_LIBRARIES
               "LLVMAnalysis"
               "LLVMAsmPrinter"
               "LLVMBinaryFormat"
               "LLVMBitReader"
               "LLVMBitWriter"
               "LLVMCodeGen"
               "LLVMCore"
               "LLVMDebugInfoCodeView"
               "LLVMDebugInfoMSF"
               "LLVMDemangle"
               "LLVMExecutionEngine"
               "LLVMGlobalISel"
               "LLVMInstCombine"
               "LLVMMC"
               "LLVMMCDisassembler"
               "LLVMMCJIT"
               "LLVMMCParser"
               "LLVMObject"
               "LLVMProfileData"
               "LLVMRuntimeDyld"
               "LLVMScalarOpts"
               "LLVMSelectionDAG"
               "LLVMSupport"
               "LLVMTarget"
               "LLVMTransformUtils"
               "LLVMX86AsmParser"
               "LLVMX86AsmPrinter"
               "LLVMX86CodeGen"
               "LLVMX86Desc"
               "LLVMX86Info"
               "LLVMX86Utils"
                )


        set(LLVM_SOURCE_DIR "${LLVM_ROOT}/llvm6.0.1-source")
        set(LLVM_BINARY_DIR "${LLVM_ROOT}/llvm6.0.1-source")
        ExternalProject_Add(LLVM
                PREFIX ${LLVM_SOURCE_DIR}
                INSTALL_DIR ${LLVM_INSTALL_DIR}
                URL ${LLVM_SOURCE_URL}
                CMAKE_ARGS
                -DCMAKE_INSTALL_PREFIX=${LLVM_INSTALL_DIR}
                --target ${LLVM_LIBRARIES}
                )
endmacro()