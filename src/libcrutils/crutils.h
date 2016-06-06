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
