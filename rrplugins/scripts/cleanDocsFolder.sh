#! /usr/bin/bash

folder=`pwd`
echo "Cleaning folder: $folder"

extensions="4ct 4tc aux css dvi idv lg log tmp xref html bak pdf"

for ext in $extensions
do
    echo "Cleaning *.$ext"
    rm *.$ext
done

echo "Done.."


