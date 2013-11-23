---
layout: page
title: "Build Roarunner from Source"
description: "howto build the library"
---

## Introduction
The RoadRunner library is written in C/C++. It has currently been compiled successfully on Windows using Visual Studio and Borland compilers, and on Linux and Mac OS X, using the gcc compiler.
Bindary distributions are available for Windows and Mac.

Please follow the instructions in order to successfully compile and link the RoadRunner library on your platform.

Choose the instructions for your platform:

* [VisualStudio 2010](buildrr_visualstudio.html) on Windows
* [Linux](buildrr_linux.html)

For each individual compiler, however, some steps, or variable in the procedure may need a special setting or a tweak. Such tweaks are found on a separate page, associated to the compiler in question. That said, before executing the steps outlined below, please take a look at the specific compiler page for any additional steps needed in order to produce a proper end target.

## The Source
The source code for RoadRunner is managed in a Git repository located on [GitHub](https://github.com/AndySomogyi/roadrunner). The build instructions that follows below will assume that you have cloned the code from the default branch, which has the address:

`git clone https://github.com/AndySomogyi/roadrunner.git`

Any standard git client can be used to clone the code repository. If you would like to contribute, please first [fork](https://help.github.com/articles/fork-a-repo) the code on GitHub and submit pull requests.

### The ThirdParty libraries
RoadRunner depends on several ThirdParty libraries. 

`git clone https://github.com/AndySomogyi/roadrunner_thirdparty`

On Windows, all such necessary dependencies are satisfied by the process described for Visual Studio, while on Linux, various ThirdParties may need to be installed separately. This will be noted in the build instructions as needed.

## Build process overview
The final 'product' of building the RoadRunner source is binary libraries and applications. In short, the libraries will be

 - ThirdParty libraries - various C and C++ libs
 - RoadRunner C++ library
 - RoadRunner SWIG Python-API library (Bindings for Python)
 - (Optionally) RoadRunner C-API library (Wrapper for the C++ library)

Useful applications that can be built are:

- RoadRunner command line application: rr
- RoadRunner command line application: rr_c (application using the C-API of roadrunner)

In short, the whole build process will require the following steps

1. Code is cloned from the repository.
2. The CMake GUI is used to create Build Files for your particular compiler. At least two sets of build files are required to completely build Roadrunner, A) ThirdParty libs build files, and B) build files for the RoadRunner library and RoadRunner applications.
3. The ThirdParty libraries are built and installed using a compiler, by the generated build files from step 2.
4. RoadRunner library, and associated applications are built.

## Get started on building and compiling
The instructions linked at the top of this page will go through the process of setting up a development environment using the Visual Studio compiler. Specific tweaks for any compiler can found be on the page associated with the relevant compiler. See below for links to specific compilers.


##Notes on Generating or updating the API Documentation#
The documentation is automatically generated using the comments in the copasi_api.h file. To change the documentation, you just need to edit that file. To generate the new documentation, follow these steps:

1) Download and install Doxygen from

[http://www.stack.nl/~dimitri/doxygen/download.html#latestsrc](http://www.stack.nl/~dimitri/doxygen/download.html#latestsrc)

2) Run Doxygen wizard (the executable is usually called doxywizard)

3) In the Doxygen window, enter "Copasi API" as the project name, and provide a version number. Set the copasi-simple-api folder as the source folder and copasi-simple-api/documentation as the destination folder. At the top of the window, specify the working directory as the copasi-simple-api/documentation folder.

4) Click Next. Select "Optimize for C" in the the programming languages option

5) Click Next a few times until you reach the end. At the top of the window

6) Go to the "Run" tab and click "Run doxygen".

7) The documentation has been generated. Use svn commit on the copasi-simple-api/documentation folder to update the online documentation.

8) Tip: You might find that a new html file generated from doxygen does not render in the browser. In order to make the html render correctly, make sure you add the text/html mime type to the file. Under TortoiseSVN, select the file and choose properties from the TortoiseSVN popup menu. Select svn:mime-type in the property type and add text/html to the property value. Select OK and commit the change. The html file will now render correctly.

Alternatively you can make SVN do this for you by editing the SVN configuration file. To do this, select Settings under TortoiseSVN. In the general tab (usually the first you'll see) there is a button marked Edit towards the right hand corner, select this and the config file will be loaded into your favourite editor. In the config file locate the line, miscellany and uncomment the line marked: enable-auto-props = yes. Finally, locate the line \[auto-props] and add the following lines to the end of section:

    *.html = svn:mime-type=text/html
    *.css = svn:mime-type=text/css
    *.js = svn:mime-type=text/javascript
    *.txt = svn:mime-type=text/plain;svn:eol-style=native
    *.png = svn:mime-type=image/png
    *.jpg = svn:mime-type=image/jpeg
    *.pdf = svn:mime-type=application/pdf
    *.jpeg = svn:mime-type=image/jpg
    *.tiff = svn:mime-type=image/tiff
    *.tif = svn:mime-type=image/tiff

