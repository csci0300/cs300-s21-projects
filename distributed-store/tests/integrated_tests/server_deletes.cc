#include <unistd.h>
#include <cassert>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include "../../test_utils/test_utils.h"

using namespace std;

int main() {
  char hostnamebuf[256];
  gethostname(hostnamebuf, 256);
  string hostname(hostnamebuf);

  string shardmaster_addr = hostname + ":8080";
  start_shardmaster(shardmaster_addr);

  string skv_1 = hostname + ":8081";
  string skv_2 = hostname + ":8082";
  string skv_3 = hostname + ":8083";

  start_shardkvs({skv_1, skv_2, skv_3}, shardmaster_addr);

  map<string, vector<shard_t>> m;

  assert(test_join(shardmaster_addr, skv_1, true));
  assert(test_join(shardmaster_addr, skv_2, true));
  assert(test_join(shardmaster_addr, skv_3, true));

  // sleep to allow shardkvs to query and get initial config
  std::chrono::milliseconds timespan(1000);
  std::this_thread::sleep_for(timespan);

  assert(test_put(skv_1, "user_200", "Bob", "", true));
  assert(test_put(skv_1, "post_202", "wow!", "user_200", true));
  assert(test_put(skv_2, "post_404", "hi", "user_200", true));

  assert(test_get(skv_1, "user_200", "Bob"));
  assert(test_get(skv_1, "post_202", "wow!"));
  assert(test_get(skv_2, "post_404", "hi"));

  // now delete all of user_200's data 
  assert(test_gdpr_delete(shardmaster_addr, "user_200", true));

  // wait for everything to be deleted
  std::this_thread::sleep_for(timespan);

  assert(test_get(skv_1, "user_200", nullopt));
  assert(test_get(skv_1, "post_202", nullopt));
  assert(test_get(skv_2, "post_404", nullopt));
}
