/*
 * include header files
 */
#include "crutils.h"

#include <signal.h>


/* listen ()
 *
 * start listening thread and listen for incomming codes
 */
bool crutils::listen () {
	if (this->connect()) {
		if (pthread_create(&this->thread, NULL, crutils::listener, this) == 0) {
			return true;
		}
	}

	return false;
}


/* stop ()
 *
 * stops listening thread. for short time disconnects please use mute!
 */
bool crutils::stop () {
	return (pthread_kill(this->thread, 9) == 0);
}
