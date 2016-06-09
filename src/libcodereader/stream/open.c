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

#define _GNU_SOURCE // required for fopencookie in stdio.h


#include "codereader.h"

#include <stdio.h>  // IO functions and types
#include <stdlib.h> // malloc
#include <fcntl.h>  // open

#include "driver.h"
#include "stream.h"


/** \brief Open a new handle to read barcodes with barcode readers.
 *
 * \details This function will setup all necessary internal data structures and
 *  read the configuration files to support an interface to read barcodes read
 *  by all connected barcode scanners with a single call to fread or similar
 *  functions.
 *
 *
 * \return Pointer to a new created file handle.
 * \return NULL An error occured.
 */
FILE *
codereader_open()
{
	/* Setup all hook functions. Thus the codereader stream is readonly and
	 * unable to seek, write and seek functions don't have to be defined. The
	 * read and close hooks will be mapped to internal codereader functions. */
	cookie_io_functions_t hooks = {0};
	hooks.read = codereader_read;
	hooks.close = codereader_close;


	/* Alllocate memory for a new cookie. The cookie will be used to save the
	 * current FILE state across multiple function calls, until it is freed by
	 * crutils_close. If malloc fails, we can return immediately, because no
	 * dynamic memoty has to be freed. */
	codereader_cookie *cookie = malloc(sizeof(codereader_cookie));
	if (cookie == NULL)
		return NULL;


	const char device[] = "/dev/null";
	const char driver[] = "none";

	/* Load the required driver for the codereader. */
	if (!codereader_driver_load(driver, &(cookie->driver)))
		goto free_cookie;


	/* Open the device file with the provided driver hook. If no hook is
	 * provided by the driver, we'll use the system's open syscall. */
	if (cookie->driver.open != NULL) {
		if ((cookie->fd = cookie->driver.open(device)) < 0)
			goto free_cookie;
	} else
		cookie->fd = open(device, O_RDONLY);


	/* Setup the new codereader stream. On success fopencookie() returns a
	 * pointer to the new stream. On error, NULL is returned, so we don't have
	 * to evaluate the result. */
	return fopencookie(cookie, "r", hooks);


free_cookie:
	free(cookie);
	return NULL;
}
