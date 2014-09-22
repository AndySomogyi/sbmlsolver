/**
 * casting to most specific SBMLExtension object
 */

#ifdef USE_SPATIAL
%pragma(csharp) modulecode =
%{
  if (pkgName == "spatial")
  {
    return new SpatialExtension(cPtr, owner);
  }
%}
#endif /* USE_SPATIAL */

