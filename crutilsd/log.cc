/* This file is part of crutilsd.
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
#include "log.h"

#include <cstdarg>

#include <syslog.h>


/* constructor
 *
 * init class and get pointer to config
 */
crutilsd_log::crutilsd_log (crutilsd_config *p_config) {
	/*
	 * init class variables
	 */
	this->syslog_opened = false;

	/* set pointer to config */
	this->config = p_config;
}


/* syslog_open
 *
 * opens a syslog connection with openlog and sets a filter dependend on
 * users configurations
 */
void crutilsd_log::syslog_open () {
	/* We want to log to daemon and a PID is needed for each log entry, because
	 * there may be multiple instances of this daemon. */
	openlog("crutilsd", LOG_PID, LOG_DAEMON);
	this->syslog_opened = true;
}


/* syslog_close
 *
 * closes syslog connection if opened before
 */
void crutilsd_log::syslog_close () {
	if (this->syslog_opened) closelog();
}


/* printf
 *
 * same as printf() but this function decides which messages are routed to
 * stdout / stderr or syslog
 */
void crutilsd_log::printf (const char *format, ...) {
	va_list args;
	va_start(args, format);
	vprintf (format, args);
	va_end(args);
}
