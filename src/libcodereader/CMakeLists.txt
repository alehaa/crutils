# This file is part of crutils.
#
# crutils is free software: you can redistribute it and/or modify it under the
# terms of the GNU Lesser General Public License as published by the Free
# Software Foundation, either version 3 of the License, or (at your option) any
# later version.
#
# crutils is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
# A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
# details.
#
# You should have received a copy of the GNU Lesser General Public License along
# with crutils. If not, see <http://www.gnu.org/licenses/>.
#
#
# Copyright (C)
#   2013-2016 Alexander Haase <ahaase@alexhaase.de>
#

# Include all subdirectories to make header files available to all other object
# libraries.
include_directories(. stream)


# Recurse into subdirectories.
add_subdirectory(stream)



add_library(codereader SHARED $<TARGET_OBJECTS:codereader-stream> empty.c)
add_sanitizers(codereader)
add_coverage(codereader)

install(TARGETS codereader LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}")
install(FILES codereader.h DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
