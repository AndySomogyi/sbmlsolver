#! bash
echo 'Run all examples in a folder'
examples="`ls *.py`"
for example in $examples 
do
    echo "Running Example: $example"
    python $example
done

#read -p "Press any key.. If you can't find the Any key, call support."

