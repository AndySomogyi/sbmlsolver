import sys
import subprocess
import datetime

install3rdParty=1
doBuild=1
doClean=1
doCommitReleases=1
doCommitWiki=1

tsvn="C:\\Program Files\\TortoiseSVN\\bin\\svn.exe"
sBoxRoot='r:/roadrunnerwork'
buildFolder='r:/builds/vs/release'
rrSLN='RoadRunner.sln'
rrBuilds=["all"]

def updateDownloadsWiki(rrUpdates, svn_revision):
    template = sBoxRoot + "/wiki/Downloads.template"
    wiki = sBoxRoot + "/wiki/Downloads.wiki"
    print 'Updating WIKI. SVN Revision: ' + svn_revision
    date = datetime.datetime.now().strftime("%Y-%m-%d")
    time = datetime.datetime.now().strftime("%H:%M")
    substs =['DATE', 'TIME', 'SVN_REV']
    #parse the downloads wiki page template
    newText = ""
    tFile  = open(template, 'r' ).readlines()
    for line in tFile:
        for word in line.split():
            if word not in substs:
                newText = newText + ' ' + word
            elif word == 'DATE':
                    newText = newText + date.rstrip()
            elif word == 'TIME':
                    newText = newText + time.rstrip()
            elif word == 'SVN_REV':
                    newText = newText + svn_revision.rstrip()

        newText = newText + '\n'

    newText = newText.split('\n')
    #Clean lines
    saveText = ""
    for line in newText:
        line = line.lstrip()
        line = line.rstrip()
        saveText = saveText + line + '\n'

    #write to wiki
    wikiF = open(wiki, 'w')
    wikiF.writelines(saveText)
    wikiF.close()

    #Finally commit
    try:
        output = subprocess.check_output([tsvn, 'commit', sBoxRoot + '/wiki/', '-m\"Build Script Commit of wiki\"'], shell=True)
        print "Commit of wiki succeded."
    except subprocess.CalledProcessError, e:
        print "Failed svn commit of wiki:\n", e.output

now = datetime.datetime.now()
print 'RR BUILD'

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

if doCommitReleases  == 1:
    try:
        output = subprocess.check_output([tsvn, 'commit', sBoxRoot + '/releases/vs', '-m\"Build Script Commit\"'], shell=True)
        print "Commit succeded"
    except subprocess.CalledProcessError, e:
        print "Failed svn commit:\n", e.output

#Get svn revision
try:
    output = subprocess.check_output([tsvn, 'info', sBoxRoot + '/releases/vs'], shell=True)
    lines = output.split('\n')
    for line in lines:
        if "Revision: " in line:
            svn_revision = line.split(" ")[-1]
except subprocess.CalledProcessError, e:
    print "Failed getting svn revision:\n", e.output

if doCommitWiki == 1:
    updateDownloadsWiki(rrUpdates, svn_revision)

try:
    output = subprocess.check_output([tsvn, 'commit', sBoxRoot + '/build_scripts', '-m\"Build Script Commit\"'], shell=True)
    print "Commiting build log succeded"
except subprocess.CalledProcessError, e:
    print "Failed svn commit:\n", e.output

print "done..."

