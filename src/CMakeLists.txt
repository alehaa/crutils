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

include(CChelper)       # Enable compiler warnings.
include(GNUInstallDirs) # Install paths.
include(easylib)        # Activate easylib interface.

find_package(Sanitizers)         # Find sanitizers
find_package(C99 REQUIRED)       # C99 support.
find_package(Libconfig REQUIRED) # libconfig used by libcodereader and drivers.


# Set some paths for crutils. We will use the paths defined by GNUInstallDirs.
# If the path is not absolute, we will preprend the install prefix. This ensures
# most compatibility with dpkg and non-dpkg installations.
macro(set_path_var NAME PATH)
	set(${NAME} ${PATH})
	if (NOT IS_ABSOLUTE ${NAME})
		set(${NAME} "${CMAKE_INSTALL_PREFIX}/${${NAME}}")
	endif ()
endmacro()

set_path_var(CODEREADER_DRIVER_DIR "${CMAKE_INSTALL_LIBDIR}/codereader")
set_path_var(CODEREADER_CONFIG_FILE
             "${CMAKE_INSTALL_SYSCONFDIR}/codereader.conf")


# Enable C99 for all targets. We are using mixed C++ style comments for doxygen
# and mixed declarations and code, so we need C99 to satisfy the compiler in
# pedantic mode (enabled by CChelper above)
add_definitions(${C99_FLAGS})


# Recurse into subdirectories.
add_subdirectory(libcodereader)
add_subdirectory(drivers)
add_subdirectory(codereader-bin)
