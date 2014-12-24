#   Minimum cmake version required
cmake_minimum_required (VERSION 2.6)

#   Define the project name
project (soapybrain)

#   The version number.
set (VERSION_MAJOR 0)
set (VERSION_MINOR 3)
set (VERSION "${VERSION_MAJOR}.${VERSION_MINOR}")

add_subdirectory ("src")
add_subdirectory ("soapybrain-examples")

#   !!EXAMPLE ONLY!!
#   configure a header file to pass some of the cmake settings
#   to the source code
# configure_file (
	#"${PROJECT_SOURCE_DIR}/exj_x.h.in"
	#"${PROJECT_SOURCE_DIR}/exj_x.h"
#)


