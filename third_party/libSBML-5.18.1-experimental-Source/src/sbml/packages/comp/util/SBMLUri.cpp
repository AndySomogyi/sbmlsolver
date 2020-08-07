/**
 * @file    SBMLUri.cpp
 * @brief   Implementation of SBMLUri, the utility class for handling uris.
 * @author  Frank Bergmann
 *
 * <!--------------------------------------------------------------------------
 * This file is part of libSBML.  Please visit http://sbml.org for more
 * information about SBML, and the latest version of libSBML.
 *
 * Copyright (C) 2019 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. University of Heidelberg, Heidelberg, Germany
 *
 * Copyright (C) 2013-2018 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 *     3. University of Heidelberg, Heidelberg, Germany
 * 
 * Copyright 2011-2012 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.  A copy of the license agreement is provided
 * in the file named "LICENSE.txt" included with this software distribution
 * and also available online as http://sbml.org/software/libsbml/license.html
 * ------------------------------------------------------------------------ -->
 */

#include <sbml/packages/comp/util/SBMLUri.h>
#include <sbml/SBMLDocument.h>
#include <sbml/SBMLConstructorException.h>

#ifdef __cplusplus

#include <cctype>
#include <functional>
#include <iterator>
#include <string>
#include <algorithm>

using namespace std;
LIBSBML_CPP_NAMESPACE_BEGIN

SBMLUri::SBMLUri (const std::string& uri)
{
  parse(uri);
}


/*
 * Copy constructor.
 */
SBMLUri::SBMLUri(const SBMLUri& orig)
 : mScheme (orig.mScheme)
 , mHost   (orig.mHost)
 , mPath   (orig.mPath)
 , mQuery  (orig.mQuery)
 , mUri    (orig.mUri)
{
}


/*
 * Destroy this object.
 */
SBMLUri::~SBMLUri ()
{

}


/*
 * Assignment operator for SBMLUri.
 */
SBMLUri&
SBMLUri::operator=(const SBMLUri& rhs)
{
  if(&rhs!=this)
  {
    mScheme = rhs.mScheme;
    mHost = rhs.mHost;
    mPath = rhs.mPath;
    mQuery = rhs.mQuery;
    mUri = rhs.mUri;
  }

  return *this;
}

/*
 * Assignment operator for strings.
 */
SBMLUri&
SBMLUri::operator=(const std::string& uri)
{
  parse(uri);
  return *this;
}


SBMLUri*
SBMLUri::clone () const
{
  return new SBMLUri(*this);
}

struct replace_back_slash
{
void operator()(char& c) { if(c == '\\') c = '/'; }
};

/*
 * adapted from: http://stackoverflow.com/questions/2616011/easy-way-to-parse-a-url-in-c-cross-platform
 */
void 
SBMLUri::parse(const std::string& uri)
{
  mScheme = "";
  mHost = "";
  mQuery = "";
  mPath = "";
  mUri = uri;
  
  std::for_each( mUri.begin(), mUri.end(), replace_back_slash() );

  const std::string constUri(mUri);

  string prot_end("://");
  string::const_iterator prot_i = search(constUri.begin(), constUri.end(),
    prot_end.begin(), prot_end.end());
  if (prot_i == constUri.end())
  {
    // not found ... 
    prot_end = ":";
    prot_i = search(constUri.begin(), constUri.end(),
    prot_end.begin(), prot_end.end());
    if (prot_i == constUri.end() || prot_i == constUri.begin() + 1)
    {
      // if we still have not found it assume this is a file uri
      mScheme = "file";
      mPath = constUri ;
      mUri = mScheme + ":///" + mPath; 
      return;
    }    
  }
  
  mScheme.reserve((size_t)distance(constUri.begin(), prot_i));
#ifdef __BORLANDC__
  transform(constUri.begin(), prot_i,
    back_inserter(mScheme),
    (int(*)(int))(tolower)); // scheme is icase
#else
  transform(constUri.begin(), prot_i,
    back_inserter(mScheme),
    ptr_fun<int,int>(tolower)); // scheme is icase
#endif

  if( prot_i == constUri.end() )
  {
    return;
  }

#ifdef __BORLANDC__
  advance(prot_i, prot_end.length());
#else
  advance(prot_i, (string::const_iterator::difference_type) prot_end.length());
#endif
  if ((prot_i + 1) != constUri.end() && *(prot_i + 1) == ':')
  {
    // turns out there are invalid urls being used internally, of the form 
    // file:drive:/ ... this is just plain wrong but needs to be parsed correctly
    // 
    mPath.reserve((size_t)distance(prot_i, constUri.end()));
#ifdef __BORLANDC__
    mPath = std::string(prot_i, constUri.end());
#else
    mPath.assign(prot_i, constUri.end());
#endif
    // but we ought to fix the URI!
    mUri = mScheme + ":///" + mPath;
    return;
  }

  string::const_iterator path_i = find(prot_i, constUri.end(), '/');
  if (mScheme != "file" && mScheme != "urn")
  {
    // file won't have a host (or could assume localhost)
    mHost.reserve((size_t)distance(prot_i, path_i));
#ifdef __BORLANDC__
    transform(prot_i, path_i,
     back_inserter(mHost),
     (int(*)(int))(tolower)); // host is icase
#else
    transform(prot_i, path_i,
     back_inserter(mHost),
     ptr_fun<int,int>(tolower)); // host is icase
#endif
  }  
  else if (mScheme == "urn")
  {
    // special handling for the miriam scheme, as it is something we ought
    // to support
    size_t pos = constUri.rfind(':');
    mScheme = constUri.substr(0,pos);
    mPath = constUri.substr(pos+1, constUri.length() - pos +1);    
    return;
  }
  else
  {
#ifdef __BORLANDC__
    mPath = std::string(prot_i, path_i);
#else
    mPath.assign(prot_i, path_i);
#endif
    if (mPath.size() > 0 && mPath[0] == '/')
    {
#ifdef __BORLANDC__
      mPath = std::string(mPath.begin() +1, mPath.end());
#else
      mPath.assign(mPath.begin() +1, mPath.end());
#endif
    } 
  }

  if (path_i == constUri.end())
  {
    return;
  }

  string::const_iterator query_i = find(path_i, constUri.end(), '?');
#ifdef __BORLANDC__
  mPath = std::string(path_i, query_i);
#else
  mPath.assign(path_i, query_i);
#endif

  if (mPath.size() > 0 && mPath[0] == '/')
  {
#ifdef __BORLANDC__
	 mPath = std::string(mPath.begin() +1, mPath.end());
#else
      mPath.assign(mPath.begin() +1, mPath.end());
#endif
  }

  if( query_i != constUri.end() )
    ++query_i;

#ifdef __BORLANDC__
  mQuery = std::string(query_i, constUri.end());
#else
  mQuery.assign(query_i, constUri.end());
#endif
}

SBMLUri 
SBMLUri::relativeTo(const std::string& uri) const
{
  SBMLUri other(uri);
  other.mScheme = mScheme;
  other.mHost = mHost;
  bool slashNeeded = ((!other.mPath.empty() && other.mPath[0] != '/') || 
		(!mPath.empty() && !other.mPath.empty() && other.mPath[0] != '/' && mPath[mPath.length() -1 ] != '/') ||
		(!mPath.empty() && other.mPath.empty() && mPath[mPath.length() -1 ] != '/') );

  if (slashNeeded && other.mPath.length() > 2 && other.mPath[1] == ':')
  {
	// the uri is a full path with drive letter
	return other;
  }

  other.mPath = mPath + (slashNeeded  ? "/" : "") + other.mPath;
  other.mUri = mScheme + "://" + mHost + (slashNeeded  ? "/" : "") + other.mPath;
  if (!other.mQuery.empty())
    other.mUri += "?" + other.mQuery;

  return other;
}

const std::string& 
SBMLUri::getScheme() const
{
  return mScheme;
}

const std::string& 
SBMLUri::getHost() const
{
  return mHost;
}

const std::string& 
SBMLUri::getPath() const
{
  return mPath;
}
  
const std::string& 
SBMLUri::getUri() const
{
  return mUri;
}

const std::string& 
SBMLUri::getQuery() const
{
  return mQuery;
}

/** @cond doxygenIgnored */
/** @endcond */

LIBSBML_CPP_NAMESPACE_END

#endif  /* __cplusplus */


