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

#include <dlfcn.h>  // dlclose
#include <stdlib.h> // free

#include "device.h"   // codereader_device*
#include "internal.h" // CODEREADER_INTERNAL


/** \brief Close the codereader stream.
 *
 * \details This function will close all opened devices and frees all internal
 *  memory allocated by previous function calls.
 *
 *
 * \param cookie Headpointer of \ref codereader_device list.
 *
 * \return 0 All devices successfully destroyed.
 * \return -1 An error occured.
 */
CODEREADER_INTERNAL
int
codereader_close(void *cookie)
{
	/* Restore the device list. Cookie is a pointer to the first element in the
	 * list, so it will be set as first element of a temporary list. The list-
	 * management-pointers in the cookie remain, so the list can be fully
	 * restored with the cookie. */
	struct codereader_device_list devices = SLIST_HEAD_INITIALIZER(devices);
	SLIST_FIRST(&devices) = cookie;

	/* Iterate over the whole list and call the close function for all loaded
	 * drivers. After the device has been closed, the loaded driver's shared
	 * object will be unloaded and the allocated memory freed. If an error
	 * happens while closing or unloading, the other drivers still will be
	 * closed, but an error will be returned at the end of the function.
	 *
	 * Note: The device has to be checked if it's fully initialized, as this
	 *       function may be called in error situations, too. */
	int ret = 0;
	struct codereader_device *iter;
	while (!SLIST_EMPTY(&devices)) {
		iter = SLIST_FIRST(&devices);
		if ((iter->driver.close != NULL) &&
		    (iter->driver.close(iter->fd, iter->cookie) != 0))
			ret = -1;
		if (iter->driver.dh != NULL)
			dlclose(iter->driver.dh);

		SLIST_REMOVE_HEAD(&devices, lmp);
		free(iter);
	}

	return ret;
}
