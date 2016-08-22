project(inet_stream_sockets)
cmake_minimum_required(VERSION 2.6)

set(TLPI_LIB_PATH "../../lib")

include_directories(${TLPI_LIB_PATH})

add_definitions(-std=c99)
set(CMAKE_C_FLAGS "${CMAXE_C_FLAGS} -Wall -Wextra -Wshadow")

set(common_source ${TLPI_LIB_PATH}/error_functions.c)
set(server_source inet_stream_server.c ${common_source})
set(client_source inet_stream_client.c ${common_source})

add_executable(inet_stream_sever  ${server_source})
add_executable(inet_stream_client ${client_source})
