#ifdef USE_SPATIAL
if (pkgName == "spatial")
{
  if (sb->getTypeCode() == SBML_MODEL)
  {
    return SWIGTYPE_p_SpatialModelPlugin;
  }
  else if (sb->getTypeCode() == SBML_COMPARTMENT)
  {
    return SWIGTYPE_p_SpatialCompartmentPlugin;
  }
  else if (sb->getTypeCode() == SBML_SPECIES)
  {
    return SWIGTYPE_p_SpatialSpeciesPlugin;
  }
  else if (sb->getTypeCode() == SBML_PARAMETER)
  {
    return SWIGTYPE_p_SpatialParameterPlugin;
  }
  else if (sb->getTypeCode() == SBML_REACTION)
  {
    return SWIGTYPE_p_SpatialReactionPlugin;
  }
}

#endif // USE_SPATIAL 

