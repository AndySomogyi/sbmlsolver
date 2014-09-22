/**
 * casting to most specific SBMLNamespaces object
 */


#ifdef USE_ARRAYS
%pragma(java) modulecode =
%{
	
	if (ns.hasURI(ArraysExtension.getXmlnsL3V1V1()))
	{
		return new ArraysPkgNamespaces(cPtr, owner);
	}
	%}
#endif /* USE_ARRAYS*/
