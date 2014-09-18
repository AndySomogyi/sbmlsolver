/**
 * casting to most specific SBMLExtension object
 */
 
#ifdef USE_ARRAYS
%pragma(csharp) modulecode =
%{
		if (pkgName == "arrays")
			return new ArraysExtension(cPtr, owner);
%}
#endif /* USE_ARRAYS*/
