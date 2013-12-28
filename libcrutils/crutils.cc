/*
 * include header files
 */
#include "crutils.h"

#include <cstdio>


/* constructor ()
 *
 * init class and internal varaibles
 */
crutils::crutils () {
	this->dbus_connection = NULL;
}


/* destructor ()
 *
 * close all open connections
 */
crutils::~crutils () {
	this->disconnect();
}


/* connect ()
 *
 * try to connect to DBUS-Daemon and return result
 */
bool crutils::connect () {
	DBusError dbus_error;
	this->dbus_connection = dbus_bus_get(DBUS_BUS_SYSTEM, &dbus_error);
	if (this->dbus_connection) return true;

	/* an error occured */
	fprintf(stderr, "Failed to connect to D-BUS daemon: %s\n", dbus_error.message);
	dbus_error_free(&dbus_error);

	return false;
}


/* disconnect ()
 *
 * disconnect the current session, if one is open
 */
void crutils::disconnect () {
	/* a connection could only be closed if one is open. */
	if (this->dbus_connection) {
		/* try to close connection */
		dbus_connection_close(this->dbus_connection);
	}
}
