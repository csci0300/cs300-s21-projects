#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include "./strings_tests.h"

static int total_tests = 0;
static int num_correct = 0;
static int num_incorrect = 0;

#define USAGE                                                               \
  "./run_tests <all | names of tests to run>"                               \
  "\n   Ex. \"./run_tests all\" runs all tests"                             \
  "\n   Ex. \"./run_tests strlen strspn\" runs the strlen and strspn tests" \
  "\n   Ex. \"./run_tests strstr\" runs the strstr tests"

int cmp_str(const char* s1, const char* s2) {
  register size_t i;

  // Determine the length of the shortest string.

  // For each character in the shortest string, see if the strings diverge.
  for (i = 0; s1[i] != '\0' && s2[i] != '\0'; i++)
    if (s1[i] != s2[i]) return s1[i] - s2[i];

  // i should be equal to min(strlen(s1), strlen(s2)) if code gets here
  // one string must have a null byte. if both have null byte, return 0
  // otherwise, return s1-s2 at end
  return s1[i] - s2[i];
}

void assert_equal_int(int expected, int actual, const char* message) {
  ++total_tests;
  if (expected == actual) {
    ++num_correct;
  } else {
    ++num_incorrect;
    printf("[FAILED Test %d: %s]: Expected %d, actual %d\n", total_tests,
           message, expected, actual);
  }
}

void assert_same_sign(int expected, int actual, const char* message) {
  ++total_tests;
  // if both are 0, both positive, or both negative
  if ((actual == 0 && expected == 0) || ((expected > 0) && (actual > 0)) ||
      ((expected < 0) && (actual < 0))) {
    ++num_correct;
  } else {
    ++num_incorrect;
    printf(
        "[FAILED Test %d: %s]: Expected %d, actual %d (should be the "
        "same sign)\n",
        total_tests, message, expected, actual);
  }
}

void assert_equal_char(char expected, char actual, int format,
                       const char* message) {
  ++total_tests;
  if (expected == actual) {
    ++num_correct;
  } else {
    ++num_incorrect;
    if (format) {
      printf("[FAILED Test %d: %s]: Expected %c, actual %c\n", total_tests,
             message, expected, actual);
    } else {
      printf("[FAILED Test %d: %s]: Expected %hhx, actual %hhx\n", total_tests,
             message, expected, actual);
    }
  }
}

void assert_equal_str(char* expected, char* actual, const char* message) {
  ++total_tests;

  if (expected == NULL || actual == NULL) {
    if (expected == NULL && actual == NULL) {
      ++num_correct;
    } else {
      ++num_incorrect;
      printf("[FAILED Test %d: %s]: Expected %s, actual %s\n", total_tests,
             message, expected, actual);
    }
    return;
  }

  if (!cmp_str(expected, actual)) {
    ++num_correct;
  } else {
    ++num_incorrect;
    printf("[FAILED Test %d: %s]: Expected %s, actual %s\n", total_tests,
           message, expected, actual);
  }
}

void assert_equal_wchar(char* expected, char* actual, const char* message) {
  ++total_tests;
  int match = 1;
  for (int i = 0; i < 4; i++) {
    if (expected[i] != actual[i]) {
      match = 0;
      printf("[FAILED Test %d: %s]: Expected %hhx, actual %hhx\n", total_tests,
             message, expected[i], actual[i]);
      break;
    }
  }
  if (match) {
    ++num_correct;
  } else {
    ++num_incorrect;
  }
}

void print_test_summary() {
  printf("Passed %d tests, Failed %d tests | %f%%\n", num_correct,
         num_incorrect,
         100 * ceil((float)num_correct) / (total_tests == 0 ? 1 : total_tests));
  total_tests = 0;
  num_incorrect = 0;
  num_correct = 0;
}

void run_test(void (*func)(), const char* message) {
  printf("=================== Running %s Tests. ===================\n",
         message);
  func();
  printf("%s: ", message);
  print_test_summary();
}

void test_all() {
  run_test(&test_strlen, "strlen");
  run_test(&test_strncmp, "strncmp");
  run_test(&test_strncpy, "strncpy");
  run_test(&test_strchr, "strchr");
  run_test(&test_strtok, "strtok");
  run_test(&test_mbslen, "mbslen");
}

void foreach_test(int num_tests, char const* test_names[]) {
  int dummy;
  for (int i = 0; i < num_tests; ++i) {
    if (i == 0) continue;  // file name

    const char* test_name = test_names[i];
    if (!cmp_str(test_name, "all")) {
      test_all();
      break;
    } else if (!cmp_str(test_name, "strlen"))
      run_test(&test_strlen, "strlen");
    else if (!cmp_str(test_name, "strncpy"))
      run_test(&test_strncpy, "strncpy");
    else if (!cmp_str(test_name, "strncmp"))
      run_test(&test_strncmp, "strncmp");
    else if (!cmp_str(test_name, "strchr"))
      run_test(&test_strchr, "strchr");
    else if (!cmp_str(test_name, "strtok"))
      run_test(&test_strtok, "strtok");
    else if (!cmp_str(test_name, "mbslen"))
      run_test(&test_mbslen, "mbslen");
    else if (sscanf(test_name, "%d", &dummy) != 1)
      printf("Unknown test: %s\n", test_name);
  }
}

int main(int argc, char const* argv[]) {
  /* code */
  printf("Testing for correctness...\n");

  if (argc < 2) {
    printf("USAGE: %s\n", USAGE);
    return 1;
  }

  foreach_test(argc, argv);

  return 0;
}
