#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

/* enums assign names to values to create alternative integer data types
 * additional values can be added to this enum if you choose to detect
 * additional encodings
 */
enum encoding_type { UNDEFINED = -1, ASCII = 0, UTF_8 = 1 };

enum encoding_type detect_encoding(int fd);