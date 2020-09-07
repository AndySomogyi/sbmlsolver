from pyomexmeta import RDF

# collect the link from "https://www.ebi.ac.uk/biomodels/BIOMD0000000308#Files"
tyson2003 = "https://www.ebi.ac.uk/biomodels/model/download/BIOMD0000000308.2?filename=BIOMD0000000308_url.xml"

# download the model, scan document for annotations and docs-build a graph
rdf = RDF.from_uri(tyson2003, format="rdfxml")

print(f"Number of annotations in graph: {len(rdf)}")
