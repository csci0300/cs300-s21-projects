#include <unistd.h>
#include <cassert>
#include <optional>
#include <string>

#include "../../test_utils/test_utils.h"

using namespace std;

int main() {
  char hostnamebuf[256];
  gethostname(hostnamebuf, 256);
  string hostname(hostnamebuf);

  const string skv_addr = hostname + ":8080";
  start_simple_shardkv(skv_addr);

  assert(test_get(skv_addr, "user_1_posts", nullopt));
  assert(test_append(skv_addr, "user_1_posts", "post_0", true));
  assert(test_get(skv_addr, "user_1_posts", "post_0"));
  assert(test_append(skv_addr, "user_1_posts", ", post_1", true));
  assert(test_get(skv_addr, "user_1_posts", "post_0, post_1"));

  return 0;
}
