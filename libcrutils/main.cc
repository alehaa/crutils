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
 * include header-files
 */
#include <cstdio>

#include "unistd.h"

#include "crutils.h"

void print_code(const char *p_code) {
	printf("code: %s\n", p_code);
}


int main() {
	crutils cr_handle;
	cr_handle.set_handler(&print_code);
	cr_handle.listen();

	sleep(100);
	cr_handle.stop();
}
