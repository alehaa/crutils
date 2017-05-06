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

#include "config.h"

#include <assert.h> // assert
#include <stdio.h>  // fopen, fclose, getline
#include <stdlib.h> // free

#include "cmake-config.h"        // CRUTILS_CONFIG_FILE
#include "codereader-internal.h" // CODEREADER_INTERNAL


/** \brief Read codereader configuration file.
 *
 * \details This function reads the codereader configuration file and looks up
 *  the required driver for the specified device. Thus the configuration file is
 *  only a key value file to map device files to drivers, no external config
 *  parser library will be used.
 *
 *
 * \param device Pointer to device file name buffer.
 * \param driver Pointer to driver name buffer.
 *
 * \return true The configuration is valid and could be parsed.
 * \return false The configuration file is not valid or could not be accessed.
 */
CODEREADER_INTERNAL
bool
codereader_read_config(char *device, char *driver)
{
	/* Assert required parameters. */
	assert(device);
	assert(driver);


	/* Read the configuration file line by line and extract device file and the
	 * required driver. */
	FILE *fh = fopen(CODEREADER_CONFIG_FILE, "r");
	if (fh == NULL)
		return false;

	char *buffer = NULL;
	size_t buffer_size = 0;
	ssize_t read;
	bool success = false;
	while ((read = getline(&buffer, &buffer_size, fh)) != -1) {
		if (sscanf(buffer, "%s %31s", device, driver) == 2) {
			success = true;
			break;
		}
	}

	free(buffer);
	fclose(fh);

	return success;
}
