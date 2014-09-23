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
 * You should have received a copy of the GNU General Public License and GNU
 * Lesser General Public License along with this program. If not, see
 *
 *  http://www.gnu.org/licenses/
 *
 *
 * Copyright (C)
 *  2013-2014 Alexander Haase <alexander.haase@rwth-aachen.de>
 */


/* include header-files
 */
#include "lxinput.h"

#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <errno.h>
#include <linux/input.h>


/** \brief Read single code from file-descriptor \p device_fd and stores it  in
 *  \p buffer
 *
 * \details Reads a single code from file-descriptor givven by \p device_fd
 *  until barcode ends and copies it into \p buffer.
 *
 *
 * \param device_fd file-descriptor for opened device-file
 * \param buffer pointer to an array of char where code should be stored
 * \param num maximum bytes to be read
 *
 * \return On success, the number of bytes read is returned.  On error, -1 is
 *  returned and an error message will be send to syslog.
 */
ssize_t
crutilsd_device_read (int device_fd, char *buffer, size_t length)
{
	struct input_event ev;
	size_t input_event_size = sizeof(ev);
	int key_press_counter = 0;
	size_t num = 0;


	while (num < length) {
		// read one input event
		if (read(device_fd, &ev, input_event_size) < input_event_size) {
			syslog(LOG_ERR, "[%s] unable to read code from device-file '%d': %s",
				"lxinput", device_fd, strerror(errno));

			return -1;
		}


		// ignore any event other than EV_KEY
		if (ev.type != EV_KEY)
			continue;


		/* handle key-presses. On each key-press, the pressed key will affect
		 * the value at the current buffer position (e.g. pressing uppercase
		 * after KEY_A will change 'a' to 'A') and the key-press-counter will
		 * be incremented.
		 */
		if (ev.value == 1) {
			*buffer = keytoc(&ev);

			// increment key-press-counter
			key_press_counter++;
		}


		/* handle key-releases. For each key-press, there will be a key-release
		 * event. If all keys are released, the current buffer positition is
		 * finished and will not be changed anymore. If the current character
		 * in buffer is '\0', this was the last character of the code and no
		 * processing is needed anymore, so this function is able to return.
		 * Otherwise the current buffer position will be seeked one character
		 * forwards, so a new character could be read.
		 */
		if (ev.value == 0) {
			// decremtent key-press-counter
			key_press_counter--;

			// character reading was finished now
			if (key_press_counter == 0) {
				// increment read characters
				num++;

				// check, if this was the last character of read code
				if (*buffer == '\0')
					break;

				// increment buffer position
				buffer++;
			}
		}
	}

	return num;
}
