
#ifdef USE_ARRAYS
else if (pkgName == "arrays")
{
	switch (sb->getTypeCode())
	{
		case SBML_LIST_OF:
			name = sb->getElementName();
			if(name =="listOfIndices")
			{
				return SWIGTYPE_p_ListOfIndices;
			}
			else if(name =="listOfDimensions")
			{
				return SWIGTYPE_p_ListOfDimensions;
			}
			return SWIGTYPE_p_ListOf;				  
			
		case SBML_ARRAYS_DIMENSION:
			return SWIGTYPE_p_Dimension;
				
		case SBML_ARRAYS_INDEX:
			return SWIGTYPE_p_Index;

		default:
			return SWIGTYPE_p_SBase;
	}
}
#endif // USE_ARRAYS				  

