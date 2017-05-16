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
# X11_FOUND: System has X11.
# X11_LIBRARIES: Which libraries to link for X11.
# X11_INCLUDE_DIRS: Which directories to include for X11 headers.
#

include(FindPackageHandleStandardArgs)

find_package(PkgConfig QUIET)


# Search for libX with pkg-config.
pkg_check_modules(PC_X11 QUIET x11)

# Search for X11 with hints from pkg-config.
find_path(X11_INCLUDE_DIR X11/Xlib.h
          HINTS ${PC_X11_INCLUDEDIR} ${PC_X11_INCLUDE_DIRS})
find_library(X11_LIBRARY NAMES X11
             HINTS ${PC_X11_LIBDIR} ${PC_X11_LIBRARY_DIRS})
mark_as_advanced(X11_INCLUDE_DIR X11_LIBRARY)

# Handle X11 components. Include directories won't be searched, as these should
# be equal to those of the core X11 library.
foreach (component IN LISTS X11_FIND_COMPONENTS)
	pkg_check_modules(PC_X11_${component} QUIET ${component})
	if (PC_X11_${component}_FOUND)
		find_library(X11_${component}_LIBRARY
		             NAMES ${PC_X11_${component}_LIBRARIES}
		             HINTS ${PC_X11_${component}_LIBDIR}
		                   ${PC_X11_${component}_LIBRARY_DIRS})
		mark_as_advanced(X11_${component}_LIBRARY)

		if (X11_${component}_LIBRARY)
			set(X11_${component}_FOUND true)
		endif ()
	endif ()
endforeach ()

# Handle the QUIET and REQUIRED arguments and set X11_FOUND to TRUE if all
# listed variables are TRUE.
find_package_handle_standard_args(
	X11
	FOUND_VAR X11_FOUND
	REQUIRED_VARS X11_LIBRARY X11_INCLUDE_DIR
	VERSION_VAR PC_X11_VERSION
	HANDLE_COMPONENTS)

# If X11 and all required components have been found, set the general variables
# for X11 and all components.
set(X11_INCLUDE_DIRS ${X11_INCLUDE_DIR})
set(X11_VERSION ${PC_X11_VERSION})
set(X11_LIBRARIES ${X11_LIBRARY})

# Add component libraries to list of all X11 libraries.
foreach (component IN LISTS X11_FIND_COMPONENTS)
	if (X11_${component}_LIBRARY)
		set(X11_LIBRARIES ${X11_LIBRARIES} ${X11_${component}_LIBRARY})
	endif ()
endforeach ()
