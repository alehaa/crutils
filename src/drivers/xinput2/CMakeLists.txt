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
#   2013-2017 Alexander Haase <ahaase@alexhaase.de>
#

find_package(X11 COMPONENTS xi) # X11 library


# If X11 and the required components are not available, this driver can't be
# compiled and this file will be ignored.
if (NOT X11_FOUND)
	return()
endif ()


include_directories(${LIBCONFIG_INCLUDE_DIRS} ${X11_INCLUDE_DIRS})

codereader_add_driver(xinput2 xinput2.c)
target_link_libraries(driver-xinput2 ${LIBCONFIG_LIBRARIES} ${X11_LIBRARIES})
