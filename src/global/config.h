#ifndef GPLATES_GLOBAL_CONFIG_H
#define GPLATES_GLOBAL_CONFIG_H

//
// "src/global/config.h.in":  CMake generates "src/global/config.h"
//
#define BOOST_NO_CXX17_NOEXCEPT_FUNCTION_TYPE

// Define if this is a public release.
#define GPLATES_PUBLIC_RELEASE 1

// // Define if installing GPlates (or pyGPlates) as a standalone bundle (by copying dependency libraries during installation).
// #define GPLATES_INSTALL_STANDALONE 0
#define GPLATES_STANDALONE_PROJ_DATA_DIR "."
#define GPLATES_STANDALONE_GDAL_DATA_DIR "."
/*
 * Whether GPlates compiles in support for profiling CPU usage of source code.
 *
 * Note if the custom build type 'ProfileGplates' has been selected for building then
 * it sets this value directly as a compiler flag (-D), effectively overrriding this.
 * Other build types set it indirectly here via the GPLATES_PROFILE_CODE CMake option
 * that must be manually turned on in the CMake GUI (or via the CMake command-line).
 * 
 * Usually it's easiest to just select the 'ProfileGplates' build type (note that enabling/disabling
 * the option below then has no effect). However, being a custom build type, that sometimes creates problems
 * (eg, the CGAL dependency does not always play nicely with custom build types). In this case you can choose
 * the builtin 'Release' build type (for example) and enable this option via CMake to achieve the same affect.
 */
// #define GPLATES_PROFILE_CODE 0

// // Define if have the <proj.h> header file (Proj5+).
// #define GPLATES_HAVE_PROJ_H 0
// Define if have the <proj_api.h> header file (Proj4).
#define GPLATES_HAVE_PROJ_API_H

// Define if have boost.python.numpy (NumPy extension package of Boost.Python).
// #define GPLATES_HAVE_BOOST_PYTHON_NUMPY

// // Define if have the numpy C-API headers (such as <numpy/arrayobject.h> located in Python's numpy package).
// #define GPLATES_HAVE_NUMPY_C_API

#endif // GPLATES_GLOBAL_CONFIG_H
