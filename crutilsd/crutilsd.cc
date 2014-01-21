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

#include <syslog.h>

#include "dbus.h"
#include "device.h"
#include "daemon.h"

#include "config.h"
#include "log.h"

int main (int argc, char **argv) {
	/* init config */
	crutilsd_config conf(argc, argv);

	/* init log */
	crutilsd_log log(&conf);


	exit(EXIT_SUCCESS);

	/* discard privilegs if they exist and daemonize */
	if (!drop_privilegs()) exit(EXIT_FAILURE);
	if (!daemonize()) exit(EXIT_FAILURE);


	/* we could only do anything, if a device is given. Get path to device file by
	 * environment variable 'DEVNAME' */
	char* device = conf.get_conf_device();
	if (!device) {
		syslog(LOG_ERR, "device-file in environment variable 'DEVNAME' not set");
		exit(EXIT_FAILURE);
	} else syslog(LOG_INFO, "started for device '%s'", device);


	/* Try to connect to D-BUS daemon */
	crutilsd_dbus dbus;
	if (!dbus.connect()) {
		syslog(LOG_ERR, "could not open D-BUS connection");
		exit(EXIT_FAILURE);
	} else syslog(LOG_DEBUG, "connected to D-BUS");

	/* open device */
	crutilsd_device dev;
	if (dev.open_device(device)) {
		/* set dbus conection */
		dev.set_dbus(&dbus);

		/* listen device until device disconnects */
		dev.listen();

		/* device is disconnected. exit application */
		syslog(LOG_INFO, "exiting");
		exit(EXIT_SUCCESS);
	}

	/* an error occured */
	exit(EXIT_FAILURE);
}
