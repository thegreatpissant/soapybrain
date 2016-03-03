set (project soapybrain-examples)

set (VERSION_MAJOR 0)
set (VERSION_MINOR 3)
set (VERSION ${VERSION_MAJOR}.${VERSION_MINOR})


#  Bring in our own modules
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMakeModules")

set (EXTRA_LIBS)
set (EXTRA_INCLUDES)

#   Look for OpenGL
find_package(OpenGL REQUIRED)
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)
set ( EXTRA_INCLUDES ${EXTRA_INCLUDES} ${OPENGL_INCLUDE_DIR} )
set ( EXTRA_LIBS ${EXTRA_LIBS} ${OPENGL_LIBRARIES} )

#   Look for GLEW
find_package(GLEW REQUIRED)
if(NOT GLEW_FOUND)
        message(ERROR " GLEW not found!")
endif(NOT GLEW_FOUND)
set ( EXTRA_INCLUDES ${EXTRA_INCLUDES} ${GLEW_INCLUDE_DIRS} )
set ( EXTRA_LIBS ${EXTRA_LIBS} ${GLEW_LIBRARIES} )

#   Look for GLUT
find_package(GLUT REQUIRED)
add_definitions(${GLUT_DEFINITIONS})
if(NOT GLUT_FOUND)
    message(ERROR " GLUT not found!")
endif(NOT GLUT_FOUND)
set ( EXTRA_INCLUDES ${EXTRA_INCLUDES} ${GLUT_INCLUDE_DIRS} )
set ( EXTRA_LIBS ${EXTRA_LIBS} ${GLUT_LIBRARY} )

find_package(Threads)
find_package(X11)

#   Use the C++11 std
add_definitions( -std=c++11 )

#   Define our dependence on the Soapybrain lib
set (EXTRA_LIBS Soapybrain ${EXTRA_LIBS} )
set (EXTRA_INCLUDES ${EXTRA_INCLUDES} ${SOAPYBRAIN_INCLUDES} )

#   Pass in our shader dir
#   TODO Copy the shaders localy to the build directory.
add_definitions( -D__SOAPYBRAIN_SHADER_DIR__=\"${SOAPYBRAIN_SHADER_DIR}\" )

#   Standard examples
add_executable (exj_5 exj_5.cpp ${RES_FILES} )
target_link_libraries (exj_5 LINK_PUBLIC ${EXTRA_LIBS} )
target_include_directories (exj_5 PUBLIC ${EXTRA_INCLUDES} )

#  Oculus Examples
IF (OCULUS_SUPPORT)
# Oculus Package dependencies
find_package(UDev)
#   Set up OCULUS Locations and Variables
set (OCULUS_SDK_DIR "$ENV{HOME}/src/OculusSDKv3.2")
#set (OCULUS_SDK_DIR "$ENV{HOME}/src/ovr_sdk_linux_0.4.3")
set (OCULUS_LIBS ${OCULUS_LIBS} "${OCULUS_SDK_DIR}/LibOVR/Lib/Linux/Debug/x86_64/libovr.a")
set (OCULUS_LIBS ${OCULUS_LIBS} ${CMAKE_THREAD_LIBS_INIT} ${UDEV_LIBARIES} ${X11_LIBRARIES})
set (OCULUS_INCLUDES ${OCLUS_INCLUDES} "${OCULUS_SDK_DIR}/LibOVR/Src")

#   Oculus applications
add_executable (oculus_scafolding oculus_scafolding.cpp)
add_executable (ex_oculus ex_oculus.cpp)
add_executable (ex_texture_render ex_texture_render.cpp)

#   Oculus Includes
target_include_directories ( oculus_scafolding PUBLIC ${OCULUS_INCLUDES})
target_include_directories ( ex_oculus PUBLIC ${OCULUS_INCLUDES})
target_include_directories ( ex_texture_render PUBLIC ${OCULUS_INCLUDES})

#   Oculus Linking flags
#   Until we get these to link as part of our cmake modules
SET_TARGET_PROPERTIES(oculus_scafolding PROPERTIES LINK_FLAGS "-ludev -lXrandr")
SET_TARGET_PROPERTIES(ex_oculus PROPERTIES LINK_FLAGS "-ludev -lXrandr")
SET_TARGET_PROPERTIES(ex_texture_render PROPERTIES LINK_FLAGS "-ludev -lXrandr")

#   Oculus linking
target_link_libraries (oculus_scafolding ${OCULUS_LIBS} ${EXTRA_LIBS} )
target_link_libraries (ex_oculus ${OCULUS_LIBS} ${EXTRA_LIBS} )
target_link_libraries (ex_texture_render ${OCULUS_LIBS} ${EXTRA_LIBS} )
ENDIF(OCULUS_SUPPORT)
