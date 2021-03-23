"""Are properties the right Python structure to store things like species or compartment values?

Short answer - no.

(remember to remove)
"""

class C:
    """A class with a property"""
    def __init__(self):
        self._x = None

    def getx(self):
        return self._x

    def setx(self, value):
        self._x = value

    def delx(self):
        del self._x

    x = property(getx, setx, delx, "I'm the 'x' property.")

# properties are class level: all instances have access.
assert isinstance(C.x, property)

a = C()
b = C()
assert a.x == None
assert b.x == None

# difference instances of C can have different values of x, but all have a property called x
a.x = 4
assert a.x == 4
assert b.x == None

# Roadrunner dynamically creates properties per model. That means the properties created in one model
# are also created in another rr instance.
#
# Here we mimic the situation with a minimal working example.
# If `a` and `b` (below) were different roadrunner instances, i.e. different models, then
# `a` might have a species called `SpeciesA` while `b` might have `SpeciesB`

setattr(C, "_speciesA",  None) # assign a class level attribute.

def setA(self, a):
    print("calling setA")
    setattr(C, "_speciesA",  a)

def getA(self):
    print("calling getA")
    return self.__class__.__dict__["_speciesA"]

def delA(self):
    print("calling delA")
    del self.__class__.__dict__["_speciesA"]

PropertyA = property(getA, setA, delA, "Property called a")

# Now bind the property to the class C
C.SpeciesA = PropertyA

# as expected, a.SpeciesA is equal to None
assert a.SpeciesA == None

# we can assign new value to SpeciesA
a.SpeciesA = 4
assert a.SpeciesA == 4

# however this is a property of the *class* not the instance
# This means it is shared throughout all instances of C.
# Therefore b.SpeciesA not only exists, but as the same value as
# a.SpeciesA. This is static.
print(b.SpeciesA)
# # print(a.SpeciesA)
# a.SpeciesA = 1234
#





















