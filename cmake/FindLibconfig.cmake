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

# Try to find libconfig. If found, the following variables will be set:
#
# LIBCONFIG_FOUND: System has libconfig.
# LIBCONFIG_LIBRARIES: Which libraries to link for libconfig.
# LIBCONFIG_INCLUDE_DIRS: Which directories to include for libconfig headers.
#

include(FindPackageHandleStandardArgs)

find_package(PkgConfig QUIET)


# Search for libconfig with pkg-config.
pkg_check_modules(PC_LIBCONFIG QUIET libconfig)


# Search for libconfig with hints from pkg-config.
find_path(LIBCONFIG_INCLUDE_DIR libconfig.h
          HINTS ${PC_LIBCONFIG_INCLUDEDIR} ${PC_LIBCONFIG_INCLUDE_DIRS})
find_library(LIBCONFIG_LIBRARY NAMES config
             HINTS ${PC_LIBCONFIG_LIBDIR} ${PC_LIBCONFIG_LIBRARY_DIRS} )


# Handle the QUIET and REQUIRED arguments and set LIBCONFIG_FOUND to TRUE if all
# listed variables are TRUE.
find_package_handle_standard_args(
	Libconfig
	FOUND_VAR LIBCONFIG_FOUND
	REQUIRED_VARS LIBCONFIG_LIBRARY LIBCONFIG_INCLUDE_DIR
	VERSION_VAR PC_LIBCONFIG_VERSION)

mark_as_advanced(LIBCONFIG_INCLUDE_DIR LIBCONFIG_LIBRARY)

set(LIBCONFIG_LIBRARIES ${LIBCONFIG_LIBRARY})
set(LIBCONFIG_INCLUDE_DIRS ${LIBCONFIG_INCLUDE_DIR})
set(LIBCONFIG_VERSION ${PC_LIBCONFIG_VERSION})
