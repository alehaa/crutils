/*
 * include header files
 */
#include "device.h"

#include <vector>

#include <syslog.h>
#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>


/* open_device(const char *p_device)
 *
 * versucht die uebergebene Geraetedatei zu oeffnen und die exklusivrechte zu
 * erhalten. Bei Erfolg wird true zurueck gegeben, in allen anderen Faellen
 * false.
 */
bool crutilsd_device::open_device(const char *p_device) {
	/* Try to open this Device */
	this->file = open(p_device, O_RDONLY);
	if (this->file >= 0) {
		syslog(LOG_DEBUG, "opened device-file");

		/* Try to get exclusive rights for this device. Otherwise e.g. X11 might output
		 * the same data as HID-input as we recive here. */
		if (ioctl(this->file, EVIOCGRAB, 1) >= 0) {
			syslog(LOG_DEBUG, "got exclusive rights for device-file");
			return true;
		} else syslog(LOG_ERR, "could not get exclusive rights for device");
	} else syslog(LOG_ERR, "could not open device-file 'DEVNAME'");

	return false;
}


/* listen ()
 *
 * wartet auf eingehende codes, speichert diese im buffer zwischen und sendet
 * jeweils einen code an die zur verfuegung stehende Funktion
 */
void crutilsd_device::listen () {
	/* start with read out the data. The chars are cached in the buffer and will be
	 * sent as complete code after read operations via dbus to the clients. In order
	 * to check after each input, if the last input event was the end of a code, the
	 * buffer-size is one.
	 *
	 * The input is - as long as the code has not finished - temporarily stored in
	 * another buffer. This has a size of 20 because although most of the bar codes
	 * (GTIN) have a max. length of 18.
	 * If this is insufficient, however, the buffer of the procedure can be increa-
	 * sed. Once the code is finished and was send, the buffer is will be deleted
	 * and a new one is created in its original size.
	 */
	syslog(LOG_INFO, "start listening for read codes");

	struct input_event ev[1];
	int rd, size = sizeof(struct input_event);
	std::vector<char> buffer;
	while (true) {
		/* exit the while loop when an error occurs. This can also happen when the
		 * device is removed. */
		if ((rd = read(this->file, ev, size)) < size) break;

		/* We only handle events of type EV_KEY. In addition, we treat only the press of
		 * keys not release that to prevent the duplicate detecting the input. */
		if (!(ev[0].type == EV_KEY && ev[0].value == 1)) continue;

		/* translate key event to char */
		char c = this->keytoc(&ev[0]);

		/* keytoc () returns for all characters that can happen the valid characters.
		 * If it is around the end of a code - represented by a globally KEY_ENTER - 0 is
		 * returned. If an error occurs -1 is returned. */
		if (c < 0) {
			syslog(LOG_NOTICE, "invalid char recognized. keycode: %i\n", ev[0].code);
			continue;
		}

		/* add char to buffer */
		buffer.push_back(c);

		/* if c is 0, then this is the end of a code. Send the code to the custom
		 * function */
		if (c == 0) {
			syslog(LOG_DEBUG, "recived code '%s'", &buffer[0]);

			buffer.clear();
		}
	}

	/* clean buffer */
	buffer.clear();

	syslog(LOG_INFO, "stoped listening");
}

