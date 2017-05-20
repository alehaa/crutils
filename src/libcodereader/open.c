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

/* The following define is required for platforms with fopencookie, so it will
 * be available in the stdio header. */
#define _GNU_SOURCE


#include <assert.h>  // assert
#include <dlfcn.h>   // dl* functions
#include <stdbool.h> // bool, false, true
#include <stdio.h>   // IO functions, types and macros
#include <stdlib.h>  // getenv, malloc
#include <string.h>  // memset

#include <libconfig.h> // libconfig API

#include "config.h"   // CODEREADER_CONFIG_FILE, CODEREADER_DRIVER_DIR
#include "device.h"   // codereader_source* and codereader_hook*
#include "internal.h" // CODEREADER_MESSAGE_PREFIX, codereader_* functions


/** \brief Get the config file to use.
 *
 * \details By default the configuration in \ref CODEREADER_CONFIG_FILE will be
 *  used. However, if the user defines the environment variable
 *  `CODEREADER_CONFIG`, this file will be used instead.
 *
 *
 * \return Pointer to the char-array to be used as filename.
 */
static inline const char *
codereader_config_file()
{
	const char *p = getenv("CODEREADER_CONFIG");
	return (p != NULL) ? p : CODEREADER_CONFIG_FILE;
}


/** \brief Resolve symbol \p name in \p handle.
 *
 * \details This function is a wrapper for `dlsym` to print an error message, if
 *  dlsym fails for any reason.
 *
 *
 * \param handle Handle of the shared library.
 * \param name Name of the symbol.
 *
 * \return The return value of `dlsym` will be pass through.
 */
static inline void *
codereader_dlsym(void *handle, const char *name)
{
	dlerror();
	void *sym = dlsym(handle, name);
	const char *err = dlerror();
	if (err != NULL)
		fprintf(stderr,
		        CODEREADER_MESSAGE_PREFIX "Can't resolv symbol %s: %s\n", name,
		        err);
	return sym;
}


/** \brief Load driver \p name into \p driver.
 *
 * \details This function loads the driver \p name and maps all required symbols
 *  to the \ref codereader_driver struct \p driver.
 *
 *
 * \param name Driver name.
 * \param driver Pointer to \ref codereader_driver to store the information in.
 *
 * \return true The driver was loaded successfully.
 * \return false The driver could not be loaded. The driver is not available or
 *  symbols in the driver file could not be resolved.
 */
static inline bool
codereader_driver_load(const char *name, struct codereader_driver *driver)
{
	assert(name);
	assert(driver);


	/* Load the reqested driver. RTLD_NOW will be used, to resolve all symbols
	 * before using the driver, so that there can't be any resolving issues at
	 * any later time. */
	char buffer[FILENAME_MAX];
	snprintf(buffer, FILENAME_MAX, "%s/%s.so", CODEREADER_DRIVER_DIR, name);
	driver->dh = dlopen(buffer, RTLD_NOW);
	if (driver->dh == NULL)
		return false;

	/* Symbolize the hook functions. If a function can't be found or there is an
	 * error while loading, codereader_dlsym will print a warning and this
	 * function will return false after processing all symbols. */
	driver->open =
	    (codereader_hook_open)codereader_dlsym(driver->dh, "device_open");
	driver->read =
	    (codereader_hook_read)codereader_dlsym(driver->dh, "device_read");
	driver->close =
	    (codereader_hook_close)codereader_dlsym(driver->dh, "device_close");

	return (driver->open != NULL && driver->read != NULL &&
	        driver->close != NULL);
}


/** \brief Open a new handle to read data from barcode readers.
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
	/* Load the configuration file. If reading the configuration file fails, a
	 * message will be printed on stderr and no further processing happens. */
	config_t cfg;
	config_init(&cfg);
	if (!config_read_file(&cfg, codereader_config_file())) {
		if (config_error_type(&cfg) == CONFIG_ERR_FILE_IO)
			fprintf(stderr,
			        CODEREADER_MESSAGE_PREFIX "Can't read config in %s\n",
			        codereader_config_file());
		else
			fprintf(stderr, CODEREADER_MESSAGE_PREFIX "%s:%d - %s\n",
			        config_error_file(&cfg), config_error_line(&cfg),
			        config_error_text(&cfg));

		config_destroy(&cfg);
		return NULL;
	}

	/* Initialize the list of all loaded codereader sources. The list will be
	 * used below to store all configuration and driver-related data. The head-
	 * pointer will not be global, as it will be stored in the cookie inside the
	 * FILE struct (below). */
	struct codereader_device_list head = SLIST_HEAD_INITIALIZER(head);
	SLIST_INIT(&head);

	/* Iterate over all config entries - each entry is one driver to load (which
	 * handles one or more devices). */
	config_setting_t *root = config_root_setting(&cfg);
	int n = config_setting_length(root);
	for (size_t i = 0; i < n; i++) {
		config_setting_t *iter = config_setting_get_elem(root, i);

		/* Allocate memory for the device struct. Default values will be set, so
		 * the close function can detect what is initialized yet on errors. */
		struct codereader_device *device =
		    malloc(sizeof(struct codereader_device));
		if (device == NULL) {
			fprintf(stderr, CODEREADER_MESSAGE_PREFIX
			        "Not enough memory in %s:%d for device %s.\n",
			        __FILE__, __LINE__, config_setting_name(iter));
			goto free_device_list;
		}
		memset(device, 0, sizeof(struct codereader_device));

		/* Append device to the list of all loaded devices. This will be done
		 * first after allocating the memory, so the 'free_device_list' label
		 * will free the memory for this device, too. */
		SLIST_INSERT_HEAD(&head, device, lmp);

		/* Get the driver used by this device and load it. If no driver is
		 * specified, or the driver can't be loaded, an error message will be
		 * send to stderr and reading the config will be stopped. */
		const char *driver_name;
		if (config_setting_lookup_string(iter, "driver", &driver_name) !=
		    CONFIG_TRUE) {
			fprintf(stderr, CODEREADER_MESSAGE_PREFIX
			        "No driver specified for device '%s'.\n",
			        config_setting_name(iter));
			goto free_device_list;
		}

		if (!codereader_driver_load(driver_name, &(device->driver))) {
			fprintf(stderr, CODEREADER_MESSAGE_PREFIX
			        "Failed to load driver %s for device %s.\n",
			        driver_name, config_setting_name(iter));
			goto free_device_list;
		}


		/* Call the driver's open function for this device. If the driver does
		 * not return a valid file-decriptor, an error message will be send to
		 * stderr and reading the config / loading further devices will be
		 * stopped. */
		device->fd = device->driver.open(iter, &(device->cookie));
		if (device->fd < 0) {
			fprintf(stderr,
			        CODEREADER_MESSAGE_PREFIX "Failed to open device %s.\n",
			        config_setting_name(iter));
			goto free_device_list;
		}
	}

	/* Free the space allocated for the configuration. */
	config_destroy(&cfg);


#ifdef HAVE_FOPENCOOKIE
	/* Setup all hook functions. Thus the codereader stream is readonly and
	 * unable to seek, write and seek functions don't have to be defined. The
	 * read and close hooks will be mapped to internal codereader functions. */
	cookie_io_functions_t hooks = {0};
	hooks.read = codereader_read;
	hooks.close = codereader_close;

	/* Setup the new codereader stream. A pointer to the device list will be
	 * passed, so the read and close functions can access the list of devices.
	 * On success fopencookie() returns a pointer to the new stream. On error,
	 * NULL is returned, so we don't have to evaluate the result and simply
	 * return it. */
	return fopencookie(SLIST_FIRST(&head), "r", hooks);

#else
	/* Setup the new codereader stream. A pointer to the device list will be
	 * passed, so the read and close functions can access the list of devices.
	 * Thus the codereader stream is readonly and unable to seek, write and seek
	 * functions don't have to be defined. The read and close hooks will be
	 * mapped to internal codereader functions. On success funopen returns a
	 * pointer to the new stream. On error, NULL is returned, so we don't have
	 * to evaluate the result and simply return it. */
	return funopen(SLIST_FIRST(&head), codereader_read, NULL, NULL,
	               codereader_close);
#endif


free_device_list:
	/* The following code will be called in error-situations. All opened devices
	 * will be closed immediately and the memory for the list and configuration
	 * freed. */
	codereader_close(SLIST_FIRST(&head));
	config_destroy(&cfg);
	return NULL;
}
