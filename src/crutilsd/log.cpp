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
#include "log.h"

#include <cstdarg>
#include <cstdio>

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


/* destructor
 *
 * close syslog connection
 */
crutilsd_log::~crutilsd_log () {
	this->syslog_close();
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
void crutilsd_log::printf (int priority, const char *format, ...) {
	/* parse arguments */
	va_list args;

	/* open syslog connection if not already opened */
	if (!this->syslog_opened) this->syslog_open();

	/* send message to syslog */
	va_start(args, format);
	vsyslog(priority, format, args);

	/* print message to stdout */
	unsigned char vlevel = this->config->get_conf_verbose_level();
	/* is vlevel > 0 ? */
	if (vlevel) {
		if (priority <= (vlevel + 4)) {
			va_start(args, format);
			vprintf (format, args);
			std::printf("\n");
		}
	}

	va_end(args);
}
