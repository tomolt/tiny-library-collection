CFLAGS+=-g -Wall -Wextra -pedantic -I..
LDFLAGS+=-lm

.PHONY: all run clean

all: all_tests

run: all_tests
	./$<

clean:
	$(RM) all_tests
	$(RM) *.o

all_tests: calm_suite.o hashtable_suite.o
