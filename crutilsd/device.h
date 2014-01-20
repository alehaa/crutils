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

/* device
 *
 * behandelt das Device und liesst es aus. Verarbeitet zudem die Eingabe und
 * schickt dann den im buffer gespeicherten Code an eine zuvor definierte
 * Funktion weiter.
 */

#ifndef CRUTILSD_DEVICE_H
#define CRUTILSD_DEVICE_H

#include "dbus.h"

class crutilsd_device {
	public:
		crutilsd_device();

		bool open_device (const char *p_device);
		void listen ();
		void set_dbus (crutilsd_dbus *p_dbus);

	protected:
		const char keytoc (struct input_event *p_ev);

	private:
		int file;
		crutilsd_dbus *dbus_connection;
};

#endif
