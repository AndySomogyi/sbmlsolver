#ifdef USE_MULTI

%newobject removePossibleSpeciesFeatureValue;
%newobject removeSpeciesFeatureValue;
%newobject removeCompartmentReference;
%newobject removeSpeciesTypeInstance;
%newobject removeInSpeciesTypeBond;
%newobject removeDenotedSpeciesTypeComponentIndex;
%newobject removeOutwardBindingSite;
%newobject removeSpeciesFeatureChange;
%newobject removeSpeciesFeatureType;
%newobject removeSpeciesTypeComponentIndex;
%newobject removeSpeciesFeature;
%newobject removeSpeciesTypeComponentMapInProduct;
%newobject removeMultiSpeciesType;

%template (MultiPkgNamespaces) SBMLExtensionNamespaces<MultiExtension>;

%include <sbml/packages/multi/extension/MultiExtension.h>
%include <sbml/packages/multi/extension/MultiModelPlugin.h>
%include <sbml/packages/multi/extension/MultiCompartmentPlugin.h>
%include <sbml/packages/multi/extension/MultiSpeciesPlugin.h>
%include <sbml/packages/multi/extension/MultiReactionPlugin.h>
%include <sbml/packages/multi/extension/MultiSimpleSpeciesReferencePlugin.h>
%include <sbml/packages/multi/extension/MultiSpeciesReferencePlugin.h>
%include <sbml/packages/multi/sbml/PossibleSpeciesFeatureValue.h>
%include <sbml/packages/multi/sbml/SpeciesFeatureValue.h>
%include <sbml/packages/multi/sbml/CompartmentReference.h>
%include <sbml/packages/multi/sbml/SpeciesTypeInstance.h>
%include <sbml/packages/multi/sbml/InSpeciesTypeBond.h>
%include <sbml/packages/multi/sbml/DenotedSpeciesTypeComponentIndex.h>
%include <sbml/packages/multi/sbml/OutwardBindingSite.h>
%include <sbml/packages/multi/sbml/SpeciesFeatureChange.h>
%include <sbml/packages/multi/sbml/SpeciesFeatureType.h>
%include <sbml/packages/multi/sbml/SpeciesTypeComponentIndex.h>
%include <sbml/packages/multi/sbml/SpeciesFeature.h>
%include <sbml/packages/multi/sbml/SpeciesTypeComponentMapInProduct.h>
%include <sbml/packages/multi/sbml/MultiSpeciesType.h>

#endif /* USE_MULTI */

