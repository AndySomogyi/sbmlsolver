#include "omexmeta/OmexMetaCApi.h" // omexmeta::RDF
#include <cstdio> // printf

using namespace omexmeta;

int main(){

    const char* rdf_str1 = "@prefix rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> .\n"
                           "@prefix OMEXlib: <http://omex-library.org/> .\n"
                           "@prefix myOMEX: <http://omex-library.org/NewOmex.omex/> .\n"
                           "@prefix local: <http://omex-library.org/NewOmex.omex/NewModel.rdf#> .\n"
                           "\n"
                           "<http://omex-library.org/NewOmex.omex/NewModel.xml>\n"
                           "      <http://purl.org/dc/terms/creator> <https://orchid.org/1234-1234-1234-1234> .\n";


    const char* rdf_str2 = "@prefix rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> .\n"
                           "@prefix bqbiol: <http://biomodels.net/biology-qualifiers/> .\n"
                           "@prefix OMEXlib: <http://omex-library.org/> .\n"
                           "@prefix myOMEX: <http://omex-library.org/NewOmex.omex/> .\n"
                           "@prefix local: <http://omex-library.org/NewOmex.omex/NewModel.rdf#> .\n"
                           "\n"
                           "<http://omex-library.org/NewOmex.omex/NewModel.xml#OmexMetaId0001>\n"
                           "    bqbiol:is <https://identifiers.org/uniprot/PD12345> .\n";

    // read the first string into our RDF graph
    RDF* rdf_ptr = RDF_fromString(rdf_str1, "turtle");

    // Add to our RDF graph by reading the second string
    RDF_addFromString(rdf_ptr, rdf_str2, "turtle");

    // serialize the string to rdfxml
    char* xml_string = RDF_toString(rdf_ptr, "rdfxml-abbrev");
    printf("RDF graph serialized to rdfxml abbreviated is:\n\n%s", xml_string);

    // clean up
    free(xml_string);
    RDF_delete(rdf_ptr);
    return 0;
}
