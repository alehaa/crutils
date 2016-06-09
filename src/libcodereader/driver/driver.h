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

#ifndef CODEREADER_DRIVER_H
#define CODEREADER_DRIVER_H


#include <stdbool.h>
#include <stdio.h>


typedef int (*codereader_driver_hook_open)(const char *path);
typedef int (*codereader_driver_hook_close)(const int fd);
typedef ssize_t (*codereader_driver_hook_read)(const int fd, char *buffer,
                                               size_t size);
typedef const char *(*codereader_driver_hook_strerror)(const int errno);


typedef struct codereader_driver
{
	void *dh; ///< Handle for the loaded library.


	/** \brief Hook provided by the driver to open the device file.
	 *
	 * \details If set to NULL, the hook will be ignored and the system's open
	 *  will be used.
	 */
	codereader_driver_hook_open open;

	/** \brief Hook provided by the driver to close the device file.
	 *
	 * \details If set to NULL, the hook will be ignored and the system's open
	 *  will be used.
	 */
	codereader_driver_hook_close close;

	/** \brief Hook provided by the driver to read data from the device file.
	 *
	 * \details If set to NULL, the hook will be ignored and the system's open
	 *  will be used.
	 */
	codereader_driver_hook_read read;

	/** \brief Hook provided by the driver to translate an errno to a string.
	 *
	 * \details If set to NULL, the hook will be ignored and the system's open
	 *  will be used.
	 */
	codereader_driver_hook_strerror strerror;
} codereader_driver;


bool codereader_driver_load(const char *name, codereader_driver *dest);
bool codereader_driver_unload(codereader_driver *driver);

#endif
