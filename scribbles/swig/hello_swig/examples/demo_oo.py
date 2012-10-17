import sys
sys.path.append('../release')
import hello_oo as h

print 'creating g'
g = h.Greeter()
print g
print 'greeting Bob'
g.greet("Bob")
print g
