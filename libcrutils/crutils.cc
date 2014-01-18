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
	this->code_handler = NULL;
}


/* destructor ()
 *
 * close all open connections
 */
crutils::~crutils () {
//	this->disconnect();
}


/* connect ()
 *
 * try to connect to DBUS-Daemon and return result
 */
bool crutils::connect () {
	DBusError dbus_error;
	dbus_error_init(&dbus_error);
	this->dbus_connection = dbus_bus_get(DBUS_BUS_SYSTEM, &dbus_error);
	if (this->dbus_connection) {
		/* we only want to recive signals by 'system.codereader' with member 'read' */
		dbus_bus_add_match(this->dbus_connection, "type='signal',interface='system.codereader',member='read'", &dbus_error);
		if (!dbus_error_is_set(&dbus_error)) {
			return true;
		}
	}

	/* an error occured */
	dbus_error_free(&dbus_error);
	return false;
}


/* set_handler ()
 *
 * sets handler function, which uses the found codes
 */
void crutils::set_handler (void (*p_handler) (const char * p_code)) {
	this->code_handler = p_handler;
}
