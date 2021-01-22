# Extra credit assignment
FILE_FILES = file.h file.c
EC_BIN = file

all: encoding_tests $(EC_BIN)

$(EC_BIN): $(FILE_FILES)
	$(CC) $(CFLAGS) -o $@ $^

encoding_tests:
	wget https://cs.brown.edu/courses/csci0300/2021/assign/projects/assets/strvec-encoding-tests.tar.gz
	tar -xzf strvec-encoding-tests.tar.gz
	rm strvec-encoding-tests.tar.gz

check: encoding_tests $(EC_BIN)
	@./run_extra_tests simple


check-stretch: encoding_tests $(EC_BIN)
	@./run_extra_tests stretch

format:
	clang-format -style=Google -i $(FILE_FILES)

clean:
	rm -f $(EC_BIN)

.PHONY: all clean
