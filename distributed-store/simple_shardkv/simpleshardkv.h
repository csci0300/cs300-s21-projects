//
// Created by raghu on 12/19/19.
//

#ifndef SHARDING_SIMPLESHARDKV_H
#define SHARDING_SIMPLESHARDKV_H

#include <grpcpp/grpcpp.h>
#include "../build/shardkv.grpc.pb.h"
#include "../common/common.h"  // <-- check this for helper functions / data structures to use!

class SimpleShardkvServer : public Shardkv::Service {
  using Empty = google::protobuf::Empty;

 public:
  // TODO: implement these three methods in simpleshardkv.cc
  ::grpc::Status Get(::grpc::ServerContext* context,
                     const ::GetRequest* request,
                     ::GetResponse* response) override;
  ::grpc::Status Put(::grpc::ServerContext* context,
                     const ::PutRequest* request, Empty* response) override;
  ::grpc::Status Append(::grpc::ServerContext* context,
                        const ::AppendRequest* request,
                        Empty* response) override;
  ::grpc::Status Delete(::grpc::ServerContext* context,
                        const ::DeleteRequest* request,
                        Empty* response) override;

 private:
  // TODO: add any fields you need here (ie. a map from key to value, any
  // mutexes you want, etc.)
};

#endif  // SHARDING_SIMPLESHARDKV_H