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

include_directories("/usr/include/dbus-1.0")
include_directories("/usr/lib/x86_64-linux-gnu/dbus-1.0/include")
include_directories("/usr/include/glib-2.0")
include_directories("/usr/lib/x86_64-linux-gnu/glib-2.0/include")

add_library(crutils SHARED
	main.cpp
	crutils.cpp
	crutils_dbus.cpp
	crutils_thread.cpp
)

set_target_properties(crutils PROPERTIES
	VERSION 1.0.0
	SOVERSION 1
)

target_link_libraries(crutils "-ldbus-glib-1 -ldbus-1 -lgobject-2.0 -lglib-2.0")

install(TARGETS crutils DESTINATION lib)
