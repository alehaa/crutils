/* libcrutils1
 *
 * client library to fetch all codes bradcasted by crutilsd via dbus
 *
 * (c) 2013 Alexander Haase <alexander.haase@rwth-aachen.de>
 */

#ifndef CRUTILS
#define CRUTILS

class crutils {
	public:
		crutils();
		~crutils();

	protected:

	private:
		DBusConnection *dbus_connection;
}

#endif
