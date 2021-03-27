CXX = g++
O = -O0
CXXFLAGS = -std=gnu++17 -W -Wall -Wshadow -g -Wno-unused-result -Wno-unused-parameter -Wno-unused-variable -Werror $(O)
WANT_TSAN = 1

-include sanitizers.mk

EXECS = client server
SRCS = vunmo-server.cc vunmo-client.cc synchronized_queue.cc network_helpers.cc

TEST_DIR = tests
QUEUE_TESTS = $(patsubst %.cc,%,$(sort $(wildcard $(TEST_DIR)/queue/*.cc)))
SERVER_TESTS = $(patsubst %.cc,%,$(sort $(wildcard $(TEST_DIR)/server_part1/*.cc))) \
               $(patsubst %.cc,%,$(sort $(wildcard $(TEST_DIR)/server_part2/*.cc)))
TESTS = $(QUEUE_TESTS) $(SERVER_TESTS)

all: $(EXECS)

tests: $(TESTS) $(SRCS)

%: %.cc $(SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $^ -lpthread

clean:
	rm -f $(EXECS) $(TESTS)

check:
	./test.sh

format:
	clang-format -style=Google -i vunmo-server.cc

.PHONY: all clean check
