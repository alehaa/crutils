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

#include <stdlib.h> // free
#include <unistd.h> // close

#include "codereader-internal.h" // CODEREADER_INTERNAL
#include "driver.h"


/** \brief Close the codereader stream.
 *
 * \details This function will free all internal memory allocated by previous
 *  function calls.
 *
 *
 * \param cookie Pointer to codereader stream cookie.
 *
 * \return 0 \p cookie successfully destroyed.
 * \return EOF An error occured.
 */
CODEREADER_INTERNAL
int
codereader_close(void *cookie)
{
	codereader_cookie *c = (codereader_cookie *)cookie;

	/* Close the device file with the provided driver hook. If no hook is
	 * provided by the driver, we'll use the system's close syscall. */
	int ret;
	if (c->driver.close != NULL)
		ret = c->driver.close(c->fd);
	else
		ret = close(c->fd);


	/* Unload the device driver. */
	codereader_driver_unload(&(c->driver));


	free(cookie);

	return ret;
}
