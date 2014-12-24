#   Minimum cmake version required
cmake_minimum_required (VERSION 2.6)

#   Define the project name
project (soapybrain)

#   The version number.
set (VERSION_MAJOR 0)
set (VERSION_MINOR 3)
set (VERSION "${VERSION_MAJOR}.${VERSION_MINOR}")

set (OCULUS_SUPPORT off CACHE BOOL OFF)
set (EXAMPLES_BUILD off CACHE BOOL OFF)

add_subdirectory ("src")

IF (EXAMPLES_BUILD)
add_subdirectory ("soapybrain-examples")
ENDIF (EXAMPLES_BUILD)

#  Export our include location for including projects
set (SOAPYBRAIN_INCLUDES "${PROJECT_SOURCE_DIR}/src" PARENT_SCOPE)
