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
 *  2013-2016 Alexander Haase <ahaase@alexhaase.de>
 */

/*
 * include header-files
 */
#include <cstdlib>
#include <cstdio>
#include <csignal>
#include <vector>

#include <syslog.h>
#include <pwd.h>
#include <unistd.h>
#include <sys/stat.h>
#include <linux/input.h>

#include "config.h"
#include "dbus.h"
#include "log.h"

extern "C" {
#include "../drivers/lxinput/lxinput.h"
}


int main (int argc, char **argv) {
	/* init config */
	crutilsd_config config(argc, argv);

	/* init log */
	crutilsd_log log(&config);


	/* drop root privilegs if should be done */
	if (config.get_conf_daemon_user() != NULL) {
		if (getuid() != 0) log.printf(LOG_WARNING, "could not discard privilegs: you are not root");
		else {
			struct passwd *user = getpwnam(config.get_conf_daemon_user());
			if (user == NULL) {
				log.printf(LOG_ERR, "could not discard privilegs: user %s does not exist", config.get_conf_daemon_user());
				exit(EXIT_FAILURE);
			}

			if (setuid(user->pw_uid) == 0) log.printf(LOG_DEBUG, "privilegs discarded - running under user %s", config.get_conf_daemon_user());
			else {
				log.printf(LOG_ERR, "could not discard privilegs: an undefined error occured");
				exit(EXIT_FAILURE);
			}
		}
	}

	/* print warning if running under root */
	if (getuid() == 0) log.printf(LOG_WARNING, "warning: you are running with a privileged user");


	/* open device file */
	int device = codereader_open(config.get_conf_device());
	if (device < 0) {
		log.printf(LOG_ERR, "could not open device-file '%s'", config.get_conf_device());
		exit(EXIT_FAILURE);
	}
	log.printf(LOG_DEBUG, "opened device-file '%s'", config.get_conf_device());


	/* open D-BUS connection */
	crutilsd_dbus dbus(&log);
	if (!dbus.connect()) {
		log.printf(LOG_ERR, "could not open D-BUS connection");
		exit(EXIT_FAILURE);
	} else log.printf(LOG_DEBUG, "connected to D-BUS");


	/* daemonize, if configured */
	if (config.get_conf_daemonize()) {
		if (daemon(0, 0) < 0) {
			log.printf(LOG_ERR, "error while daemonize process");
			exit(EXIT_FAILURE);
		}
		log.printf(LOG_DEBUG, "daemonized process");
	}


	/* start with read out the data. The chars are cached in the buffer and will be
	 * sent as complete code after read operations via dbus to the clients. In order
	 * to check after each input, if the last input event was the end of a code, the
	 * buffer-size is one.
	 *
	 * The input is - as long as the code has not finished - temporarily stored in
	 * a vector as buffer. Once the ending of a code is found, the complete buffer
	 * will be send via D-BUS to all clients and buffer cleared.
	 */
	log.printf(LOG_INFO, "start listening for read codes");

	char buffer[256];
	while (codereader_read(device, buffer, 256) >= 0) {
		log.printf(LOG_DEBUG, "recived code '%s'", buffer);
		dbus.send_code(buffer);
	}

	codereader_close(device);

	log.printf(LOG_INFO, "stoped listening");
	exit(EXIT_SUCCESS);
}
