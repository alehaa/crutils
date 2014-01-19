/*
 * include header files
 */
#include "crutils.h"


/* constructor ()
 *
 * init class and internal varaibles
 */
crutils::crutils () {
	this->dbus_connection = NULL;
	this->code_handler = NULL;

	this->muted = false;
}


/* destructor ()
 *
 * close all open connections
 */
crutils::~crutils () {
	this->stop();
}


/* set_handler ()
 *
 * sets handler function, which uses the found codes
 */
void crutils::set_handler (void (*p_handler) (const char * p_code)) {
	this->code_handler = p_handler;
}


/* mute ()
 *
 * listen to incomming codes, but don't send them to code_handler
 */
void crutils::mute () {
	this->muted = true;
}


/* unmute ()
 *
 * re-enables listening after being muted
 */
void crutils::unmute () {
	this->muted = false;
}
