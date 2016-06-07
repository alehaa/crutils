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

include(CChelper)       # Enable compiler warnings.
include(GNUInstallDirs) # Install paths.

find_package(Sanitizers)   # Find sanitizers
find_package(C99 REQUIRED) # C99 support.


# Enable C99 for all targets. We are using mixed C++ style comments for doxygen
# and mixed declarations and code, so we need C99 to satisfy the compiler in
# pedantic mode (enabled by CChelper above)
add_definitions(${C99_FLAGS})


# Recurse into subdirectories.
add_subdirectory(libcodereader)
