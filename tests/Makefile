CFLAGS+=-g -Wall -Wextra -pedantic -std=gnu99 -I.. # TODO -std=c99 or lower
LDLIBS+=-lm

.PHONY: all run clean

all: all_tests

run: all_tests
	./$<

clean:
	$(RM) all_tests
	$(RM) *.o

all_tests: calm_suite.o hashtable_suite.o sd_cuts_suite.o
