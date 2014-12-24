set (project soapybrain-examples)

set (VERSION_MAJOR 0)
set (VERSION_MINOR 3)
set (VERSION ${VERSION_MAJOR}.${VERSION_MINOR})


#  Bring in our own modules
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMakeModules")

#   Look for our dependencies
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLUT REQUIRED)
find_package(Threads)
find_package(X11)
find_package(UDev)

#   Add the shader files to the project space
file (GLOB RES_FILES shaders/*.frag shaders/*.vert shaders/*.glsl)

#   Set up OCULUS Locations and Variables
set (OCULUS_SDK_DIR "$ENV{HOME}/src/OculusSDKv3.2")
#set (OCULUS_SDK_DIR "$ENV{HOME}/src/ovr_sdk_linux_0.4.3")
set (OCULUS_LIBS ${OCULUS_LIBS} "${OCULUS_SDK_DIR}/LibOVR/Lib/Linux/Debug/x86_64/libovr.a")
set (OCULUS_LIBS ${OCULUS_LIBS} ${CMAKE_THREAD_LIBS_INIT} ${UDEV_LIBARIES} ${X11_LIBRARIES})
set (OCULUS_INCLUDES ${OCLUS_INCLUDES} "${OCULUS_SDK_DIR}/LibOVR/Src")

#   Define our dependence on the Enginecommon lib
set (EXTRA_LIBS ${EXTRA_LIBS} libsoapybrain)
#   Define our dependence on OpenGL GLEW and GLUT
set (EXTRA_LIBS ${EXTRA_LIBS} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${GLUT_LIBRARY})

#   Use the C++11 std
add_definitions(-std=c++11)

#   Set up the Executables
add_executable (exj_5 exj_5.cpp ${RES_FILES})

#   Oculus applications
add_executable (oculus_scafolding oculus_scafolding.cpp)
add_executable (ex_oculus ex_oculus.cpp)
add_executable (ex_texture_render ex_texture_render.cpp)

include_directories (../src)

#   Oculus Includes
target_include_directories ( oculus_scafolding PUBLIC ${OCULUS_INCLUDES})
target_include_directories ( ex_oculus PUBLIC ${OCULUS_INCLUDES})
target_include_directories ( ex_texture_render PUBLIC ${OCULUS_INCLUDES})

#   Oculus Linking flags
#   Until we get these to link as part of our cmake modules
SET_TARGET_PROPERTIES(oculus_scafolding PROPERTIES LINK_FLAGS "-ludev -lXrandr")
SET_TARGET_PROPERTIES(ex_oculus PROPERTIES LINK_FLAGS "-ludev -lXrandr")
SET_TARGET_PROPERTIES(ex_texture_render PROPERTIES LINK_FLAGS "-ludev -lXrandr")

#   Set up the libs for our executables
target_link_libraries (exj_5 ${EXTRA_LIBS})

#   Oculus linking
target_link_libraries (oculus_scafolding ${OCULUS_LIBS} ${EXTRA_LIBS} )
target_link_libraries (ex_oculus ${OCULUS_LIBS} ${EXTRA_LIBS} )
target_link_libraries (ex_texture_render ${OCULUS_LIBS} ${EXTRA_LIBS} )

