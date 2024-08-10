CFLAGS=-g -O2 -Wall -Wextra -Isrc -rdynamic -DNDEBUG $(OPTFLAGS)
LIBS=-ldl $(OPTLIBS)

PROGRAMS_SRC=$(wildcard bin/*.c)
PROGRAMS=$(patsubst %.c,%,$(PROGRAMS_SRC))

SOURCES=$(wildcard src/**/*.c src/*.c)
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))

TEST_SRC=$(wildcard test/*_tests.c)
TESTS=$(patsubst %.c,%,$(TEST_SRC))

TARGET=build/ringbuffer.a
SO_TARGET=$(patsubst %.a,%.so,$(TARGET))

# The target build.
all: $(TARGET) $(SO_TARGET) tests $(PROGRAMS)

$(TARGET): CFLAGS += -fPIC
$(TARGET): build $(OBJECTS)
	ar rcs $@ $(OBJECTS)
	ranlib $@
$(SO_TARGET): $(TARGET) $(OBJECTS)
	$(CC) -shared -o $@ $(OBJECTS)
$(PROGRAMS): LDLIBS += $(TARGET)

build:
	@mkdir -p build
	@mkdir -p bin

# The unit tests.
.PHONY: tests
tests: LDLIBS += $(TARGET)
tests: $(TESTS)
	sh ./test/runtests.sh

clean:
	rm -rf build $(OBJECTS) $(TESTS) $(PROGRAMS)
	rm -rf test/tests.log
	find . -name "*.gc*" -exec rm {} \;
	rm -rf `find . -name "*.dSYM" -print`
