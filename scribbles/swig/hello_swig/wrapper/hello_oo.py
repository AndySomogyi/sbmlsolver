'''
This is a Python module that wraps the SWIG wrapper.
It groups the generated functions into classes
that behave more or less pythonically. The other
advantage is that it automates memory management.
'''

import hello_swig as h

class Greeter(object):
    def __init__(self):
        '''
        this Python class just stores a pointer
        to the equivalent C struct
        '''
        self.ptr = h.create_greeter()

    def __str__(self):
        '''
        this is what you see when you print an instance of the class
        '''
        return '<%s count:%s>' % (self.__class__.__name__, self.count)

    def __del__(self):
        '''
        when the class is garbage collected,
        this frees the struct
        '''
        h.delete_greeter(self.ptr)

    def greet(self, name):
        '''
        functions operating on the struct
        should be methods of the class
        '''
        h.print_greeting(self.ptr, name)

    @property
    def count(self):
        '''
        this exposes a getter as if it were a plain
        attribute of the Python class rather than a function
        '''
        return h.get_count(self.ptr)

