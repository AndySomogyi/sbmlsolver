import sys
import subprocess
import datetime

install3rdParty=1
doBuild=1
doClean=0

tsvn="C:\\Program Files\\TortoiseSVN\\bin\\svn.exe"
sBoxRoot='r:/roadrunnerwork'
buildFolder='r:/builds/vs/release'
rrSLN='RoadRunner.sln'
rrBuilds=["all"]

now = datetime.datetime.now()
print 'Build started at: ' + now.strftime("%Y-%m-%d %H:%M")

rrUpdates=[]
buildConfig='Release'

#ThirdParty
if install3rdParty== 1:
    print 'Building \"' + buildConfig + '\" ThirdParties' 
    try:
        output = subprocess.check_output(['msbuild', '/p:Configuration='+buildConfig, buildFolder +'/ThirdParty/INSTALL.vcxproj'], shell=True)
        print 'ThirdParty install succeeded'
    except subprocess.CalledProcessError, e:
        print "Third Party build failed:\n", e.output

#Cleaning....
if doClean == 1:
    for build in rrBuilds:
        try:
            output = subprocess.check_output(['msbuild', '/p:Configuration='+buildConfig, buildFolder +'/'+ build + '/RoadRunner.sln', '/t:clean'], shell=True)
            print 'Cleaning build \"' + build + '\" succeded'
        except subprocess.CalledProcessError, e:
            print "Cleaning package " + build + " failed: \n", e.output

if doBuild == 1:
    #Create Packages
    for build in rrBuilds:
        try:
            output = subprocess.check_output(['msbuild', '/p:Configuration='+buildConfig, buildFolder +'/'+ build + '/PACKAGE.vcxproj'], shell=True)
            print 'Creating package \"' + build + '\" succeded'
            rrUpdates.append(build)
        except subprocess.CalledProcessError, e:
            print "Building package " + build + " failed:\n", e.output

print "done..."

