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
add_definitions(-g -std=c++11)

# Set headers and source
set (${PROJECT_NAME}_headers ./Actor.h ./Camera.h ./Display.h ./Entity.h ./Model.h ./Model_vbotorus.h ./Render.h ./Shader.h )
set (${PROJECT_NAME}_sources ./Actor.cpp ./Display.cpp ./Entity.cpp ./Model_vbotorus.cpp ./Render.cpp ./Shader.cpp shader_utils.cpp)

#  Define the library and its dependencies
add_library(EngineCommon ${${PROJECT_NAME}_headers} ${${PROJECT_NAME}_sources})

target_link_libraries(EngineCommon ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARIES})


