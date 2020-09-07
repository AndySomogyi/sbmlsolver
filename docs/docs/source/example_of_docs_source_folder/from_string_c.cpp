#include "omexmeta/OmexMetaCApi.h" // omexmeta::RDF

using namespace omexmeta;

int main(){

    const char* rdf_str = "@prefix rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> .\n"
                          "@prefix OMEXlib: <http://omex-library.org/> .\n"
                          "@prefix myOMEX: <http://omex-library.org/NewOmex.omex/> .\n"
                          "@prefix local: <http://omex-library.org/NewOmex.omex/NewModel.rdf#> .\n"
                          "\n"
                          "<http://omex-library.org/NewOmex.omex/NewModel.xml#>\n"
                          "    <http://purl.org/dc/terms/creator> <https://orchid.org/1234-1234-1234-1234> .\n\n";
    // uses dynamic memory
    RDF *rdf_ptr = RDF_fromString(rdf_str, "turtle");

    // serialize the string to rdfxml
    char* xml_string = RDF_toString(rdf_ptr, "rdfxml-abbrev");
    printf("RDF graph serialized to rdfxml abbreviated is:\n\n%s", xml_string);

    // clean up
    free(xml_string);
    RDF_delete(rdf_ptr);

    return 0;
}
