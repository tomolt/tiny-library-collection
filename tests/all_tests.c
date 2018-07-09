#include <stdlib.h>

#define SD_IMPLEMENT_HERE
#include "sd_cuts.h"

void calm_suite(void);
void hashtable_suite(void);

int main()
{
	sd_init();
	sd_branchv(calm_suite);
	sd_branchv(hashtable_suite);
	sd_summarize();
	return EXIT_SUCCESS;
}

