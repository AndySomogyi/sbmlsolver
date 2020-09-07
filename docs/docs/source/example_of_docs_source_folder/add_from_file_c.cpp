#include "omexmeta/OmexMetaCApi.h"

using namespace omexmeta;

int writeToFile(const char* fname);

int writeToFile(const char* fname) {
    const char *rdf_str = "@prefix rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> .\n"
                          "@prefix bqbiol: <http://biomodels.net/biology-qualifiers/> .\n"
                          "@prefix OMEXlib: <http://omex-library.org/> .\n"
                          "@prefix myOMEX: <http://omex-library.org/NewOmex.omex/> .\n"
                          "@prefix local: <http://omex-library.org/NewOmex.omex/NewModel.rdf#> .\n"
                          "\n"
                          "<http://omex-library.org/NewOmex.omex/NewModel.xml#OmexMetaId0000>\n"
                          "     bqbiol:is <https://identifiers.org/uniprot/PD12345> .\n";

    FILE *fp;
    int i;
    /* open the file for writing*/
    fp = fopen(fname, "w");

    for (i = 0; i < 10; i++) {
        fprintf(fp, "%s", rdf_str);
    }

    /* close the file*/
    fclose(fp);
    return 0;
}

int main() {

    const char *fname = "/mnt/d/libOmexMeta/docs/turtle_string.rdf"; // change for your own system
    writeToFile(fname);

    // create an RDF graph so we have something to add to
    RDF* rdf_ptr = RDF_new();

    // Add to our RDF graph
    RDF_addFromFile(rdf_ptr, fname, "turtle");// assume the content of annotation_file is turtle

    // generate some output
    char*rdfxml_string = RDF_toString(rdf_ptr, "rdfxml-abbrev");

    // print to console
    printf("%s", rdfxml_string);

    // free dynamically generated output string
    free(rdfxml_string);

    // clear up the file we wrote
    remove(fname);

    // clear up rdf
    RDF_delete(rdf_ptr);

    return 0;
}