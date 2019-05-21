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
# Try to find ASSMIP and set the following: #
#                                           #
# SDL2_FOUND                                #
# SDL2_INCLUDE_DIRS                         #
# SDL2_LIBRARIES                            #
#############################################

set( SDL2_SEARCH_PATHS
		${SDL2_ROOT_DIR}				# SDL2
		./lib/SDL2
		$ENV{PROGRAMFILES}/SDL2			# WINDOWS
		"$ENV{PROGRAMFILES\(X86\)}/SDL2"	# WINDOWS
		~/Library/Frameworks			# MAC
		/Library/Frameworks				# MAC
		/usr/local						# LINUX/MAC/UNIX
		/usr							# LINUX/MAC/UNIX
		/opt							# LINUX/MAC/UNIX
		/sw								# Fink
		/opt/local						# DarwinPorts
		/opt/csw						# Blastwave
)

find_path( SDL2_INCLUDE_DIRS
		NAMES
			SDL.h SDL2/SDL.h
		PATHS
			${SDL2_SEARCH_PATHS}
		PATH_SUFFIXES
			include
		DOC
			"The directory where SDL.h resides"
)

find_library( SDL2_LIBRARIES
		NAMES
			SDL2 SDL2main
		PATHS
			${SDL2_SEARCH_PATHS}
		PATH_SUFFIXES
			lib
			lib64
			lib/x86
			lib/x64
		DOC
			"The SDL2 library"
)

# Check if we found it!
if( SDL2_INCLUDE_DIRS AND SDL2_LIBRARIES )
		set( SDL2_FOUND TRUE )
		message( STATUS "Looking for SDL2 - found" )
else( SDL2_INCLUDE_DIRS AND SDL2_LIBRARIES )
		set( SDL2_FOUND FALSE )
		message( STATUS "Looking for SDL2 - not found" )
endif( SDL2_INCLUDE_DIRS AND SDL2_LIBRARIES )