#include <grpcpp/grpcpp.h>

#include "shardkv.h"

/**
 * This method is analogous to a hashmap lookup. A key is supplied in the
 * request and if its value can be found, we should either set the appropriate
 * field in the response Otherwise, we should return an error. An error should
 * also be returned if the server is not responsible for the specified key
 *
 * @param context - you can ignore this
 * @param request a message containing a key
 * @param response we store the value for the specified key here
 * @return ::grpc::Status::OK on success, or
 * ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT, "<your error message
 * here>")
 */
::grpc::Status ShardkvServer::Get(::grpc::ServerContext* context,
                                  const ::GetRequest* request,
                                  ::GetResponse* response) {
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "not implemented");
}

/**
 * Insert the given key-value mapping into our store such that future gets will
 * retrieve it
 * If the item already exists, you must replace its previous value.
 * This function should error if the server is not responsible for the specified
 * key.
 *
 * @param context - you can ignore this
 * @param request A message containing a key-value pair
 * @param response An empty message, as we don't need to return any data
 * @return ::grpc::Status::OK on success, or
 * ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT, "<your error message
 * here>")
 */
::grpc::Status ShardkvServer::Put(::grpc::ServerContext* context,
                                  const ::PutRequest* request,
                                  Empty* response) {
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "not implemented");
}

/**
 * Appends the data in the request to whatever data the specified key maps to.
 * If the key is not mapped to anything, this method should be equivalent to a
 * put for the specified key and value. If the server is not responsible for the
 * specified key, this function should fail.
 *
 * @param context - you can ignore this
 * @param request A message containngi a key-value pair
 * @param response An empty message, as we don't need to return any data
 * @return ::grpc::Status::OK on success, or
 * ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT, "<your error message
 * here>"
 */
::grpc::Status ShardkvServer::Append(::grpc::ServerContext* context,
                                     const ::AppendRequest* request,
                                     Empty* response) {
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "not implemented");
}

/**
 * Deletes the key-value pair associated with this key from the server.
 * If this server does not contain the requested key, do nothing and return
 * the error specified
 *
 * @param context - you can ignore this
 * @param request A message containing the key to be removed
 * @param response An empty message, as we don't need to return any data
 * @return ::grpc::Status::OK on success, or
 * ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT, "<your error message
 * here>")
 */
::grpc::Status ShardkvServer::Delete(::grpc::ServerContext* context,
                                           const ::DeleteRequest* request,
                                           Empty* response) {
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "not implemented");
}

/**
 * This method is called in a separate thread on periodic intervals (see the
 * constructor in shardkv.h for how this is done). It should query the shardmaster
 * for an updated configuration of how shards are distributed. You should then
 * find this server in that configuration and look at the shards associated with
 * it. These are the shards that the shardmaster deems this server responsible
 * for. Check that every key you have stored on this server is one that the
 * server is actually responsible for according to the shardmaster. If this
 * server is no longer responsible for a key, you should find the server that
 * is, and call the Put RPC in order to transfer the key/value pair to that
 * server. You should not let the Put RPC fail. That is, the RPC should be
 * continually retried until success. After the put RPC succeeds, delete the
 * key/value pair from this server's storage. Think about concurrency issues like
 * potential deadlock as you write this function!
 *
 * @param stub a grpc stub for the shardmaster, which we use to invoke the Query
 * method!
 */
void ShardkvServer::QueryShardmaster(Shardmaster::Stub* stub) {
  Empty query;
  QueryResponse response;
  ::grpc::ClientContext cc;

  auto status = stub->Query(&cc, query, &response);
  if (status.ok()) {
    // TODO: figure out what to do here!
  } else {
    // TODO: here too!
  }
}
