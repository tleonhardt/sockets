C code for UNIX domain datagram sockets
=======================================
This directory contains pure C code for working with UNIX domain datagram sockets
on a POSIX operating system.

**unix_datagram_server** is the server
**unix_datagram_client** is the client

In general, datagram sockets are unreliable.  However for UNIX domain datagram
sockets, the datagram transmission is carried out within the kernel and is reliable.
Moreover, all messages are delivered in order and are unduplicated.

On Linux, we can send quite large datagrams, but portable POSIX implementations should
impose a maximum datagram size, such as 2048 bytes.


UNIX datagram server
------------------
The server runs in an infinite loop waiting for clients to connect.

When a client connects, it reads incoming data on the bound socket,
converts it to uppercase, and sends it back to the client.

You need to kill it manually by sending it a signal.


UNIX datagram client
------------------
The client loops, sending each of its command-line arguments as a separate
message to the server.  After sending each message, the client reads the server
response and displays it on standard output.


Using the code
--------------
If the code has been build using **cmake** in directory *build*, the code can be
tested in the following manner:

    cd build
    ./unix_datagram_server &
    ./unix_datagram_client hello world
    kill %1f

The client should display two responses from the server:
Response 1:  HELLO
Response 2:  WORLD
