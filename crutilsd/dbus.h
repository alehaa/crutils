/* die Klasse barcode_server_dbus ist fuer die Kommunikation zwischen dieser
 * Anwendung und dbus verantwortlich. */

#ifndef BARCODE_SERVER_DBUS
#define BARCODE_SERVER_DBUS

#include <dbus/dbus.h>

class crutilsd_dbus {
	public:
		crutilsd_dbus();
		~crutilsd_dbus();

		bool connect();
		void send_code (const char *p_code);


	protected:
		DBusConnection *dbus_connection;
		DBusError dbus_error;
};

#endif

