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

#include "driver.h"

#include <assert.h> // assert
#include <dlfcn.h>  // dlopen, dlsym
#include <stdio.h>  // snprintf, FILENAME_MAX

#include "cmake-config.h"        // CRUTILS_DRIVER_DIR
#include "codereader-internal.h" // CMAKE_INTERNAL


/** \brief Map a \ref codereader_driver hook to a dynamic symbol.
 *
 * \details This macro maps the dynamic symbol \p symbol in a freshly opened
 *  driver to its pointer location in the \ref codereader_driver struct.
 *
 *
 * \param target Destination variable name in \ref codereader_driver struct.
 * \param symbol Symbol name.
 */
#define codereader_map_symbol(target, symbol) \
	dlerror();                                \
	*(void **)(&(dest->target)) = dlsym(dest->dh, symbol);


/** \brief Load driver \p name.
 *
 * \details This function loads the driver \p name and maps all required symbols
 *  to the \ref codereader_driver struct \p dest.
 *
 *
 * \param name Driver name.
 * \param dest Pointer to \ref codereader_driver to store the information in.
 *
 * \return true The driver was loaded successfully.
 * \return false The driver could not be loaded. This means, the driver is not
 *  available or symbols in the driver file could not be resolved.
 */
CODEREADER_INTERNAL
bool
codereader_driver_load(const char *name, codereader_driver *dest)
{
	/* Check all function parameters, if they are valid. */
	assert(name);


	/* Load the reqested driver. RTLD_NOW will be used, to resolve all symbols
	 * before using the driver, so that there can't be any resolving issues at a
	 * later time. */
	char buffer[FILENAME_MAX];
	snprintf(buffer, FILENAME_MAX, "%s/%s.so", CODEREADER_DRIVER_DIR, name);
	dest->dh = dlopen(buffer, RTLD_NOW);
	if (dest->dh == NULL)
		return false;

	/* Symbolize the hook functions. If a hook function is not present (dlerror)
	 * is not NULL, ignore the error, because all hooks are optional and may be
	 * undefined. */
	codereader_map_symbol(open, "codereader_open");
	codereader_map_symbol(close, "codereader_close");
	codereader_map_symbol(read, "codereader_read");
	codereader_map_symbol(strerror, "codereader_strerror");

	return true;
}
