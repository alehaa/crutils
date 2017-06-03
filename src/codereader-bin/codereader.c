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

#include <stdio.h>  // fclose, fprintf, printf
#include <stdlib.h> // atoi, EXIT_SUCCESS, EXIT_FAILURE

#include <argp.h>       // argp functions
#include <codereader.h> // codereader_open

#include "config.h"


/* Configure argp.
 *
 * Argp is used to parse command line options. It handles the most common
 * options like --help and --version, so that a manual coding of getopt code is
 * not required anymore. For detailed information about the varaibles below, see
 * the argp documentation.
 */
const char *argp_program_version = "crutils " CRUTILS_VERSION;

const char *argp_program_bug_address =
    "https://github.com/alehaa/crutils/issues";

static char doc[] = "Read barcodes from multiple barcode readers";

static struct argp_option options[] = {
    {"config", 'c', "FILE", 0, "Configuration file"},
    {"count", 'n', "NUMBER", 0, "How many barcodes to read"},
    {0}};

/* Initialize argp parser. We'll use above defined parameters for documentation
 * strings of argp. A forward declaration for parse_arguments is added to define
 * parse_arguments together with the other functions below. */
static error_t parse_arguments(int key, char *arg, struct argp_state *state);
static struct argp argp = {options, &parse_arguments, NULL, doc};


/** \brief Argument parser for argp.
 *
 * \note See argp parser documentation for detailed information about the
 *  structure and functionality of function.
 */
static error_t
parse_arguments(int key, char *arg, struct argp_state *state)
{
	switch (key) {
		case 'c': setenv("CODEREADER_CONFIG", arg, 1); break;
		case 'n': *((int *)state->input) = atoi(arg); break;

		default: return ARGP_ERR_UNKNOWN;
	}

	return 0;
}


int
main(int argc, char **argv)
{
	/* Parse our arguments. Parsed arguments will manipulate the current
	 * environment to set options for libcodereader. If the user specified how
	 * many barcodes to read, this number will be stored in num. */
	int num = -1;
	argp_parse(&argp, argc, argv, 0, NULL, &num);


	/* Open the codereader stream. This will open a connection to all available
	 * barcode readers. */
	FILE *ch = codereader_open();
	if (ch == NULL) {
		fprintf(stderr, "Can't open codereader!\n");
		return EXIT_FAILURE;
	}

	/* Read data from the barcode readers for the specified number of barcodes
	 * to read or n an endless loop if no maximum is defined. The read barcodes
	 * will be printed to stdout. */
	char buffer[32];
	while ((num == -1 || num--) && (fgets(buffer, 32, ch) != NULL))
		printf("%s", buffer);

	/* Close the codereader stream and return success or failure depending on
	 * the return code. */
	return (fclose(ch) == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
