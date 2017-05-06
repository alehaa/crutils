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

/* This header file is for internal functions, which should not be used public.
 * Functions defined in this header are non-stable and may change in future
 * releases! */

#ifndef CODEREADER_STREAM_H
#define CODEREADER_STREAM_H


#include <stdio.h> // ssize_t, size_t

#include "driver.h"


typedef struct codereader_cookie
{
	int fd; ///< Device filedescriptor.

	codereader_driver driver; ///< Driver used for device.
} codereader_cookie;


ssize_t codereader_read(void *cookie, char *buf, size_t size);
int codereader_close(void *cookie);


#endif
