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

#include "lxinput.h"

#include <unistd.h>
#include <linux/input.h>


/** \brief Read single code from file-descriptor \p device_fd and stores it  in
 *  \p buffer
 *
 * \details Reads a single code from file-descriptor givven by \p device_fd
 *  until barcode ends and copies it into \p buffer.
 *
 *
 * \param fd file-descriptor for opened device-file
 * \param buffer pointer to an array of char where code should be stored
 * \param size maximum bytes to be read
 * \param cookie Data cookie (unused)
 *
 * \return On success, the number of bytes read is returned. On any error, a
 *  negative value inidicating the error will be returned.
 */
int
codereader_read(int fd, char *buffer, int size, void *cookie)
{
	struct input_event ev;
	int key_press_counter = 0;
	size_t num = 0;


	while (num < size) {
		// read one input event
		if (read(fd, &ev, sizeof(ev)) < (ssize_t)sizeof(ev))
			return ERR_READ;


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
				if (*buffer == '\n')
					break;

				// increment buffer position
				buffer++;
			}
		}
	}

	return num;
}
