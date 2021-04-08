#include "dynamic_shardmaster.h"

/**
 *
 * @param context - you can ignore this
 * @param request A message containing the address of a key-value server that's
 * joining
 * @param response An empty message, as we don't need to return any data
 * @return ::grpc::Status::OK on success, an appropriate error code otherwise
 */
::grpc::Status ShardmasterServer::Join(::grpc::ServerContext* context,
                                       const ::JoinRequest* request,
                                       Empty* response) {
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "not implemented");
}

/**
 *
 * @param context - you can ignore this
 * @param request A message containing a destination server address and the
 * lower/upper bounds of a shard we're putting on the destination server.
 * @param response An empty message, as we don't need to return any data
 * @return ::grpc::Status::OK on success, an appropriate error code otherwise
 */
::grpc::Status ShardmasterServer::Move(::grpc::ServerContext* context,
                                       const ::MoveRequest* request,
                                       Empty* response) {
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "not implemented");
}

/**
 *
 * @param context - you can ignore this
 * @param request A message containing a list of server addresses that are
 * leaving
 * @param response An empty message, as we don't need to return any data
 * @return ::grpc::Status::OK on success, an appropriate error code otherwise
 */
::grpc::Status ShardmasterServer::Leave(::grpc::ServerContext* context,
                                        const ::LeaveRequest* request,
                                        Empty* response) {
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "not implemented");
}

/**
 *
 * @param context - you can ignore this
 * @param request An empty message, as we don't need to send any data
 * @param response A message that specifies which shards are on which servers
 * @return ::grpc::Status::OK on success, an appropriate error code otherwise
 */
::grpc::Status ShardmasterServer::Query(::grpc::ServerContext* context,
                                        const Empty* request,
                                        ::QueryResponse* response) {
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "not implemented");
}

/**
 * This method will be called from another thread on a fixed interval - see
 * shardmaster/main.cc for how this is done. This is where you should handle
 * splitting of hot shards and merging of adjacent cold shards.
 */
void ShardmasterServer::Rebalance() {}
