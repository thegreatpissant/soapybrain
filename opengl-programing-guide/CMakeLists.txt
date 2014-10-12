#   Minimum cmake version required
cmake_minimum_required (VERSION 2.6)

#   Define the project name
project (opengl-programing-guide)

#   The version number.
set (opengl-programing-guide_VERSION_MAJOR 1)
set (opengl-programing-guide_VERSION_MINOR 0)

#   Look for our dependencies
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLUT REQUIRED)

#   !!EXAMPLE ONLY!!
#   configure a header file to pass some of the cmake settings
#   to the source code
configure_file (
	"${PROJECT_SOURCE_DIR}/exj_x.h.in"
	"${PROJECT_SOURCE_DIR}/exj_x.h"
)

#   Add the shader files to the project space
file (GLOB RES_FILES shaders/*.frag shaders/*.vert shaders/*.glsl)

#   Ensure We include Everything we need
include_directories ("${PROJECT_BINARY_DIR}")
include_directories ("${PROJECT_SOURCE_DIR}/common")

#   Suck in the EngineCommon lib
add_subdirectory (common)

#   Define our dependence on the Enginecommon lib
set (EXTRA_LIBS ${EXTRA_LIBS} EngineCommon)
#   Define our dependence on OpenGL GLEW and GLUT
set (EXTRA_LIBS ${EXTRA_LIBS} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${GLUT_LIBRARY})

#   Use the C++11 std
add_definitions(-std=c++11)

#   Set up the Executables
add_executable (exj_5 exj_5.cpp ${RES_FILES})
add_executable (triangles triangles.cpp ${RES_FILES})
add_executable (ex15_1 ex15_1.cpp ${RES_FILES})
add_executable (glm_tests glm_tests.cpp)

#   Set up the libs for our executables
target_link_libraries (exj_5 ${EXTRA_LIBS})
target_link_libraries (triangles ${EXTRA_LIBS})
target_link_libraries (ex15_1 ${EXTRA_LIBS})



