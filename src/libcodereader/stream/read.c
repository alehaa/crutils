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

#include "stream.h"

#include <string.h>

#include "codereader-internal.h" // CODEREADER_INTERNAL


/** \brief Read data from codereader device.
 *
 *
 * \param cookie Unused.
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
	strncpy(buf, "Hello\0", size);
	return 7;
}
