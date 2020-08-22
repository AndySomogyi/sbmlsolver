import  telplugins as c
import random
 
def printData(telData):
    for row in range(nrRows):
        rowLine = ''        
        for col in range(nrCols):
            if row == 0 and col ==0:
                print colHdr                
            rowLine = rowLine +  `c.getTelluriumDataElement(telDataH, row, col)` + ' '
        print rowLine
        
    if c.hasWeights(telData):
        printWeights(telData)        
    
def printWeights(telData):
    for row in range(nrRows):
        rowLine = ''        
        for col in range(nrCols):
            if row == 0 and col ==0:
                print colHdr                
            rowLine = rowLine +  `c.getTelluriumDataWeight(telDataH, row, col)` + ' '
        print rowLine


#=============================================================
try:
    telDataH = c.createTelluriumData(4,2)    
    nrRows = c.getTelluriumDataNumRows(telDataH)
    nrCols = c.getTelluriumDataNumCols(telDataH)
    colHdr = c.getTelluriumDataColumnHeader(telDataH)
    
    #Setup some numbers
    nr = 1.1    
    for row in range(nrRows):        
        for col in range(nrCols):            
            c.setTelluriumDataElement(telDataH, row, col, nr)
            nr = nr + 1            
        
    #Print the data        
    printData(telDataH)             
             
    ## Weights ===================
    if c.hasWeights(telDataH) is False:
        print 'allocating weights'
        c.allocateWeights(telDataH)
        
    print 'Weights======== '
    printWeights(telDataH)
                
    #Change weights
    for row in range(nrRows):        
        for col in range(nrCols):           
            randomNr = random.random() 
            c.setTelluriumDataWeight(telDataH, row, col, randomNr)                        
    printWeights(telDataH)        
    
    #===== Column header
    print 'ColumnHeader is:' + `c.getTelluriumDataColumnHeader(telDataH)`    
    c.setTelluriumDataColumnHeader(telDataH, 'A,B')
    print 'ColumnHeader is:' + `c.getTelluriumDataColumnHeader(telDataH)`

    #This should fail
    c.setTelluriumDataColumnHeader(telDataH, 'A,B,C')
    print 'ColumnHeader is:' + `c.getTelluriumDataColumnHeader(telDataH)`
    
    index = 0
    c.setTelluriumDataColumnHeaderByIndex(telDataH, index, 'Q')
    print 'ColumnHeader is:' + `c.getTelluriumDataColumnHeader(telDataH)`
    print 'ColumnHeader index `index` is:' + `c.getTelluriumDataColumnHeaderByIndex(telDataH, index)`
                      
    index = 1
    c.setTelluriumDataColumnHeaderByIndex(telDataH, index, 'w')
    print 'ColumnHeader is:' + `c.getTelluriumDataColumnHeader(telDataH)`
    print 'ColumnHeader index `index` is:' + `c.getTelluriumDataColumnHeaderByIndex(telDataH, index)`

    ##File saving and writing
    fName = 'data.dat'
    c.writeTelluriumData(telDataH, fName)                            

    ##Read it into another data object
    telData2 = c.createTelluriumData(40,2)   #Data will be reshaped when reading..
    c.writeTelluriumData(telDataH,"data.dat")
                                
    c.readTelluriumData(telData2, fName)
    printData(telData2)
                     
    #Read Non existing file
    if c.readTelluriumData(telData2, "notFound.dat") == False:
        print c.getLastError()
    else:
        printData(telData2)
                         
    #Check some exceptions                         
    #Try to read a file with bad format
    if c.readTelluriumData(telData2, "badDataFormat1.dat") == False:
        print c.getLastError()
    else:        
        printData(telData2)

    #Try to read a file with bad format
    if c.readTelluriumData(telData2, "badDataFormat2.dat") == False:
        print c.getLastError()
    else:        
        printData(telData2)
     
                            
except Exception as e:
    print 'There was an exception: ' + `e`
        







