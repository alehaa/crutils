/*
 * include header-files
 */
#include "device.h"

#include <linux/input.h>

const char crutilsd_device::keytoc(struct input_event *p_ev) {
	/* is this an enter signal? */
	if (p_ev->code == KEY_ENTER) return 0;

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

