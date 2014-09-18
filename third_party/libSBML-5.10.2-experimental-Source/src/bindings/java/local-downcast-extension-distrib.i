/**
 * casting to most specific SBMLExtension object
 */

#ifdef USE_DISTRIB
%pragma(java) modulecode =
%{
	if (pkgName.equals("distrib"))
	{
		return new DistribExtension(cPtr, owner);
	}
%}
#endif // USE_DISTRIB

