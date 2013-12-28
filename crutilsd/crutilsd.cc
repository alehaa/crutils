/*
 * include header-files
 */
#include <cstdlib>

#include <syslog.h>

#include "dbus.h"
#include "device.h"
#include "daemon.h"


int main () {
	/* open syslog. We want to log to daemon and a PID is needed for each log entry,
	 * because there may be multiple instances of this daemon. */
	openlog("crutilsd", LOG_PID, LOG_DAEMON);


	/* discard privilegs if they exist and daemonize */
	if (!drop_privilegs()) exit(EXIT_FAILURE);
	if (!daemonize()) exit(EXIT_FAILURE);


	/* we could only do anything, if a device is given. Get path to device file by
	 * environment variable 'DEVNAME' */
	char* device = getenv("DEVNAME");
	if (device == NULL) {
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
