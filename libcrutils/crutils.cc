/* This file is part of libcrutils1.
*
* crutilsd is free software: you can redistribute it and/or modify it under the
* terms of the GNU General Public License as published by the Free Software
* Foundation, either version 3 of the License, or (at your option) any later
* version.
*
* This program is distributed in the hope that it will be useful,but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
* FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
* details.
*
* You should have received a copy of the GNU General Public License along with
* this program. If not, see <http://www.gnu.org/licenses/>.
*
*
* copyright 2013-2014 Alexander Haase
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
