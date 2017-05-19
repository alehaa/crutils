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


/** \brief Close device-connection
 *
 * \details Closes a file descriptor, so that it no longer refers to any device.
 *
 *
 * \param fd File-descriptor to be closed.
 * \param cookie Data cookie (unused).
 *
 * \return Returns zero on success. On any error, a negative value inidicating
 *  the error will be returned.
 */
int
device_close(int fd, void *cookie)
{
	/* Try to ungrab device, so that other processes (e.g. by X11) may receive
	 * events by this device. */
	if (ioctl(fd, EVIOCGRAB, 0) < 0)
		return ERR_UNGRAB;


	// close device handler
	if (close(fd) < 0)
		return ERR_CLOSE;


	return 0;
}
