/**
 * casting to most specific SBMLNamespaces object
 */

#ifdef USE_DISTRIB
%pragma(java) modulecode =
%{
	if (ns.hasURI(DistribExtension.getXmlnsL3V1V1()))
	{
		return new DistribPkgNamespaces(cPtr, owner);
	}
%}
#endif /* USE_DISTRIB */

