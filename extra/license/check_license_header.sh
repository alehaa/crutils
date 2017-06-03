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

readonly HEADER_DIR=$(dirname $0)

exec git filter-branch --force --tree-filter '
	num=$(wc -l '$HEADER_DIR'/header.c | sed "s/[^0-9]*//g") ;
	find . \( -name "*.c" -o -name "*.h" -o -name "*.h.in"            \
	          -o -name "./extra/codereader.conf" \)                   \
		-exec sh -c "                                                 \
			cat {} | head -n $num                                     \
				| git diff  --no-index -- '$HEADER_DIR'/header.c -    \
			|| (                                                      \
				echo \"\";                                            \
				echo \"{} does not contain a valid license header.\"; \
				echo 1 > error                                        \
			)" \; ;

	num=$(wc -l '$HEADER_DIR'/header.txt | sed "s/[^0-9]*//g") ;
	find . \( -name "*.txt" -o -name "*.conf" -o -name "*.yml"        \
	          -o -name "*.cmake" \)                                   \
	        ! -path "./extra/codereader.conf"                         \
		-exec sh -c "                                                 \
			cat {} | head -n $num                                     \
				| git diff  --no-index -- '$HEADER_DIR'/header.txt -  \
			|| (                                                      \
				echo \"\";                                            \
				echo \"{} does not contain a valid license header.\"; \
				echo 1 > error                                        \
			)" \; ;                                                   \

	find . \( -name "*.sh" \) -exec sh -c "                       \
		cat {} | head -n $((num + 2)) | tail -n $num              \
			| git diff  --no-index -- extra/license/header.txt -  \
		|| (                                                      \
			echo \"\";                                            \
			echo \"{} does not contain a valid license header.\"; \
			echo 1 > error                                        \
		)" \; ;                                                   \

	test \! -f error ;
	' --tag-name-filter cat origin/master~1..HEAD
