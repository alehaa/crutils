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
	/*
	 * init class variables
	 */
	this->conf_verbose_level = 0;
	this->conf_device = NULL;
	this->conf_daemonize = true;
	this->conf_daemon_user = NULL;


	/* get config by comand line options and env vars */
	this->get_conf_by_argv(argc, argv);
	this->get_conf_by_env();
}


/* print_usage
 *
 * print a list of all avivable config params to stdout
 */
void crutilsd_config::print_usage () {
	printf("usage: crutilsd (options) (device)\n\nlist of all options:\n");
	printf(" -v\t\t\tEnable verbose output. Number of -v flags increases verbose level\n");
	printf("--verbose(=level)\tSame as -v but verbose level could be set with [level]\n\n");
	printf("--device [file]\t\tdevicefile for codereader\n\t\t\tCould also be set as environment variable DEVNAME or as last comand line argument \n\n");
	printf(" -d, --daemon\t\tdaemonize process after initialisation\n");
	printf(" -f, --foreground\tdo not daemonize and leave process in front\n");
	printf(" -u, --user [user]\tuser under which process runs\n");
}


/* get_conf_by_argv
 *
 * get config by comandline arguments
 */
void crutilsd_config::get_conf_by_argv (int argc, char **argv) {
	static struct option long_options[] = {
		{"verbose", optional_argument, NULL, 'v'},
		{"device", required_argument, NULL, 0},
		{"daemon", no_argument, NULL, 'd'},
		{"foreground", no_argument, NULL, 'f'},
		{"user", required_argument, NULL, 'u'}
	};


	/* getopt_long stores the option index here. */
	int option_index = 0;
	char c;
	while ((c = getopt_long(argc, argv, "dfu:v", long_options, &option_index)) != -1) {
		switch (c) {
			case 0:
				switch (option_index) {
					case 1:
						this->conf_device = optarg;
						break;
				}
				break;

			case 'd':
				this->conf_daemonize = true;
				break;

			case 'f':
				this->conf_daemonize = false;
				break;

			case 'u':
				this->conf_daemon_user = optarg;
				break;

			case 'v':
				if (this->conf_verbose_level < 3) {
					/* handle long option --verbose(=level), too */
					if (optarg) this->conf_verbose_level = (unsigned char) atoi(optarg);
					else this->conf_verbose_level++;
				}
				break;

			default:
				/* an error occured. print error and exit */
				this->print_usage();
				exit(EXIT_FAILURE);
		}
	}

	if (optind < argc) {
		this->conf_device = argv[optind];
	}
}


/* get_conf_by_env
 *
 * get config by env vars if not already set by argv
 */
void crutilsd_config::get_conf_by_env () {
	if (!this->conf_device) {
		char* device = getenv("DEVNAME");
		if (device) this->conf_device = device;
	}
}


/* get_conf_verbose
 *
 * return this->conf_verbose;
 */
unsigned char crutilsd_config::get_conf_verbose_level () {
	return this->conf_verbose_level;
}


/* get_conf_device
 *
 * return this->conf_device;
 */
char * crutilsd_config::get_conf_device () {
	return this->conf_device;
}


/* get_conf_daemonize
 *
 * return this->conf_daemonize;
 */
bool crutilsd_config::get_conf_daemonize () {
	return this->conf_daemonize;
}


/* get_conf_daemon_user
 *
 * return this->conf_daemon_user;
 */
char * crutilsd_config::get_conf_daemon_user () {
	return this->conf_daemon_user;
}
