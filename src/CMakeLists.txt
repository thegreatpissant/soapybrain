project(libsoapybrain)

set (EXTRA_LIBS)
set (EXTRA_INCLUDES)

#   Look for OpenGL
find_package(OpenGL REQUIRED)
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)
set ( EXTRA_INCLUDES ${EXTRA_INCLUDES} ${OPENGL_INCLUDE_DIR} )
#set ( EXTRA_LIBS ${EXTRA_LIBS} ${OPENGL_LIBRARIES} )

#   Look for GLEW
find_package(GLEW REQUIRED)
if(NOT GLEW_FOUND)
        message(ERROR " GLEW not found!")
endif(NOT GLEW_FOUND)
set ( EXTRA_INCLUDES ${EXTRA_INCLUDES} ${GLEW_INCLUDE_DIRS} )
#set ( EXTRA_LIBS ${EXTRA_LIBS} ${GLEW_LIBRARIES} )

#   Look for GLUT
find_package(GLUT REQUIRED)
add_definitions(${GLUT_DEFINITIONS})
if(NOT GLUT_FOUND)
    message(ERROR " GLUT not found!")
endif(NOT GLUT_FOUND)
set ( EXTRA_INCLUDES ${EXTRA_INCLUDES} ${GLUT_INCLUDE_DIRS} )
#set ( EXTRA_LIBS ${EXTRA_LIBS} ${GLUT_LIBRARY} )

#  Use the C++11 std
add_definitions(-std=c++11)

# Set headers and source
set (${PROJECT_NAME}_headers ./Actor.hpp ./Camera.hpp ./Display.hpp ./Entity.hpp ./Model.hpp ./Model_vbotorus.hpp ./Render.hpp ./Shader.hpp )
set (${PROJECT_NAME}_sources ./Actor.cpp ./Display.cpp ./Entity.cpp ./Model_vbotorus.cpp ./Render.cpp ./Shader.cpp shader_utils.cpp)

#  Define the library and its dependencies
add_library( Soapybrain SHARED ${${PROJECT_NAME}_headers} ${${PROJECT_NAME}_sources} )

#target_link_libraries( Soapybrain ${EXTRA_LIBS} )
target_include_directories ( Soapybrain PUBLIC ${EXTRA_INCLUDES} )
