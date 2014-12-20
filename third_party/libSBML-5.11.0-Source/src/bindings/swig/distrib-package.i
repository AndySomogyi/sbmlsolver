#ifdef USE_DISTRIB

%newobject removeDistribInput;

%template (DistribPkgNamespaces) SBMLExtensionNamespaces<DistribExtension>;

%include <sbml/packages/distrib/extension/DistribExtension.h>
%include <sbml/packages/distrib/extension/DistribFunctionDefinitionPlugin.h>
%include <sbml/packages/distrib/extension/DistribSBasePlugin.h>
%include <sbml/packages/distrib/sbml/DrawFromDistribution.h>
%include <sbml/packages/distrib/sbml/DistribInput.h>
%include <sbml/packages/distrib/sbml/Uncertainty.h>
%include <sbml/packages/distrib/util/UncertMLNode.h>

#endif /* USE_DISTRIB */

