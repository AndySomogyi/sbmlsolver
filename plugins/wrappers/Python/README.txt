libRoadRunners Python PluginAPI using ctypes
Totte Karlsson (tottek@gmail.com)

The rrPlugins.py file is a python wrapper using ctypes, for RoadRunners pluginlib_c_api library.
If you import this script in your on script, you will get access to the functionality as
documented on the RoadRunner website:
http://docs.libroadrunner.org/rrLibPluginsPythonAPI/html/index.html

==== IMPORTANT NOTES ====
To use the rrPlugins.py in your own scripts, or if you are to run the example scripts, 
the following requirements need to be satisfied

1) The folder containing the rrPlugins.py need to be added to your SYSTEMS PYTHONPATH, 
	e.g. add c:\libroadrunner-1.0.0-win32\plugins\python to your PYTHONPATH environment variable
    if you unzipped the distribution in the c:\libroadrunner-1.0.0-win32 folder

2) The roadrunner "bin" folder need to be on your user PATH, or system PATH, 
    e.g. add c:\libroadrunner-1.0.0-win32\bin to the PATH environment variable
    if you unzipped the distribution in the c:\libroadrunner-1.0.0-win32 folder

=================================================================================

=== ADDITIONAL NOTES ====
In general, there is no need to move the plugins folder. However if you decide to do so, the follwoing apply:
1)  Understand that you may need to give the absolute path to the plugins folder, in the createPluginManager() 
    function. This is becuase the plugin manager, as deployed in the binary distribution, has a default path set 
    that is relative to the plugins folder. If this is altered, the new path may need to be supplied EXPLICITLY.

2)  Update the PYTHONPATH to include the folder in which rrPython is located


The Example folder contains Examples illustrating how to get access and use the pluginlibrary and plugins.
See further API documentation on the web, http://libroadrunner.org/ on how to use this wrapper.



