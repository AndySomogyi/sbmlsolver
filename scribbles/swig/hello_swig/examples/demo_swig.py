import sys
sys.path.append('../release')
import hello_swig as h

print 'creating g'
g = h.create_greeter()
print 'g is a', g
print 'g has greeted %s things' % h.get_count(g)
print
print 'greeting Bob'
h.print_greeting(g, 'Bob')
print 'g has greeted %s things' % h.get_count(g)
print
print 'deleting g using SWIG'
h.delete_greeter(g)
print 'accessing g now will cause undefined behavior'
print 'so we delete it from Python too'
del g

