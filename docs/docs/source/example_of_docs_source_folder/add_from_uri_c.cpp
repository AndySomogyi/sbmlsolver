#include "omexmeta/OmexMetaCApi.h" // omexmeta::RDF
#include <cstdio>     // printf

using namespace omexmeta;

int main(){

    const char* rdf_str = "@prefix rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> .\n"
                          "@prefix bqbiol: <http://biomodels.net/biology-qualifiers/> .\n"
                          "@prefix OMEXlib: <http://omex-library.org/> .\n"
                          "@prefix myOMEX: <http://omex-library.org/Welsh2020.omex/> .\n"
                          "@prefix local: <http://omex-library.org/Welsh2020.omex/Welsh2020.rdf#> .\n"
                          "\n"
                          "<http://omex-library.org/Welsh2020.omex/Welsh2020.xml#OmexMetaId0000>\n"
                          "   bqbiol:is <https://identifiers.org/uniprot/PD12345> .\n";

    // first create a RDF graph, by any means available - here we choose from_string
    RDF* rdf_ptr = RDF_fromString(rdf_str, "turtle");

    // collect the link from "https://www.ebi.ac.uk/biomodels/BIOMD0000000308#Files"
    const char* tyson2003 = "https://www.ebi.ac.uk/biomodels/model/download/BIOMD0000000308.2?filename=BIOMD0000000308_url.xml";

    // And now add to it from a uri
    RDF_addFromUri(rdf_ptr, tyson2003, "rdfxml");

    // Serialize the graph
    char* turtle_tyson_string = RDF_toString(rdf_ptr, "turtle");

    // write to console
    printf("The tyon2003 model annotations in turtle syntax is:\n\n%s", turtle_tyson_string);

    // free string
    free(turtle_tyson_string);

    RDF_delete(rdf_ptr);
    return 0;
}