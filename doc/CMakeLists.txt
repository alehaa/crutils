# This file is part of crutils.
#
# crutilsd is free software: you can redistribute it and/or modify it under the
# terms of the GNU General Public License as published by the Free Software
# Foundation, either version 3 of the License, or (at your option) any later
# version.
#
# libcrutils is free software: you can restribute it and/or modify it under the
# terms of the GNU Lesser General Public license as published by the Free
# Software Foundation, either version 3 of the License, or (at your option) any
# later version.
#
#
# This program is distributed in the hope that it will be useful,but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
# more details.
#
# You should have received a copy of the GNU General Public License and GNU
# Lesser General Public License along with this program. If not, see
#
#  http://www.gnu.org/licenses/
#
#
# Copyright (C)
#   2013-2014 Alexander Haase <alexander.haase@rwth-aachen.de>
#


#
# configuration
#
set(doxyfile_in "${CMAKE_CURRENT_SOURCE_DIR}/doxygen.conf.in")
set(doxyfile "${CMAKE_CURRENT_BINARY_DIR}/doxygen.conf")
set(doxy_output_root "${CMAKE_CURRENT_BINARY_DIR}")
set(doxy_html_index_file "${doxy_output_root}/html/index.html")


#
# add a target to generate documentation with Doxygen
#
option(BUILD_DOCUMENTATION "Create HTML based documentation" ON)

if (BUILD_DOCUMENTATION)
	# try to find doxygen
	find_package(Doxygen)

	# print error, if doxygen was not found
	if (NOT DOXYGEN_FOUND)
		message(FATAL_ERROR "Doxygen is needed to build the documentation.")
	endif (NOT DOXYGEN_FOUND)


	# configure doxyfile
	configure_file(${doxyfile_in} ${doxyfile} @ONLY)


	# add commands to run doxygen
	add_custom_command (OUTPUT ${doxy_html_index_file}
		COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
		MAIN_DEPENDENCY ${doxyfile} ${doxyfile_in}

		DEPENDS crutilsd driver_lxinput crutils

		COMMENT "Generating documentation"
	)

	add_custom_target(doc ALL DEPENDS ${doxy_html_index_file})


	# install documentation
	install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html DESTINATION share/doc)
endif (BUILD_DOCUMENTATION)
