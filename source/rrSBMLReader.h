/*
 * rrRoadRunnerOptions.h
 *
 *  Created on: Oct 10, 2013
 *      Author: andy
 */

#ifndef RRSBMLREADER_H_
#define RRSBMLREADER_H_

#include "rrExporter.h"
#include <string>

namespace rr
{

/**
 * Read an sbml document from either disk, a remote url, or as a std::string.
 *
 * This function will first try to iterperet the std::string as uri, if that
 * fails, it will try as a local file path, and finally if that fails, it will
 * treat the std::string as whole sbml document and return the std::string.
 *
 * This is simple enough that it should belong in RoadRunner proper, however we currently
 * use Poco for reading http streams, and it seems to compilation issues if "windows.h"
 * is included before, an evidently some other roadrunner file includes windows.h,
 * so we put all of this here by itself we we can make sure that there is no
 * nasty windows files included.
 *
 * also, we put it as a static class method because libSBML already uses 'readSBML'
 * and we want to avoid namespace collisions.
 */
class RR_DECLSPEC SBMLReader
{
public:

    /**
     * read an SBML document from a local file path, a remote URI, or
     * directlly from a sbml std::string. If the document is a local file,
     * and has the comp extension, it is automatically flattened before
     * begin returned.
     *
     * If the std::string is already a SBML std::string, it is passed through.
     * If the std::string is a local file or URI, the document is read
     * from the source and the contents returned (and flattened if comp).
     */
    static std::string read(const std::string& sbml_or_uri);

    static bool is_sbml(const std::string& str);

};

}

#endif

