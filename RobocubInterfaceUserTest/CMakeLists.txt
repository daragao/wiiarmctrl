# Generated by "yarp cmake"
cmake_minimum_required(VERSION 2.6)


# A cmake file to get started with for new YARP projects.
# It assumes you want to build an executable from source code in 
# the current directory.
# Replace "yarpy" with whatever your executable should be called.
# There's some stuff to make compiling devices easier too.

SET(KEYWORD "WiiICubInterfaceUserTest")

MESSAGE( "Start of " ${KEYWORD})

# Start a project.
PROJECT(${KEYWORD})

FIND_PACKAGE(GTK2 REQUIRED gtk glade)
INCLUDE_DIRECTORIES(${GTK2_INCLUDE_DIRS})
#LINK_LIBRARIES(${GTK2_LIBRARIES})

# Find packages!
FIND_PACKAGE(YARP)
#LINK_LIBRARIES(${YARP_LIBRARIES})
#INCLUDE_DIRECTORIES(${YARP_INCLUDE_DIRS})


FIND_PACKAGE(ICUB)
#INCLUDE_DIRECTORIES(${ICUB_INCLUDE_DIRS})
#LINK_LIBRARIES(icubmod)

FIND_PACKAGE(GLUT)
FIND_PACKAGE(OpenGL)

SET(CORELIBS ${GTK2_LIBRARIES} ${YARP_LIBRARY} icubmod ${OPENGL_LIBRARY} ${GLUT_LIBRARY})

# Search for source code.
FILE(GLOB folder_source *.cpp *.cc *.c)
FILE(GLOB folder_header *.h)
SOURCE_GROUP("Source Files" FILES ${folder_source})
SOURCE_GROUP("Header Files" FILES ${folder_header})

# Automatically add include directories if needed.
FOREACH(header_file ${folder_header})
  GET_FILENAME_COMPONENT(p ${header_file} PATH)
ENDFOREACH(header_file ${folder_header})

# Set up our main executable.
IF (folder_source)
  ADD_EXECUTABLE(${KEYWORD} ${folder_source} ${folder_header})
ELSE (folder_source)
  MESSAGE(FATAL_ERROR "No source code files found. Please add something")
ENDIF (folder_source)

#TARGET_LINK_LIBRARIES(${KEYWORD} ${YARP_LIBRARIES} ${GTK2_LIBRARIES} icubmod)
TARGET_LINK_LIBRARIES(${KEYWORD} ${CORELIBS})
