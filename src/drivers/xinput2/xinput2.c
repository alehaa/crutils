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
 *  2013-2017 Alexander Haase <ahaase@alexhaase.de>
 */

/** \file
 *
 * \brief Driver for fetching keyboard events from the X-server.
 *
 * \details This driver attaches to the X-server session and grabs selected
 *  keyboards. Their input will be parsed and translated to ASCII text, which
 *  will be send to the codereader stream.
 *
 * \note Unlike other drivers, only one invocation is needed for several devices
 *  in the current X-session.
 */

#include <assert.h>  // assert
#include <stdbool.h> // bool, true, false
#include <stdlib.h>  // free, malloc
#include <string.h>  // memset, strdup

#include <X11/XKBlib.h>             // X11 xkb extension API
#include <X11/Xlib.h>               // X11 API
#include <X11/extensions/XInput2.h> // X11 xinput2 extension API
#include <libconfig.h>              // libconfig API


/** \brief Prefix for error messages of this driver.
 */
#define MESSAGE_PREFIX "[codereader-xinput2] "


/** \brief Storage for driver related information.
 */
struct codereader_xinput2_cookie
{
	Display *display; ///< Pointer to the struct for the X-server connection.
	int xi_opcode;    ///< Major opcode of X11 XI extension.
	char **match_devices;  ///< Which device names to match.
	int match_devices_num; ///< Number of entries in \ref match_devices.
};


/** \brief Check if all required extensions are loaded into the X-server.
 *
 *
 * \param cookie Pointer to device data storage.
 *
 * \return If all required extensions are available true will be returned,
 *  otherwise false.
 */
static bool
check_x_extensions(struct codereader_xinput2_cookie *cookie)
{
	int evcode, error;
	if (!XQueryExtension(cookie->display, "XInputExtension",
	                     &(cookie->xi_opcode), &evcode, &error)) {
		fprintf(stderr, MESSAGE_PREFIX "X Input extension is not available.\n");
		return false;
	}

	int opcode;
	if (!XQueryExtension(cookie->display, "XKEYBOARD", &opcode, &evcode,
	                     &error)) {
		fprintf(stderr,
		        MESSAGE_PREFIX "X keyboard extension is not available.\n");
		return false;
	}

	return true;
}


/** \brief Read the related configuration for this device.
 *
 * \details This function reads the related configuration for this device and
 *  saves it in the persistent \p cookie.
 *
 *
 * \param config Pointer to device configuration.
 * \param cookie Pointer to device data storage.
 *
 * \return If reading the configuration was successful, true will be returned,
 *  otherwise false.
 */
static bool
read_config(const config_setting_t *config,
            struct codereader_xinput2_cookie *cookie)
{
	/* Get the match-array listing all input devices that should be grabbed by
	 * this driver. If the entry is not available in the configuration, an error
	 * will be returned and an error message printed. */
	config_setting_t *matches =
	    config_setting_lookup((config_setting_t *)config, "match");
	if (matches == NULL) {
		fprintf(stderr, MESSAGE_PREFIX "Missing config option 'match'.\n");
		return false;
	}

	/* Allocate memory to store all data of the match-array in a C-array. This
	 * has to be done, as the parsed config structure will be destroyed after
	 * all devices have been opened, but we'd like to grab new devices, when
	 * they are connected. */
	cookie->match_devices_num = config_setting_length(matches);
	if (cookie->match_devices_num == 0) {
		fprintf(stderr, MESSAGE_PREFIX
		        "Config option 'match' needs to be a non-empty array.\n");
		return false;
	}

	cookie->match_devices = malloc(cookie->match_devices_num * sizeof(char *));
	if (cookie->match_devices == NULL) {
		fprintf(stderr,
		        MESSAGE_PREFIX "Failed to allocate memory for match-array.\n");
		return false;
	}
	memset(cookie->match_devices, 0,
	       sizeof(char *) * cookie->match_devices_num);

	/* Duplicate the entries of the matches in the configuration and store the
	 * copy in the internal data storage cookie, so it can be accessed at any
	 * later time. */
	for (int i = 0; i < cookie->match_devices_num; i++) {
		const char *p = config_setting_get_string_elem(matches, i);
		if (p[0] == '\0') {
			fprintf(stderr,
			        MESSAGE_PREFIX "Match-entry %d must not be empty.\n", i);
			return false;
		}
		if ((cookie->match_devices[i] = strdup(p)) == NULL) {
			fprintf(stderr, MESSAGE_PREFIX "Failed to copy match-entry.\n");
			return false;
		}
	}

	return true;
}


/** \brief Grab all matching devices.
 *
 * \details This function will check the list of devices of the current X-server
 *  session and grabs all devices with a name matching the match-list of the
 *  loaded configuration.
 *
 *
 * \param cookie Pointer to device data storage.
 *
 * \return If grabbing all devices was successful, true will be returned,
 *  otherwise false.
 */
static bool
grab_devices(struct codereader_xinput2_cookie *cookie)
{
	/* Get a list of all devices connected in the current X-session. If the list
	 * can't be obtained, an error message will be printed and an error
	 * returned. */
	int num_devices;
	XIDeviceInfo *devices =
	    XIQueryDevice(cookie->display, XIAllDevices, &num_devices);
	if (devices == NULL) {
		fprintf(stderr, MESSAGE_PREFIX "Failed to get device list.\n");
		return false;
	}

	/* Create a new device mask to be used for grabbing devies later. */
	XIEventMask mask = {.deviceid = XIAllDevices,
	                    .mask_len = XIMaskLen(XI_KeyPress)};
	mask.mask = calloc(mask.mask_len, sizeof(char));
	if (mask.mask == NULL) {
		fprintf(stderr, MESSAGE_PREFIX
		        "Failed to allocate memory for device event mask.\n");
		goto free_devices;
	}
	XISetMask(mask.mask, XI_KeyPress);


	/* Iterate over the list of devices. If the name of a device includes one of
	 * the matching strings, the device will be assumed to be a codereader and
	 * will tried to be grabbed. */
	for (int i = 0; i < num_devices; i++) {
		/* We're only interested in keyboards. Other pointing devices like mice
		 * can savely be ignored. This will ignore already grabbed devices, too,
		 * as these have the type 'XIFloatingSlave'. */
		if (devices[i].use != XIMasterKeyboard &&
		    devices[i].use != XISlaveKeyboard)
			continue;

		for (int j = 0; j < cookie->match_devices_num; j++)
			if (strstr(devices[i].name, cookie->match_devices[j]) != NULL)
				/* Try to grab the device. There is no check needed, if the
				 * device is already grabbed, as XIGrabDevice will simply ignore
				 * it in this case. If grabbing the device fails, an error
				 * message will be printed and an error code returned. */
				if (XIGrabDevice(cookie->display, devices[i].deviceid,
				                 DefaultRootWindow(cookie->display),
				                 CurrentTime, None, GrabModeAsync,
				                 GrabModeAsync, False, &mask) != GrabSuccess) {
					fprintf(stderr,
					        MESSAGE_PREFIX "Failed to grab device '%s'.\n",
					        devices[i].name);
					goto free_mask;
				}
	}
	free(mask.mask);
	XIFreeDeviceInfo(devices);

	return true;


free_mask:
	free(mask.mask);
free_devices:
	XIFreeDeviceInfo(devices);
	return false;
}


/** \brief Connects to the X-server session and grabs all matching codereaders.
 *
 * \details This function opens a connection to the current X-server session and
 *  loads the neccessary configuration. All required events will be registered
 *  and (if already connected) codereaders grabbed to get their input exclusive.
 *
 *
 * \param config Pointer to device configuration.
 * \param cookie Pointer to device data storage.
 *
 * \return On success a file descriptor for the connection to the X-server will
 *  be returned, otherwise -1.
 */
int
device_open(const config_setting_t *config,
            struct codereader_xinput2_cookie **cookie)
{
	/* Allocate memory for the internal cookie. We'll register the memory in the
	 * cookie pointer first, so errors don't have to be specially handled in
	 * this function, as the close function will be called on errors
	 * automatically, which frees the allocated memory. */
	*cookie = malloc(sizeof(struct codereader_xinput2_cookie));
	if (*cookie == NULL) {
		fprintf(stderr,
		        MESSAGE_PREFIX "Failed to allocate memory for cookie.\n");
		return -1;
	}
	memset(*cookie, 0, sizeof(struct codereader_xinput2_cookie));

	/* Open a connection to the X11 server. The display from the DISPLAY
	 * environment variable will be used. */
	if (((*cookie)->display = XOpenDisplay(NULL)) == NULL) {
		fprintf(stderr, MESSAGE_PREFIX "Failed to open display connection.\n");
		return -1;
	}
	if (!check_x_extensions(*cookie))
		return -1;

	/* Register hierarchy changed events, which will be triggered, if a device
	 * is con- or deconnected. They will be used to grab new devices, after the
	 * initial open call has been done. */
	XIEventMask hierarchy_mask = {.deviceid = XIAllDevices,
	                              .mask_len = XIMaskLen(XI_HierarchyChanged)};
	hierarchy_mask.mask = calloc(hierarchy_mask.mask_len, sizeof(char));
	if (hierarchy_mask.mask == NULL) {
		fprintf(stderr, MESSAGE_PREFIX
		        "Failed to allocate memory for hierarchy event mask.\n");
		return -1;
	}
	XISetMask(hierarchy_mask.mask, XI_HierarchyChanged);
	XISelectEvents((*cookie)->display, DefaultRootWindow((*cookie)->display),
	               &hierarchy_mask, 1);
	free(hierarchy_mask.mask);

	/* Read all required data from the configuration and try to grab all devices
	 * matching the matches array in the config. If an error occures in one of
	 * the calls, an error will be returned, but no extra error message printed,
	 * as the functions did already. */
	if (!read_config(config, *cookie) || !grab_devices(*cookie))
		return -1;

	/* Flush all actions to the X11 server. This is required, as this will be
	 * done automatically only when calling XNextEvent and similar functions. As
	 * this function will be called only after new data was detected by the
	 * codereader library, the events which will notify it would never be
	 * registered.
	 *
	 * A file descriptor will be returned, so the codereader library can monitor
	 * multiple codereaders in parallel. */
	XFlush((*cookie)->display);
	return ConnectionNumber((*cookie)->display);
}


/** \brief Parse the X-server events to a valid code.
 *
 *
 * \param fd File descriptor of the X-server connection (ignored).
 * \param buffer Where to store read data.
 * \param size Size of \p buffer.
 * \param cookie Pointer to the driver's data storage.
 *
 * \return On success the number of read bytes will be returned, if the event
 *  was ignored zero and on errors -1.
 */
int
device_read(int fd, char *buffer, int size,
            struct codereader_xinput2_cookie *cookie)
{
	assert(cookie);


	/* Process all pending events from the X-server session. The loop will run
	 * for at least one event and stops after either a hierarchy event occured,
	 * or a complete barcode has been read. */
	XEvent ev;
	int num_read = 0;
	while (true) {
		XGenericEventCookie *event = &ev.xcookie;
		XNextEvent(cookie->display, &ev);

		/* We are only interested in events from the xinput2 extension. If
		 * either the event has no data, or the event is not relevant, it will
		 * be ignored. */
		if (!XGetEventData(cookie->display, event) ||
		    event->type != GenericEvent ||
		    event->extension != cookie->xi_opcode) {
			/* If recent events have been parsed yet and the code is not full
			 * read, just skip this event but don't return. */
			if (num_read > 0)
				continue;
			else
				return 0;
		}

		switch (event->evtype) {
			/* If the hierarchy changed, check the device list for new devices
			 * and try to grab them. If this fails, return an error, otherwise
			 * tell libcodereader that this event was not a read code. */
			case XI_HierarchyChanged:
				XFreeEventData(cookie->display, event);
				if (grab_devices(cookie)) {
					/* If recent events have been parsed yet and the code is not
					 * full read, just skip this event but don't return. */
					if (num_read > 0)
						continue;
					else
						return 0;
				} else
					return -1;

			/* If a key was pressed, parse the key event. If a key has been
			 * composed, add this key to the buffer and process the next event,
			 * until reading the code has been finished. */
			case XI_KeyPress: {
				/* Get the keyboard layout for this barcode reader. */
				XIDeviceEvent *kev = event->data;
				XkbDescPtr kbd = XkbGetKeyboard(
				    cookie->display, XkbAllComponentsMask, kev->deviceid);

				/* Translate the keycode into a keysym. This keysym will be
				 * translated into the composed ASCII output which will be
				 * filled into the buffer. */
				KeySym keysym;
				bool end_of_code = false;
				if (XkbTranslateKeyCode(kbd, kev->detail, kev->mods.effective,
				                        NULL, &keysym))
					if (XkbTranslateKeySym(
					        cookie->display, &keysym, kev->mods.effective,
					        buffer + num_read, size - num_read, NULL) > 0) {
						/* If the end of the code is detected, set the last byte
						 * to a newline and add an extra terminating null-
						 * character. */
						if (buffer[num_read] == '\r') {
							buffer[num_read] = '\n';
							end_of_code = true;
						}
						num_read++;
					}
				XkbFreeKeyboard(kbd, XkbAllComponentsMask, True);

				/* If the end of code was detected, finish parsing the X-server
				 * events and return the number of read bytes. */
				if (end_of_code) {
					XFreeEventData(cookie->display, event);
					return num_read;
				}

				break;
			}
		}
		XFreeEventData(cookie->display, event);
	}

	/* The program should never reach this region, so return an error if it
	 * does. */
	return -1;
}


/** \brief Close the connection to the X-server and free allocated memory.
 *
 *
 * \param fd The file descriiptor to close.
 * \param cookie Pointer to device data storage.
 *
 * \return On success zero will be returned, otherwise -1.
 */
int
device_close(int fd, struct codereader_xinput2_cookie *cookie)
{
	/* If no storage for cookie has been reserved yet, nothing has to be freed
	 * and this function has nothing to do. */
	if (cookie == NULL)
		return 0;

	/* If a connection to the X-server is open, close the connection. Grabbed
	 * devices don't have to be ungrabbed, as this will be done automatically
	 * when closing the connection. */
	if (cookie->display != NULL)
		XCloseDisplay(cookie->display);

	/* If the cookie contains a list of devices to match, free this list. */
	if (cookie->match_devices != NULL) {
		for (int i = 0; i < cookie->match_devices_num; i++)
			if (cookie->match_devices[i] != NULL)
				free(cookie->match_devices[i]);
		free(cookie->match_devices);
	}

	/* Free the whole cookie. */
	free(cookie);

	return 0;
}
