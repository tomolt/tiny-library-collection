#include <stdlib.h>

#define SD_IMPLEMENT_HERE
#include "sd_cuts.h"

void calm_suite(void);
void hashtable_suite(void);
void sd_cuts_suite(void);

int main()
{
	sd_init(stdout);
	sd_branch (
		calm_suite();
	)
	sd_branch (
		hashtable_suite();
	)
	sd_branch (
		sd_cuts_suite();
	)
	sd_summarize();
	return EXIT_SUCCESS;
}

