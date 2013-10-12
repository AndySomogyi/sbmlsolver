// The poco files have issues on windows and have to be included before 
// any potential 'windows.h' is included. 
#include <Poco/URI.h>
#include <Poco/URIStreamOpener.h>
#include <Poco/Net/HTTPStreamFactory.h>
#include "rrSBMLReader.h"

#include <iostream>

namespace rr {

std::string SBMLReader::read(const std::string& uri)
{

    try
    {
        // not working on windows yet. 
        #if !defined(_WIN32)
        Poco::Net::HTTPStreamFactory::registerFactory();
        #endif

        Poco::URIStreamOpener &opener = Poco::URIStreamOpener::defaultOpener();


        std::istream* stream = opener.open(uri);

        std::istreambuf_iterator<char> eos;
        std::string s(std::istreambuf_iterator<char>(*stream), eos);

        return s;

    }
    catch(std::exception& ex)
    {
        std::cout << "caught exception " << ex.what() << std::endl;
        return ex.what();
    }
}

} /* namespace rr */




