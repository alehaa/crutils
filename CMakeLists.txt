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

# minimum required cmake version
cmake_minimum_required(VERSION 2.6)


# Project information.
#
project("crutils" C CXX)


# Recurse into subdirectories.
#
add_subdirectory(src)
add_subdirectory(doc)
