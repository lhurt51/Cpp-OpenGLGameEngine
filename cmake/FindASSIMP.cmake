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
# ASSIMP_FOUND                              #
# ASSIMP_INCLUDE_DIRS                       #
# ASSIMP_LIBRARIES                          #
#############################################

set( ASSIMP_SEARCH_PATHS
		${ASSIMP_ROOT_DIR}					# ASSIMP!
		./lib/Assimp
		$ENV{PROGRAMFILES}/ASSIMP			# WINDOWS
		"$ENV{PROGRAMFILES\(X86\)}/ASSIMP"	# WINDOWS
		~/Library/Frameworks				# MAC
		/Library/Frameworks					# MAC
		/usr/local							# LINUX/MAC/UNIX
		/usr								# LINUX/MAC/UNIX
		/opt								# LINUX/MAC/UNIX
		/sw									# Fink
		/opt/local							# DarwinPorts
		/opt/csw							# Blastwave
)

find_path( ASSIMP_INCLUDE_DIRS
		NAMES
			assimp/mesh.h
		PATHS
			${ASSIMP_SEARCH_PATHS}
		PATH_SUFFIXES
			include
		DOC
			"The directory where assimp/mesh.h is located"
)

find_library( ASSIMP_LIBRARIES
		NAMES
			assimp ASSIMP assimp-vc140-mt
		PATHS
			${ASSIMP_SEARCH_PATHS}
		PATH_SUFFIXES
			lib
			lib64
			lib/x86
			lib/x64
		DOC
			"The ASSIMP library"
)

# Check if we found it!
if( ASSIMP_INCLUDE_DIRS AND ASSIMP_LIBRARIES )
		set( ASSIMP_FOUND TRUE )
		message( STATUS "Looking for ASSIMP - found" )
else( ASSIMP_INCLUDE_DIRS AND ASSIMP_LIBRARIES )
		set( ASSIMP_FOUND FALSE )
		message( STATUS "Looking for ASSIMP - not found" )
endif( ASSIMP_INCLUDE_DIRS AND ASSIMP_LIBRARIES )