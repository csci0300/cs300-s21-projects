CC ?= gcc
CFLAGS = -std=gnu11 -Wall -g -fsanitize=address
LDFLAGS = -lm
TESTS = run_tests
TEST_BUILTIN = run_builtin_tests

STRING_OBJS = strings.o
MBSTRING_OBJS = mbstrings.o
STRING_DEP = strings_tests.o test_runner.o
BUILTIN_HELPER_OBJ = builtin_helper.o
BUILTIN_OBJS = $(BUILTIN_HELPER_OBJ) strings_tests_built.o test_runner_built.o
OBJS_ALL = $(STRING_OBJS) $(MBSTRING_OBJS) $(STRING_DEP) $(BUILTIN_OBJS)

S_FILES = strings.h strings.c
MBS_FILES = mbstrings.h mbstrings.c

all: $(TESTS) $(TEST_BUILTIN)

check: $(TESTS)
	./run_tests all

format:
	clang-format -style=Google -i $(S_FILES) $(MBS_FILES)

%.o: %.c
	$(CC) $(CFLAGS) -fno-builtin -c $< -o $@

$(TESTS): $(STRING_OBJS) $(MBSTRING_OBJS) $(STRING_DEP)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(TEST_BUILTIN): $(BUILTIN_HELPER_OBJ) $(STRING_DEP)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(TESTS) $(TEST_BUILTIN) $(OBJS_ALL)

.PHONY: all clean
