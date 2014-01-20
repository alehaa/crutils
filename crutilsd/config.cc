/* This file is part of crutilsd.
 *
 * crutilsd is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful,but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * copyright 2013-2014 Alexander Haase
 */

/*
 * include header files
 */
#include "config.h"

#include <cstddef>
#include <cstdio>
#include <cstdlib>

#include <getopt.h>


/* constructor
 *
 * init class and get config from comand line options or env vars
 */
crutilsd_config::crutilsd_config (int argc, char **argv) {
	static struct option long_options[] = {
		{"verbose", no_argument, NULL, 'v'}
	};


	/* getopt_long stores the option index here. */
	int option_index = 0;
	char c;
	while ((c = getopt_long(argc, argv, "v", long_options, &option_index)) != -1) {
		switch (c) {
			/* currently there are no long arguments set, which are NOT mapped to short
			 * flags
			 * case 0:
			 * 	printf("%i\n", option_index);
			 * 	break;
			 */

			case 'v':
				this->conf_verbose = true;
				break;

			default:
				/* an error occured. print error and exit */
				fprintf(stderr, "usage: %s (options) device_file\n\nlist of all options:\n-v, --verbose\tprint all messages to stdout\n", argv[0]);
				exit(EXIT_FAILURE);
		}
	}
}
