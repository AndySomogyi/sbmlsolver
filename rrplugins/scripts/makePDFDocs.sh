#! /usr/bin/bash

if [ $# -ne 1 ]; then
    echo "Missing argument -> plugin name"
    exit 1
fi
echo "Creating PDF documentation for the plugin"
mainDoc=mainDoc.tex
pluginName=$1
pdfName=$pluginName.pdf
chapter=$pluginName

#run twice to resolve references
pdflatex -jobname=thePDF "\includeonly{$chapter}\input{$mainDoc}"
pdflatex -jobname=thePDF "\includeonly{$chapter}\input{$mainDoc}"

mv thePDF.pdf $pdfName

##Create the C file to link with the plugin
hdrFile="$pluginName"_doc.h
cppFile="$pluginName"_doc.cpp  

#Create header
echo "#ifndef $pluginName"_docH > $hdrFile
echo "#define $pluginName"_docH >> $hdrFile
echo "#include <stddef.h>" >> $hdrFile
echo "extern unsigned char pdf_doc[];" >> $hdrFile
echo "extern size_t sizeofPDF;" >> $hdrFile
echo "#endif" >> $hdrFile

#create source
echo "#include \"$hdrFile\"" > $cppFile
echo "" >> $cppFile
echo "unsigned char pdf_doc[]={" >> $cppFile

cat $pluginName.pdf | (xxd -i; echo "};") >> $cppFile
echo "size_t sizeofPDF = sizeof(pdf_doc);" >> $cppFile

mv $hdrFile $cppFile ./..
mkdir pdf
mv $pdfName ./pdf
echo "========== Done createing PDF for plugin: $pluginName."


