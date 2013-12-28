/* libcrutils1
 *
 * client library to fetch all codes bradcasted by crutilsd via dbus
 *
 * (c) 2013 Alexander Haase <alexander.haase@rwth-aachen.de>
 */

#ifndef CRUTILS
#define CRUTILS

#include <dbus/dbus.h>

class crutils {
	public:
		crutils();
		~crutils();

		bool connect();
		void disconnect();

	protected:
		DBusConnection *dbus_connection;

	private:

};

#endif
