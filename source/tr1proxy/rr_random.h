// == PREAMBLE ================================================

// * Licensed under the Apache License, Version 2.0; see README

// == FILEDOC =================================================

/** @file rr_random.h
  * @author JKM
  * @date 12/15/2014
  * @copyright Apache License, Version 2.0
  * @brief A proxy for maintaining compatibility across C++ std library versions
  * @details C++11 moved tr1 headers and classes into the standard namespace,
  * hence no use for a tr1 prefix.
**/

# if __APPLE__
  // is Mavericks
  # if MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_9
    # include <random>
  # else
    # include <tr1/random>
  # endif                                                   // OSX ver
# else                                                      // not __APPLE__
  # if (__cplusplus >= 201103L) || defined(_MSC_VER)
    # include <random>
  # else
    # include <tr1/random>
  # endif
# endif                                                     // __APPLE__

