#include "omexmeta/RDF.h" // omexmeta::RDF
#include <iostream>     // std::cout, std::endl

using namespace omexmeta;

int main(){

    // collect the link from "https://www.ebi.ac.uk/biomodels/BIOMD0000000308#Files"
    std::string model_uri = "https://www.ebi.ac.uk/biomodels/model/download/BIOMD0000000308.2?filename=BIOMD0000000308_url.xml";

    // download the model, scan document for annotations and docs-build a graph
    RDF rdf = RDF::fromUri(model_uri, "rdfxml");

    // serialize to turtle and output to console
    std::string tyson2003_turtle = rdf.toString("turtle");
    std::cout << "The tyon2003 model annotations in turtle syntax is\n\n" << tyson2003_turtle << std::endl;

    return 0;
}






