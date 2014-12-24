#   Minimum cmake version required
cmake_minimum_required (VERSION 2.6)

#   Define the project name
project (soapybrain)

#   The version number.
set (VERSION_MAJOR 0)
set (VERSION_MINOR 3)
set (VERSION "${VERSION_MAJOR}.${VERSION_MINOR}")

set (OCULUS_SUPPORT off CACHE BOOL OFF)

add_subdirectory ("src")
add_subdirectory ("soapybrain-examples")
