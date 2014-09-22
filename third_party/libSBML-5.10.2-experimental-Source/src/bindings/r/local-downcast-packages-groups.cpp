
#ifdef USE_GROUPS
else if (pkgName == "groups")
{
	switch (sb->getTypeCode())
	{
		case SBML_LIST_OF:
			name = sb->getElementName();
			if(name == "listOfMembers"){
				return SWIGTYPE_p_ListOfMembers;
			}
			else if (name == "listOfMemberConstraints")
			{
				return SWIGTYPE_p_ListOfMemberConstraints;
			}
			else if(name == "listOfGroups"){
				return SWIGTYPE_p_ListOfGroups;
			}
			return SWIGTYPE_p_ListOf;				  
			
		case SBML_GROUPS_MEMBER:
			return SWIGTYPE_p_Member;

		case SBML_GROUPS_MEMBER_CONSTRAINT:
			return SWIGTYPE_p_MemberConstraint;

		case SBML_GROUPS_GROUP:
			return SWIGTYPE_p_Group;

		default:
			return SWIGTYPE_p_SBase;
	}
}
#endif // USE_GROUPS				  

