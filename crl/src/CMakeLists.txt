# CMAKELISTS.TXT sous-projet CRL_LIB

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


MESSAGE (".................... CRL library")


# ------------------------------ Gathers all source files
# ------------------------------ in variables

SET  ( CRL_LIB_INCLUDE_DIR   "${CMAKE_SOURCE_DIR}/include" CACHE PATH "" FORCE )
SET  ( CRL_LIB_SOURCE_DIR    "${CMAKE_SOURCE_DIR}/src"     CACHE PATH "" FORCE )

FILE ( GLOB CRL_LIB_HEADERS ${CRL_LIB_INCLUDE_DIR}/*.h   )
FILE ( GLOB CRL_LIB_SOURCES ${CRL_LIB_SOURCE_DIR}/*.cpp )

# ------------------------------ Adds these directories to the INCLUDE path

MESSAGE ("CRL_LIB_HEADERS == ${CRL_LIB_HEADERS}")
SOURCE_GROUP (include FILES ${CRL_LIB_HEADERS})

INCLUDE_DIRECTORIES(
  ${CRL_LIB_INCLUDE_DIR}
  ${CRL_USER_INCLUDE}
)

# ------------------------------ Adds the library to the project on the basis of
# ------------------------------ the supplied source files

# ${CRL_LIB_HEADERS} is also added to have the "include" directory inside the visual project
ADD_LIBRARY ( CRL_LIB STATIC
  ${CRL_LIB_SOURCES}
  ${CRL_LIB_HEADERS}
)

SET_TARGET_PROPERTIES (CRL_LIB PROPERTIES DEBUG_OUTPUT_NAME "CRL_LIB_Debug")

INSTALL (FILES ${CRL_LIB_HEADERS} DESTINATION include)
INSTALL (TARGETS CRL_LIB DESTINATION lib)





