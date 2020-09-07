#include "omexmeta/OmexMetaCApi.h" // omexmeta::RDF
#include <cstdio> // printf

using namespace omexmeta;

int main(){

    // collect the link from "https://www.ebi.ac.uk/biomodels/BIOMD0000000308#Files"
    const char* model_uri = "https://www.ebi.ac.uk/biomodels/model/download/BIOMD0000000308.2?filename=BIOMD0000000308_url.xml";

    // download the model, scan document for annotations and docs-build a graph
    RDF *rdf_ptr = RDF_fromUri(model_uri, "rdfxml");

    // Serialize the graph
    char* turtle_tyson_string = RDF_toString(rdf_ptr, "turtle");

    // write to console
    printf("The tyon2003 model annotations in turtle syntax is:\n\n%s", turtle_tyson_string);

    // free string
    free(turtle_tyson_string);

    RDF_delete(rdf_ptr);

    return 0;

}






