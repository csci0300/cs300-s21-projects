#include "common.h"
#include <algorithm>
#include <cassert>
#include <regex>
#include <cstring>

void sortAscendingInterval(std::vector<shard_t>& shards) {
  std::sort(
      shards.begin(), shards.end(),
      [](const shard_t& a, const shard_t& b) { return a.lower < b.lower; });
}

size_t size(const shard_t& s) { return s.upper - s.lower + 1; }

std::pair<shard_t, shard_t> split_shard(const shard_t& s) {
  // can't get midpoint of size 1 shard
  assert(s.lower < s.upper);
  unsigned int midpoint = s.lower + ((s.upper - s.lower) / 2);
  return std::make_pair<shard_t, shard_t>({s.lower, midpoint},
                                          {midpoint + 1, s.upper});
}

void sortAscendingSize(std::vector<shard_t>& shards) {
  std::sort(
      shards.begin(), shards.end(),
      [](const shard_t& a, const shard_t& b) { return size(a) < size(b); });
}

void sortDescendingSize(std::vector<shard_t>& shards) {
  std::sort(
      shards.begin(), shards.end(),
      [](const shard_t& a, const shard_t& b) { return size(b) < size(a); });
}

size_t shardRangeSize(const std::vector<shard_t>& vec) {
  size_t tot = 0;
  for (const shard_t& s : vec) {
    tot += size(vec);
  }
  return tot;
}

OverlapStatus get_overlap(const shard_t& a, const shard_t& b) {
  if (a.upper < b.lower || b.upper < a.lower) {
    /**
     * A: [-----]
     * B:         [-----]
     */
    return OverlapStatus::NO_OVERLAP;
  } else if (b.lower <= a.lower && a.upper <= b.upper) {
    /**
     * A:    [----]
     * B:  [--------]
     * Note: This also includes the case where the two shards are equal!
     */
    return OverlapStatus::COMPLETELY_CONTAINED;
  } else if (a.lower < b.lower && a.upper > b.upper) {
    /**
     * A: [-------]
     * B:   [---]
     */
    return OverlapStatus::COMPLETELY_CONTAINS;
  } else if (a.lower >= b.lower && a.upper > b.upper) {
    /**
     * A:    [-----]
     * B: [----]
     */
    return OverlapStatus::OVERLAP_START;
  } else if (a.lower < b.lower && a.upper <= b.upper) {
    /**
     * A: [-------]
     * B:    [------]
     */
    return OverlapStatus::OVERLAP_END;
  } else {
    throw std::runtime_error("bad case in get_overlap\n");
  }
}

std::vector<std::string> split(const std::string& s) {
  std::vector<std::string> v;
  std::regex ws_re("\\s+");  // whitespace
  std::copy(std::sregex_token_iterator(s.begin(), s.end(), ws_re, -1),
            std::sregex_token_iterator(), std::back_inserter(v));
  return v;
}

std::vector<std::string> parse_value(std::string val, std::string delim) {
  std::vector<std::string> tokens;

  char *save;
  char *tok = strtok_r((char *)val.c_str(), delim.c_str(), &save);
  while(tok != NULL){
    tokens.push_back(std::string(tok));
    tok = strtok_r(NULL, delim.c_str(), &save);
  }

  return tokens;
}

int extractID(std::string key){
  std::vector<std::string> tokens;

  char *save;
  char *tok = strtok_r((char *)key.c_str(), "_", &save);
  while(tok != NULL){
    tokens.push_back(std::string(tok));
    tok = strtok_r(NULL, "_", &save);
  }
  
  assert(tokens.size() > 1); //illformed key

  return stoi(tokens[1]);
}
