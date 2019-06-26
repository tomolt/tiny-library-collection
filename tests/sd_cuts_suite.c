#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "sd_cuts.h"

static void test_resilient(void)
{
	sd_push("resilient mode");
	pid_t pid = fork();
	if (pid == 0) {
		freopen("/dev/null", "w", stdout);
		sd_execmodel = sd_resilient;
		sd_branch( raise(SIGFPE); )
		exit(0);
	}
	if (pid > 0) {
		int ret = 0;
		waitpid(pid, &ret, 0);
		sd_assert(ret == 0);
	}
	sd_pop();
}

void sd_cuts_suite(void)
{
	sd_push("sd_cuts");
	test_resilient();
	sd_pop();
}

