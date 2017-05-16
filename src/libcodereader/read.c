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

#include <stdio.h>      // fprintf
#include <sys/select.h> // select and FD_* macros

#include "device.h"   // codereader_device*
#include "internal.h" // CODEREADER_INTERNAL, CODEREADER_MESSAGE_PREFIX


/** \brief Read data from codereader devices.
 *
 * \details This function behaves like a proxy to the system's `read` function.
 *  It will read one barcode from all devices defined in \p cookie and route the
 *  request to one of the opened devices ready to read data from.
 *
 * \note As the purpose of this function is to read a whole barcode, the buffer
 *  needs to big enough to store a whole barcode plus the terminating null-
 *  character. Otherwise the driver's function will return an error.
 *
 *
 * \param cookie Headpointer of \ref codereader_device list.
 * \param buf Destination buffer.
 * \param size Size of \p buf.
 *
 * \return Number of integers read.
 * \return A negative value indicates an error.
 */
CODEREADER_INTERNAL
CODEREADER_READ_RETURN_TYPE
codereader_read(void *cookie, char *buf, CODEREADER_READ_SIZE_TYPE size)
{
	/* Restore the device list. Cookie is a pointer to the first element in the
	 * list, so it will be set as first element of a temporary list. The list-
	 * management-pointers in the cookie remain, so the list can be fully
	 * restored with the cookie. */
	struct codereader_device_list devices = SLIST_HEAD_INITIALIZER(devices);
	SLIST_FIRST(&devices) = cookie;

	/* Build a list of all file descriptors, so a select can be done on them
	 * below. */
	fd_set fds;
	FD_ZERO(&fds);
	int fd_max = 0;
	struct codereader_device *iter;
	SLIST_FOREACH(iter, &devices, lmp)
	{
		FD_SET(iter->fd, &fds);
		if (iter->fd > fd_max)
			fd_max = iter->fd;
	}

listen_devices:
	/* Do a select on all device file descriptors, to wait for available data on
	 * any of them. There will be no time-limit. Only one file descriptor will
	 * be processed, even if more than one is ready for reading, as this
	 * function should only return one barcode at a time. The next barcode will
	 * be fetched by the next call to this function. */
	if (select(fd_max + 1, &fds, NULL, NULL, NULL) < 0) {
		fprintf(stderr, CODEREADER_MESSAGE_PREFIX
		        "Failed to select a device file descriptor.\n");
		return -1;
	}

	/* Check which device is ready for reading now. The first matching device
	 * will be used and data be returned to the user. */
	SLIST_FOREACH(iter, &devices, lmp)
	{
		if (FD_ISSET(iter->fd, &fds)) {
			int ret = iter->driver.read(iter->fd, buf, size, iter->cookie);
			if (ret == 0)
				goto listen_devices;
			return ret;
		}
	}

	/* If the function reached this point, a fatal error occured, as select
	 * returned with no file descriptor set ready to read. */
	fprintf(stderr, CODEREADER_MESSAGE_PREFIX
	        "Select set no valid file descriptors.\n");
	return -1;
}
