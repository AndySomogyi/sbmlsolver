
.. class:: ConservedMoietyConverter(*args)
   :module: roadrunner

   Takes a sbml document, determines if there are any conserved moieties 
   and generates a new document with the dependent species replaces with 
   assignment rules and new set of parameters introduced which are the 
   conserved moieties. 
   

.. method:: ConservedMoietyConverter.setDocument(sbmlOrURI)

   Sets the source document. This may be either the contents of an sbml docment
   as a string, or the path the the file. 

   :param str sbmlOrURI: contents or path of source document
   :returns: an integer indicating success or failure, 0 means success. 
   :rtype: int


.. method:: ConservedMoietyConverter.covert()
   
   Perform the document conversion. 

   :returns: an integer indicating success or failure, 0 means success. 
   :rtype: int


.. method:: ConservedMoietyConverter.getDocument()
   
   Get the converted document contents. 

   :returns: The contents of the converted document, or empty string
             if there is no source document.
