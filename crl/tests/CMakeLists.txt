# CMAKELISTS.TXT sous-projet CRL_TESTS

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



MESSAGE (".................... CRL tests")


# ------------------------------ Gathers all source files
# ------------------------------ in variables

SET  ( CRL_TESTS_DIR   "${CMAKE_SOURCE_DIR}/tests" CACHE PATH "" FORCE )

FILE ( GLOB CRL_TESTS_HEADERS ${CRL_TESTS_DIR}/*.h   )
FILE ( GLOB CRL_TESTS_SOURCES ${CRL_TESTS_DIR}/*.cpp )

# ------------------------------ Adds these directories to the INCLUDE path

INCLUDE_DIRECTORIES(
  ${CRL_LIB_INCLUDE_DIR}
  ${CRL_TESTS_INCLUDE_DIR}
  ${CRL_USER_INCLUDE}
)

SOURCE_GROUP (include FILES ${CRL_TESTS_HEADERS})

# ------------------------------ Adds the test files for
# ------------------------------ teh supplied source files.


ADD_EXECUTABLE(CRL_TESTS
  ${CRL_TESTS_SOURCES}
  ${CRL_TESTS_HEADERS}
)


TARGET_LINK_LIBRARIES(CRL_TESTS
  CRL_LIB
  ${CRL_USER_LIBRARY}
)

ADD_TEST(CRL
  ${EXECUTABLE_OUTPUT_PATH}/CRL_TESTS
) 
