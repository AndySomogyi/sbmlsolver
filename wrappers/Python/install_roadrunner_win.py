""" 
Usage:
   python install_roadrunner.py

   This file will install all the relevant roadRunner files 
   so that roadrunner can be used from Python
"""

import site
import shutil
import os.path
import os

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

src = os.path.abspath(os.path.join('site-packages', 'roadrunner'))

print('src directory is :' + src)

print("destination directory is: " + dst)

shutil.copytree(src, dst)

