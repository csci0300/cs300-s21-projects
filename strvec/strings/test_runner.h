void run_test(void (*func)(), const char* message);
void print_test_summary();

/*
 * assert_equal_int - checks if the expected int is equal to the actual int,
 * and returns an error message if not.
 * ex: useful for strlen and other string functions that return ints
 */
void assert_equal_int(int expected, int actual, const char* message);

/*
 * assert_equal_sign - checks if the expected int is the same sign as the actual
 * int, and returns an error message if not. ex: useful for strncmp
 */
void assert_same_sign(int expected, int actual, const char* message);

/*
 * assert_equal_char - checks if the expected char is equal to the actual char,
 * and returns an error message if not.
 * ex: useful for strncpy
 */
void assert_equal_char(char expected, char actual, int format,
                       const char* message);

/*
 * assert_equal_str - checks if the expected string is equal to the actual
 * string, and returns an error message if not.
 */
void assert_equal_str(char* expected, char* actual, const char* message);

/*
 * assert_equal_wchar - checks if 2 wchars are equal to eachother
 */
void assert_equal_wchar(char* expected, char* actual, const char* message);