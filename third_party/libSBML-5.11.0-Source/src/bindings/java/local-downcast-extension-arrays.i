/**
 * casting to most specific SBMLExtension object
 */
 

#ifdef USE_ARRAYS
%pragma(java) modulecode =
%{
    if (pkgName.equals("arrays"))
		return new ArraysExtension(cPtr, owner);
	%}
#endif // USE_ARRAYS		

