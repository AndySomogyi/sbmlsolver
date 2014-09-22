#ifdef USE_DISTRIB
else if (pkgName == "distrib")
{
	switch (sb->getTypeCode() )
	{
		case SBML_LIST_OF:
			name = sb->getElementName();
			if (name == "listOfDistribInputs")
			{
				return SWIGTYPE_p_ListOfDistribInputs;
			}

			return SWIGTYPE_p_ListOf;

		case SBML_DISTRIB_DRAW_FROM_DISTRIBUTION:
			return SWIGTYPE_p_DrawFromDistribution;

		case SBML_DISTRIB_INPUT:
			return SWIGTYPE_p_DistribInput;

		case SBML_DISTRIB_UNCERTAINTY:
			return SWIGTYPE_p_Uncertainty;

		default:
			return SWIGTYPE_p_SBase;
	}
}

#endif // USE_DISTRIB 

