#   Minimum cmake version required
cmake_minimum_required (VERSION 2.6)

#   Define the project name
project (soapybrain)

#   The version number.
set (soapybrain_VERSION_MAJOR 1)
set (soapybrain_VERSION_MINOR 0)

add_subdirectory ("src")
add_subdirectory ("opengl-programming-guide")

#   !!EXAMPLE ONLY!!
#   configure a header file to pass some of the cmake settings
#   to the source code
# configure_file (
	#"${PROJECT_SOURCE_DIR}/exj_x.h.in"
	#"${PROJECT_SOURCE_DIR}/exj_x.h"
#)


