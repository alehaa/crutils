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

#include "driver.h"

#include <assert.h> // assert
#include <dlfcn.h>  // dlclose

#include "codereader-internal.h" // CMAKE_INTERNAL


/** \brief Unmap a \ref codereader_driver hook.
 *
 * \details This macro maps the hook pointer in \p driver to NULL. This should
 *  give an extra level of security for programmers to not use hook symbols
 *  accidentialy after unloading, if the shared object of the driver keeps in
 *  memory.
 *
 *
 * \param target Destination variable name in \ref codereader_driver struct.
 */
#define codereader_unmap_symbol(target) driver->target = NULL;


/** \brief Unload driver \p name.
 *
 * \details This function unloads the driver \p name. The loaded shared object
 *  will be closed and all mapped symbols set to NULL.
 *
 *
 * \param driver Pointer to driver.
 *
 * \return true The driver was unloaded successfully.
 * \return false An error occured while unloading the driver.
 */
CODEREADER_INTERNAL
bool
codereader_driver_unload(codereader_driver *driver)
{
	/* Check all function parameters, if they are valid. */
	assert(driver);


	/* Close the shared object handle. There is no guarantee, that the shared
	 * object will be closed really, because other drivers or libraries may have
	 * dependencies to this one, so the reference counter will only be decre-
	 * mented. To gain an extra level of security for source code errors, all
	 * driver hook symbols will be set to NULL, so future calls to the hooks
	 * will fail. */
	int ret = dlclose(driver->dh);

	codereader_unmap_symbol(open);
	codereader_unmap_symbol(close);
	codereader_unmap_symbol(read);
	codereader_unmap_symbol(strerror);

	return (ret == 0) ? true : false;
}
