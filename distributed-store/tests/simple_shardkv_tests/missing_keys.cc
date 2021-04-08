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

  const string skv_1 = hostname + ":8080";
  const string skv_2 = hostname + ":8081";
  const string skv_3 = hostname + ":8082";

  start_simple_shardkvs({skv_1, skv_2, skv_3});

  assert(test_get(skv_1, "user_0", nullopt));
  assert(test_get(skv_2, "post_1", nullopt));
  assert(test_get(skv_3, "user_2", nullopt));

  assert(test_put(skv_1, "user_1", "Bob", "user_1", true));
  assert(test_put(skv_2, "post_5", "hello!", "user_1", true));

  assert(test_get(skv_1, "user_1", "Bob"));
  assert(test_get(skv_2, "post_5", "hello!"));

  assert(test_get(skv_3, "user_1", nullopt));
  assert(test_get(skv_3, "post_5", nullopt));

  assert(test_delete(skv_2, "post_5", true));
  assert(test_get(skv_2, "post_5", nullopt));

  assert(test_delete(skv_1, "user_1", true));
  assert(test_get(skv_1, "user_1", nullopt));

  assert(test_append(skv_1, "user_5_posts", "post_1", true));
  assert(test_get(skv_1, "user_5_posts", "post_1"));

  return 0;
}
