# cmake setup
install_cmake () {
    pushd ~ && wget https://github.com/Kitware/CMake/releases/download/v3.15.2/cmake-3.15.2.tar.gz && \
        tar -zxvf cmake-3.15.2.tar.gz && cd cmake-3.15.2 && ./bootstrap && make && sudo make install && popd
}

# go, for boringssl
install_go () {
    sudo apt-get install snapd
    sudo snap install go --classic
}

install_zlib () {
    sudo apt-get install zlib1g zlib1g-dev
}

install_openssl () {
    sudo apt install libssl-dev
}

# misc grpc dependencies
setup_grpc_deps () {
    sudo apt-get install build-essential autoconf libtool pkg-config
}

# some missing threads packages apparently
setup_threads () {
    sudo apt -y install libboost-tools-dev libboost-thread1.62-dev magics++
}

# helper to install c-ares
install_cares () {
    mkdir -p "third_party/cares/cares/cmake/build" && \
    pushd "third_party/cares/cares/cmake/build" && \
    cmake -DCMAKE_BUILD_TYPE=Release ../.. && \
    sudo make -j 2 install && \
    popd
}

# helper to install protobuf
install_protobuf () {
    mkdir -p "third_party/protobuf/cmake/build" && \
    pushd "third_party/protobuf/cmake/build" && \
    cmake -Dprotobuf_BUILD_TESTS=OFF -DCMAKE_BUILD_TYPE=Release .. && \
    sudo make -j 2 install && \
    popd
}

# actually installs grpc, along with protobuf and c-ares
install_grpc () {
    pushd ~ && cd grpc && \
        # this checkout is for the 1.26.0 stable release
        git checkout de893acb6aef88484a427e64b96727e4926fdcfd && git submodule update --init && \
        install_cares && install_protobuf && mkdir -p "cmake/build" && cd cmake/build && \
        # actually build grpc and install it now that all deps are set up
        cmake -DCMAKE_BUILD_TYPE=Release -DgRPC_INSTALL=ON -DgRPC_BUILD_TESTS=OFF -DgRPC_CARES_PROVIDER=package -DgRPC_PROTOBUF_PROVIDER=package \
              -DgRPC_SSL_PROVIDER=package -DgRPC_ZLIB_PROVIDER=package -DCMAKE_INSTALL_PREFIX=/usr/local/grpc ../.. && make -j 2 && sudo make install && popd
}

if test ! -e ${HOME}/grcp; then
    git clone https://github.com/grpc/grpc.git ${HOME}/grpc
fi

setup_grpc_deps && setup_threads && install_cmake && install_go && install_zlib && install_openssl && install_grpc

wget -O shardkv-clients.tar.gz "https://cs.brown.edu/courses/csci0300/2021/assign/projects/assets/shardkv-clients.tar.gz" && \
  tar -xzf shardkv-clients.tar.gz && rm shardkv-clients.tar.gz
