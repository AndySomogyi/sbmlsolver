from pyomexmeta import RDF
import os

rdf_str = """@prefix rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> .
@prefix bqbiol: <http://biomodels.net/biology-qualifiers/> .
@prefix OMEXlib: <http://omex-library.org/> .
@prefix myOMEX: <http://omex-library.org/NewOmex.omex/> .
@prefix local: <http://omex-library.org/NewOmex.omex/NewModel.rdf#> .

<http://omex-library.org/NewOmex.omex/NewModel.xml#OmexMetaId0000>
  bqbiol:is <https://identifiers.org/uniprot/PD12345> ."""

# we write the annotations to file, so we can read it in from file
#   filename is in same directory as your python script
fname = os.path.join(os.path.dirname(__file__), "annotations.rdf")
#   write turtle syntax to file
with open (fname, "w") as f:
    f.write(rdf_str)

# Add to our RDF graph by reading the second string
rdf = RDF.from_file(fname, "turtle")

# remove the file we wrote
if os.path.isfile(fname):
    os.remove(fname)
