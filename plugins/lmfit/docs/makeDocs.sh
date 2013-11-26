#! /usr/bin/bash

echo "Creating documentation for the plugin"
pluginName=lmfit
chapter=plugin_$pluginName
mainDoc=$pluginName.tex
pdflatex -jobname=$pluginName "\includeonly{$chapter}\input{$mainDoc}"

#Create the C file to link with the plugin
hdrFile="$pluginName"_doc.h
cppFile="$pluginName"_doc.cpp  

#Create header
echo "#ifndef $pluginName"_docH > $hdrFile
echo "#define $pluginName"_docH >> $hdrFile

echo "RR_PLUGIN_DECLSPEC extern unsigned char pdf_doc[];" >> $hdrFile
echo "RR_PLUGIN_DECLSPEC extern size_t sizeofPDF;" >> $hdrFile
echo "#endif" >> $hdrFile

#create source
cat $pluginName.pdf | (echo "unsigned char pdf_doc[]={";xxd -i; echo "};") > $cppFile
echo "int sizeofPDF = sizeof(pdf_doc);" >> $cppFile

echo "Done.."


