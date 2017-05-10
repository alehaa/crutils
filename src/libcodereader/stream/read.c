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

#include "stream.h"

#include <unistd.h> // read

#include "internal.h" // CODEREADER_INTERNAL


/** \brief Read data from codereader device.
 *
 *
 * \param cookie \ref codereader_cookie current status storage struct.
 * \param buf Destination buffer.
 * \param size Size of \p buf.
 *
 * \return Number of integers read.
 * \return A negative value indicates an error.
 */
CODEREADER_INTERNAL
ssize_t
codereader_read(void *cookie, char *buf, size_t size)
{
	codereader_cookie *c = (codereader_cookie *)cookie;

	/* Read data from the device file with the provided driver hook. If no hook
	 * is provided by the driver, we'll use the system's read syscall. */
	if (c->driver.read != NULL)
		return c->driver.read(c->fd, buf, size);
	else
		return read(c->fd, buf, size);
}
