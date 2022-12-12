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
set ( EXTRA_INCLUDES ${EXTRA_INCLUDES} OpenGL::GL )
set ( EXTRA_LIBS ${EXTRA_LIBS} OpenGL::GL )

#   Look for GLEW
find_package(GLEW REQUIRED)
if(NOT GLEW_FOUND)
        message(ERROR " GLEW not found!")
endif(NOT GLEW_FOUND)
set ( EXTRA_INCLUDES ${EXTRA_INCLUDES} GLEW::GLEW )
set ( EXTRA_LIBS ${EXTRA_LIBS} GLEW::GLEW )

#   Look for GLUT
find_package(GLUT REQUIRED)
add_definitions(${GLUT_DEFINITIONS})
if(NOT GLUT_FOUND)
    message(ERROR " GLUT not found!")
endif(NOT GLUT_FOUND)
set ( EXTRA_INCLUDES ${EXTRA_INCLUDES} GLUT::GLUT )
set ( EXTRA_LIBS ${EXTRA_LIBS} GLUT::GLUT )


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