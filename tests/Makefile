CFLAGS+=-Wall -Wextra -pedantic -std=gnu99 -I.. # TODO -std=c99 or lower
LDFLAGS+=-g
LDLIBS+=-lm

.PHONY: all run clean

all: all_tests

run: all_tests
	./$<

clean:
	$(RM) all_tests
	$(RM) *.o

# This part would be much nicer if we could use implicit rules for it,
# but because god-awful 'travis.ci' is mostly running software releases
# from the stone age, we get into this stupid situation where both the GNU
# make installation is so old that it puts LDLIBS in the front, and the
# GCC installation is so cranky that it requires library dependencies to
# be listed at the back.
all_tests: all_tests.o calm_suite.o hashtable_suite.o
	$(CC) $(LDFLAGS) $^ -o$@ $(LDLIBS)
