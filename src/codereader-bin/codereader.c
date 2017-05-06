/* This file is part of crutils.
 *
 * crutils is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * crutils is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with crutils. If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * Copyright (C)
 *  2013-2017 Alexander Haase <ahaase@alexhaase.de>
 */

#include <stdio.h>
#include <stdlib.h>

#include <codereader.h>


int
main(int argc, char **argv)
{
	FILE *ch = codereader_open();
	if (ch == NULL) {
		fprintf(stderr, "Can't open codereader!\n");
		return EXIT_FAILURE;
	}


	char buffer[32];

	if (argc == 2)
		for (size_t i = 0; i < atoi(argv[1]); i++) {
			if (fgets(buffer, 32, ch) == NULL)
				break;
			printf("%s", buffer);
		}
	else
		while (fgets(buffer, 32, ch) != NULL)
			printf("%s", buffer);

	fclose(ch);
}
