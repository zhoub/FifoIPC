cmake_minimum_required(VERSION 3.0.0)

project(FifoIPC)

add_executable(RX RX.cpp)
add_executable(TX TX.cpp)

install(TARGETS RX TX DESTINATION bin)
