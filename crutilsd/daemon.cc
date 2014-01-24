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
 * include header-files
 */
#include "daemon.h"

#include <syslog.h>
#include <pwd.h>
#include <unistd.h>
#include <csignal>

#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstdlib>

/* daemonize ()
 *
 * make this process to a daemon
 */
bool daemonize () {
	/* fork parent */
	pid_t pid;
	pid = fork();
	if (pid < 0) {
		/* an error occured */
		syslog(LOG_ERR, "error while forking process");
		exit(EXIT_FAILURE);
	} else {
		/* kill the parent */
		if (pid > 0) exit(EXIT_SUCCESS);

		/* create a new session */
		if (setsid() < 0) {
			syslog(LOG_ERR, "error while generate new session in forked process");
			exit (EXIT_FAILURE);
		} else {
			/* ignore SIGHUP */
			signal(SIGHUP, SIG_IGN);

			/* fork the forked process to prevent zombie-processes */
			pid = fork();
			if (pid < 0) {
				/* an error occured */
				syslog(LOG_ERR, "error while second forking process");
				exit(EXIT_FAILURE);
			} else {
				/* kill the parent */
				if (pid > 0) exit(EXIT_SUCCESS);

				/* change working dir */
				chdir ("/");

				/* don't use umask of parrent processes */
				umask (0);

				/* close all file descriptors (STDOUT, STDERR, etc. */
				for (int i = sysconf (_SC_OPEN_MAX); i > 0; i--) close (i);

				syslog(LOG_INFO, "daemonized process");
				return true;
			}
		}
	}
}
