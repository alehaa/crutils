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


/* include header-files
 */
#include "lxinput.h"

#include <fcntl.h>
#include <syslog.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <linux/input.h>


/** \brief Open device-connection
 *
 * \details Try to open the device named in \p device and return a new
 *  filedescriptor on success.
 *
 *
 * \param device C-string containing the path of the device to be opened *
 *
 * \return Returns the new file-descriptor on success. On error, -1 is returned
 *  and an error message will be send to syslog.
 */
int
crutilsd_device_open (const char* device)
{
	/* Try to open device. If this operation fails, return -1 and log an error-
	 * message to syslog.
	 */
	int fd = open(device, O_RDONLY);
	if (fd < 0) {
		syslog(LOG_ERR, "[%s] unable to open device-file '%s': %s",
			"lxinput", device, strerror(errno));

		return -1;
	}

	/* Try to get exclusive rights for this device. Otherwise e.g. X11 might
	 * get the same data as HID-input-event and print it as keyboard input.
	 */
	if (ioctl(fd, EVIOCGRAB, 1) < 0) {
		syslog(LOG_ERR, "[%s] unable to grab device-file '%s' exclusive: %s",
			"lxinput", device, strerror(errno));

		// close device handler
		if (close(fd) < 0) {
			syslog(LOG_ERR, "[%s] unable to close device-file '%s': %s",
				"lxinput", device, strerror(errno));
		}

		return -1;
	}


	// return file-descriptor
	return fd;
}
