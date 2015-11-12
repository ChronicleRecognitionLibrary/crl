# CMAKELISTS.TXT du projet CRL (Chronicle Recognition Library)

# -----------------------------------------------------------------------------
# Copyright (C) 2012, 2013, 2014  ONERA – http://www.onera.fr
# This file is part of CRL : Chronicle Recognition Library.

# CRL is free software: you can redistribute it and/or modify it under
# the terms of the Lesser GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# CRL is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# Lesser GNU General Public License for more details.

# You should have received a copy of the Lesser GNU General Public License
# along with CRL.  If not, see <http://www.gnu.org/licenses/>.

# -----------------------------------------------------------------------------



MESSAGE (".................... CRL Chronicle Recognition Library Project")

# ------------------------------ C++ project name
# ------------------------------ Creates variables: DAMAGE_BINARY_DIR and DAMAGE_SOURCE_DIR

PROJECT (CRL CXX)
CMAKE_MINIMUM_REQUIRED (VERSION 2.7)


# ------------------------------ General case : don't use these options
# ------------------------------ and dates will be defined as double

OPTION (CRL_USERDEFINED_DATES "User defines classes representing dates")

IF (CRL_USERDEFINED_DATES)

	SET (CRL_USER_DATETYPE "" CACHE STRING "User-defined type/class for date types")
	SET (CRL_USER_DATETYPE_HEADER "" CACHE STRING "User-defined header file")
	add_definitions(-DDateType=${CRL_USER_DATETYPE})
	add_definitions(-DDateTypeHeader=${CRL_USER_DATETYPE_HEADER})

	SET (CRL_USER_DURATIONTYPE "" CACHE STRING "User-defined type/class for duration types")
	SET (CRL_USER_DURATIONTYPE_HEADER "" CACHE STRING "User-defined header file")
	add_definitions(-DDurationType=${CRL_USER_DURATIONTYPE})
	add_definitions(-DDurationTypeHeader=${CRL_USER_DURATIONTYPE_HEADER})

	SET (CRL_USER_DATEINFINITY "" CACHE STRING "User-defined type/class/value for infinity date")
	add_definitions(-DINFTY_DATE=${CRL_USER_DATEINFINITY})

	SET (CRL_USER_DATENONE "" CACHE STRING "User-defined class to represent a date tbd")
	add_definitions(-DNO_DATE=${CRL_USER_DATENONE})

	OPTION (CRL_USERDEFINED_LIBRARY "User defines library")
	IF (CRL_USERDEFINED_LIBRARY)
		SET(CRL_USER_INCLUDE "" CACHE STRING "User-defined additional includes (header files)")
		SET(CRL_USER_LIBRARY_RELEASE "" CACHE STRING "User-defined additional release library")
		SET(CRL_USER_LIBRARY_DEBUG "" CACHE STRING "User-defined additional debug library")
		IF (MSVC)
 		 	SET (CRL_USER_LIBRARY  "optimized;${CRL_USER_LIBRARY_RELEASE};debug;${CRL_USER_LIBRARY_DEBUG}")
		ENDIF(MSVC)
	ENDIF (CRL_USERDEFINED_LIBRARY)
ENDIF (CRL_USERDEFINED_DATES)


# ------------------------------ Search directory when
# ------------------------------ FIND_PACKAGE() or INCLUDE() are used

SET (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)


# ------------------------------ warning

#INCLUDE (Warning)
#INCLUDE (ThirdConfigurations)


# ------------------------------ Allow unit tests

ENABLE_TESTING ()
INCLUDE (CTest)


# ------------------------------ SPECIAL MS Visual Studio

IF (MSVC80)
  ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE)
ENDIF (MSVC80)


# ------------------------------ SUB-DIRECTORIES CONTAINING CMAKEFILES

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(tests)


