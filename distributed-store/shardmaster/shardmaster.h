#ifndef SHARDING_SHARDMASTER_H
#define SHARDING_SHARDMASTER_H

#include "../common/common.h"

#include <grpcpp/grpcpp.h>
#include "../build/shardmaster.grpc.pb.h"

class StaticShardmaster : public Shardmaster::Service {
  using Empty = google::protobuf::Empty;

 public:
  // TODO implement these four methods!
  ::grpc::Status Join(::grpc::ServerContext* context,
                      const ::JoinRequest* request, Empty* response) override;
  ::grpc::Status Leave(::grpc::ServerContext* context,
                       const ::LeaveRequest* request, Empty* response) override;
  ::grpc::Status Move(::grpc::ServerContext* context,
                      const ::MoveRequest* request, Empty* response) override;
  ::grpc::Status Query(::grpc::ServerContext* context, const Empty* request,
                       ::QueryResponse* response) override;
  ::grpc::Status GDPRDelete(::grpc::ServerContext* context,
                        const ::GDPRDeleteRequest* request,
                        Empty* response) override;

 private:
  // TODO add any fields you want here!
  // Hint: think about what sort of data structures make sense for keeping track
  // of which servers have which shards, as well as what kind of locking you
  // will need to ensure thread safety.
};

#endif  // SHARDING_SHARDMASTER_H
