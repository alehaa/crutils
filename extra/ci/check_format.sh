#!/bin/sh

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

# This script is used for checking the coding convention in all source-code
# files of this repository.

exec git filter-branch --force --tree-filter '
	find . \( -name "*.c" -o -name "*.h" -o -name "*.h.in" \) \
		-exec clang-format-3.8 -style=file -fallback-style=none -i {} \; ;
	if test $(git diff | wc -l) -gt 0;
	then
		echo "";
		git --no-pager diff --color=always;
		exit 1;
	fi
	'  --tag-name-filter cat origin/master~1..HEAD
