#   Look for GLUT
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})
if(NOT GLUT_FOUND)
    message(ERROR " GLUT not found!")
endif(NOT GLUT_FOUND)

#   Look for OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

#   Look for GLEW
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
if(NOT GLEW_FOUND)
	message(ERROR " GLEW not found!")
endif(NOT GLEW_FOUND)

#  Use the C++11 std
add_definitions(-std=c++11)

#  Make sure all of these are in the project as source files
set (EXTRA_INCLUDE ${EXTRA_INCLUDE} Model.h Display.h Entity.h Camera.h)

#  Define the library and its dependencies
add_library(EngineCommon Actor.h Actor.cpp Render.h Render.cpp Shader.cpp Display.h Display.cpp shader_utils.cpp Model_vbotorus.cpp ${EXTRA_INCLUDE})
target_link_libraries(EngineCommon ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARIES})


