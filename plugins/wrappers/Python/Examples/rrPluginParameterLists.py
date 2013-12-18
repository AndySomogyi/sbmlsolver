from rrPlugins import *

paramHandle = createParameter("myList", "listOfParameters", "a Hint")

#getHandle toValue
paraListHandle = getParameterValueHandle(paramHandle)

#Should be empty
print getNamesFromParameterList(paraListHandle)

#Add a new parameter
aParameterHandle = createParameter("newOne", "double", "the hint")
addParameterToList(paraListHandle, aParameterHandle)

setDoubleParameter(aParameterHandle, 34)
print getDoubleParameter(aParameterHandle)

#Or you can use
print getParameterValue(aParameterHandle)

#Should not be empty
print getNamesFromParameterList (paraListHandle)

#Get the parameter
paraHandle = getFirstParameter(paraListHandle)

print getParameterHint (paraHandle)
print getParameterInfo (paraHandle)
print getNamesFromParameterList (paraListHandle)


print clearParameterList(paraListHandle)
#Should be empty
print getNamesFromParameterList (paraListHandle)

freeParameter(paramHandle)

