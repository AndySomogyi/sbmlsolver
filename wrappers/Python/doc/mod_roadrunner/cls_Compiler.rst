JIT Compilation
_______________


.. class:: Compiler()
   :module: roadrunner

   The Compiler object provides information about the JIT compiler
   currently in use. 

.. method:: Compiler.getCompiler()
   :module: roadrunner

   gets the name of the JIT compiler, i.e. "LLVM" means we are using the
   LLVM JIT compiler.

   :rtype: str


.. method:: Compiler.getVersion()
   :module: roadrunner:

   get the version of the JIT compiler.

   :rtype: str


.. method:: Compiler.getDefaultTargetTriple()
   :module: roadrunner

   Return the default target triple the compiler  has been configured to produce code for.
   A 'triple' is just a string that contains three items, it is called 'triple' as 
   that is a LLVM historical convention.

   The target triple is a string in the format of:
   CPU_TYPE-VENDOR-OPERATING_SYSTEM
   
   or
   
   CPU_TYPE-VENDOR-KERNEL-OPERATING_SYSTEM

   :rtype: str
   
  
.. method:: Compiler.getProcessTriple()
   :module: roadrunner
            
   Return an appropriate target triple for generating
   code to be loaded into the current process, e.g. when using the JIT.

   :rtype: str


.. method:: Compiler.getHostCPUName()
   :module: roadrunner

   getHostCPUName - Get the LLVM name for the host CPU. The particular format
   of the name is target dependent, and suitable for passing as -mcpu to the
   target which matches the host.
     
   return - The host CPU name, or empty if the CPU could not be determined.

   :rtype: str
     
.. method:: Compiler.setCompiler(compiler)
   :module: roadrunner

   A legacy method that currently does not do anything.

.. method:: Compiler.getCompilerLocation()
   :module: roadrunner

   A legacy method that currently does not do anything.

.. method:: Compiler.setCompilerLocation(loc)
   :module: roadrunner

   A legacy method that currently does not do anything.

.. method:: Compiler.getSupportCodeFolder()
   :module: roadrunner

   A legacy method that currently does not do anything.

.. method:: Compiler.setSupportCodeFolder(path)
   :module: roadrunner

   A legacy method that currently does not do anything.
