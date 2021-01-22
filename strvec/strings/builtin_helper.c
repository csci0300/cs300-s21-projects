#include <stdlib.h>
#include <string.h>

/*
 * Helper function to map `mbslen`, which doesn't have a builtin equibalent,
 * to builtin function `mbstowcs`. Only used for builtin test targets.
 */

ssize_t mbslen(const char* s) {
  if (s != NULL) {
    return mbstowcs(NULL, s, strlen(s));
  }
  return -1;
}
