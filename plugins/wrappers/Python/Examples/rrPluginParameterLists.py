from rrPlugins import *

listParaHandle = createParameter("myList", "listOfParameters", "a Hint")

#Should be empty
print getNamesFromParameterList(listParaHandle)

#Add a new parameter
aParameterHandle = createParameter("newOne", "double", "the hint")
addParameterToList(listParaHandle, aParameterHandle)

setDoubleParameter(aParameterHandle, 34)
print getDoubleParameter(aParameterHandle)

#Or you can use
print getParameterValue(aParameterHandle)

#Should not be empty
print getNamesFromParameterList (listParaHandle)

#Get the parameter
paraHandle = getFirstParameter(listParaHandle)

print getParameterHint (paraHandle)
print getParameterInfo (paraHandle)
print getNamesFromParameterList (listParaHandle)


print clearParameterList(listParaHandle)
#Should be empty
print getNamesFromParameterList (listParaHandle)

freeParameter(listParaHandle)

