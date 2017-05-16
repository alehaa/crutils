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

## \brief Add a new codereader driver.
#
# \note The target will be named `driver-${NAME}`.
#
#
# \param NAME Name of the driver.
# \param ... List of source files.
#
function (codereader_add_driver NAME)
	# Add a new library target for the driver.
	add_library(driver-${NAME} MODULE ${ARGN})

	# Set necessary properties for the library. The library should not have
	# the `lib` prefix and is not named like its target name but simply ${NAME}.
	set_target_properties(driver-${NAME} PROPERTIES
		OUTPUT_NAME "${NAME}"
		PREFIX "")

	# Enable code coverage and sanitizing for the driver.
	add_sanitizers(driver-${NAME})
	add_coverage(driver-${NAME})

	# Install the driver to the driver path.
	install(TARGETS driver-${NAME} DESTINATION ${CODEREADER_DRIVER_DIR})
endfunction ()


add_subdirectory(lxinput)
add_subdirectory(xinput2)
