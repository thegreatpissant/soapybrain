#   Minimum cmake version required
cmake_minimum_required (VERSION 2.6)

#   Define the project name
project (soapybrain)

#   The version number.
set (VERSION_MAJOR 0)
set (VERSION_MINOR 3)
set (VERSION "${VERSION_MAJOR}.${VERSION_MINOR}")

if ( NOT UNIX )
    MESSAGE ( WARNING "Has been tested only on linux" )
endif (NOT UNIX )
set (OpenGL_GL_PREFERENCE "GLVND")

file (GLOB RES_FILES shaders/*.frag shaders/*.vert shaders/*.glsl)

#  Export our include location for including projects
# TODO : Depricate SOAPYBRAIN_INCLUDES in broception
set (SOAPYBRAIN_INCLUDES "${PROJECT_SOURCE_DIR}/src" )
set (SOAPYBRAIN_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/src" )
set (SOAPYBRAIN_SHADER_DIR "${PROJECT_SOURCE_DIR}/shaders/")

#   Add the shader files to the project space

add_subdirectory ("src")

set (EXAMPLES_BUILD off CACHE BOOL OFF)
IF (EXAMPLES_BUILD)
add_subdirectory ("soapybrain-examples")
ENDIF (EXAMPLES_BUILD)
