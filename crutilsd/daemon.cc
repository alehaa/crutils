/*
 * include header-files
 */
#include "daemon.h"

#include <syslog.h>
#include <pwd.h>
#include <unistd.h>
#include <csignal>

#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstdlib>

/* drop_privilegs ()
 *
 * if current user is root, root-privilegs will be dropped.
 */
bool drop_privilegs () {
	if(getuid() == 0) {
		struct passwd *usr = getpwnam("crutils");
		if (usr != NULL) {
			if (setuid(usr->pw_uid) == 0) {
				if (setgid(usr->pw_gid) == 0) {
					syslog(LOG_DEBUG, "root-privileges discarded");
					return true;
				}
			}
		} else syslog(LOG_ERR, "user 'crutils' does not exists");

		syslog(LOG_ERR, "Could not discard root-privilegs");
		return false;
	}

	return true;
}


/* daemonize ()
 *
 * make this process to a daemon
 */
bool daemonize () {
	/* fork parent */
	pid_t pid;
	pid = fork();
	if (pid < 0) {
		/* an error occured */
		syslog(LOG_ERR, "error while forking process");
		exit(EXIT_FAILURE);
	} else {
		/* kill the parent */
		if (pid > 0) exit(EXIT_SUCCESS);

		/* create a new session */
		if (setsid() < 0) {
			syslog(LOG_ERR, "error while generate new session in forked process");
			exit (EXIT_FAILURE);
		} else {
			/* ignore SIGHUP */
			signal(SIGHUP, SIG_IGN);

			/* fork the forked process to prevent zombie-processes */
			pid = fork();
			if (pid < 0) {
				/* an error occured */
				syslog(LOG_ERR, "error while second forking process");
				exit(EXIT_FAILURE);
			} else {
				/* kill the parent */
				if (pid > 0) exit(EXIT_SUCCESS);

				/* change working dir */
				chdir ("/");

				/* don't use umask of parrent processes */
				umask (0);

				/* close all file descriptors (STDOUT, STDERR, etc. */
				for (int i = sysconf (_SC_OPEN_MAX); i > 0; i--) close (i);

				syslog(LOG_INFO, "daemonized process");
				return true;
			}
		}
	}
}

