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

/*
 * include header files
 */
#include "crutils.h"

#include <signal.h>


/* listen ()
 *
 * start listening thread and listen for incomming codes
 */
bool crutils::listen () {
	if (this->connect()) {
		if (pthread_create(&this->thread, NULL, crutils::listener, this) == 0) {
			return true;
		}
	}

	return false;
}


/* stop ()
 *
 * stops listening thread. for short time disconnects please use mute!
 */
bool crutils::stop () {
	return (pthread_kill(this->thread, 9) == 0);
}
