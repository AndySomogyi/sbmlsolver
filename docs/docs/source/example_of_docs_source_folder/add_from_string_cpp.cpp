#include "omexmeta/RDF.h" // omexmeta::RDF
#include <iostream>     // std::cout, std::endl

using namespace omexmeta;

int main(){

    std::string rdf_str1 = "@prefix rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> .\n"
                           "@prefix OMEXlib: <http://omex-library.org/> .\n"
                           "@prefix myOMEX: <http://omex-library.org/NewOmex.omex/> .\n"
                           "@prefix local: <http://omex-library.org/NewOmex.omex/NewModel.rdf#> .\n"
                           "\n"
                           "<http://omex-library.org/NewOmex.omex/NewModel.xml#>\n"
                           "      <http://purl.org/dc/terms/creator> <https://orchid.org/1234-1234-1234-1234> .\n";


    std::string rdf_str2 = "@prefix rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> .\n"
                           "@prefix bqbiol: <http://biomodels.net/biology-qualifiers/> .\n"
                           "@prefix OMEXlib: <http://omex-library.org/> .\n"
                           "@prefix myOMEX: <http://omex-library.org/NewOmex.omex/> .\n"
                           "@prefix local: <http://omex-library.org/NewOmex.omex/NewModel.rdf#> .\n"
                           "\n"
                           "<http://omex-library.org/NewOmex.omex/NewModel.xml#OmexMetaId0000>\n"
                           "    bqbiol:is <https://identifiers.org/uniprot/PD12345> .\n";

    // read the first string into our RDF graph
    RDF rdf = RDF::fromString(rdf_str1, "turtle");
    // Add to our RDF graph by reading the second string
    rdf.addFromString(rdf_str2, "turtle");

    // serialize to rdfxml abbreviated
    std::string rdfxml_abbrev_string = rdf.toString("rdfxml-abbrev");

    std::cout << "RDF graph serialized to rdfxml abbreviated is:\n\n" << rdfxml_abbrev_string << std::endl;

    return 0;
}
