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

#ifndef CRUTILSD_CONFIG_H
#define CRUTILSD_CONFIG_H

class crutilsd_config {
	public:
		crutilsd_config (int argc, char **argv);

		unsigned char get_conf_verbose_level ();
		char * get_conf_device ();
		bool get_conf_daemonize ();
		char * get_conf_daemon_user ();

	protected:
		void print_usage ();

	private:
		unsigned char conf_verbose_level;
		char *conf_device;
		bool conf_daemonize;
		char * conf_daemon_user;

		void get_conf_by_argv (int argc, char **argv);
		void get_conf_by_env ();
};

#endif
