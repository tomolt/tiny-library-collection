#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "dh_cuts.h"

#define HT_IMPLEMENT_HERE
#include "hashtable.h"

char *my_strdup(char const *string)
{
	size_t length = strlen(string) + 1;
	char *copy = malloc(length * sizeof(char));
	memcpy(copy, string, length);
	return copy;
}

void test_insertions(void)
{
#define NUM_ITERATIONS 5000
	dh_push("batch insertions and lookups");
	struct HT ht = htNew(500, sizeof(int));
	int values[NUM_ITERATIONS];
	char const *keys[NUM_ITERATIONS];
	for (int i = 0; i < NUM_ITERATIONS; ++i) {
		dh_push("insertion #%d", i);
		int rd = rand() % 0xFFFF;
		char buf[50];
		sprintf(buf, "%x", rd);
		char const *name = my_strdup(buf);
		htSet(&ht, name, strlen(name), &rd);
		values[i] = rd;
		keys[i] = name;
		dh_pop();
	}
	for (int i = 0; i < NUM_ITERATIONS; ++i) {
		dh_push("lookup #%d", i);
		int has = htHas(&ht, keys[i], strlen(keys[i]));
		dh_assert(has);
		if (has) {
			dh_assertiq(*(int*)htGet(&ht, keys[i], strlen(keys[i])), values[i]);
		}
		dh_pop();
	}
	htFree(&ht);
	dh_pop();
}

void hashtable_suite(void)
{
	dh_push("hashtable");
	test_insertions();
	dh_pop();
}
