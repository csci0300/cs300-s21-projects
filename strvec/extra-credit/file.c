#include "file.h"
#include <stdlib.h>

/* detect_encoding() - reads through the file indicated by fd
 * to determine its encoding
 *
 * Arguments - fd, the file descriptor for the desired file
 *
 * Return Value - returns the encoding of the file
 */
enum encoding_type detect_encoding(int fd) {
  // TODO: your implementation here!
  return UNDEFINED;
}

/* The current implementation of main() opens the file and calls
 * `detect_encoding()`, then prints the result.
 *
 * We only ask you to make `detect_encoding()` work correctly,
 * but if you feel motivated for a stretch goal, can you make
 * this implementation match the output of the standard "file"
 * utility in Linux? Here's how file works:
 *
 * $ file strings.c
 * strings.c: C source, ASCII text
 * $ file emojis.txt
 * emojis.txt: UTF-8 Unicode text
 *
 * We provide some challenging examples. Run `make
 * check-stretch` to see if your implementation meets the
 * challenges!
 */
int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: file <PATH>\n");
    return 1;
  }

  int fd = open(argv[1], O_RDONLY);
  if (fd == -1) {
    perror("open failed");
    return 1;
  }

  switch (detect_encoding(fd)) {
    case UNDEFINED:
      printf("Undefined\n");
      break;
    case ASCII:
      printf("ASCII\n");
      break;
    case UTF_8:
      printf("UTF-8\n");
      break;
  }

  if (close(fd) == -1) {
    perror("close");
    return 1;
  }

  return 0;
}
