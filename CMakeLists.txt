# MIT License
# 
# Copyright (c) 2019 Liam Daniel Hurt
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

################
# CMAKE RULES! #
################

# Need feature in 3.8 or better
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# Set the compiler to clang++ if not visual studio
if (!MSVC)
	set(CMAKE_CXX_COMPILER clang++)
endif(!MSVC)

# Setting the project to cxx_std_11
# set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Name and Version of the project
project(3DEngineOpenGL LANGUAGES CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if (CMAKE_BUILD_TYPE STREQUAL "")
	# CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up differentiation between debug and releaase builds
	set(CMAKE_BUILD_TYPE
				"Debug"
		CACHE STRING
				"Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) \"Debug\" \"Release\" \"RelWithDebInfo\" \"MinSizeRel\"."
		FORCE
)
endif ()

# Lets LOAD app our headers!
file(GLOB_RECURSE HDRS CONFIGURE_DEPENDS
		${PROJECT_SOURCE_DIR}/src/*.h
		${PROJECT_SOURCE_DIR}/src/*.hpp
)

# Lets LOAD app our sources!
file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS
		${PROJECT_SOURCE_DIR}/src/*.cpp
		${PROJECT_SOURCE_DIR}/src/*.c
)

foreach(FILE ${SRCS} ${HDRS}) 
    # Get the directory of the source file
    get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)

    # Remove common directory prefix to make the group
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/src" "" GROUP "${PARENT_DIR}")

    # Make sure we are using windows slashes
    string(REPLACE "/" "\\" GROUP "${GROUP}")

    # Group into "Source Files" and "Header Files"
    if ("${FILE}" MATCHES ".*\\.cpp")
       set(GROUP "Source Files${GROUP}")
	elseif ("${FILE}" MATCHES ".*\\.c")
       set(GROUP "Source Files${GROUP}")
    elseif("${FILE}" MATCHES ".*\\.h")
       set(GROUP "Header Files${GROUP}")
	elseif("${FILE}" MATCHES ".*\\.hpp")
       set(GROUP "Header Files${GROUP}")
    endif()

    source_group("${GROUP}" FILES "${FILE}")
endforeach(FILE ${SRCS} ${HDRS})

include_directories (.)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
	add_compile_options(${PROJECT_NAME} -02)
endif (CMAKE_BUILD_TYPE STREQUAL "Release")

# Checking if it is in visual studio for correct errors flags
if (MSVC)
	add_compile_options(-c /W4)
else()
	add_compile_options(-c -Wall -Wextra -Werror -Wpedantic)
endif()

# Define the executable
add_executable(${PROJECT_NAME} ${HDRS} ${SRCS})

#if (CMAKE_BUILD_TYPE STREQUAL "Release")
#	target_compile_options(${PROJECT_NAME} -02)
#endif (CMAKE_BUILD_TYPE STREQUAL "Release")

# Checking if it is in visual studio for correct errors flags
#if (MSVC)
#	target_compile_options(${PROJECT_NAME} PUBLIC -c -W4)
#else()
#	target_compile_options(${PROJECT_NAME} PUBLIC -c -Wall -Wextra -Werror -Wpedantic)
#endif()

# We need a CMake_Dir with some code to find external dependencies
set(PROJECT_CMAKE_DIR "${PROJECT_SOURCE_DIR}/cmake")

#######################################
# LOOK for the packages that we need! #
#######################################

# OpenGL
find_package(OpenGL REQUIRED)

# GLEW
include(${PROJECT_CMAKE_DIR}/FindGLEW.cmake)

# SDL2
include(${PROJECT_CMAKE_DIR}/FindSDL2.cmake)

# ASSIMP
include(${PROJECT_CMAKE_DIR}/FindASSIMP.cmake)

# Define the include DIRs
include_directories(
		${PROJECT_SOURCE_DIR}/headers
		${PROJECT_SOURCE_DIR}/sources
		${OPENGL_INCLUDE_DIRS}
		${GLEW_INCLUDE_DIRS}
		${SDL2_INCLUDE_DIRS}
		${ASSIMP_INCLUDE_DIRS}
)

# Define the link libraries
target_link_libraries(${PROJECT_NAME}
		${OPENGL_LIBRARIES}
		${GLEW_LIBRARIES}
		${SDL2_LIBRARIES}
		${ASSIMP_LIBRARIES}
)

set_property (TARGET ${PROJECT_NAME} PROPERTY
  PUBLIC
    CXX_STANDARD 17
)

install(TARGETS ${PROJECT_NAME} DESTINATION ${PROJECT_SOURCE_DIR}/bin)