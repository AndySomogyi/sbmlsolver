/*
 * GitInfo.h
 *
 *  Created on: Sep 7, 2013
 *      Author: andy
 *
 * A file that is pre-processed by cmake to generate a GitInfo.h.
 * The generated file is intended to be include only in VersionInfo.cpp
 * as a way to get info about what git branch we are on. 
 * 
 * this has a few static (so they only show up in the current compilation unit)
 * function get return info about the git branch and last commit. 
 */

#ifndef GITINFO_H_
#define GITINFO_H_


static const char* getGitBranch() {
    return "develop-add-subdirectory-dependencies";
}


static const char* getGitLastCommit() {
    return "6635c3a07b64b627350d923a8705cf9774e69581";
}

#endif
