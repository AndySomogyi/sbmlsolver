SBMLSolver installation instructions
====================================


1. Check to see if zlib1g is installed. If it is not installed you will need to install it by

   ```
   sudo apt-get install zlib1g-dev
   ```

2. Check to see if libncurses is installed. If it is not installed you will need to install it by

   ```
   sudo apt-get install libncurses-dev
   ```

3. First build the SBMLSolver dependencies, these are in a separate git repo. You
   will install this into its own directory, reccomended to install to $HOME/local

   <https://github.com/AndySomogyi/sbmlsolver_dep>


4. grab this repo from git, assuming you'll call it '$HOME/src/sbmlsolver'.

5. Create a build directory, ```$HOME/src/sbmlsolver_build```, go to this dir and
   run ccmake,

   ```
   ccmake ../sbmlsolver
   ```

   If you're using an IDE, you can use CMake to generate proejct files like:

   ```
   ccmake -G"Eclipse CDT4 - Unix Makefiles" ../sbmlsolver
   ```


   Or XCode project files like

   ```
   ccmake -G"Xcode" ../sbmlsolver
   ```

 6. In the ccmake gui, select ```$HOME/local``` (or wherever you installed the dependencies to)
    as the value for the ``` SBMLSOLVER_DEP_DIR``` build option.

    Hit 'c' a few times to configure, and 'g' to generate the project file.

    Now just build

    ```
    make
    ```