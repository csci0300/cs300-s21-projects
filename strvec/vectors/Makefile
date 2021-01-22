FLAGS = -g3 -Wall -Wextra -Wconversion -Werror -Wno-error=unused-parameter -Wcast-align -g -fsanitize=address
CFLAGS = -std=gnu11 $(FLAGS)
CXXFLAGS = -std=gnu++17 $(FLAGS)
FILES = vector300.h vector300.c

.PHONY: all clean check format

#creates all executables
all: testsuite

#formats files that may be edited
format:
	clang-format -style=Google -i $(FILES)

#command to run the tests
check: testsuite
	./testsuite

#creates an executable for the testsuite, linking the vector object file
testsuite: tests.cc vector300.o
	$(CXX) $(CXXFLAGS) $^ -o $@

#creates an object files from a the vector300.c and h files
vector300.o: vector300.c vector300.h
	$(CC) $(CFLAGS) -c $^

#removes executables and object files
clean:
	rm -f testsuite vector300.o vector300.h.gch
