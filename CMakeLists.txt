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
endif()

# Setting the project to cxx_std_11
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
		${3DEngineOpenGL_SOURCE_DIR}/src/*.h
		${3DEngineOpenGL_SOURCE_DIR}/src/*.hpp
)

# Lets LOAD app our sources!
file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS
		${3DEngineOpenGL_SOURCE_DIR}/src/*.cpp
		${3DEngineOpenGL_SOURCE_DIR}/src/*.c
)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
	add_compile_options(3DEngineOpenGL -02)
endif (CMAKE_BUILD_TYPE STREQUAL "Release")

# Checking if it is in visual studio for correct errors flags
if (MSVC)
	add_compile_options(-c /W4)
else()
	add_compile_options(-c -Wall -Wextra -Werror -Wpedantic)
endif()

# Define the executable
add_executable(3DEngineOpenGL ${HDRS} ${SRCS})

# We need a CMake_Dir with some code to find external dependencies
set(3DEngineOpenGL_CMAKE_DIR "${3DEngineOpenGL_SOURCE_DIR}/cmake")

#######################################
# LOOK for the packages that we need! #
#######################################

# OpenGL
find_package(OpenGL REQUIRED)

# GLEW
include(${3DEngineOpenGL_CMAKE_DIR}/FindGLEW.cmake)

# SDL2
include(${3DEngineOpenGL_CMAKE_DIR}/FindSDL2.cmake)

# ASSIMP
include(${3DEngineOpenGL_CMAKE_DIR}/FindASSIMP.cmake)

# Define the include DIRs
include_directories(
		${3DEngineOpenGL_SOURCE_DIR}/headers
		${3DEngineOpenGL_SOURCE_DIR}/sources
		${3DEngineOpenGL_SOURCE_DIR}/src
		${OPENGL_INCLUDE_DIRS}
		${GLEW_INCLUDE_DIRS}
		${SDL2_INCLUDE_DIRS}
		${ASSIMP_INCLUDE_DIRS}
)

# Define the link libraries
target_link_libraries(3DEngineOpenGL
		${OPENGL_LIBRARIES}
		${GLEW_LIBRARIES}
		${SDL2_LIBRARIES}
		${ASSIMP_LIBRARIES}
)

if (WIN32)
	string(REPLACE "/" "\\" source_path_windows "${3DEngineOpenGL_SOURCE_DIR}/res")
	string(REPLACE "/" "\\" build_path_windows "${3DEngineOpenGL_BINARY_DIR}/res")
	execute_process(COMMAND cmd.exe /c mklink /J "${build_path_windows}" "${source_path_windows}" RESULT_VARIABLE exitcode)
else()
	execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${3DEngineOpenGL_SOURCE_DIR}/res ${3DEngineOpenGL_BINARY_DIR}/res RESULT_VARIABLE exitcode)
endif()

if(NOT ${exitcode} EQUAL 0)
	message("SYMLINKING FAILED: ${exitcode}")
	message("FAILED BACK TO COPYING")
endif()

# Copy only if the build folder contains outdated res folder. Only if symlink failed.
file(GLOB_RECURSE RES RELATIVE ${3DEngineOpenGL_SOURCE_DIR}/res/ ${3DEngineOpenGL_SOURCE_DIR}/res/*.*)
foreach(file IN LISTS RES)
	configure_file(${3DEngineOpenGL_SOURCE_DIR}/res/${file} ${3DEngineOpenGL_BINARY_DIR}/res/${file} COPYONLY)
endforeach()

#Creating virt folders for VS
if(MSVC_IDE)
	foreach(source IN LISTS SRCS HDRS)
		get_filename_component(source_path "${source}" PATH)
		string(REPLACE "${3DEngineOpenGL_SOURCE_DIR}" "" relative_source_path "${source_paths}")
		string(REPLACE "/" "\\" source_path_msvc "${relative_source_path}")
		source_group("${source_path_msvc}" FILES "${source}")
	endforeach()
endif()

install(TARGETS 3DEngineOpenGL DESTINATION ${3DEngineOpenGL_SOURCE_DIR}/bin)