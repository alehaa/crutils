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

#ifndef CODEREADER_PRIVATE_DEVICE_H
#define CODEREADER_PRIVATE_DEVICE_H


#include <sys/queue.h> // SLIST_* macros

#include <libconfig.h> // libconfig API


/** \brief Hook provided by the driver to open a device.
 *
 *
 * \param config The parsed configuration for this device.
 * \param cookie Pointer to a void-pointer, a driver can set to it's allocaed
 *  data storage.
 *
 * \return Like the system's open function, this hook should return the opened
 *  file-descriptor on success or -1 if an error occurs.
 */
typedef int (*codereader_hook_open)(const config_setting_t *config,
                                    void **cookie);

/** \brief Hook provided by the driver to read from a device.
 *
 * \details This hook will be called, if data is read to read at the given file-
 *  descriptor \p fd.
 *
 *
 * \param fd The previously opened file-descriptor.
 * \param buffer Where to store read data.
 * \param size Size of \p buffer.
 * \param cookie Pointer to the driver's data storage.
 *
 * \return On success the number of read bytes should be returned, otherwise -1.
 */
typedef int (*codereader_hook_read)(int fd, char *buffer, int size,
                                    void *cookie);

/** \brief Hook provided by the driver to close a device.
 *
 *
 * \param fd The previously opened file-descriptor.
 * \param cookie Pointer to the driver's data storage.
 *
 * \return On success zero should be returned, otherwise -1.
 */
typedef int (*codereader_hook_close)(int fd, void *cookie);


/* Forward declaration required for the following struct. */
struct codereader_device;

/** \brief Struct storing all information about the used device driver.
 *
 * \details This struct stores all necessary handles and pointers for a device
 *  driver.
 */
struct codereader_driver
{
	void *dh; ///< Handle for the loaded shared object of the driver.

	codereader_hook_open open;   ///< Driver hook to open a device.
	codereader_hook_read read;   ///< Driver hook to read from a device.
	codereader_hook_close close; ///< Driver hook to close a device.
};


/** \brief Struct storing all information about a barcode reader device.
 *
 * \details This struct stores all data that is required for managing a
 *  barcode readers.
 *
 * \note Although this struct is meant for managing devices, some drivers may
 *  handle more than one device in the same instance.
 */
struct codereader_device
{
	int fd;                          ///< File-descriptor of the device.
	struct codereader_driver driver; ///< The driver used by this device.
	void *cookie;                    ///< Optional pointer to data storage.

	SLIST_ENTRY(codereader_device) lmp; ///< List management struct.
};


/** \brief Struct storing the head of all \ref codereader_device entries.
 */
SLIST_HEAD(codereader_device_list, codereader_device);


#endif
