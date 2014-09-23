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
 * include header-files
 */
#include "lxinput.h"


const char keytoc(struct input_event *p_ev) {
	/* is this an enter signal? */
	if (p_ev->code == KEY_ENTER) return '\0';

	/* numeric values */
	switch (p_ev->code) {
		case KEY_0:	return '0';
		case KEY_1:	return '1';
		case KEY_2:	return '2';
		case KEY_3:	return '3';
		case KEY_4:	return '4';
		case KEY_5:	return '5';
		case KEY_6:	return '6';
		case KEY_7:	return '7';
		case KEY_8:	return '8';
		case KEY_9:	return '9';
	}

	return -1;
}
