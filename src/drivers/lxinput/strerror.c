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

#include "lxinput.h"


/** \brief Return error string for \p errno.
 *
 * \details This function returns an error string for the error code defined in
 *  \p errno, which can be used to print error messages.
 *
 *
 * \param errno Error code returned by a function called before.
 *
 * \return A pointer to an array of char (C-String) will be returned.
 *
 *
 * \warning The returned pointer <b> MUST NOT </b> be freed!
 */
const char *
codereader_strerror(const int errno)
{
	/* Switch for all known error codes. As a programmer safety concept, there
	 * is no default label, so the compiler will print a warning, if there is
	 * not a case label for all enum values. */
	switch ((errorcodes)errno) {
		case ERR_OPEN: return "Failed to open device file.";
		case ERR_CLOSE: return "Failed to close device file.";
		case ERR_READ: return "Failed to read data.";
		case ERR_GRAB: return "Failed to grab device.";
		case ERR_UNGRAB: return "Failed to ungrab device.";
	}


	/* If errno is no known error code of this driver, return unknown error. */
	return "Unknown error.";
}
