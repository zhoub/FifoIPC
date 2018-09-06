project(FifoIPC)

add_executable(RX RX.cpp)
add_executable(TX TX.cpp)

install(TARGETS RX TX DESTINATION bin)
