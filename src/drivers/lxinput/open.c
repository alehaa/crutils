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

#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>


/** \brief Open device-connection
 *
 * \details Try to open the device named in \p device and return a new
 *  filedescriptor on success.
 *
 *
 * \param config Pointer to device configuration.
 * \param cookie Pointer to device data storage (unused).
 *
 * \return Returns the new file-descriptor on success. On any error, a negative
 *  value inidicating the error will be returned.
 */
int
codereader_open(const config_setting_t *config, void **cookie)
{
	/* Try to open the device file. */
	const char *path;
	if (config_setting_lookup_string(config, "device", &path) != CONFIG_TRUE)
		return ERR_OPEN;
	int fd = open(path, O_RDONLY);
	if (fd < 0)
		return ERR_OPEN;

	/* Try to get exclusive rights for this device. Otherwise e.g. X11 might
	 * get the same data as HID-input-event and print it as keyboard input.
	 */
	if (ioctl(fd, EVIOCGRAB, 1) < 0) {
		/* Grab failed. Close device file descriptor and return an error. */
		close(fd);
		return ERR_GRAB;
	}

	// return file-descriptor
	return fd;
}
