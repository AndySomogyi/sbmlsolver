/**
 * casting to most specific SBMLExtension object
 */

#ifdef USE_DISTRIB
%pragma(csharp) modulecode =
%{
	if (pkgName == "distrib")
	{
		return new DistribExtension(cPtr, owner);
	}
%}
#endif /* USE_DISTRIB */

