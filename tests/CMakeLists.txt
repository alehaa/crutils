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

include(easytest) # easytest_* functions and macros


# Set the CODEREADER_BIN variable for all test cases, so they may use it for
# calling the codereader binary. If AddressSanitizer is enabled, an extra
# wrapper will be used, so AddressSanitizer will be initialized.
set(CODEREADER_BIN "$<TARGET_FILE:codereader-bin>")
if (SANITIZE_ADDRESS)
	set(CODEREADER_BIN "${ASan_WRAPPER} ${CODEREADER_BIN}")
endif ()


add_subdirectory(codereader-bin)
