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

include_directories("/usr/include/dbus-1.0")
include_directories("/usr/lib/x86_64-linux-gnu/dbus-1.0/include")
include_directories("/usr/include/glib-2.0")
include_directories("/usr/lib/x86_64-linux-gnu/glib-2.0/include")

add_executable(crutilsd
	crutilsd.cpp
	config.cpp
	dbus.cpp
	log.cpp
)

target_link_libraries(crutilsd "-ldbus-glib-1 -ldbus-1 -lgobject-2.0 -lglib-2.0" driver-lxinput)

install(TARGETS crutilsd DESTINATION bin)
