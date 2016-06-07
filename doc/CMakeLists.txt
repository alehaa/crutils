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

# Doxygen documentation.
#
option(BUILD_DOCUMENTATION "Create HTML documentation" Off)

set(doxyfile_in "${CMAKE_CURRENT_SOURCE_DIR}/doxygen.conf.in")
set(doxyfile "${CMAKE_CURRENT_BINARY_DIR}/doxygen.conf")
set(doxy_output_root "${CMAKE_CURRENT_BINARY_DIR}")
set(doxy_html_index_file "${doxy_output_root}/html/index.html")

if (BUILD_DOCUMENTATION)
	find_package(Doxygen REQUIRED)

	if (NOT DOXYGEN_DOT_EXECUTABLE)
		set(DOXYGEN_DOT_EXECUTABLE "")
	endif ()

	configure_file(${doxyfile_in} ${doxyfile} @ONLY)

	add_custom_command (OUTPUT ${doxy_html_index_file}
		COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
		MAIN_DEPENDENCY ${doxyfile} ${doxyfile_in}
		DEPENDS crutilsd driver_linux crutils
		COMMENT "Generating documentation"
	)
	add_custom_target(doc ALL DEPENDS ${doxy_html_index_file})


	# install documentation
	install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html DESTINATION share/doc)
endif (BUILD_DOCUMENTATION)
