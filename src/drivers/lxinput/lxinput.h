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

#include <linux/input.h>
#include <limits.h>

#include <libconfig.h> // libconfig API


typedef enum errorcodes {
	ERR_OPEN = INT_MIN,
	ERR_CLOSE,
	ERR_READ,
	ERR_GRAB,
	ERR_UNGRAB
} errorcodes;


const char *codereader_strerror(const int errno);

int codereader_open(const config_setting_t *config, void **cookie);
int codereader_read(int fd, char *buffer, int size, void *cookie);
int codereader_close(int fd, void *cookie);

const char keytoc(struct input_event *p_ev);
