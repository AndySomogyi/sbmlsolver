#ifdef USE_MULTI
if (pkgName == "multi")
{
  if (sb->getTypeCode() == SBML_MODEL)
  {
    return SWIGTYPE_p_MultiModelPlugin;
  }
  else if (sb->getTypeCode() == SBML_COMPARTMENT)
  {
    return SWIGTYPE_p_MultiCompartmentPlugin;
  }
  else if (sb->getTypeCode() == SBML_SPECIES)
  {
    return SWIGTYPE_p_MultiSpeciesPlugin;
  }
  else if (sb->getTypeCode() == SBML_REACTION)
  {
    return SWIGTYPE_p_MultiReactionPlugin;
  }
  else if (sb->getTypeCode() == SBML_MODIFIER_SPECIES_REFERENCE)
  {
    return SWIGTYPE_p_MultiSimpleSpeciesReferencePlugin;
  }
  else if (sb->getTypeCode() == SBML_SPECIES_REFERENCE)
  {
    return SWIGTYPE_p_MultiSpeciesReferencePlugin;
  }
}

#endif // USE_MULTI 

