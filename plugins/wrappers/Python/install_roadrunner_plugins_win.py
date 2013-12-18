""" 
Usage:
   python install_roadrunner_plugins.py

   This file will install all the relevant roadRunner files 
   and plugins so that roadrunner can be used from Python
   In addition ise setup the PYTHONPATH and modified the PATH
   Currently for Windows only
"""

import site
import shutil
import os.path
import os
import sys

import _winreg
import win32gui, win32con

def rmroadrunner(path):
	print('deleting roadrunner from ' + path)
	
	try:
		os.remove(os.path.join(path, 'roadrunner.py'))
	except:
		pass
	
	try:
		os.remove(os.path.join(path, 'roadrunner.pyc'))
	except:
		pass
	
	try:
		os.remove(os.path.join(path, '_roadrunner.pyd'))
	except:
		pass
	
	try:
		os.remove(os.path.join(path, '_roadrunner.so'))
	except:
		pass
	
	shutil.rmtree(os.path.join(path, 'roadrunner'), ignore_errors=True)

splist = site.getsitepackages()
sp = ''
	
	
for s in splist:
	rmroadrunner(s)
	if s.find('site-packages') > 0:
		sp = s
		
print('installing roadrunner to site packages dir: ' + sp)

dst = os.path.join(sp, 'roadrunner')

# src dir should be location of the roadunner package, i.e.
# src directory is :c:\Users\IEUser\local\site-packages\roadrunner

# need to get relative to this file as cwd may be different than
# dir the file dir if its say run from desktop. 
cwd =  os.path.dirname(os.path.abspath(__file__))
upOneCwd = os.path.split(cwd)[0]

src = os.path.abspath(os.path.join(upOneCwd, 'site-packages', 'roadrunner'))

print
print('src directory is :' + src)

print("destination directory is: " + dst)

shutil.copytree(src, dst)

# --------------------------------------------------------------------------
# Find the src root path
pluginsrc = os.path.split(src)[0]
rootDirSrc = os.path.split(pluginsrc)[0]
print "Root directory: " + rootDirSrc

# Find the dest root path
plugindst = os.path.split(dst)[0]
plugindst = os.path.split(plugindst)[0]
print "Plugin destination: " + plugindst

# Add to PythonPath
# sys.path.append (plugindst + '\\roadrunner\\python')
print "Python Path = " + plugindst + '\\roadrunner\\python'

# Copy rrPlugins.py and rrPython.py
#shutil.copyfile (rootDirSrc + '\\plugins\\python\\rrPlugins.py', plugindst + '\\roadrunner\\python\\rrPlugins.py')
#shutil.copyfile (rootDirSrc + '\\c_api_python\\rrPython.py', plugindst + '\\roadrunner\\python\\rrPython.py')

localMachine = _winreg.ConnectRegistry(None, _winreg.HKEY_CURRENT_USER)

envKey = _winreg.OpenKey(localMachine, r"Environment",  0, _winreg.KEY_ALL_ACCESS)
try:
  currentPythonPath = _winreg.QueryValueEx(envKey, "PYTHONPATH")[0]
except:
  print "Can't find python path"
  currentPythonPath = ""
_winreg.CloseKey(envKey)

# Now add the rootsrc to the PATH directory
localMachine = _winreg.ConnectRegistry(None, _winreg.HKEY_LOCAL_MACHINE)
binPath = rootDirSrc + "\\bin"
envKey =_winreg.OpenKey(localMachine, r"SYSTEM\CurrentControlSet\Control\Session Manager\Environment", 0, _winreg.KEY_ALL_ACCESS)
path = _winreg.QueryValueEx(envKey, "Path")[0]
# Check if the rootDirSrc + \\bin is already in the path
if not (binPath.lower() in path.lower()):
   # now append root/bin to the path
   _winreg.SetValueEx(envKey, "PATH", 0, _winreg.REG_EXPAND_SZ, path  + ";" + binPath)
   print "PATH modified to: " + binPath
else:
   print "Path already pointing to root directory: " + binPath
_winreg.CloseKey(envKey)

pythonPath = rootDirSrc + '\\plugins\\python'
localMachine = _winreg.ConnectRegistry(None, _winreg.HKEY_CURRENT_USER)
envKey =_winreg.OpenKey(localMachine, r"Environment", 0, _winreg.KEY_ALL_ACCESS)
if not (pythonPath.lower() in currentPythonPath.lower()):
   # now set the env var
   if (currentPythonPath == ""):
      _winreg.SetValueEx(envKey, "PYTHONPATH", 0, _winreg.REG_SZ, pythonPath)
   else:
      _winreg.SetValueEx(envKey, "PYTHONPATH", 0, _winreg.REG_EXPAND_SZ, currentPythonPath  + ";" + pythonPath)
   print "PYTHONPATH modified to: " + pythonPath
else:
   print "PythonPath already pointing to python path: " + pythonPath
_winreg.CloseKey(envKey)
_winreg.CloseKey(localMachine)

win32gui.SendMessage(win32con.HWND_BROADCAST, win32con.WM_SETTINGCHANGE, 0, 'Environment')





