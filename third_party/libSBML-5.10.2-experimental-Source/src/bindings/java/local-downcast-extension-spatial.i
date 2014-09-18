/**
 * casting to most specific SBMLExtension object
 */

#ifdef USE_SPATIAL
%pragma(java) modulecode =
%{
  if (pkgName.equals("spatial"))
  {
    return new SpatialExtension(cPtr, owner);
  }
%}
#endif // USE_SPATIAL

