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

/* die Klasse barcode_server_dbus ist fuer die Kommunikation zwischen dieser
 * Anwendung und dbus verantwortlich. */

#ifndef BARCODE_SERVER_DBUS
#define BARCODE_SERVER_DBUS

#include <dbus/dbus.h>

#include "log.h"

class crutilsd_dbus {
	public:
		crutilsd_dbus(crutilsd_log *p_log);
		~crutilsd_dbus();

		bool connect();
		void send_code (const char *p_code);

	protected:
		DBusConnection *dbus_connection;
		DBusError dbus_error;

		crutilsd_log *log;
};

#endif
