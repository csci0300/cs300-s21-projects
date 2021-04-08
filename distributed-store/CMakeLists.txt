# Copyright 2018 gRPC authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# cmake build file for C++ helloworld example.
# Assumes protobuf and gRPC have been installed using cmake.
# See cmake_externalproject/CMakeLists.txt for all-in-one cmake build
# that automatically builds all the dependencies before building helloworld.

cmake_minimum_required(VERSION 2.8)

project(Sharding C CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

# This branch assumes that gRPC and all its dependencies are already installed
# on this system, so they can be located by find_package().

# Find Protobuf installation
# Looks for protobuf-config.cmake file installed by Protobuf's cmake installation.
set(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using protobuf ${protobuf_VERSION}")

set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)

# Find gRPC installation
# Looks for gRPCConfig.cmake file installed by gRPC's cmake installation.
find_package(gRPC PATHS /usr/local/grpc NO_DEFAULT_PATH CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")

set(_GRPC_GRPCPP_UNSECURE gRPC::grpc++_unsecure)
set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)

# Proto files
get_filename_component(sm_proto "./protos/shardmaster.proto" ABSOLUTE)
get_filename_component(sm_proto_path "${sm_proto}" PATH)

get_filename_component(kv_proto "./protos/shardkv.proto" ABSOLUTE)
get_filename_component(kv_proto_path "${kv_proto}" PATH)

# Generated sources
set(sm_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/shardmaster.pb.cc")
set(sm_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/shardmaster.pb.h")
set(sm_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/shardmaster.grpc.pb.cc")
set(sm_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/shardmaster.grpc.pb.h")
add_custom_command(
      OUTPUT "${sm_proto_srcs}" "${sm_proto_hdrs}" "${sm_grpc_srcs}" "${sm_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${sm_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${sm_proto}"
      DEPENDS "${sm_proto}")

set(kv_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/shardkv.pb.cc")
set(kv_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/shardkv.pb.h")
set(kv_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/shardkv.grpc.pb.cc")
set(kv_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/shardkv.grpc.pb.h")
add_custom_command(
        OUTPUT "${kv_proto_srcs}" "${kv_proto_hdrs}" "${kv_grpc_srcs}" "${kv_grpc_hdrs}"
        COMMAND ${_PROTOBUF_PROTOC}
        ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${kv_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${kv_proto}"
        DEPENDS "${kv_proto}")

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

set("common_srcs" "common/common.cc")

add_executable(dynamic_shardmaster "dynamic_shardmaster/main.cc"
  "dynamic_shardmaster/dynamic_shardmaster.cc"
  ${common_srcs}
  ${sm_proto_srcs}
  ${sm_grpc_srcs})
target_link_libraries(dynamic_shardmaster
  ${_GRPC_GRPCPP_UNSECURE}
  ${_PROTOBUF_LIBPROTOBUF})

# all tests
file( GLOB TEST_SOURCES tests/*/*.cc )
foreach( testsourcefile ${TEST_SOURCES} )
    get_filename_component(testname ${testsourcefile} NAME_WE)
    add_executable( ${testname} ${testsourcefile}
            ${common_srcs}
            ${sm_proto_srcs}
            ${sm_grpc_srcs}
            ${kv_proto_srcs}
            ${kv_grpc_srcs}
            shardmaster/shardmaster.cc
            simple_shardkv/simpleshardkv.cc
            shardkv/shardkv.cc

            ${sm_grpc_srcs} test_utils/test_utils.cc test_utils/test_utils.h )

    target_link_libraries(${testname}
            ${_GRPC_GRPCPP_UNSECURE}
            ${_PROTOBUF_LIBPROTOBUF})
endforeach( testsourcefile ${TEST_SOURCES} )

add_executable(shardmaster "shardmaster/main.cc"
        "shardmaster/shardmaster.cc"
        ${common_srcs}
        ${sm_proto_srcs}
        ${sm_grpc_srcs})
target_link_libraries(shardmaster
        ${_GRPC_GRPCPP_UNSECURE}
        ${_PROTOBUF_LIBPROTOBUF})

add_executable(simple_shardkv "simple_shardkv/main.cc"
        "simple_shardkv/simpleshardkv.cc"
        ${common_srcs}
        ${kv_proto_srcs}
        ${kv_grpc_srcs})
target_link_libraries(simple_shardkv
        ${_GRPC_GRPCPP_UNSECURE}
        ${_PROTOBUF_LIBPROTOBUF})

add_executable(shardkv "shardkv/main.cc"
        "shardkv/shardkv.cc"
        ${common_srcs}
        ${kv_proto_srcs}
        ${kv_grpc_srcs}
        ${sm_proto_srcs}
        ${sm_grpc_srcs})
target_link_libraries(shardkv
        ${_GRPC_GRPCPP_UNSECURE}
        ${_PROTOBUF_LIBPROTOBUF})

add_custom_target(check ./test.sh)
