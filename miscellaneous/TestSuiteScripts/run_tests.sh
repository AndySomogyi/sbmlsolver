#! /usr/bin/bash

#Require three arguments
start=$1
end=$2
compiler=$3

reportFolder="/cygdrive/r/roadrunnerwork/miscellaneous/TestSuiteReports"
tempFolder="/cygdrive/r/RRTesting"
dataOutputRoot=$tempFolder"/DataOutput/"$compiler
logFile=$tempFolder/"testLog_$compiler.txt"
logTable=$reportFolder"/logTable_$compiler.csv"
failed=$reportFolder"/failedTests_$compiler.csv"
binFolder="/cygdrive/r/installs/"$compiler"/full/bin/"
simulator=$binFolder"/rr_ts_tester.exe"
zipper="/cygdrive/r/roadrunnerwork/miscellaneous/zipper/7za.exe"
ModelsDir="R://SBML_l2v4"

#empty files
echo "" > $logFile
echo "" > $failed

cd $binFolder
echo "Current working folder is"`pwd`
echo "Simulator program is "$simulator

#Remove previous data files
echo "Removing previous data files..."
find $dataOutputRoot -name '*l2v4.csv' -exec rm -f {} \;
echo "Start runnning tests..."
echo

for ((i=$start; i<=$end; i++ ));
do
	echo "Running $i" ;
	echo "Case_"$i >> $logFile;
    winPath=`cygpath -w $dataOutputRoot`
    #(time $simulator -m$ModelsDir -n$i -d$winPath -vError) >> $logFile 2>&1 
    $simulator -m$ModelsDir -n$i -d$winPath -vError >> $logFile  
#    sleep .0015
    echo "Done" >> $logFile #This is used as a separator for the make_table app
done

echo "Waiting for background jobs to finish..."
wait

#Create a table for the result
echo "Making result csv table: $logTable"

make_table -c -f`cygpath -w $logFile` -w"Done" > $logTable 

#filter failed ones
cat $logTable | grep "FAIL," > $failed

cd $tempFolder
#echo "Current folder is"`pwd`

#Copy files and zip them up
dataFiles="dataFiles_$compiler.txt"

echo "Find data files in $dataOutputRoot. Saving to $dataFiles"
find $dataOutputRoot -name '*l2v4.csv' > $dataFiles

cygpath -w `cat $dataFiles` > $dataFiles

zipFile=`cygpath -w $reportFolder`"/data_`date +"%m%d%Y"`_$compiler.zip"
rm -vf $zipFile

echo "Zip it up..."
$zipper a $zipFile @$dataFiles
echo "Done"
