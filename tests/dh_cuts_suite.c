#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "dh_cuts.h"

static void test_crash_recovery(void)
{
	dh_push("crash recovery");
	pid_t pid = fork();
	if (pid == 0) {
		freopen("/dev/null", "w", stdout);
		dh_branch( raise(SIGFPE); )
		exit(0);
	}
	if (pid > 0) {
		int ret = 0;
		waitpid(pid, &ret, 0);
		dh_assert(ret == 0);
	}
	dh_pop();
}

void dh_cuts_suite(void)
{
	dh_push("dh_cuts");
	test_crash_recovery();
	dh_pop();
}

