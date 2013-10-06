---
layout: page
title: "Roadrunner CLI"
description: "using the roadrunner standalone executable"
---
{% include JB/setup %}

How to use the roadrunner standalone executable

The roadrunner command line client, rr, is built and deployed when you select 'Applications' in the CMake UI. rr can be used to test the integrity of the current build, but also as a fully fledged SBML simulator.

##Models
When you build and install RoadRunner, a folder with some test models are deployed. On a Linux build, this folder is typically placed in /usr/local/models.

The first test after building roadrunner library would be to run the rr application, using one of the models as input.

##Run rr
To run rr using one of the models as input, type the following in a terminal; rr -m/usr/models/test_1.xml

If the library is properly built, you should get simulated data back. You will also see some new files in the same directory as where you executed rr. These files, .h, .c and .so are on the fly generated sources and libs, representing the sbml model in your xml model file.

Typing rr in a terminal window will give you some more options for the application. For example `rr -t<aFolder>`, will write temporary files to specified folder.

To trace problems, `-v<DebugLevel>`, where DebugLevel is Debug, Debug1, Debug2 etc as a 'string', can be used to get more verbose feedback.

Feel free to report any problems, suggestions by email.


