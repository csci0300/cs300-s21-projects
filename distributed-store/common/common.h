#ifndef SHARDING_COMMON_H
#define SHARDING_COMMON_H

#include <string>
#include <vector>

/* ========================= */
/* ====== Definitions ====== */
/* ========================= */

// thresholds for shard splitting and merging
// ignore these unless you implement part 4
constexpr unsigned int HOT_THRESH = 100;
constexpr unsigned int COLD_THRESH = 10;

// range of keys -- be sure to use these as your bounds
// when sharding in any part of the project
constexpr unsigned int MIN_KEY = 0;
constexpr unsigned int MAX_KEY = 1000;

// a simple struct to represent a shard!
// lower should be always be <= higher
typedef struct shard {
  unsigned int lower;
  unsigned int upper;

  bool operator==(const shard& rhs) const {
    return lower == rhs.lower && upper == rhs.upper;
  }

} shard_t;

// An enum used to represent the overlap between two shards - returned by the
// get_overlap function
enum class OverlapStatus {
  NO_OVERLAP,
  OVERLAP_START,
  OVERLAP_END,
  COMPLETELY_CONTAINS,
  COMPLETELY_CONTAINED
};

/* ========================= */
/* === Helper functions ==== */
/* ========================= */

// Note: You do not have to use these unless you want to!

// sorts a vector of shards by their lower interval
void sortAscendingInterval(std::vector<shard_t>& shards);
// sorts a vector of shards into ascending/descending order of shard length
void sortAscendingSize(std::vector<shard_t>& shards);
void sortDescendingSize(std::vector<shard_t>& shards);

// gets the size of a shard
size_t size(const shard_t& s);
// gets the total size of a vector of shards
size_t shardRangeSize(const std::vector<shard_t>& vec);

// splits a shard evenly into two. if it can't be split evenly the first shard
// in the pair will get the extra key. will error if called on a size 1 shard.
std::pair<shard_t, shard_t> split_shard(const shard_t& s);

// returns the overlap status of a relative to b.
// Example: if OVERLAP_END is returned, it means that the end of a overlaps with
// b. The comments in each case show an example of what a pair of shards in that
// case would look like -- so check out the implementation in common.cc
// This function will be very useful in implementing your shardmaster's Move
// RPC!
OverlapStatus get_overlap(const shard_t& a, const shard_t& b);

// utility function for splitting strings on whitespace
std::vector<std::string> split(const std::string& s);

// like split, but for an arbitrary delimiter
std::vector<std::string> parse_value(std::string val, std::string delim);

//extracts the ID number out of the key
//you may find the utility helpful when implementing shardmaster
int extractID(std::string key);

#endif  // SHARDING_COMMON_H
