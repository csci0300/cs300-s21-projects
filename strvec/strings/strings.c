/*
 * Adapted with permission from CSCI 0330's "strings" assignment.
 */
#include <stdlib.h>

/*
 * strlen() - abbreviated from "string length".
 * - Description: determine the number of characters in a character string.
 *
 * - Arguments: a pointer to a null-terminated character string.
 *
 * - Return value: the number of characters using a special unsigned numerical
 *   type, which is intended specifically for pointer arithmetic, called size_t.
 *
 * Examples:
 * 1. >  strlen("") = 0
 * 2. >  strlen("ilovecs300") = 10
 *
 * - Hint: the length of a C string does _not_ include the null ('\0')
 *   character that terminates it.
 */
size_t strlen(const char* s) {
  // TODO: Fill In
  return 0;
}

/*
 * strncmp() - abbreviated from "string compare (n bytes)".
 * - Description: compares up to the first n bytes of two strings, s1 and s2
 *
 * - Arguments: two null terminated strings (s1 and s2) and the maximum number
 * of bytes to compare (n).
 *
 * - Return value: negative number if s1 < s2, 0 if s1 == s2, positive number if
 * s1 > s2
 *
 * Examples:
 * 1. >  strncmp("abcde", "abdde", 3) = some negative number
 * 2. >  strncmp("abcde", "abdde", 2) = 0
 * 3. >  strncmp("1234567", "123", 100) = some positive number
 *
 */
int strncmp(const char* s1, const char* s2, size_t n) {
  // TODO: Fill In
  return 0;
}

/*
 * strncpy() - abbreviated from "string copy (n bytes)".
 * - Description: copies the first n bytes of the contents of the source string
 *   into the location of the destination string. As with strcpy(), we are
 *   assuming the destimation string is large enough to hold the entire
 *   contents of the source string. If the number of bytes to be copied is less
 *   than the number of bytes in the src string, the null character will
 *   not be copied. If the number of bytes to be copied is longer than the
 *   length of src, the difference will be padded with null characters appended
 *   onto the end of dest to fill out the requested in bytes.
 *
 * - Arguments: a pointer (char *) to the destination string, a pointer
 *   (char *) to the source string, and the number of bytes (size_t) to copy.
 *
 * - Return value: a pointer (char *) to the destination string.
 *
 * - Usage:
 *
 *      char abbreviation[5];
 *      strncpy(abbreviation, "professor", 4);
 *
 *      (The variable abbreviation now contains the four bytes of "prof", but
 *      it is not null-terimated.)
 *
 *      char buffer[10];
 *
 *      strncpy(buffer, "brown", 10);
 *
 *      (The variable buffer now contains the five bytes of "brown" followed by
 *      five null characters.)
 *
 *      char good_fit[5];
 *
 *      strncpy(good_fit, "okay", 5);
 *
 *      (The variable good_fit now contains the four bytes of "okay" and is
 *      correctly null-terminated.)
 *
 * - Hint: make sure not to modify the source string. Please pay attention to
 * the function description to see what is expected of your implementation.
 */
char* strncpy(char* dest, const char* src, size_t n) {
  // TODO: Fill In
  return NULL;
}

/*
 * strchr() - abbreviated from "string (find) character".
 * - Description: Returns a pointer to the first occurance of the character,
 * chr, in the string, str. Returns NULL if chr is not contained in str
 *
 * - Arguments: a pointer to a null-terminated character string to find
 * (str), and a single character to find in the string (chr)
 *
 * - Return value: a pointer to the first occurance of chr in str
 *
 * Examples:
 * 1. >  strchr("abcde", 'b') = "bcde"
 * 2. >  strchr("hello", 'z') = NULL
 * 3. > strchr("1234", '/0') = "/0"
 *
 * - Hint: The terminating null charater is considered part of the string for
 * strchr
 */
char* strchr(const char* str, int chr) {
  // TODO: Fill In
  return NULL;
}

/*
 * strtok() - abbreviated form of string tokenize
 * - Description: The strtok() function parses a string into a sequence of
 * tokens. On the first call to strtok() the string to be parsed should be
 * specified in str. In each subsequent call that should parse the
 * same string, str should be NULL.
 * Note: strtok does not require that the characters pointed to by str
 * remain unmodified
 *
 * - Arguments: a pointer to the string being tokenized or a NULL pointer to
 * continue parsing the previous string (str), and a pointer to the characters
 * that the string should be tokenized around (delim)
 *
 * - Return value: a pointer to the next token or NULL if there are no more
 * tokens
 *
 * Example:
 *  char str[20] = ",,a,b,,,cd,,";
 *  char *t1 = strtok(str, ",");  // t1 should point to the token "a"
 *  char *t2 = strtok(NULL, ","); // t2 should point to the token "b"
 *  char *t3 = strtok(NULL, ","); // t3 should point to the token starting
 *                                   at "c" (the entire token is "cd")
 *  char *t4 = strtok(NULL, ","); // t4 should point to NULL
 */
char* strtok(char* str, const char* delim) {
  static char* saved;  // used to save state between calls to strtok
  if (str != NULL) {
    // reset saved if str is not null
    saved = str;
  }
  // TODO: Complete this function
  return NULL;
}
