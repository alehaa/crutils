/*
 * include header files
 */
#include "crutils.h"

#include <cstdio>
#include <cstring>


/* listen ()
 *
 * start listening thread and listen for incomming codes
 */
bool crutils::listen () {
	int status = pthread_create(&this->thread, NULL, crutils::listener, this);
	if (status == 0) {
		return true;
	}

	return false;
}


/* listener ()
 *
 * function that is executed as thread. This function will listen to dbus and
 * filter all codes
 */
void *crutils::listener (void *p_args) {
	crutils *t_this = (crutils *)p_args;

	/* add filter and wait until dbus sidconnects (or thread is killed) */
	if (dbus_connection_add_filter(t_this->dbus_connection, crutils::parse_dbus_signal, t_this, NULL)) {
		while (dbus_connection_read_write_dispatch(t_this->dbus_connection, -1));
	}

	return NULL;
}


/* parse_signal ()
 *
 * function that extracts the code of the dbus signal
 */
DBusHandlerResult crutils::parse_dbus_signal (DBusConnection *p_connection, DBusMessage *p_message, void *p_user_data) {
	if (!dbus_message_is_signal (p_message, DBUS_INTERFACE_LOCAL, "Disconnected")) {
		/* test interface, because dbus internal signals are not affected by the filter,
		 * but we dont want to read them */
		if (strcmp(dbus_message_get_interface(p_message), "system.codereader") == 0) {
			DBusMessageIter iter;
			dbus_message_iter_init(p_message, &iter);

			do {
				int type = dbus_message_iter_get_arg_type (&iter);

				if (type == DBUS_TYPE_STRING) {
					char *val;
					dbus_message_iter_get_basic (&iter, &val);

					crutils *t_this = (crutils *)p_user_data;
					if(t_this->code_handler != NULL) t_this->code_handler(val);

					break;
				}
			} while (dbus_message_iter_next(&iter));
		}
	}

	/* Conceptually we want this to be DBUS_HANDLER_RESULT_NOT_YET_HANDLED, but this
	 * raises some problems. See bug 1719. */
	return DBUS_HANDLER_RESULT_HANDLED;
}
