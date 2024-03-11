cmake_minimum_required(VERSION 3.0)
project(ZeroMQExamples)

# Server executable
add_executable(server server.c)
target_link_libraries(server zmq)

# Client executable
add_executable(client client.c)
target_link_libraries(client zmq)
