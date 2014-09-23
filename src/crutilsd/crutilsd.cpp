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
#include <cstdlib>
#include <cstdio>
#include <csignal>
#include <vector>

#include <syslog.h>
#include <pwd.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/input.h>

#include "config.h"
#include "dbus.h"
#include "keytoc.h"
#include "log.h"


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
	int device = open(config.get_conf_device(), O_RDONLY);
	if (device < 0) {
		log.printf(LOG_ERR, "could not open device-file '%s'", config.get_conf_device());
		exit(EXIT_FAILURE);
	}
	log.printf(LOG_DEBUG, "opened device-file '%s'", config.get_conf_device());

	/* Try to get exclusive rights for this device. Otherwise e.g. X11 might output
	 * the same data as HID-input as we recive here. */
	if (ioctl(device, EVIOCGRAB, 1) < 0) {
		log.printf(LOG_ERR, "could not get exclusive rights for device");
		exit(EXIT_FAILURE);
	}
	log.printf(LOG_ERR, "got exclusive rights for device");


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

	struct input_event ev[1];
	int rd, size = sizeof(struct input_event);
	std::vector<char> buffer;
	while (true) {
		/* exit the while loop when an error occurs. This can also happen when the
		 * device is removed. */
		if ((rd = read(device, ev, size)) < size) break;

		/* We only handle events of type EV_KEY. In addition, we treat only the press of
		 * keys not release that to prevent the duplicate detecting the input. */
		if (!(ev[0].type == EV_KEY && ev[0].value == 1)) continue;

		/* translate key event to char */
		char c = keytoc(&ev[0]);

		/* keytoc () returns for all characters that can happen the valid characters.
		 * If it is around the end of a code - represented by a globally KEY_ENTER - 0 is
		 * returned. If an error occurs -1 is returned. */
		if (c < 0) {
			log.printf(LOG_NOTICE, "invalid char recognized. keycode: %i\n", ev[0].code);
			continue;
		}

		/* add char to buffer */
		buffer.push_back(c);

		/* if c is 0, then this is the end of a code. Send the code to the custom
		 * function */
		if (c == 0) {
			log.printf(LOG_DEBUG, "recived code '%s'", &buffer[0]);
			dbus.send_code(&buffer[0]);

			buffer.clear();
		}
	}

	/* clean buffer */
	buffer.clear();

	log.printf(LOG_INFO, "stoped listening");
	exit(EXIT_SUCCESS);
}
