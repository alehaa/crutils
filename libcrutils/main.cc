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
