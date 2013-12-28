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
