#include <stdlib.h>

#define DH_IMPLEMENT_HERE
#include "dh_cuts.h"

void calm_suite(void);
void hashtable_suite(void);
void dh_cuts_suite(void);

int main()
{
	dh_init(stdout);
	dh_branch (
		calm_suite();
	)
	dh_branch (
		hashtable_suite();
	)
	dh_branch (
		dh_cuts_suite();
	)
	dh_summarize();
	return EXIT_SUCCESS;
}

