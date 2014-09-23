/* This file is part of libcrutils1.
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
