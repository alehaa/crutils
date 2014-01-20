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
#include "dbus.h"

#include <syslog.h>


/* constructor ()
 * init this class
 */
crutilsd_dbus::crutilsd_dbus () {
	this->dbus_connection = NULL;
	dbus_error_init(&this->dbus_error);
}


/* destructor ()
 *
 * close dbus-connection and -error if exist
 */
crutilsd_dbus::~crutilsd_dbus () {
	//if(this->dbus_connection = NULL) dbus_bus_close(this->dbus_connection);
	//dbus_error_init(&this->dbus_error);
}


/* connect ()
 * try to connect to DBUS-Daemon and return result
 */
bool crutilsd_dbus::connect () {
	this->dbus_connection = dbus_bus_get(DBUS_BUS_SYSTEM, &this->dbus_error);
	if (this->dbus_connection) return true;

	/* an error occured */
	syslog(LOG_ERR, "Failed to connect to D-BUS daemon: %s\n", this->dbus_error.message);
	dbus_error_free(&this->dbus_error);

	return false;
}


/* send_code (const char *p_code)
 * sends the code to the D-BUS where applications could fetch it */
void crutilsd_dbus::send_code (const char *p_code) {
	DBusMessage *message;

	message = dbus_message_new_signal("/system/codereader/read", "system.codereader", "read");

	dbus_message_append_args(message, DBUS_TYPE_STRING, &p_code, DBUS_TYPE_INVALID);

	dbus_connection_send(this->dbus_connection, message, NULL);
	dbus_connection_flush(this->dbus_connection);
	dbus_message_unref(message);
}
