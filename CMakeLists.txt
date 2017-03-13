cmake_minimum_required(VERSION 2.8.12)
# Project Name
PROJECT(Firework)

#########################################################
# FIND GLUT
#########################################################
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})
if(NOT GLUT_FOUND)
	  message(ERROR " GLUT not found!")
endif(NOT GLUT_FOUND)

#########################################################
# FIND OPENGL
#########################################################
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
	  message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

#########################################################
# Include Files
#########################################################
add_compile_options(-std=c++11 -Wall)
add_executable(test main.cpp particle.cpp fireworks_2d_opengl.cpp)

########################################################
# Linking & stuff
#########################################################
target_link_libraries(test ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )
