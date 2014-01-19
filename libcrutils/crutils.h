/* libcrutils1
 *
 * client library to fetch all codes bradcasted by crutilsd via dbus
 *
 * (c) 2013 Alexander Haase <alexander.haase@rwth-aachen.de>
 */

#ifndef CRUTILS
#define CRUTILS

#include <dbus/dbus.h>
#include <pthread.h>

class crutils {
	public:
		crutils ();
		~crutils ();

		void set_handler (void (*p_handler) (const char * p_code));

		bool listen ();
		bool stop();

		void mute();
		void unmute();

	protected:
		bool connect ();

		DBusConnection *dbus_connection;
		bool muted;

	private:
		pthread_t thread;
		static void *listener (void *p_args);
		static DBusHandlerResult parse_dbus_signal (DBusConnection *p_connection, DBusMessage *p_message, void *p_user_data);

		void (*code_handler) (const char* p_code);
};

#endif
