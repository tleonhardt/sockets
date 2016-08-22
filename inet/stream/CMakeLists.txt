project(ip4_stream_sockets)
cmake_minimum_required(VERSION 2.6)

set(TLPI_LIB_PATH "../../lib")

include_directories(${TLPI_LIB_PATH})

# Defining _XOPEN_SOURCE=600 enables POSIX.1:2001 extensions
# This matters in <netdb.h> and is needed for struct addrinfo to be defined.
add_definitions(-std=c99 -D_XOPEN_SOURCE=600)
set(CMAKE_C_FLAGS "${CMAXE_C_FLAGS} -Wall -Wextra -Wshadow")

set(common_source read_line.c ${TLPI_LIB_PATH}/error_functions.c ${TLPI_LIB_PATH}/get_num.c)
set(server_source ip4_stream_server.c ${common_source})
set(client_source ip4_stream_client.c ${common_source})

add_executable(ip4_stream_sever  ${server_source})
add_executable(ip4_stream_client ${client_source})
