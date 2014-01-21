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

#ifndef CRUTILSD_LOG_H
#define CRUTILSD_LOG_H

#include <cstdio>

#include "config.h"

class crutilsd_log {
	public:
		crutilsd_log (crutilsd_config *p_config);

		void printf (const char *format, ...);

	protected:
		crutilsd_config *config;

		void syslog_open ();
		void syslog_close ();

	private:
		bool syslog_opened;
};

#endif
