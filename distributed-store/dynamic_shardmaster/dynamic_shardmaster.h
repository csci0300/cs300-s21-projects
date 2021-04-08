#ifndef SHARDING_DYNAMIC_SHARDMASTER_H
#define SHARDING_DYNAMIC_SHARDMASTER_H

#include <grpcpp/grpcpp.h>

#include "../build/shardmaster.grpc.pb.h"

using namespace std;

class ShardmasterServer final : public Shardmaster::Service {
  using Empty = google::protobuf::Empty;

 public:
  // TODO first, implement these four methods (should be fairly similar to your
  // static shardmaster)
  ::grpc::Status Join(::grpc::ServerContext* context,
                      const ::JoinRequest* request, Empty* response) override;
  ::grpc::Status Leave(::grpc::ServerContext* context,
                       const ::LeaveRequest* request, Empty* response) override;
  ::grpc::Status Move(::grpc::ServerContext* context,
                      const ::MoveRequest* request, Empty* response) override;
  ::grpc::Status Query(::grpc::ServerContext* context, const Empty* request,
                       ::QueryResponse* response) override;

  // TODO then think about how this method should be implemented! This is where
  // you should handle hot/cold shard splitting/merging.
  void Rebalance();

 private:
  // TODO add fields here!
};

#endif  // SHARDING_DYNAMIC_SHARDMASTER_H
