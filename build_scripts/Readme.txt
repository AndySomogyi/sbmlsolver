Automatic Building Using builder.py

Invoke from command shell like this:
python -u builder.py | tee builder.log

The -u flag tells python not to buffer output

Part of the builder.py script is committing the builder.log file. 

The shell need to have python, svn . On windows, a cygwin bash shell is recommended.


See builder.py for more information.

