#! /usr/bin/bash

if [ $# -ne 1 ]; then
    echo "Missing argument -> plugin name"
    exit 1
fi
echo "Creating PDF documentation for the plugin"

pluginName=$1
mainDoc=mainDoc

htlatex $mainDoc.tex

mkdir html
cp -v $mainDoc.html ./html/index.html
rm -v $mainDoc.html
cp *.html ./html
cp -r Figures ./html
cp *.css ./html

echo "========== Done createing HTML for plugin: $pluginName."
