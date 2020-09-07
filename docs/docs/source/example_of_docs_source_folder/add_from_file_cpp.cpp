#include "omexmeta/OmexMeta.h" // omexmeta::RDF
#include <iostream>     // std::cout, std::endl
#include <filesystem>

using namespace omexmeta;

int writeToFile(const std::string& fname);

int writeToFile(const std::string& fname){
    std::string rdf_str = "@prefix rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> .\n"
                          "@prefix bqbiol: <http://biomodels.net/biology-qualifiers/> .\n"
                          "@prefix OMEXlib: <http://omex-library.org/> .\n"
                          "@prefix myOMEX: <http://omex-library.org/NewOmex.omex/> .\n"
                          "@prefix local: <http://omex-library.org/NewOmex.omex/NewModel.rdf#> .\n"
                          "\n"
                          "<http://omex-library.org/NewOmex.omex/NewModel.xml#OmexMetaId0000>\n"
                          "     bqbiol:is <https://identifiers.org/uniprot/PD12345> .\n";
    std::ofstream annot_file;
    annot_file.open(fname);
    annot_file << rdf_str;
    annot_file.close();
    return 0;

}

int main(){

    // create an empty rdf graph
    RDF rdf = RDF();

    // note: you need C++17 standard minimum to use std::filesystem
    std::filesystem::path fname = std::filesystem::current_path() /+ "annotation_file.rdf";

    // write some annotations to a file so we can read it in with add from file
    writeToFile(fname.string());

    // Add to our RDF graph
    rdf.addFromFile(fname.string(), "turtle"); // assume the content of annotation_file is turtle

    // clear up file we wrote
    remove(fname.string().c_str());

    std::string xml_abbrev_string = rdf.toString("rdfxml-abbrev");

    // Write rdf graph to rdfxml and print to console
    std::cout << xml_abbrev_string << std::endl;

    // RDF clears up after itself via RAII

    return 0;
}
