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
find_package(Threads)
find_package(X11)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMakeModules")
find_package(UDev)

#   !!EXAMPLE ONLY!!
#   configure a header file to pass some of the cmake settings
#   to the source code
# configure_file (
	#"${PROJECT_SOURCE_DIR}/exj_x.h.in"
	#"${PROJECT_SOURCE_DIR}/exj_x.h"
#)

#   Add the shader files to the project space
file (GLOB RES_FILES shaders/*.frag shaders/*.vert shaders/*.glsl)

#   Ensure We include Everything we need
include_directories ("${PROJECT_BINARY_DIR}")
include_directories ("${PROJECT_SOURCE_DIR}/common")

#   Suck in the EngineCommon lib
add_subdirectory (common)

#   Set up OCULUS stuff
set (OCULUS_SDK_DIR "$ENV{HOME}/src/OculusSDKv3.2")
set (OCULUS_LIBS ${OCULUS_LIBS} "${OCULUS_SDK_DIR}/LibOVR/Lib/Linux/Debug/x86_64/libovr.a")
set (OCULUS_LIBS ${OCULUS_LIBS} ${CMAKE_THREAD_LIBS_INIT} ${UDEV_LIBARIES} ${X11_LIBRARIES})
set (OCULUS_INCLUDES ${OCLUS_INCLUDES} "${OCULUS_SDK_DIR}/LibOVR/Src")

#   Define our dependence on the Enginecommon lib
set (EXTRA_LIBS ${EXTRA_LIBS} EngineCommon)
#   Define our dependence on OpenGL GLEW and GLUT
set (EXTRA_LIBS ${EXTRA_LIBS} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${GLUT_LIBRARY})

#   Use the C++11 std
add_definitions(-std=c++11)

#   Set up the Executables
add_executable (exj_5 exj_5.cpp ${RES_FILES})
add_executable (exj_3 exj_3.cpp ${RES_FILES})
add_executable (exj_2 exj_2.cpp ${RES_FILES})
add_executable (triangles triangles.cpp ${RES_FILES})
add_executable (ex15_1 ex15_1.cpp ${RES_FILES})
add_executable (glm_tests glm_tests.cpp)
add_executable (oculus_scafolding oculus_scafolding.cpp)

#   Oculus Stuff
target_include_directories ( oculus_scafolding PUBLIC ${OCULUS_INCLUDES})
#   Until we get these to link as part of our cmake modules
SET_TARGET_PROPERTIES(oculus_scafolding PROPERTIES LINK_FLAGS "-ludev -lXrandr")

#   Set up the libs for our executables
target_link_libraries (exj_5 ${EXTRA_LIBS})
target_link_libraries (exj_3 ${EXTRA_LIBS})
target_link_libraries (exj_2 ${EXTRA_LIBS})
target_link_libraries (triangles ${EXTRA_LIBS})
target_link_libraries (ex15_1 ${EXTRA_LIBS})
#   Oculus stuff
target_link_libraries (oculus_scafolding ${OCULUS_LIBS} ${EXTRA_LIBS} )

