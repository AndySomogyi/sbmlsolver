import sys
import shutil
import rrPython

rr = rrPython

if sys.platform.startswith('win32'):
    testFileName ="r:/installs/cg/xe3/debug/testing/ModelSourceTest"
    tempFolder = 'r:/temp'
else:
    testFileName ="/home/totte/rrInstall/testing/ModelSourceTest"
    tempFolder = '/home/totte/temp'

if not rr.setTempFolder(tempFolder) == True:
    print "Failed to set temp folder!!"
    exit()

#Copy test source to temp folder
shutil.copy(testFileName + ".h", tempFolder)
shutil.copy(testFileName + ".c", tempFolder)

sourceFileName = tempFolder + "/ModelSourceTest.c"
print rr.compileSource(sourceFileName)

print "done"
