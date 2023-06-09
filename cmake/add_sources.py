#!/usr/bin/env python
#

import sys
import os
import string
import os.path
import re
from optparse import OptionParser

HELP_USAGE = "Usage: add_sources.py\n"

__usage__ = "%prog [-h --help]"
__description__ = "Finds all source files '.cc', '.h', '.ui' and '.qrc' in subdirectories of 'src/' and adds to "\
                  "corresponding 'CMakeLists.txt' files.\n"

GPLATES_SUB_DIRECTORIES = [
    'api',
    'app-logic',
    'canvas-tools',
    'cli',
    'data-mining',
    'feature-visitors',
    'file-io',
    'global',
    'gui',
    'maths',
    'model',
    'opengl',
    'presentation',
    'property-values',
    'qt-resources',
    'qt-widgets',
    'scribe',
    'unit-test',
    'utils',
    'view-operations'
]

#
# A source file is included if it:
# - matches the regular expression pattern in FILE_INCLUDE_REGEXP
#    *and*
# - does not match the regular expression pattern in FILE_EXCLUDE_REGEXP.
#
# What constitutes a source file.
FILE_INCLUDE_REGEXP = r'\S+\.cc$|\S+\.h$|\S+\.ui$|\S+\.qrc$'
# What of the above pattern to exclude from source file list.
FILE_EXCLUDE_REGEXP = r'\S+Ui\.h$|^moc_\S+\.cc|_pch\.h$|^qrc_\S+\.cc$|\S+_test\.(h|cc)$|^SubversionInfo.cc$'


def get_existing_sources(directory, file_include_regexp, file_exclude_regexp):
    srcs_list = []
    for file in os.listdir(directory):
        file_include_match = file_include_regexp.search(file)
        if file_include_match:
            file_exclude_match = file_exclude_regexp.search(file)
            if not file_exclude_match:
                srcs_list.append(file)
                
    return srcs_list

def get_sources(file_contents, srcs_regexp, directory, file_include_regexp, file_exclude_regexp):
    srcs_match = srcs_regexp.search(file_contents)
    if srcs_match:
        # Unique list of source files.
        srcs_list = set()
        
        # Get a list of source files that match in the specified directory.
        srcs_list.update( get_existing_sources(directory, file_include_regexp, file_exclude_regexp) )
        
        # Print out list of source files in format used by CMakeLists.txt file.
        # Keep list sorted (case-insensitive).
        srcs_text = 'set(srcs\n'
        for src_file in sorted(srcs_list, key=str.lower):
            srcs_text += '    ' + src_file + '\n'
        srcs_text += ')'
        
        # Replace source file list in existing CMakeLists.txt file.
        file_contents = srcs_regexp.sub(srcs_text, file_contents, 1)
    return file_contents

def add_sources_to_cmake_lists_file(cmake_lists_filename, directory, name):
    # Read 'CMakeLists.txt' file.
    fread = open(cmake_lists_filename, 'r')
    file_contents = fread.read()
    fread.close()
    
    # Search for 'set(srcs ... )' in file.
    srcs_regexp  = re.compile(r'set\s*\(srcs[^\)]*\)', re.IGNORECASE)

    # What constitutes a source file.
    file_include_regexp = re.compile(FILE_INCLUDE_REGEXP)
    
    # What of the above pattern to exclude from source file list.
    file_exclude_regexp = re.compile(FILE_EXCLUDE_REGEXP)
    
    # Modify file contents with new 'set(src ... )' expression.
    file_contents = get_sources(file_contents, srcs_regexp, directory, file_include_regexp, file_exclude_regexp)

    # Write new contents to same file.
    fwrite = open(cmake_lists_filename, 'w')
    fwrite.write(file_contents)
    fwrite.close()

def add_sources(gplates_src_dir):
    for subdir in GPLATES_SUB_DIRECTORIES:
        abs_subdir = os.path.join(gplates_src_dir, subdir);
        cmake_lists_filename = os.path.join(abs_subdir, 'CMakeLists.txt')
        add_sources_to_cmake_lists_file(cmake_lists_filename, abs_subdir, subdir)

def main(argv):
    # Parse the command-line options. 
    parser = OptionParser(usage = __usage__,
                          description = __description__)
    
    # Parse command-line options.
    (options, args) = parser.parse_args()
    if len(args) > 0:
        parser.error("incorrect number of arguments")

    # Get the GPlates 'src/' directory from the location of this script (in 'cmake' directory).
    gplates_src_dir = os.path.join( os.path.dirname( \
        os.path.dirname( os.path.abspath(argv[0]) ) ), 'src')
    
    add_sources(gplates_src_dir)

if __name__ == "__main__":
    main( sys.argv )
