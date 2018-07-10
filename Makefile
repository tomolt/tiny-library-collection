.PHONY: all tests clean

all: tests

tests:
	cd tests; make run

clean:
	cd tests; make clean
