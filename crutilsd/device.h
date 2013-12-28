/* device
 *
 * behandelt das Device und liesst es aus. Verarbeitet zudem die Eingabe und
 * schickt dann den im buffer gespeicherten Code an eine zuvor definierte
 * Funktion weiter.
 */

#ifndef CRUTILSD_DEVICE_H
#define CRUTILSD_DEVICE_H

class crutilsd_device {
	public:
		bool open_device(const char *p_device);
		void listen ();

	protected:
		const char keytoc(struct input_event *p_ev);

	private:
		int file;
};

#endif

