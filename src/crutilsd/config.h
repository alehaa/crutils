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
