from pyomexmeta import RDF

rdf_str = """@prefix rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> .
             @prefix bqbiol: <http://biomodels.net/biology-qualifiers/> .
             @prefix OMEXlib: <http://omex-library.org/> .
             @prefix myOMEX: <http://omex-library.org/NewOmex.omex/> .
             @prefix local: <http://omex-library.org/NewOmex.omex/NewModel.rdf#> .
             
             local:OmexMetaId0000
                 bqbiol:is <https://identifiers.org/uniprot/PD12345> ."""

# collect the link from "https://www.ebi.ac.uk/biomodels/BIOMD0000000308#Files"
tyson2003 = "https://www.ebi.ac.uk/biomodels/model/download/BIOMD0000000308.2?filename=BIOMD0000000308_url.xml"

# first create a RDF graph, by any means available - here we choose from_string
rdf = RDF.from_string(rdf_str, format="turtle")

# And now add to it from a uri
rdf.add_from_uri(tyson2003, "rdfxml")

print(f"Number of annotations in graph: {len(rdf)}")
