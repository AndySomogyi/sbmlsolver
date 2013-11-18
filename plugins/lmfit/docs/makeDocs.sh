#! /usr/bin/bash

echo "Creating documentation for your plugin"
pluginName=lmfit
chapter=plugin_$pluginName
mainDoc=main.tex
pdflatex -jobname=$pluginName "\includeonly{$chapter}\input{$mainDoc}"


