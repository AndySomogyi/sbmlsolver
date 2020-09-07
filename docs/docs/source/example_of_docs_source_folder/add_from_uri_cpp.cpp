#include "omexmeta/RDF.h" // omexmeta::RDF
#include <iostream>     // std::cout, std::endl

using namespace omexmeta;

int main(){

    std::string rdf_str = "@prefix rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> .\n"
                          "@prefix bqbiol: <http://biomodels.net/biology-qualifiers/> .\n"
                          "@prefix OMEXlib: <http://omex-library.org/> .\n"
                          "@prefix myOMEX: <http://omex-library.org/NewOmex.omex/> .\n"
                          "@prefix local: <http://omex-library.org/NewOmex.omex/NewModel.rdf#> .\n"
                          "\n"
                          "<http://omex-library.org/NewOmex.omex/NewModel.xml#OmexMetaId0000>\n"
                          "   bqbiol:is <https://identifiers.org/uniprot/PD12345> .\n";

    // first create a RDF graph, by any means available - here we choose from_string
    RDF rdf = RDF::fromString(rdf_str, "turtle");

    // collect the link from "https://www.ebi.ac.uk/biomodels/BIOMD0000000308#Files"
    std::string tyson2003 = "https://www.ebi.ac.uk/biomodels/model/download/BIOMD0000000308.2?filename=BIOMD0000000308_url.xml";

    // And now add to it from a uri
    rdf.addFromUri(tyson2003, "rdfxml");

    // serialize to turtle and output to console
    std::string tyson2003_turtle = rdf.toString("turtle");
    std::cout << "The tyon2003 model annotations in turtle syntax is\n\n" << tyson2003_turtle << std::endl;
    return 0;
}