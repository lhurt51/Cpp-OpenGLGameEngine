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

#############################################
# Try to find GLEW and set the following:   #
#                                           #
# GLEW_FOUND                                #
# GLEW_INCLUDE_DIRS                         #
# GLEW_LIBRARIES                            #
#############################################

set( GLEW_SEARCH_PATHS
		${GLEW_ROOT_DIR}				# GLEW!
		./lib/glew
		$ENV{PROGRAMFILES}/GLEW			# WINDOWS
		"$ENV{PROGRAMFILES\(X86\)}/GLEW"	# WINDOWS
		~/Library/Frameworks			# MAC
		/Library/Frameworks				# MAC
		/usr/local						# LINUX/MAC/UNIX
		/usr							# LINUX/MAC/UNIX
		/opt							# LINUX/MAC/UNIX
		/sw								# Fink
		/opt/local						# DarwinPorts
		/opt/csw						# Blastwave
)

find_path( GLEW_INCLUDE_DIRS
		NAMES
			GL/glew.h
		PATHS
			${GLEW_SEARCH_PATHS}
		PATH_SUFFIXES
			include
		DOC
			"The directory where GL/glew.h resides"
)

find_library( GLEW_LIBRARIES
		NAMES
			glew GLEW libGLEW.a glew32 glew32s
		PATHS
			${GLEW_SEARCH_PATHS}
		PATH_SUFFIXES
			lib
			lib64
			lib/Release/Win32
			lib/Release/x64
		DOC
			"The GLEW library"
)

# Check if we found it
if( GLEW_INCLUDE_DIRS AND GLEW_LIBRARIES )
	set( GLEW_FOUND TRUE )
	message( STATUS "Looking for GLEW - found" )
else( GLEW_INCLUDE_DIRS AND GLEW_LIBRARIES )
	set( GLEW_FOUND FALSE )
	message( STATUS "Looking for GLEW - not found" )
endif( GLEW_INCLUDE_DIRS AND GLEW_LIBRARIES )