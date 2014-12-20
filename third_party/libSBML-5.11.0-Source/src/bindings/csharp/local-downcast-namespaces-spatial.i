/**
 * casting to most specific SBMLNamespaces object
 */

#ifdef USE_SPATIAL
%pragma(csharp) modulecode =
%{
  if (ns.hasURI(SpatialExtension.getXmlnsL3V1V1()))
  {
    return new SpatialPkgNamespaces(cPtr, owner);
  }
%}
#endif /* USE_SPATIAL */

