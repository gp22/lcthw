CFLAGS=-g -O2 -Wall -Wextra -Isrc -rdynamic -DNDEBUG $(OPTFLAGS)

TEST_SRC=$(wildcard test/*_tests.c)
TESTS=$(patsubst %.c,%,$(TEST_SRC))

# The target build.
all: tests

# The unit tests.
tests: $(TESTS)
	sh ./test/runtests.sh

clean:
	rm -f *.o
	rm -f devpkg
	rm -rf *.dSYM
