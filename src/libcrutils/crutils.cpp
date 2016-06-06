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


/* constructor ()
 *
 * init class and internal varaibles
 */
crutils::crutils () {
	this->dbus_connection = NULL;
	this->code_handler = NULL;

	this->muted = false;
}


/* destructor ()
 *
 * close all open connections
 */
crutils::~crutils () {
	this->stop();
}


/* set_handler ()
 *
 * sets handler function, which uses the found codes
 */
void crutils::set_handler (void (*p_handler) (const char * p_code)) {
	this->code_handler = p_handler;
}


/* mute ()
 *
 * listen to incomming codes, but don't send them to code_handler
 */
void crutils::mute () {
	this->muted = true;
}


/* unmute ()
 *
 * re-enables listening after being muted
 */
void crutils::unmute () {
	this->muted = false;
}
